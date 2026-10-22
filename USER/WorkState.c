#include "WorkState.h"
#include "Table.h"
#include "Memory.h"
#include "MassageLogic.h"
#include "Heat.h"

/* Timeout constants (milliseconds for TICK_COUNTER) */
#define TIMEOUT_1S 1000
#define TIMEOUT_2S 2000
#define TIMEOUT_3S 3000
#define TIMEOUT_5S 5000
#define TIMEOUT_10S 10000
#define TIMEOUT_30S 30000
#define TIMEOUT_60S 60000
#define TIMEOUT_120S 120000
#define TIMEOUT_20MIN 1200000

WorkState_S Work;

// extern uint8_t Core_RestSuccess;
static u8 ResetStep = 0;

bool Shoulder_Update = 0;
bool AutoLyingSkip_Flag = 0;
static bool Manual_Air_Flag = 0; // @ 打延时补丁3
static uint32_t Manual_Air_tick = 0;

static u32 DriveCmd_Manual_Tick = 0; // * 手动定时重发 250806

void WorkStateInit(void)
{
	memset(&Work, 0, sizeof(WorkState_S));
}

void WorkStateSet(u8 sta)
{
	Work.CurState = sta;
}

u8 WorkStateGet(void)
{
	return Work.CurState;
}

void WorkStateProcess(void)
{
	u8 i;
	u16 MoveUpPos;
	uint16_t CurCircle;
	static uint8_t CurUserIntensity = 0;

	switch (Work.CurState)
	{
	case WorkState_Wait:
		break;

	case WorkState_TurnOn:
		if (Massage_Flag.Reset && !TICK_COUNTER(Work.Tick, TIMEOUT_60S)) // * 增加复位超时 251212
		// if (Massage_Flag.Reset && !TICK_COUNTER(Work.Tick, 6000)) // @ 调试使用 260813
		{
			if (AtTabGet(Table_Auto)->PartOverMark == EXEC_TurnOn) // @ add 260129 加快异常开机速度
			{
				AtTabSet_PartOverMark(Table_Auto, 0);
				Massage_Flag.Reset = 0;
				MassMode_Load(FA_AUTO_CLOSE);
			}
			else if (AtTabGet(Table_Auto)->PartOverMark == EXEC_Reset)
			{
				AtTabSet_PartOverMark(Table_Auto, 0);
				Core_RestSuccess |= 0x03;
				Massage_Flag.Reset = 0;
				MassMode_Load(FA_AUTO_CLOSE);
			}
			else if (AtTabGet(Table_Auto)->CurAutoTab != A_Reset)
			{
				MassMode_Load(A_Reset);
			}
			return;
		}

#ifdef USE_S0555
		if (!TICK_COUNTER(Handle_Success_tick, TIMEOUT_5S))
			return;
#elif defined USE_S0551
		if (!TICK_COUNTER(Work.Tick, TIMEOUT_5S))
			return;
#endif
		ResetStep = 0;				  // * 增加 250617
		MChair.CFG.Bit.CoreReset = 0; // * 增加 250617
		Work.CurState = WorkState_WaitFunc;
		Work.VoicePlayFlag = 0;

#ifdef USE_M2
		if (DwTreadPlace != MassPlace_Bot) //* 开机复位 250416
		{
			Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
			Part_Date_Set(MoveDw_Knead, 1, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 6);
			Part_Date_Set(MoveDw_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 6);
		}
#endif
		break;

	case WorkState_AutoLying:

		if (PowerStateGet() != PowerReady)
		{
			TICK_RESET(Work.Tick);
			return;
		}
#ifdef A391S_32
		if (!Work.VoicePlayFlag && TICK_COUNTER(Work.Tick, TIMEOUT_2S))
		{
			Voice_Ack(VC_AutoSit); // @ 不能应答自动程序了 屏蔽 250414 recover 250926 rm 250930 add 260317
			Work.VoicePlayFlag = 1;
		}
#else
		if (!Work.VoicePlayFlag)
		{
			// Voice_Ack(VC_AutoSit);
			Work.VoicePlayFlag = 1;
		}
#endif
		MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
		MassWiseSet(MoveDw, TZ);
#endif

#ifdef AutoLying_Control_BackLeg // * 手机app 也需要躺倒时操作 250407
		if (Manual_BackLeg_Flag)
			TICK_RESET(Work.Tick);
		if ((DDG_Get_Compete && TICK_COUNTER(Work.Tick, TIMEOUT_3S)) || TICK_COUNTER(Work.Tick, TIMEOUT_60S) || AutoLyingSkip_Flag) // * 跳过 250407
#else
		if (DDG_Get_Compete || TICK_COUNTER(Work.Tick, TIMEOUT_60S))
#endif
		{
			AutoLyingSkip_Flag = 0;
			Massage_Flag.BodyCheck = 0; // * add 250903
			Work.CurState = WorkState_BodyCheck;
			Voice_Ack(VC_BodyCheck); // * 增加 250225

			i = MassModeGet(AutoModeGet())->_3DIntensity[1]; /* 选择程序之后手动调节力度模式，一开始默认模式2 */
			MassIntensitySet(i << 1);

			BodyPlaceCal(DEFAULT_SHOULDER);
			TICK_RESET(Work.Tick);
		}
		break;

	case WorkState_WaitFunc:
		UserIntensitySet(2); /* 默认强度2 */

		//===========================================================
		if (!Work.VoicePlayFlag)
		{
			// Voice_Ack(VC_AutoWait);
			Work.VoicePlayFlag = 1;
		}

		AutoLyingSkip_Flag = 0; // * 避免跳过自动躺倒 250414
		if (AutoModeGet() > 0)
		{
			Work.CurState = WorkState_AutoLying;
			TICK_RESET(Work.Tick); // * 增加 250414
			DDG_Set_Zero(MassModeGet(AutoModeGet())->Zero);
			// DDG_Set_Zero(ZERO3); // * 体型检测 251127
			Work.VoicePlayFlag = 0;
			// Buzzer = SOUND_ACK;
		}
		else if (KeyPush.WiseKeyState) /* 只要手动选择手法，哪怕是无手法，也会进行状态切换 */
		{
			KeyPush.WiseKeyState = 0;
			Work.CurState = WorkState_AutoLying; //? 手动关机、开机直接进入这里 250304
			TICK_RESET(Work.Tick);				 // * 增加 250414
			Work.VoicePlayFlag = 0;
			// Buzzer = SOUND_ACK;
		}
		else if (AirModeGet() > 0)
		{
			Work.CurState = WorkState_Work;
			TICK_RESET(Manual_Air_tick); // * 开机手动模式补丁 250409
			Work.VoicePlayFlag = 0;
		}
		else if (Leg_Protect_Flag == 0) // * 没有小腿防夹 250410
		{
			if (TICK_COUNTER(Work.Tick, TIMEOUT_20MIN)) // 暂停超过20min 关机
				UserFunc_Set(UserFunc_PowerOff, 1);
		}
		break;

	case WorkState_BodyCheck:
		if (Massage_Flag.BodyCheck == 0)
		{
			Massage_Flag.BodyCheck = 1;
			MassMode_Load(A_BodyCheck);
			break;
		}

		if (AtTabGet(Table_Auto)->PartOverMark != EXEC_BodyCheck && !TICK_COUNTER(Work.Tick, TIMEOUT_120S)) // * 60S-120S 251106
			break;

		AtTabSet_PartOverMark(Table_Auto, 0);
		Massage_Flag.BodyCheck = 0;
		MassMode_Load(FA_AUTO_CLOSE);
		AT_TableClear(Table_Auto);

		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, 3);
		Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, 6, 0, 3); //* 10->6
		Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);

		CurCircle = MoveCurTreadCircleGet(MoveUp);
		BodyPlaceCal(CurCircle);
		Work.CurState = WorkState_ShoulderAdj;
		Voice_Ack(VC_ShoulderAdj);
		Work.VoicePlayFlag = 0;
		Shoulder_Update = 1;
		TICK_RESET(Work.Tick);
		break;

	case WorkState_ShoulderAdj:
		if (!Work.VoicePlayFlag)
		{
			Voice_Ack(VC_ShoulderAdj);
			Buzzer = SOUND_SHOULDER; // * 提示音 250516
			Work.VoicePlayFlag = 1;
		}
		if (Buzzer != SOUND_SHOULDER && Buzzer_Ararm == 0) // * add 251119
			Buzzer = SOUND_SHOULDER;
		if (TICK_COUNTER(Work.Tick, TIMEOUT_10S)) // 10s后进入按摩
		{
			Buzzer = SOUND_NULL;
			Work.CurState = WorkState_Work;
			TICK_RESET(Manual_Air_tick); // * 开机手动模式补丁 250409
			Voice_Ack(VC_MassStart);	 // * 正常开始按摩
		}
		if (Shoulder_Update)
		{
			Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, 6);
			Part_Date_Set(MoveUp_Tread, 1, TREAD_FIX_CIRCLE, BodyArea[LB_2] & 0xff, BodyArea[LB_2] >> 8, 8);
			Shoulder_Update = 0;
			TICK_RESET(Work.Tick);
		}
		break;

	case WorkState_Work:
		MassageUpdate();
		if (Shoulder_Update)
		{
			Shoulder_Update = 0;
		}

		if (0 == AutoModeGet())
		{
			DDGVariable.DDGAutoControl = 0;
			// * 手动模式 开启气压打补丁 250304
			if (Manual_Air_Flag)
			{
				if (TICK_COUNTER(Manual_Air_tick, TIMEOUT_1S)) // * 延时1秒再开启
				{
					Manual_Air_Flag = 0;
					AirModeSet(AirModeFull);
				}
			}
			if (Manual_Save_Flag) // * 打补丁 250323
			{
				if (TICK_COUNTER(Manual_Save_tick, TIMEOUT_1S)) // * 延时1秒再开启
				{
					Manual_Save_Flag = 0;
					AirModeSet(Manual_Save_AirState);
					AirIntensitySet(Manual_Save_AirIntensity);
				}
			}
			if (TICK_COUNTER(DriveCmd_Manual_Tick, TIMEOUT_5S)) // * 5s 重发补丁 250806
			{
				TICK_RESET(DriveCmd_Manual_Tick);
				// todo 待完善 250925
			}
		}

		break;
	case WorkState_Pause:
		if (TICK_COUNTER(Work.Tick, TIMEOUT_20MIN) && Leg_Protect_Flag == 0) // 暂停超过20min 关机
		{
			UserFunc_Set(UserFunc_PowerOff, 1);
		}
		break;
	case WorkState_Reset:

		if (!Work.VoicePlayFlag)
		{
			Work.VoicePlayFlag = 1;
			// Voice_Ack(VC_Reset);
		}

		switch (ResetStep)
		{
		case 0:
#if (LEG_COM)
#ifdef USE_FOOT_ROCK
			if ((GetLegDataRx()->Flex.Place == 2 && LegRockIsAtPosition(ROCKPOS_N2)) ||
				TICK_COUNTER(Work.Tick, TIMEOUT_60S))
#else
			if (GetLegDataRx()->Flex.Place == 2 || TICK_COUNTER(Work.Tick, TIMEOUT_60S))
#endif
			{
				FootFlexStateSet(FlexStop);
				// DDG_Set_SitUp();
				DDG_Set_BackUp(); // @ 先复位靠背 260421
				LegAutoAdj = 0;	  // * 复位不找脚 250410
				ResetStep = 1;
				TICK_RESET(Work.Tick);
			}
			else if (FootFlexStateGet() != FlexUp) // * 增加 250403
			{
				FootFlexStateSet(FlexUp);
				FootFlexSpeedSet(5);
			}
#else
			// DDG_Set_SitUp();
			DDG_Set_BackUp(); // @ 先复位靠背 260421
			ResetStep = 1;
			TICK_RESET(Work.Tick);
#endif
			break;

		case 1:
			if (DDG_Get_BnCurCircle <= (DDG_BCircle_Max / 3) || TICK_COUNTER(Work.Tick, TIMEOUT_30S)) // 电动杆、机芯无动作或者超时60s
			{
				DDG_Set_SitUp();
				ResetStep = 2;
			}
			break;

		case 2:
			if ((DDG_Get_Compete && (0x03 == Core_RestSuccess)) || TICK_COUNTER(Work.Tick, TIMEOUT_60S)) // 电动杆、机芯无动作或者超时60s
			{
				AT_TableStop(Table_Auto); //* add 251215
				Work.CurState = WorkState_Wait;
				Core_RestSuccess = 0;
				MChair.CFG.Bit.CoreReset = 0;
				ResetStep = 0;
#ifdef PART_USE_SUM_TIME
				App_Update_Cnt_Set(3); // @ run time 260927
#endif
			}
			break;
		}
		break;

	case WorkState_UserIntensityChose:

		if (!Work.VoicePlayFlag)
		{
			Work.VoicePlayFlag = 1;
			// Voice_Ack(VC_MassMode);
		}

		if (TICK_COUNTER(Work.Tick, TIMEOUT_20MIN) && Leg_Protect_Flag == 0) /* 在该状态下停留20Min则关机 */
		{
			UserFunc_Set(UserFunc_PowerOff, 1);
		}

		if (UserIntensityGet() > 0)
		{
			TICK_RESET(Work.Tick);
			Work.CurState = WorkState_AutoLying;
			Work.VoicePlayFlag = 0;
			DDG_Set_Zero(MassModeGet(AutoModeGet())->Zero);
		}
		else if (TICK_COUNTER(Work.Tick, TIMEOUT_10S)) // 10后默认2挡
		{
			UserIntensitySet(2);
			AutoModeSet(1);
			//  Buzzer = SOUND_ACK;
		}

		break;

	case WorkState_FCT:
		break;

	case WorkState_Warn:

		break;

	case WorkState_MassageFinish:
		if (TICK_COUNTER(Work.Tick, TIMEOUT_20MIN)) // * 结束提示 20 分钟不操作自动关机 261022
			UserFunc_Set(UserFunc_PowerOff, 1);
		break;

	default:
		break;
	}
}

static u8 nPause_Pt[TableNum];
static u8 Pause_State;
static u8 airmodecur, airintensitycur;
#if !(LEG_COM)
static u8 Roolmode_save, Roolstate_save, Rollspeed_save;
#endif
void Enter_Pause(void)
{
	if (Work.CurState != WorkState_Pause)
	{
		airintensitycur = AirIntensityGet(); // * 保存气压强度 250308
		nPause_Pt[Table_Auto] = AtTabGet(Table_Auto)->State;
		nPause_Pt[Table_MoveUp] = AtTabGet(Table_MoveUp)->State;
		nPause_Pt[Table_Sub] = AtTabGet(Table_Sub)->State;
#ifdef USE_M2
		nPause_Pt[Table_MoveDw] = AtTabGet(Table_MoveDw)->State;
		nPause_Pt[Table_Sub_MoveDw] = AtTabGet(Table_Sub_MoveDw)->State;
#endif
		nPause_Pt[Table_Air] = AtTabGet(Table_Air)->State;
		nPause_Pt[Table_Sub2] = AtTabGet(Table_Sub2)->State;
		nPause_Pt[Table_Manual_MoveUp] = MassWiseGet(MoveUp);
#ifdef USE_M2
		nPause_Pt[Table_Manual_MoveDw] = MassWiseGet(MoveDw);
#endif

#if !(LEG_COM) // * add 250830
		Roolmode_save = RollModeGet();
		Roolstate_save = RollStateGet();
		Rollspeed_save = RollSpeedGet();
		RollModeSet(0);
		RollStateSet(0);
		RollSpeedSet(0);
#endif

		AtTabGet(Table_Auto)->State = 0;
		AtTabGet(Table_MoveUp)->State = 0;
		AtTabGet(Table_Sub)->State = 0;
#ifdef USE_M2
		AtTabGet(Table_MoveDw)->State = 0;
		AtTabGet(Table_Sub_MoveDw)->State = 0;
#endif
		AtTabGet(Table_Air)->State = 0;
		AtTabGet(Table_Sub2)->State = 0;
		// AtTabGet(Table_Manual)->State = 0;
		MassWiseSet(MoveUp, TZ);
		Move_Flag[MoveUp].Area = 0xff; // todo add 260613
#ifdef USE_M2
		MassWiseSet(MoveDw, TZ);
#endif
		Pause_State = Work.CurState;
		Work.CurState = WorkState_Pause;
		TICK_RESET(Work.Tick);
	}
}

void Exit_Pause(void)
{
	if (Work.CurState != WorkState_Pause)
		return;
	Leg_Touch_Flag = 0;		// * 增加 250513
	Leg_Touch_Air_Flag = 0; // * 增加 250612
	Work.CurState = Pause_State;
	TICK_RESET(Work.Tick);
	if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
	{
		AtTabGet(Table_Auto)->State = nPause_Pt[Table_Auto];
		AtTabGet(Table_MoveUp)->State = nPause_Pt[Table_MoveUp];
		AtTabGet(Table_Sub)->State = nPause_Pt[Table_Sub];
#ifdef USE_M2
		AtTabGet(Table_MoveDw)->State = nPause_Pt[Table_MoveDw];
		AtTabGet(Table_Sub_MoveDw)->State = nPause_Pt[Table_Sub_MoveDw];
#endif
		AtTabGet(Table_Air)->State = nPause_Pt[Table_Air];
		AtTabGet(Table_Sub2)->State = nPause_Pt[Table_Sub2];
		MassWiseSet(MoveUp, (ManualWise_E)nPause_Pt[Table_Manual_MoveUp]);
#ifdef USE_M2
		MassWiseSet(MoveDw, (ManualWise_E)nPause_Pt[Table_Manual_MoveDw]);
#endif
		airmodecur = AirModeGet(); // * 气压表暂停 重新开始 250305
		AirIntensitySet(airintensitycur);
		AirModeSet(airmodecur);

#if !(LEG_COM) // * add 250830
		RollModeSet(Roolmode_save);
		RollStateSet(Roolstate_save);
		RollSpeedSet(Rollspeed_save);
#endif
	}
	else if (WorkStateGet() == WorkState_BodyCheck) // * 重新开始体型检测 250420
	{
		Massage_Flag.BodyCheck = 0; // * modify 250911
	}
	else if (WorkStateGet() == WorkState_ShoulderAdj) // * 重新调节肩位 250917
	{
		Shoulder_Update = 1;
	}
	else if (WorkStateGet() == WorkState_Reset) // * 需要修改 重新复位 250420
	{
		if (MChair.CFG.Bit.CoreReset)
		{
			ResetStep = 0;
			Core_RestSuccess = 0;
			Massage_Flag.Reset = 0; // * modify 250911
		}
	}
	else if (WorkStateGet() == WorkState_TurnOn) // * 增加 251211
	{
		MassMode_Load(A_Reset);
		Massage_Flag.Reset = 1;
	}
}

void UserFunc_Set(u8 ntype, u8 nparam)
{
	MoveUpState.ManualArea_ID = Area_Fixed;
	MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
	MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
	TICK_RESET(Work.Tick);
	switch (ntype)
	{
	case UserFunc_PowerOn:
#ifdef USE_FOOT_ROCK
		LegRockSwingSet(0);
#endif
		if (MChair.CFG.Bit.CoreReset)
		{
			Core_RestSuccess = 0;		  // * 增加 250516
			MChair.CFG.Bit.CoreReset = 0; // * 开机就当作已经复位 250123 删除 250221 恢复 250419 屏蔽测试 250421
			if (nparam)
			{
				Massage_Flag.Reset = 0; // * 跳过机芯复位 继续按摩 261022
			}
			else
			{
				if (AtTabGet(Table_Auto)->CurAutoTab == A_Reset)
					AtTabSet_ID(Table_Auto, 0); // * 复位清零，方便重新加载 251007
			}
		}

		MoveWhichSet(MoveUp);
		if (nparam)
			WorkStateSet(WorkState_WaitFunc); // * 继续按摩 直接进入功能等待
		else
			WorkStateSet(WorkState_TurnOn);
		DDG_Set_Stop();
		FootFlexStateSet(FlexStop);
		Work.CurMin = 20;
		Work.CurSec = 0;
		nPowerOn = 1;
		_5VC_ON(); // * 手控电源再次打开，防止手控电源不开bug 250416
		// Find_Shoulder_Resend = 0;
		Massage_Flag.BodyCheck = 0; // * add 250930
		KeyPush.WiseKeyState = 0;	// * 开关机都要把这个标志位清零 250304
		if (nLigntSw)
		{
			LED1_ON();
#ifdef LED2_PWM
			LED2_PWM_Flag = 1;
#else
			LED2_ON();
#endif
		}
#ifdef USE_REDLIGHT
		RedLight_PWM(RedLight_DutyToPwm(RedLight_Duty)); // todo test 261015
#else
		Heat3_On(RedLight_DutyToPwm(RedLight_Duty));
#endif
		break;

	case UserFunc_PowerOff:

		Memorystate &= 0x0f; // * 增加 取消调用显示 250617
#ifdef USE_FOOT_ROCK
		LegRockSwingSet(0);
#endif
#ifdef USE_SOUNDWAVE
		Flash_Parameter_read(); // todo 读取存储值 主要为了音波 260707
#endif
		Flash_Parameter_Write(); // @ 关机保存 250124

		MChair.CFG.All = 0;
		MChair.CFG.Bit.CoreReset = 1;
		ResetStep = 0;
		Core_RestSuccess = 0;
#if (LEG_COM)
		MChair.Leg.FootFinding = 0;
		LegAdj_Flex_Flag = 0;
#endif
		DDGVariable.DDGAutoControl = 0;
#ifdef AutoLying_Control_BackLeg
		Manual_BackLeg_Flag = 0; // * 增加 250402
#endif
		BlueState_Play = 0;
		Blue_Play_ID1 = 0;
		Blue_Play_ID2 = 0;

		HeatStateSet(BackHeat, 0, 0);
		HeatStateSet(LegHeat, 0, 0);

		AirInit();		   // * add 251010
		Motor_Work_Iint(); // * add 251010

		RollSpeedSet(0);
		RollModeSet(0);
		RollStateSet(0);

		KneadSpeedSet(0);
		KneadModeSet(0);
		KneadStateSet(0);

		MassageInit();
		AutoModeSet(0);
		MassMode_Load(FA_AUTO_CLOSE);

		nLCState = 0; //* 关闭老化
		nPowerOn = 0;
		KeyPush.WiseKeyState = 0; // @ 开关机都要把这个标志位清零 250304
		LED1_OFF();
#ifdef LED2_PWM
		LED2_PWM_Flag = 0;
#else
		LED2_OFF();
#endif

#ifdef USE_REDLIGHT
		RedLight_Off(); // todo test 261015
#else
		Heat3_Off(); // todo test 261015
#endif

#if (LEG_COM)
		MChair.Leg.FootFinding = 0;
		LegAdj_Flex_Flag = 0;
#endif
		Work.CurMin = 0;
		Work.CurSec = 0;
		AirModeSet(AirModeClose);
		memset(&AirT_Work, 0, sizeof(AirT_Work_S));
		memset(&MotorT_Work, 0, sizeof(MotorT_Work_S)); // todo add 260702

		Health_Set_State(Health_State_Stop);
		if (nparam == 0)
		{
			DDG_Set_Stop();
// todo add 翻转复位 和小腿伸缩一起 261015
#ifdef USE_FOOT_ROCK
			LegRockMoveToPosition(ROCKPOS_N2, 5);
#endif
#if (LEG_COM)
			FootFlexStateSet(FlexUp); // 先缩小腿
			FootFlexSpeedSet(5);
#endif
			Work.CurState = WorkState_Reset;
			Work.VoicePlayFlag = 0;
			ResetStep = 0;
		}
		else if (nparam == 1)
		{
			DDG_Set_Stop();
			Work.CurState = WorkState_Reset;
			ResetStep = 2; // @ 1-2 修改 260421
		}
		else if (nparam == 2)
		{
#ifdef USE_FOOT_ROCK
			LegRockMoveToPosition(ROCKPOS_N2, 5);
#endif
			DDG_Set_Stop();
			TICK_RESET(Work.Tick);
			Work.CurState = WorkState_MassageFinish;
		}
		break;

	case UserFunc_AutoMode:

		break;

	case UserFunc_ManualMode:

		BlueState_Play = 0; // * 关闭背景音乐 250320
		Blue_Play_ID1 = 0;
		Blue_Play_ID2 = 0;

		MoveUpState.ManualArea_ID = Area_Fixed;
		MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
		AT_TableStop(Table_Auto);
		AutoModeSet(0);

		AirModeSet(AirModeClose);
		memset(&AirT_Work, 0, sizeof(AirT_Work_S));

		if (nparam == 0)
		{
			Manual_Air_Flag = 1; // @ 打延时补丁
			TICK_RESET(Manual_Air_tick);
		}
		else
		{
			LED1_OFF(); // * 空闲状态熄灯
#ifdef USE_REDLIGHT
			RedLight_Off(); // todo test 261015
#else
			Heat3_Off(); // todo test 261015
#endif
#ifdef LED2_PWM
			LED2_PWM_Flag = 0;
#else
			LED2_OFF();
#endif

			WorkMinSet(20);
			WorkSecSet(0);

			HeatStateSet(BackHeat, 0, 0);
			HeatStateSet(LegHeat, 0, 0);

			RollSpeedSet(0);
			RollModeSet(0);
			RollStateSet(0);

			KneadSpeedSet(0);
			KneadModeSet(0);
			KneadStateSet(0);

			nBackIntensity = 0;
			MassIntensitySet(0);
			MassWiseSet(MoveUp, TZ);
			MassWidthSet(MoveUp, 0);
			MassAreaSet(MoveUp, Area_Fixed);
			MassSpeedSet(MoveUp, 3);
			MassArea_PartParamSet(MoveUp, BodyArea[LB_2]);

#ifdef USE_M2
#ifdef USE_M2_4D
			nBackIntensity_M2 = 0; // todo add 260109
#endif
			MassWiseSet(MoveDw, TZ);
			MassWidthSet(MoveDw, 0);
			MassAreaSet(MoveDw, Area_Fixed);
			MassSpeedSet(MoveDw, 3);
			MassArea_PartParamSet(MoveDw, 0);
#endif
		}
		break;

	default:
		break;
	}
}

u8 WorkMinGet(void)
{
	return Work.CurMin;
}

u8 WorkSecGet(void)
{
	return Work.CurSec;
}

void WorkMinSet(u8 min)
{
	Work.CurMin = min;
}

void WorkSecSet(u8 sec)
{
	Work.CurSec = sec;
}

u32 WorkTickGet(void)
{
	return Work.Tick;
}

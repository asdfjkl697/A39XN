#include "MassageInit.h"
#include "Table.h"
#include "MoveBoth.h"
#include "Memory.h"

#ifdef Debug_Mode
MassageInfo_S *MassP;
AutoModeInfo_S Auto;
#else
static MassageInfo_S *MassP;
static AutoModeInfo_S Auto;
#endif

ManualModeInfo_S Manual[2];

void MassageInit(void)
{
	memset(&Auto, 0, sizeof(AutoModeInfo_S));
	memset(Manual, 0, sizeof(ManualModeInfo_S) * 2);
	MSGMoveInit();
	MoveUp_Updata_Set(0x0f);
#ifdef USE_M2
	MoveDw_Updata_Set(0x0f);
#endif
}

// 按摩程序初始状态设置
static MassageInfo_S MassMode[] =
	{
		// 零重力          气压模式      气压档位        3D档位     滚轮 揉搓 加热 音乐 负氧
		//        {VALI,  VALI, {VALI, VALI, VALI}, {VALI, VALI, VALI}, VALI, VALI, VALI, VALI, VALI},
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, VALI}, // 0-手动模式

#ifdef VIETNAM_VOICE
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 1-零压舒展
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 14-白领专属
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 5-泰式拉筋
		{AUTO_ZERO_POS3, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, UpperHeat, P0, P0}, // 9-腰部护理
		{AUTO_ZERO_POS3, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 7-肩颈护理
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 16-元气唤醒
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 2-中式推拿
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 3-疲劳恢复
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 6-舒筋活络
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 13-女王纤体
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 20-安神助眠
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 11-天伦养生
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 19-运动辅助
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 17-午间小憩
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 15-御宅充电
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 4-海浪轻揉
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 8-提臀美腿
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 12-总裁释压
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P1, P2, P3}, P1, P1, LowerHeat, P0, P0}, // 10-腿足舒缓
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, FullHeat, P0, P0},  // 18-温热祛湿
#else
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 1-零压舒展
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 2-中式推拿
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P1, P0},  // 3-疲劳恢复
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 4-海浪轻揉
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 5-泰式拉筋
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 6-舒筋活络
		{AUTO_ZERO_POS3, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 7-肩颈护理
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 8-提臀美腿
		{AUTO_ZERO_POS3, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, UpperHeat, P0, P0}, // 9-腰部护理
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P1, P2, P3}, P1, P1, LowerHeat, P0, P0}, // 10-腿足舒缓
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 11-天伦养生
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 12-总裁释压
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 13-女王纤体
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 14-白领专属
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 15-御宅充电
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 16-元气唤醒
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 17-午间小憩
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, FullHeat, P0, P0},  // 18-温热祛湿
		{AUTO_ZERO_POS2, AirModeFull, {P3, P4, P5}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0},  // 19-运动辅助
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0},  // 20-安神助眠
#endif
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0}, // 21
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0}, // 22
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0}, // 23
		{AUTO_ZERO_POS2, AirModeFull, {P1, P2, P3}, {P0, P1, P2}, P1, P1, NullHeat, P0, P0}, // 24
		{AUTO_ZERO_POS2, AirModeFull, {P2, P3, P4}, {P1, P2, P3}, P1, P1, NullHeat, P0, P0}, // 25

		{AUTO_ZERO_POS2, AirModeFull, {P5, P5, P5}, {P1, P3, P5}, P3, P1, NullHeat, P0, P0}, // 26 AT_LC,

		{VALI, AirModeFull, {P1, P3, P5}, {P1, P3, P5}, P0, P1, NullHeat, P0, P0}, // 27 Package

		{AUTO_ZERO_POS0, AirModeClose, {P1, P3, P5}, {P1, P3, P5}, P0, P1, NullHeat, P0, P0}, // 28 A_Reset

		// {AUTO_ZERO_POS2, AirModeFull, {P1, P3, P5}, {P1, P3, P5}, P3, P1, NullHeat, P0, P0}, // 23 AT_Shoulder,

		// {AUTO_ZERO_POS2, AirModeFull, {P1, P3, P5}, {P1, P3, P5}, P3, P1, NullHeat, P0, P0}, // 24 Aging
};

// 按摩模式数据更新
void MassageUpdate(void)
{

	u8 temp, temp1;

	if (Auto.CurMode == Auto.PreMode) // * 手动模式排除 250410
		return;

	memset(&MotorT_Work, 0, sizeof(MotorT_Work_S));		 // todo add 260702
	memset(&AirT_Work, 0, sizeof(AirT_Work_S));			 // * add 250930
	memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T)); // * 初始化跟随 250930
	memset(&ASSIGN_Array, 0, ASSIGNum);					 // * 赋值清零 251219

	if (0 == Auto.PreMode)
	{
		memset(Manual, 0, sizeof(ManualModeInfo_S) * 2);
		MassWiseSet(MoveUp, 0xff); // * add 250911
#ifdef USE_M2
		MassWiseSet(MoveDw, 0xff);
#endif
	}

	if (Auto.CurMode > 0)
	{
		AT_TableStop(Table_Auto); // * add 250928
		AT_TableStop(Table_Air);  // * add 250928
		AT_TableStop(Table_Sub);  // * add 250920
		AT_TableStop(Table_Sub2); // todo add 260616
#ifdef USE_M2
		AT_TableStop(Table_MoveDw);
#endif
		AtTabSet_ReadTBFlag(Table_Auto, 1); // * md 250919
	}

	MoveWhichSet(MoveUp); // * 增加 250521
	/* 切换自动程序时复位手动操作标志位 */
	KeyPush.FootRollKeyState = 0;
	KeyPush.KeyCalfKnead = 0;
	KeyPush.KeyBackHeat = 0;
	KeyPush.KeyLegHeat = 0;
	/*--------------------------------*/
	// * 如果小腿在伸缩 触发停止 250528
#if (LEG_COM)
	if (FootFlexStateGet() == FlexDown || FootFlexStateGet() == FlexUp)
	{
		FootFlexStateSet(FlexStop);
	}
#endif

	Auto.PreMode = Auto.CurMode;
	MassP = MassMode + Auto.PreMode; // 指针默认赋值数组首地址+移位

	if (LoadSave_Flag == 0) // * 调用收藏 不需要读表
	{
		WorkMinSet(20);
		WorkSecSet(0);
#ifdef A39X_LC
		if (nLCState)
		{
			WorkMinSet(76); // * 75分钟循环 250423
			WorkSecSet(0);
		}
#endif

#ifdef USE_FOOT_ROCK
		if (FootSwing_Level == 0) // todo add 261024
			LegRockMoveToPosition(ROCKPOS_N2, 5);
#endif
		// 零重力更新
		temp = MassP->Zero;
		if (temp < Max_Zero)
		{
			DDG_Set_Zero(temp);
#if (LEG_COM)
			LegAutoAdj = 0; // * 自动程序切换不找脚 251127
#endif						// LEG_COM
		}

		// 自动强度（气压、3D）更新
		temp = Auto.Intensity;
		if (temp > MassInten_0 && temp <= MassInten_3)
		{
			// MassIntensitySet(MassP->_3DIntensity[temp - 1]);
			nBackIntensity = MassModeGet(AutoModeGet())->_3DIntensity[temp - 1]; // * 根据强度 设置3D 档位
#ifdef USE_M2_4D
			nBackIntensity_M2 = MassModeGet(AutoModeGet())->_3DIntensity[temp - 1]; // todo add 260109
#endif
			AirIntensitySet(MassP->AirIntensity[temp - 1]);
		}

		// 滚轮更新
		temp = MassP->Roll;
		if (temp)
		{
			RollSpeedSet(temp);
		}

		// 揉搓更新
		temp = MassP->LegKnead;
		if (temp)
		{
			KneadSpeedSet(temp);
		}

		// 加热更新
		temp = MassP->Heat;
		switch (temp)
		{
		default:
			break;
		case NullHeat:
			HeatStateSet(BackHeat, 0, 0);
			HeatStateSet(LegHeat, 0, 0);
			break;

		case UpperHeat:
			HeatStateSet(BackHeat, 1, 3);
			HeatStateSet(LegHeat, 0, 0);
			break;

		case LowerHeat:
			HeatStateSet(BackHeat, 0, 0);
			HeatStateSet(LegHeat, 1, 3);
			break;

		case FullHeat:
			HeatStateSet(BackHeat, 1, 3);
			HeatStateSet(LegHeat, 1, 3);
			break;
		}

#ifdef A39X_LC
		if (nLCState)
		{
			HeatStateSet(BackHeat, 1, 3);
			HeatStateSet(LegHeat, 1, 3);
		}
#endif
	}
	else
	{
		AirModeSet(Manual_Save_AirState);
		AirIntensitySet(Manual_Save_AirIntensity);
		LoadSave_Flag = 0;
	}

#ifdef USE_SOUNDWAVE
	Flash_Parameter_read(); // todo 读取存储值 主要为了音波 260707
#endif
	BlueState_Play_Deal(); // @ change 260326

	// 负氧离子
	temp = MassP->NOI;
	if (temp != VALI)
	{
		if (temp)
		{
			NOI_ON();
			FS_ON();
		}
		else
		{
			NOI_OFF();
			FS_OFF();
		}
	}

	// 记载需要运行的自动程序表格
	MassMode_Load(Auto.PreMode);
}

u8 AutoModeGet(void)
{
	return Auto.CurMode;
}

void AutoModeSet(u8 mode)
{
	Auto.CurMode = mode;
}

void AutoPreModeSet(u8 mode)
{

	Auto.PreMode = mode;
}

u8 UserIntensityGet(void)
{
	return Auto.Intensity;
}

void UserIntensitySet(u8 inten)
{
	Auto.Intensity = inten;
}

u8 MassIntensityGet(void)
{
	return Manual[0].MassIntensity;
}

void MassIntensitySet(u8 inten)
{
	Manual[MoveUp].MassIntensity = inten;
	Move_Flag[MoveUp].Inten = 1; //* add 250925
}
#ifdef USE_M2_4D
void MassIntensitySet_M2(u8 inten) // todo add 260108
{
	Manual[MoveDw].MassIntensity = inten;
	Move_Flag[MoveDw].Inten = 1; //* add 250925
}
#endif
u8 MassWiseGet(u8 whi)
{
	return Manual[whi].MassWise;
}

void MassWiseSet(u8 whi, ManualWise_E wise)
{
	Manual[whi].MassWise = wise;
	Move_Flag[whi].Wise = 0xff; // * add 250925
}

u8 MassSpeedGet(u8 whi)
{
	return Manual[whi].MassSpeed;
}

void MassSpeedSet(u8 whi, u8 spd)
{
	Manual[whi].MassSpeed = spd;
	Move_Flag[whi].Speed = 1; //* add 250925
}

u8 MassWidthGet(u8 whi)
{
	return Manual[whi].MassWidth;
}

void MassWidthSet(u8 whi, u8 width)
{
	Manual[whi].MassWidth = width;
	Move_Flag[whi].Width = 1; //* add 250925
}

void MassAreaSet(u8 whi, MassageArea_E area)
{
	Manual[whi].MassArea = area;
	Move_Flag[whi].Area = 0xff; // * add 250925
}

uint8_t MassAreaGet(u8 whi)
{
	return Manual[whi].MassArea;
}

void MassArea_PartParamSet(u8 whi, u16 para)
{
	Manual[whi].MassParam = para;
}

u16 MassArea_PartParamGet(u8 whi)
{
	return Manual[whi].MassParam;
}

MassageInfo_S *MassModeGet(u8 param)
{
	return &MassMode[param];
}

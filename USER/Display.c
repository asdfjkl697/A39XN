/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "Display.h"
#include "_485Com.h"
#include "Air.h"
#include "Memory.h" //* 增加
#include "System.h"
#include "Table.h"
#include "MassageLogic.h"
/*
 * 类型定义 *************************************************************************************************************************************
 */
Display_t Display;

/*
 * 变量定义 *************************************************************************************************************************************
 */
u8 Buzzer = SOUND_NULL;
u8 Buzzer_Ararm = 0;		  // * 急促蜂鸣器
u32 Buzzer_Shoulder_Tick = 0; // * 补丁长按按键音补丁
u8 nAllEor = 0;

//----------------------------错误代码显示------------------------------
#define Max_Eor_Group 50

u8 Eor_Show_Data[Max_Eor_Group];
u8 Eor_Show_Cnt = 0;
u8 Memorystate = 0x0; // * 增加
u8 Dispaly_Language = 1;

u8 Handle_Dispaly_Step = 0;
u32 Handle_Dispaly_tick = 0;
#if defined(USE_S0555) || defined(USE_S0551)
u32 Handle_Success_tick = 0; // @ add 260128
#endif
u8 DIY_packet[10];

#ifdef AutoLying_Control_BackLeg
bool Manual_BackLeg_Flag = 0; // * 手动靠背小腿调节 暂停时有用 250402
#endif

/*
 ************************************************************************************************************************************************
 * 函数定义 *************************************************************************************************************************************
 ************************************************************************************************************************************************
 */
/*****************************************************************************************************
 * 描述：错误报警更新
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void Eor_Update(void)
{
	u8 i, j;
	u8 error[4];
	j = 0;

	error[0] = GetMoveDataTx(MoveUp)->ErrorCode[0];
	if (error[0] & 0x06)
		Eor_Show_Data[j] = 0xA4; //* 行走电机过载 短路
	else if (error[0] & 0x08)
		Eor_Show_Data[j] = 0xA5; //* 行走电机断路
	else if (error[0] & 0x30)
		Eor_Show_Data[j] = 0xAE; //* 伸缩电机过载 短路
	else if (error[0] & 0x40)
		Eor_Show_Data[j] = 0xAD; //* 伸缩电机断路
	else if (error[0] & 0x80)
		Eor_Show_Data[j] = 0xA7; //* 揉捏电机过载 短路
	else
		Eor_Show_Data[j] = 0;
	j++;

	error[1] = GetMoveDataTx(MoveUp)->ErrorCode[1];
	if (error[1] == 0)
		Eor_Show_Data[j] = 0x00;
	else if (error[1] & 0x01)
		Eor_Show_Data[j] = 0xA7; //* 揉捏电机过载 短路
	else if ((error[1] & 0x02) && (WorkStateGet() == WorkState_Work))
		Eor_Show_Data[j] = 0xA8; // * 揉捏电机断路 未运行 未连接  打补丁 250415
	else if (error[1] & 0x0C)
		Eor_Show_Data[j] = 0xAC; //* 拍打电机过载 短路
	else if (error[1] & 0x10)
		Eor_Show_Data[j] = 0xAB; //* 拍打电机断路 未运行 未连接
	else if (error[1] & 0x20)
		Eor_Show_Data[j] = 0xA3; //* 行走限位故障
	else if (error[1] & 0x40)
		Eor_Show_Data[j] = 0xA2; //* 行走计数故障
	else if (error[1] & 0x80)
		Eor_Show_Data[j] = 0xA4; //* 行走驱动故障
	j++;

	error[2] = GetMoveDataTx(MoveUp)->ErrorCode[2];
	if (error[2] == 0)
		Eor_Show_Data[j] = 0x00;
	else if (error[2] & 0x01)
		Eor_Show_Data[j] = 0xAF; //* 伸缩电机限位故障
	else if (error[2] & 0x02)
		Eor_Show_Data[j] = 0xB0; //* 伸缩电机计数故障
	else if (error[2] & 0x04)
		Eor_Show_Data[j] = 0xAE; //* 伸缩电机过载 短路 驱动故障
	else if (error[2] & 0x08)
		Eor_Show_Data[j] = 0xA9; //* 揉捏电机原点检测不到
	else if (error[2] & 0x10)
		Eor_Show_Data[j] = 0xAA; //* 揉捏电机计数故障
	else if (error[2] & 0x20)
		Eor_Show_Data[j] = 0xA7; //* 揉捏电机过载 短路 驱动故障
	j++;

	// 靠背电动杆
	if (GetErrorDis(BackMotor_Err, OverCur))
		Eor_Show_Data[j] = 0xb4;
	else if (GetErrorDis(BackMotor_Err, ShortCur))
		Eor_Show_Data[j] = 0xb4;
	else if (GetErrorDis(BackMotor_Err, OpenCur)) // * 测试
		Eor_Show_Data[j] = 0xb5;
	else if (DDG_Get_BEor & DDG_Eor_Circle)
		Eor_Show_Data[j] = 0xb5;
	else
		Eor_Show_Data[j] = 0;
	j++;

	// 小腿电动杆
	if (GetErrorDis(LegMotor_Err, OverCur))
		Eor_Show_Data[j] = 0xb9;
	else if (GetErrorDis(LegMotor_Err, ShortCur))
		Eor_Show_Data[j] = 0xb9;
	else if (LMotorFaultFlag && (WorkStateGet() == WorkState_Work))
		Eor_Show_Data[j] = 0x93; // * 增加 250719
	else if (GetErrorDis(LegMotor_Err, OpenCur))
		Eor_Show_Data[j] = 0xba;
	else if (DDG_Get_LEor & DDG_Eor_Circle)
		Eor_Show_Data[j] = 0xba;
	else
		Eor_Show_Data[j] = 0;
	j++;

	// 靠背加热
	if (GetErrorDis(BackHeat_Err, OverCur))
		Eor_Show_Data[j] = 0xcc;
	else if (GetErrorDis(BackHeat_Err, ShortCur))
		Eor_Show_Data[j] = 0xcc;
	// else if (GetErrorDis(BackHeat_Err, OpenCur))
	// 	Eor_Show_Data[j] = 0xcd;
	else
		Eor_Show_Data[j] = 0;
	j++;

	// 气泵
	if (GetErrorDis(Pump_Err, OverCur))
		Eor_Show_Data[j] = 0xcb;
	else if (GetErrorDis(Pump_Err, ShortCur))
		Eor_Show_Data[j] = 0xcb;
	else if (GetErrorDis(Pump_Err, OpenCur))
		Eor_Show_Data[j] = 0xca;
	else
		Eor_Show_Data[j] = 0;
	j++;

	//* 小腿 250527
	error[0] = GetLegDataRx()->ErrorCode[0];
	if (error[0] & 0x03)
		Eor_Show_Data[j] = 0xEC; //* 小腿加热 过载、短路
	else if (error[0] & 0x04)
		Eor_Show_Data[j] = 0xED; //* 小腿加热 开路
	else if (error[0] & 0x18)
		Eor_Show_Data[j] = 0xC6; //* 小腿揉搓 过载、短路
	else if (error[0] & 0x20)
		Eor_Show_Data[j] = 0xC5; //* 小腿揉搓 开路
	else
		Eor_Show_Data[j] = 0;
	j++;

	error[1] = GetLegDataRx()->ErrorCode[1];
	if (error[1] & 0x06)
		Eor_Show_Data[j] = 0xC8; //* 足底滚轮 过载 短路
	else if (error[1] & 0x08)
		Eor_Show_Data[j] = 0xC7; //* 足底滚轮 开路
#ifdef LEG_COM
	else if (error[1] & 0x30)
		Eor_Show_Data[j] = 0xC1; //* 伸缩电机 过载 短路
	else if (error[1] & 0x40)
		Eor_Show_Data[j] = 0xC0; //* 伸缩电机 开路
#endif
	else
		Eor_Show_Data[j] = 0;
	j++;

#ifdef USE_M2
	error[0] = GetMoveDataTx(MoveDw)->ErrorCode[0];
	if (error[0] & 0x06)
		Eor_Show_Data[j] = 0xD6; //* 行走电机过载 短路
	else if (error[0] & 0x08)
		Eor_Show_Data[j] = 0xD7; //* 行走电机断路
	else if (error[0] & 0x80)
		Eor_Show_Data[j] = 0xD8; //* 揉捏电机过载 短路
	else
		Eor_Show_Data[j] = 0;
	j++;

	error[1] = GetMoveDataTx(MoveDw)->ErrorCode[1];
	if (error[1] == 0)
		Eor_Show_Data[j] = 0x00;
	else if (error[1] & 0x01)
		Eor_Show_Data[j] = 0xD8; //* 揉捏电机过载 短路
	else if ((error[1] & 0x02) && (WorkStateGet() == WorkState_Work))
		Eor_Show_Data[j] = 0xD9; // * 揉捏电机断路 未运行 未连接 补丁 屏蔽 250415
	else if (error[1] & 0x0C)
		Eor_Show_Data[j] = 0xDD; //* 拍打电机过载 短路
	else if (error[1] & 0x10)
		Eor_Show_Data[j] = 0xDC; //* 拍打电机断路 未运行 未连接
	else if (error[1] & 0x20)
		Eor_Show_Data[j] = 0xD5; //* 行走限位故障
	else if (error[1] & 0x40)
		Eor_Show_Data[j] = 0xD4; //* 行走计数故障
	else if (error[1] & 0x80)
		Eor_Show_Data[j] = 0xD6; //* 行走驱动故障
	j++;

	error[2] = GetMoveDataTx(MoveDw)->ErrorCode[2];
	if (error[2] == 0)
		Eor_Show_Data[j] = 0x00;
	else if (error[2] & 0x08)
		Eor_Show_Data[j] = 0xDA; //* 揉捏电机原点检测不到
	else if (error[2] & 0x10)
		Eor_Show_Data[j] = 0xDB; //* 揉捏电机计数故障
	else if (error[2] & 0x20)
		Eor_Show_Data[j] = 0xD8; //* 揉捏电机过载 短路 驱动故障
	j++;

#endif //

#ifdef M1M2LOSS_NO_PAUSE
	if (TICK_COUNTER(ArmKey_Loss_tick, 5000))
		Eor_Show_Data[j] = 0xE4; //* 左扶手通信故障
	else
		Eor_Show_Data[j] = 0;
	j++;
	if (TICK_COUNTER(Leg_Loss_tick, 5000))
		Eor_Show_Data[j] = 0xC9; //* 小腿通信故障
	else
		Eor_Show_Data[j] = 0;
#else
	if (TICK_COUNTER(MoveUp_Loss_tick, 3000))
		Eor_Show_Data[j] = 0xA0; //* 上机芯通信故障
	else
		Eor_Show_Data[j] = 0;

#ifdef USE_M2
	j++;
	if (TICK_COUNTER(MoveDw_Loss_tick, 3000))
		Eor_Show_Data[j] = 0xD2; //* 下机芯通信故障
	else
		Eor_Show_Data[j] = 0;
#endif
	j++;
	if (TICK_COUNTER(ArmKey_Loss_tick, 5000))
		Eor_Show_Data[j] = 0xE4; //* 左扶手通信故障
	else
		Eor_Show_Data[j] = 0;
#if LEG_COM
	j++;
	if (TICK_COUNTER(Leg_Loss_tick, 5000))
		Eor_Show_Data[j] = 0xC9; //* 小腿通信故障
	else
		Eor_Show_Data[j] = 0;
#endif

#endif
	j++;
	// * 新增 250604
	if (Leg_Protect_Flag) //* 防夹
		Eor_Show_Data[j] = 0x97;
	else if (Leg_Touch_Flag) //* 触地
		Eor_Show_Data[j] = 0x96;
#ifdef USE_M2
	else if (PreventFlag == 3) //* 机芯碰撞
		Eor_Show_Data[j] = 0x95;
#endif
	else
		Eor_Show_Data[j] = 0;
	j++;

	if (j >= Max_Eor_Group)
		return;

	for (i = 0; i < j; i++)
	{
		nAllEor = Eor_Show_Data[Eor_Show_Cnt];
		if (++Eor_Show_Cnt >= j)
			Eor_Show_Cnt = 0;

		if (nAllEor != 0)
			break;
	}
}
//------------------------错误代码显示---end------------------------------------------------

//------------------------错误代码显示---end------------------------------------------------

u8 Display_Data[80];

u8 DisType = 0, lcd_time = 0;

#ifdef A390
char device_Model[] = "{model A390    soft V1.0 firm V1.0 }";
#elif defined A390_2
char device_Model[] = "{model A390-2  soft V1.0 firm V1.0 }";
#elif defined A390_5
char device_Model[] = "{model A390-5  soft V1.0 firm V1.0 }";
#elif defined A390_8
char device_Model[] = "{model A390-8  soft V1.0 firm V1.0 }";
#elif defined A391
char device_Model[] = "{model A391    soft V1.0 firm V1.0 }";
#elif defined A391_8
char device_Model[] = "{model A391-8  soft V1.0 firm V1.0 }";
#elif defined A390Y
char device_Model[] = "{model A390Y soft V1.0 firm V1.0 }";
#elif defined A391Y
char device_Model[] = "{model A391Y soft V1.0 firm V1.0 }";
#elif defined A391S
char device_Model[] = "{model A391S soft V1.0 firm V1.0 }";
#elif defined A391S_1
char device_Model[] = "{model A391S-1 soft V1.6 firm V1.6 }";
#elif defined A391S_2
char device_Model[] = "{model A391S-2 soft V1.1 firm V1.1 }";
#elif defined A391S_5
char device_Model[] = "{model A391S-5 soft V1.0 firm V1.0 }";
#elif defined A391S_6
char device_Model[] = "{model A391S-6 soft V1.0 firm V1.0 }";
#elif defined A391S_8
char device_Model[] = "{model null soft V1.0 firm V1.0 }";
#elif defined A391S_31
char device_Model[] = "{model Virtus  soft V1.4 firm V1.4 }";
#elif defined A391S_32
char device_Model[] = "{model A391S-32  soft V1.1 firm V1.1 }";
#elif defined A391S_33
char device_Model[] = "{model 020.002299  soft V1.0 firm V1.0 }";
#elif defined A391S_35
char device_Model[] = "{model MOLAP  soft V1.0 firm V1.0 }";
#elif defined A391S_36
char device_Model[] = "{model A391S-36 soft V1.1 firm V1.1 }";
#elif defined A391S_37
char device_Model[] = "{model A391S-37 soft V1.0 firm V1.0 }";
#elif defined A391S_38
char device_Model[] = "{model A391S-38 soft V1.1 firm V1.1 }";
#elif defined A390S_2
char device_Model[] = "{model A390S-2 soft V1.1 firm V1.1 }";
#elif defined A390S_6
char device_Model[] = "{model AURORA soft V1.1 firm V1.1 }";
#elif defined A390S_8
char device_Model[] = "{model Zenith soft V1.0 firm V1.0 }";
#elif defined A390S_9
char device_Model[] = "{model A390S-9  soft V1.0 firm V1.0 }";
#elif defined A390S_36
char device_Model[] = "{model A390S-36 soft V1.1 firm V1.1 }";
#elif defined A390S_20
char device_Model[] = "{model A390S-20 soft V1.0 firm V1.0 }";
#elif defined A393S_20
char device_Model[] = "{model A393S_20 soft V1.0 firm V1.0 }";
#elif defined A390S_31
char device_Model[] = "{model A390S-31 soft V1.0 firm V1.0 }";
#elif defined A390S_32
char device_Model[] = "{model A390S-32 soft V1.0 firm V1.0 }";
#elif defined A390S_33
char device_Model[] = "{model A390S-33 soft V1.0 firm V1.0 }";
#elif defined A390S_11
char device_Model[] = "{model A390S-11 soft V1.0 firm V1.0 }";
#elif defined A390S_10
char device_Model[] = "{model Premium soft V1.0 firm V1.0 }";
#elif defined A390S_35
char device_Model[] = "{model A390S-35 soft V1.0 firm V1.0 }";
#elif defined A393S_2
char device_Model[] = "{model A393S-2  soft V1.0 firm V1.0 }";
#elif defined A393S_5
char device_Model[] = "{model A393S-5  soft V1.0 firm V1.0 }";
#elif defined A395SY_2
char device_Model[] = "{model A395SY-2  soft V1.1 firm V1.1 }";
#elif defined A395SY
char device_Model[] = "{model null soft V1.0 firm V1.0 }";
#elif defined A395SY_6
char device_Model[] = "{model null soft V1.0 firm V1.0 }";
#elif defined A393SY_2
char device_Model[] = "{model A393SY-2  soft V1.0 firm V1.0 }";
#elif defined A396SY_2
char device_Model[] = "{model A396SY-2  soft V1.0 firm V1.0 }";
#elif defined A396S_1
char device_Model[] = "{model A396S-1  soft V1.0 firm V1.0 }";
#elif defined A560S_2
char device_Model[] = "{model A560S-2 soft V1.0 firm V0.8 }";
#elif defined A396SYN_2
char device_Model[] = "{model A396SYN-2  soft V1.0 firm V1.0 }";
#elif defined A671S_21
char device_Model[] = "{model A671S-21  soft V1.0 firm V1.0 }";
#else
char device_Model[] = "{model A39x    soft V1.0 firm V0.8 }";
#endif

u8 DeviceInf_Handle_Update = 0, custom_State, custom_Update;
u8 DeviceInf_Error = 0;

/*****************************************************************************************************
 * 描述：手控器显示更新
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
uint8_t EndByte;
uint16_t DataLength;
uint16_t crc_temp;
void Display_Drive_A1(void)
{
#ifndef USART0_DMA
	uint8_t i;
#endif
#ifdef USE_M2

#endif
	u8 temp, width, method;

	memset(&Display, 0, sizeof(Display));

	DataLength = sizeof(Display);
	// 数据包固定段==================================================
	Display_Data[0] = 0x55;
	Display_Data[1] = 0xaa;
	Display_Data[2] = 0x07;
	Display_Data[3] = 0x01;
	Display_Data[4] = 0x01;
	Display_Data[5] = 0xA1;
	Display_Data[6] = 0x00;
	Display_Data[7] = DataLength;
	Display_Data[8] = DataLength >> 8;
	// 数据段========================================================
	Display.WorkState = WorkStateGet(); /* 工作状态 */
	if (Leg_Protect_Flag)				// * add for health test with leg protect 250821
	{
		Display.WorkState = WorkStateGet();
	}
	else if (Health_State_Fail == Health_State)
	{
		Display.WorkState = 9;
	}
	else if (Health_State_Check == Health_State)
	{
		Display.WorkState = 10;
	}
	else if (Health_State_Finish == Health_State)
	{
		Display.WorkState = 11;
	}

	Display.Timing_min = WorkMinGet(); /* 运行时间 min */
	Display.Timing_s = WorkSecGet();   /* 运行时间 sec */

	if (WorkStateGet() == WorkState_ShoulderAdj) // * 肩位调节时不动 250630
		Display.Byte3.Bit.MovementPosition_Upper = 10;
	else
		Display.Byte3.Bit.MovementPosition_Upper = 12 - BackPosUpGet(); // * 修改 最下端1点

#ifdef USE_M2
	uint16_t M2Circle = GetMoveDataTx(MoveDw)->Tread.CircleH << 8 | GetMoveDataTx(MoveDw)->Tread.CircleL;
	Display.Byte3.Bit.MovementPosition_Lower = M2Circle / 58 + 1; /* 下机芯位置 (0-505)/51 250924 */
#else
	Display.Byte3.Bit.MovementPosition_Lower = 0;
#endif

	Display.Byte4.Bit.HandCheck = MoveWhichGet() + 1; /* 机械手选择 0：上机芯  1：下机芯 */ // * 修改 250211
	Display.Byte4.Bit.Shoulderposition = 11 - ShoulderValue;								/* 肩位置档位 */
	Display.AutoMode = AutoModeGet();														/* 自动模式 */
	if (Display.AutoMode == A_LC)
	{
		Display.AutoMode = 1; //*老化模式给个显示
	}
	else if (Display.AutoMode > A_LC)
	{
		Display.AutoMode = 0; /* 总共20个自动程序，老化、包装等模式超过20后显示无手法 */
	}

	if (TICK_COUNTER(Buzzer_Shoulder_Tick, 2000) && Buzzer_Ararm > 0) // * 修改 250509
	{
		if (Buzzer == SOUND_NULL)
			Buzzer = SOUND_ALARM;
		else if (Buzzer > SOUND_NULL)
			Buzzer = SOUND_NULL;
		Display.SoundValue = Buzzer;
	}
	else
	{
		Display.SoundValue = Buzzer;  //* 普通应答声 只响一次
		if (Buzzer != SOUND_SHOULDER) //* 这种声音自己清零 250225
			Buzzer = SOUND_NULL;
	}

	if (MoveUp == MoveWhichGet()) /* 显示机芯位置 */
	{
		Display.Byte8.Bit.AreaValue = MassAreaGet(MoveUp); /* 按摩区间 一般下机芯不可调 */
		Display.Byte7.Bit.Intensity4D = nBackIntensity + 1;
		if (AutoModeGet() == 0)
		{
			if ((TZ == MassWiseGet(MoveUp)) || (ZYS == MassWiseGet(MoveUp)))
			{
				Display.Byte7.Bit.MassageSpeed = 0;
			}
			else
				Display.Byte7.Bit.MassageSpeed = MassSpeedGet(MoveUp); /* 按摩速度 上机芯 */

			if ((RNZ == MassWiseGet(MoveUp)) || (RPZ == MassWiseGet(MoveUp)) || (D3RNZ == MassWiseGet(MoveUp)) ||
				(D3RPZ == MassWiseGet(MoveUp)) || (JRZ == MassWiseGet(MoveUp)))
			{
				Display.Byte8.Bit.WidthValue = 0;
			}
			else
				Display.Byte8.Bit.WidthValue = MassWidthGet(MoveUp) + 1; /* 按摩宽度 上机芯 */
		}
		else // * 手动 自动 速度宽度显示分开 250924
		{
			temp = (MSG2Move[MoveUp].Knead.Speed & 0x0f) >> 1;
			if (temp == 0)
				temp = (MSG2Move[MoveUp].Tap.Speed & 0x0f) >> 1;
			if (temp == 0)
				temp = (MSG2Move[MoveUp].Tread.Speed & 0x0f) >> 1;
			if (temp > 5)
				temp = 5;
			Display.Byte7.Bit.MassageSpeed = temp; /* 按摩速度 上机芯 */
			method = MSG2Move[MoveUp].Knead.Method;
			temp = 0;
			if (method == KNEAD_FIX_CIRCLE || method == KNEAD_FWD_ABS || method == KNEAD_REV_ABS || method == KNEAD_STOP)
			{
				width = MSG4Move[MoveUp].Knead.CircleL;
				if (width < 30)
					temp = (30 - temp) / 7;
				else if (width > 35)
					temp = (width - 35) / 7;
				Display.Byte8.Bit.WidthValue = temp + 1;
			}
			else
			{
				Display.Byte8.Bit.WidthValue = 0;
			}
		}
	}
#ifdef USE_M2
	else if (MoveDw == MoveWhichGet())
	{
		Display.Byte8.Bit.AreaValue = 4; // * 下机芯只有定点 250218
#ifdef USE_M2_4D
		Display.Byte7.Bit.Intensity4D = nBackIntensity_M2 + 1; // todo add 260109
#endif
		Display.Byte7.Bit.MassageSpeed = MassSpeedGet(MoveDw);	 /* 按摩速度 下机芯 */
		Display.Byte8.Bit.WidthValue = MassWidthGet(MoveDw) + 1; /* 按摩宽度 下机芯 */
	}
#endif

	if (RollStateGet())
		Display.Byte9.Bit.RollState = RollSpeedGet(); /* 脚滚速度 */
	else
		Display.Byte9.Bit.RollState = 0; /* 脚滚速度 */

	Display.Byte9.Bit.ZeroState = nCurZeroState; /* 零重力档位 */

	Display.Byte10.Bit.AirStrength = AirIntensityGet(); /* 气压强度 */
#if defined(USE_S0555) || defined(USE_S0551)
	temp = AirModeGet(); // @ md 251205
	if (temp > 3)
		temp = 3;
	Display.Byte10.Bit.AirMode = temp;
#else
	Display.Byte10.Bit.AirMode = AirModeGet(); /* 充气模式 1：上半身 2：下半身 3：全身 */
#endif
	Display.Byte11.Bit.BackHeatState = (HeatStateGet(BackHeat) > 0); /* 背部加热 */
	Display.Byte11.Bit.LegsHeatState = (HeatStateGet(LegHeat) > 0);	 /* 腿部加热 */
	Display.Byte11.Bit.PJHeatState = 0;								 // HeatStateGet(PJHeat); /* 披肩加热 一般和背部不共存，暂时和背部同一参数 */ // * 有bug

	/*气囊图标显示----------------------------------------------------------------------------------------------------------------------------*/
	Display.Byte12.Bit.Foot = AirPartGet().Part2.Bit.Ankle | AirPartGet().Part2.Bit.FeetF | AirPartGet().Part2.Bit.FeetSide; /* 足部气囊 */
	// Display.Byte12.Bit.Leg = AirPartGet().Part2.Bit.CalfBot | AirPartGet().Part2.Bit.CalfSide;
	Display.Byte12.Bit.Leg = AirPartGet().Part2.Bit.CalfSide;																							/* 腿部气囊 */
	Display.Byte12.Bit.Arm = AirPartGet().Part1.Bit.ArmLB | AirPartGet().Part1.Bit.ArmLF | AirPartGet().Part1.Bit.ArmRB | AirPartGet().Part1.Bit.ArmRF; /* 手臂气囊 */
#if (AIR_SEAT)
	Display.Byte12.Bit.Hip = AirPartGet().Part2.Bit.SeatL | AirPartGet().Part2.Bit.SeatR; /* 座侧气囊 */
#else
	Display.Byte12.Bit.Hip = 0;
#endif
	Display.Byte12.Bit.Shoulder = AirPartGet().Part1.Bit.ShoulderL | AirPartGet().Part1.Bit.ShoulderR; /* 肩膀气囊 */

	/*--------------------------------------------------------------------------------------------------------------*/
	Display.Byte13.Bit.LowPower = (nLowPower == 0);
#ifdef USE_SOUNDWAVE
	Display.Byte13.Bit.nSoundWaveState = nSoundWaveSW; // @ add 音波开关 260324
#endif
	Display.Byte13.Bit.BlueToothState = nBlueState;	 /* 蓝牙开关 */
	Display.Byte13.Bit.VoiceState = nVoiceControlSw; /* 声控开关 */
	Display.Byte13.Bit.LC_State = nLCState;			 /* 老化状态 */
#if (ROLLER_CALVES_EN)
	Display.Byte13.Bit.CalfKneadState = (GetLegDataRx()->Knead1.State > 0); /* 腿部揉搓开关 */
	Display.Byte13.Bit.CalfKnead = (KneadModeGet() > 0);					// * 增加腿部揉搓状态显示
#else
	Display.Byte13.Bit.CalfKnead = 0;
#endif
#ifdef USE_ION
	Display.Byte13.Bit.Ion = nIonState; /* 负氧离子开关 */ // todo 没有负氧离子 260818
#endif
	Display.MassageStrength = UserIntensityGet(); /* 力度模式 0：轻揉 1：舒缓 2：强劲 */
	Display.EorValue = nAllEor;					  /* 错误提示 */

	Display.Byte16.Bit.RedLightGear = RedLight_Duty; // todo add 261021

	Display.WiseValue = 0; // @ 1->0 王琦翔修改 260707
	/*手法显示===========================================================================*/
	if ((MoveUp == MoveWhichGet()) && (0 == AutoModeGet())) /* 上机芯手法显示 */
	{
		switch (MassWiseGet(MoveUp))
		{
		default:
			break;
		case TZ: // 无手法
			Display.WiseValue = 1;
			Display.Byte7.Bit.MassageSpeed = 0; // *  增加 250310
			break;
		case RNZ:	// 揉捏正
		case RNF:	// 揉捏反
		case RNZB:	// 揉捏正变（内圈快 外圈慢）
		case RNFB:	// 揉捏反--颈部停顿
		case RNZFS: // 快速1/4正反 -- 风池穴
		case RNZFD: // 快速1/4正反 -- 肩部
			Display.WiseValue = 2;
			Display.Byte8.Bit.WidthValue = 0;
			break;

		case PD:
			Display.WiseValue = 3;
			break;
		case RPZ:
		case RPZB:
			Display.WiseValue = 4;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case PDB:
			Display.WiseValue = 5;
			break;
#ifdef USE_S0555
		case ZYS:
			Display.WiseValue = 6;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case ZY:
			Display.WiseValue = 7;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case JRZ:
			Display.WiseValue = 8;
			Display.Byte8.Bit.WidthValue = 0;
			break;

		case D3RNZ:
		case D3RNF:
			Display.WiseValue = 9;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case D3PD:
			Display.WiseValue = 10;
			break;
		case D3RPZ:
			Display.WiseValue = 11;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case ZYD:
			Display.WiseValue = 12;
			break;
#else

		case ZY:
		case ZYS:
			Display.WiseValue = 6;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case D3RNZ:
		case D3RNF:
			Display.WiseValue = 7;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case D3PD:
			Display.WiseValue = 8;
			break;
		case D3RPZ:
			Display.WiseValue = 9;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case ZYD:
			Display.WiseValue = 10;
			break;
#endif
		}
		//		Display.WiseValue = MassWiseGet(MoveUp);
	}
#ifdef USE_M2
	else if ((MoveDw == MoveWhichGet()) && (0 == AutoModeGet())) /* 下机芯手法显示 */
	{
		switch (MassWiseGet(MoveDw))
		{
		default:
			break;
		case TZ: // 无手法
			Display.WiseValue = 1;
			Display.Byte7.Bit.MassageSpeed = 0; // *  增加 250310
			break;
		case RNZ:	// 揉捏正
		case RNF:	// 揉捏反
		case RNZB:	// 揉捏正变（内圈快 外圈慢）
		case RNFB:	// 揉捏反--颈部停顿
		case RNZFS: // 快速1/4正反 -- 风池穴
		case RNZFD: // 快速1/4正反 -- 肩部
			Display.WiseValue = 2;
			Display.Byte8.Bit.WidthValue = 0;
			break;

		case PD:
			Display.WiseValue = 3;
			break;
		case RPZ:
		case RPZB:
			Display.WiseValue = 4;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case PDB:
			Display.WiseValue = 5;
			break;
#ifdef USE_S0555
		case ZYS:
			Display.WiseValue = 6;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case ZY:
			Display.WiseValue = 7;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case JRZ:
			Display.WiseValue = 8;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;

		case D3RNZ:
		case D3RNF:
			Display.WiseValue = 9;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case D3PD:
			Display.WiseValue = 10;
			break;
		case D3RPZ:
			Display.WiseValue = 11;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case ZYD:
			Display.WiseValue = 12;
			break;
#else

		case ZY:
		case ZYS:
			Display.WiseValue = 6;
			Display.Byte7.Bit.MassageSpeed = 0;
			break;
			//			case JRZ:

		case D3RNZ:
		case D3RNF:
			Display.WiseValue = 7;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case D3PD:
			Display.WiseValue = 8;
			break;
		case D3RPZ:
			Display.WiseValue = 9;
			Display.Byte8.Bit.WidthValue = 0;
			break;
		case ZYD:
			Display.WiseValue = 10;
			break;
#endif
		}
	}
#endif
	/*=======================================================================*/
	Display.HeartRate = Health_Heart; /* 心率 */
	Display.Sop2 = Health_Blood;	  /* 血氧 */
	Display.MicLoop = Health_WXH;	  /* 微循环 */
	Display.TiredStat = Health_Tired; /* 疲劳值 */
	Display.CountDown = Health_Cnt;	  //(Health_Cnt * 100) / Health_MaxCheckCnt;       /* 疲劳检测进度条 */

	Display.Byte26.Bit.VoicePlay = nVoicePlayer; /* 语音播报开关*/
	Display.Byte26.Bit.Volume = nVolume;		 /* 音量 */

	Display.Byte27.Bit.LedMode = nLigntSw;			   /* LED灯开关 */
	Display.Byte27.Bit.BGM = nMusicSW;				   /* 背景音乐开关 */
	Display.Byte27.Bit.TimOutReset = nTimOutResetSw;   /* 按摩时间到椅子自动复位开关 */
	Display.Byte27.Bit.AnkleRocking = FootSwing_Level; /* 足踝摇摆挡位 */

	Display.Language = Dispaly_Language; /* 语言  */ // * 修改 nLanguage 250226

	Display.Memory = Memorystate; /* 存储状态 */
	Display.Byte30 = 0;			  //@ 工程模式 暂无
#ifdef USE_SOUNDWAVE
	Display.Byte31 = nVolume_SoundWave / 3; // todo md 260803
	if (BlueState_Play && nPlay_Device == 2)
	{
		switch (Blue_Play_ID1)
		{
		case VC_Music1:
			Display.SoundNum = 1 << 2;
			Display.SoundMin = 2;
			Display.SoundSec = 5;
			break;
		case VC_Music2:
			Display.SoundNum = 2 << 2;
			Display.SoundMin = 2;
			Display.SoundSec = 5;
			break;

		case VC_Music3:
			Display.SoundNum = 3 << 2;
			Display.SoundMin = 1;
			Display.SoundSec = 40;
			break;

		case VC_YBMusic1:
			Display.SoundNum = 4 << 2;
			Display.SoundMin = 3;
			Display.SoundSec = 5;
			break;

		case VC_YBMusic2:
			Display.SoundNum = 5 << 2;
			Display.SoundMin = 2;
			Display.SoundSec = 56;
			break;

		case VC_YBMusic3:
			Display.SoundNum = 6 << 2;
			Display.SoundMin = 5;
			Display.SoundSec = 1;
			break;

		case VC_YBMusic4:
			Display.SoundNum = 7 << 2;
			Display.SoundMin = 2;
			Display.SoundSec = 40;
			break;

		default:
			break;
		}
	}
	if (nSoundMusicSW) // todo 增加 260707
		Display.SoundNum |= 2;
	else
		Display.SoundNum |= 1;
#endif
#ifdef USE_HEALTH_AUTO_NUM
	// todo 疲劳程度 0-3 对应程序由主板控制 260918
	switch (Health_Tired)
	{
#ifdef VIETNAM_VOICE
	case 0:
		Display.Health_Auto_Num = 16;
		break;
	case 1:
		Display.Health_Auto_Num = 1;
		break;
	case 2:
		Display.Health_Auto_Num = 6;
		break;
	case 3:
		Display.Health_Auto_Num = 3;
		break;
#else
	case 0:
		Display.Health_Auto_Num = 4;
		break;
	case 1:
		Display.Health_Auto_Num = 1;
		break;
	case 2:
		Display.Health_Auto_Num = 3;
		break;
	case 3:
		Display.Health_Auto_Num = 5;
		break;
#endif
	}
#endif
	/*==================================================================================*/
	memcpy(&Display_Data[9], &Display, sizeof(Display));

	EndByte = 9 + sizeof(Display);
	crc_temp = CRC16_CCITT_FALSE(Display_Data, EndByte);
	Display_Data[EndByte + 0] = crc_temp & 0x00FF;
	Display_Data[EndByte + 1] = crc_temp >> 8;
#ifdef USART0_DMA
	EUSART0_Write(Display_Data, EndByte + 2);
#else
	for (i = 0; i < (EndByte + 2); i++)
		EUSART0_Write(Display_Data[i]);
#endif

	usart_interrupt_enable(USART0, USART_INT_TBE);
}

void Display_Drive_A2(void)
{
	u8 i, length;
	Display_Data[0] = 0x55;
	Display_Data[1] = 0xaa;
	Display_Data[2] = 0x07;
	Display_Data[3] = 0x01;
	Display_Data[4] = 0x01;
	Display_Data[5] = 0xA2; // * 协议关键字
	Display_Data[6] = 0x00;

	length = sizeof(device_Model) - 1;
	Display_Data[7] = length; // * 协议长度
	Display_Data[8] = 0x00;

	memcpy(&Display_Data[9], device_Model, length);

	crc_temp = CRC16_CCITT_FALSE(Display_Data, 9 + length);
	Display_Data[9 + length] = crc_temp & 0x00FF;
	Display_Data[10 + length] = crc_temp >> 8;
#ifdef USART0_DMA
#else
	for (i = 0; i < 11 + length; i++)
		EUSART0_Write(Display_Data[i]);
#endif
	usart_interrupt_enable(USART0, USART_INT_TBE);
}

void Display_Drive_A3(void)
{
	u8 i;
	Display_Data[0] = 0x55;
	Display_Data[1] = 0xaa;
	Display_Data[2] = 0x07;
	Display_Data[3] = 0x01;
	Display_Data[4] = 0x01;
	Display_Data[5] = 0xA3; // * 协议关键字
	Display_Data[6] = 0x00;
	Display_Data[7] = 0x04; // * 协议长度
	Display_Data[8] = 0x00;

	Display_Data[9] = UserCode_data >> 24;
	Display_Data[10] = UserCode_data >> 16;
	Display_Data[11] = UserCode_data >> 8;
	Display_Data[12] = UserCode_data;

	crc_temp = CRC16_CCITT_FALSE(Display_Data, 13);
	Display_Data[13] = crc_temp & 0x00FF;
	Display_Data[14] = crc_temp >> 8;
#ifdef USART0_DMA
#else
	for (i = 0; i < 15; i++)
		EUSART0_Write(Display_Data[i]);
#endif
	usart_interrupt_enable(USART0, USART_INT_TBE);
}

void Display_Drive_A4(void)
{
	u8 i, size;
#ifdef USE_S0555
	size = 2 + 5; // @ 定制有效长度 增加设置按钮控制功能5个字节  260306
#elif defined USE_S0551
	size = 2 + 6;
#else
	size = 2;
#endif

	Display_Data[0] = 0x55;
	Display_Data[1] = 0xaa;
	Display_Data[2] = 0x07;
	Display_Data[3] = 0x01;
	Display_Data[4] = 0x01;
	Display_Data[5] = 0xA4; // * 协议关键字
	Display_Data[6] = 0x00;
	Display_Data[7] = size;
	Display_Data[8] = 0x00;

	// * 增加 单双机芯 判断 250623
	//* 0x28 双推杆、电动伸缩小腿  0x24 弹簧小腿
	i = 0;
#ifdef USE_M2
	i |= 0x02;
#else
	i |= 0x01;
#endif

#if (LEG_COM)
	i |= 0x08;
#else
	i |= 0x04;
#endif

	i |= 0x20; //* 目前全是双推杆
	Display_Data[9] = i;

	//@ 特别注意250516 0x1D 有滚轮、揉搓、小腿加热、靠背加热 0x15 无小腿揉搓 0x11 无小腿揉搓、无小腿加热
	i = 0;
#if (HEAT_BACK)
	i |= 0x01;
#endif
#if (HEAT_LEG)
	i |= 0x04;
#endif
#if (ROLLER_CALVES_EN)
	i |= 0x08;
#endif
#if (ROLLER_FOOT_EN)
	i |= 0x10;
#endif
	Display_Data[10] = i;
#ifdef USE_S0555
	// @ add 260306 从右到左
	Display_Data[11] = 0x21; //* 1复位 2童锁
	Display_Data[12] = 0x43; //* 3蓝牙 4LED
	Display_Data[13] = 0x65; //* 5背景 6声控
	Display_Data[14] = 0x07; //* 7语音 8负氧
	Display_Data[15] = 0x09; //* 音量 音波
#elif defined USE_S0551
	Display_Data[11] = 0x21; //* 1复位 2童锁
	Display_Data[12] = 0x43; //* 3蓝牙 4LED

	// @ 分页显示 不同于S0555 260328
#if (VOICE_CONTROL_EN == 1)
	Display_Data[13] = 0x21; //* 5背景 6声控
	Display_Data[14] = 0x03; //* 7语音 8负氧
#else
	Display_Data[13] = 0x01; //* 5背景 6声控
	Display_Data[14] = 0x02; //* 7语音 8负氧
#endif

	Display_Data[15] = 0x07; //* 音量 音波
	Display_Data[16] = 0x00; // @ 音波 res 260328
#endif

	crc_temp = CRC16_CCITT_FALSE(Display_Data, size + 9);
	Display_Data[size + 9] = crc_temp & 0x00FF;
	Display_Data[size + 10] = crc_temp >> 8;
#ifdef USART0_DMA
#else
	for (i = 0; i < (size + 11); i++)
		EUSART0_Write(Display_Data[i]);
#endif
	usart_interrupt_enable(USART0, USART_INT_TBE);
}

void Display_Drive_B1(void)
{
	u8 i;

	Display_Data[0] = 0x55;
	Display_Data[1] = 0xaa;
	Display_Data[2] = 0x07;
	Display_Data[3] = 0x01;
	Display_Data[4] = 0x01;
	Display_Data[5] = 0xB1; // * 协议关键字
	Display_Data[6] = 0x00;
	Display_Data[7] = 0x0A; // * 协议长度
	Display_Data[8] = 0x00;

	memcpy(&Display_Data[9], DIY_packet, 10);

	crc_temp = CRC16_CCITT_FALSE(Display_Data, 19);
	Display_Data[19] = crc_temp & 0x00FF;
	Display_Data[20] = crc_temp >> 8;
#ifdef USART0_DMA
#else
	for (i = 0; i < 21; i++)
		EUSART0_Write(Display_Data[i]);
#endif
	usart_interrupt_enable(USART0, USART_INT_TBE);
}

void Display_Drive(void) //* 200ms时基
{
	BackPosUpdate(); //* 上机芯位置
	if (TICK_COUNTER(Handle_Dispaly_tick, 5000))
	{
		Handle_Dispaly_Step = 0;
	}

	switch (Handle_Dispaly_Step)
	{
	case 0:
		Display_Drive_A2();
		break;
	case 1:
		Display_Drive_A3();
		break;
	case 2:
		Display_Drive_A4();
		break;
	case 3:
		Display_Drive_B1();
		break;
	case 4:
		Display_Drive_A1();
		break;

	default:
		break;
	}
}

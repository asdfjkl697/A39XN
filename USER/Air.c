/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "Air.h"
#include "Table.h"

/*
 * 变量定义 *************************************************************************************************************************************
 */
AirInfo_S AirValve;
static UnionBit AirLegBit;
AirT_Work_S AirT_Work;

/*****************************************************************************************************
 * 描述：气囊动作执行
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void AirLogic_Process(void)
{
	static uint8_t AirPreState = 0;

	if (AirPreState != AirModeGet())
	{
		AirPreState = AirModeGet();
		if (AirPreState != AirModeAuto)
		{
			AirMode_Load(AirPreState);
		}
	}

	if (AirModeClose == AirPreState)
	{
		Gas_CloseAll();
	}

	if (WorkState_Pause == WorkStateGet())
	{
		Gas_CloseAll();
	}
}
/*****************************************************************************************************
 * 描述：8804 气阀开关控制
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void DRV8804_OUT(u16 dat)
{
	u8 i;
	u16 temp;

	temp = 0;
	for (i = 0; i < 16; i++)
	{
		temp = dat >> i;
		temp &= 0x0001;
		VALVE_CLK_SET;
		if (temp)
		{
			VALVE_DAT_SET;
		}
		else
		{
			VALVE_DAT_RESET;
		}
		VALVE_CLK_RESET;
	}
	VALVE_CLK_SET;

	for (i = 0; i < 50; i++)
		;
	VALVE_LATCH_SET;
	for (i = 0; i < 100; i++)
		;
	VALVE_LATCH_RESET;
}

/************************************************************************************************************************************************
 * 描述：充气 关闭所有
 * 参数：无
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void Gas_CloseAll(void)
{
	uint16_t Part = 0;
	Gas_Keep_Clear();	// * add 250910
	AirIntensitySet(0); // * 关闭气压强度 250301
	PumpStateSet(0);
	LegAirStateSet(0);
	PumpOff();
	AirValve.GasPart.Part1.All = 0;
	AirValve.GasPart.Part2.All = 0;
	Part = AirValve.GasPart.Part1.All | (AirValve.GasPart.Part2.All << 8); /* 通过Air内部数据控制8804 */

	DRV8804_OUT(Part);
}

//* 运行过程中转换气囊工作模式 清除一下
void Gas_Clear(void) // * add 250905
{
	uint16_t Part = 0;
	Gas_Keep_Clear(); // * add 250910
	PumpStateSet(0);
	LegAirStateSet(0);
	PumpOff();
	AirValve.GasPart.Part1.All = 0;
	AirValve.GasPart.Part2.All = 0;
	Part = AirValve.GasPart.Part1.All | (AirValve.GasPart.Part2.All << 8); /* 通过Air内部数据控制8804 */

	DRV8804_OUT(Part);
}

/************************************************************************************************************
 * 描述：设置要充气的部位   将外部传入的充气参数（一般已经指定充气部位），映射到实际的气阀控制口
 * 参数：AirPart1:外部传入的充气部位参数1
		 AirPart2:外部传入的充气部位参数2
		 AirPart3:外部传入的充气部位参数3
 * 返回：无
 * 注意：有的8804的OUTx中的x可能会和Part1、Part2的bit顺序相反，即Part2bit8可能代表着8804的OUT1，具体需核对原理图
************************************************************************************************************/
void Air_SetGasPart(GasPart1_t AirPart1, GasPart2_t AirPart2)
{
	UnionBit Part1, Part2;

	Part1.Strack = 0;
	Part2.Strack = 0;

	AirValve.GasPart.Part1.All = AirPart1.All; /* 将外部传入的数据赋给Air内部数据 */
	AirValve.GasPart.Part2.All = AirPart2.All;
	/*PART1   8804的OUT16~OUT9=============================================*/

	Part1.StrackBit.BIT0 = 0;
	Part1.StrackBit.BIT1 = 0;
	Part1.StrackBit.BIT2 = 0;
	Part1.StrackBit.BIT3 = 0;
	// @ 白色端子
#if (LEG_COM)
	Part1.StrackBit.BIT4 = 0;
	Part1.StrackBit.BIT5 = 0;
	Part1.StrackBit.BIT6 = 0;
	Part1.StrackBit.BIT7 = 0;
#else
	Part1.StrackBit.BIT4 = 0;
	Part1.StrackBit.BIT5 = AirValve.GasPart.Part2.Bit.FeetSide;
#if (AIR_CLAFBOT)
	Part1.StrackBit.BIT6 = AirValve.GasPart.Part2.Bit.CalfBot;
#else
	Part1.StrackBit.BIT6 = AirValve.GasPart.Part2.Bit.CalfSide;
#endif
	Part1.StrackBit.BIT7 = AirValve.GasPart.Part2.Bit.CalfSide;
#endif

	/*PART2   8804的OUT8~OUT1=============================================*/
	// @ 绿色端子
#if defined(AIR_SEAT) && !defined(AIR_SEAT_GONGHE)
	Part2.StrackBit.BIT0 = AirValve.GasPart.Part2.Bit.SeatR | AirValve.GasPart.Part2.Bit.SeatL; // todo 待确认 260518
#else
	Part2.StrackBit.BIT0 = 0;
#endif
	Part2.StrackBit.BIT1 = AirValve.GasPart.Part1.Bit.ShoulderL | AirValve.GasPart.Part1.Bit.ShoulderR; // AirPart1.Bit.ShoulderL | AirPart1.Bit.ShoulderR;  /* 肩部 */
	Part2.StrackBit.BIT2 = AirValve.GasPart.Part1.Bit.ArmRF | AirValve.GasPart.Part1.Bit.ArmRB;			// AirPart1.Bit.ArmRF | AirPart1.Bit.ArmRB;          /* 右手 */
	Part2.StrackBit.BIT3 = AirValve.GasPart.Part1.Bit.ArmLF | AirValve.GasPart.Part1.Bit.ArmLB;			// AirPart1.Bit.ArmLF | AirPart1.Bit.ArmLB;          /* 左手 */
	// @ 黑色端子
	Part2.StrackBit.BIT4 = 0;
#ifdef AIR_SEAT_GONGHE
	Part2.StrackBit.BIT5 = AirValve.GasPart.Part2.Bit.SeatR | AirValve.GasPart.Part2.Bit.SeatL;
	Part2.StrackBit.BIT6 = AirValve.GasPart.Part2.Bit.SeatR | AirValve.GasPart.Part2.Bit.SeatL;
#else
	Part2.StrackBit.BIT5 = 0;
	Part2.StrackBit.BIT6 = 0;
#endif
	Part2.StrackBit.BIT7 = 0;

	/* 通过485通讯，将气阀状态传输到腿部，具体BIT位与气囊对应关系核对协议 */
#if (LEG_COM)
	#if (1)																								// ! 默认都是脚侧第一阀 大问题 以前量产的脚部气压可能不对 产线过来的样机没有按照SOP生产 260701
		AirLegBit.StrackBit.BIT1 = AirValve.GasPart.Part2.Bit.FeetF | AirValve.GasPart.Part2.Bit.Ankle; /* 脚前+脚后跟 */
		AirLegBit.StrackBit.BIT0 = AirValve.GasPart.Part2.Bit.FeetSide;									/* 脚侧 */
	#else
		AirLegBit.StrackBit.BIT0 = AirValve.GasPart.Part2.Bit.FeetF | AirValve.GasPart.Part2.Bit.Ankle; /* 脚前+脚后跟 */
		AirLegBit.StrackBit.BIT1 = AirValve.GasPart.Part2.Bit.FeetSide;									/* 脚侧 */
	#endif
		AirLegBit.StrackBit.BIT2 = AirValve.GasPart.Part2.Bit.CalfSide; /* 腿侧 */
		AirLegBit.StrackBit.BIT3 = 0;

	// TODO 样机使用 261014
	// AirLegBit.StrackBit.BIT0 = AirValve.GasPart.Part2.Bit.FeetF | AirValve.GasPart.Part2.Bit.Ankle; /* 脚前+脚后跟 */
	// AirLegBit.StrackBit.BIT1 = AirValve.GasPart.Part2.Bit.FeetSide;									/* 脚侧 */
	// AirLegBit.StrackBit.BIT2 = 0;																	/* 腿侧 */
	// AirLegBit.StrackBit.BIT3 = AirValve.GasPart.Part2.Bit.CalfSide;

	AirLegBit.StrackBit.BIT4 = 0;
	AirLegBit.StrackBit.BIT5 = 0;
	AirLegBit.StrackBit.BIT6 = 0;
	AirLegBit.StrackBit.BIT7 = 0;

	LegAirStateSet(AirLegBit.Strack);
#endif

	Air_ActionHandle(Part1, Part2);
}

/************************************************************************************************************************************************
 * 描述：充气动作处理
 * 参数：ExePeriod 执行周期
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
// static uint32_t AirPart;
void Air_ActionHandle(UnionBit Part1, UnionBit Part2)
{
	uint32_t airpart;
	u8 keep_state = Get_Gas_Keep_State();
	// airpart = 0;
	airpart = Part1.Strack | Part2.Strack << 8;
#if (LEG_COM)
	if ((airpart != 0 || AirLegBit.Strack != 0) && keep_state == 0) // * 增加保压判断 250910
#else
	if (airpart != 0 && keep_state == 0)
#endif
	{
		PumpStateSet(1);
		PumpOn(100);
	}
	else
	{
		PumpStateSet(0);
		// PumpOn(0);
		PumpOff(); // * modify 250910
	}
	DRV8804_OUT(airpart);
}

// * 气囊工作模式变化
static uint8_t PreAirMode_a = 0; // * 增加补丁 250407
void Air_All_Change(u8 para1)
{
	if (AirModeGet() == PreAirMode_a && AtTabGet(Table_Air)->CurAirTab == para1)
		return;
	PreAirMode_a = AirModeGet();
	memset(&AirT_Work, 0, sizeof(AirT_Work_S)); // * 初始化气压参数 251113

	if (AirModeGet() == AirModeUpper) // * 修改 250407
	{
		AirMode_Load(AirModeUpper);
	}
	else if (AirModeGet() == AirModeLower)
	{

		AirMode_Load(AirModeLower);
	}
	else
	{
		if (AtTabGet(Table_Air)->CurAirTab != para1)
		{
			AirMode_Load(para1);
		}
	}
}

/*****************************************************************************************************
 * 描述：充气初始化
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void AirInit(void)
{
	memset(&AirValve, 0, sizeof(AirInfo_S));
	//	AirIntensitySet(Air_Intensity_default);
	memset(&AirT_Work, 0, sizeof(AirT_Work_S));
}

/*****************************************************************************************************
 * 描述：设置气囊工作模式   手动模式
 * 参数：mode：0：气囊不工作 1：上半身模式 2：下半身模式 3：全身模式
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void AirModeSet(uint8_t mode)
{
	u8 temp;
	AirValve.CurMode = mode;
	if (mode == 0) // * 增加 250301
	{
		AirIntensitySet(0);
	}
	else if (AirIntensityGet() == 0) // * 修改 250304
	{
		// AirIntensitySet(Air_Intensity_default);
		temp = UserIntensityGet(); // @ 修改气压切换后的默认强度 260509
		AirIntensitySet(MassModeGet(AutoModeGet())->AirIntensity[temp - 1]);
	}
}

/*****************************************************************************************************
 * 描述：获取气囊工作模式   手动模式
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
uint8_t AirModeGet(void)
{
	return AirValve.CurMode;
}

/*****************************************************************************************************
 * 描述：设置气囊充气强度
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void AirIntensitySet(u8 inten)
{
	if (AirValve.Intensity > 5)
		AirValve.Intensity = Air_Intensity_Max;
	else
		AirValve.Intensity = inten;
}

/*****************************************************************************************************
 * 描述：获取气囊充气强度
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
u8 AirIntensityGet(void)
{
	return AirValve.Intensity;
}

/*****************************************************************************************************
 * 描述：设置气泵工作状态
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void PumpStateSet(u8 sta)
{
	AirValve.PumpState = sta;
}

/*****************************************************************************************************
 * 描述：获取气泵工作状态
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
u8 PumpStateGet(void)
{
	return AirValve.PumpState;
}

/*****************************************************************************************************
 * 描述：获取气阀的工作状态
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
GasPart_t AirPartGet(void)
{
	return AirValve.GasPart;
}

void AirT_100ms_Deal(void) //* 100ms 时基
{
	u8 i;
	u16 temp, temp1, temp2;
	bool airbag_act[AirT_Max] = {0};

	GasPart1_t Air_Part1;
	GasPart2_t Air_Part2;
	Air_Part1.All = 0; //* add 251211
	Air_Part2.All = 0;
	for (i = AirT_SHD; i < AirT_Max; i++)
	{
		if (AirT_Work.RunFlag[i] == 0)
			continue;

		// if (AirT_Work.Time2[i] == 0 || AirT_Work.Time3[i] == 0)
		if (AirT_Work.Time2[i] == 0 && AirT_Work.Time3[i] == 0) // todo 修改 260701
		{
			AirT_Work.RunFlag[i] = 0;
			AirT_Work.RunTime[i] = 0;
			continue;
		}
		temp = AirT_Work.RunTime[i];
		if (temp < AirT_Work.Time1[i])
		{
			airbag_act[i] = 1;
		}
		else
		{
			temp1 = temp - AirT_Work.Time1[i];
			temp2 = AirT_Work.Time2[i] + AirT_Work.Time3[i];
			if ((temp1 % temp2) < AirT_Work.Time2[i])
			{
				airbag_act[i] = 0;
			}
			else
				airbag_act[i] = 1;
		}
		AirT_Work.RunTime[i]++;
	}

	for (i = AirT_SHD; i < AirT_Max; i++)
	{
		switch (i)
		{
		default:
			break;
		case AirT_SHD:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part1.Bit.ShoulderL = 1;
				Air_Part1.Bit.ShoulderR = 1;
			}
			else
			{
				Air_Part1.Bit.ShoulderL = 0;
				Air_Part1.Bit.ShoulderR = 0;
			}
			break;
		case AirT_ArmL:
			if (airbag_act[i] || AirT_Work.KeepFlag[i])
			{
				Air_Part1.Bit.ArmLF = 1;
				Air_Part1.Bit.ArmLB = 1;
			}
			else
			{
				Air_Part1.Bit.ArmLF = 0;
				Air_Part1.Bit.ArmLB = 0;
			}
			break;
		case AirT_ArmR:
			if (airbag_act[i] || AirT_Work.KeepFlag[i])
			{
				Air_Part1.Bit.ArmRF = 1;
				Air_Part1.Bit.ArmRB = 1;
			}
			else
			{
				Air_Part1.Bit.ArmRF = 0;
				Air_Part1.Bit.ArmRB = 0;
			}
			break;
		case AirT_CalfSide:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part2.Bit.CalfSide = 1;
			}
			else
			{
				Air_Part2.Bit.CalfSide = 0;
			}
			break;
		case AirT_FeetSide:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part2.Bit.FeetSide = 1;
			}
			else
			{
				Air_Part2.Bit.FeetSide = 0;
			}
			break;
		case AirT_Ankle:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part2.Bit.Ankle = 1;
			}
			else
			{
				Air_Part2.Bit.Ankle = 0;
			}
			break;
#if (AIR_CLAFBOT)
		case AirT_CalfBot:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part2.Bit.CalfBot = 1;
			}
			else
			{
				Air_Part2.Bit.CalfBot = 0;
			}
			break;
#elif (AIR_SEAT)
		case AirT_Seat:
			if ((airbag_act[i] || AirT_Work.KeepFlag[i]) && AirT_Work.Paradox[i] == 0)
			{
				Air_Part2.Bit.SeatR = 1;
				Air_Part2.Bit.SeatL = 1;
			}
			else
			{
				Air_Part2.Bit.SeatR = 0;
				Air_Part2.Bit.SeatL = 0;
			}
			break;
#endif
		}
	}
	Air_SetGasPart(Air_Part1, Air_Part2); // * 修改 250519
}

u8 Get_Gas_Keep_State(void)
{
	u8 i;
	for (i = AirT_SHD; i < AirT_Max; i++)
	{
		if (AirT_Work.KeepFlag[i])
			return 1;
	}
	return 0;
}

void Gas_Keep_Clear(void) // * 清除保压标志 250910
{
	u8 i;
	for (i = AirT_SHD; i < AirT_Max; i++)
	{
		AirT_Work.KeepFlag[i] = 0;
	}
}

void Gas_Keep_Set(u8 method) // * 设置保压标志 250910
{
	u8 i;
	PumpOff();
	Gas_Keep_Clear();
	for (i = AirT_SHD; i < AirT_Max; i++)
	{
		if (method & (1 << i))
			AirT_Work.KeepFlag[i] = 1;
	}
}

// * 气囊指令解析
// * AIRINTEN SET 251119 气囊强度设置
#define AIRINTENMAXA (20 + AIR_WEAK) // -4*1 <> -4*5
// #define AIRINTENMAXB (10 + AIR_WEAK)
#define AIRINTENMAXB (16 + AIR_WEAK) // todo 修改气压调节范围 260617
// #define AIRINTENMAXA 20 // -4*1 <> -4*5
// #define AIRINTENMAXB 10
static const u8 Air_Value_A[] = {0, 0, 1, 1, 1, 2};
void AirPart_Date_Set(u8 airpart, u8 para0, u8 method, u8 para1, u8 para2, u8 para3)
{
	u8 i;
	u8 airintensity = AirIntensityGet();
	u8 airtemp = airintensity << 2;
	switch (airpart)
	{
	default:
		break;
	case Air_UpDw:
		for (i = AirT_SHD; i < AirT_Max; i++)
		{
			if (method & (1 << i))
			{
				AirT_Work.RunFlag[i] = 1;
				if ((para1 + airtemp) < AIRINTENMAXA)
					AirT_Work.Time1[i] = 0;
				else
					AirT_Work.Time1[i] = para1 + airtemp - AIRINTENMAXA;

				if ((para2 + AIRINTENMAXB) < airtemp)
					AirT_Work.Time2[i] = 0;
				else
					AirT_Work.Time2[i] = para2 + AIRINTENMAXB - airtemp;

				if ((para3 + airtemp) < AIRINTENMAXB)
					AirT_Work.Time3[i] = 0;
				else
					AirT_Work.Time3[i] = para3 + airtemp - AIRINTENMAXB;
				AirT_Work.RunTime[i] = 0;
			}
			else
			{
				AirT_Work.RunFlag[i] = 0;
				AirT_Work.RunTime[i] = 0;
				AirT_Work.Time1[i] = 0;
				AirT_Work.Time2[i] = 0;
				AirT_Work.Time3[i] = 0;
			}
		}
		break;
	case Air_SHD:
	case Air_ArmL:
	case Air_ArmR:
	case Air_CalfS:
	case Air_FeetS:
	case Air_Ankle:
#if (AIR_CLAFBOT)
	case Air_CalfB:
#elif (AIR_SEAT)
	case Air_Seat:
#endif
		switch (method)
		{
		case AirPart_ModeA:
			AirT_Work.RunFlag[airpart - Air_SHD] = 1;
			if ((para1 + airtemp) < AIRINTENMAXA)
				AirT_Work.Time1[airpart - Air_SHD] = 0;
			else
				AirT_Work.Time1[airpart - Air_SHD] = para1 + airtemp - AIRINTENMAXA;
			AirT_Work.Time2[airpart - Air_SHD] = para2 - Air_Value_A[airintensity];
			AirT_Work.Time3[airpart - Air_SHD] = para3 + Air_Value_A[airintensity];
			AirT_Work.RunTime[airpart - Air_SHD] = 0;
			break;
		case AirPart_ModeB:
			AirT_Work.RunFlag[airpart - Air_SHD] = 1;
			if ((para1 + airtemp) < AIRINTENMAXA)
				AirT_Work.Time1[airpart - Air_SHD] = 0;
			else
				AirT_Work.Time1[airpart - Air_SHD] = para1 + airtemp - AIRINTENMAXA;
			if ((para2 + AIRINTENMAXB) < airtemp)
				AirT_Work.Time2[airpart - Air_SHD] = 0;
			else
				AirT_Work.Time2[airpart - Air_SHD] = para2 + AIRINTENMAXB - airtemp;
			if ((para3 + airtemp) < AIRINTENMAXB)
				AirT_Work.Time3[airpart - Air_SHD] = 0;
			else
				AirT_Work.Time3[airpart - Air_SHD] = para3 + airtemp - AIRINTENMAXB;
			AirT_Work.RunTime[airpart - Air_SHD] = 0;
			break;
		case AirPart_ModeC:
			AirT_Work.RunFlag[airpart - Air_SHD] = 1;
			if (para1 < AIRINTENMAXA)
				AirT_Work.Time1[airpart - Air_SHD] = 0;
			else
				AirT_Work.Time1[airpart - Air_SHD] = para1 + airtemp - AIRINTENMAXA;
			AirT_Work.Time2[airpart - Air_SHD] = para2;
			AirT_Work.Time3[airpart - Air_SHD] = para3;
			AirT_Work.RunTime[airpart - Air_SHD] = 0;
			break;
		case AirPart_STOP:
			AirT_Work.RunFlag[airpart - Air_SHD] = 0;
			AirT_Work.RunTime[airpart - Air_SHD] = 0;
			AirT_Work.Time1[airpart - Air_SHD] = 0;
			AirT_Work.Time2[airpart - Air_SHD] = 0;
			AirT_Work.Time3[airpart - Air_SHD] = 0;
			break;

		default:
			break;
		}
	}
}

#include "ArmKey.h"
#include "KeyFunc.h"

RxPro_S RxVar;
ArmKeyRx_S ArmKeyRx;

bool Arm_Buzzer_Flag = 0;
bool Voice_Button_Flag = 0;
bool Voice_Button_Flag_A = 0;

void ArmKeyInit(void)
{
	memset(&RxVar, 0, sizeof(RxPro_S));
}

void ArmKeyDataTx(void)
{
	u8 ArmKeyDataUpdate[sizeof(ArmKeyRx_S)] = {0};

	ArmKeyRx.LedMode = nPowerOn;
	// ArmKeyRx.LedMode = 2;//* 测试 250328
	ArmKeyRx.Ledpara1 = 0;
	ArmKeyRx.Ledpara2 = 0;
	memcpy(&ArmKeyDataUpdate, &ArmKeyRx, sizeof(ArmKeyDataUpdate) / sizeof(ArmKeyDataUpdate)[0]);
	_485_ComTx(ArmKeyAddr, MainAddr, ArmKeyTx_Count, 0x0001, sizeof(ArmKeyDataUpdate) / sizeof(ArmKeyDataUpdate)[0], ArmKeyDataUpdate);
}

/**************************************************************************************************************
 *描述：快捷按键板发送数据的处理
 *参数：无
 *返回值：无
 *注意：无
 ***************************************************************************************************************/
#if defined(A365B_ARMKEY) || defined(USE_S0558)
void ArmKey_Handler(void)
{
	u8 i;
	switch (RxVar.RxBuffer[10])
	{
	case 0:
		Key.Value = Key_NULL;
		Key.Param = 0;
		break;

	case 1:
		Key.Value = Key_Pause;
		Key.Param = 0xff;
		break;

	case 2:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Heat; //* 修改 加热 250122
		Key.Param = 0xff;
		break;

	case 3:
		Key.Value = Key_AutoMode; //* 修改 切换自动模式
		Key.Param = 0xff;		  //* 自动模式+1
		break;

	case 4:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Air; //* 修改 气压模式
		i = AirModeGet() + 1;
		if (i > 3) //* 循环
			i = 0;
		Key.Param = i;
		break;

	case 5:
		if (WorkStateGet() != WorkState_Work) // * 工作状态
		{
			break;
		}
		MoveWhichSet(MoveUp);		 // * 上下机芯都切换成上机芯 250512
		Key.Value = Key_BackArea;	 //* 修改 按摩区域
		i = MassAreaGet(MoveUp) + 1; //* 读取上机芯按摩区域
#if (defined USE_M2) && !(defined A391S_1)
		if (i > 5 || i < 3)
			i = 3;
#else
		if (i > 5)
			i = 1;
#endif
		Key.Param = i;
		break;

	case 6:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效 下机芯无效 250319
		{
			break;
		}
		MoveWhichSet(MoveUp); // * 上下机芯都切换成上机芯 250512
		Key.Value = Key_Wise; //* 修改 手动模式
		i = MassWiseGet(MoveUp);
		switch (i)
		{
		case TZ:
			Key.Param = 2;
			break;
		case RNZ:
			Key.Param = 3;
			break;
		case PD:
			Key.Param = 4;
			break;
		case RPZ:
			Key.Param = 5;
			break;
		case PDB:
			Key.Param = 6;
			break;
		case ZYS:
			Key.Param = 7;
			break;
		case D3RNZ:
			Key.Param = 8;
			break;
		case D3PD:
			Key.Param = 9;
			break;
		case D3RPZ:
			Key.Param = 10;
			break;
		case ZYD:
			Key.Param = 1;
			break;
		default:
			Key.Param = 1;
			break;
		}
		break;

	case 7:
		break;

	case 8:
#ifdef USE_ENCODER_4D
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效 下机芯无效 250319
		{
			break;
		}
		Key.Value = Key_BackStrength;
		Key.Param = 1;
		break;
#endif
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 2;
		TICK_RESET(Key.KeyKeepTick);
		break;

	case 9:
#ifdef USE_ENCODER_4D
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效 下机芯无效 250319
		{
			break;
		}
		Key.Value = Key_BackStrength;
		Key.Param = 2;
		break;
#endif
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 1;
		TICK_RESET(Key.KeyKeepTick);
		break;

	default:
		break;
	}

	Key.State = RxVar.RxBuffer[9];

	KeyFunc();
}
#elif defined A390_ARMKEY
void ArmKey_Handler(void)
{
	switch (RxVar.RxBuffer[10])
	{
	case 0:
		Key.Value = Key_NULL;
		Key.Param = 0;
		break;

	case 1:
		Key.Value = Key_Pause;
		Key.Param = 0xff;
		break;

	case 2:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_Zero;
		Key.Param = 0xff;
		break;

	case 3:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_LegsUpDown;
		Key.Param = 1;
		Buzzer = SOUND_SHOULDER;
		TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
		break;

	case 4:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_LegsUpDown;
		Key.Param = 2;
		Buzzer = SOUND_SHOULDER;
		TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
		break;

	case 5:

#ifdef A390S_8
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Heat; //* 修改 加热 250122
		Key.Param = 0xff;
#else
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.State = RxVar.RxBuffer[9];
		// * 声控 长按 短按 250524
		if (Key.State == 1) //*  按下
		{
			Voice_Button_Flag = 1;
			Voice_Button_Flag_A = 0;
			TICK_RESET(Key.KeyKeepTick);
		}
		else if (Key.State == 2) //* 抬起
		{
			if (Voice_Button_Flag_A) // * 修改 250702
			{
				Voice_Button_Flag_A = 0;
				break;
			}
			Voice_Button_Flag = 0;

			if (nVoiceControlSw)
			{
				Voice_WakeUp();
				Voice_Ack(VC_WakeUp);
				Buzzer = SOUND_ACK;
			}
			else
			{
				Voice_Ack(VC_Invalid);
				Buzzer = SOUND_ALARM;
			}
		}
#endif
		break;

	case 6:
		Key.Value = Key_AutoMode;
		Key.Param = 0xff;
		break;

	case 7:
		break;

	case 9:									   // * 王工修改
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 2;
		TICK_RESET(Key.KeyKeepTick);
		break;

	case 8:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 1;
		TICK_RESET(Key.KeyKeepTick);
		break;

	default:
		break;
	}

	Key.State = RxVar.RxBuffer[9];
	if (Key.State == 2 && Key.Value == Key_LegsUpDown) // * ADD 251124
		Buzzer = SOUND_NULL;
	KeyFunc();
}
#elif defined A395_ARMKEY
void ArmKey_Handler(void)
{
	u8 i;
	switch (RxVar.RxBuffer[10])
	{
	case 0:
		Key.Value = Key_NULL;
		Key.Param = 0;
		break;

	case 5:
		Key.Value = Key_Pause;
		Key.Param = 0xff;
		break;

	case 3:
		if (WorkStateGet() != WorkState_Work) // * 工作状态
		{
			break;
		}
		MoveWhichSet(MoveUp);		 // * 上下机芯都切换成上机芯 250512
		Key.Value = Key_BackArea;	 //* 修改 按摩区域
		i = MassAreaGet(MoveUp) + 1; //* 读取上机芯按摩区域
#if (defined USE_M2)
		if (i > 5 || i < 3)
			i = 3;
#else
		if (i > 5)
			i = 1;
#endif
		Key.Param = i;
		break;

	case 1:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Air; //* 修改 气压模式
		i = AirModeGet() + 1;
		if (i > 3) //* 循环
			i = 0;
		Key.Param = i;
		break;

	case 2:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Heat; //* 修改 加热 250122
		Key.Param = 0xff;
		break;

	case 4:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效 下机芯无效 250319
		{
			break;
		}
		MoveWhichSet(MoveUp); // * 上下机芯都切换成上机芯 250512
		Key.Value = Key_Wise; //* 修改 手动模式
		i = MassWiseGet(MoveUp);

		switch (i)
		{
		case TZ:
			Key.Param = 2;
			break;
		case RNZ:
			Key.Param = 3;
			break;
		case PD:
			Key.Param = 4;
			break;
		case RPZ:
			Key.Param = 5;
			break;
		case PDB:
			Key.Param = 6;
			break;
		case ZYS:
			Key.Param = 7;
			break;
		case D3RNZ:
			Key.Param = 8;
			break;
		case D3PD:
			Key.Param = 9;
			break;
		case D3RPZ:
			Key.Param = 10;
			break;
		case ZYD:
			Key.Param = 1;
			break;
		default:
			Key.Param = 1;
			break;
		}
		break;

	case 6:
		Key.Value = Key_AutoMode;
		Key.Param = 0xff;
		break;

	case 7:
		break;

	case 9:									   // * 王工修改
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 2;
		TICK_RESET(Key.KeyKeepTick);
		break;

	case 8:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.KeyEncoder = 1;
		TICK_RESET(Key.KeyKeepTick);
		break;

	default:
		break;
	}

	Key.State = RxVar.RxBuffer[9];
	KeyFunc();
}
#elif defined A560_ARMKEY
void ArmKey_Handler(void)
{
	u8 i;
	switch (RxVar.RxBuffer[10])
	{
	case 0:
		Key.Value = Key_NULL;
		Key.Param = 0;
		break;

	case 3:
		Key.Value = Key_AutoMode;
		Key.Param = 0xff;
		break;

	case 2:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效 下机芯无效 250319
		{
			break;
		}
		MoveWhichSet(MoveUp); // * 上下机芯都切换成上机芯 250512
		Key.Value = Key_Wise; //* 修改 手动模式
		i = MassWiseGet(MoveUp);
		switch (i)
		{
		case 0:
			Key.Param = 2;
			break;
		case 1:
			Key.Param = 3;
			break;
		case 5:
			Key.Param = 4;
			break;
		case 7:
			Key.Param = 5;
			break;
		case 6:
			Key.Param = 6;
			break;
		case 10:
			Key.Param = 7;
			break;
		case 13:
			Key.Param = 8;
			break;
		case 15:
			Key.Param = 9;
			break;
		case 16:
			Key.Param = 10;
			break;
		case 11:
			Key.Param = 1;
			break;

		default:
			Key.Param = 1;
			break;
		}
		break;

	case 5:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Heat; //* 修改 加热 250122
		Key.Param = 0xff;
		break;

	case 4:
		if (WorkStateGet() != WorkState_Work) // * 工作状态 才有效
		{
			break;
		}
		Key.Value = Key_Air; //* 修改 气压模式
		i = AirModeGet() + 1;
		if (i > 3) //* 循环
			i = 0;
		Key.Param = i;
		break;

	case 7:									   // * 王工修改
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_Lying;
		Key.Param = 2;
		break;

	case 6:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_Lying;
		Key.Param = 1;
		break;

	case 9:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_LegsUpDown;
		Key.Param = 1;
		break;

	case 8:
		if (WorkStateGet() == WorkState_Pause) // * 暂停无效
		{
			break;
		}
		Key.Value = Key_LegsUpDown;
		Key.Param = 2;

		break;

	default:
		break;
	}

	Key.State = RxVar.RxBuffer[9];
	KeyFunc();
}
#endif
/**************************************************************************************************************
 *描述：编码器只会发送状态2，通过延时保持的方式，处理编码器按键值
 *参数：无
 *返回值：无
 *注意：无
 ***************************************************************************************************************/
bool Key_Lying_Up = 0;
bool Key_Lying_Dw = 0;
void KeyEncoderHandler(void)
{
	if (Key.KeyEncoder > 0)
	{
		if (Key.KeyEncoder == 1 && (Key_Lying_Up == 0 || Key_Lying_Dw))
		{
			Key_Lying_Up = 1;
			Key_Lying_Dw = 0; // * ADD 260122
			Key.State = 1;
			Key.Param = 1;
			Key.Value = Key_Lying;
			KeyFunc();
			Key.State = 0;
			Key.Param = 0;
			Key.Value = 0;
			Buzzer = SOUND_SHOULDER;
			TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
		}
		else if (Key.KeyEncoder == 2 && (Key_Lying_Dw == 0 || Key_Lying_Up))
		{
			Key_Lying_Up = 0; // * ADD 260122
			Key_Lying_Dw = 1;
			Key.State = 1;
			Key.Param = 2;
			Key.Value = Key_Lying;
			KeyFunc();
			Key.State = 0;
			Key.Param = 0;
			Key.Value = 0;
			Buzzer = SOUND_SHOULDER;
			TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
		}
	}

	if (Key_Lying_Up || Key_Lying_Dw)
	{
		if (TICK_COUNTER(Key.KeyKeepTick, 1000))
		{
			Key.KeyEncoder = 0;
			Key_Lying_Up = 0;
			Key_Lying_Dw = 0;
			Key.State = 2;
			Key.Param = 1;
			Key.Value = Key_Lying;
			KeyFunc();
			Key.State = 0;
			Key.Param = 0;
			Key.Value = 0;
			Buzzer = SOUND_NULL;
		}
	}

	if (Voice_Button_Flag)
	{
		if (TICK_COUNTER(Key.KeyKeepTick, 3000))
		{
			Voice_Button_Flag = 0;
			Voice_Button_Flag_A = 1;
			Key.Value = Key_Voice;
			Key.Param = 0xff;
			Key.State = 2;
			KeyFunc();
		}
	}
}

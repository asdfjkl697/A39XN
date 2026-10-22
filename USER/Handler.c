#include "Handler.h"
#include "Memory.h"

#define KeyCom_Real_Read COM0.RX_read
#define KeyCom_Real_Write COM0.RX_write
#define KeyCom_GetValue() EUSART0_Read()

#define KeyCom_Head0 0x55
#define KeyCom_Head1 0xAA

KeyRx_t KeyRx;
KeyState_t KeyState;
u8 Debug_Diy_save = 0;

static u8 CRC_CheckSum_L8, CRC_CheckSum_H8;

void HandlerKeyFunc(void);

HandlerKeyMap_t KeyMap[] = {
	{0x01, Key_Power, 0xFF},			/* 开关机 */
	{0x02, Key_Pause, 0xFF},			/* 暂停 */
	{0x03, Key_AutoMode, 0xFF},			/* 自动程序 */
	{0x04, key_AutoLyJump, 0xFF},		/* 自动躺倒跳过 */
	{0x05, Key_ShoulderAdj_Jump, 0xFF}, /* 肩位调节跳过 */
	{0x06, Key_BodyCheck_Jump, 0xFF},	/* 体型检测跳过 */
	{0x07, Key_MassageReset, 0xFF},		/* 复位 */
	{0x08, Key_Ion, 0xFF},				/* 负氧离子开关 */
	{0x09, Key_Timing, 0x02},			/* 时间减 */
	{0x0A, Key_Timing, 0x01},			/* 时间加 */
	{0x0B, Key_Voice, 0xFF},			/* 声控开关 */
	{0x0C, Key_Music, 0xFF},			/* 背景音乐开关 */
	{0x0D, Key_Blue, 0xFF},				/* 蓝牙开关 */
	{0x0E, Key_LegsHeat, 0xFF},			/* 小腿加热开关 */
	{0x0F, Key_BackHeat, 0xFF},			/* 腰背加热开关 */
	{0x10, Key_LegsKnead, 0xFF},		/* 小腿揉搓开关 */
	{0x11, Key_Roll, 0xFF},				/* 足底滚轮开关 */
	{0x12, Key_MasMode, 0xFF},			/* 按摩强度模式选择 */
	{0x13, Key_BackSpeed, 0xFF},		/* 按摩速度 加减*/
	{0x14, Key_UpperAirStrength, 0xFF}, /* 气压强度 加减 */
	{0x15, Key_Shoulder, 0xFF},			/* 肩位置上下移 */
	{0x16, Key_BackWidth, 0xFF},		/* 按摩宽度 加减 */
	{0x17, Key_BackStrength, 0xFF},		/* 4D强度 加减 */
	{0x18, Key_M1_M2, 0xFF},			/* 机芯切换 */
	{0x19, Key_Wise, 0xFF},				/* 手法 1~10 */
	{0x1A, Key_BackArea, 0xFF},			/* 按摩区间 1~5 */
	{0x1B, Key_BackTread, 0xFF},		/* 机械手行走 上行 下行 */
	{0x1C, Key_Air, 0xFF},				/* 气压模式1~3 */
	{0x1D, Key_Zero, 0xFF},				/* 零重力档位 */
	{0x1E, Key_LegsUpDown, 0xFF},		/* 小腿升降 */
	{0x1F, Key_LegsFlex, 0xFF},			/* 小腿伸缩 */
	{0x20, Key_Lying, 0xFF},			/* 联动起座 */
	{0x21, Key_BackUpDown, 0xFF},		/* 靠背升降 */
	{0x22, Key_Memory_Save, 0xFF},		/* 存储1~4 */
	{0x23, Key_Memory_Use, 0xFF},		/* 调用1~4 */
	{0x24, key_Light, 0xFF},			/* LED开关 */
	{0x25, Key_Volume_Set, 0xFF},		/* 音量设置*/
	{0x26, Key_Volume, 0xFF},			/* 音量加减 */
	{0x27, Key_Language, 0xFF},			/* 语音 1~2 */
	{0x28, Key_Health, 0xFF},			/* 体型检测开始、退出 1：开始检测 2：退出检测 */
	{0x29, key_DeviceInf, 0xFF},		/* 产品信息查询 */
	{0x2A, Key_Player, 0xFF},			/* 语音播报开关 */
	{0x2B, Key_TimOutReset, 0xFF},		/* 按摩时间到后椅子自动复位 */
	{0x2C, Key_LC, 0x10},				/* 整机老化 */
	{0x2D, Key_PACK, 0x11},				/* 打包模式 */
	{0x2E, Key_FCT, 0x12},				/* 工程模式 */
	{0x30, key_DevInfOk, 0xFF},			/* 产品信息查询完成 */
	{0x32, key_Error, 0xFF},			/* 产品信息查询完成 */
	{0x33, key_DiyWork, 0xFF},			/* 自定义执行 */
	{0x36, Key_4DIntensity, 0xFF},		/* 4D强度 */
	{0x37, Key_MassSpeed, 0xFF},		/* 速度 */
	{0x38, Key_MassWidth, 0xFF},		/* 宽度 */
	{0x39, Key_AirIntensity, 0xFF},		/* 气压强度 */
	{0x40, key_Recovery, 0xFF},			/* 恢复出产设置 */
	{0x44, Key_SoundWavex, 0xFF},		/* 音波设置 */
	{0x45, Key_SoundWave, 0xFF},		/* 音波加减 */
	{0x46, Key_SoundWaveSW, 0xFF},		/* 音波开关 */
	{0x47, Key_SoundUpDown, 0xFF},		/* 音波加减 */
	{0x48, Key_SoundMusicSW, 0xFF},		/* 音乐开关 */
	{0x49, Key_FootRock, 0xFF},			/* 足底翻转 */
	{0x4D, Key_LowPower, 0xFF},			/* 低功耗 */
	{0x51, Key_FootSwing, 0xFF},		/* 足底摇摆 */
	{0x66, Key_RedLight, 0xFF},			/* 红光 */
	{0x67, Key_MassFinish, 0xFF},		/* 按摩结束选择 */
	{0xB0, key_SOUND_ACK, 0xFF},		/* 应答声 */
};

#define KeyMapNum sizeof(KeyMap) / sizeof(KeyMap[0])
//* 自定义处理
void DiyWork_Deal(u8 *data)
{
	memset(&AirT_Work, 0, sizeof(AirT_Work_S));			 // * add 250930
	memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T)); // * 初始化跟随 250930
	memset(&ASSIGN_Array, 0, ASSIGNum);					 // * 赋值清零 251219

	u8 i, temp;
	temp = data[0] & 0x0f;
	i = Handlekey_to_M1M2(temp);
	MassWiseSet(MoveUp, (ManualWise_E)i);
#ifdef USE_M2
	MassWiseSet(MoveDw, TZ);
#endif
	temp = data[1] & 0x0f;
	AirModeSet((AirMode_E)temp); // * 设置气压模式 待确定
	temp = (data[1] & 0xf0) >> 4;
	MassAreaSet(MoveUp, (MassageArea_E)temp);
	MoveUpState.ManualArea_ID = MassAreaGet(MoveUp);
	if (MassWiseGet(MoveUp) == ZYS)
		temp = 3; //@ ZYS 默认速度
	else
		temp = data[2] & 0x0f;
	MassSpeedSet(MoveUp, temp);
	temp = (data[2] & 0xf0) >> 4;
	if (temp < 1)
		temp = 1;
	MassWidthSet(MoveUp, temp - 1); // * 修改
	temp = data[3] & 0x0f;
	if (temp < 1)
		temp = 1;
	nBackIntensity = temp - 1;			   // * 修改
	MassIntensitySet(nBackIntensity << 1); //* 手动强度 *2
	temp = (data[3] & 0xf0) >> 4;
	if (temp < 1) // * 增加 250626 防止 没有气压
		temp = 1;
	AirIntensitySet(temp);

	if (data[4] & 0x01)
		HeatStateSet(LegHeat, 1, 3);
	else
		HeatStateSet(LegHeat, 0, 0);

	if (data[4] & 0x02)
		HeatStateSet(BackHeat, 1, 3);
	else
		HeatStateSet(BackHeat, 0, 0);

	if (data[4] & 0x04) //* 小腿揉搓
	{
		KneadModeSet(1);
		KneadStateSet(1);
		KneadSpeedSet(1);
	}
	else
	{
		KneadModeSet(0);
		KneadStateSet(0);
		KneadSpeedSet(0);
	}

	temp = (data[4] & 0xf0) >> 4;
	if (temp)
	{
		RollModeSet(1);
		RollStateSet(1);
		RollSpeedSet(temp);
	}
	else
	{
		RollModeSet(0);
		RollStateSet(0);
		RollSpeedSet(0);
	}
}
//* 自定义保存
void DiyWork_Save(u8 *data)
{
	u8 temp;
	KeyPush.FootRollKeyState = 1;
	KeyPush.KeyCalfKnead = 1;

	temp = (data[0] & 0xf0) >> 4;
	if (temp == 1) //* 保存DIY数据
	{
		memcpy(DIY_packet, &data[0], 5);
		Debug_Diy_save = User_Memory_Save(4); // * 不判断 250323
		Handle_Dispaly_Step = 3;
		Buzzer = SOUND_ACK;
	}
	else
	{
		memcpy(&DIY_packet[5], &data[0], 5);
		Debug_Diy_save = User_Memory_Save(5);
		Handle_Dispaly_Step = 3;
		Buzzer = SOUND_ACK;
	}

	if (AutoModeGet() > 0)
	{
		BlueState_Play = 0; // * 关闭背景音乐
		Blue_Play_ID1 = 0;
		Blue_Play_ID2 = 0;
		AT_TableStop(Table_Auto);
		AutoModeSet(0);
		AutoPreModeSet(0);
#ifdef USE_M2
		MassWiseSet(MoveDw, TZ);
#endif
		Manual_Save_AirState = AirModeGet();
		Manual_Save_AirIntensity = AirIntensityGet();
		AirModeSet(AirModeClose);
		memset(&AirT_Work, 0, sizeof(AirT_Work_S));
		TICK_RESET(Manual_Save_tick);
		Manual_Save_Flag = 1;
	}
}

/*****************************************************************************************************
 * 描述：手控器通讯 数据接收
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void KeyCom_Handler(void)
{
	u8 i, temp;

	while (KeyCom_Real_Read != KeyCom_Real_Write)
	{
		u16 CRC_CheckSum;

		// @ 添加边界检查 260810
		if (KeyRx.RxCount >= MaxKeyData - 1)
		{
			KeyRx.RxCount = 0;
			KeyRx.RxState = 0;
			break;
		}

		KeyRx.RxBuffer[KeyRx.RxCount] = EUSART0_Read();
		switch (KeyRx.RxState)
		{
		case 0: // 解析帧头
			if (KeyRx.RxBuffer[KeyRx.RxCount] == KeyCom_Head0)
			{
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			else
			{

				KeyRx.RxCount = 0;
				KeyRx.RxState = 0;
			}
			break;

		case 1: // 解析帧头
			if (KeyRx.RxBuffer[KeyRx.RxCount] == KeyCom_Head1)
			{
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			else
			{

				KeyRx.RxCount = 0;
				KeyRx.RxState = 0;
			}
			break;

		case 2: // 解析目标地址
			if (KeyRx.RxBuffer[KeyRx.RxCount] == MainAddr)
			{
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			else
			{

				KeyRx.RxCount = 0;
				KeyRx.RxState = 0;
			}
			break;

		case 3: // 解析源地址
			if (KeyRx.RxBuffer[KeyRx.RxCount] == HandlerAddr)
			{
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			else
			{

				KeyRx.RxCount = 0;
				KeyRx.RxState = 0;
			}
			break;

		case 4: // 解析序号

			KeyRx.RxOrder = KeyRx.RxBuffer[KeyRx.RxCount];
			KeyRx.RxCount++;
			KeyRx.RxState++;

			break;

		case 5: // 解析命令L8
			//									 if(KeyRx.RxBuffer[KeyRx.RxCount] )  {

			KeyRx.RxDataCmdL8 = KeyRx.RxBuffer[KeyRx.RxCount];
			KeyRx.RxCount++;
			KeyRx.RxState++;
			//									 }
			//										else{
			//
			//												KeyRx.RxCount=0;
			//												KeyRx.RxState=0;
			//
			//										}
			break;

		case 6: // 解析命令H8

			KeyRx.RxDataCmdH8 = KeyRx.RxBuffer[KeyRx.RxCount];
			KeyRx.RxDataCmd = KeyRx.RxDataCmdH8 << 8 | KeyRx.RxDataCmdL8;

			// if (KeyRx.RxDataCmd == 0x0001 || KeyRx.RxDataCmd == 0x0000) //* 新协议 屏蔽
			// {
			// 	KeyRx.RxCount++;
			// 	KeyRx.RxState++;
			// }
			// else
			// {
			// 	KeyRx.RxCount = 0;
			// 	KeyRx.RxState = 0;
			// }

			KeyRx.RxCount++;
			KeyRx.RxState++;

			break;

		case 7: // 解析数据长度L8
			KeyRx.RxDataLenL8 = KeyRx.RxBuffer[KeyRx.RxCount];

			KeyRx.RxCount++;
			KeyRx.RxState++;

			break;

		case 8: // 解析数据长度H8
			KeyRx.RxDataLenH8 = KeyRx.RxBuffer[KeyRx.RxCount];
			KeyRx.RxDataNum = KeyRx.RxDataLenH8 << 8 | KeyRx.RxDataLenL8;
			if (KeyRx.RxDataNum > 0 && KeyRx.RxDataNum < MaxKeyData) //! 1000 查map 有问题 覆盖 LDDGOverLoadFlag 修改成 数组最大长度 MaxKeyData 250724
			{
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			else
			{
				KeyRx.RxCount = 0;
				KeyRx.RxState = 0;
			}

			break;

		case 9: // 解析数据
			if (KeyRx.RxDataNum > 0)
			{
				KeyRx.RxDataNum--;
				KeyRx.RxCount++;
			}
			else
			{
				CRC_CheckSum_L8 = KeyRx.RxBuffer[KeyRx.RxCount]; // CRC校验值 低8位
				KeyRx.RxCount++;
				KeyRx.RxState++;
			}
			break;

		case 10: // CRC校验

			CRC_CheckSum_H8 = KeyRx.RxBuffer[KeyRx.RxCount]; // CRC校验值 高8位
			CRC_CheckSum = CRC_CheckSum_H8 << 8 | CRC_CheckSum_L8;

			if (CRC_CheckSum == CRC16_CCITT_FALSE(KeyRx.RxBuffer, (KeyRx.RxDataLenH8 << 8 | KeyRx.RxDataLenL8) + 9))
			{
				TICK_RESET(Handle_Dispaly_tick); // * 心跳计数清零
				if (KeyRx.RxDataCmd == 0x00D1)	 // * 自定义
				{
					if (nLCState) //* 老化模式 不响应 250401
					{
						Buzzer = SOUND_ALARM;
						Handle_Dispaly_Step = 3; // * 直接完成
						break;
					}
					WorkMinSet(20); // * 设定时间 20分钟 250424
					WorkSecSet(0);
					Memorystate &= 0x0f; // * add 250816 清除存储调用标志
					DiyWork_Deal(&KeyRx.RxBuffer[9]);
					DiyWork_Save(&KeyRx.RxBuffer[9]);
				}
				else if (KeyRx.RxDataCmd == 0x00B1)
				{
					Handle_Dispaly_Step = 4;
#if defined(USE_S0555) || defined(USE_S0551)
					TICK_RESET(Handle_Success_tick); // @ 连接成功计时，给开机动画 260128
#endif
				}
				else if (KeyRx.RxDataCmd == 0x00A4)
				{
					Handle_Dispaly_Step = 3;
				}
				else if (KeyRx.RxDataCmd == 0x00A3)
				{
					Handle_Dispaly_Step = 2;
				}
				else if (KeyRx.RxDataCmd == 0x00A2)
				{
					Handle_Dispaly_Step = 1;
				}
				else if (KeyRx.RxDataCmd == 0x00A1)
				{
					Key.State = KeyRx.RxBuffer[9];
					HandlerKeyFunc();
					// * 增加按键音 小腿伸出有时候没有提示音 250417
					if (Key.Value == Key_Lying || Key.Value == Key_BackLying || Key.Value == Key_LegsUpDown || Key.Value == Key_BackUpDown ||
						Key.Value == Key_BackTread || Key.Value == Key_LegsFlex || Key.Value == Key_FootRock)
					{
						if (Key.State == 1)
						{
							Buzzer = SOUND_SHOULDER;
							TICK_RESET(Buzzer_Shoulder_Tick); // * 增加 250509
						}
						else if (Key.State == 2)
						{
							Buzzer = SOUND_NULL;
						}
					}
					KeyFunc();
				}
			}
			KeyRx.RxState = 0;
			KeyRx.RxCount = 0;
			break;

		default:
			KeyRx.RxState = 0;
			KeyRx.RxCount = 0;
			break;
		}

		if (KeyRx.RxState > 10)
		{
			KeyRx.RxState = 0;
			KeyRx.RxCount = 0;
		}
	}
}

/***************************************************************************************************
 *功能：手控器只传输简单的0 1 2等的按键键值和参数，根据实际需求将手控器传输来的按键键值和按键参数进行映射
 *参数：无
 *返回值：无
 *注意：无
 ****************************************************************************************************/
void HandlerKeyFunc(void)
{
	uint8_t i;

	if (KeyRx.RxBuffer[10] == 0xff) // * 增加 250625
	{
		Key.Value = 0xff;
		return;
	}

	for (i = 0; i < KeyMapNum; i++)
	{
		if (KeyRx.RxBuffer[10] == KeyMap[i].HandlerKey)
		{
			Key.Value = KeyMap[i].MainKey;
			if (KeyMap[i].KeyPara != 0xFF)
			{
				Key.Param = KeyMap[i].KeyPara;
			}
			else
			{
				Key.Param = KeyRx.RxBuffer[11];
			}
		}
	}
}

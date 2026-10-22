/*
 * File:   DIsplay.h
 * Author: Administrator
 *
 * Created on September 9, 2017, 11:24 AM
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "config.h"
#include "main.h"

/*
 * 宏定义 *************************************************************************************************************************************
 */
#define Max_custom_group 2
#define Max_custom_Data 9

	/*
	 * 类型定义 *************************************************************************************************************************************
	 */
	/********************************************************
	 *蜂鸣器工作模式
	 *********************************************************/
	typedef enum
	{
		SOUND_NULL = 0,
		SOUND_ACK,
		SOUND_ALARM,
		SOUND_SHOULDER, // * 需要改名字 250225
		SOUND_KEY,
		SOUND_KJ,
		SOUND_GJ,
		SOUND_DDD,
	} SOUND_MODE;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t MovementPosition_Upper : 4; /* 上机械手位置 */
			uint8_t MovementPosition_Lower : 4; /* 下机械手位置 */
		} Bit;
	} Byte3_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t HandCheck : 4;		  /* 先择设置机械手M1 M2 */
			uint8_t Shoulderposition : 4; /* 肩位置档位 */
		} Bit;
	} Byte4_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t Intensity4D : 4;  /* 4D强度 */
			uint8_t MassageSpeed : 4; /* 按摩速度 */
		} Bit;

	} Byte7_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t AreaValue : 4;	/* 按摩区域 */
			uint8_t WidthValue : 4; /* 按摩宽度 */
		} Bit;

	} Byte8_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t RollState : 4; /* 滚轮速度 */
			uint8_t ZeroState : 4; /* 零重力档位 */
		} Bit;

	} Byte9_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t AirStrength : 4; /* 气囊强度 */
			uint8_t AirMode : 4;	 /* 充气模式 0：关闭 1：上半身 2：下半身 3：全身 */
		} Bit;

	} Byte10_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t BackHeatState : 1; /* 腰背加热开关 */
			uint8_t LegsHeatState : 1; /* 腿部加热开关 */
			uint8_t PJHeatState : 1;   /* 披肩加热开关 */
			uint8_t Res : 5;
		} Bit;
	} Byte11_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t Foot : 1;	  /* 脚部气囊 */
			uint8_t Leg : 1;	  /* 腿部气囊 */
			uint8_t Arm : 1;	  /* 手臂气囊 无效 */
			uint8_t Hip : 1;	  /* 座侧气囊 */
			uint8_t Shoulder : 1; /* 肩部气囊 */
			uint8_t ArmL : 1;	  /* 手臂气囊左 */
			uint8_t ArmR : 1;	  /* 手臂气囊右 */
			uint8_t Res : 1;
		} Bit;
	} Byte12_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;
		struct
		{
			uint8_t LowPower : 1;		 /* 低功耗 */
			uint8_t BlueToothState : 1;	 /* 蓝牙开关 */
			uint8_t VoiceState : 1;		 /* 声控开关 */
			uint8_t LC_State : 1;		 /* 老化模式 */
			uint8_t CalfKnead : 1;		 /* 腿部揉搓 */
			uint8_t Ion : 1;			 /* 负离子开关 */
			uint8_t CalfKneadState : 1;	 /* 腿部揉搓状态 */
			uint8_t nSoundWaveState : 1; // @ add 260324
		} Bit;
	} Byte13_t;

	typedef union
	{
		uint8_t All;
		struct
		{
			uint8_t ChildLock : 1; // 童锁开关
			uint8_t Res_Byte16 : 3;
			uint8_t RedLightGear : 4; // 红光挡位
		} Bit;
	} Byte16_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t Volume : 7;	   /* 音量 */
			uint8_t VoicePlay : 1; /* 语音播报 */
		} Bit;
	} Byte26_t;

	/********************************************************
	 * 手控器显示控制接口   参数字节
	 *********************************************************/
	typedef union
	{
		uint8_t All;

		struct
		{
			uint8_t LedMode : 1;	  /* 设置LED灯 */
			uint8_t BGM : 1;		  /* 背景音乐 */
			uint8_t TimOutReset : 1;  /* 自动复位 */
			uint8_t AnkleRocking : 4; // 足踝摇摆挡位
			uint8_t Res : 1;
		} Bit;
	} Byte27_t;

	/********************************************************
	 * 手控器显示控制接口
	 *********************************************************/
	typedef struct
	{
		uint8_t WorkState;	/* 工作状态 */
		uint8_t Timing_min; /* 按摩时间 分 */
		uint8_t Timing_s;	/* 按摩时间 秒 */

		Byte3_t Byte3; /* 设置机械手M1、M2位置 */
		Byte4_t Byte4; /* 选择机械手以及设置肩位置档位 */

		uint8_t AutoMode;	/* 自动模式 */
		uint8_t SoundValue; /* 声音处理—按键音 */

		Byte7_t Byte7;	 /* 设置按摩强度和速度 */
		Byte8_t Byte8;	 /* 设置按摩区域和宽度 */
		Byte9_t Byte9;	 /* 设置滚轮和零重力档位 */
		Byte10_t Byte10; /* 设置气压强度和模式 */
		Byte11_t Byte11; /* 设置加热部位 */
		Byte12_t Byte12; /* 设置气囊状态 */
		Byte13_t Byte13; /* 设置蓝牙、声控、小腿揉搓、负离子 */

		uint8_t MassageStrength; /* 设置按摩强度模式 0：轻揉 1：舒缓 2：强劲 */
		uint8_t EorValue;		 /* 错误代码 */
		Byte16_t Byte16;
		uint8_t Res_4[3];  // @ 保留字节
		uint8_t WiseValue; /* 设置手法 */
		uint8_t HeartRate; /* 心率 */
		uint8_t Sop2;	   /* 血氧 */
		uint8_t MicLoop;   /* 微循环 */
		uint8_t TiredStat; /* 疲劳等级 */
		uint8_t CountDown; /* 疲劳检测进度条 0-30 */

		Byte26_t Byte26; /* 设置语音播报和音量 */

		Byte27_t Byte27; /* 设置LED开关、背景音乐、时间到自动复位标志 */

		uint8_t Language; /* 设置语言 */
		uint8_t Memory;	  /* 存储状态 */
		uint8_t Byte30;	  // * 工程
		uint8_t Byte31;	  // * 防夹显示  0、1、2、3
#ifdef USE_SOUNDWAVE
		uint8_t SoundNum;
		uint8_t SoundSec;
		uint8_t SoundMin;
#elif defined USE_HEALTH_AUTO_NUM
	uint8_t Byte32;
	uint8_t Byte33;
	uint8_t Byte34;
	uint8_t Health_Auto_Num;
#endif
	} Display_t;

	/*
	 * 对外声明 *************************************************************************************************************************************
	 */
	extern u8 nAllEor;
	extern u8 Buzzer_Ararm;
	extern u8 Buzzer;
	extern u32 Buzzer_Shoulder_Tick;
	extern u8 DeviceInf_Handle_Update, custom_State, custom_Update;
	extern u8 DeviceInf_Error;
	extern u16 custom_buf[Max_custom_group * Max_custom_Data];
	extern u8 Memorystate;
	extern u8 Dispaly_Language;
	extern char device_Model[]; // @ add for fuji ota 260303

	extern u8 Handle_Dispaly_Step;
	extern u32 Handle_Dispaly_tick;
#if defined(USE_S0555) || defined(USE_S0551)
	extern u32 Handle_Success_tick; // @ add 260128
#endif
	extern u8 DIY_packet[10];

#ifdef AutoLying_Control_BackLeg
	extern bool Manual_BackLeg_Flag;
#endif

	void Display_Drive(void);
	void Eor_Update(void);
	void User_custom_Read(void);
	void User_custom_Write(void);
	u8 User_custom_Save(u8 *nbuff, u8 ngroup);
	u8 User_custom_Use(u8 ngroup);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H */

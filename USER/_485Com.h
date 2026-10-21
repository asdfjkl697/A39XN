#ifndef _485COM_H
#define _485COM_H

#include "config.h"
#include <string.h>

#define MainAddr 0x01
#define MoveUpAddr 0x02
#define MoveDwAddr 0x03
#define ArmKeyAddr 0x04
#define LegAddr 0x05
#define HandlerAddr 0x07

#define Max_RxBuffer_Len 256 // @ AI 260928: 50→256, OTA数据包200字节+帧头/校验/帧尾≈211字节, 原50字节溢出冲掉 ota_state 导致0xf4数据帧被丢弃

typedef struct
{
	uint8_t RxState;					// 接收状态
	uint16_t RxDataLen;					// 接收数据数量
	uint16_t RxDataNum;					// 接收数据数量
	uint8_t RxDataLenL8;				// 接收数据长度L8
	uint8_t RxDataLenH8;				// 接收数据长度H8
	uint8_t RxDataCmdL8;				// 接收数据命令L8
	uint8_t RxDataCmdH8;				// 接收数据命令H8
	uint16_t RxDataCmd;					// 接收数据命令
	uint8_t RxDataParm;					// 接收数据参数
	uint8_t RxDataNoPage;				// 接收数据包序号
	uint8_t RxCount;					// 接收计数
	uint8_t RxBuffer[Max_RxBuffer_Len]; // 接收缓存
} RxPro_S;

// 小腿接收数据
typedef struct
{
	// 行走电机
	struct
	{
		uint8_t State : 4; /// 0:停止 1：正转 2：反转  预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度1~5
	} Roll;

	// 揉捏电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度 1~5

	} Knead1;

	// 滚刮电机
	struct
	{

		uint8_t State : 4; // 0:停止 1：正转 2：反转 预留（3；自动1  4自动2  。。。。）
		uint8_t Speed : 4; // 速度 1~5

	} Knead2;

	// 加热
	struct
	{

		uint8_t State : 4; // 0:停止 1：加热  预留（2；自动1  3自动2  。。。。）
		uint8_t Level : 4; // 档位 1~5

	} Heat;

	// 气阀
	union
	{

		u8 Valves;
		struct
		{
			uint8_t Gas1 : 1; //
			uint8_t Gas2 : 1; //
			uint8_t Gas3 : 1; //
			uint8_t Gas4 : 1; //
			uint8_t Gas5 : 1; //
			uint8_t Gas6 : 1; //
			uint8_t Gas7 : 1; //
			uint8_t Gas8 : 1; //
		} Valve;

	} AirValve;

	// 伸缩电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：伸 2：缩 3：伸多少圈 4：缩多少圈 5:指定位置 6：找脚(最大速度)
		uint8_t Speed : 4; // State 1~5 ：速度1~x	 6：找脚补偿量 单位:s
		uint8_t Circle_L8; // 圈数低8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点） 6 无效
		uint8_t Circle_H8; // 圈数高8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点） 6 无效
	} Flex;

	// 翻转电机
	struct
	{
		uint8_t State : 4; // 0:停止 1：缩 2：伸 3：缩多少圈 4：伸多少圈 5:指定位置
		uint8_t Speed : 4; //
		uint8_t Circle_L8; // 圈数低8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点）
		uint8_t Circle_H8; // 圈数高8位 0~x  当State ：3 4 相对位置  ； 5：绝对位置（缩到底零点）
	} Rock;

	struct
	{
		uint8_t State;
	} Led;

} LegDataTx_S;

// 小腿发送数据

typedef struct
{
	// 滚轮电机
	struct
	{
		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作  （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA

	} Roll;

	// 揉搓电机
	struct
	{
		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Knead1;

	// 滚刮电机
	struct
	{
		uint8_t State;		// 0:停止 1：正转中 2：反转中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Knead2;

	// 加热
	struct
	{
		uint8_t State;		// 0:停止 1：加热中 2：加热中 3.完成当前动作   （主板发送获取命令）
		uint8_t Current_L8; // 电流低8位 mA
		uint8_t Current_H8; // 电流高8位 mA
	} Heat;

	// 伸缩电机
	struct
	{
		uint8_t State : 4;	  // 0:停止 1：伸出 2：回缩 3：完成    （主板发送获取命令）
		uint8_t Place : 4;	  // 当前位置 0：有伸出 1：伸到顶 2：缩到底 。。。
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA
	} Flex;

	struct
	{
		uint8_t State : 4;	  // 0:停止 1：往上 2：往下 3：完成    （主板发送获取命令）
		uint8_t Place : 4;	  // 当前位置 0：有伸出 1：往上到最小 2：往下到最大
		uint8_t CurCircle_L8; // 当前圈数 低8位
		uint8_t CurCircle_H8; // 当前圈数 高8位
		uint8_t Current_L8;	  // 电流低8位 mA
		uint8_t Current_H8;	  // 电流高8位 mA
	} Rock;

	// 加热
	struct
	{
		uint8_t Top : 1;	  // 上限位
		uint8_t Bot : 1;	  // 下限位
		uint8_t Foot : 1;	  // 找脚
		uint8_t Touch : 1;	  // 触地
		uint8_t Protect : 1;  // 防夹
		uint8_t Protect2 : 1; //* 侧边防夹
		uint8_t Res : 2;
	} Singal;

	// 32位 每个bit代表其中一个错误
	uint8_t ErrorCode[4];

} LegDataRx_S;

// 扶手按键接收数据

typedef struct
{
	uint8_t LedMode;  // 0：灭 ;  1: 常亮   2：亮 Ledpara1x0.1s 灭 Ledpara2x0.1s
	uint8_t Ledpara1; // 参数1	 	LedMode为2有效
	uint8_t Ledpara2; // 参数2		LedMode为2有效
} ArmKeyRx_S;

// 扶手按键发送数据  200ms一次主动发送

#define _485_Com_Head0 0x55
#define _485_Com_Head1 0xAA

extern u32 MoveUp_Loss_tick;
extern u32 ArmKey_Loss_tick;
extern u32 Leg_Loss_tick;

extern u8 MSG2MoveUp_Count;
extern u8 LegTx_Count;
extern u8 ArmKeyTx_Count;

#ifdef USE_M2
extern u32 MoveDw_Loss_tick;
extern u8 MSG2MoveDw_Count;
#endif

extern u8 RS485_Poll;

extern u16 CRC16_CCITT_FALSE(u8 *puchMsg, u32 usDataLen);
extern void _485_ComRx(void);
extern void _485_ComTx(u8 taraddr, u8 souraddr, u8 count, u16 cmd, u16 len, u8 *data);
extern void RS485_Polling(void);
extern void RS485_Poll_Reset(void);
#endif

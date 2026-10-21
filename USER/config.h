#ifndef CONFIG_H
#define CONFIG_H

#ifndef GD32F30X_XD
#define GD32F30X_XD
#endif

#include "gd32f30x.h"
#include "gd32f30x_it.h"
#include "systick.h"
#include "binary.h"
#include "SysCfg.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// #define USART0_DMA //* 手控器串口 不能通信

#define ENABLE 1
#define DISABLE 0

/* Preferred type aliases — use these in new code */
#define u8 unsigned char
#define u16 unsigned short int
#define u32 unsigned int

/* @deprecated Legacy aliases — use u8/u16/u32 instead */
#define U8 unsigned char
#define U16 unsigned short int
#define U32 unsigned int

/* @deprecated Legacy aliases — use u8/u16/u32 instead */
#define UCHAR unsigned char
#define UINT unsigned short int

/* uint8_t/uint16_t/uint32_t 已由 <stdint.h> 提供 (经 gd32f30x.h -> core_cm4.h 引入),
   切勿用 #define 重定义 —— 会把 typedef 覆盖成宏, 导致跨文件类型冲突 */
typedef struct
{
    const u8 *p;
} Addr_ST;

typedef union
{
    u16 B16;
    u8 B8[2];
} u16_u8_union;

typedef union
{
    struct
    {
        u8 BIT0 : 1;
        u8 BIT1 : 1;
        u8 BIT2 : 1;
        u8 BIT3 : 1;
        u8 BIT4 : 1;
        u8 BIT5 : 1;
        u8 BIT6 : 1;
        u8 BIT7 : 1;
    } StrackBit; //
    u8 Strack;
} UnionBit;

// 继电器状态
extern UnionBit UnionState, UnionState1, UnionState2;
extern UnionBit UnionState_Flash;

// #define nVoiceState UnionState.StrackBit.BIT0
#define nTimOutResetSw UnionState.StrackBit.BIT0 //* 按摩椅复位
#define nBlueState UnionState.StrackBit.BIT1     //* 蓝牙开关
#define nMusicSW UnionState.StrackBit.BIT2       //* 背景音乐
#define nVoicePlayer UnionState.StrackBit.BIT3   //* 语音播报
// * 第四位是儿童锁 替代为低功耗
#define nSoundWaveSW UnionState.StrackBit.BIT4    // @ 音波 add 260310
#define nLigntSw UnionState.StrackBit.BIT5        //* 灯
#define nVoiceControlSw UnionState.StrackBit.BIT6 //* 声控
#define nSoundMusicSW UnionState.StrackBit.BIT7   //* 音乐开关

#define nBluePlay UnionState2.StrackBit.BIT2 // * 手动播放标志位
// #define nAutoUp UnionState2.StrackBit.BIT0
// #define nLedSensor UnionState2.StrackBit.BIT1
#define nPowerOn UnionState2.StrackBit.BIT6
#define nLCState UnionState2.StrackBit.BIT7
// #define nBlueAutoCon UnionState2.StrackBit.BIT2

#define nLowPower UnionState_Flash.StrackBit.BIT0
#define nIonState UnionState_Flash.StrackBit.BIT1 //* 负离子

typedef enum //* 增加 250124
{
    Chinese = 1,
    English,
    Vietnamese,
    German,
    Hungarian,
    Russian,
    Japanese,

} Language_ENUM;

// #define DefaultLanguage 20
// #define DefaultLanguage 0x0f
#ifdef A395SY
#define DefaultLanguage 0x7
#else
#define DefaultLanguage 0x0
#endif

typedef enum
{
    AD_24VPower,
    AD_Pump,
    AD_Leg2DDG,
    AD_BackDDG,
    AD_LegDDG,
    AD_BackHeat,
    AD_LegHeat,

} AD_Type_E;

// * move here 250926
typedef enum
{
    Part_NULL = 0,
    MoveUp_Tread,  // 上机芯行走
    MoveUp_TreadB, // 上机芯行走档位
    MoveUp_Knead,  // 上机芯揉捏
    MoveUp_Tap,    // 上机芯拍打
    MoveUp_Flex,   // 上机芯伸缩
    MoveDw_Tread,  // 下机芯行走
    MoveDw_TreadB, // 下机芯行走档位
    MoveDw_Knead,  // 下机芯揉捏
    MoveDw_Tap,    // 下机芯拍打
    MoveDw_Flex,
    MoveDw_TreadF, // 下机芯行走跟随
    MoveDw_KneadF, // 下机芯揉捏复制
    MoveDw_TapF,   // 下机芯拍打复制
    MoveDw_FlexF,
    Air_Refresh,
    Air_Keep,  // 保压
    Air_UpDw,  // 上+下
    Air_SHD,   // 肩位
    Air_ArmL,  // 左手
    Air_ArmR,  // 右手
    Air_CalfS, // 腿侧
    Air_FeetS, // 脚
    Air_Ankle, // 脚后跟
#if (AIR_CLAFBOT)
    Air_CalfB, // @ add 260320
#elif (AIR_SEAT)
    Air_Seat, // @ add 260518
#endif
    Motor_Back, // 靠背电机
    Motor_Leg,  // 小腿升降电机
    Motor_Flex, // 小腿伸缩电机
    Motor_Calf, // 小腿滚刮揉搓
    Motor_Feet, // 足底滚轮
    Motor_Zero, // 角度
    Motor_Sway, // @ 摇摆
    Motor_Rock, // @ 翻转小腿
    Part_Max,
    Part_SUB2 = 0xA2,   //* 调用二级子表
    Part_AIR = 0xA3,    //* 调用气压动作表
    Part_MVDW = 0xA4,   //* 调用下机芯表
    Part_SUB = 0xA5,    //* 调用子表
    Part_STOP = 0xA6,   //* 结束子表 配合Part_SUB使用
    Part_LOOP = 0xA8,   //* 循环
    Part_BREAK = 0xA9,  //* 退出循环 配合Part_LOOP使用
    Part_ASSIGN = 0xAA, //* 赋值 用于进程间交互 配合Part_IF使用
    Part_IF = 0xAB,     //* 条件执行
    Part_EXEC = 0xAC,   //* 执行
} Part_enum;

static inline u16 Get_Pump_Current(void)
{
    u16 ad_val = AD_Get_Value[AD_Pump];
    return (ad_val <= 4095) ? ad_val : 0;
}
static inline u16 Get_BackDDG_Current(void)
{
    u16 ad_val = AD_Get_Value[AD_BackDDG];
    return (ad_val <= 4095) ? ad_val : 0;
}
static inline u16 Get_LegDDG_Current(void)
{
    u16 ad_val = AD_Get_Value[AD_LegDDG];
    return (ad_val <= 4095) ? ad_val : 0;
}
static inline u16 Get_BackHeat_Current(void)
{
    u16 ad_val = AD_Get_Value[AD_BackHeat];
    return (ad_val <= 4095) ? ad_val : 0;
}
static inline u16 Get_LegHeat_Current(void)
{
    u16 ad_val = AD_Get_Value[AD_LegHeat];
    return (ad_val <= 4095) ? ad_val : 0;
}

#define Leg2DDG_Current AD_Get_Value[AD_Leg2DDG]
#define _24VPower AD_Get_Value[AD_24VPower]
// #define Pump_Current AD_Get_Value[AD_Pump]
// #define BackDDG_Current AD_Get_Value[AD_BackDDG]
// #define LegDDG_Current AD_Get_Value[AD_LegDDG]
// #define BackHeat_Current AD_Get_Value[AD_BackHeat]
// #define LegHeat_Current AD_Get_Value[AD_LegHeat]
#define Pump_Current Get_Pump_Current()
#define BackDDG_Current Get_BackDDG_Current()
#define LegDDG_Current Get_LegDDG_Current()
#define BackHeat_Current Get_BackHeat_Current()
#define LegHeat_Current Get_LegHeat_Current()

#define TICK_GET GetTick
#define TICK_RESET(x) ((x) = GetTick())
#define TICK_COUNTER(x, y) ((GetTick() - (x)) >= (y))

uint32_t GetTick(void);

typedef enum
{
    P0 = 0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7,
    P8,
    P9,
} PublicParam_E;

#define VALI 0xFF

typedef struct
{
    uint8_t TX_read;
    uint8_t TX_write;
    uint8_t RX_read;
    uint8_t RX_write;
    uint8_t B_TX_busy;
    uint8_t Rx_OverTime;

} COMx_Define;

#define COM_TX0_Lenth 100
#define COM_RX0_Lenth 100

extern volatile COMx_Define COM0;
extern u8 TX0_Buffer[COM_TX0_Lenth];
extern u8 RX0_Buffer[COM_RX0_Lenth];

uint8_t EUSART0_Read(void);
#ifdef USART0_DMA
void EUSART0_Write(uint8_t *data_buf, uint32_t length);
#else
void EUSART0_Write(uint8_t dat);
#endif

#define COM_TX1_Lenth 200
#define COM_RX1_Lenth 200

extern volatile COMx_Define COM1;
extern u8 TX1_Buffer[COM_TX1_Lenth];
extern u8 RX1_Buffer[COM_RX1_Lenth];

uint8_t EUSART1_Read(void);
#ifdef USART1_DMA
void EUSART1_Write(uint8_t *data_buf, uint32_t length);
#else
void EUSART1_Write(uint8_t dat);
#endif

#define COM_TX2_Lenth 200
#define COM_RX2_Lenth 254 // todo 修改 50-254

extern volatile COMx_Define COM2;
extern u8 TX2_Buffer[COM_TX2_Lenth];
extern u8 RX2_Buffer[COM_RX2_Lenth];
extern u8 tx2_checksum;
void PrintString2(char *puts);
uint8_t EUSART2_Read(void);
#ifdef USART2_DMA
void EUSART2_Write(uint8_t *data_buf, uint32_t length);
#else
void EUSART2_Write(uint8_t dat);
#endif

#define COM_TX3_Lenth 20
#define COM_RX3_Lenth 20

extern volatile COMx_Define COM3;
extern u8 TX3_Buffer[COM_TX3_Lenth];
extern u8 RX3_Buffer[COM_RX3_Lenth];

uint8_t EUART3_Read(void);
#ifdef UART3_DMA
void EUART3_Write(uint8_t *data_buf, uint32_t length);
#else
void EUART3_Write(uint8_t dat);
#endif

#define COM_TX4_Lenth 100
#define COM_RX4_Lenth 100

extern volatile COMx_Define COM4;
extern u8 TX4_Buffer[COM_TX4_Lenth];
extern u8 RX4_Buffer[COM_RX4_Lenth];

uint8_t EUART4_Read(void);
void EUART4_Write(uint8_t dat);

#define USART0_BAUD 57600
#define USART1_BAUD 38400
#define USART2_BAUD 115200
#define UART3_BAUD 19200
#define UART4_BAUD 115200

void UserConfig(void);

extern uint16_t ADC0_Buffer[];

#ifndef LONG_TO_BIN
#define LONG_TO_BIN(n)       \
    (                        \
        ((n >> 21) & 0x80) | \
        ((n >> 18) & 0x40) | \
        ((n >> 15) & 0x20) | \
        ((n >> 12) & 0x10) | \
        ((n >> 9) & 0x08) |  \
        ((n >> 6) & 0x04) |  \
        ((n >> 3) & 0x02) |  \
        ((n) & 0x01))

#define BIN(n) LONG_TO_BIN(0x##n##l)
#define B(n) BIN(n)
#define b(n) BIN(n)
#endif

/**********************OS阻塞任务定义************************/
typedef enum
{
    TASK_ID_BLUE,
    TASK_ID_VOICE,
#if (USE_Health_Check)
    TASK_ID_HEALTH,
#endif // USE_Health_Check
    TASK_ID_APP,
    MAX_OS_TASK
} OS_TASK_ID;

extern u16 OS_RTC_TICK;
extern u16 OS_Pt_Tick[MAX_OS_TASK];
extern u16 OS_Pt[MAX_OS_TASK];

// 任务开头
#define Pt_Start             \
    switch (OS_Pt[nTask_ID]) \
    {                        \
    case 0:                  \
        return;              \
    case 1:

// 任务结尾
#define Pt_End  \
    default:    \
        return; \
        }

// 任务等待条件x,x为真处于等待状态,x为假等待结束
#define Pt_Wait_Sig(x)            \
    OS_Pt[nTask_ID] = (__LINE__); \
    case (__LINE__):              \
        if (x)                    \
        return

// 任务等待x个心跳,心跳单位5ms, x取值1~65535
#define Pt_Wait_Tick(x)                            \
    OS_Pt[nTask_ID] = (__LINE__);                  \
    OS_Pt_Tick[nTask_ID] = OS_RTC_TICK + (x);      \
    case (__LINE__):                               \
        if ((OS_RTC_TICK != OS_Pt_Tick[nTask_ID])) \
        return

// 任务等待条件x或者y个心跳
#define Pt_Wait_SigOrTick(x, y)                           \
    OS_Pt[nTask_ID] = (__LINE__);                         \
    OS_Pt_Tick[nTask_ID] = OS_RTC_TICK + (y);             \
    case (__LINE__):                                      \
        if ((x) && (OS_RTC_TICK != OS_Pt_Tick[nTask_ID])) \
        return

// 暂停当前任务
#define Pt_Pause()       \
    OS_Pt[nTask_ID] = 0; \
    return

// 复位并运行当前任务
#define Pt_Reset()       \
    OS_Pt[nTask_ID] = 1; \
    return

#define Pt_Pause_ID(x) OS_Pt[x] = 0 // 暂停任务(x)

#define Pt_Reset_ID(x) OS_Pt[x] = 1 /* 复位并运行任务(x) */

#endif

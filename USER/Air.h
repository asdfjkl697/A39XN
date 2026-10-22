#ifndef AIR_H
#define AIR_H

#include "config.h"
// #include "AT.h"
#include <string.h>

/****************DRV8804***************/

#define VALVE_DAT_SET gpio_bit_set(GPIOE, GPIO_PIN_7) // 数据
#define VALVE_DAT_RESET gpio_bit_reset(GPIOE, GPIO_PIN_7)
#define VALVE_CLK_SET gpio_bit_set(GPIOE, GPIO_PIN_8) // 时钟
#define VALVE_CLK_RESET gpio_bit_reset(GPIOE, GPIO_PIN_8)
#define VALVE_LATCH_SET gpio_bit_set(GPIOE, GPIO_PIN_9) // 锁存
#define VALVE_LATCH_RESET gpio_bit_reset(GPIOE, GPIO_PIN_9)

/****************DRV8840***************/
#define PUMPCircle_Fault() gpio_input_bit_get(GPIOB, GPIO_PIN_14) // * 气泵故障 250526

#define PUMP_DECAY_HIGH gpio_bit_set(GPIOB, GPIO_PIN_13)
#define PUMP_DECAY_LOW gpio_bit_reset(GPIOB, GPIO_PIN_13)

#define PUMP_PHASE_HIGH gpio_bit_set(GPIOB, GPIO_PIN_12)
#define PUMP_PHASE_LOW gpio_bit_reset(GPIOB, GPIO_PIN_12)

#define PUMP_RESET_HIGH gpio_bit_set(GPIOB, GPIO_PIN_15)
#define PUMP_RESET_LOW gpio_bit_reset(GPIOB, GPIO_PIN_15)

#define Air_Intensity_default 3
#define Air_Intensity_Max 5

#define PumpOn(x)    \
    PUMP_RESET_HIGH; \
    PUMP_DECAY_LOW;  \
    PUMP_PHASE_HIGH; \
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, x)

#define PumpOff()    \
    PUMP_DECAY_HIGH; \
    timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, 0)

// V12 0x1000 V11 0x2000  V10 0x4000 V9 0x8000
// V8  0x0100 V7 0x0200  V6 0x0400 V5 0x0800
// V4 0x0010 V11 0x0020  V10 0x0040 V1 0x0080

#define ArmLeftVal 0x0800
#define ArmRightVal 0x0400
#define ArmVal 0x0C00

#define ShoulderVal 0x0200

#define LegLeftVal 0x0001
#define LegRightVal 0x0002
#define LegVal 0x0003

#define FootSideVal 0x0004
#define FootFrontVal 0x0008
#define FootVal 0x000C

#define HipLeftVal 0x0010
#define HipRightVal 0x0020
#define HipVal 0x0030

typedef enum
{
    AirPart_STOP,
    AirPart_RUN,
    AirPart_MANUAL,
    AirPart_PAUSE,
    AirPart_CONT,
    AirPart_ModeA,
    AirPart_ModeB,
    AirPart_ModeC,
    AirPart_END
} AirPartCmd;

typedef enum
{
    AirT_SHD = Air_SHD - Air_SHD, // 肩位
    AirT_ArmL = Air_ArmL - Air_SHD,
    AirT_ArmR = Air_ArmR - Air_SHD,
    AirT_CalfSide = Air_CalfS - Air_SHD,
    AirT_FeetSide = Air_FeetS - Air_SHD,
    AirT_Ankle = Air_Ankle - Air_SHD, // 脚后跟
#if (AIR_CLAFBOT)
    AirT_CalfBot = Air_CalfB - Air_SHD,
#elif (AIR_SEAT)
    AirT_Seat = Air_Seat - Air_SHD,
#endif
    AirT_Max,
} AirT_enum;

typedef struct
{
    u8 Time1[AirT_Max];      // 预先充气
    u8 Time2[AirT_Max];      // 释放
    u8 Time3[AirT_Max];      // 充气
    bool RunFlag[AirT_Max];  // 启动标志
    bool KeepFlag[AirT_Max]; // 保压标志
    bool PreFlag[AirT_Max];  // 预先充气标志
    bool Fault[AirT_Max];    //* Fault > Manual > Paradox > Auto
    bool Manual[AirT_Max];
    bool Paradox[AirT_Max];
    u16 RunTime[AirT_Max]; // 运行时间
} AirT_Work_S;

extern AirT_Work_S AirT_Work;

typedef enum
{
    AS,  // 肩部
    AL1, // 左手1
    AL2, // 左手2
    AR1, // 右手1
    AR2, // 右手2
    TL,  // 左臀
    TR,  // 右臀
    VD1, // 小腿1
    VD2, // 小腿2
    VE,  // 脚侧
    VG,  // 脚后跟
    VEG, // 脚
    AIRMAX

} AirPart_E;

/********************************************************************************
 * 类型定义 : 充气部位1
 ********************************************************************************/
typedef union
{
    uint8_t All;
    struct
    {
        uint8_t Res1 : 1;      /* 保留 */
        uint8_t Res2 : 1;      /* 保留 */
        uint8_t ArmRB : 1;     /* 右手后 */
        uint8_t ArmRF : 1;     /* 右手前 */
        uint8_t ArmLB : 1;     /* 左手后 */
        uint8_t ArmLF : 1;     /* 左手前 */
        uint8_t ShoulderR : 1; /* 肩右 */
        uint8_t ShoulderL : 1; /* 肩左 */
    } Bit;

} GasPart1_t;

/********************************************************************************
 * 类型定义 : 充气部位2
 ********************************************************************************/
typedef union
{
    uint8_t All;
    struct
    {
        uint8_t Res1 : 1;
        uint8_t Ankle : 1;    /* 脚后跟 */
        uint8_t FeetF : 1;    /* 脚前 */
        uint8_t FeetSide : 1; /* 脚侧 */
        uint8_t CalfBot : 1;  /* 腿肚 */
        uint8_t CalfSide : 1; /* 腿侧 */
        uint8_t SeatR : 1;    /* 右臀 */
        uint8_t SeatL : 1;    /* 左臀 */
    } Bit;
} GasPart2_t;

/********************************************************************************
 * 类型定义 : 充气部位
 ********************************************************************************/
typedef struct
{
    GasPart1_t Part1;
    GasPart2_t Part2;
} GasPart_t;

typedef enum
{

    All_off,

} AirType_E;

typedef enum
{
    AirModeClose,
    AirModeUpper,
    AirModeLower,
    AirModeFull,
    AirModeAuto,
} AirMode_E;

typedef struct
{

    u8 PumpState;            // 气泵状态
    u8 PumpSpeed;            // 充气速度
    u8 Intensity;            // 气压强度
    u8 CurMode;              // 当前模式
    u16 WorkTimeCnt[AIRMAX]; // 工作时间计数
    u16 KeepTimeCnt[AIRMAX]; // 保压时间计数

    GasPart_t GasPart; /* 充气部位 */

} AirInfo_S;

extern AirInfo_S AirValve;

void DRV8804_OUT(u16 dat);

void AirModeSet(uint8_t mode);
uint8_t AirModeGet(void);
void AirIntensitySet(u8 inten);
extern u8 AirIntensityGet(void);

void PumpStateSet(u8 sta);
u8 PumpStateGet(void);

void Air_SetGasPart(GasPart1_t AirPart1, GasPart2_t AirPart2);
void AirLogic_Process(void);
void AirInit(void);

GasPart_t AirPartGet(void);

void Gas_CloseAll(void);

void Air_ActionHandle(UnionBit Part1, UnionBit Part2);
extern void Air_All_Change(u8 para1);

extern void Gas_Clear(void);

extern void Gas_Keep_Set(u8 method);
extern void AirT_100ms_Deal(void);
extern void AirPart_Del(u8 airpart);
#endif

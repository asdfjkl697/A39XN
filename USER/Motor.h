
#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"
#include "WorkState.h"
#include "Power.h"

#define DDG_Type_Signal1 // 有伸缩信号和圈数信号的电动杆

#ifdef DDG_Type_Signal1
#ifdef BDDG_Leng_110
#define DDG_BCircle_Max (u32)110
#else
#define DDG_BCircle_Max (u32)146
#endif
#define DDG_LCircle_Max (u32)66

#elif defined(DDG_Type_NoSignal)

#ifdef DEBUG

#endif // DEBUG

#ifdef Track_Open_A350
#define DDG_BCircle_Max 52
#define DDG_LCircle_Max 36
#elif defined(Track_LS)
#define DDG_BCircle_Max 82
#define DDG_LCircle_Max 36
#endif // DEBUG

#define BDDG_MaxCircle DDG_BCircle_Max * 100
#define LDDG_MaxCircle DDG_BCircle_Max * 100

#endif // DDG_Type

#define DDG_BCircle_Zero0 0
#define DDG_BCircle_Zero1 50
#define DDG_BCircle_Zero2 70 // @ 扶手水平 90-85
#define DDG_BCircle_Zero3 99 // todo 修改 90-99 260928
#define DDG_BCircle_DB 98

#define DDG_LCircle_Zero0 0
#define DDG_LCircle_Zero1 85
#define DDG_LCircle_Zero2 93 //* 250915 90->93
#define DDG_LCircle_Zero3 99 //* 250915 95->98->99 260928
#define DDG_LCircle_DB 0

// 全身拉伸位置
// #define DDG_BCircle_ST0 20 // 靠背起始%
#define DDG_BCircle_ST0 50 // 靠背起始% //* 30->50 250214
#define DDG_LCircle_ST0 95 // 小腿起始%  //* 100->95 250214

#define DDG_BCircle_ST1 100 // 靠背拉伸%
#define DDG_LCircle_ST1 30	// 小腿拉伸%

// 小腿拉伸位置
#define DDG_BCircle_ST2 100 // 靠背起始%
#define DDG_LCircle_ST2 100 // 小腿起始%

#define DDG_BCircle_ST3 100 // 靠背拉伸%
#define DDG_LCircle_ST3 90	// 小腿拉伸%

#define DDG_BCircle_ST4 95 // 靠背拉伸%
#define DDG_LCircle_ST4 50 // 小腿拉伸%

#define DDG_BCircle_ST5 100 // 靠背拉伸%
#define DDG_LCircle_ST5 0	// 小腿拉伸%

#define DDG_BCircle_ST6 50 // 靠背拉伸%
#define DDG_LCircle_ST6 85 // 小腿拉伸%

#define DDG_BCircle_ST7 70 // 靠背拉伸%
#define DDG_LCircle_ST7 93 // 小腿拉伸%

#define DDG_BCircle_ST8 90 // 靠背拉伸%
#define DDG_LCircle_ST8 98 // 小腿拉伸%

#define DDG_BCircle_UpDown 33 //%
#define DDG_LCircle_UpDown 33 //%

	//--------------------电机

	extern void Motor_Init(void);
	extern void Motor_Func_Update(void);

#define Max_ZeroRank 14 // * 修改 11-14 250610
#define Max_Zero 4		// 前面4个为零重力挡位
						// #define Max_Zero 8 //* 测试

	typedef enum
	{
		AUTO_SIT_NONE = 0,
		AUTO_SIT_ZERO1,
		AUTO_SIT_ZERO2,
		AUTO_SIT_ZERO3,
		AUTO_SIT_STRETCH0,
		AUTO_SIT_STRETCH1,
		AUTO_SIT_STRETCH2,
		AUTO_SIT_STRETCH3,
		AUTO_SIT_STRETCH4,
		AUTO_SIT_STRETCH5,
		AUTO_SIT_ZERO1N,
		AUTO_SIT_ZERO2N,
		AUTO_SIT_ZERO3N,
		AUTO_SIT_DB,
	} ZeroRank_enum;

#define AUTO_ZERO_POS0 AUTO_SIT_NONE
#define AUTO_ZERO_POS1 AUTO_SIT_ZERO1
#define AUTO_ZERO_POS2 AUTO_SIT_ZERO2
#define AUTO_ZERO_POS3 AUTO_SIT_ZERO3

#define AUTO_STRETCH_POS0 AUTO_SIT_STRETCH0
#define AUTO_STRETCH_POS1 AUTO_SIT_STRETCH1
#define AUTO_STRETCH_POS2 AUTO_SIT_STRETCH2
#define AUTO_STRETCH_POS3 AUTO_SIT_STRETCH3

	//---------------------------------------------------------------------------------------电动杠

#define DDG_LC_TIM 3 // min
#define DDG_Speed 255
#define BDDG_Speed 255						  // * 测试
#define LDDG_Speed 90						  // * 测试 250612 加速 80-100-90 251023
#define DDG_OutCurrent ((u32)50 * 4095 / 330) // x(0.1A)*4095/330

#define DDG_StopCurrent 8

#define DDG_Soft_Time1 50  // 缓停1
#define DDG_Soft_Time2 80  // 缓停2
#define DDG_Stop_Time 100  // 5ms  放电时间
#define DDG_Stop_Time1 110 // 5ms  总时间

#define DDG_Tread_OutTime 200 * 90 // 5ms
#define DDG_Circle_OutTime 200 * 5 // 5ms

#define BDDGCircle_Fault() gpio_input_bit_get(GPIOA, GPIO_PIN_15) // * 靠背电动杆故障
#define LDDGCircle_Fault() gpio_input_bit_get(GPIOD, GPIO_PIN_14) // * 小腿电动杆故障 250322

#define BDDG_DECAY_HIGH gpio_bit_set(GPIOA, GPIO_PIN_11)
#define BDDG_DECAY_LOW gpio_bit_reset(GPIOA, GPIO_PIN_11)

#define BDDG_PHASE_HIGH gpio_bit_set(GPIOA, GPIO_PIN_12)
#define BDDG_PHASE_LOW gpio_bit_reset(GPIOA, GPIO_PIN_12)

#define BDDG_RESET_HIGH gpio_bit_set(GPIOD, GPIO_PIN_3)
#define BDDG_RESET_LOW gpio_bit_reset(GPIOD, GPIO_PIN_3)

#define LDDG_DECAY_HIGH gpio_bit_set(GPIOD, GPIO_PIN_13)
#define LDDG_DECAY_LOW gpio_bit_reset(GPIOD, GPIO_PIN_13)

#define LDDG_PHASE_HIGH gpio_bit_set(GPIOD, GPIO_PIN_12)
#define LDDG_PHASE_LOW gpio_bit_reset(GPIOD, GPIO_PIN_12)

#define LDDG_RESET_HIGH gpio_bit_set(GPIOD, GPIO_PIN_15)
#define LDDG_RESET_LOW gpio_bit_reset(GPIOD, GPIO_PIN_15)

#define LDDG2_DECAY_HIGH gpio_bit_set(GPIOD, GPIO_PIN_9)
#define LDDG2_DECAY_LOW gpio_bit_reset(GPIOD, GPIO_PIN_9)

#define LDDG2_PHASE_HIGH gpio_bit_set(GPIOD, GPIO_PIN_8)
#define LDDG2_PHASE_LOW gpio_bit_reset(GPIOD, GPIO_PIN_8)

#define LDDG2_RESET_HIGH gpio_bit_set(GPIOD, GPIO_PIN_11)
#define LDDG2_RESET_LOW gpio_bit_reset(GPIOD, GPIO_PIN_11)

#define BDDG_PWM_Enable(x) timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, x); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, x)
#define BDDG_PWM_Disable() timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, 0); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, 0)

#ifdef DDG_Type_Signal1

#define BDDG_Dir_Set_Down() \
	BDDG_RESET_HIGH;        \
	BDDG_DECAY_LOW;         \
	BDDG_PHASE_HIGH

#define BDDG_Dir_Set_Up() \
	BDDG_RESET_HIGH;      \
	BDDG_DECAY_LOW;       \
	BDDG_PHASE_LOW

#define BDDG_Dir_Set_Stop() \
	BDDG_DECAY_HIGH;

#elif defined(DDG_Type_NoSignal)

#define BDDG_Dir_Set_Down()             \
	gpio_bits_set(GPIOB, GPIO_PINS_13); \
	gpio_bits_reset(GPIOB, GPIO_PINS_12)

#define BDDG_Dir_Set_Up()               \
	gpio_bits_set(GPIOB, GPIO_PINS_12); \
	gpio_bits_reset(GPIOB, GPIO_PINS_13)

#define BDDG_Dir_Set_Stop()               \
	gpio_bits_reset(GPIOB, GPIO_PINS_12); \
	gpio_bits_reset(GPIOB, GPIO_PINS_13)

#endif

#define LDDG_PWM_Enable(x) timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_1, x); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, x)

#define LDDG_PWM_Disable() timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_1, 0); // tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 0)

#ifdef DDG_Type_Signal1

#define LDDG_Dir_Set_Down() \
	LDDG_RESET_HIGH;        \
	LDDG_DECAY_LOW;         \
	LDDG_PHASE_HIGH;

#define LDDG_Dir_Set_Up() \
	LDDG_RESET_HIGH;      \
	LDDG_DECAY_LOW;       \
	LDDG_PHASE_LOW;

#define LDDG_Dir_Set_Stop() \
	LDDG_DECAY_HIGH  // todo LDDG2_DECAY_HIGH->LDDG_DECAY_HIGH 261015

#elif defined(DDG_Type_NoSignal)

#define LDDG_Dir_Set_Down()            \
	gpio_bits_set(GPIOF, GPIO_PINS_6); \
	gpio_bits_reset(GPIOF, GPIO_PINS_7)

#define LDDG_Dir_Set_Up()              \
	gpio_bits_set(GPIOF, GPIO_PINS_7); \
	gpio_bits_reset(GPIOF, GPIO_PINS_6)

#define LDDG_Dir_Set_Stop()              \
	gpio_bits_reset(GPIOF, GPIO_PINS_7); \
	gpio_bits_reset(GPIOF, GPIO_PINS_6)

#endif

	typedef struct
	{
		U8 UserState;
		U8 nCurUserState;

		U8 LMotorState;
		U8 LNextMotorState;
		U16 LStopTime;

		U8 LSignal;
		U8 LSignalInit; // 信号是否初始化判断标志。
		U8 LPlace;
		U16 LnCurCircle; //
		U16 LCircleSet;
		U16 LnLocalCircle;
		U16 LnCurSpeed;
		U16 LnDelay;

		U8 BMotorState;
		U8 BNextMotorState;
		U16 BStopTime;

		U8 BSignal;
		U8 BSignalInit; // 信号是否初始化判断标志。
		U8 BPlace;
		U16 BnCurCircle; //
		U16 BCircleSet;
		U16 BnLocalCircle;
		U16 BnCurSpeed;
		U16 BnDelay;

		U8 BEor;
		U8 LEor;

		U16 BSignalOutTime;
		U16 BCircleOutTime;
		U16 LSignalOutTime;
		U16 LCircleOutTime;

		// u8 B_No_Tick;
		// u8 B_OutCurrent_Tick;
		// u8 B_MissCurrent_Tick;

		// u8 L_No_Tick;
		// u8 L_OutCurrent_Tick;
		// u8 L_MissCurrent_Tick;

		u8 DDG_Pt;

		u8 LDir;
		u8 BDir;

		u8 LOverLoadCnt;
		u8 BOverLoadCnt;
		u8 LOverLoadTick;
		u8 BOverLoadTick;

		U8 DDGAutoControl; /* 自动程序中控制DDG进行动作 */
	} DDGMotor_Struct;

	typedef enum
	{
		DDG_UserState_Stop = 0,
		DDG_UserState_BackUp,
		DDG_UserState_BackDown,
		DDG_UserState_LegUp,
		DDG_UserState_LegDown,
		DDG_UserState_SitUp,
		DDG_UserState_SitDown,
		DDG_UserState_SitUp_Circle,
		DDG_UserState_SitDown_Circle,
		DDG_UserState_Sit_Circle,
		DDG_UserState_StretchUp,
		DDG_UserState_StretchDown,
	} DDG_UserState_ENUM;

	typedef enum
	{
		DDG_MotorState_Stop = 0,
		DDG_MotorState_Up,
		DDG_MotorState_Down,
	} DDG_MotorState_ENUM;

	typedef enum
	{
		DDG_Place_NULL = 0,
		DDG_Place_Top,
		DDG_Place_Bot,
		DDG_Place_Eor,
	} DDG_Place_ENUM;

	typedef enum
	{
		DDG_Eor_NULL = 0,
		DDG_Eor_OutTime = 0x01,
		DDG_Eor_Circle = 0x02,
		DDG_Eor_Tread = 0x04,
		DDG_Eor_MotorBlock = 0x08,
		DDG_Eor_MotorNoContact = 0x10, // 未连接
		DDG_Eor_MotorNoControl = 0x20, // 失控
	} DDG_Eor_ENUM;

	extern DDGMotor_Struct DDGVariable;

	extern u8 LDDGOverLoadFlag, BDDGOverLoadFlag;
	extern const u8 ZeroRank[Max_ZeroRank][2]; // 3挡0重力圈数

	//----------用户调用
	extern void DDG_Set_Stop(void);
	extern void DDG_Set_BackUp(void);
	extern void DDG_Set_BackDown(void);
	extern void DDG_Set_BackAction(uint8_t act);
	extern void DDG_Set_LegUp(void);
	extern void DDG_Set_LegDown(void);
	extern void DDG_Set_LegAction(uint8_t act);
	extern void DDG_Set_SitUp(void);
	extern void DDG_Set_SitDown(void);
	extern void DDG_Set_SitUpCircle(u8 x1, u8 x2);
	extern void DDG_Set_SitDownCircle(u8 x1, u8 x2);
	extern void DDG_Set_SitCircle(u8 x1, u8 x2);
	extern void DDG_Set_Zero(u8 x);
	extern void DDG_Set_StretchUpC(u8 x1, u8 x2);
	extern void DDG_Set_StretchDownC(u8 x1, u8 x2);

	extern u16 nPreLDDGCircle, nPreBDDGCircle;

	extern u8 nCurZeroState, LegAutoAdj, LegVoid;
	extern u8 BMotorFaultCnt;
	extern u8 BMotorMaxCurrent;

	extern bool LegAdj_Flex_Flag;
	extern bool SIT_NONE_Flag;

	extern bool LMotorFaultFlag;

	void MotorProcess(void);

#define DDG_Get_BLocalCircle DDGVariable.BnLocalCircle
#define DDG_Get_LLocalCircle DDGVariable.LnLocalCircle

#define DDG_Get_BnCurCircle DDGVariable.BnCurCircle
#define DDG_Get_LnCurCircle DDGVariable.LnCurCircle

#define DDG_Get_Compete DDGVariable.UserState == DDG_UserState_Stop
#define DDG_Get_Run DDGVariable.UserState != DDG_UserState_Stop
#define DDG_NoAction (DDGVariable.LMotorState == DDG_MotorState_Stop && DDGVariable.BMotorState == DDG_MotorState_Stop)
#define BDDG_NoAction DDGVariable.BMotorState == DDG_MotorState_Stop
#define LDDG_NoAction DDGVariable.LMotorState == DDG_MotorState_Stop
#define LDDG_Action DDGVariable.LMotorState != DDG_MotorState_Stop
#define DDG_Get_BEor DDGVariable.BEor
#define DDG_Get_LEor DDGVariable.LEor

#ifdef __cplusplus
}
#endif

#endif

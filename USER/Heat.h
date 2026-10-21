#ifndef HEAT_H
#define HEAT_H

/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "config.h"
#include <string.h>

/*
 * 宏定义 *************************************************************************************************************************************
 */
#define FUNC_OFF 0	 /* 功能长关 */
#define FUNC_ON 1	 /* 功能长开 */
#define FUNC_BYTAB 2 /* 功能跟随表格开关 */

#define Heat1_On() gpio_bit_reset(GPIOE, GPIO_PIN_11)
#define Heat1_Off() gpio_bit_set(GPIOE, GPIO_PIN_11)

extern uint8_t RedLight_Duty;
uint8_t RedLight_DutyToPwm(uint8_t level);

#ifdef USE_REDLIGHT
#define RedLight_On() timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 100)
#define RedLight_Off() timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0)
#define RedLight_PWM(duty) timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, duty)
#else
#define Heat2_On() gpio_bit_reset(GPIOA, GPIO_PIN_7)
#define Heat2_Off() gpio_bit_set(GPIOA, GPIO_PIN_7)
#endif

#if (LEG_COM == 0 && ROLLER_CALVES_EN) 
#define CalfKnead_Pwm(dyty) timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, dyty);
#else
#define Heat3_On(dyty) timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, dyty)
#define Heat3_Off() timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0)
#endif

// #define Heat4_On(dyty)   timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_3,dyty)
// #define Heat4_Off()  timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_3,0)

#if (LEG_COM == 0 && ROLLER_FOOT_EN) // @ md 260513
#define FootRoll_PWM_Enable(x) timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, x);
#define FootRoll_PWM_Disable() timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 0);
#endif



/*
 * 类型定义 *************************************************************************************************************************************
 */
/********************************************************
 * 加热部位    单独控制
 *********************************************************/
typedef enum
{
	BackHeat,
	LegHeat,
	ShoulderHeat,
	ArmHeat,
	PJHeat,
} Heat_E;

/********************************************************
 * 加热模式   分区控制
 *********************************************************/
typedef enum
{
	NullHeat,
	UpperHeat,
	LowerHeat,
	FullHeat,

} HeatMode_E;

/********************************************************
 * 加热控制接口
 *********************************************************/
typedef struct
{

	uint8_t State;			  // 开关
	uint8_t Level;			  // 档位
	uint8_t Step;			  // 步进
	uint16_t OnTime;		  // 计时加热时间
	uint16_t OffTime;		  // 计时关闭时间
	uint32_t TargetOnOffTime; // 加热目标时间
	//			uint16_t TargetOffTime;//关闭目标时间
	uint16_t KeepOnTime; // 保持加热时间
	void (*HeatOn)(void);
	void (*HeatOff)(void);

} HeatInfo_S;

/*
 * 对外声明 *************************************************************************************************************************************
 */
extern bool BackHeatOn_Flag;
void BackHeatOn(void);
void BackHeatOff(void);
void LegHeatOn(void);
void LegHeatOff(void);
void HeatInit(void);
void HeatInitValSet(u8 whi, u16 para1, u16 para2, u16 para3);
u8 HeatStateGet(u8 whi);
void HeatStateSet(u8 whi, u8 sta, u8 level);
u8 HeatLevelGet(u8 whi);
void HeatLevelSet(u8 whi, u8 level);
void HeatProcess(void);

#endif

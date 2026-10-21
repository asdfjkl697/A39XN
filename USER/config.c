#include "config.h"

#define USART0_DATA_ADDRESS ((uint32_t)&USART_DATA(USART0))
#define USART1_DATA_ADDRESS ((uint32_t)&USART_DATA(USART1))
#define USART2_DATA_ADDRESS ((uint32_t)&USART_DATA(USART2))
#define UART3_DATA_ADDRESS ((uint32_t)&USART_DATA(UART3))
// #define UART4_DATA_ADDRESS       ((uint32_t)&USART_DATA(UART4))

uint16_t ADC0_Buffer[70] = {0};

UnionBit UnionState, UnionState1, UnionState2; //* 继电器
UnionBit UnionState_Flash;

//************RCU**************

void RCU_Config(void)
{

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_GPIOD);
	rcu_periph_clock_enable(RCU_GPIOE);
	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_DMA1);
	rcu_periph_clock_enable(RCU_ADC0);
	//	rcu_periph_clock_enable(RCU_ADC2);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV6);
	rcu_periph_clock_enable(RCU_USART0);
	rcu_periph_clock_enable(RCU_USART1);
	rcu_periph_clock_enable(RCU_USART2);
	rcu_periph_clock_enable(RCU_UART3);
	rcu_periph_clock_enable(RCU_UART4);
	rcu_periph_clock_enable(RCU_TIMER0);
	rcu_periph_clock_enable(RCU_TIMER2);
	rcu_periph_clock_enable(RCU_TIMER7);
	rcu_periph_clock_enable(RCU_AF);
}

//***********POWER**************

void Power_Config(void)
{

	// 5VA
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	gpio_bit_set(GPIOE, GPIO_PIN_5);

	// 5VB
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_bit_set(GPIOE, GPIO_PIN_4);

	// 5VC
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	gpio_bit_set(GPIOE, GPIO_PIN_3);

	// 24V_1
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
	gpio_bit_set(GPIOD, GPIO_PIN_1);

	// 24V_2
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_bit_set(GPIOD, GPIO_PIN_4);

	// 24V ADC
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_0);
}

/********LED*************/

void LED_Config(void)
{

	// MCU_LED
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	gpio_bit_reset(GPIOE, GPIO_PIN_6);

#ifdef LED2_PWM
	// LED1
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_set(GPIOE, GPIO_PIN_12);
#else
	// LED1
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_set(GPIOE, GPIO_PIN_12);
	// LED2
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_bit_set(GPIOE, GPIO_PIN_10);
#endif

	// LED3
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
	gpio_bit_set(GPIOC, GPIO_PIN_14);
}

/********Ion*************/

void NOI_Config(void)
{

	// FS
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_bit_reset(GPIOB, GPIO_PIN_4);

	// NOI
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	gpio_bit_set(GPIOC, GPIO_PIN_13);
}

/**********KeyOn/Off**********/
void Power_Key_Init(void)
{
	// GPIO设置开漏输出模式 默认输出高
	gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_bit_reset(GPIOB, GPIO_PIN_9);
}

//***********Pump**************

void Pump_IO_Config(void)
{

	// DECAY
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	gpio_bit_reset(GPIOB, GPIO_PIN_13);

	// PHASE
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_reset(GPIOB, GPIO_PIN_12);

	// FAULT
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

	// RESET
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	gpio_bit_reset(GPIOB, GPIO_PIN_15);

	// ENBL

	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

	// ADC
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4);
}

void Pump_PWM_Config(void)
{
	/* TIMER7 configuration: generate PWM signals with different duty cycles:
	   TIMER0CLK = SystemCoreClock / 120 = 1MHz */
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER7);
	timer_deinit(TIMER7);

	/* TIMER0 configuration */
	timer_initpara.prescaler = 119;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;	// 对齐
	timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
	timer_initpara.period = 99;							// 10kHz
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER7, &timer_initpara);

	/* CH0 configuration in PWM mode */
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMER7, TIMER_CH_2, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_2, 0);
	timer_channel_output_mode_config(TIMER7, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER7, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

	timer_primary_output_config(TIMER7, (ControlStatus)ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER7);
	timer_enable(TIMER7);
}

//**************Value***************

void Value_Config(void)
{

	// DAT
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	gpio_bit_reset(GPIOE, GPIO_PIN_7);

	// CLK
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_bit_reset(GPIOE, GPIO_PIN_8);

	// LATCH
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_bit_reset(GPIOE, GPIO_PIN_9);
}

//**************Heat***************

//	gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);

void Heat_IO_Config(void)
{

	/*************Heat1***********/
	gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_bit_set(GPIOE, GPIO_PIN_11);

	/*************Heat2***********/
#ifdef USE_REDLIGHT
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
#else
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
	gpio_bit_set(GPIOA, GPIO_PIN_7);
#endif
	/*************Heat3***********/
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

	/*************Heat4***********/
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

	// ADC
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_4 | GPIO_PIN_5);
}

void Heat_PWM_Config(void)
{
	/* TIMER2 clock is 120MHz: PSC=119 gives a 1MHz counter clock. */
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER2);
	timer_deinit(TIMER2);

	/* Select 10kHz, 5kHz, or 1kHz; PWM period remains 100 timer counts. */
#if defined(USE_REDLIGHT_1KHZ)
	timer_initpara.prescaler = 1199;
#elif defined(USE_REDLIGHT_2KHZ)
	timer_initpara.prescaler = 599;
#elif defined(USE_REDLIGHT_5KHZ)
	timer_initpara.prescaler = 239;
#elif defined(USE_REDLIGHT_500HZ)
	timer_initpara.prescaler = 2399;
#else
	timer_initpara.prescaler = 119;
#endif
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;	// 对齐
	timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
	timer_initpara.period = 99;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER2, &timer_initpara);

	/* CH0 configuration in PWM mode */
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
#ifdef USE_REDLIGHT
	timer_channel_output_config(TIMER2, TIMER_CH_1, &timer_ocintpara);
	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, 0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
	timer_channel_output_polarity_config(TIMER2, TIMER_CH_1, TIMER_OC_POLARITY_LOW);
#endif
	timer_channel_output_config(TIMER2, TIMER_CH_2, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

	timer_channel_output_polarity_config(TIMER2, TIMER_CH_2, TIMER_OC_POLARITY_LOW); // 极性取反

	timer_channel_output_config(TIMER2, TIMER_CH_3, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_3, 0);
	timer_channel_output_mode_config(TIMER2, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER2, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

	timer_channel_output_polarity_config(TIMER2, TIMER_CH_3, TIMER_OC_POLARITY_LOW); // 极性取反

	// timer_primary_output_config(TIMER2,ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER2);
	timer_enable(TIMER2);
}

//**************Motor***************

void Motor_IO_Config(void)
{

	/*********Back************/

	// DECAY
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_bit_reset(GPIOA, GPIO_PIN_11);

	// PHASE
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_reset(GPIOA, GPIO_PIN_12);

	// FAULT
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

	// RESET
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	gpio_bit_reset(GPIOD, GPIO_PIN_3);

	// ENBL

	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

	// singal
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

	/* enable and set key wakeup EXTI interrupt to the higher priority */
	nvic_irq_enable(EXTI5_9_IRQn, 2, 0);

	/* connect key wakeup EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOD, GPIO_PIN_SOURCE_6);

	/* configure key wakeup EXTI line */
	exti_init(EXTI_6, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
	exti_interrupt_flag_clear(EXTI_6);

	/*********Leg************/

	// DECAY
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	gpio_bit_reset(GPIOD, GPIO_PIN_13);

	// PHASE
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	gpio_bit_reset(GPIOD, GPIO_PIN_12);

	// FAULT
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);

	// RESET
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	gpio_bit_reset(GPIOD, GPIO_PIN_15);

	// ENBL

	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

	// singal
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_5);

	/* enable and set key wakeup EXTI interrupt to the higher priority */
	nvic_irq_enable(EXTI5_9_IRQn, 2, 1);

	/* connect key wakeup EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOD, GPIO_PIN_SOURCE_5);

	/* configure key wakeup EXTI line */
	exti_init(EXTI_5, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
	exti_interrupt_flag_clear(EXTI_5);

	/*********Leg2************/

	// DECAY
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_bit_reset(GPIOD, GPIO_PIN_9);

	// PHASE
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_bit_reset(GPIOD, GPIO_PIN_8);

	// FAULT
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	// RESET
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	gpio_bit_reset(GPIOD, GPIO_PIN_11);

	// ENBL

	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

	// singal
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

	/* enable and set key wakeup EXTI interrupt to the higher priority */
	nvic_irq_enable(EXTI5_9_IRQn, 2, 2);

	/* connect key wakeup EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOD, GPIO_PIN_SOURCE_7);

	/* configure key wakeup EXTI line */
	exti_init(EXTI_7, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
	exti_interrupt_flag_clear(EXTI_7);

	// ADC
	gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_5 | GPIO_PIN_6);
	gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
}

void Motor_PWM_Config(void)
{
	/* TIMER7 configuration: generate PWM signals with different duty cycles:
	   TIMER0CLK = SystemCoreClock / 120 = 1MHz */
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	//    rcu_periph_clock_enable(RCU_TIMER7);
	//    timer_deinit(TIMER7);

	/* TIMER0 configuration */
	timer_initpara.prescaler = 119;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;	// 对齐
	timer_initpara.counterdirection = TIMER_COUNTER_UP; // 向上计数
	timer_initpara.period = 99;							// 10kHz
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(TIMER7, &timer_initpara);

	/* CH0 configuration in PWM mode */
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
	timer_channel_output_config(TIMER7, TIMER_CH_0, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_0, 0);
	timer_channel_output_mode_config(TIMER7, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER7, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

	timer_channel_output_config(TIMER7, TIMER_CH_1, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_1, 0);
	timer_channel_output_mode_config(TIMER7, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER7, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

	timer_channel_output_config(TIMER7, TIMER_CH_3, &timer_ocintpara);

	timer_channel_output_pulse_value_config(TIMER7, TIMER_CH_3, 0);
	timer_channel_output_mode_config(TIMER7, TIMER_CH_3, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER7, TIMER_CH_3, TIMER_OC_SHADOW_DISABLE);

	timer_primary_output_config(TIMER7, (ControlStatus)ENABLE);
	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(TIMER7);
	timer_enable(TIMER7);
}

void ADC_Config(void)
{

	/* ADC mode config */
	adc_mode_config(ADC_MODE_FREE);

	/* ADC continous function enable */
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, (ControlStatus)ENABLE);
	//	adc_special_function_config(ADC2, ADC_CONTINUOUS_MODE, ENABLE);

	/* ADC SCAN function enable */
	adc_special_function_config(ADC0, ADC_SCAN_MODE, (ControlStatus)ENABLE);
	//	adc_special_function_config(ADC2,ADC_SCAN_MODE,ENABLE);

	/* ADC data alignment config */
	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	//	adc_data_alignment_config(ADC2,ADC_DATAALIGN_RIGHT);

	/* ADC resolution config */
	//		adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
	//		adc_resolution_config(ADC2, ADC_RESOLUTION_12B);

	/* ADC channel length config */
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 7);
	//	adc_channel_length_config(ADC2,ADC_REGULAR_CHANNEL,2);
	/* ADC temperature sensor channel config */
	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_0, ADC_SAMPLETIME_55POINT5);  // 24V
	adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_4, ADC_SAMPLETIME_55POINT5);  // pump
	adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_5, ADC_SAMPLETIME_55POINT5);  // Leg2
	adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_6, ADC_SAMPLETIME_55POINT5);  // back
	adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_13, ADC_SAMPLETIME_55POINT5); // leg1
	adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_14, ADC_SAMPLETIME_55POINT5); // heat3
	adc_regular_channel_config(ADC0, 6, ADC_CHANNEL_15, ADC_SAMPLETIME_55POINT5); // heat4

	/* ADC trigger config */
	adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
	// adc_external_trigger_source_config(ADC2, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
	/* ADC external trigger enable */
	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, (ControlStatus)ENABLE);
	// adc_external_trigger_config(ADC2,ADC_REGULAR_CHANNEL,ENABLE);

	adc_dma_mode_enable(ADC0);

	/* enable ADC interface */
	adc_enable(ADC0);
	delay_1ms(1);
	/* ADC calibration and reset calibration */
	adc_calibration_enable(ADC0);
	/* enable ADC interface */
	//		adc_enable(ADC2);
	//		delay_1ms(1);
	//		/* ADC calibration and reset calibration */
	//		adc_calibration_enable(ADC2);

	/* ADC DMA function enable */

	// adc_dma_mode_enable(ADC2);

	/* trigger start ADCx_channels conversion */
	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);

	//	adc_software_trigger_enable(ADC2, ADC_REGULAR_CHANNEL);
}

// ADC->DMA
void DMA_ADC_Config(void)
{

	dma_parameter_struct dma_init_ADC0;
	//	dma_parameter_struct dma_init_ADC2;

	/* deinitialize DMA channel */
	dma_deinit(DMA0, DMA_CH0); // ADC0
	// dma_deinit(DMA1, DMA_CH4);									//ADC2

	/* initialize DMA0 channel0(ADC0) */
	dma_init_ADC0.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
	dma_init_ADC0.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_init_ADC0.memory_addr = (uint32_t)(ADC0_Buffer);
	dma_init_ADC0.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_init_ADC0.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
	dma_init_ADC0.memory_width = DMA_MEMORY_WIDTH_16BIT;
	dma_init_ADC0.direction = DMA_PERIPHERAL_TO_MEMORY;
	dma_init_ADC0.number = 70;
	dma_init_ADC0.priority = DMA_PRIORITY_HIGH;
	dma_init(DMA0, DMA_CH0, &dma_init_ADC0);

	/* initialize DMA1 channel4(ADC2) */
	//	dma_init_ADC2.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
	//	dma_init_ADC2.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	//	dma_init_ADC2.memory_addr = (uint32_t)(ADC2_Buffer);
	//	dma_init_ADC2.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	//	dma_init_ADC2.periph_width = DMA_PERIPHERAL_WIDTH_32BIT;
	//	dma_init_ADC2.memory_width = DMA_MEMORY_WIDTH_32BIT;
	//	dma_init_ADC2.direction = DMA_PERIPHERAL_TO_MEMORY;
	//	dma_init_ADC2.number = 20;
	//	dma_init_ADC2.priority = DMA_PRIORITY_HIGH;
	//	dma_init(DMA1, DMA_CH4, &dma_init_ADC2);

	//	dma_circulation_enable(DMA1, DMA_CH4);

	//	dma_memory_to_memory_disable(DMA0,DMA_CH0);				//ADC0
	//	dma_memory_to_memory_disable(DMA1,DMA_CH4);				//ADC2

	/* enable all DMA channels you need */

	// dma_channel_enable(DMA1,DMA_CH4);									//ADC2

	dma_circulation_enable(DMA0, DMA_CH0); // 循环模式开启dma_circulation_enable(DMA0, DMA_CH0)//dma_circulation_disable
	//	dma_memory_to_memory_disable(DMA0, DMA_CH0);

	dma_channel_enable(DMA0, DMA_CH0);
	/* enable DMA transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH0, DMA_INT_FTF); // 打开全部完成中断
	/* enable DMA channel */
	//  dma_channel_enable(DMA0, DMA_CH0);	/* DMA内存到内存模式不开启 */
	// ADC0
	nvic_irq_enable(DMA0_Channel0_IRQn, 1, 4);
}

//*************DMA**********************

// USART->DMA0

void DMA0_USART_Config(void)
{

	dma_parameter_struct dma_data_parameter_usart_ch3;
#ifdef USART1_DMA
	dma_parameter_struct dma_data_parameter_usart_ch6;
#endif
	dma_parameter_struct dma_data_parameter_usart_ch1;

	/* enable DMA clock */
	rcu_periph_clock_enable(RCU_DMA0);

	/* initialize DMA channe34 */

	dma_deinit(DMA0, DMA_CH3);
#ifdef USART1_DMA
	dma_deinit(DMA0, DMA_CH6);
#endif
	dma_deinit(DMA0, DMA_CH1);

	/*************DMA_CH3************/

	dma_struct_para_init(&dma_data_parameter_usart_ch3);
	dma_data_parameter_usart_ch3.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_data_parameter_usart_ch3.memory_addr = (uint32_t)(&TX0_Buffer);
	dma_data_parameter_usart_ch3.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter_usart_ch3.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_data_parameter_usart_ch3.number = 20;
	dma_data_parameter_usart_ch3.periph_addr = USART0_DATA_ADDRESS; //((uint32_t)0x40013804);
	dma_data_parameter_usart_ch3.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter_usart_ch3.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_data_parameter_usart_ch3.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH3, &dma_data_parameter_usart_ch3);

	/*************DMA_CH6************/
#ifdef USART1_DMA
	dma_struct_para_init(&dma_data_parameter_usart_ch6);
	dma_data_parameter_usart_ch6.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_data_parameter_usart_ch6.memory_addr = (uint32_t)(&TX1_Buffer);
	dma_data_parameter_usart_ch6.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter_usart_ch6.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_data_parameter_usart_ch6.number = 20;
	dma_data_parameter_usart_ch6.periph_addr = USART1_DATA_ADDRESS;
	dma_data_parameter_usart_ch6.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter_usart_ch6.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_data_parameter_usart_ch6.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH6, &dma_data_parameter_usart_ch6);
#endif

	/*************DMA_CH1************/

	dma_struct_para_init(&dma_data_parameter_usart_ch1);
	dma_data_parameter_usart_ch1.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_data_parameter_usart_ch1.memory_addr = (uint32_t)(&TX2_Buffer);
	dma_data_parameter_usart_ch1.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter_usart_ch1.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_data_parameter_usart_ch1.number = 20;
	dma_data_parameter_usart_ch1.periph_addr = USART2_DATA_ADDRESS;
	dma_data_parameter_usart_ch1.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter_usart_ch1.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_data_parameter_usart_ch1.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA0, DMA_CH1, &dma_data_parameter_usart_ch1);

	/* configure DMA mode */
	dma_circulation_disable(DMA0, DMA_CH3);
	dma_memory_to_memory_disable(DMA0, DMA_CH3);
#ifdef USART1_DMA
	dma_circulation_disable(DMA0, DMA_CH6);
	dma_memory_to_memory_disable(DMA0, DMA_CH6);
#endif
	dma_circulation_disable(DMA0, DMA_CH1);
	dma_memory_to_memory_disable(DMA0, DMA_CH1);

	/* USART DMA enable for transmission */
	usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
	usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
	usart_dma_transmit_config(USART2, USART_TRANSMIT_DMA_ENABLE);
	/* enable DMA channel1 transfer complete interrupt */
	dma_interrupt_enable(DMA0, DMA_CH3, DMA_INT_FTF);
#ifdef USART1_DMA
	dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
#endif
	dma_interrupt_enable(DMA0, DMA_CH1, DMA_INT_FTF);
	/* disable DMA channel1 */
	dma_channel_disable(DMA0, DMA_CH3);
#ifdef USART1_DMA
	dma_channel_disable(DMA0, DMA_CH6);
#endif
	dma_channel_disable(DMA0, DMA_CH1);
	/*configure DMA0 interrupt*/
	nvic_irq_enable(DMA0_Channel3_IRQn, 1, 0);
#ifdef USART1_DMA
//		nvic_irq_enable(DMA0_Channel6_IRQn, 1,1);
#endif
	nvic_irq_enable(DMA0_Channel1_IRQn, 1, 2);
}

// UART->DMA1

void DMA1_UART_Config(void)
{

	dma_parameter_struct dma_data_parameter_usart_ch4;

	/* enable DMA clock */
	rcu_periph_clock_enable(RCU_DMA1);

	/* initialize DMA channe34 */

	dma_deinit(DMA1, DMA_CH4);

	/*************DMA_CH3************/

	dma_struct_para_init(&dma_data_parameter_usart_ch4);
	dma_data_parameter_usart_ch4.direction = DMA_MEMORY_TO_PERIPHERAL;
	dma_data_parameter_usart_ch4.memory_addr = (uint32_t)(&TX3_Buffer);
	dma_data_parameter_usart_ch4.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	dma_data_parameter_usart_ch4.memory_width = DMA_MEMORY_WIDTH_8BIT;
	dma_data_parameter_usart_ch4.number = 20;
	dma_data_parameter_usart_ch4.periph_addr = UART3_DATA_ADDRESS;
	dma_data_parameter_usart_ch4.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	dma_data_parameter_usart_ch4.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	dma_data_parameter_usart_ch4.priority = DMA_PRIORITY_ULTRA_HIGH;
	dma_init(DMA1, DMA_CH4, &dma_data_parameter_usart_ch4);

	/* configure DMA mode */
	dma_circulation_disable(DMA1, DMA_CH4);
	dma_memory_to_memory_disable(DMA1, DMA_CH4);

	/* USART DMA enable for transmission */

	usart_dma_transmit_config(UART3, USART_TRANSMIT_DMA_ENABLE);
	/* enable DMA channel1 transfer complete interrupt */
	dma_interrupt_enable(DMA1, DMA_CH4, DMA_INT_FTF);

	/* disable DMA channel1 */
	dma_channel_disable(DMA1, DMA_CH4);

	/*configure DMA1 interrupt*/

	nvic_irq_enable(DMA1_Channel3_Channel4_IRQn, 1, 3);
}

// * 0 UI 、1 health 、2 bluetooth 、 3 voicecontrol 、 4 com485 260415
void USART_Config(u32 baud0, u32 baud1, u32 baud2, u32 baud3, u32 baud4)
{

	//************USART0**********

	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* USART configure */
	usart_deinit(USART0);
	usart_baudrate_set(USART0, baud0);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_enable(USART0);
	nvic_irq_enable(USART0_IRQn, 0, 0); // 中断使能

	usart_interrupt_enable(USART0, USART_INT_RBNE); // 接收中断使能
	//  	usart_interrupt_enable(USART0, USART_INT_TBE); //发送中断使能

	//************USART1**********

	/* connect port to USARTx_Tx */
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	/* USART configure */
	usart_deinit(USART1);
	usart_baudrate_set(USART1, baud1);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	usart_enable(USART1);
	nvic_irq_enable(USART1_IRQn, 0, 1); // 中断使能

	usart_interrupt_enable(USART1, USART_INT_RBNE); // 接收中断使能
	//	usart_interrupt_enable(USART1, USART_INT_TBE); //发送中断使能

	//************USART2**********

	/* connect port to USARTx_Tx */
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	/* USART configure */
	usart_deinit(USART2);
	usart_baudrate_set(USART2, baud2);
	usart_receive_config(USART2, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
	usart_enable(USART2);
	nvic_irq_enable(USART2_IRQn, 0, 2); // 中断使能

	usart_interrupt_enable(USART2, USART_INT_RBNE); // 接收中断使能
	usart_interrupt_enable(USART2, USART_INT_IDLE); // IDLE线路检测中断
	//	usart_interrupt_enable(USART2, USART_INT_TBE); //发送中断使能

	//************UART3**********

	/* connect port to USARTx_Tx */
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	/* USART configure */
	usart_deinit(UART3);
	usart_baudrate_set(UART3, baud3);
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
	usart_enable(UART3);
	nvic_irq_enable(UART3_IRQn, 0, 3); // 中断使能

	usart_interrupt_enable(UART3, USART_INT_RBNE); // 接收中断使能
	//	usart_interrupt_enable(UART3, USART_INT_TBE); //发送中断使能

	//************UART4********** 485

	/* connect port to USARTx_Tx */
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	/* connect port to USARTx_Rx */
	gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	// DE
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	gpio_bit_reset(GPIOD, GPIO_PIN_0);
	/* USART configure */
	usart_deinit(UART4);
	usart_baudrate_set(UART4, baud4);
	usart_receive_config(UART4, USART_RECEIVE_ENABLE);
	usart_transmit_config(UART4, USART_TRANSMIT_ENABLE);
	usart_enable(UART4);
	nvic_irq_enable(UART4_IRQn, 0, 4); // 中断使能

	usart_interrupt_enable(UART4, USART_INT_RBNE); // 接收中断使能
												   // 	usart_interrupt_enable(UART4, USART_INT_TBE); //发送中断使能
}

u8 TX0_Buffer[COM_TX0_Lenth];
u8 RX0_Buffer[COM_RX0_Lenth];

volatile COMx_Define COM0 = {0, 0, 0, 0, 0, 0};

uint8_t EUSART0_Read(void)
{
	uint8_t readValue = 0;

	readValue = RX0_Buffer[COM0.RX_read];
	if (++COM0.RX_read >= COM_RX0_Lenth)
		COM0.RX_read = 0;
	return readValue;
}
#ifdef USART0_DMA
void EUSART0_Write(uint8_t *data_buf, uint32_t length)
{
	dma_memory_address_config(DMA0, DMA_CH3, (uint32_t)data_buf);
	dma_transfer_number_config(DMA0, DMA_CH3, length);
	dma_channel_enable(DMA0, DMA_CH3);
}
#else

void EUSART0_Write(uint8_t dat)
{
	TX0_Buffer[COM0.TX_write] = dat;
	if (++COM0.TX_write >= COM_TX0_Lenth)
		COM0.TX_write = 0;
	COM0.B_TX_busy = 0;
}

#endif

u8 TX1_Buffer[COM_TX1_Lenth];
u8 RX1_Buffer[COM_RX1_Lenth];

volatile COMx_Define COM1 = {0, 0, 0, 0, 0, 0};

uint8_t EUSART1_Read(void)
{
	uint8_t readValue = 0;

	readValue = RX1_Buffer[COM1.RX_read];
	if (++COM1.RX_read >= COM_RX1_Lenth)
		COM1.RX_read = 0;
	return readValue;
}
#ifdef USART1_DMA
void EUSART1_Write(uint8_t *data_buf, uint32_t length)
{
	dma_memory_address_config(DMA0, DMA_CH6, (uint32_t)data_buf);
	dma_transfer_number_config(DMA0, DMA_CH6, length);
	dma_channel_enable(DMA0, DMA_CH6);
}
#else

void EUSART1_Write(uint8_t dat)
{
	TX1_Buffer[COM1.TX_write] = dat;
	if (++COM1.TX_write >= COM_TX1_Lenth)
		COM1.TX_write = 0;
	COM1.B_TX_busy = 0;
	usart_interrupt_enable(USART1, USART_INT_TBE); // 发送中断使能
}

#endif

u8 TX2_Buffer[COM_TX2_Lenth];
u8 RX2_Buffer[COM_RX2_Lenth];

volatile COMx_Define COM2 = {0, 0, 0, 0, 0, 0};

u8 tx2_checksum = 0;

void PrintString2(char *puts)
{
	for (; *puts != 0; puts++)
		EUSART2_Write((u8)*puts); // 遇到停止符0结束
}

uint8_t EUSART2_Read(void)
{
	uint8_t readValue = 0;

	readValue = RX2_Buffer[COM2.RX_read];
	if (++COM2.RX_read >= COM_RX2_Lenth)
		COM2.RX_read = 0;
	return readValue;
}

#ifdef USART2_DMA
void EUSART2_Write(uint8_t *data_buf, uint32_t length)
{
	dma_memory_address_config(DMA0, DMA_CH1, (uint32_t)data_buf);
	dma_transfer_number_config(DMA0, DMA_CH1, length);
	dma_channel_enable(DMA0, DMA_CH1);
}
#else

void EUSART2_Write(uint8_t dat)
{
	tx2_checksum += dat;

	TX2_Buffer[COM2.TX_write] = dat;
	if (++COM2.TX_write >= COM_TX2_Lenth)
		COM2.TX_write = 0;
	COM2.B_TX_busy = 0;
	usart_interrupt_enable(USART2, USART_INT_TBE); // 发送中断使能
}

#endif

u8 TX3_Buffer[COM_TX3_Lenth];
u8 RX3_Buffer[COM_RX3_Lenth];

volatile COMx_Define COM3 = {0, 0, 0, 0, 0, 0};

uint8_t EUART3_Read(void)
{
	uint8_t readValue = 0;

	readValue = RX3_Buffer[COM3.RX_read];
	if (++COM3.RX_read >= COM_RX3_Lenth)
		COM3.RX_read = 0;
	return readValue;
}

#ifdef UART3_DMA
void EUART3_Write(uint8_t *data_buf, uint32_t length)
{
	dma_memory_address_config(DMA1, DMA_CH4, (uint32_t)data_buf);
	dma_transfer_number_config(DMA1, DMA_CH4, length);
	dma_channel_enable(DMA1, DMA_CH4);
}
#else

void EUART3_Write(uint8_t dat)
{
	TX3_Buffer[COM3.TX_write] = dat;
	if (++COM3.TX_write >= COM_TX3_Lenth)
		COM3.TX_write = 0;
	COM3.B_TX_busy = 0;
	usart_interrupt_enable(UART3, USART_INT_TBE); // 发送中断使能
}

#endif

u8 TX4_Buffer[COM_TX4_Lenth];
u8 RX4_Buffer[COM_RX4_Lenth];

volatile COMx_Define COM4 = {0, 0, 0, 0, 0, 0};

uint8_t EUART4_Read(void)
{
	uint8_t readValue = 0;

	readValue = RX4_Buffer[COM4.RX_read];
	if (++COM4.RX_read >= COM_RX4_Lenth)
		COM4.RX_read = 0;
	return readValue;
}

void EUART4_Write(uint8_t dat)
{
	TX4_Buffer[COM4.TX_write] = dat;
	if (++COM4.TX_write >= COM_TX4_Lenth)
		COM4.TX_write = 0;
	//			 COM4.B_TX_busy=0;
}

void UserConfig(void)
{

	RCU_Config();																 // 时钟源
	Power_Config();																 // 电源
	NOI_Config();																 // 负氧离子
	LED_Config();																 // 彩灯
	Pump_IO_Config();															 // 气泵
	Pump_PWM_Config();															 // 气泵
	Value_Config();																 // 气阀
	Heat_IO_Config();															 // 加热
	Heat_PWM_Config();															 // 加热
	Motor_IO_Config();															 // 电动缸
	Motor_PWM_Config();															 // 电动缸
	USART_Config(USART0_BAUD, USART1_BAUD, USART2_BAUD, UART3_BAUD, UART4_BAUD); // 通信
#ifdef USART_DMA
	DMA0_USART_Config(); // 通信
	DMA1_UART_Config();	 // 通信
#endif
	ADC_Config();	  // ADC检测
	DMA_ADC_Config(); // ADC检测
}

/********
 *OS阻塞任务配置
 * *********/
u16 OS_RTC_TICK = 0;
u16 OS_Pt_Tick[MAX_OS_TASK];
u16 OS_Pt[MAX_OS_TASK];

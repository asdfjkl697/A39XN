/*!
    \file  gd32f30x_it.c
    \brief interrupt service routines

    \version 2017-02-10, V1.0.0, demo for GD32F30x
    \version 2018-10-10, V1.1.0, demo for GD32F30x
    \version 2018-12-25, V2.0.0, demo for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"
#include "systick.h"
#include "config.h"
#include "Motor.h"

volatile uint32_t Timer_1ms = 0, Timer_5ms_Cnt = 0, Timer_10ms_Cnt = 0, Timer_50ms_Cnt = 0, Timer_100ms_Cnt = 0, Timer_150ms_Cnt = 0, Timer_200ms_Cnt = 0, Timer_1s_Cnt;
volatile u8 ADC0_Finish = 0;

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
    Timer_1ms++;
    Timer_5ms_Cnt++;
    Timer_10ms_Cnt++;
    Timer_50ms_Cnt++;
    Timer_100ms_Cnt++;
    Timer_200ms_Cnt++;
}

uint32_t GetTick(void)
{

    return Timer_1ms;
}

/* UART ISR helper macros — common RX (RBNE) and TX (TBE) patterns */
#define UART_ISR_RX(USARTx, COMx, RX_BUF, RX_LEN)                      \
    if (RESET != usart_interrupt_flag_get(USARTx, USART_INT_FLAG_RBNE)) \
    {                                                                    \
        usart_interrupt_flag_clear(USARTx, USART_INT_FLAG_RBNE);        \
        RX_BUF[COMx.RX_write] = usart_data_receive(USARTx);             \
        if (++COMx.RX_write >= RX_LEN)                                   \
            COMx.RX_write = 0;                                           \
    }

#define UART_ISR_TXE(USARTx, COMx, TX_BUF, TX_LEN)                     \
    if (RESET != usart_interrupt_flag_get(USARTx, USART_INT_FLAG_TBE)) \
    {                                                                    \
        usart_interrupt_flag_clear(USARTx, USART_INT_FLAG_TBE);         \
        if (COMx.TX_read != COMx.TX_write)                               \
        {                                                                 \
            usart_data_transmit(USARTx, TX_BUF[COMx.TX_read]);           \
            if (++COMx.TX_read >= TX_LEN)                                 \
                COMx.TX_read = 0;                                         \
        }                                                                 \
        else                                                               \
        {                                                                  \
            usart_interrupt_disable(USARTx, USART_INT_TBE);                \
        }                                                                  \
    }

void USART0_IRQHandler(void)
{
    UART_ISR_RX(USART0, COM0, RX0_Buffer, COM_RX0_Lenth);
    UART_ISR_TXE(USART0, COM0, TX0_Buffer, COM_TX0_Lenth);
}

void USART1_IRQHandler(void)
{
    UART_ISR_RX(USART1, COM1, RX1_Buffer, COM_RX1_Lenth);
#ifndef USART1_DMA
    UART_ISR_TXE(USART1, COM1, TX1_Buffer, COM_TX1_Lenth);
#endif
}

volatile uint8_t UART2_RE_TIMEOUT = 0;     // 超时计数器 (fallback)
volatile uint8_t UART2_DATAPRO_SIGE = 0;   // IDLE帧结束信号
void USART2_IRQHandler(void)
{
    // ① RBNE: 逐字节接收
    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE))
    {
        usart_interrupt_flag_clear(USART2, USART_INT_FLAG_RBNE);
        RX2_Buffer[COM2.RX_write] = usart_data_receive(USART2);
        if (++COM2.RX_write >= COM_RX2_Lenth)
            COM2.RX_write = 0;
        UART2_RE_TIMEOUT = 0; // 超时清零
    }
    // ② IDLE: RX线路空闲 → 一帧接收完毕
    if (RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE))
    {
        // 清除IDLE标志: 读STAT0(_get已完成) + 读DR
        usart_data_receive(USART2);
        UART2_DATAPRO_SIGE = 1;
    }
#ifndef USART2_DMA
    UART_ISR_TXE(USART2, COM2, TX2_Buffer, COM_TX2_Lenth);
#endif
}

void UART3_IRQHandler(void)
{
    UART_ISR_RX(UART3, COM3, RX3_Buffer, COM_RX3_Lenth);
#ifndef UART3_DMA
    UART_ISR_TXE(UART3, COM3, TX3_Buffer, COM_TX3_Lenth);
#endif
}

void UART4_IRQHandler(void)
{
    UART_ISR_RX(UART4, COM4, RX4_Buffer, COM_RX4_Lenth);

    /* UART4 TBE handler with RS485 direction control (GPIO PD0) */
    if (RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_TBE))
    {
        usart_interrupt_flag_clear(UART4, USART_INT_FLAG_TBE);

        if (COM4.TX_read != COM4.TX_write)
        {
            usart_data_transmit(UART4, TX4_Buffer[COM4.TX_read]);
            if (++COM4.TX_read >= COM_TX4_Lenth)
                COM4.TX_read = 0;
        }
        else if (COM4.B_TX_busy == 1)
        {
            usart_data_transmit(UART4, TX4_Buffer[COM4.TX_read]);
            COM4.B_TX_busy = 0;
        }
        else
        {
            gpio_bit_reset(GPIOD, GPIO_PIN_0);
            usart_interrupt_disable(UART4, USART_INT_TBE);
        }
    }
}

void DMA0_Channel3_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH3, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH3, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH3);
    }
    // RX
    //    if(RESET != dma_interrupt_flag_get(DMA_CH4, DMA_INT_FLAG_FTF)){
    //        dma_interrupt_flag_clear(DMA_CH4, DMA_INT_FLAG_G);
    //        //g_transfer_complete = SET;
    //    }
}

void DMA0_Channel6_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH6);
    }
}

void DMA0_Channel1_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH1, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH1, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA0, DMA_CH1);
    }
}

void DMA1_Channel3_4_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA1, DMA_CH4, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA1, DMA_CH4, DMA_INT_FLAG_FTF);
        dma_channel_disable(DMA1, DMA_CH4);
    }
}

void DMA0_Channel0_IRQHandler(void)
{
    // TX
    if (RESET != dma_interrupt_flag_get(DMA0, DMA_CH0, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(DMA0, DMA_CH0, DMA_INT_FLAG_FTF);
        //		dma_channel_disable(DMA0, DMA_CH0);
        ADC0_Finish = 1;
    }
}

volatile uint32_t Leg_Debounce_tick = 0; // todo add 260527
void EXTI5_9_IRQHandler(void)
{
    if (RESET != exti_interrupt_flag_get(EXTI_5))
    {
        exti_interrupt_flag_clear(EXTI_5);

        if (DDGVariable.LDir == DDG_MotorState_Up)
        {
            DDGVariable.LCircleOutTime = 0;
            // if (DDGVariable.LnCurCircle)
            // {
            //     DDGVariable.LnCurCircle--;
            // }

            // if (DDGVariable.LnLocalCircle > 0)
            //     DDGVariable.LnLocalCircle--;
            // nPreLDDGCircle++;

            if (TICK_COUNTER(Leg_Debounce_tick, 5)) // todo md 260605
            {
                if (DDGVariable.LnCurCircle)
                {
                    DDGVariable.LnCurCircle--;
                }

                if (DDGVariable.LnLocalCircle > 0)
                    DDGVariable.LnLocalCircle--;
                nPreLDDGCircle++;
            }
            TICK_RESET(Leg_Debounce_tick);
        }
        else if (DDGVariable.LDir == DDG_MotorState_Down)
        {
            DDGVariable.LCircleOutTime = 0;
            // if (DDGVariable.LSignalInit)
            // {
            //     DDGVariable.LnCurCircle++;
            // }
            // if (DDGVariable.LnLocalCircle > 0)
            //     DDGVariable.LnLocalCircle--;
            // nPreLDDGCircle++;

            if (TICK_COUNTER(Leg_Debounce_tick, 5)) // todo md 260605
            {
                if (DDGVariable.LSignalInit)
                {
                    DDGVariable.LnCurCircle++;
                }

                if (DDGVariable.LnLocalCircle > 0)
                    DDGVariable.LnLocalCircle--;
                nPreLDDGCircle++;
            }
            TICK_RESET(Leg_Debounce_tick);
        }
    }

    if (RESET != exti_interrupt_flag_get(EXTI_6))
    {

        exti_interrupt_flag_clear(EXTI_6);
        if (DDGVariable.BDir == DDG_MotorState_Up)
        {
            DDGVariable.BCircleOutTime = 0;
            if (DDGVariable.BnCurCircle > 0)
                DDGVariable.BnCurCircle--;
            if (DDGVariable.BnLocalCircle > 0)
                DDGVariable.BnLocalCircle--;
            nPreBDDGCircle++;
        }
        else if (DDGVariable.BDir == DDG_MotorState_Down)
        {

            DDGVariable.BCircleOutTime = 0;
            if (DDGVariable.BSignalInit)
                DDGVariable.BnCurCircle++;
            if (DDGVariable.BnLocalCircle > 0)
                DDGVariable.BnLocalCircle--;
            nPreBDDGCircle++;
        }
    }

    if (RESET != exti_interrupt_flag_get(EXTI_7))
    {

        exti_interrupt_flag_clear(EXTI_7);
    }
}

#define ADC_CHANNEL_COUNT   7    /* Number of ADC channels */
#define ADC_SAMPLES_PER_CH  10   /* Samples collected per channel */
#define ADC_FILTER_SAMPLES  (ADC_SAMPLES_PER_CH - 2) /* Samples after discarding 1 max + 1 min (=8) */

volatile uint16_t AD_Get_Value[ADC_CHANNEL_COUNT];
volatile uint8_t GetValueFlag = 0;

void ADC_Process(void)
{
    uint8_t i, j;
    uint16_t AD_Temp_Value[ADC_CHANNEL_COUNT] = {0};
    uint16_t MaxValue, MinValue;

    if (ADC0_Finish)
    {
        ADC0_Finish = 0;
        for (i = 0; i < ADC_CHANNEL_COUNT; i++)
        {

            for (j = i; j <= (i + (ADC_SAMPLES_PER_CH - 1) * ADC_CHANNEL_COUNT); j = j + ADC_CHANNEL_COUNT)
            {

                if (GetValueFlag == 0)
                {

                    MinValue = ADC0_Buffer[j];
                    MaxValue = ADC0_Buffer[j];
                    GetValueFlag = 1;
                }
                if (ADC0_Buffer[j] >= MaxValue)
                    MaxValue = ADC0_Buffer[j];
                if (ADC0_Buffer[j] <= MinValue)
                    MinValue = ADC0_Buffer[j];

                AD_Temp_Value[i] = AD_Temp_Value[i] + ADC0_Buffer[j];
            }

            AD_Temp_Value[i] = (AD_Temp_Value[i] - MaxValue - MinValue) / ADC_FILTER_SAMPLES;
            GetValueFlag = 0;
        }

        for (i = 0; i < ADC_CHANNEL_COUNT; i++)
        {

            AD_Get_Value[i] = AD_Temp_Value[i];
        }
    }
}

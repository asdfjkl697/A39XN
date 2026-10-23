/*
 * File:   Define.h
 * Author: Administrator
 *
 * Created on September 7, 2017, 9:31 PM
 */

#ifndef AIRSENSOR_H
#define AIRSENSOR_H

// #include "DEFINE.h"
#include "config.h"

#define RxLength 10
#define TxLength 10u

// #define P_RXB gpio_input_data_bit_read(GPIOB, GPIO_PINS_8)
// #define P_TXB_SET(x) gpio_bits_write(GPIOB, GPIO_PINS_4, (confirm_state)x)

// extern u8 TxBusyFlag; // 正在发送一个字节, 用户设置1开始发送, 发送完毕系统清自动0
// extern u8 B_Rx_OK;    // 接收完的标志位, 收到数据块系统设置1, 用户处理数据后必须清0
// extern u8 Rx_Cnt;     // 接收到的字节数, 用户处理数据后必须清0
// extern u8 Tx_Cnt;
// extern u8 RxBuf[RxLength]; // 接收缓冲
// extern u8 TxBuf[TxLength]; // 发送缓冲

typedef union
{
    u16 Data;
    u8 Value[2];
} Key_Union;

extern void AirSensor_Init(void);

extern uint8_t gAirIntersity;

extern void AirSensor_Handler(void);

#endif /* DEFINE_H */

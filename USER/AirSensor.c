#include "AirSensor.h"
#if defined(USE_AIRSENSOR) && defined(USE_S0553)
#error USE_AIRSENSOR and USE_S0553 share TIMER4 and the software UART pins
#endif

#ifdef USE_AIRSENSOR

u8 TxBusyFlag; // 正在发送一个字节, 用户设置1开始发送, 发送完毕系统清自动0
u8 B_Rx_OK;    // 接收完的标志位, 收到数据块系统设置1, 用户处理数据后必须清0
u8 Rx_Cnt = 0;
u8 Rx_Cnt1 = 0;
u8 Tx_Cnt = 0;
u8 Tx_Cnt1 = 0;
volatile u8 RxBuf[RxLength];
volatile u8 TxBuf[TxLength];

//===== 下面这些为系统使用的常量或变量, 用户不可见，请勿修改 =============

#define RxBitLenth 9       // 8个数据位+1个停止位
#define TxBitLenth 9       // 8个数据位+1个停止位
u8 TxShiftReg, RxShiftReg; // 发送 接收 移位
u8 TxSample, RxSample;     // 发送和接收检测 采样计数器(3倍速率检测)
u8 TxBitCnt, RxBitCnt;     // 发送和接收的数据 位计数器
u8 RxTimeOut;              // 接收超时计数
u8 RxStartFlag;            // 正在接收一个字节(已收到起始位)

//=====================================================================

/*************  外部函数和变量声明 *****************/

//========================================================================
// 函数: void UART_INIT(void)
// 描述: UART模块的初始变量.
// 参数: 无.
// 返回: 无.
// 版本: V1.0, 2012-10-22
//========================================================================

void TXRX_SetOutput(void)
{
}

void TXRX_SetInput(void)
{
}

#define P_RXB gpio_input_bit_get(GPIOE, GPIO_PIN_13)
#define P_TXB_SET(x) gpio_bit_write(GPIOE, GPIO_PIN_14, (bit_status)x)

void AirSensor_Init(void)
{
    timer_parameter_struct timer_initpara;

    /* 1. 开启时钟 */
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_TIMER4);

    /* 2. 配置 GPIO */
    // TX: PE14, 推挽输出, 初始化为高电平 (空闲状态)
    gpio_init(GPIOE, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_bit_set(GPIOE, GPIO_PIN_14); // 【重要】空闲时为高电平

    // RX: PE13, 上拉输入 (抗干扰能力比浮空强)
    gpio_init(GPIOE, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_13);

    timer_deinit(TIMER4);

    timer_initpara.prescaler = 120 - 1; // 1MHz 计数时钟
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 139 - 1; // 动态计算
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;

    timer_init(TIMER4, &timer_initpara);

    /* 4. 清除标志位并使能中断 */
    timer_flag_clear(TIMER4, TIMER_FLAG_UP);
    timer_interrupt_enable(TIMER4, TIMER_INT_UP);

    /* 5. 配置 NVIC - 高优先级保证时序准确 */
    nvic_irq_enable(TIMER4_IRQn, 3, 0); // 抢占优先级3，子优先级0

    /* 6. 启动定时器 */
    timer_enable(TIMER4);

    /* 7. 初始化软件状态变量 */
    TxBusyFlag = 0;
    RxStartFlag = 0;
    B_Rx_OK = 0;

    Rx_Cnt = 0;
    Rx_Cnt1 = 0;
    Tx_Cnt = 0;
    Tx_Cnt1 = 0;

    TxSample = 3;
    RxSample = 4; // 起始位采样点通常稍晚，以避开边沿抖动
    RxTimeOut = 0;

    TxBitCnt = 0;
    RxBitCnt = 0;
    TxShiftReg = 0;
    RxShiftReg = 0;
}

// void TIMER4_IRQHandler(void)
// {
//     if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP) == SET)
//     {
//         timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);

//         // ================= 发送状态机 =================
//         if (TxBusyFlag)
//         {
//             if (--TxSample == 0)
//             {
//                 TxSample = 3; // 重置采样计数器

//                 if (TxBitCnt == 0)
//                 {
//                     // --- 准备发送新字节 ---
//                     if (Tx_Cnt < Tx_Cnt1)
//                     {
//                         P_TXB_SET(0);               // 1. 发送起始位 (Low)
//                         TxShiftReg = TxBuf[Tx_Cnt]; // 2. 加载数据
//                         TxBitCnt = TxBitLenth;      // 3. 设置位计数器 (9位: 8数据+1停止)
//                         Tx_Cnt++;                   // 4. 移动发送索引
//                     }
//                     else
//                     {
//                         // --- 所有数据发送完毕 ---
//                         P_TXB_SET(1);   // 保持总线空闲 (High)
//                         TxBusyFlag = 0; // 清除忙标志
//                         // 可选：在此处切换GPIO为输入模式以接收，如果硬件需要
//                     }
//                 }
//                 else
//                 {
//                     // --- 发送数据位或停止位 ---
//                     TxBitCnt--;

//                     if (TxBitCnt == 0)
//                     {
//                         // 最后一位是停止位，发送高电平
//                         P_TXB_SET(1);
//                     }
//                     else
//                     {
//                         // 发送数据位 (LSB First)
//                         if (TxShiftReg & 0x01)
//                             P_TXB_SET(1);
//                         else
//                             P_TXB_SET(0);

//                         TxShiftReg >>= 1;
//                     }
//                 }
//             }
//         }

//         // ================= 接收状态机 =================
//         else
//         {
//             // 1. 超时处理：用于判断帧结束或切换发送
//             if (RxTimeOut > 0)
//             {
//                 RxTimeOut--;
//             }
//             else
//             {
//                 // 如果超时且没有正在接收，可以在此处检查是否有发送请求
//                 // 注意：不要在这里自动置位 TxBusyFlag，应由主程序控制
//             }

//             // 2. 接收逻辑
//             if (RxStartFlag)
//             {
//                 RxTimeOut = 10; // 收到数据位，重置超时
//                 if (--RxSample == 0)
//                 {
//                     RxSample = 3; // 重置采样
//                     if (--RxBitCnt == 0)
//                     {
//                         // --- 一字节接收完成 ---
//                         RxStartFlag = 0;

//                         // 可选：校验停止位 (P_RXB 应为 1)
//                         if (Rx_Cnt < RxLength)
//                         {
//                             RxBuf[Rx_Cnt] = RxShiftReg;
//                             Rx_Cnt++;
//                         }

//                         // 如果接收完预期长度，设置标志
//                         if (Rx_Cnt >= Rx_Cnt1 && Rx_Cnt1 != 0)
//                         {
//                             B_Rx_OK = 1;
//                         }
//                     }
//                     else
//                     {
//                         // --- 采样数据位 ---
//                         RxShiftReg >>= 1;
//                         if (P_RXB)
//                         {
//                             RxShiftReg |= 0x80; // MSB 置 1
//                         }
//                     }
//                 }
//             }
//             else
//             {
//                 // --- 检测起始位 (下降沿) ---
//                 if (!P_RXB)
//                 {
//                     RxStartFlag = 1;
//                     RxSample = 4;          // 第一个采样点延后，避开起始边沿噪声
//                     RxBitCnt = RxBitLenth; // 9位
//                     RxShiftReg = 0;
//                     RxTimeOut = 10; // 启动超时计数
//                 }
//             }
//         }
//     }
// }

void TIMER4_IRQHandler(void)
{
    if (timer_interrupt_flag_get(TIMER4, TIMER_INT_FLAG_UP) == SET)
    {
        timer_interrupt_flag_clear(TIMER4, TIMER_INT_FLAG_UP);
        //====================== 模拟串口发送程序 ========================================
        if (TxBusyFlag) // 发送开始标志位   judge whether sending
        {
            if (--TxSample == 0) // 发送数据以定时器的1/3来发送
            {
                TxSample = 3;      // 重置发送计数器   reset send baudrate counter
                if (TxBitCnt == 0) // 发送计数器为0 表明单字节发送还没开始
                {
                    P_TXB_SET(0);               // 发送开始位     					send start u8
                    TxShiftReg = TxBuf[Tx_Cnt]; // 把缓冲的数据放到发送的buff		load data from TxSBUF to TxShiftReg
                    TxBitCnt = TxBitLenth;      // 发送数据位数 (8数据位+1停止位)	initial send u8 number (8 data bits + 1 stop u8)
                }
                else // 发送计数器为非0 正在发送数据
                {
                    if (--TxBitCnt == 0) // 发送计数器减为0 表明单字节发送结束
                    {
                        P_TXB_SET(1); // 送停止位数据

                        if (++Tx_Cnt >= TxLength)
                            Tx_Cnt = 0;

                        if (Tx_Cnt == Tx_Cnt1)
                        {
                            TxBusyFlag = 0;
                            TXRX_SetInput();
                        }
                    }
                    else
                    {
                        if (TxShiftReg & 0x01)
                            P_TXB_SET(1);
                        else
                            P_TXB_SET(0);
                        TxShiftReg >>= 1;
                    }
                }
            }
        }
        else
        {
            //====================== 模拟串口接收程序 ========================================
            if (RxStartFlag) // 已接收到起始位
            {
                RxTimeOut = 10;      // 超时计数初值, 35个位的时间(对应5个字节), 参考MODBUS协议
                if (--RxSample == 0) // 接收数据以定时器的1/3来接收
                {
                    RxSample = 3;        // 重置接收计数器  接收数据以定时器的1/3来接收	reset send baudrate counter
                    if (--RxBitCnt == 0) // 接收完一帧数据
                    {
                        RxStartFlag = 0;
                        RxBuf[Rx_Cnt] = RxShiftReg; // 存储数据到缓冲区	save the data to RBUF
                        if (++Rx_Cnt >= RxLength)
                            Rx_Cnt = 0; // 溢出判断
                    }
                    else
                    {
                        RxShiftReg >>= 1; // 把接收的单b数据 暂存到 RxShiftReg(接收缓冲)
                        if (P_RXB)
                            RxShiftReg |= 0x80; // shift RX data to RX buffer
                    }
                }
            }
            else if (!P_RXB) // 判断是不是开始位 P_RXB=0;
            {
                RxStartFlag = 1;       // 如果是则设置开始接收标志位 	set start receive flag
                RxSample = 4;          // 初始化接收波特率计数器       	initial receive baudrate counter
                RxBitCnt = RxBitLenth; // 初始化接收的数据位数(8个数据位+1个停止位)    initial receive u8 number (8 data bits + 1 stop u8)
            }
            if (RxTimeOut > 0) // 接收超时处理
            {
                RxTimeOut--;
            }
            else
            {
                if (Tx_Cnt != Tx_Cnt1)
                {
                    TXRX_SetOutput();
                    TxBusyFlag = 1;
                    TxBitCnt = 0;
                }
            }
        }
    }
}

void usart_Send_Data(u8 x)
{
    TxBuf[Tx_Cnt1] = x;
    if (++Tx_Cnt1 >= TxLength)
        Tx_Cnt1 = 0;
}

u8 usart_Get_Data(void)
{
    u8 i;
    i = RxBuf[Rx_Cnt1];
    if (++Rx_Cnt1 >= RxLength)
        Rx_Cnt1 = 0;
    return i;
}

#ifdef USE_AIRSENSOR
#define AirSensor_Head 0XAA
#define AirSensor_Tial 0X55

#define AirSensor_Receive_Data 5

uint8_t AirIntersityTable[6] = {0, 11, 12, 13, 14, 15};

uint8_t gAirIntLimitValue = 13; // 15--19 气压传感器压强值，超过该值认为气压传感器检测到气压

uint8_t gAirIntersity;

uint8_t AirSensor_HandBuffer[AirSensor_Receive_Data], AirSensor_HandCnt = 0, AirSensor_RX_Status = 0;

static void AirSensor_Process(void)
{
    uint8_t CheckSum = 0;

    while (Rx_Cnt1 != Rx_Cnt)
    {
        AirSensor_HandBuffer[AirSensor_HandCnt] = usart_Get_Data();

        switch (AirSensor_RX_Status)
        {
        default:
            if (AirSensor_HandBuffer[AirSensor_HandCnt] == AirSensor_Head)
                AirSensor_RX_Status = 1, AirSensor_HandCnt++;

            break;

        case 1:
            if (AirSensor_HandBuffer[AirSensor_HandCnt] == AirSensor_Tial)
                AirSensor_RX_Status = 2, AirSensor_HandCnt++;
            else
            {
                if (AirSensor_HandBuffer[AirSensor_HandCnt] != AirSensor_Head)
                    AirSensor_RX_Status = 0, AirSensor_HandCnt = 0;
            }

            break;

        case 2:
            if (++AirSensor_HandCnt >= AirSensor_Receive_Data)
                AirSensor_HandCnt = 0, AirSensor_RX_Status = 3;

            break;
        }

        if (AirSensor_RX_Status == 3)
        {
            AirSensor_RX_Status = 0;
            CheckSum = AirSensor_HandBuffer[0] + AirSensor_HandBuffer[1] + AirSensor_HandBuffer[2] + AirSensor_HandBuffer[3]; // 计算校验和

            if (CheckSum != AirSensor_HandBuffer[4])
                return;

            gAirIntersity = AirSensor_HandBuffer[2];
        }
    }
}
#endif

void AirSensor_Handler(void)
{
    AirSensor_Process();
}

#endif

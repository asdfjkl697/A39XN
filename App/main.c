/*!
    \file  main.c
    \brief GPIO running led demo

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

#include "gd32f30x.h"
#include <stdio.h>
#include "systick.h"
#include "config.h"
#include "main.h"
#include "Table.h"        // * 250829
#include "MassageLogic.h" // * add 250925
#include "MassageEXEC.h"
#include "FUJIApp.h" //@ add for blueota 260303

/* Task timing defines (SysTick period = 1ms) */
#define TASK_PERIOD_5MS 5     /* 5 × 1ms (Timer_5ms_Cnt) trigger at ~25ms */
#define TASK_PERIOD_100MS 100 /* 100 × 1ms = 100ms */
#define TASK_PERIOD_200MS 200 /* 200 × 1ms = 200ms */
#define TASK_PERIOD_1S 5      /* 5 × 200ms = 1s (count of Timer_200ms_Cnt rollovers) */

/* Aging mode (LC) timing */
#define LC_CYCLE_MINUTES 76      /* Total cycle: 75 minutes + 1 minute resume buffer */
#define LC_PAUSE_MINUTES 16      /* Enter pause at 16 minutes remaining */
#define LC_PAUSE_THRESHOLD_SEC 3 /* Resume when seconds drop below this in pause window */

uint8_t PumpState = 0;

uint8_t LegResetStep = 0;
uint8_t Core_RestSuccess = 0;

void Variable_Init(void)
{
#ifdef PART_USE_SUM_TIME
    Flash_PART_USE_read();
#endif
    BodyPlaceCal(DEFAULT_SHOULDER); // * 上电给默认位置 250414
    Flash_Parameter_read();
    User_Memory_Use_DIY();    // * 增加自定义读取
    KeyPush.WiseKeyState = 0; // * 初始化把这个标志位清零 250304
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);

    /* configure systick */
    systick_config();
    __enable_irq();
    ota_init();
    UserConfig();
    AirInit();
#ifdef USE_AIRSENSOR
    AirSensor_Init(); // todo add 260629
#endif
    Motor_Work_Iint(); // * add 251010
    HeatInit();
    Motor_Init();
    PowerInit();
    ArmKeyInit();
    LegInit();
    // MoveUpInit();
    AT_Init();
    rgbInit();
    Blue_Init();
    Health_Init();
    Variable_Init(); //* 增加 变量初始化
    UserFunc_Set(UserFunc_PowerOff, 0);
#ifdef USE_S0553
    S0553Key_Init();
#endif

#ifdef USE_WDT_ENABLE
    fwdgt_init();
#endif

    while (1)
    {
        PowerProcess();
        ADC_Process();
        // 蓝牙
        os_task_BlueRx();
        os_task_blue(TASK_ID_BLUE);
        os_task_voice(TASK_ID_VOICE);
#if (USE_BLUE_APP == 1)
        os_task_app(TASK_ID_APP);
#endif

#if (USE_Health_Check)
        os_task_Health(TASK_ID_HEALTH);
#endif // USE_Health_Check

        // 声控
        VoiceControl_Handler();
        _485_ComRx();

        // Light_Update(); //* rm 250926

        KeyCom_Handler(); /* 手控器接收数据 */

#ifdef USE_AIRSENSOR
        AirSensor_Handler(); // todo add 260629
#endif

#ifdef USE_S0553
        if (Timer_10ms_Cnt >= 10u)
        {
            Timer_10ms_Cnt -= 10u;
            S0553Key_Task10ms();
        }
#endif

        CoreRest(); /* 机芯复位 */

        if (Timer_5ms_Cnt > TASK_PERIOD_5MS)
        {
            OS_RTC_TICK++;
            Timer_5ms_Cnt = 0;
            MotorProcess();  //* 靠背、小腿 执行
            RS485_Polling(); //* 485轮询 执行 上下机芯 小腿 扶手按键
#if (USE_BLUE_OTA == 1)
            if (OS_RTC_TICK & 0x01)  //* 借用时基
                FUJIApp_Deal_10ms(); // todo add for blueota 260303
#endif
        }

        if (Timer_100ms_Cnt >= TASK_PERIOD_100MS)
        {
            Timer_100ms_Cnt = 0;
            Health_OutTime_Hand();
            BluePlay_time(); //* 播放时间--
            // Flash_Check_Deal(); // todo 无效 260821
            KeyPowerScan();
            KeyEncoderHandler(); /* 按键板编码器处理 */
#ifdef USE_S0555
            Display_Drive(); //* 手控器显示 100ms ->200ms 250318
#endif

            WorkStateProcess(); //* 工作状态调度
            AT_Process();       //* 读表
            RS485_Poll_Reset(); //* 读表发送补丁 251104

            MassageLogic_Process(); //* 配置逻辑
            AirLogic_Process();     //* 气压配置逻辑

            Part_EXEC_Process(); //* 部件执行
#ifdef USE_WDT_ENABLE
            feed_watchdog();
#endif
        }

        if (Timer_200ms_Cnt >= TASK_PERIOD_200MS)
        {
            Timer_200ms_Cnt = 0;
            Timer_1s_Cnt++;
            // BackPosUpdate();
#ifndef USE_S0555
            Display_Drive(); //* 手控器显示 100ms ->200ms 250318
#endif
#ifdef LED2_PWM
            Led_Pwm_Update(); // * 呼吸灯
#endif
            ErrorCheckProcess(); // * 主板直接检测故障代码  转移到这里 250926
        }

        if (Timer_1s_Cnt >= TASK_PERIOD_1S)
        {
            Timer_1s_Cnt = 0;

            MCU_LedProcess();
            Eor_Update(); // * 故障代码更新
#ifdef PART_USE_SUM_TIME
            PartUse_Process_S(); // * 增加部件运行时间
#endif
#ifdef Debug_Mode
            Debug_Pro();
#endif

            //* 非老化模式下需要定时 定时结束后不计时 250403
#ifdef A39X_LC
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_Pause && nLCState)) //* 增加老化停止判断 250423
#else
            if (WorkStateGet() == WorkState_Work)
#endif
            {
#ifdef A39X_LC
                if (nLCState)
                {
                    if (WorkMinGet() == 0)
                    {
                        WorkMinSet(LC_CYCLE_MINUTES); // * 75分钟循环 250423
                        WorkSecSet(0);
                        Exit_Pause();
                    }
                    if (WorkMinGet() == LC_PAUSE_MINUTES && WorkSecGet() < LC_PAUSE_THRESHOLD_SEC)
                    {
                        WorkMinSet(15);
                        WorkSecSet(59);
                        Enter_Pause();
                    }
                }
#endif
                if (WorkSecGet() > 0)
                    WorkSecSet(WorkSecGet() - 1);
                else
                {
                    if (WorkMinGet() > 0)
                    {
#ifdef PART_USE_SUM_TIME
                        PartUse_Process_Min(); // * 增加部件运行时间
#endif
                        WorkMinSet(WorkMinGet() - 1);
                        WorkSecSet(59);
                    }
                    else
                    {
                        // if (nTimOutResetSw)
                        // {
                        //     UserFunc_Set(UserFunc_PowerOff, 0);
                        // }
                        // else
                        // {
                        //     UserFunc_Set(UserFunc_PowerOff, 1);
                        // }
                        UserFunc_Set(UserFunc_PowerOff, 2);
                    }
                }
            }
        }
    }
}

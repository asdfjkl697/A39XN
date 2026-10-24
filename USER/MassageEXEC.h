#ifndef MASSAGEEXEC_H
#define MASSAGEEXEC_H

#include "config.h"
#include "MoveBoth.h"
#include <string.h>

typedef enum
{
    MotorPart_STOP = 0, // 伸缩停⽌, para1,para2,speed⽆效
    MotorPart_FWD,      // 伸缩正转, para1,para2,speed有效
    MotorPart_REV,
    MotorPart_FWD_CIRCLE, // 以当前位置正转指定圈数，para1, para2合并圈数, speed有效
    MotorPart_REV_CIRCLE, // 以当前位置反转指定圈数，para1, para2合并圈数, speed有效
    Motor_FOLW_TREAD,     //* 跟随机芯行走
    MotorPart_Zero,       //* 零重力
    MotorPart_Stretch,
    MotorPart_END,
} MotorPartCmd;

typedef enum
{
    FootRock_STOP = 0, // 翻转停⽌, para1,para2,speed⽆效
    FootRock_FIXED,    // 以当前位置正转指定圈数，para1, para2合并圈数, speed有效
    FootRock_2POS,     // 2点循环
    FootRock_END,
} RockPartCmd;

typedef enum
{
    MotorT_Back = Motor_Back - Motor_Back,
    MotorT_Leg = Motor_Leg - Motor_Back,
    MotorT_Flex = Motor_Flex - Motor_Back,
    MotorT_Calf = Motor_Calf - Motor_Back,
    MotorT_Feet = Motor_Feet - Motor_Back,
    MotorT_Zero = Motor_Zero - Motor_Back,
    MotorT_Sway = Motor_Sway - Motor_Back,
    MotorT_Rock = Motor_Rock - Motor_Back,
    MotorT_Max,
} MotorT_enum;

typedef struct
{
    u8 Cmd[MotorT_Max];
    u8 Para1[MotorT_Max];
    u8 Para2[MotorT_Max];
    u8 Para3[MotorT_Max];
    bool RunFlag[MotorT_Max]; // 启动标志
    bool PreFlag[MotorT_Max];
    bool Fault[MotorT_Max]; //* Fault > Manual > Paradox > Auto
    bool Manual[MotorT_Max];
    bool Paradox[MotorT_Max];
    u16 RunTime[MotorT_Max]; // 运行时间
} MotorT_Work_S;

extern MotorT_Work_S MotorT_Work;

extern void MotorPart_Date_Set(u8 motorpart, u8 para0, u8 method, u8 para1, u8 para2, u8 para3);
extern void Part_EXEC_Process(void);
extern void Motor_Work_Iint(void);
extern void MotorT_FootFindAfterStretchArm(void);
#endif

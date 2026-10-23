#include "Table.h"

#ifndef USE_M2_4D

//* Test
const CoreMixOtherTabItem_t SubTab_TEST[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{Part_LOOP, 0, 10, 6, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{MoveUp_Tread, TREAD_FOLLOW_FLEX2, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 5, 6, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 10, 0, 3},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 5, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 10, 6, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{MoveUp_Tread, TREAD_FOLLOW_FLEX1, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 5, 6, 0},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 10, 0, 3},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD1, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_LOOP, 0, 10, 4, 0},
	// {MoveUp_Tread, TREAD_FWD_CIRCLE, 10, 0, 5},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 10, 0, 5},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD3, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD3, 0, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_FWD_CNT, 2, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_CNT, 2, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_CNT, 2, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_LOOP, 0, 3, 6, 0},
	// {MoveUp_Knead, KNEAD_FWD_ABS, 25, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_ABS, 55, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_LOOP, 0, 3, 6, 0},
	// {MoveUp_Knead, KNEAD_FWD_ABS, 25, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_ABS, 55, 0, 6},
	// {MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRA
const CoreMixOtherTabItem_t SubTab_STRA[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREA, 0, 0}, // * 调用气压表
	// {Motor_Feet, MotorPart_STOP, 0, 0, 0},

	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * 揉凤池穴
	// {MoveUp_Knead, KNEAD_1F2R, 0x96, 5, 0x85},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 5},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_SUB2, ConMth_Loop, SUB2TB_RNFN, 5, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_LOOP, 0, 3, 5, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * 修改 0x55-0x53 251007
	// {MoveUp_Knead, KNEAD_1F2R_WAIT, 0x54, 0xFF, 0x55},  // * 修改 251007
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2}, //* LB_2 -> LB_H2 -> 2
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x39, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 0, 0}, //* 小腿揉搓 滚刮
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 3, 12, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * test 251119
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Motor_Flex, MotorPart_FWD, 35, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Motor_Flex, MotorPart_REV, 35, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 2, 0x95},
	// {MoveUp_Knead, KNEAD_1R2R, 0xA4, 3, 0x85},   // wqx test
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRB
const CoreMixOtherTabItem_t SubTab_STRB[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREA, 0, 0}, // * 调用气压表
	// {Motor_Feet, MotorPart_STOP, 0, 0, 0},		   //* 关闭足底滚轮

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 3, 15, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tap, FLAP_ACT_CLK, 3, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 3, 30, 0},
	{MoveUp_Tap, FLAP_ACT_CLK, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_LOOP, 0, 3, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	// * 揉凤池穴
	{MoveUp_Knead, KNEAD_1F2R, 0x96, 5, 0x85},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x39, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 0, 0}, //* 小腿揉搓 滚刮
#endif
	{Part_LOOP, 0, 3, 12, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Motor_Flex, MotorPart_FWD, 35, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Motor_Flex, MotorPart_REV, 35, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	// {MoveUp_Knead, KNEAD_1F2R, 0x84, 3, 0x55},
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 2, 0x95},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRC
const CoreMixOtherTabItem_t SubTab_STRC[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREA, 0, 0}, // * 调用气压表

	// @ replace 260119
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_LOOP, 0, 2, 20, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 6, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 6, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 2, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x39, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 0, 0}, //* 小腿揉搓 滚刮
#endif
	{Part_LOOP, 0, 3, 14, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Motor_Flex, MotorPart_FWD, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 55, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N4 & 0xFF, ROCKPOS_N4 >> 8, 5},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Motor_Flex, MotorPart_REV, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 45, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	// {MoveUp_Knead, KNEAD_1F2R, 0x84, 3, 0x55},
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 2, 0x95},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRD
const CoreMixOtherTabItem_t SubTab_STRD[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREB, 0, 0}, // * 调用气压表

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2}, // @ add 260117
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//* 5 风池穴揉捏
	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_SUB2, ConMth_Loop, SUB2TB_TNFN, 5, 0},
	{Part_SUB2, ConMth_Loop, SUB2TB_RNFN, 5, 0},
	//* 6 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// //*  肩部反向变速揉捏
	// {MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#if (AIR_CLAFBOT)
	{Air_Keep, 0x78, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x38, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},

	// @ 修改 260106
	{Part_LOOP, 0, 3, 24, 0},
	{Motor_Flex, MotorPart_FWD, 50, 0, 3},
	{MoveUp_Knead, KNEAD_1R2R, 0xA4, 3, 0x85},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 55, 30, 0},
	// todo 增加 翻转
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N4 & 0xFF, ROCKPOS_N4 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 5, 0x22},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{Motor_Flex, MotorPart_REV, 50, 0, 3},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 24, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH7, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTH1 - 3, 0, 3},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH7, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTH1 - 3, 0, 3},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 2, 0x73},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRE
const CoreMixOtherTabItem_t SubTab_STRE[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREA, 0, 0}, // * 调用气压表
	// {Motor_Feet, MotorPart_STOP, 0, 0, 0},

	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},

	{MoveUp_Tap, FLAP_ACT, 0, 0, 3}, // @ add 260117
	{Part_LOOP, 0, 4, 4, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTH5, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, 35, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0}, // @ md 260117

	{Part_LOOP, 0, 6, 5, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 4, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH8, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},		// TODO MOVE HERE 261023
	{Motor_Rock, FootRock_2POS, 0x32, 4, 0x21}, // todo add 261015

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{MoveUp_Knead, KNEAD_REV_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x53, 0xFF, 0x63}, // * 修改 0x55-0x53 251007
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x73, 5, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x53, 0xFF, 0x63}, // * 修改 0x55-0x53 251007
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x39, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#ifndef MOTOR_CALF_WEAK
// {Motor_Calf, MotorPart_FWD, 0, 0, 0}, //* 小腿揉搓 滚刮
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 2, 15, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * test 251119
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Motor_Flex, MotorPart_FWD, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 55, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N4 & 0xFF, ROCKPOS_N4 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 5, 0x22},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Motor_Flex, MotorPart_REV, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 45, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Air_Del, Air_CalfS, 0, 0, 0}, // @ 释放小腿气压 261022
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	// {Part_LOOP, 0, 1, 12, 0},
	// {MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * test 251119
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	// {Motor_Flex, MotorPart_FWD, 35, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	// {Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	// {MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	// {Motor_Flex, MotorPart_REV, 35, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	// {MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	// {Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Motor_Calf, MotorPart_STOP, 0, 0, 0},

	{Part_LOOP, 0, 1, 14, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * test 251119
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Motor_Flex, MotorPart_FWD, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N4 & 0xFF, ROCKPOS_N4 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 6, 0x22},
	// {MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 120, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Motor_Flex, MotorPart_REV, 50, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 2, 0x95},
	// {MoveUp_Knead, KNEAD_1R2R, 0xA4, 3, 0x85},   // wqx test
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STRF
const CoreMixOtherTabItem_t SubTab_STRF[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		   // todo add 260521
	{Part_AIR, PartCon_Run, FA_GAS_STRPREB, 0, 0}, // * 调用气压表

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//* 5 风池穴揉捏
	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//* 6 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#if (AIR_CLAFBOT)
	{Air_Keep, 0x78, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#else
	{Air_Keep, 0x38, 0, 30, 0}, //*  保压模式 部位/时间/超时时间
#endif
	{Motor_Feet, MotorPart_STOP, 0, 0, 0},

	{Part_LOOP, 0, 3, 18, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, 60, 0}, //* 倒脚
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},					   //* 小腿揉搓 滚刮 停止
	{MoveUp_Knead, KNEAD_1F2F, 0x83, 0xFF, 0x83},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Motor_Flex, MotorPart_FWD, 40, 0, 3},
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 4, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 角度恢复
	{Motor_Calf, MotorPart_FWD, 0, 0, 0},									  //* 小腿揉搓 滚刮
	{Motor_Flex, MotorPart_REV, 40, 0, 3},
	{MoveUp_Knead, KNEAD_1R2R, 0x38, 4, 0x83},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0}, //* 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0}, //* 小腿揉搓 滚刮 停止
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xff, 0x73},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STR1
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t SubTab_STR1[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01111011), 90, 10, 50},
#else
	{Air_UpDw, B(00111011), 70, 15, 45},
#endif
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// todo RNZFS 消磨一点时间 260521
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 6, 0x55},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0}, //* 小腿揉搓 滚刮 //rm 251120
#endif
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // @ add 260302
#endif
	// {MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_Knead, KNEAD_1F2R, 0x96, 6, 0x32},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 8, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 70 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 70 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
#ifdef USE_M2
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 4},
#else
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 6},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Calf, MotorPart_STOP, 0, 0, 0}, //* 小腿揉搓 滚刮 停止

	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x97, 4, 0x62},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Air_UpDw, B(00111011), 0, 10, 60},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5}, // X=1
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},		  // Y=8
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 3},			  // Z=30
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x7B, 0, 30, 0},
#else
	{Air_Keep, 0x3B, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 95, 50, 0}, //* 拉伸角度2
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 4, 0x62},
#if (AIR_CLAFBOT)
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 3},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
	{Part_LOOP, 0, 3, 18, 0},
	{Motor_Flex, MotorPart_FWD, 25, 0, 3}, //* 伸脚
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// todo 增加 翻转
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 5, 0x22},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Motor_Flex, MotorPart_REV, 25, 0, 3}, //* 缩脚
#ifdef USE_M2
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
#else
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 4, 0x62},
#endif

#if (AIR_CLAFBOT)
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 3},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
#endif
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0}, // todo 需要停留时间 261011

#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Del, AIR_CLAFBOT, 0, 0, 0},
	{Air_Del, AIR_SEAT, 0, 0, 0},
#else
	{Air_Del, Air_ArmL, 0, 0, 0},
#endif
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* STR2
//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t SubTab_STR2[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01111101), 90, 10, 50},
#else
	{Air_UpDw, B(00111101), 70, 15, 45},
#endif
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0}, //* 小腿揉搓 滚刮 //rm 251120
#endif
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 8, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 80 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 80 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	// {MoveDw_Tap, FLAP_ACT, 0, 0, 6}, // todo rm 260523
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Calf, MotorPart_STOP, 0, 0, 0}, //* 小腿揉搓 滚刮 停止

	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 150, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x7D, 0, 30, 0},
#else
	{Air_Keep, 0x3D, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 95, 50, 0}, //* 拉伸角度2

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},

	{Motor_Flex, MotorPart_FWD, 30, 0, 3}, //* 伸脚
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_REV, 30, 0, 3}, //* 缩脚
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x97, 4, 0x62},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};
//* STR3
//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t SubTab_STR3[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01111001), 90, 10, 50},
#else
	{Air_UpDw, B(00111001), 70, 15, 45},
#endif
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0}, //* 小腿揉搓 滚刮 //rm 251120
#endif
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 8, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 80 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 80 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV, 0, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		 //* 小腿揉搓 滚刮 停止
	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 24, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0},
#else
	{Air_Keep, 0x39, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 95, 50, 0}, //* 拉伸角度2

	{Part_LOOP, 0, 2, 16, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 24, 0, 3},
	{Motor_Flex, MotorPart_FWD, 25, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	// todo 增加 翻转 261015
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 5, 0x22},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Motor_Flex, MotorPart_REV, 25, 0, 3},
	{Air_Del, Air_ArmR, 0, 0, 0},
	{Air_Del, Air_CalfS, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#if (AIR_SEAT)
	{Air_Seat, AirPart_STOP, 0, 0, 0},
#endif

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 24, 0, 3},
	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	// todo 增加 翻转 261015
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 5, 0x22},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};
//* STR4
const CoreMixOtherTabItem_t SubTab_STR4[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01111001), 90, 10, 50},
#else
	{Air_UpDw, B(00111001), 70, 15, 45},
#endif
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 4},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0}, //* 小腿揉搓 滚刮 //rm 251120
#endif
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 8, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 80 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 80 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 4},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV, 0, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},		 //* 小腿揉搓 滚刮 停止
	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_LOOP, 0, 2, 4, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x79, 0, 30, 0},
#else
	{Air_Keep, 0x39, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 95, 50, 0}, //* 拉伸角度2
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_LOOP, 0, 3, 10, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};
//* STR5
//* 7 RES 6 RES 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t SubTab_STR5[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},

	{Air_UpDw, B(00111101), 70, 15, 45},

	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0}, //* 小腿揉搓 滚刮 //rm 251120
#endif
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 8, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 80 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 80 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 6},
	// {MoveDw_Tap, FLAP_ACT, 0, 0, 6}, // TODO RM 260527
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Calf, MotorPart_STOP, 0, 0, 0}, //* 小腿揉搓 滚刮 停止

	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 150, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x7D, 0, 30, 0},
#else
	{Air_Keep, 0x3D, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 95, 50, 0}, //* 拉伸角度2

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_FWD, 25, 0, 3}, //* 伸脚
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	// todo 增加 翻转 261015
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{Motor_Flex, MotorPart_REV, 25, 0, 3}, //* 缩脚
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Refresh, 0x79, 0, 30, 0}, // todo add 260530
#else
	{Air_Del, Air_ArmR, 0, 0, 0},
#endif

	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	// todo 增加 翻转 261015
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	// todo 增加 翻转 261015
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N34 & 0xFF, ROCKPOS_N34 >> 8, 5},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},

	{Motor_Flex, MotorPart_REV, 30, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};
//* STR6
//* 7 RES 6 RES 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t SubTab_STR6[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},

	{Air_UpDw, B(00111011), 70, 15, 45},

	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 拉伸准备角度

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 5},
	{MoveDw_KneadF, FOLW_COPY, 0, 0, 0}, // todo md 260521
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// todo RNZFS 消磨一点时间 260521
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 6, 0x55},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Motor_Calf, MotorPart_FWD, 0, 100, 0}, //* 小腿揉搓 滚刮 //rm 251120

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // todo add 260302
#endif
	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Air_CalfS, AirPart_ModeB, 80 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 60 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 80, 10, 80},
#endif
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
	{Air_Ankle, AirPart_ModeB, 80 - ANKLE_AIR_WEAK * 2, 8 + ANKLE_AIR_WEAK, 80 - ANKLE_AIR_WEAK * 2},

	{MoveUp_Knead, KNEAD_1F2F, 0x95, 0xff, 0x73},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 3},
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{Motor_Calf, MotorPart_STOP, 0, 0, 0}, //* 小腿揉搓 滚刮 停止

	{Motor_Zero, MotorPart_Stretch, 100, 30, 0}, //* 拉伸角度1

	// {MoveUp_Knead, KNEAD_1F2R, 0x64, 0xff, 0x55},
	{MoveUp_Knead, KNEAD_1F2R, 0x83, 0xff, 0x37},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{MoveDw_Knead, KNEAD_1F2R, 0x97, 5, 0x42},
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	// {MoveUp_Knead, KNEAD_1F2R, 0x73, 0xff, 0x55}, // @ ADD 260302
	{MoveUp_Knead, KNEAD_1F2R, 0x95, 0xff, 0x24},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 4},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 2},
	{Part_EXEC, ConMth_Move, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_Move, 0, 30, 0},
	{Part_EXEC, ConMth_Move, 50, 30, 0}, // todo 延时5秒 260521
#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Keep, 0x7B, 0, 30, 0},
#else
	{Air_Keep, 0x3B, 0, 30, 0},
#endif
	{Motor_Zero, MotorPart_Stretch, 90, 50, 0}, // TODO MD 95-99 260524

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5}, // TODO ADD 260521
	{MoveDw_Knead, KNEAD_1F2R, 0x97, 4, 0x63},
	// {MoveDw_Flex, FLEX_FIX_CIRCLE, 58, 0, 3}, // TODO MD 260521
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{Motor_Feet, MotorPart_STOP, 0, 0, 0},
	{Part_LOOP, 0, 3, 20, 0},
	{Motor_Flex, MotorPart_FWD, 30, 0, 3},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5}, // @ ADD 260302
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},

	{MoveDw_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 3},
	{Part_EXEC, ConMth_Move, 0, 30, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 3},
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 3},
	{MoveUp_Knead, KNEAD_1F2R, 0x64, 0xff, 0x42},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 3},
	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 5},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 9}, // TODO H1 与 2 之间 260531
	{Part_EXEC, ConMth_Move, 0, 30, 0},
	{Motor_Flex, MotorPart_REV, 30, 0, 3}, //* 缩脚
	// {MoveUp_Knead, KNEAD_1F2R, 0x64, 0xff, 0x42},
	// {MoveDw_Knead, KNEAD_1F2R, 0xa6, 4, 0x63},
	{MoveDw_Knead, KNEAD_1F2R, 0x85, 4, 0x35},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_C0, 0, 4},

#if (AIR_CLAFBOT | AIR_SEAT)
	{Air_Refresh, 0x79, 0, 30, 0}, // todo add 260530
#else
	{Air_Del, Air_Ankle, 0, 0, 0},
	{Air_Del, Air_ArmR, 0, 0, 0},
#endif
	{Part_EXEC, ConMth_Move, 0, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	{Motor_Zero, MotorPart_Stretch, 50, 95, 0}, //* 恢复准备角度
#ifdef USE_M2
#ifdef USE_M2_4D
	{MoveDw_Flex, FLEX_FIX_CIRCLE, 5, 0, 3}, // todo 10-5 260521
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
#endif

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 8, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};
//* STR7
const CoreMixOtherTabItem_t SubTab_STR7[] = {

	{Part_STOP, 0, 0, 0, 0},
};
//* STR8
const CoreMixOtherTabItem_t SubTab_STR8[] = {

	{Part_STOP, 0, 0, 0, 0},
};
//* STR9
const CoreMixOtherTabItem_t SubTab_STR9[] = {

	{Part_STOP, 0, 0, 0, 0},
};

//* FullMa1
const CoreMixOtherTabItem_t SubTab_FullMa1[] = {
	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_REV, 0, 0, 6},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 4},
	// #endif

	// AC_M1Intensity,0x80,AC_M1WiseWidthSetPos,(W8|0x80),AC_M1TreadGoto,(LN|0x80),
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// #ifdef USE_M2
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 6},
	// #endif

	// AC_M1Intensity,SP,AC_M1TreadGoto,(LJW|0x80),
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	// #endif

	// AC_M1WiseSpeed,S3,
	// AC_M1Wise,RNZ,0x84, //* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// AC_M1TreadSpeed,M0,
	// AC_M1TreadGoto,LFC,AC_M1Wise,RNF,0x84, //* 5 风池穴揉捏
	{MoveUp_Knead, KNEAD_REV_CNT, 4, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	// #endif

	// AC_M1TreadGoto,LJW,AC_M1Wise,RNZ,0x84, //* 6 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 6},
	// #endif

	// AC_M1TreadSpeed,M3,
	// AC_M1WiseWidthSetPos,W5,AC_M1ResetGoto,LB1,
	// {MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55},	// * 修改 251007
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 6}, //* LB_4 -> LB_6
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
#endif

	// AC_M1TreadSpeed,	M0, //* 7 背部揉捏
	// AC_M1WiseSpeed,	S1,
	// AC_M1Intensity,	0,
	// AC_M1Wise,	RNZ,	0,
	// AC_M1TreadGoto,	LY3 | 0x80,
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_Move, 5, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3}, //* 顶出
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	// 	{MoveDw_Tap, FLAP_ACT, 0, 0, 6},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 5},
	// #endif

	// AC_M1Wise,	RPZ,	0x83, //* 8 臀部揉拍 其实是腰部揉拍 双机芯到不了臀部
	// AC_M1Wise,	RPF,	0x83,
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// // AC_M1TreadGoto,	LY2 | 0x80,
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// // AC_M1Wise,	RNZ,	0x83, //* 9 腰部揉捏
	// // AC_M1Wise,	RNF,	0x83,
	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	//* 9 腰部揉捏 修改 顶腰揉捏
	// {MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 2},
	{MoveUp_Knead, KNEAD_1F2R, 0xA8, 10, 0x62},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// {MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 2},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 40, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
#endif

	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH4, 0, 3},
	// 	{MoveDw_Tap, FLAP_ACT, 0, 0, 8},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 2},
	// #endif

	// AC_M1Wise,	RNF,	0, //* 10 背部揉捏
	// AC_M1TreadGoto,	LB1 | 0x80,
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},		//* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 3}, //* LB_4 -> LB_6
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 3},
	// 	{MoveDw_Tap, FLAP_ACT, 0, 0, 8},
	// 	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	// #endif

	// AC_M1WiseWidthSetPos,	W5,
	// AC_M1ResetGoto,	LUP,
	// FullMa1 END
	// {MoveUp_Knead, KNEAD_FIX_CIRCLE, 35, 0, 2},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* UpperAuto21
const CoreMixOtherTabItem_t SubTab_UpperAuto21[] = {
	// 1、检测完，肩颈两侧，先走一下
	// AC_M1_General,1,1,  LN,M2,0,TZ,S4,W0,0x80,MSJ,10,CMV,
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	//* 11 肩颈刮痧
	// AC_M1_General,4,4,  LFC,M4,0,TZ,S0,W0,SP,MSJ,10,CMV,
	// 					LJW,M4,0,TZ,S0,W0,SP,MSJ,10,CMV,
	// 					LJW,M4,0,TZ,S0,W4|0X80,SP,MSJ,10,CMV,
	// 					LJW,M4,0,TZ,S0,W0,SP,MSJ,10,CMV,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3}, // 10-5
	{Part_LOOP, 0, 4, 7, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// AC_M1_General,1,1,  LJWD,M4,0,TZ,S0,W0,20,MSJ,10,CMV,
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	//* 12 沿肩胛骨线推拿 体感差舍弃
	// AC_M1_General,4,3, 	LQS_DW,M4,12,TZ,S0,W0,0,MSJ,10,CMV,
	// 					LTZ,M4,15,TZ,S0,W4,0,MSJ,10,CMV,
	// 					LTZ,M4,15,TZ,S0,W0|0X80,0X80+20,MSJ,10,CMV,
	// 					LJWD,M4,0,TZ,S0,W0,0X80+20,MSJ,10,CMV,
	{Part_LOOP, 0, 3, 9, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 12, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0}, // 16
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3}, //* 太顶 20-10
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	//* 13 上背部推拿 没有特色 放普通程序
	// 2、然后颈椎要腰椎，从下到上，再从上到下，走几遍，刚开始可以速度稍微慢一点，后面可以速度加快；
	//  AC_M1_General,4,1,  LN,M4,0,TZ,S0,W0,SP,MSJ,10,CMV,
	//  					LB3,M4,0,TZ,S0,W0,SP,MSJ,10,CMV,
	//  					LN,M4,0,TZ,S0,W1,SP,MSJ,10,CMV,
	//  					LB3,M4,0,TZ,S0,W1,SP,MSJ,10,CMV,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0}, // 34

	// 3、前面预热的时间大概五分钟，再开始具体部位按摩；
	// 4、肩颈按摩时，由下往上提拉的时候，要在颈椎两侧，不能到脖子两边；
	// 5、肩颈揉捏后，再做敲击，速度不要太快，有节奏有力度的敲击

	// AC_M1_General,1,1,  LN,M2,0,TZ,S0,W1,0,MSJ,10,CMV,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	//* 14 肩部3D揉捏
	// AC_M1_General,2,6,  LJW,M4,0,TZ,S0,W4,0,MSJ,10,CMV,
	// 					LJW,M4,0,TZ,S0,W0,18,MSJ,10,CMV,
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{Part_LOOP, 0, 6, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2}, //*太顶 18-8
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// AC_M1_General,1,1,  LQS_DW,M4,2,TZ,S0,W0,10,MSJ,10,CMV,
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 2, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	//* 15 大椎拍打
	// AC_M1_General,2,2,  LTZ,M4,0,PDBS,S0,W0,10,MSJ,30,CMVA,
	// 					LTZ,M4,0,PDBS,S0,W1|0X80,10,MSJ,40,CMVA,
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},
	//* 16 上背部点揉
	// AC_M1_General,7,1,  LTZ,M2,4,RNZFS,S4,W0|0X80,0X96,MCY,8,CMVA,
	// 					LQS_DW,M2,2,TZ,S4,W0|0X80,0X0,MSJ,8,CMVA,
	// 					LTZ,M2,4,RNZFS,S4,W0|0X80,0X96,MCY,8,CMVA,
	// 					LQS_DW,M2,2,TZ,S4,W0|0X80,0X0,MSJ,8,CMVA,
	// 					LTZ,M2,4,RNZFS,S4,W0|0X80,0X96,MCY,8,CMVA,
	// 					LQS_DW,M2,2,TZ,S4,W0|0X80,0X0,MSJ,8,CMVA,
	// 					LTZ,M2,4,RNZFS,S4,W0|0X80,0X96,MCY,8,CMVA,
	{Part_LOOP, 0, 3, 8, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x53, 8, 0x66}, //* 太快 88-66
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},   //* 太顶 22-10
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},
	//* 17 上背部捶打
	// AC_M1_General,4,1,  LJW,M0,0,PDBS,S0,W0|0x80,10,MSJ,30,CMVA,
	// 					LQS_DW,M0,8,PDBS,S0,W0,10,MSJ,30,CMVA,
	// 					LJW,M0,0,PDBS,S0,W0,10,MSJ,30,CMVA,
	// 					LY3,M0,8,TZ,S0,W1|0X80,0,MSJ,40,CMVA,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	{MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
#endif

	//* 18 八髎穴捶打
	// AC_M1_General,5,2,   LY3,M4,0,TZ,S3,W1,0x9a,MSJ,10,CMVA,
	// 					 LY3,M4,0,PDBS,S3,W1,0x9a,MSJ,80,CMVA,
	// 					 LY3,M4,0,TZ,S3,W1,0xa5,MSJ,10,CMVA,
	// 					 LY3,M4,0,PDBS,S3,W1,0xa5,MSJ,80,CMVA,
	// 					 LY3,M4,0,TZ,S3,W1,0x80,MSJ,0,CMVA,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 6},
	{Part_LOOP, 0, 2, 12, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 80, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 80, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0}, // 93
	//* 19 腰部推拿
	// AC_M1_General,3,3,  LY1,M4,0,TZ,S0,W2,0x80,MSJ,8,CMVA,
	// 					LY1,M4,0,TZ,S0,W2,0xa5,MSJ,8,CMVA,
	// 					LY3,M4,0,TZ,S0,W2,0xa5,MSJ,8,CMVA,
	// UpperAuto21 END
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{Part_LOOP, 0, 3, 7, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0}, // 102

	{Part_STOP, 0, 0, 0, 0},
};

//* M1AT_WaistMaA
const CoreMixOtherTabItem_t SubTab_WaistMaA[] = {
	// AC_M1TreadSpeed,M2,AC_M1WiseWidthSetPos,W6|0x80,
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	// AC_LoopStart,3,
	// AC_M1Intensity,0xC0,AC_M1TreadGoto,LY1|0x80,
	// AC_M1Intensity,0xC0+28,AC_M1TreadGoto,LY3|0x80,
	// AC_LoopEnd,
	{Part_LOOP, 0, 3, 6, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_WaistMaB
const CoreMixOtherTabItem_t SubTab_WaistMaB[] = {
	//* 18 八髎穴捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_LOOP, 0, 2, 12, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 80, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 80, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 93
	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_WaistMaC
const CoreMixOtherTabItem_t SubTab_WaistMaC[] = {
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 3, 17, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0xA7, 4, 0x62},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_WaistMaD
const CoreMixOtherTabItem_t SubTab_WaistMaD[] = {
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H8, 0, 2}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 2, 10, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4}, //* H8->8
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0xA6, 4, 0x66},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 6},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x84},
	{Part_EXEC, ConMth_MoveUp, 60, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_WaistMaE
const CoreMixOtherTabItem_t SubTab_WaistMaE[] = {
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, //* H8->8
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 3, 14, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4}, //* H8->8
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H7, 0, 4}, //* H8->8
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackMaA
const CoreMixOtherTabItem_t SubTab_BackMaA[] = {
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 3, 17, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 6},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackMaB
const CoreMixOtherTabItem_t SubTab_BackMaB[] = {
	//* 捶打 +
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	// {Part_LOOP, 0, 2, 17, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 6},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2}, // @ 20-30 顶高一点 260407
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 6},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
#ifdef USE_SOUNDWAVE
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0}, // @ md 停留5秒 260424
#else
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#endif
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 35, 0, 2}, // @ 20-30 顶高一点 260407
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackMaC
const CoreMixOtherTabItem_t SubTab_BackMaC[] = {
	//* 捶打 +
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_LOOP, 0, 2, 17, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 36, 0, 6},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
#endif
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackMaD
const CoreMixOtherTabItem_t SubTab_BackMaD[] = {
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 2, 15, 0}, // @ 3->2回合 260119
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
#ifdef MOVEUP_3D_WEAK
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},
#elif defined USE_M2
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 2},
#else
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1}, // todo md 降低 260608
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0xA6, 5, 0x66},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
#ifdef USE_SOUNDWAVE
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0}, // @ md 停留5秒 260424
#else
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
#endif
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackMaE
const CoreMixOtherTabItem_t SubTab_BackMaE[] = {
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_LOOP, 0, 3, 15, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 6},
	// {MoveUp_Flex, FLEX_FOLLOW_KNEAD1, 0, 0, 1},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 1},	   // * md 48->40->30251206
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2}, //* H8->8
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0xa7, 5, 0x24},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 40, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 0xff, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_TNA
const CoreMixOtherTabItem_t SubTab_TNA[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 3, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_TNB
const CoreMixOtherTabItem_t SubTab_TNB[] = {
	{Part_LOOP, 0, 6, 8, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 4},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackA
const CoreMixOtherTabItem_t SubTab_BackA[] = {
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H7, 0, 6},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 4, 8, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, 25, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 25, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, 49, 0, 4},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD1, 0, 0, 4},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Flex, FLEX_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackB
const CoreMixOtherTabItem_t SubTab_BackB[] = {

	{Part_LOOP, 0, 5, 8, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, 20, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV_ABS, 50, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_BackC
const CoreMixOtherTabItem_t SubTab_BackC[] = {

	//* 20 全背部拔筋
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 1, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 25, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H7, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 1, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 1, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 35, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H8, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 1, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 40, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 4, 8, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackA
const CoreMixOtherTabItem_t SubTab_Neck2BackA[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 4},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackB
const CoreMixOtherTabItem_t SubTab_Neck2BackB[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackC
const CoreMixOtherTabItem_t SubTab_Neck2BackC[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 4},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackD
const CoreMixOtherTabItem_t SubTab_Neck2BackD[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 6},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD2, 0, 0, 4},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackE
const CoreMixOtherTabItem_t SubTab_Neck2BackE[] = {

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Tread, TREAD_REV, 0, 0, 2},
	{Part_LOOP, 0, 30, 6, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_BREAK, ConMth_TreadBMore, LB_H6, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Neck2BackF
const CoreMixOtherTabItem_t SubTab_Neck2BackF[] = {

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 3, 0x42},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_4, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 3, 0x42},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 3, 0x42},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 3, 0x42},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Back2NeckA
const CoreMixOtherTabItem_t SubTab_Back2NeckA[] = {

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* SubTab_Back2NeckB
const CoreMixOtherTabItem_t SubTab_Back2NeckB[] = {

	{Part_LOOP, 0, 30, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 35, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 45, 0, 5},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 10, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_BREAK, ConMth_TreadBLess, LB_H1, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* JNNeckA
const CoreMixOtherTabItem_t SubTab_JNNeckA[] = {
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{Part_LOOP, 0, 3, 5, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x54, 0xFF, 0x52}, // * 修改 0x55-0x53 251007
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2}, //* LB_2 -> LB_H2
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* JNNeckB
const CoreMixOtherTabItem_t SubTab_JNNeckB[] = {
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 6, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 2},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* JNNeckC
const CoreMixOtherTabItem_t SubTab_JNNeckC[] = {
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_FWD, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBLess, LB_H0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 15, 7, 0},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{MoveUp_Knead, KNEAD_FWD_ABS, MOVEUPWIDTHA, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 7, 30, 0},
	{MoveUp_Tread, TREAD_REV, 0, 0, 3},
	{MoveUp_Knead, KNEAD_REV_ABS, MOVEUPWIDTH9, 0, 7},
	{Part_BREAK, ConMth_TreadBMore, LB_H2, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* BJNeck
const CoreMixOtherTabItem_t SubTab_BJNeckA[] = {
	{Part_LOOP, 0, 3, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* BJNeckB
const CoreMixOtherTabItem_t SubTab_BJNeckB[] = {
	{Part_LOOP, 0, 3, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* BJNeckC
const CoreMixOtherTabItem_t SubTab_BJNeckC[] = {
	{Part_LOOP, 0, 5, 9, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 12, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 3, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* JRNeckA
const CoreMixOtherTabItem_t SubTab_JRNeckA[] = {
	{Part_LOOP, 0, 3, 14, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 16, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 1
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 2
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 24, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 3
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 10, 30, 0}, // 4
	{MoveUp_Tread, TREAD_REV_CIRCLE, 3, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 5

	{Part_STOP, 0, 0, 0, 0},
};

//* PDBSHDA
const CoreMixOtherTabItem_t SubTab_PDBSHDA[] = {
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_LOOP, 0, 3, 15, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tap, FLAP_ACT_CLK, 3, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 3, 30, 0},
	{MoveUp_Tap, FLAP_ACT_CLK, 5, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* PDBSHDB
const CoreMixOtherTabItem_t SubTab_PDBSHDB[] = {
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},

	{Part_LOOP, 0, 2, 32, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 6, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 6, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 4},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 4},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 4},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 8, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 4},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 6, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TNSHDA //* 搓肩
const CoreMixOtherTabItem_t SubTab_TNSHDA[] = {
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 8, 0},
	{MoveUp_Knead, KNEAD_1F2R, 0x57, 6, 0x66},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 2},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 5, 0, 4}, //* 往下
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* TNSHDB
const CoreMixOtherTabItem_t SubTab_TNSHDB[] = {

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{Part_LOOP, 0, 3, 18, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0xA4, 2, 0x85},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 3},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 3},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1 + 3, 0, 6},
	{MoveUp_Tread, TREAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 3},
	{MoveUp_Tread, TREAD_FOLLOW_KNEAD2, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TNSHDNeckA
const CoreMixOtherTabItem_t SubTab_TNSHDNeckA[] = {

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 5},
	{Part_LOOP, 0, 2, 12, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_3, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* TNBACKA
const CoreMixOtherTabItem_t SubTab_TNBACKA[] = {
	{Part_LOOP, 0, 3, 11, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* TNBACKB
const CoreMixOtherTabItem_t SubTab_TNBACKB[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_LOOP, 0, 6, 6, 0},
	{MoveUp_Tread, TREAD_FWD_CIRCLE, 20, 0, 2},
	{MoveUp_Flex, FLEX_FOLLOW_TREAD1, 0, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Tread, TREAD_REV_CIRCLE, 10, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_STOP, 0, 0, 0, 0},
};

//* TNBACKC
const CoreMixOtherTabItem_t SubTab_TNBACKC[] = {
	{Part_LOOP, 0, 2, 34, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H7, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPA
const CoreMixOtherTabItem_t SubTab_HIPA[] = {
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_11, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV, 0, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 30, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPB
const CoreMixOtherTabItem_t SubTab_HIPB[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{Part_LOOP, 0, 3, 10, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_11, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_10, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPC
const CoreMixOtherTabItem_t SubTab_HIPC[] = {
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_10, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 5, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 50, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_11, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 100, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPD
const CoreMixOtherTabItem_t SubTab_HIPD[] = {

	{Part_LOOP, 0, 2, 8, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_11, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV, 0, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPE
const CoreMixOtherTabItem_t SubTab_HIPE[] = {

	{Part_LOOP, 0, 2, 9, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_11, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{MoveUp_Knead, KNEAD_REV, 0, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_STOP, 0, 0, 0, 0},
};

//* HIPF
// const CoreMixOtherTabItem_t SubTab_HIPF[] = {
// 	{MoveUp_Tread, TREAD_FWD, 0, 0, 0},
// 	{MoveUp_Knead, KNEAD_FWD_CLK, 100, 0, 0},

// 	{Part_SUB, 0, SUBTB_TEST, 0, 0},
// 	{Part_EXEC, ConMth_MoveUp, 5, 10, 0},

// 	{Part_STOP, 0, 0, 0, 0},
// };

//* 子程序工作模式索引表
ActionTabAttr_t SubMode_Addr[] = {
	{SUBTB_CLOSE, NULL},
	{SUBTB_TEST, (void *)SubTab_TEST},
	{SUBTB_STRA, (void *)SubTab_STRA},
	{SUBTB_STRB, (void *)SubTab_STRB},
	{SUBTB_STRC, (void *)SubTab_STRC},
	{SUBTB_STRD, (void *)SubTab_STRD},
	{SUBTB_STRE, (void *)SubTab_STRE},
	{SUBTB_STRF, (void *)SubTab_STRF},
	{SUBTB_STR1, (void *)SubTab_STR1},
	{SUBTB_STR2, (void *)SubTab_STR2},
	{SUBTB_STR3, (void *)SubTab_STR3},
	{SUBTB_STR4, (void *)SubTab_STR4},
	{SUBTB_STR5, (void *)SubTab_STR5},
	{SUBTB_STR6, (void *)SubTab_STR6},
	{SUBTB_STR7, (void *)SubTab_STR7},
	{SUBTB_STR8, (void *)SubTab_STR8},
	{SUBTB_STR9, (void *)SubTab_STR9},
	{SUBTB_FullMa1, (void *)SubTab_FullMa1},
	{SUBTB_UpperAuto21, (void *)SubTab_UpperAuto21},
	{SUBTB_WaistMaA, (void *)SubTab_WaistMaA},
	{SUBTB_WaistMaB, (void *)SubTab_WaistMaB},
	{SUBTB_WaistMaC, (void *)SubTab_WaistMaC},
	{SUBTB_WaistMaD, (void *)SubTab_WaistMaD},
	{SUBTB_WaistMaE, (void *)SubTab_WaistMaE},
	{SUBTB_BackMaA, (void *)SubTab_BackMaA},
	{SUBTB_BackMaB, (void *)SubTab_BackMaB},
	{SUBTB_BackMaC, (void *)SubTab_BackMaC},
	{SUBTB_BackMaD, (void *)SubTab_BackMaD},
	{SUBTB_BackMaE, (void *)SubTab_BackMaE},
	{SUBTB_TNA, (void *)SubTab_TNA},
	{SUBTB_TNB, (void *)SubTab_TNB},
	{SUBTB_BackA, (void *)SubTab_BackA},
	{SUBTB_BackB, (void *)SubTab_BackB},
	{SUBTB_BackC, (void *)SubTab_BackC},
	{SUBTB_Neck2BackA, (void *)SubTab_Neck2BackA},
	{SUBTB_Neck2BackB, (void *)SubTab_Neck2BackB},
	{SUBTB_Neck2BackC, (void *)SubTab_Neck2BackC},
	{SUBTB_Neck2BackD, (void *)SubTab_Neck2BackD},
	{SUBTB_Neck2BackE, (void *)SubTab_Neck2BackE},
	{SUBTB_Neck2BackF, (void *)SubTab_Neck2BackF},
	{SUBTB_Back2NeckA, (void *)SubTab_Back2NeckA},
	{SUBTB_Back2NeckB, (void *)SubTab_Back2NeckB},
	{SUBTB_JNNeckA, (void *)SubTab_JNNeckA},
	{SUBTB_JNNeckB, (void *)SubTab_JNNeckB},
	{SUBTB_JNNeckC, (void *)SubTab_JNNeckC},
	{SUBTB_BJNeckA, (void *)SubTab_BJNeckA},
	{SUBTB_BJNeckB, (void *)SubTab_BJNeckB},
	{SUBTB_BJNeckC, (void *)SubTab_BJNeckC},
	{SUBTB_JRNeckA, (void *)SubTab_JRNeckA},
	{SUBTB_PDBSHDA, (void *)SubTab_PDBSHDA},
	{SUBTB_PDBSHDB, (void *)SubTab_PDBSHDB},
	{SUBTB_TNSHDA, (void *)SubTab_TNSHDA},
	{SUBTB_TNSHDB, (void *)SubTab_TNSHDB},
	{SUBTB_TNSHDNeckA, (void *)SubTab_TNSHDNeckA},
	{SUBTB_TNBACKA, (void *)SubTab_TNBACKA},
	{SUBTB_TNBACKB, (void *)SubTab_TNBACKB},
	{SUBTB_TNBACKC, (void *)SubTab_TNBACKC},
	{SUBTB_HIPA, (void *)SubTab_HIPA},
	{SUBTB_HIPB, (void *)SubTab_HIPB},
	{SUBTB_HIPC, (void *)SubTab_HIPC},
	{SUBTB_HIPD, (void *)SubTab_HIPD},
	{SUBTB_HIPE, (void *)SubTab_HIPE},
};

#define SubModeMaxAddr sizeof(SubMode_Addr) / sizeof(SubMode_Addr[0])

void SubMassMode_Load(u8 ID)
{
	u8 *temp;

	if (ID >= SubModeMaxAddr)
		return;

	temp = SubMode_Addr[ID].pTab;

	if (ID == SUBTB_CLOSE)
	{
		AT_TableStop(Table_Sub);
	}
	else
	{
		AT_TableStart(Table_Sub, temp);
		AtTabSet_ID(Table_Sub, ID);
	}
}

#endif // USE_M2_4D

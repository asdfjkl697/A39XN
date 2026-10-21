#include "Table.h"

#ifndef USE_M2_4D

// * 上机芯打包位置腰部
const CoreMixOtherTabItem_t Table_PACK[] = {
	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Motor_Flex, MotorPart_REV, 200, 0, 5},
	{Part_EXEC, ConMth_Motor, 10, 30, 0},

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero0, 0},

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_DB, DDG_LCircle_Zero0, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 10},
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH1, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 10},
#endif
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};
// * 老化 修改速度
#ifdef USE_M2
#define MOVEUP_LC 400
#else
#define MOVEUP_LC 640
#endif
const CoreMixOtherTabItem_t Table_LC[] = {
	{Part_AIR, PartCon_Run, FA_GAS_LC, 0, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 10},
	{MoveUp_Flex, FLEX_BACK_FORTH, 65, 0, 10},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 10},
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, (Follow_Space + 20) & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 10},
	{MoveDw_Tap, FLAP_ACT, 0, 0, 10},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{Part_LOOP, 0, 2, 28, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Flex, MotorPart_FWD, 150, 0, 5},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Flex, MotorPart_REV, 180, 0, 5}, // * md 150->180 251209
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero0, DDG_LCircle_Zero0, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},

	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
#else
	{Part_LOOP, 0, 2, 28, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Flex, MotorPart_FWD, 150, 0, 5},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Flex, MotorPart_REV, 150, 0, 5},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero0, DDG_LCircle_Zero0, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},

	{MoveUp_Tread, TREAD_FIX_CIRCLE, MOVEUP_LC & 0xff, MOVEUP_LC >> 8, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 0, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 5, 60, 0},
#endif

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 复位动作表
const CoreMixOtherTabItem_t Table_Reset[] = {
	{Part_IF, ConMth_KneadZero, 0, 0, 0}, // * 判断是否揉捏找零 251105
#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, 0, 0, 5}, //*揉捏找0
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 0, 0, 5}, //*揉捏找0 251105
	{Part_EXEC, ConMth_Move, 2, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 3},
	{MoveDw_Tread, TREAD_FIX_CIRCLE, 0, 0, 5},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 0, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Move, 5, 30, 0},

	{Part_ASSIGN, ASSIGN_Mark, 0, 0, EXEC_TurnOn}, // @ 开机复位完成 260129

#ifdef USE_M2
	{MoveDw_Tread, TREAD_FIX_CIRCLE, 180, 0, 5}, // @ 150->200->240->180 提高一点 260603
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 3},
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 100, 0, 5},
	{Part_EXEC, ConMth_Move, 5, 30, 0},

	{Part_EXEC, ConMth_AD, 5, 30, EXEC_Reset}, // * 结束标志 抛出结束标志
};

//@ 肩位检测动作表
// const u16 BodyCheck_Low = 400; //* 600
#ifdef USE_M2
#define BodyCheck_Low 400
#else
#define BodyCheck_Low 520
#endif
const CoreMixOtherTabItem_t Table_BodyCheck[] = {
#ifdef USE_M2
	// {MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH5, 0, 3},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH6, 0, 3}, // @ 消除咯屁股 260313
	{MoveDw_Tread, TREAD_FIX_CIRCLE, 10, 0, 5},			  // * 咯屁股 251205
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3},

	{MoveUp_Tread, TREAD_FIX_CIRCLE, BodyCheck_Low & 0xff, BodyCheck_Low >> 8, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Move, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 10, 0, 3}, //* 中途顶一下
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 350 & 0xff, 1, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, 10, 0, 3}, //* 2-5-10-12-10 251107
	{MoveUp_Tread, TREAD_FIX_CIRCLE, 200, 0, 8},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 15, 0, 3}, // * 15->10 251216
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},	  // * 测试 边走边测试 251104

	{MoveUp_Tread, TREAD_FIND_SDR, 0, 0, 1},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_STOP, 0, 0, 3},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_EXEC, ConMth_AD, 5, 30, EXEC_BodyCheck}, // * 结束标志 抛出结束标志
};

//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
//@ 零压舒展
const CoreMixOtherTabItem_t Table_Auto1[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开启足底滚轮
#ifdef USE_M2
	// {Part_MVDW, PartCon_Stop, 0, 0, 0},
	// {MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	// {MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	// {MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0},
	{Part_MVDW, PartCon_Run, DWTB_FullA, 0, 0},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0}, // * 释放气压 251215
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸E

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},		   // @ speed 2->3 260117
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_BJNeckC, 0, 0},
	// {Part_SUB, 0, SUBTB_JRNeckA, 0, 0}, // @ 肩部提抓 260312
	{Part_SUB2, ConMth_Loop, SUB2TB_TLB, 3, 0}, // @ md 260727

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	// {MoveDw_TapF, FOLW_COPY, 0, 0, 0}, //@ 测试 260313
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // @ 交换BackMaD 260117
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
// {Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007 54->65
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
	// //* 下机芯跟随上机芯
	// {MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	// {MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	// // {MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	// // {MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	// {MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	// {Part_EXEC, ConMth_MoveDw, 0, 30, 0},

	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	{Part_MVDW, PartCon_Run, DWTB_FullA, 0, 0},
#endif

	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D
	{Part_SUB, 0, SUBTB_STRC, 0, 0}, // * 拉伸C

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// {Part_SUB, 0, SUBTB_BJNeckC, 0, 0}, // * 拔筋B
	// {Part_SUB, 0, SUBTB_JRNeckA, 0, 0}, // @ add 260312

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 15 大椎拍打
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	// #ifdef USE_M2
	// 	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	// 	{MoveDw_Knead, KNEAD_REV, 0, 0, 4},
	// 	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
	// #endif

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚
	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_REV, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
	// {MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_FIX_CIRCLE, MOVEDWWIDTH6, 0, 3}, // @ 消除咯屁股 260313
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0},				  //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_BackMaD, 0, 0}, // @ 交换BackMaB 260117
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPA, 0, 0},
#endif

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@  中式推拿
const CoreMixOtherTabItem_t Table_Auto2[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH4, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 10
	{Part_SUB, 0, SUBTB_BJNeckC, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackE, 0, 0},

	{Part_SUB, 0, SUBTB_BackC, 0, 0},
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	// {MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_Back2NeckB, 0, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0}, // * 颈到背 251109

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaD, 0, 0}, // * 背部1 251109

#ifdef USE_M2
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckB, 0, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackC, 0, 0}, // * 颈到背 251109

	{Part_SUB, 0, SUBTB_TNBACKC, 0, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, (MOVEUPWIDTH1 + MOVEUPWIDTH2) >> 1, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2}, //* 顶出收回
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};
//@ 疲劳恢复
const CoreMixOtherTabItem_t Table_Auto3[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDB, 0, 0}, // * 肩部变速拍B
	// {Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 5},
	{MoveUp_Flex, FLEX_BACK_FORTH, 10, 0, 5},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},

#ifdef USE_M2
	{MoveUp_Knead, KNEAD_1F2R, 0x97, 5, 0x62},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 48, 0, 6},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 5},
	{MoveUp_Flex, FLEX_FOLLOW_KNEAD2, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109

#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, //* 18 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif

	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_STRB, 0, 0}, // * 拉伸B

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 海浪轻柔
const CoreMixOtherTabItem_t Table_Auto4[] = {
	{Part_ASSIGN, ASSIGN_Flex, 3, 0, 0},  // @ 设置M1flex 251219
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_FullB, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x75},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x75},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 25, 3}, // @ 摇摆 260702
	{Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 13, 5}, // @ 摇摆 260820

	// {Part_SUB, 0, SUBTB_Neck2BackD, 0, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x75, 0xff, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaE, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2F, 0x93, 5, 0x73}, // * 腰部揉
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x44}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 25, 3},
	{Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 13, 5},

	{MoveUp_Knead, KNEAD_FWD_CNT, 5, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0},	   //* 搓肩
	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0}, // * 颈到背B

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	// {Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
	{Part_SUB, 0, SUBTB_BackB, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x76, 0xFF, 0x44},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 10, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * 揉凤池穴
	// {MoveUp_Knead, KNEAD_1F2R, 0x96, 5, 0x74},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 5},
	// {MoveUp_Flex, FLEX_FIX_CIRCLE, 8, 0, 3},
	// {Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_SUB2, ConMth_Loop, SUB2TB_RNFN, 5, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 124 复位
};
//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
//@ 泰式拉筋
const CoreMixOtherTabItem_t Table_Auto5[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},

	// {Part_SUB, 0, SUBTB_STR6, 0, 0}, // todo rm Stretch6 拉伸不用 STR6 261011
	{Part_SUB, 0, SUBTB_STR1, 0, 0}, // * Stretch1
	{Part_SUB, 0, SUBTB_STR5, 0, 0}, // * Stretch5

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0},
	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},		//* 打开足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0}, // 10

	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// 	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	// 	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// 	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	// #ifdef USE_M2
	// 	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	// 	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
	// #endif
	// 	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
	// #ifndef USE_M2
	// 	{Part_SUB, 0, SUBTB_HIPB, 0, 0},
	// #endif

	// 	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	// 	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	// 	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 2},
	// 	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 15 大椎拍打
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
// {MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
#endif
	{Part_SUB, 0, SUBTB_STRF, 0, 0},
	{Part_SUB, 0, SUBTB_STR3, 0, 0}, // * Stretch3
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0},
#ifndef A391S_37
// {Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸A
#endif
	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},

//*  变速拍 往下
#ifdef USE_M2
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x73},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
#else
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部2 251109
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0},
};

//@ 舒筋活络
const CoreMixOtherTabItem_t Table_Auto6[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表

	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JRNeckA, 0, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	// {Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},
	{Part_SUB, 0, SUBTB_Neck2BackF, 0, 0},

	{Part_SUB, 0, SUBTB_BackC, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPA, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, //* 18 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_SUB, 0, SUBTB_STR2, 0, 0}, // * Stretch2
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 肩颈护理 250326
const CoreMixOtherTabItem_t Table_Auto7[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckC, 0, 0},

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

	// {Part_SUB, 0, SUBTB_BJNeckB, 0, 0},
	{Part_SUB, 0, SUBTB_JRNeckA, 0, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckC, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A
	{Part_SUB, 0, SUBTB_TNSHDB, 0, 0}, // * 推压肩部

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_SUB, 0, SUBTB_BJNeckA, 0, 0},
	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	{Part_SUB, 0, SUBTB_TNA, 0, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaE, 0, 0}, // * 背部1 251109

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 提臀美腿
const CoreMixOtherTabItem_t Table_Auto8[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
	{Part_SUB, 0, SUBTB_HIPA, 0, 0},
#endif
	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	{Part_SUB, 0, SUBTB_STRF, 0, 0}, // * 拉伸F
	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D
	{Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸B

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 腰部护理 250318
const CoreMixOtherTabItem_t Table_Auto9[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 4, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_Neck2BackD, 0, 0},

	{Part_SUB, 0, SUBTB_WaistMaD, 0, 0},
	{Part_SUB, 0, SUBTB_WaistMaE, 0, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 4, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaE, 0, 0},
	{Part_SUB, 0, SUBTB_WaistMaC, 0, 0}, // * 腰部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109

	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 腿足舒缓
const CoreMixOtherTabItem_t Table_Auto10[] = {

	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_SUB, 0, SUBTB_STRF, 0, 0},

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表

	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D
	{Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸B

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

// * 天伦养生 老年人修改 更安全一点 250429 PY02->PYH1
const CoreMixOtherTabItem_t Table_Auto11[] = {
	{Part_ASSIGN, ASSIGN_Flex, 3, 0, 0},  // @ 设置M1flex 251219
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_FullA, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x74},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x44},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x74},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	// {Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x75, 0xff, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BackB, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x44}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x74},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x73},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	// {Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 10, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//* 总裁释压 //* 零压舒展 251007
const CoreMixOtherTabItem_t Table_Auto12[] = {
	{Part_ASSIGN, ASSIGN_KneadSpeed, 2, 0, 0}, // @ 增加揉捏速度 260119
	{Part_ASSIGN, ASSIGN_TapSpeed, 2, 0, 0},   // @ 增加拍打速度 260119
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},	   //* 开启足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0}, // * 释放气压 251215
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸E

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0},
	{Part_MVDW, PartCon_Run, DWTB_0RPZ, 0, 0},
// {Part_MVDW, PartCon_Run, DWTB_FullB, 0, 0}, //@ md 260312
#endif

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},		   // @ speed 2->3 260117
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_BJNeckC, 0, 0},
	{Part_SUB, 0, SUBTB_JRNeckA, 0, 0}, // @ 颈部提抓 260312

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	// {MoveDw_TapF, FOLW_COPY, 0, 0, 0}, //@ 测试 260313
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // @ 交换BackMaD 260117
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPA, 0, 0},
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007 54->65
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
	//* 下机芯跟随上机芯
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D
	{Part_SUB, 0, SUBTB_STRC, 0, 0}, // * 拉伸C

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckC, 0, 0}, // * 拔筋B
	// {Part_SUB, 0, SUBTB_JRNeckA, 0, 0}, // @ add 260312

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 15 大椎拍打
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

#ifdef USE_M2
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_REV, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚
	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_BackMaD, 0, 0}, // @ 交换BackMaB 260117
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, //* 18 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 女王纤体 250326
const CoreMixOtherTabItem_t Table_Auto13[] = {

#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	// {Part_MVDW, PartCon_Run, DWTB_SWPD1, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 5},
#endif
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开启足底滚轮
	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_SUB, 0, SUBTB_STRA, 0, 0}, // * 拉伸A

#ifdef USE_M2
	// {MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0}, // * 设置下机芯拍打复制
	// {MoveDw_KneadF, FOLW_COPY, 0, 0, 0}, // * 设置下机芯揉捏复制
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
#endif

	{Part_AIR, PartCon_Run, FA_GAS_FullA, 0, 0}, // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},		 //* 开启足底滚轮

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_BJNeckA, 0, 0},
	{Part_SUB, 0, SUBTB_JRNeckA, 0, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//* 5 风池穴揉捏
	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//* 6 肩部揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	//*  肩部反向变速揉捏
	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x75, 0xff, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
// {MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
// {MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
#endif

	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
#endif

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
	//* 下机芯跟随上机芯
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	// {MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
#endif

	{Part_SUB, 0, SUBTB_STRB, 0, 0}, // * 拉伸B

	{Part_AIR, PartCon_Run, FA_GAS_FullA, 0, 0}, // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},		 //* 开启足底滚轮

	// 1、检测完，肩颈两侧，先走一下
	// AC_M1_General,1,1,  LN,M2,0,TZ,S4,W0,0x80,MSJ,10,CMV,
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_BJNeckB, 0, 0}, // * 拔筋A

	//* 15 大椎拍打
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

#ifdef USE_M2
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_REV, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	// {Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部2 251109

	// {Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 深度脊柱护理
const CoreMixOtherTabItem_t Table_Auto14[] = {

// * --- Phase 1: 准备与热身 ---
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif
	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},	  // * 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开启足底滚轮
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	// * 全背热身揉捏
	// {MoveUp_Knead, KNEAD_FWD, 0, 0, 4},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸E

	// * --- Phase 2: 肩颈深度松解 (Zero G 3) ---
	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档

	// * 颈肩正反向顶揉
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 3},		   // @ speed 2->3 260117
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * 机芯收窄过渡
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Part_SUB, 0, SUBTB_JRNeckA, 0, 0}, // @ 颈部提抓
	{Part_SUB2, ConMth_Loop, SUB2TB_TLB, 3, 0}, // * 提拉颈部

	// * 肩部集中揉捏
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// * 风池穴揉捏
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckC, 0, 0}, // * 肩部拔筋

// * --- Phase 3: 脊柱深度推拿 (Zero G 3→2) ---
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0}, // * 颈到背推拿

	{Part_SUB, 0, SUBTB_BackC, 0, 0}, // * 全背部拔筋

	// * 脊椎慢速捶打 LB_5→LB_9 循环2次
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_LOOP, 0, 2, 4, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 8, 30, 0},

	// * 转零重力2档
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0},

	{Part_SUB, 0, SUBTB_BackMaD, 0, 0}, // * 背部推拿
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#else
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	// * 宽幅揉捏过渡
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

// * --- Phase 4: 腰臀护理 (Zero G 2) ---
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 下机芯行走跟随 避免碰撞
	{MoveDw_KneadF, FOLW_COPY, 0, 0, 0},									// * 下机芯揉捏跟随
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
#endif

	// * 腰部分段3D揉拍 — 第一段 LB_7
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Flex, FLEX_BACK_FORTH, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// * 腰部分段3D揉拍 — 第二段 LB_8
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Flex, FLEX_BACK_FORTH, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// * 腰部分段3D揉拍 — 第三段 LB_9
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 20, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FWD_CNT, 4, 0, 4},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Flex, FLEX_BACK_FORTH, 30, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// * 腰部往返揉捏
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 8},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_REV, 0, 0, 8},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},
	{MoveUp_Knead, KNEAD_FWD, 0, 0, 8},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

// * --- Phase 5: 拉伸与收尾放松 (Zero G 2) ---
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
#endif

	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},		//* 开启足底滚轮

	// * 上背部变速捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部拍打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPA, 0, 0}, // * 臀部放松
#endif

	// * 最终放松揉捏
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 御宅充电 原来的中式推拿
const CoreMixOtherTabItem_t Table_Auto15[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_1R2F, 0x5A, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackC, 0, 0},
// {Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x52}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//* 元气唤醒
const CoreMixOtherTabItem_t Table_Auto16[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随
	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109

	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 午间小憩
const CoreMixOtherTabItem_t Table_Auto17[] = {
	{Part_ASSIGN, ASSIGN_Flex, 3, 0, 0},  // @ 设置M1flex 251219
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_FullB, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 25, 2}, // @ 摇摆 260702
	{Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 13, 4}, // @ 摇摆 260702

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x44},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x74},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackB, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x44},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2}, //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 25, 5}, // @ 摇摆 260702
	{Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 13, 8}, // @ 摇摆 260702

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档
	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaE, 0, 0}, // * 背部1 251109

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x42},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaE, 0, 0}, // * 背部1 251109
	// {Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_BACK_FORTH, 10, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 温热祛湿
const CoreMixOtherTabItem_t Table_Auto18[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开启足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	// * 拉伸
	{Part_SUB, 0, SUBTB_STRE, 0, 0}, // * 拉伸A

#ifdef USE_M2
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0},
	{Part_MVDW, PartCon_Run, DWTB_0RPZ, 0, 0},
#endif

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚

	//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH5, 0, 5},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckC, 0, 0},

	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackMaD, 0, 0}, // * 背部1 251109

	{Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

#ifdef USE_M2
	//* 下机芯跟随上机芯
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	// {MoveDw_KneadF, FOLW_FIX2KNEAD, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_STRD, 0, 0}, // * 拉伸D
	{Part_SUB, 0, SUBTB_STRC, 0, 0}, // * 拉伸B

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},								  // * 调用气压表
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},									  //* 开启足底滚轮
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_BJNeckA, 0, 0}, // * 拔筋A

	//* 4 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 15 大椎拍打
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_LOOP, 0, 2, 6, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0}, //*53
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH3, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},

#ifdef USE_M2
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_REV, 0, 0, 4},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档 不找脚
	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

#ifdef USE_M2
	{MoveDw_Knead, KNEAD_STOP, 0, 0, 0},
	{MoveDw_TreadF, FOLW_STOP, 0, 0, 0}, //* 放弃行走跟随 251007
	{MoveDw_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 8},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0},
#endif

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部2 251109

	//* 18 八髎穴捶打
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif

	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档 不找脚

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 运动辅助
const CoreMixOtherTabItem_t Table_Auto19[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0},	 // * 背部1 251109
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_SUB, 0, SUBTB_STR3, 0, 0}, // * Stretch3
	{Part_SUB, 0, SUBTB_STR2, 0, 0}, // * Stretch2
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位

};

//* 安神助眠
const CoreMixOtherTabItem_t Table_Auto20[] = {
	{Part_ASSIGN, ASSIGN_Flex, 3, 0, 0},  // @ 设置M1flex 251219
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 4},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_FullB, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 25, 3}, // @ 摇摆 260702
	{Motor_Sway, AUTO_SIT_ZERO1N, AUTO_SIT_ZERO3N, 13, 5}, // @ 摇摆 260702

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x44},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x74},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	// {Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{MoveUp_Knead, KNEAD_1F2R, 0x75, 0xff, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_BackB, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x44}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// {Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 25, 5}, // @ 摇摆 260702
	{Motor_Sway, AUTO_SIT_ZERO3N, AUTO_SIT_ZERO2N, 13, 8}, // @ 摇摆 260702

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档
	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaE, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档
	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_ACT, 0, 0, 2},
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x42},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},

	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaE, 0, 0}, // * 背部1 251109
	// {Part_SUB, 0, SUBTB_TNBACKB, 0, 0},
	{Part_SUB, 0, SUBTB_TNBACKA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_BACK_FORTH, 10, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 御手解忧
const CoreMixOtherTabItem_t Table_Auto21[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0},	 // * 背部1 251109
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位

};

//@ 正脊正骨
const CoreMixOtherTabItem_t Table_Auto22[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPD, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0},	 // * 背部1 251109
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位

};

//@ 固本培元
const CoreMixOtherTabItem_t Table_Auto23[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位

};

//@ 调和阴阳
const CoreMixOtherTabItem_t Table_Auto24[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//* 酸痛改善
const CoreMixOtherTabItem_t Table_Auto25[] = {
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 关闭足底滚轮
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0}, // * 设置下机芯行走跟随

	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0}, // * 设置下机芯停止复制
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif

	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0}, // 10

	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0}, // * 调用气压表
	//* 1 肩部揉捏
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 5, 0x95},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * RNZFS
	{MoveUp_Knead, KNEAD_1F2R, 0x92, 3, 0x55},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 10, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_1R2R, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_JNNeckA, 0, 0},

	{Part_SUB, 0, SUBTB_Neck2BackA, 0, 0},

	{Part_SUB, 0, SUBTB_BackA, 0, 0},
#ifndef USE_M2
	{Part_SUB, 0, SUBTB_HIPE, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x55}, // * 修改 251007
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},	  //* 顶出收回
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_0, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDNeckA, 0, 0}, // * 推拿肩颈A

	{MoveUp_Knead, KNEAD_REV_CNT, 3, 0, 6},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_TNSHDA, 0, 0}, //* 搓肩

	{Part_SUB, 0, SUBTB_Neck2BackB, 0, 0},

#ifdef USE_M2
	{MoveDw_TapF, FOLW_COPY, 0, 0, 0},
	{Part_EXEC, ConMth_MoveDw, 0, 30, 0}, // 10
#endif
	{Part_SUB, 0, SUBTB_BackMaA, 0, 0}, // * 背部1 251109

	{MoveUp_Knead, KNEAD_1F2R, 0x65, 0xFF, 0x22},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_Knead, KNEAD_FWD_CNT, 3, 0, 6},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_LJA, 3, 0},

	// 颈肩部正反向顶揉3次，//* 3 肩部按压 拍
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4}, //* LB_4 -> LB_6
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_Tap, FLAP_ACT, 0, 0, 2},		   //*调试
	{MoveUp_Knead, KNEAD_1F2F, 0xa5, 3, 0x37}, // * 没感觉
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	{MoveUp_Knead, KNEAD_1R2R, 0x5a, 3, 0x73},
	{Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	{Part_SUB, 0, SUBTB_PDBSHDA, 0, 0}, // * 肩部变速拍A

	// * 变速揉
	{MoveUp_Knead, KNEAD_1F2F, 0x95, 3, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 0, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	//* 17 上背部捶打
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 2},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 2},
	{MoveUp_Tap, FLAP_BS, 30, 30, 0x95},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0}, // 70
	// {MoveUp_Tread, TREAD_REV_CIRCLE, 8, 0, 2},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 30, 30, 0},
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH2, 0, 2},
	// {MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_H6, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 40, 30, 0},

	{Part_SUB, 0, SUBTB_BackMaB, 0, 0}, // * 背部1 251109
#ifdef USE_M2
	{Part_SUB, 0, SUBTB_WaistMaB, 0, 0}, // * 腰部2 八髎穴捶打
#else
	{Part_SUB, 0, SUBTB_HIPC, 0, 0},
#endif
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 8},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//@ 新体感
const CoreMixOtherTabItem_t Table_NewSense[] = {

// * --- Phase 1: 热身 (不对称揉捏) ---
#ifdef USE_M2
	{Part_MVDW, PartCon_Stop, 0, 0, 0},
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_TapF, FOLW_STOP, 0, 0, 0},
	{MoveDw_KneadF, FOLW_STOP, 0, 0, 0},
	{MoveDw_Knead, KNEAD_FWD, 0, 0, 6},
#endif
	{Part_AIR, PartCon_Stop, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},	  // * 释放气压
	{Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开启足底滚轮
	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 5},
	{MoveUp_Flex, FLEX_FIX_CIRCLE, 6, 0, 3},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 4},
	{Part_EXEC, ConMth_MoveUp, 20, 30, 0},

	// * RNZFD 全背热身
	// {Part_SUB2, ConMth_Loop, SUB2TB_RNZFD, 4, 0}, // * 不对称揉捏
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_6, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},
	// {Part_SUB2, ConMth_Loop, SUB2TB_RNZFD, 3, 0},
	// {MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	// {Part_EXEC, ConMth_MoveUp, 5, 30, 0},

	// * --- Phase 2: 肩颈新体感 (Zero G 3) ---
	{Part_AIR, PartCon_Run, FA_GAS_Full, 0, 0},
	{Motor_Feet, MotorPart_FWD, 0, 0, 0},
	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero3, DDG_LCircle_Zero3, 0}, //* 零重力3档

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4}, // * 窄幅
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_1, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// {Part_SUB2, ConMth_Loop, SUB2TB_TLB, 5, 0},	 // * 提拉颈部
	{Part_SUB2, ConMth_Loop, SUB2TB_TNFN, 5, 0}, // * OK
	{Part_SUB2, ConMth_Loop, SUB2TB_RNFN, 5, 0}, // *
	// {Part_SUB2, ConMth_Loop, SUB2TB_PLSE, 4, 0}, // * 脉冲揉捏
	// {Part_SUB2, ConMth_Loop, SUB2TB_TLD, 3, 0}, // * 肩部揉按

	// * --- Phase 3: 背部新体感 (Zero G 2) ---
	// {Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4}, // * 窄幅 肩胛骨保护
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_5, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_TKSTG, 3, 0}, // * 拍揉交替
	{Part_SUB2, ConMth_Loop, SUB2TB_RMPUP, 3, 0}, // * 渐快揉捏
	{Part_SUB2, ConMth_Loop, SUB2TB_TLH, 2, 0},	  // * 背部推揉 窄幅

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_7, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_INWAV, 3, 0}, // * 力度波浪

// * --- Phase 4: 腰部新体感 (Zero G 2) ---
#ifdef USE_M2
	{MoveDw_TreadF, FOLW_SPACE, Follow_Space & 0xff, Follow_Space >> 8, 0},
	{MoveDw_KneadF, FOLW_COPY, 0, 0, 0},
	{MoveDw_Tap, FLAP_STOP, 0, 0, 0},
#endif

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 4},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_8, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	{Part_SUB2, ConMth_Loop, SUB2TB_TLF, 3, 0}, // * 腰部顶揉

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 2},
	{Part_EXEC, ConMth_MoveUp, 2, 30, 0},

	// * --- Phase 5: 收尾 (Zero G 2) ---
	{Motor_Zero, MotorPart_Stretch, DDG_BCircle_Zero2, DDG_LCircle_Zero2, 0}, //* 零重力2档

	{MoveUp_Knead, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3},
	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_2, 0, 3},
	{MoveUp_Flex, FLEX_BACK_FORTH, 15, 0, 5},
	{MoveUp_Tap, FLAP_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{MoveUp_TreadB, TREAD_FIX_CIRCLE, LB_9, 0, 3},
	{Part_EXEC, ConMth_MoveUp, 0, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

ActionTabAttr_t MassMode_Addr[] = {
	{FA_AUTO_CLOSE, NULL},
#ifdef VIETNAM_VOICE
	{FA_AUTO_AUTO1, (void *)Table_Auto1},	//* 舒适按摩 零压舒展
	{FA_AUTO_AUTO2, (void *)Table_Auto2},	//* 轻松按摩 中式推拿 推拿为主
	{FA_AUTO_AUTO3, (void *)Table_Auto5},	//* 牵引按摩 泰式拉筋
	{FA_AUTO_AUTO4, (void *)Table_Auto9},	//* 背腰按摩 腰部护理
	{FA_AUTO_AUTO5, (void *)Table_Auto7},	//* 肩颈按摩 肩颈护理
	{FA_AUTO_AUTO6, (void *)Table_Auto16},	//* 全身按摩 元气唤醒 拍打为主
	{FA_AUTO_AUTO7, (void *)Table_Auto14},	//* 护椎按摩 白领专享
	{FA_AUTO_AUTO8, (void *)Table_Auto3},	//* 疲劳恢复 疲劳恢复
	{FA_AUTO_AUTO9, (void *)Table_Auto6},	//* 舒筋活络 舒筋活络
	{FA_AUTO_AUTO10, (void *)Table_Auto13}, //* 久坐舒展 女王纤体
	{FA_AUTO_AUTO11, (void *)Table_Auto20}, //* 安神助眠 安神助眠
	{FA_AUTO_AUTO12, (void *)Table_Auto11}, //* 休闲养生 天伦养生
	{FA_AUTO_AUTO13, (void *)Table_Auto19}, //* 运动放松 运动辅助
	{FA_AUTO_AUTO14, (void *)Table_Auto17}, //* 颐神养性 午间小憩
	{FA_AUTO_AUTO15, (void *)Table_Auto15}, //* 平衡身心 御宅充电
	{FA_AUTO_AUTO16, (void *)Table_Auto4},	//* 海浪轻柔 海浪轻柔 揉捏为主
	{FA_AUTO_AUTO17, (void *)Table_Auto8},	//* 提臀美腿 提臀美腿
	{FA_AUTO_AUTO18, (void *)Table_Auto12}, //* 总裁释压 总裁释压
	{FA_AUTO_AUTO19, (void *)Table_Auto10}, //* 腿足舒缓 腿足舒缓
	{FA_AUTO_AUTO20, (void *)Table_Auto18}, //* 腰臀按摩 温热祛湿 待修改
	{FA_AUTO_AUTO21, (void *)Table_Auto21}, //* 御手解忧 御手解忧
	{FA_AUTO_AUTO22, (void *)Table_Auto22}, //* 正脊正骨 正脊正骨
	{FA_AUTO_AUTO23, (void *)Table_Auto23}, //* 固本培元 固本培元
	{FA_AUTO_AUTO24, (void *)Table_Auto24}, //* 调和阴阳 调和阴阳
	{FA_AUTO_AUTO25, (void *)Table_Auto25}, //* 酸痛改善 酸痛改善
#else
	{FA_AUTO_AUTO1, (void *)Table_Auto1},
	{FA_AUTO_AUTO2, (void *)Table_Auto2},
	{FA_AUTO_AUTO3, (void *)Table_Auto3},
	{FA_AUTO_AUTO4, (void *)Table_Auto4},
	{FA_AUTO_AUTO5, (void *)Table_Auto5},
	{FA_AUTO_AUTO6, (void *)Table_Auto6},
	// {FA_AUTO_AUTO6, (void *)Table_NewSense}, //* 新体感 (屏蔽旧程序)
	{FA_AUTO_AUTO7, (void *)Table_Auto7},
	{FA_AUTO_AUTO8, (void *)Table_Auto8},
	{FA_AUTO_AUTO9, (void *)Table_Auto9},
	{FA_AUTO_AUTO10, (void *)Table_Auto10},
	{FA_AUTO_AUTO11, (void *)Table_Auto11},
	{FA_AUTO_AUTO12, (void *)Table_Auto12},
	{FA_AUTO_AUTO13, (void *)Table_Auto13},
	{FA_AUTO_AUTO14, (void *)Table_Auto14},
	{FA_AUTO_AUTO15, (void *)Table_Auto15},
	{FA_AUTO_AUTO16, (void *)Table_Auto16},
	{FA_AUTO_AUTO17, (void *)Table_Auto17},
	{FA_AUTO_AUTO18, (void *)Table_Auto18},
	{FA_AUTO_AUTO19, (void *)Table_Auto19},
	{FA_AUTO_AUTO20, (void *)Table_Auto20},

	{FA_AUTO_AUTO21, (void *)0x0803b800}, // * 待调试 250412
	{FA_AUTO_AUTO22, (void *)0x0803c000},
	{FA_AUTO_AUTO23, (void *)0x0803c800},
	{FA_AUTO_AUTO24, (void *)0x0803d000},
	{FA_AUTO_AUTO25, (void *)0x0803d800},
#endif

	{A_LC, (void *)Table_LC},
	{A_PACK, (void *)Table_PACK},
	{A_Reset, (void *)Table_Reset},
	{A_BodyCheck, (void *)Table_BodyCheck},

};

#define MassModeMaxAddr sizeof(MassMode_Addr) / sizeof(MassMode_Addr[0]) // * 修改 250406

void MassMode_Load(u8 ID)
{
	u8 *temp;

	if (ID >= MassModeMaxAddr)
		return;

	temp = MassMode_Addr[ID].pTab;

	if (ID == FA_AUTO_CLOSE)
	{
		AT_TableStop(Table_Auto);
		AT_TableStop(Table_Sub);  // * 250919
		AT_TableStop(Table_Sub2); // todo add 260616
		AT_TableStop(Table_Air);
#ifdef USE_M2
		AT_TableStop(Table_MoveDw);
#endif
	}
	else
	{
		AT_TableStart(Table_Auto, temp);
		AtTabSet_ID(Table_Auto, ID); // * ADD 250903
	}
}

#endif // USE_M2_4D

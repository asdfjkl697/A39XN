#include "Table.h"

//* 老化伴随程序
const CoreMixOtherTabItem_t GasTab_LC[] = {
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_2POS, 0x51, 5, 0x21},
#endif
	{Air_FeetS, AirPart_ModeB, 50, 10, 40},
	{Air_Ankle, AirPart_ModeB, 50, 10, 40},
	{Air_CalfS, AirPart_STOP, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_STOP, 0, 0, 0},
#endif
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_SHD, AirPart_STOP, 0, 0, 0},
#if (AIR_SEAT)
	{Air_Seat, AirPart_STOP, 0, 0, 0},
#endif
	{Part_EXEC, ConMth_Air, 20, 30, 0},
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Air_Ankle, AirPart_STOP, 0, 0, 0},
	{Air_CalfS, AirPart_ModeB, 50, 10, 40},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 50, 10, 40},
#endif
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Air_SHD, AirPart_ModeB, 50, 10, 40},
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 50, 10 + SEAT_AIR_WEAK, 40},
#endif
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Air_FeetS, AirPart_ModeB, 50, 10, 40},
	{Air_Ankle, AirPart_ModeB, 50, 10, 40},
	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Air_Ankle, AirPart_STOP, 0, 0, 0},
	{Air_SHD, AirPart_ModeB, 50, 10, 40},
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 50, 10 + SEAT_AIR_WEAK, 40},
#endif
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

const CoreMixOtherTabItem_t GasTab_CloseAir[] = {
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 5, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};
//* 7 res 6 Seat 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t GasTab_UpperAir[] = {
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000001), 50, 10, 40},
	{Part_EXEC, ConMth_Air, 10, 60, 0},

	{Air_UpDw, B(00000101), 50, 10, 40},
	{Part_EXEC, ConMth_Air, 15, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 15, 30, 0},

	{Air_SHD, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeA, 50, 4, 6},
	{Part_EXEC, ConMth_Air, 15, 30, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeA, 50, 4, 6},
	{Part_EXEC, ConMth_Air, 15, 30, 0},

	{Air_SHD, AirPart_ModeB, 50, 10, 40},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 5, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0},
};

const CoreMixOtherTabItem_t GasTab_LowerAir[] = {
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001000), 50, 10, 40},
#else
	{Air_UpDw, B(00001000), 50, 10, 40},
#endif
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_FeetS, AirPart_ModeB, 60, 3, 7},
	{Air_Ankle, AirPart_ModeB, 55, 3, 7},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_2POS, 0x32, 2, 0x21},
#endif

#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 50, 10 + SEAT_AIR_WEAK, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#endif

	{Air_CalfS, AirPart_STOP, 0, 0, 0},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_STOP, 0, 0, 0},
#endif
	{Air_FeetS, AirPart_ModeB, 50, 10, 40},
	{Air_Ankle, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 30, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0},
};

//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t GasTab_Full[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

#ifdef USE_FOOT_ROCK
	{Air_FeetS, AirPart_ModeA, 80 - FEET_AIR_WEAK * 2, 4, 6},
	{Air_Ankle, AirPart_ModeA, 75 - FEET_AIR_WEAK, 3, 7},
	{Part_EXEC, ConMth_Air, 10, 60, 0},
	{Motor_Rock, FootRock_2POS, 0x02, 4, 0x31},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#else
	{Air_FeetS, AirPart_ModeA, 80 - FEET_AIR_WEAK * 2, 4, 6},
	{Air_Ankle, AirPart_ModeA, 75 - FEET_AIR_WEAK, 4, 6},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Motor_Calf, MotorPart_FWD, 0, 90, 0}, //* 小腿揉搓 滚刮 延时9秒开启

#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001000), 90, 10, 50},
#elif defined USE_FOOT_ROCK
	{Air_CalfS, AirPart_ModeB, 90 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 50},
#else
	{Air_UpDw, B(00001000), 90 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 50},
#endif
	{Air_ArmL, AirPart_ModeA, 90, 3, 7},
	{Part_EXEC, ConMth_Air, 25, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeA, 90, 3, 7},
	{Part_EXEC, ConMth_Air, 25, 60, 0},

	// {Motor_Feet, MotorPart_STOP, 0, 0, 0}, //* 关足底滚轮
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_2POS, 0x42, 3, 0x21},
	{Air_CalfS, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmL, AirPart_ModeA, 60, 3, 7},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmR, AirPart_ModeA, 60, 3, 7},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#else
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmL, AirPart_ModeA, 60, 3, 7},
	{Air_FeetS, AirPart_ModeA, 55, 5, 5},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmR, AirPart_ModeA, 60, 3, 7},
	{Air_FeetS, AirPart_ModeA, 55, 5, 5},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#endif

// {Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开足底滚轮
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 90, 0},
#endif
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001001), 90, 0, 50},
	{Part_EXEC, ConMth_Air, 20, 60, 0}, // @ 对应减少时间 30->10 260323

	{Air_UpDw, B(00000001), 60, 10, 40}, // @ 需要测试效果 260321
	{Air_CalfB, AirPart_STOP, 0, 0, 0},
	{Air_CalfS, AirPart_ModeB, 90, 80, 90},
	{Part_EXEC, ConMth_Air, 9, 60, 0},

	{Air_CalfB, AirPart_ModeB, 90, 80, 90},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#elif (AIR_SEAT)
	{Air_UpDw, B(00001001), 60, 10, 50},
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // TODO 减少座侧 260605
	{Part_EXEC, ConMth_Air, 40, 60, 0},
#else
	{Air_UpDw, B(00001001), 90, 10, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Air_ArmR, AirPart_ModeB, 80, 10, 50},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 80, 10, 50},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#if (AIR_CLAFBOT)
	{Air_SHD, AirPart_STOP, 0, 0, 0},
#else
	{Air_UpDw, B(00001000), 60, 10 + CALF_AIR_WEAK, 50},
#endif
	{Air_ArmR, AirPart_ModeB, 60, 10, 50},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 60, 10, 50},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

#ifdef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 50, 0},
#else
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},

	{Air_FeetS, AirPart_ModeA, 80 - FEET_AIR_WEAK * 2, 4, 6},
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK},
#endif
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Air_SHD, AirPart_ModeB, 60, 10, 50},
	{Part_EXEC, ConMth_Air, 6, 60, 0},

	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Part_EXEC, ConMth_Air, 10, 60, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

const CoreMixOtherTabItem_t GasTab_FullA[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

	{Air_FeetS, AirPart_ModeA, 70 - FEET_AIR_WEAK, 4, 6},
	{Air_Ankle, AirPart_ModeA, 65 - FEET_AIR_WEAK, 4, 6},
#ifdef USE_FOOT_ROCK
	{Part_EXEC, ConMth_Air, 10, 60, 0},
	{Motor_Rock, FootRock_2POS, 0x32, 5, 0x21},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#else
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Motor_Calf, MotorPart_FWD, 0, 90, 0}, //* 小腿揉搓 滚刮 延时15秒开启

#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001000), 70, 10, 40},
#elif defined USE_FOOT_ROCK
	{Air_CalfS, AirPart_ModeB, 70 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 40},
#else
	{Air_UpDw, B(00001000), 70 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 40},
#endif
	{Air_ArmL, AirPart_ModeA, 70, 4, 6},
	{Part_EXEC, ConMth_Air, 25, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeA, 70, 4, 6},
	{Part_EXEC, ConMth_Air, 25, 60, 0},

	// {Motor_Feet, MotorPart_STOP, 0, 0, 0}, //* 关足底滚轮
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},

	{Air_UpDw, B(00000000), 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 45, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmL, AirPart_ModeA, 40, 4, 6},
	{Air_FeetS, AirPart_ModeA, 35, 5, 5},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 45, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmR, AirPart_ModeA, 40, 4, 6},
	{Air_FeetS, AirPart_ModeA, 35, 5, 5},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

// {Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开足底滚轮
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 90, 0},
#endif
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 10, 60, 0},

	{Air_UpDw, B(00001001), 50, 50, 50}, // @ 需要测试效果 260321
	{Part_EXEC, ConMth_Air, 5, 60, 0},

	{Air_CalfB, AirPart_ModeB, 50, 50, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#elif (AIR_SEAT)
	{Air_UpDw, B(01001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 40, 60, 0},
#else
	{Air_UpDw, B(00001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(00000000), 50, 10, 50},
#else
	{Air_UpDw, B(00001000), 50, 10 + CALF_AIR_WEAK, 50},
#endif
	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

const CoreMixOtherTabItem_t GasTab_FullB[] = {
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
#ifdef USE_FOOT_ROCK
	{Motor_Rock, FootRock_FIXED, ROCKPOS_N2 & 0xFF, ROCKPOS_N2 >> 8, 5},
#endif
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

	{Air_UpDw, B(00110000), 70, 10, 40},
#ifdef USE_FOOT_ROCK
	{Part_EXEC, ConMth_Air, 10, 60, 0},
	{Motor_Rock, FootRock_2POS, 0x32, 2, 0x21},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#else
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Motor_Calf, MotorPart_FWD, 0, 90, 0}, //* 小腿揉搓 滚刮 延时15秒开启
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001010), 70, 10, 40},
#else
	{Air_UpDw, B(00001010), 70 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 40},
#endif

	{Part_EXEC, ConMth_Air, 25, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 70, 10, 40},
	{Part_EXEC, ConMth_Air, 25, 60, 0},

	// {Motor_Feet, MotorPart_STOP, 0, 0, 0}, //* 关足底滚轮
	{Motor_Calf, MotorPart_STOP, 0, 0, 0},
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 45, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmL, AirPart_ModeB, 40, 10, 40},
	{Air_FeetS, AirPart_ModeB, 40, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_FeetS, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 45, 10, 40},
	{Part_EXEC, ConMth_Air, 5, 60, 0}, //* 先充5秒
	{Air_ArmR, AirPart_ModeB, 40, 10, 40},
	{Air_FeetS, AirPart_ModeB, 40, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

// {Motor_Feet, MotorPart_FWD, 0, 0, 0}, //* 开足底滚轮
#ifndef MOTOR_CALF_WEAK
	{Motor_Calf, MotorPart_FWD, 0, 90, 0},
#endif
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 10, 60, 0},

	{Air_UpDw, B(00001001), 50, 50, 50}, // @ 需要测试效果 260321
	{Part_EXEC, ConMth_Air, 5, 60, 0},

	{Air_CalfB, AirPart_ModeB, 50, 50, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#elif (AIR_SEAT)
	{Air_UpDw, B(01001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 40, 60, 0},
#else
	{Air_UpDw, B(00001001), 70, 10, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif

	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 20, 60, 0},
#if (AIR_CLAFBOT)
	{Air_UpDw, B(01001000), 50, 10, 50},
#else
	{Air_UpDw, B(00001000), 50, 10 + CALF_AIR_WEAK, 50},
#endif
	{Air_ArmR, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
	{Air_ArmL, AirPart_ModeB, 50, 10, 40},
	{Part_EXEC, ConMth_Air, 12, 60, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t GasTab_STRPREA[] = {
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

	{Air_UpDw, B(00110000), 60, 10, 50},
	{Air_ArmL, AirPart_ModeB, 80, 0, 80},
	{Part_EXEC, ConMth_Air, 10, 60, 0},

	{Air_CalfS, AirPart_ModeB, 60 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 50 - CALF_AIR_WEAK},

#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 60, 10, 50},
#endif
	{Part_EXEC, ConMth_Air, 18, 60, 0},

#ifdef FEET_STRAIR_ADD
	{Air_FeetS, AirPart_ModeB, 120, 0, 120},
#else
	{Air_FeetS, AirPart_ModeB, 80 - FEET_AIR_WEAK * 2, 8 + FEET_AIR_WEAK, 80 - FEET_AIR_WEAK * 2},
#endif
	{Air_Ankle, AirPart_ModeB, 100 - FEET_AIR_WEAK, 0, 100 - FEET_AIR_WEAK},

	{Air_ArmL, AirPart_STOP, 0, 0, 0},
	{Air_ArmR, AirPart_ModeB, 80, 0, 80},
#if (AIR_SEAT)
	{Air_Seat, AirPart_ModeB, 60, 15 + SEAT_AIR_WEAK, 45 - SEAT_AIR_WEAK}, // todo 效果待确认 260518
#endif
	{Part_EXEC, ConMth_Air, 28, 60, 0},

	{Air_ArmR, AirPart_STOP, 0, 0, 0},
#if (AIR_SEAT)
	{Part_LOOP, 0, 5, 3, 0},
	{Air_SHD, AirPart_ModeB, 80, 0, 80},
	{Air_Ankle, AirPart_ModeB, 160, 0, 160},
	{Part_EXEC, ConMth_Air, 40, 60, 0},
#else
	{Part_LOOP, 0, 5, 2, 0},
	{Air_SHD, AirPart_ModeB, 60, 10, 50},
	{Part_EXEC, ConMth_Air, 30, 60, 0},
#endif
	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

//* Air_UpDw 默认 AirPart_ModeB
//* 7-6 res 5 Ankle 4 FeetS 3 CalfS 2 ArmR 1 ArmL 0 SHD
const CoreMixOtherTabItem_t GasTab_STRPREB[] = {
	{Air_UpDw, B(00000000), 0, 0, 0},
	{Part_EXEC, ConMth_Air, 3, 60, 0}, // todo add 260629

	{Air_UpDw, B(00010000), 60 - FEET_AIR_WEAK, 10 + FEET_AIR_WEAK, 60 - FEET_AIR_WEAK},
	{Air_Ankle, AirPart_ModeB, 100, 0, 100}, // @ 增加脚后跟不泄气 MD 251220

	{Air_CalfS, AirPart_ModeB, 60 - CALF_AIR_WEAK, 10 + CALF_AIR_WEAK, 50 - CALF_AIR_WEAK},
#if (AIR_CLAFBOT)
	{Air_CalfB, AirPart_ModeB, 60, 10, 50},
#endif
	{Part_EXEC, ConMth_Air, 30, 60, 0},

	{Part_LOOP, 0, 5, 1, 0},
	{Part_EXEC, ConMth_Air, 30, 60, 0},

	{Part_EXEC, ConMth_AC, 0, 0, 0}, // * 复位
};

/************************
 * 气囊工作模式索引表
 * **********************/
ActionTabAttr_t Air_Addr[] = {
	{FA_GAS_CLOSE, (void *)GasTab_CloseAir},

	{FA_GAS_UPPERGAS, (void *)GasTab_UpperAir},
	{FA_GAS_LOWERGAS, (void *)GasTab_LowerAir},
	{FA_GAS_Full, (void *)GasTab_Full},
	{FA_GAS_FullA, (void *)GasTab_FullA},
	{FA_GAS_FullB, (void *)GasTab_FullB},
	{FA_GAS_STRPREA, (void *)GasTab_STRPREA},
	{FA_GAS_STRPREB, (void *)GasTab_STRPREB},

	{FA_GAS_LC, (void *)GasTab_LC},
};

#define AIR_TABMAX sizeof(Air_Addr) / sizeof(Air_Addr[0])

/*
 ************************************************************************************************************************************************
 * 函数定义 *************************************************************************************************************************************
 ************************************************************************************************************************************************
 */
/*****************************************************************************************************
 * 描述：加载要运行的气囊表格
 * 参数：无
 * 返回：无
 * 注意：无
 *****************************************************************************************************/
void AirMode_Load(uint8_t airmode_id)
{
	uint8_t *p;
	uint8_t i;

	for (i = 0; i < AIR_TABMAX; i++)
	{
		if (Air_Addr[i].TabID == airmode_id)
		{
			p = Air_Addr[i].pTab;
		}
	}

	if (airmode_id == AirModeClose)
	{
		AT_TableStop(Table_Air);
		Gas_CloseAll();
	}
	else
	{
		AtTabSet_ID(Table_Air, airmode_id); // * add 250917
		AT_TableStart(Table_Air, p);
	}
}

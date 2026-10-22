#include "Leg.h"
#include "WorkState.h"
#include "MassageEXEC.h"

LegDataTx_S LegTx;
LegDataRx_S LegRx;

Leg_State_t Leg_State;

#ifdef USE_FOOT_ROCK

uint16_t Rock_Tab[6] = {ROCKPOS_N0, ROCKPOS_N1, ROCKPOS_N2, ROCKPOS_N3, ROCKPOS_N4, ROCKPOS_N5};
u8 FootSwing_Level;

static u8 LegRock_Init_Success = 0;
static u8 LegRockLoopActive = 0;
static u8 LegRockLoopPhase = 0;
static u8 LegRockLoopFirstPosition = 0;
static u8 LegRockLoopSecondPosition = 0;
static u8 LegRockLoopCyclesLeft = 0;
static u16 LegRockLoopFirstDwellTicks = 0;
static u16 LegRockLoopSecondDwellTicks = 0;

enum
{
	LegRockLoop_MoveFirst,
	LegRockLoop_WaitFirst,
	LegRockLoop_MoveSecond,
	LegRockLoop_WaitSecond,
};

static u16 LegRockGetCircle(void)
{
	return (u16)((u16)LegRx.Rock.CurCircle_H8 << 8) | LegRx.Rock.CurCircle_L8;
}

u8 LegRockIsAtPosition(u16 position)
{
	u16 circle = LegRockGetCircle();
	u16 distance = circle > position ? circle - position : position - circle;

	return distance <= 2;
}

void LegRockMoveToPosition(u16 position, u8 speed)
{
	if (LegRockIsAtPosition(position))
	{
		LegTx.Rock.State = 0;
		return;
	}

	LegTx.Rock.State = ROCK_STATE_POSITION;
	LegTx.Rock.Speed = speed;
	LegTx.Rock.Circle_L8 = (u8)position;
	LegTx.Rock.Circle_H8 = (u8)(position >> 8);
}

void LegRockManualStateSet(u8 state, u8 speed)
{
	LegRockLoopActive = 0;
	MotorT_Work.RunFlag[MotorT_Rock] = 0;
	MotorT_Work.RunTime[MotorT_Rock] = 0;
	FootSwing_Level = 0;
	LegTx.Rock.State = state;
	LegTx.Rock.Speed = state ? speed : 0;
}

void LegRockSwingSet(u8 level)
{
	static const u8 position_pairs[] = {0x21, 0x31, 0x41, 0x40};

	if (level > 4)
		return;

	LegRockLoopActive = 0;
	MotorT_Work.RunTime[MotorT_Rock] = 0;
	FootSwing_Level = level;
	KeyPush.RockKeyState = level ? 3 : 0;

	if (level == 0)
	{
		MotorT_Work.RunFlag[MotorT_Rock] = 0;
		LegTx.Rock.State = 0;
		LegTx.Rock.Speed = 0;
		return;
	}

	MotorT_Work.Cmd[MotorT_Rock] = FootRock_2POS;
	MotorT_Work.Para1[MotorT_Rock] = position_pairs[level - 1];
	MotorT_Work.Para2[MotorT_Rock] = 0xFF;
	MotorT_Work.Para3[MotorT_Rock] = 0x21;
	MotorT_Work.RunFlag[MotorT_Rock] = 1;
}

static u8 LegRockLoopStart(void)
{
	/* Position nibbles are 0-based Rock_Tab indexes; dwell nibbles are seconds. */
	u8 firstIndex = MotorT_Work.Para1[MotorT_Rock] >> 4;
	u8 secondIndex = MotorT_Work.Para1[MotorT_Rock] & 0x0F;
	u8 dwell = MotorT_Work.Para3[MotorT_Rock];

	if (firstIndex >= (sizeof(Rock_Tab) / sizeof(Rock_Tab[0])) ||
		secondIndex >= (sizeof(Rock_Tab) / sizeof(Rock_Tab[0])) ||
		MotorT_Work.Para2[MotorT_Rock] == 0)
	{
		LegTx.Rock.State = 0;
		MotorT_Work.RunFlag[MotorT_Rock] = 0;
		MotorT_Work.RunTime[MotorT_Rock] = 0;
		return 0;
	}

	LegRockLoopFirstPosition = firstIndex;
	LegRockLoopSecondPosition = secondIndex;
	LegRockLoopCyclesLeft = MotorT_Work.Para2[MotorT_Rock];
	LegRockLoopFirstDwellTicks = (u16)(dwell >> 4) * 10;
	LegRockLoopSecondDwellTicks = (u16)(dwell & 0x0F) * 10;
	MotorT_Work.RunTime[MotorT_Rock] = 0;
	LegRockLoopPhase = LegRockLoop_MoveFirst;
	LegRockLoopActive = 1;
	return 1;
}

static void LegRockLoopUpdate(void)
{
	u16 position;

	if (LegRockLoopActive == 0 && !LegRockLoopStart())
		return;

	if (LegRockLoopPhase == LegRockLoop_MoveFirst ||
		LegRockLoopPhase == LegRockLoop_WaitFirst)
		position = Rock_Tab[LegRockLoopFirstPosition];
	else
		position = Rock_Tab[LegRockLoopSecondPosition];

	if (LegRockLoopPhase == LegRockLoop_MoveFirst ||
		LegRockLoopPhase == LegRockLoop_MoveSecond)
	{
		LegRockMoveToPosition(position, 5);
		if (LegRockIsAtPosition(position))
		{
			LegTx.Rock.State = 0;
			LegRockLoopPhase = LegRockLoopPhase == LegRockLoop_MoveFirst
								   ? LegRockLoop_WaitFirst
								   : LegRockLoop_WaitSecond;
			MotorT_Work.RunTime[MotorT_Rock] = 0;
		}
		return;
	}

	if (LegRockLoopPhase == LegRockLoop_WaitFirst)
	{
		if (++MotorT_Work.RunTime[MotorT_Rock] >= LegRockLoopFirstDwellTicks)
		{
			MotorT_Work.RunTime[MotorT_Rock] = 0;
			LegRockLoopPhase = LegRockLoop_MoveSecond;
		}
	}
	else if (LegRockLoopPhase == LegRockLoop_WaitSecond &&
			 ++MotorT_Work.RunTime[MotorT_Rock] >= LegRockLoopSecondDwellTicks)
	{
		MotorT_Work.RunTime[MotorT_Rock] = 0;
		if (LegRockLoopCyclesLeft != 0xFF && --LegRockLoopCyclesLeft == 0)
		{
			LegTx.Rock.State = 0;
			MotorT_Work.RunFlag[MotorT_Rock] = 0;
			MotorT_Work.RunTime[MotorT_Rock] = 0;
			LegRockLoopActive = 0;
		}
		else
		{
			LegRockLoopPhase = LegRockLoop_MoveFirst;
		}
	}
}

void LegRock_Updata(void)
{
	if (KeyPush.RockKeyState != 0)
	{
		if (KeyPush.RockKeyState == 3)
		{
			if (MotorT_Work.RunFlag[MotorT_Rock] &&
				MotorT_Work.Cmd[MotorT_Rock] == FootRock_2POS)
				LegRockLoopUpdate();
			return;
		}

		if (((DDG_Get_BnCurCircle < ROCK_DEF_BACK && DDG_Get_LnCurCircle < ROCK_DEF_LEG + 9) ||
			 DDG_Get_LnCurCircle < ROCK_DEF_LEG) &&
			KeyPush.RockKeyState == 2)
		{
			u16 circle = LegRockGetCircle();
			if (circle > ROCKPOS_N2)
				LegRockMoveToPosition(ROCKPOS_N2, 5);
			else if (circle >= ROCKPOS_N2 - 2)
				LegTx.Rock.State = 0;
		}
		return;
	}

	if (LegRock_Init_Success == 0)
	{
		LegRockMoveToPosition(ROCKPOS_N2, 5);
		if (LegRockIsAtPosition(ROCKPOS_N2))
		{
			LegTx.Rock.State = 0;
			LegRock_Init_Success = 1;
		}
		else
			return;
	}

	if ((DDG_Get_BnCurCircle < ROCK_DEF_BACK && DDG_Get_LnCurCircle < ROCK_DEF_LEG + 9) ||
		DDG_Get_LnCurCircle < ROCK_DEF_LEG)
	{
		u8 stopCommand = 0;
		if (MotorT_Work.RunFlag[MotorT_Rock])
		{
			if (MotorT_Work.Cmd[MotorT_Rock] == FootRock_2POS)
				stopCommand = 1;
			else if (MotorT_Work.Cmd[MotorT_Rock] == FootRock_FIXED)
			{
				u16 target = (u16)((u16)MotorT_Work.Para2[MotorT_Rock] << 8) |
							 MotorT_Work.Para1[MotorT_Rock];
				stopCommand = target > ROCKPOS_N2;
			}
		}

		if (stopCommand)
		{
			MotorT_Work.Cmd[MotorT_Rock] = FootRock_STOP;
			MotorT_Work.RunFlag[MotorT_Rock] = 0;
			MotorT_Work.RunTime[MotorT_Rock] = 0;
			LegRockLoopActive = 0;
		}

		if (LegRockGetCircle() > ROCKPOS_N2)
		{
			LegRockMoveToPosition(ROCKPOS_N2, 5);
			return;
		}

		if (stopCommand)
		{
			LegTx.Rock.State = 0;
			return;
		}
	}

	if (MotorT_Work.RunFlag[MotorT_Rock] == 0)
	{
		LegRockLoopActive = 0;
		return;
	}

	if (MotorT_Work.Cmd[MotorT_Rock] == FootRock_FIXED)
	{
		LegRockLoopActive = 0;
		u16 position = (u16)((u16)MotorT_Work.Para2[MotorT_Rock] << 8) |
					   MotorT_Work.Para1[MotorT_Rock];
		u8 speed = MotorT_Work.Para3[MotorT_Rock];

		LegRockMoveToPosition(position, speed);
		if (LegRockIsAtPosition(position))
		{
			LegTx.Rock.State = 0;
			MotorT_Work.RunFlag[MotorT_Rock] = 0;
			MotorT_Work.RunTime[MotorT_Rock] = 0;
		}
	}
	else if (MotorT_Work.Cmd[MotorT_Rock] == FootRock_2POS)
	{
		LegRockLoopUpdate();
	}
	else if (MotorT_Work.Cmd[MotorT_Rock] == FootRock_STOP)
	{
		LegRockLoopActive = 0;
		LegTx.Rock.State = 0;
		MotorT_Work.RunFlag[MotorT_Rock] = 0;
		MotorT_Work.RunTime[MotorT_Rock] = 0;
	}
}
#endif

void LegInit(void)
{
	memset(&LegRx, 0, sizeof(LegDataRx_S));
	memset(&LegTx, 0, sizeof(LegDataTx_S));
#ifdef USE_FOOT_ROCK
	FootSwing_Level = 0;
	LegRock_Init_Success = 0;
	LegRockMoveToPosition(ROCKPOS_N2, 5);
#endif
}

void LegDataTx(void) // @ 100ms 内轮询一次 261014
{
	u8 LegDataUpdate[sizeof(LegDataTx_S)] = {0};

	LegTx.Roll.State = RollStateGet();
	LegTx.Roll.Speed = RollSpeedGet();
	LegTx.Knead1.State = KneadStateGet(); /* 腿部滚刮和揉搓状态一致 */
	LegTx.Knead1.Speed = KneadSpeedGet();
	LegTx.Knead2.State = KneadStateGet();
	LegTx.Knead2.Speed = KneadSpeedGet();

	LegTx.Flex.State = FootFlexStateGet();
	LegTx.Flex.Speed = FootFlexSpeedGet();
	LegTx.Flex.Circle_L8 = FootFlexCircleGet();
	LegTx.Flex.Circle_H8 = FootFlexCircleGet() >> 8;

	if (WorkState_Pause == WorkStateGet())
	{
		memset(&LegTx, 0, sizeof(LegDataTx_S));
	}

	if (nPowerOn)
		LegTx.Led.State = 3; // @ 小腿灯 261016
	else
		LegTx.Led.State = 0;

	memcpy(&LegDataUpdate, &LegTx, sizeof(LegDataUpdate) / sizeof(LegDataUpdate)[0]);
	_485_ComTx(LegAddr, MainAddr, LegTx_Count, 0x0001, sizeof(LegDataUpdate) / sizeof(LegDataUpdate)[0], LegDataUpdate);
}

LegDataRx_S *GetLegDataRx(void)
{
	return &LegRx;
}

void LegRollStateSet(uint8_t sta, uint8_t spd)
{
	LegTx.Roll.State = sta;
	LegTx.Roll.Speed = spd;
}

void LegKneadStateSet(uint8_t sta, uint8_t spd)
{
	LegTx.Knead1.State = sta;
	LegTx.Knead2.State = sta;

	LegTx.Knead1.Speed = spd;
	LegTx.Knead2.Speed = spd;
}

void LegHeatStateSet(uint8_t sta, uint8_t level)
{
	LegTx.Heat.State = sta;
	LegTx.Heat.Level = level;
}

void LegAirStateSet(uint8_t air)
{
	LegTx.AirValve.Valve.Gas1 = (air & 0x01) ? 1 : 0;
	LegTx.AirValve.Valve.Gas2 = (air & 0x02) ? 1 : 0;
	LegTx.AirValve.Valve.Gas3 = (air & 0x04) ? 1 : 0;
	LegTx.AirValve.Valve.Gas4 = (air & 0x08) ? 1 : 0;
	LegTx.AirValve.Valve.Gas5 = (air & 0x10) ? 1 : 0;
	LegTx.AirValve.Valve.Gas6 = (air & 0x20) ? 1 : 0;
	LegTx.AirValve.Valve.Gas7 = (air & 0x40) ? 1 : 0;
	LegTx.AirValve.Valve.Gas8 = (air & 0x80) ? 1 : 0;
}

void LegFlexStateSet(uint8_t sta, uint8_t spd, uint16_t circle)
{
	LegTx.Flex.State = sta;
	LegTx.Flex.Speed = spd;
	LegTx.Flex.Circle_L8 = circle;
	LegTx.Flex.Circle_H8 = circle >> 8;
}

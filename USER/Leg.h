#ifndef LEG_H
#define LEG_H

#include "config.h"
#include "_485Com.h"
#include "Heat.h"
#include "FootRoll.h"
#include "CalfKnead.h"
#include "FootFlex.h"
#include "Air.h"
#include <string.h>

typedef enum
{
	LegActionStop,
	LegActionDown,
	LegActionUp,
	LegActionSuccess,
} Leg_State_t;

// todo 翻转取值范围 50-480 261014
#ifdef USE_FOOT_ROCK
#define ROCK_DEF_BACK DDG_BCircle_Max / 2
#define ROCK_DEF_LEG DDG_LCircle_Max / 2 - 2
#define ROCKPOS_MIN 0
#define ROCKPOS_N0 0
#define ROCKPOS_N1 50
#define ROCKPOS_N2 130
#define ROCKPOS_N3 260
#define ROCKPOS_N34 300
#define ROCKPOS_N4 360
#define ROCKPOS_N5 480
#define ROCKPOS_MAX 480
#define ROCK_STATE_POSITION 5
extern uint16_t Rock_Tab[6];
void LegRock_Updata(void);
void LegRockMoveToPosition(u16 position, u8 speed);
u8 LegRockIsAtPosition(u16 position);
void LegRockManualStateSet(u8 state, u8 speed);
#endif

void LegInit(void);
// void LegDataRx(u8 dat);
void LegDataTx(void);

extern LegDataTx_S LegTx;
extern LegDataRx_S LegRx;

LegDataRx_S *GetLegDataRx(void);
extern Leg_State_t Leg_State;

void LegRollStateSet(uint8_t sta, uint8_t spd);
void LegKneadStateSet(uint8_t sta, uint8_t spd);
void LegHeatStateSet(uint8_t sta, uint8_t level);
void LegAirStateSet(uint8_t air);
void LegFlexStateSet(uint8_t sta, uint8_t spd, uint16_t circle);

#endif

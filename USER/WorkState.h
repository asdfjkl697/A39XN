#ifndef WORKSTATE_H
#define WORKSTATE_H

#include "config.h"
#include "Voice.h"
#include "Power.h"
#include "Motor.h"
#include "MassageInit.h"
#include "MoveLogic.h"
#include "AT.h"
#include "Leg.h"
#include "System.h"
#include <string.h>

typedef enum
{
	WorkState_Wait = 0,
	WorkState_AutoLying,
	WorkState_WaitFunc,
	WorkState_BodyCheck,
	WorkState_ShoulderAdj,
	WorkState_Work,
	WorkState_Pause,
	WorkState_Reset,
	WorkState_TurnOn,
	WorkState_HealthPre, // NONE
	WorkState_HealthCheck,
	WorkState_HealthFinish,
	WorkState_FCT,
	WorkState_WifiWait,
	WorkState_MassageFinish,
	WorkState_UserIntensityChose = 24,
	WorkState_Warn,
	WorkState_Update = 255,

} WorkState_E;

typedef enum
{
	UserFunc_PowerOn = 0, // 开机
	UserFunc_PowerOff,	  // 关机
	UserFunc_AutoMode,	  // 自动模式设置
	UserFunc_ManualMode,  // 进入手动模式
	UserFunc_Reset,		  // 恢复
} UserFunc_E;

typedef struct
{

	uint32_t Tick;
	uint8_t CurState;
	uint8_t CurMin; // 当前分
	uint8_t CurSec; // 当前秒
	uint8_t VoicePlayFlag;

} WorkState_S;

void WorkStateInit(void);
void WorkStateSet(u8 sta);
u8 WorkStateGet(void);
u8 WorkMinGet(void);
u8 WorkSecGet(void);
void WorkSecSet(u8 sec);
void WorkMinSet(u8 min);
u32 WorkTickGet(void);
void WorkStateProcess(void);
void UserFunc_Set(u8 ntype, u8 nparam);
void Exit_Pause(void);
void Enter_Pause(void);

extern bool Shoulder_Update;
extern WorkState_S Work;
extern bool AutoLyingSkip_Flag;

#endif

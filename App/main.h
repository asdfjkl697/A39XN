#ifndef MAIN_H
#define MAIN_H

#include "Voice.h"
#include "Heat.h"
#include "Motor.h"
#include "Bluetooth.h"
#include "Air.h"
#include "Led.h"
#include "Heat.h"
#include "NOI.h"
#include "Power.h"
#include "MassageInit.h"
#include "WorkState.h"
#include "Leg.h"
#include "_485Com.h"
#include "ArmKey.h"
#include "FootRoll.h"
#include "CalfKnead.h"
#include "FootFlex.h"
#include "Display.h"
#include "Handler.h"
#include "health.h"
#include "Memory.h"     //* 修改
#include "ErrorCheck.h" //* 增加 250305
#include "app.h"

#include "MoveBoth.h"

#if defined(USE_AIRSENSOR)
#include "AirSensor.h"
#endif
#ifdef USE_S0553
#include "S0553Key.h"
#endif

extern uint8_t TxPolling;
extern uint8_t Core_RestSuccess;
#endif

#ifndef KEYFUNC_H
#define KEYFUNC_H

#include "config.h"
#include "MoveLogic.h"
#include "WorkState.h"
#include "MassageInit.h"
#include "Display.h"
#include <string.h>

typedef enum
{
    Key_NULL,
    Key_Power,
    Key_Pause,
    Key_Shoulder,
    Key_Timing,

    Key_AutoMode,
    Key_Wise,
    Key_BackSpeed,
    Key_BackWidth,
    Key_BackArea,

    Key_BackTread,
    Key_UpperAirStrength, //* 气压强度
    Key_LowerAirStrength,
    Key_BackHeat,
    Key_HipsHeat,

    Key_LegsHeat,
    Key_Roll,
    Key_LegsUpDown,
    Key_Lying,
    Key_BodyCheck_Jump, //* 19

    Key_ShoulderAdj_Jump, //*20
    Key_Zero,
    Key_Memory_Save,
    Key_Memory_Use,
    Key_Blue,

    Key_BackStrength,
    Key_LegsFlex,
    Key_BackLying,
    Key_LegsKnead,
    Key_LogoCompleted,

    Key_LcdOK,
    Key_Ion,
    Key_KeepLying,
    Key_UpperAir,
    Key_LowerAir,
    Key_Air, //* 气压 上 下 全
    Key_NULL_0x24,
    Key_NULL_0x25,
    Key_Voice,
    Key_Health,
    Key_TimOutReset,
    Key_LC = 0x2c,   //*修改 250209
    Key_PACK = 0x2d, //*修改 250209
    key_AutoLyJump = 0x2f,
    Key_FCT,
    Key_Customize,
    Key_MasMode,          // 模式强度
    key_DeviceInf = 0x34, // 产品信息查询
    key_Light,
    Key_VoiceLan,
    Key_Player,
    Key_Music,
    Key_Volume,
    Key_ShoulderHeat,
    key_DevInfOk, // * 产品信息查询完成
    key_SleepMode = 0x3c,
    Key_Warn = 0x3e,
    Key_M1M2 = 0x40,
    Key_M1_M2 = 0x41,
    Key_O2 = 0x42,
    key_Error = 0x44,
    key_DiyWork = 0x45,
    key_Recovery = 0x46, //* 恢复出产设置
    Key_SoundWavex = 0x47,
    Key_SoundWave,
    Key_SoundWaveSW,
    Key_Volumex,
    Key_SoundUpDown,
    Key_SoundMusicSW,

    Key_4DIntensity = 0x53, // @ add 251203
    Key_MassSpeed,          // @ add 251203
    Key_MassWidth,          // @ add 251203
    Key_AirIntensity,       // @ add 251203
    Key_Volume_Set,

    Key_FootRock = 0x60, // @ add 261015

    Key_WakeUp = 200,
    Key_BackUpDown,
    Key_Sit_coder,
    Key_Heat,
    key_SOUND_ACK, // * 应答声

    Key_Language = 0xf1,
    Key_AppAir,
    Key_LowPower,
    Key_MassageReset = 0xfe,

} KeyFunc_E;

typedef struct
{
    u8 State;

    u8 Value;

    u8 Param;

    uint32_t KeyKeepTick;

    uint8_t KeyEncoder;
} KeyFunc_S;

/********************************************************************************
 * 类型定义 : 按键是否按下的标志，一般用于推杆、机芯等按键持续按下的标志
 ********************************************************************************/
typedef struct
{
    uint8_t TreadKeyState;
    uint8_t LegFlexKeyState;
    uint8_t BackFlexKeyState;
    uint8_t FlexKeyState;
    uint8_t FootFlexKeyState;
#ifdef USE_M2
    uint8_t M2TreadKeyState;
#endif

    uint8_t RockKeyState; // todo 手动翻转小腿 260702
    /* 手动操作时置1，自动程序运行时以手动设置为主，未手动设置以表格设置为准 */
    uint8_t FootRollKeyState;
    uint8_t KeyCalfKnead;

    uint8_t KeyBackHeat;
    uint8_t KeyLegHeat;
    /*--------------------------------------------------------------------*/

    uint8_t WiseKeyState; /* 手动选择手法哪怕是无手法，也会置1，供上层操作 */ // * 开关机一定要清零，不然会进入无手法
} KeyPush_t;

extern KeyPush_t KeyPush;
// typedef struct
//{
//	KeyFunc_E      KeyValID;                                          /* 键值 */
//
//	uint8_t        KeyAttr;                                           /* 按键属性：暂停、低功耗等状态下按键执行状态 */
//
//	uint8_t        (*pKeyHandle)(uint8_t SrcAddr, uint8_t KeyVal, uint8_t *pData);  /* 键值对应的操作处理 */
// }SysMode_KeyLogicTabItem_t;

extern KeyFunc_S Key;
extern u8 Handlekey_to_M1M2(u8 temp);
extern u8 M1M2_to_Handlekey(u8 temp);
extern u8 HandleArea_to_M1M2(u8 temp);
extern u8 M1M2_to_HandleArea(u8 temp);

void KeyPowerScan(void);
void KeyFunc(void);

#endif

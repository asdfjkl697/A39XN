#ifndef AT_H
#define AT_H

#include "config.h"
#include "WorkState.h"
#include "Motor.h"
#include "Leg.h"
#include "Air.h"
#include "MoveLogic.h"
#include <string.h>

typedef enum
{
    PartCon_Stop = 0,
    PartCon_Run,
    PartCon_Pause,
    PartCon_Continue,
} PartCon_enum;

typedef enum
{
    ConMth_NULL = 0,
    ConMth_MoveUp,
    ConMth_MoveDw,
    ConMth_Move,
    ConMth_Air,
    ConMth_Motor,
    ConMth_All,
    ConMth_KneadZero = 0x20,
    ConMth_TreadLess,
    ConMth_TreadMore,
    ConMth_TreadBLess,
    ConMth_TreadBMore,
    ConMth_Loop = 0xa5, //* 附带循环次数
    ConMth_AC = 0xac,   //* 复位
    ConMth_AD = 0xad,   //* 结束
} ConMth_enum;

/********************************************************************************
 * 类型定义 : 【充气】动作对象
 ********************************************************************************/
// typedef struct
// {
//     GasPart1_t GasPart1;
//     GasPart2_t GasPart2;
// } GasActionObj_t;

typedef struct
{
    uint8_t Jump2Exec : 1; //* 是否揉捏找0
    uint8_t KneadZero : 1; //* 是否揉捏找0
} PartIf_T;
extern PartIf_T PartIf_Para;

typedef struct
{
    uint8_t Module;
    uint8_t Method;
    uint8_t Para1;
    uint8_t Para2;
    uint8_t Para3;
} CoreMixOtherTabItem_t;

/********************************************************************************
 * 类型定义 : 动作表格的属性
 ********************************************************************************/
typedef struct
{
    uint8_t TabID; // * MD 250919 混合的表格ID
    void *pTab;    /*混合的表格*/
} ActionTabAttr_t;

/********************************************************************************
 * 类型定义 : 动作命令属性
 ********************************************************************************/
typedef struct
{
    u8 State; // 运行状态

    u16 RunTime100Ms; // 当前时间 100ms
    u8 TableStep;     // 表格步进
    u8 LoopCnt;       // 循环计数
    u8 LoopStart;     // 循环起始
    u8 LoopEnd;       // @ 循环结束 新增

    bool ReadTBFlag;
    u16 PartRunTime;
    u16 PartOverTime;
    u16 PartOverMark;

    CoreMixOtherTabItem_t *AutoTable;
    uint8_t CurAutoTab;

    CoreMixOtherTabItem_t *AutoSubTable;
    uint8_t CurSubTab;

    CoreMixOtherTabItem_t *AutoSub2Table;
    uint8_t CurSub2Tab;

    CoreMixOtherTabItem_t *AirTable;
    uint8_t CurAirTab;

#ifdef USE_M2
    CoreMixOtherTabItem_t *MoveDwTable;
    uint8_t CurMoveDwTab;
#endif

} AT_CMD_S;

/********************************************************************************
 * 类型定义 : 动作表格的类型
 ********************************************************************************/
typedef enum
{
    Table_Auto = 0,   // 自动
    Table_Sub,        // 子表
    Table_Sub2,       // 二级子表
    Table_MoveDw,     // 下机芯主表
    Table_Sub_MoveDw, // 下机芯子表
    Table_Air,        // 气压主表
    Table_MoveUp,     // 上机芯主表
    Table_Manual_MoveUp,
    Table_Manual_MoveDw,
    TableNum,
} TableType_E;

/********************************************************************************
 * 类型定义 : 执行过程抛出标志
 ********************************************************************************/
typedef enum
{
    EXEC_Null = 0,      // 无执行标志
    EXEC_TurnOn = 0x10, // * 开机完成标志
    EXEC_Reset = 0x11,  // 复位完成标志
    EXEC_BodyCheck,     // 体型检测
    EXECNum,
} EXECType_E;

typedef enum
{
    ASSIGN_Null = 0,
    ASSIGN_Paradox,
    ASSIGN_Flex,
    ASSIGN_KneadSpeed,
    ASSIGN_TapSpeed,
    ASSIGN_TreadSpeed,
    ASSIGN_Mark,
    ASSIGN_FootFindAfterStretch,
    ASSIGNum,
} ASSIGN_enum;

extern u8 ASSIGN_Array[ASSIGNum];

/*
 * 对外声明 *************************************************************************************************************************************
 */
void AT_Init(void);
void AT_State_Get(void);
void AT_TableRead(AT_CMD_S *cmd_p, TableType_E ID);
bool AT_RunGet(void);
void AT_Process(void);
void AT_TableStart(u8 ID, const u8 *Table);
void AT_TableStop(u8 ID);
void AT_TableContinue(u8 ID);
void AT_TableReset(u8 ID);
extern void AT_TableClear(u8 ID); //* add 250905
AT_CMD_S *AtTabGet(u8 whi);
extern void AtTabSet_ID(u8 whi, u8 id);
extern void AtTabSet_ReadTBFlag(u8 whi, u8 id);
extern void AtTabSet_PartOverMark(u8 whi, u8 id);

extern void AirPart_Date_Set(u8 airpart, u8 para0, u8 method, u8 para1, u8 para2, u8 para3);

extern u8 Get_Gas_Keep_State(void);
extern void Gas_Keep_Clear(void);

#endif

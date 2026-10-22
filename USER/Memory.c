#include "Memory.h"
#include "MoveLogic.h"
#include "fmc.h" //* 增加
#include "Heat.h"
// 记忆模式

static Union_Save Union_Save_Value[Max_Save];

#define Max_Flash_Parameter 54
uint16_t Flash_Check_Tick;
static uint16_t Flash_Parameter_Buf[Max_Flash_Parameter + 2];

u8 DIY_Save_Flag = 0;
bool Manual_Save_Flag = 0;
u32 Manual_Save_tick = 0;
u8 Manual_Save_AirState = 0;
u8 Manual_Save_AirIntensity = 0;
bool LoadSave_Flag = 0; // * 读取收藏标志位

#ifdef PART_USE_SUM_TIME
#define PART_USE_SUM_ADDR 0x08040000                    // * 增加 2K 0x0807a000-0x0807a800 250528
static uint16_t Flash_USE_SUM_Buf[Max_USE_SUM * 2 + 2]; // @ AI 改32位 260927 每值拆2半字 + 2魔术字
static uint32_t PART_USE_Time_S[Max_USE_SUM];
uint32_t PART_USE_Time[Max_USE_SUM]; // @ AI 防溢出 260927 原uint16_t(≈45天)
void Flash_PART_USE_read(void)
{
    u8 i;
    GDFLASH_Read(PART_USE_SUM_ADDR, Flash_USE_SUM_Buf, Max_USE_SUM * 2 + 2);
    if (Flash_USE_SUM_Buf[Max_USE_SUM * 2] == 0x5aa5 && Flash_USE_SUM_Buf[Max_USE_SUM * 2 + 1] == 0xfeef) // 有数据
    {
        for (i = 0; i < Max_USE_SUM; i++)
        {
            PART_USE_Time[i] = (uint32_t)Flash_USE_SUM_Buf[i * 2] | ((uint32_t)Flash_USE_SUM_Buf[i * 2 + 1] << 16); // @ AI 拼32位 260927 低半字|高半字<<16
            PART_USE_Time_S[i] = PART_USE_Time[i] * 60;
        }
    }
    else
    {
        memset(PART_USE_Time, 0, sizeof(PART_USE_Time));
    }
}

static void Flash_PART_USE_Write(void)
{
    u8 i;
    for (i = 0; i < Max_USE_SUM; i++)
    {
        Flash_USE_SUM_Buf[i * 2] = (uint16_t)(PART_USE_Time[i] & 0xFFFF);  // @ AI 拆32位 260927 低半字
        Flash_USE_SUM_Buf[i * 2 + 1] = (uint16_t)(PART_USE_Time[i] >> 16); // @ AI 拆32位 260927 高半字
    }

    Flash_USE_SUM_Buf[Max_USE_SUM * 2] = 0x5aa5;
    Flash_USE_SUM_Buf[Max_USE_SUM * 2 + 1] = 0xfeef;

    GDFLASH_Write(PART_USE_SUM_ADDR, Flash_USE_SUM_Buf, Max_USE_SUM * 2 + 2);
}

void PartUse_Process_S(void)
{
    if (LDDG_NoAction == 0)
        PART_USE_Time_S[8]++; // * 小腿电动杆 计数s
    if (BDDG_NoAction == 0)
        PART_USE_Time_S[9]++; // * 靠背电动杆 计数s

    if (WorkStateGet() != WorkState_Work)
        return;

    // if (MSG4Move[MoveUp].Knead.Finish == 0)
    u8 method = MSG2Move[MoveUp].Knead.Method;
    if (method == KNEAD_FWD || method == KNEAD_FWD_CLK || method == KNEAD_FWD_CNT ||
        method == KNEAD_REV || method == KNEAD_REV_CLK || method == KNEAD_REV_CNT ||
        method == KNEAD_BACK_FORTH || method == KNEAD_1F2R || method == KNEAD_1R2F ||
        method == KNEAD_1F2F || method == KNEAD_1R2R) // todo md 260927
        PART_USE_Time_S[1]++;                         // * 揉捏 计数s

    if (MSG2Move[MoveUp].Tap.Speed > 0) // todo md 260927
        PART_USE_Time_S[2]++;           // * 拍打 计数s

    method = MSG2Move[MoveUp].Tread.Method;
    if (MSG4Move[MoveUp].Tread.Finish == 0 || method == TREAD_BACK_AND_FORTH || method == TREAD_BACK_AND_FORTH_CUR_POINT)
        PART_USE_Time_S[3]++; // * 伸缩 计数s

    method = MSG2Move[MoveUp].Flex.Method;
    if (MSG4Move[MoveUp].Flex.Finish == 0 || method == FLEX_BACK_FORTH || method == FLEX_BACK_FORTH2)
        PART_USE_Time_S[4]++; // * 伸缩 计数s

#ifdef USE_M2
    // if (MSG4Move[MoveDw].Knead.Finish == 0)
    method = MSG2Move[MoveDw].Knead.Method;
    if (method == KNEAD_FWD || method == KNEAD_FWD_CLK || method == KNEAD_FWD_CNT ||
        method == KNEAD_REV || method == KNEAD_REV_CLK || method == KNEAD_REV_CNT ||
        method == KNEAD_BACK_FORTH || method == KNEAD_1F2R || method == KNEAD_1R2F ||
        method == KNEAD_1F2F || method == KNEAD_1R2R) // todo md 260927
        PART_USE_Time_S[5]++;
    if (MSG2Move[MoveDw].Tap.Speed > 0)
        PART_USE_Time_S[6]++; // * 拍打 计数s
    if (MSG4Move[MoveDw].Tread.Finish == 0)
        PART_USE_Time_S[7]++; // * 行走 计数s
#endif
}

void PartUse_Process_Min(void)
{
    if (WorkStateGet() != WorkState_Work)
        return;
    PART_USE_Time[10]++; // @ 总运行时间 260927
    if (AirModeGet() != AirModeClose)
        PART_USE_Time[0]++;     // * 气泵 计数min
    if (WorkMinGet() % 10 != 5) // @ 5分钟 15分钟 25分钟 保存运行时间 260927
        return;
    PART_USE_Time[1] = PART_USE_Time_S[1] / 60;
    PART_USE_Time[2] = PART_USE_Time_S[2] / 60;
    PART_USE_Time[3] = PART_USE_Time_S[3] / 60; // * 行走 计数min
    PART_USE_Time[4] = PART_USE_Time_S[4] / 60;
#ifdef USE_M2
    PART_USE_Time[5] = PART_USE_Time_S[5] / 60;
    PART_USE_Time[6] = PART_USE_Time_S[6] / 60;
    PART_USE_Time[7] = PART_USE_Time_S[7] / 60; // * 下行走 计数min
#endif
    PART_USE_Time[8] = PART_USE_Time_S[8] / 60;
    PART_USE_Time[9] = PART_USE_Time_S[9] / 60;

    Flash_PART_USE_Write();
}

uint32_t Part_USE_Get(u8 cnt)
{
    return PART_USE_Time[cnt];
}
#endif

void Flash_Parameter_read(void)
{
    // u8 i;
    GDFLASH_Read(SYSTEM_PARA_ADDR, Flash_Parameter_Buf, Max_Flash_Parameter + 2);

    if (Flash_Parameter_Buf[Max_Flash_Parameter] == 0x5aa5 &&
        Flash_Parameter_Buf[Max_Flash_Parameter + 1] == 0xfeef) // 有数据
    {
        // nBlueState = Flash_Parameter_Buf[0];  //@ rm 260310
        UnionState_Flash.Strack = Flash_Parameter_Buf[1]; // todo add 260702
        nVolume = Flash_Parameter_Buf[2];
        if (nVolume > 15)
            nVolume = 8;
        UnionState.Strack = Flash_Parameter_Buf[3]; // * 修改
        Dispaly_Language = Flash_Parameter_Buf[4];
#ifdef USE_SOUNDWAVE
        nVolume_SoundWave = Flash_Parameter_Buf[5];
        if (nVolume_SoundWave > 15)
            nVolume_SoundWave = 9;
#endif
        Memorystate = Flash_Parameter_Buf[8] & 0x0f; //* add
        RedLight_Duty = Flash_Parameter_Buf[9];
        if (RedLight_Duty > 3)
            RedLight_Duty = 3;
    }
    else
    {
        nVolume = 8;
        RedLight_Duty = 2;
#ifdef USE_SOUNDWAVE
        nVolume_SoundWave = 9;
#endif
        UnionState.Strack = 0xff; // * 默认全打开
        Dispaly_Language = DefaultLanguage;
        UnionState_Flash.Strack = 0xff; // todo add 260702
    }
#ifdef USE_SOUNDWAVE
    nVolume_Real = nVolume; // @ add 260326
#endif

#ifdef RecPreSaveEnable
    RecPreFlashRead();
#endif
    memory_read();

    //  User_custom_Read();
}

void Flash_Parameter_Write(void)
{
    if (
        UnionState_Flash.Strack != Flash_Parameter_Buf[1] ||
        nVolume != Flash_Parameter_Buf[2] ||
        UnionState.Strack != Flash_Parameter_Buf[3] ||
        Dispaly_Language != Flash_Parameter_Buf[4] ||
#ifdef USE_SOUNDWAVE
        nVolume_SoundWave != Flash_Parameter_Buf[5] ||
#endif
        Memorystate != Flash_Parameter_Buf[8] || //* add
        RedLight_Duty != Flash_Parameter_Buf[9])
    {
        Flash_Parameter_Buf[1] = UnionState_Flash.Strack;
        Flash_Parameter_Buf[2] = nVolume;
        Flash_Parameter_Buf[3] = UnionState.Strack;
        Flash_Parameter_Buf[4] = Dispaly_Language;
#ifdef USE_SOUNDWAVE
        Flash_Parameter_Buf[5] = nVolume_SoundWave;
#endif
        Flash_Parameter_Buf[8] = Memorystate; //* add
        Flash_Parameter_Buf[9] = RedLight_Duty;

        // Flash_Parameter_Buf[52] = PwrLowModeFlag;
        // Flash_Parameter_Buf[53] = nScreen;
        Flash_Parameter_Buf[54] = 0x5aa5;
        Flash_Parameter_Buf[55] = 0xfeef;

        GDFLASH_Write(SYSTEM_PARA_ADDR, Flash_Parameter_Buf, Max_Flash_Parameter + 2);
    }
}

void memory_read(void)
{
    u8 i, j;
    u16 nCheckSum;
    GDFLASH_Read(USER_PROGRAM_ADDR, Union_Save_Value[0].Save_Value, (Max_Save_world * Max_Save));

    // 检验校验和,不正确禁止使用
    for (i = 0; i < Max_Save; i++)
    {
        nCheckSum = 0;

        for (j = 0; j < Max_Save_world; j++)
            nCheckSum += Union_Save_Value[i].Save_Value[j];

        if (nCheckSum != 0)
            Union_Save_Value[i].Save_struct_Value.Save_AutoMode = 0xff;
    }
}

void memory_write(void)
{
    GDFLASH_Write(USER_PROGRAM_ADDR, Union_Save_Value[0].Save_Value, Max_Save_world * Max_Save);
}

// 鸿蒙调取收藏
u8 HM_GetSaveMode(u8 nuser)
{
    u8 i;

    if (nuser >= Max_Save)
        return 0;

    i = Union_Save_Value[nuser].Save_struct_Value.Save_AutoMode;

    if (i <= Max_AutoMode)
        return (i + 1);

    return 0;
}

u8 HM_SaveMode(void)
{
    u8 i;

    for (i = 0; i < Max_Save; i++)
    {
        if (Union_Save_Value[i].Save_struct_Value.Save_AutoMode > Max_AutoMode)
        {
            return User_Memory_Save(i);
        }

        if (Union_Save_Value[i].Save_struct_Value.Save_AutoMode == AutoModeGet()) //* 修改
            return 0;
    }
    return 0;
}

u8 HM_DeleteSaveMode(u8 x)
{
    u8 i, j;
    u16 *np1;

    if (x >= Max_Save)
        return 0;

    // if (CollectorSwitch == x + 1)
    // 	CollectorSwitch = 0;

    if (Union_Save_Value[x].Save_struct_Value.Save_AutoMode <= Max_AutoMode)
    {
        // 储存项目左移
        if (x == (Max_Save - 1))
        {
            Union_Save_Value[x].Save_struct_Value.Save_AutoMode = 0xff;
        }
        else
        {
            np1 = Union_Save_Value[x].Save_Value;

            for (i = x; i < (Max_Save - 1); i++)
            {
                for (j = 0; j < Max_Save_world; j++)
                {
                    *np1 = *(np1 + Max_Save_world);
                    np1++;
                }

                // if (CollectorSwitch == i + 2)
                // 	CollectorSwitch--;

                Union_Save_Value[i + 1].Save_struct_Value.Save_AutoMode = 0xff;
            }
        }
        memory_write();
        return 1;
    }
    return 0;
}

u8 HM_DeleteSaveAll(void)
{
    u8 i;
    for (i = 0; i < Max_Save; i++)
        Union_Save_Value[i].Save_struct_Value.Save_AutoMode = 0xff;
    memory_write();
    return 1;
}

u8 User_Memory_Save(u8 x)
{
    Save_struct *Flash_Parameter_Buf;
    u16 *np;
    u16 nCheckSum;

    u8 i;

    i = 0;

    if (x >= Max_Save)
        return 0;

    Flash_Parameter_Buf = &(Union_Save_Value[x].Save_struct_Value);

    np = Union_Save_Value[x].Save_Value;

    if (AutoModeGet() != Flash_Parameter_Buf->Save_AutoMode)
    {
        i = 1;
        Flash_Parameter_Buf->Save_AutoMode = AutoModeGet();
        // if (Flash_Parameter_Buf->Save_AutoMode == 0)
        //   Flash_Parameter_Buf->Save_AutoMode = Max_AutoMode + 1; //*手动模式 不能是0
    }

    if (AutoModeGet() == 0) // 手动模式
    {
        if (Flash_Parameter_Buf->Save_Wise != MassWiseGet(MoveUp))
        {
            Flash_Parameter_Buf->Save_Wise = MassWiseGet(MoveUp);
            i = 1;
        }
#ifdef USE_M2
        if (Flash_Parameter_Buf->Save_Wise_M2 != MassWiseGet(MoveDw))
        {
            Flash_Parameter_Buf->Save_Wise_M2 = MassWiseGet(MoveDw);
            i = 1;
        }
#endif
        if (Flash_Parameter_Buf->Save_Area != MassAreaGet(MoveUp))
        {
            Flash_Parameter_Buf->Save_Area = MassAreaGet(MoveUp);
            i = 1;
        }
#ifdef USE_M2
        if (Flash_Parameter_Buf->Save_Area_M2 != MassAreaGet(MoveDw))
        {
            Flash_Parameter_Buf->Save_Area_M2 = MassAreaGet(MoveDw);
            i = 1;
        }
#endif
        if (Flash_Parameter_Buf->Save_Speed != MassSpeedGet(MoveUp))
        {
            Flash_Parameter_Buf->Save_Speed = MassSpeedGet(MoveUp);
            i = 1;
        }
#ifdef USE_M2
        if (Flash_Parameter_Buf->Save_Speed_M2 != MassSpeedGet(MoveDw))
        {
            Flash_Parameter_Buf->Save_Speed_M2 = MassSpeedGet(MoveDw);
            i = 1;
        }
#endif
        if (Flash_Parameter_Buf->Save_Width != MassWidthGet(MoveUp))
        {
            Flash_Parameter_Buf->Save_Width = MassWidthGet(MoveUp);
            i = 1;
        }
#ifdef USE_M2
        if (Flash_Parameter_Buf->Save_Width_M2 != MassWidthGet(MoveDw))
        {
            Flash_Parameter_Buf->Save_Width_M2 = MassWidthGet(MoveDw);
            i = 1;
        }
#endif
        if (Flash_Parameter_Buf->Save_BackPos != 0 /*ManualArea_Initial*/) // 手动模式初始位置
        {
            Flash_Parameter_Buf->Save_BackPos = 0; // ManualArea_Initial;
            i = 1;
        }

        if (Flash_Parameter_Buf->Save_BackPos_M2 != 0 /*CurM2TreadCircle*/) // 手动模式初始位置
        {
            Flash_Parameter_Buf->Save_BackPos_M2 = 0; // CurM2TreadCircle;
            i = 1;
        }
    }

    if (nCurZeroState <= 3)
    {
        if (Flash_Parameter_Buf->Save_ZeroState != nCurZeroState)
        {
            Flash_Parameter_Buf->Save_ZeroState = nCurZeroState;
            i = 1;
        }
    }
    else
    {
        if (Flash_Parameter_Buf->Save_BDDGCircle != DDG_Get_BnCurCircle ||
            Flash_Parameter_Buf->Save_LDDGCircle != DDG_Get_LnCurCircle)
        {

            Flash_Parameter_Buf->Save_ZeroState = 0x80;
            i = 1;
            Flash_Parameter_Buf->Save_BDDGCircle = DDG_Get_BnCurCircle;
            Flash_Parameter_Buf->Save_LDDGCircle = DDG_Get_LnCurCircle;
        }
    }

    if (Flash_Parameter_Buf->Save_AirState != AirModeGet())
    {
        Flash_Parameter_Buf->Save_AirState = AirModeGet();
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_AirIntensity != AirIntensityGet())
    {
        Flash_Parameter_Buf->Save_AirIntensity = AirIntensityGet();
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_Roll != RollModeGet())
    {
        Flash_Parameter_Buf->Save_Roll = RollModeGet();
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_RollSpeed != RollSpeedGet()) // * 增加 250322
    {
        Flash_Parameter_Buf->Save_RollSpeed = RollSpeedGet();
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_LegKnead != KneadModeGet())
    {
        Flash_Parameter_Buf->Save_LegKnead = KneadModeGet();
        i = 1;
    }
    // #if(HEAT_BACK || HEAT_LEG || HEAT_SHOULDER || HEAT_ARM || HEAT_PJ)
#if (HEAT_BACK)
    Flash_Parameter_Buf->Save_Heat.MemoryBackHeat = HeatStateGet(BackHeat);
    Flash_Parameter_Buf->Save_Heat.MemoryBackHeatLevel = HeatLevelGet(BackHeat);
#endif

#if (HEAT_LEG)
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(LegHeat);
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(LegHeat);
#endif

#if (HEAT_SHOULDER)
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(ShoulderHeat);
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(ShoulderHeat);
#endif

#if (HEAT_ARM)
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeat = HeatStateGet(ArmHeat);
    Flash_Parameter_Buf->Save_Heat.MemoryLegHeatLevel = HeatLevelGet(ArmHeat);
#endif

#if (HEAT_PJ)
    Flash_Parameter_Buf->Save_Heat.MemoryPJHeat = HeatStateGet(PJHeat);
    Flash_Parameter_Buf->Save_Heat.MemoryPJHeatLevel = HeatLevelGet(PJHeat);
#endif

    if (Flash_Parameter_Buf->Save_Shouler != BodyArea[LB_2]) //* 修改
    {
        Flash_Parameter_Buf->Save_Shouler = BodyArea[LB_2];
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_BackIntensity != nBackIntensity)
    {
        Flash_Parameter_Buf->Save_BackIntensity = nBackIntensity;
        i = 1;
    }

    if (Flash_Parameter_Buf->Save_AutoIntensity != UserIntensityGet()) // * 增加
    {
        Flash_Parameter_Buf->Save_AutoIntensity = UserIntensityGet();
        i = 1;
    }

    if (i == 1)
    {
        nCheckSum = 0;

        for (i = 0; i < (Max_Save_world - 1); i++)
            nCheckSum += np[i];
        np[Max_Save_world - 1] = (u16)0 - nCheckSum;

        memory_write();
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 User_Memory_Use(u8 x)
{
    u8 i;
    Save_struct *nUse;

    if (x >= Max_Save)
        return 0;

    nUse = &(Union_Save_Value[x].Save_struct_Value);

    if (nUse->Save_AutoMode > Max_AutoMode)
        return 0; // 调用失败

    memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T)); // * 初始化跟随 250930
    memset(&ASSIGN_Array, 0, ASSIGNum);                  // * 赋值清零 251219

    //    Air_Stre_State = 0;
    if (AutoModeGet() > 0 && nUse->Save_AutoMode == 0) // * 自动转手动 250323
    {
        UserFunc_Set(UserFunc_ManualMode, 0);
    }

    AutoModeSet(nUse->Save_AutoMode);
    AutoPreModeSet(0); /* 将Pre参数清零，不管当前运行的自动程序和调用的自动程序是否是同一个，都能触发切换逻辑 */

    if (AutoModeGet() > 0)
    {
        Manual_Save_AirState = nUse->Save_AirState; // * 打补丁 先关闭再打开 250410
        Manual_Save_AirIntensity = nUse->Save_AirIntensity;
        AirModeSet(AirModeClose);
        memset(&AirT_Work, 0, sizeof(AirT_Work_S));
        // AirModeSet(nUse->Save_AirState);
        // AirIntensitySet(nUse->Save_AirIntensity);
    }
    else
    {
        Manual_Save_AirState = nUse->Save_AirState;
        Manual_Save_AirIntensity = nUse->Save_AirIntensity;
        AirModeSet(AirModeClose);
        memset(&AirT_Work, 0, sizeof(AirT_Work_S));
        Air_All_Change(AirModeGet()); // * add delete air bug 251204
        TICK_RESET(Manual_Save_tick);
        Manual_Save_Flag = 1; // * 打补丁 250323

        RollModeSet(nUse->Save_Roll);
        RollStateSet(nUse->Save_Roll);
        RollSpeedSet(nUse->Save_RollSpeed);
        KneadModeSet(nUse->Save_LegKnead);
        KneadStateSet(nUse->Save_LegKnead);
        KneadSpeedSet(nUse->Save_LegKnead);
        KeyPush.FootRollKeyState = 1;
        KeyPush.KeyCalfKnead = 1;

        MassWiseSet(MoveUp, (ManualWise_E)nUse->Save_Wise);
        MassSpeedSet(MoveUp, nUse->Save_Speed);
        MassWidthSet(MoveUp, nUse->Save_Width);

        MassAreaSet(MoveUp, (MassageArea_E)nUse->Save_Area); //* 修改
        MoveUpState.ManualArea_ID = MassAreaGet(MoveUp);     // * 增加 250609

#ifdef USE_M2
        MassWiseSet(MoveDw, (ManualWise_E)nUse->Save_Wise_M2);
        MassSpeedSet(MoveDw, nUse->Save_Speed_M2);
        MassWidthSet(MoveDw, nUse->Save_Width_M2);
        MassAreaSet(MoveDw, (MassageArea_E)nUse->Save_Area_M2);
#endif // USE_M2
    }

    i = nUse->Save_ZeroState;

    if (i <= 3)
    {
        DDG_Set_Zero(i);
    }
    else
    {
        // DDG_Set_SitCircle(nUse->Save_BDDGCircle, nUse->Save_LDDGCircle);
        DDGVariable.UserState = DDG_UserState_Sit_Circle;
        DDGVariable.BCircleSet = nUse->Save_BDDGCircle;
        DDGVariable.LCircleSet = nUse->Save_LDDGCircle;

        nCurZeroState = 0x80;

#if (LEG_COM)
        LegAutoAdj = 1;
        LegVoid = 1; // 小腿自动避让
#endif               // LEG_COM
    }

    // if (AutoModeGet() == 0)
    // {
    //   AirModeSet((AirMode_E)nUse->Save_AirState);
    // }
    UserIntensitySet(nUse->Save_AutoIntensity); // * 增加综合强度
    nBackIntensity = nUse->Save_BackIntensity;
    if (AutoModeGet() == 0)
        MassIntensitySet(nBackIntensity << 1); //* 手动强度 *2
    else
        LoadSave_Flag = 1; // * 修改 自动模式才需要 250418

    // RollModeSet(nUse->Save_Roll);
    // RollSpeedSet(nUse->Save_RollSpeed);
    // KneadModeSet(nUse->Save_LegKnead);

#if (HEAT_BACK)
    HeatStateSet(BackHeat, nUse->Save_Heat.MemoryBackHeat, nUse->Save_Heat.MemoryBackHeatLevel);
#endif

#if (HEAT_LEG)
    HeatStateSet(LegHeat, nUse->Save_Heat.MemoryLegHeat, nUse->Save_Heat.MemoryLegHeatLevel);
#endif

#if (HEAT_SHOULDER)
    HeatStateSet(ShoulderHeat, nUse->Save_Heat.MemoryShoulderHeat, nUse->Save_Heat.MemoryShoulderHeatLevel);
#endif

#if (HEAT_ARM)
    HeatStateSet(ArmHeat, nUse->Save_Heat.MemoryArmHeat, nUse->Save_Heat.MemoryArmHeatLevel);
#endif

#if (HEAT_PJ)
    HeatStateSet(PJHeat, nUse->Save_Heat.MemoryPJHeat, nUse->Save_Heat.MemoryPJHeatLevel);
#endif

    BodyPlaceCal(nUse->Save_Shouler);

    WorkMinSet(20);
    WorkSecSet(0);

    return 1;
}

void User_Memory_Use_DIY(void) // * 读取自定义数值
{
    u8 i, temp;
    Save_struct *nUse;

    nUse = &(Union_Save_Value[4].Save_struct_Value);

    temp = M1M2_to_Handlekey(nUse->Save_Wise);
    DIY_packet[0] = 0x10 + temp;
    i = nUse->Save_Area;
    DIY_packet[1] = (i << 4) + nUse->Save_AirState;
    if (temp == 2 || temp == 4 || temp == 7 || temp == 9) //* 宽度值与手法有关
        i = 0;
    else
        i = nUse->Save_Width + 1; //* 宽度转义 + 1
    DIY_packet[2] = (i << 4) + nUse->Save_Speed;
    i = nUse->Save_AirIntensity;
    DIY_packet[3] = (i << 4) + nUse->Save_BackIntensity + 1; //* 4D+1

    i = nUse->Save_RollSpeed << 4; // * 需要保存滚轮速度
    if (nUse->Save_Heat.MemoryLegHeat)
        i |= 1;
    else
        i &= 0xfe;
    if (nUse->Save_Heat.MemoryBackHeat)
        i |= 2;
    else
        i &= 0xfd;
    if (nUse->Save_LegKnead)
        i |= 4;
    else
        i &= 0xfb;

    DIY_packet[4] = i;

    nUse = &(Union_Save_Value[5].Save_struct_Value);

    temp = M1M2_to_Handlekey(nUse->Save_Wise);
    DIY_packet[5] = 0x20 + temp;
    i = nUse->Save_Area;
    DIY_packet[6] = (i << 4) + nUse->Save_AirState;
    if (temp == 2 || temp == 4 || temp == 7 || temp == 9) //* 宽度值与手法有关
        i = 0;
    else
        i = nUse->Save_Width + 1; //* 宽度转义 + 1
    DIY_packet[7] = (i << 4) + nUse->Save_Speed;
    i = nUse->Save_AirIntensity;
    DIY_packet[8] = (i << 4) + nUse->Save_BackIntensity + 1;

    i = nUse->Save_Roll << 4;
    if (nUse->Save_Heat.MemoryLegHeat)
        i |= 1;
    else
        i &= 0xfe;
    if (nUse->Save_Heat.MemoryBackHeat)
        i |= 2;
    else
        i &= 0xfd;
    if (nUse->Save_LegKnead)
        i |= 4;
    else
        i &= 0xfb;
    DIY_packet[9] = i;

    if (DIY_packet[1] > 0x80 || DIY_packet[2] > 0x80 || DIY_packet[3] > 0x80) // * 默认值 250410
    {
        DIY_packet[0] = 0x12;
        DIY_packet[1] = 0x43;
        DIY_packet[2] = 0x02;
        DIY_packet[3] = 0x31;
        DIY_packet[4] = 0x10;
    }
    if (DIY_packet[6] > 0x80 || DIY_packet[7] > 0x80 || DIY_packet[8] > 0x80) // * 默认值 250410
    {
        DIY_packet[5] = 0x23;
        DIY_packet[6] = 0x33;
        DIY_packet[7] = 0x12;
        DIY_packet[8] = 0x22;
        DIY_packet[9] = 0x15;
    }
}

void Flash_Check_Deal(void) // todo add 260821
{
    if (Flash_Check_Tick > 0) // * 修改成 关机保存
    {
        if (--Flash_Check_Tick == 0)
        {
            Flash_Parameter_Write();
        }
    }
}

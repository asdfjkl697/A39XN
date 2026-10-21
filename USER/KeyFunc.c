#include "KeyFunc.h"
#include "Voice.h"
#include "health.h"
#include "System.h"
#include "Memory.h"
#include "Table.h"
#include "MassageLogic.h"
#include "Leg.h"

KeyFunc_S Key;

// u8 TreadKeyState=0,LegFlexKeyState=0,BackFlexKeyState=0,FlexKeyState=0,FootFlexKeyState=0,M2TreadKeyState=0;

KeyPush_t KeyPush;

// u8 Buzzer=0;
u8 Power_key_Tick;

void KeyFuncInit()
{

    memset(&Key, 0, sizeof(KeyFunc_S));
}

u8 Handlekey_to_M1M2(u8 temp) //* 动作转义
{
    u8 i;
    switch (temp)
    {
    default: // 停止
        i = TZ;
        break;
    case 1: // 手法1 停止
        i = TZ;
        break;
    case 2: // 揉捏
        i = RNZ;
        break;
    case 3: // 拍打2
        i = PD;
        break;
    case 4: // 揉捏拍打
        i = RPZ;
        break;
    case 5: // 敲击
        i = PDB;
        break;
    case 6: // 指压
        i = ZYS;
        break;
    case 7: // 3D1
        i = D3RNZ;
        break;
    case 8: // 3D2
        i = D3PD;
        break;
    case 9: // 3D3
        i = D3RPZ;
        break;
    case 10: // 3D指压
        i = ZYD;
        break;
    }
    return i;
}

u8 M1M2_to_Handlekey(u8 temp) //* 动作转义反
{
    u8 i;
    switch (temp)
    {
    default: // 停止
        i = TZ;
        break;
    case TZ: // 手法1 停止
        i = 1;
        break;
    case RNZ: // 揉捏
        i = 2;
        break;
    case PD: // 拍打2
        i = 3;
        break;
    case RPZ: // 揉捏拍打
        i = 4;
        break;
    case PDB: // 敲击
        i = 5;
        break;
    case ZYS: // 指压
        i = 6;
        break;
    case D3RNZ: // 3D1
        i = 7;
        break;
    case D3PD: // 3D2
        i = 8;
        break;
    case D3RPZ: // 3D3
        i = 9;
        break;
    case ZYD: // 3D指压
        i = 10;
        break;
    }
    return i;
}

u8 HandleArea_to_M1M2(u8 temp) //* 区域转义
{
    u8 i;
    switch (temp)
    {
    case Area_Upper:
        i = 3;
        break;

    case Area_Lower:
        i = 4;
        break;

    case Area_Full:
        i = 2;
        break;

    case Area_Fixed:
        i = 0;
        break;

    case Area_Part:
        i = 1;
        break;
    default:
        i = 0;
        break;
    }
    return i;
}

u8 M1M2_to_HandleArea(u8 temp)
{
    u8 i;
    switch (temp)
    {
    case 3:
        i = Area_Upper;
        break;

    case 4:
        i = Area_Lower;
        break;

    case 2:
        i = Area_Full;
        break;

    case 0:
        i = Area_Fixed;
        break;

    case 1:
        i = Area_Part;
        break;

    default:
        i = 0;
        break;
    }
    return i;
}

void KeyPowerScan(void) //* 100ms
{
    if (0 == gpio_input_bit_get(GPIOE, GPIO_PIN_1))
    {
        if (Power_key_Tick < 10)
        {
            Power_key_Tick++;
            if (Power_key_Tick == 10) // * 20->10 250214
            {
                Key.State = 2;
                Key.Value = Key_Power;
                Key.Param = 0;
                KeyFunc();
            }
        }
    }
    else
    {
#ifdef A560_ARMKEY
        if (Power_key_Tick > 1 && Power_key_Tick < 10)
        {
            Key.State = 2;
            Key.Value = Key_Pause;
            KeyFunc();
        }
#endif
        Power_key_Tick = 0;
    }

    // * 行走按键到位报警声
    // if ((DDG_Get_BnCurCircle >= DDG_BCircle_Max || DDG_Get_BnCurCircle == 0) && KeyPush.BackFlexKeyState != 0)
    //     Buzzer = SOUND_ALARM;
    if ((KeyPush.FlexKeyState != 0 || KeyPush.LegFlexKeyState != 0 || KeyPush.BackFlexKeyState != 0) && DDG_Get_Compete)
    {
        Buzzer_Ararm = 0x01; //* 急促蜂鸣器
    }
    else
        Buzzer_Ararm &= 0xfe;

    if (KeyPush.FootFlexKeyState && (GetLegDataRx()->Singal.Top || GetLegDataRx()->Singal.Bot)) // * 小腿伸缩上限位报警
    {
        Buzzer_Ararm = 0x20;
    }
    else
        Buzzer_Ararm &= 0xdf;

    // todo add 261015
    if ((KeyPush.RockKeyState && GetLegDataRx()->Rock.Place > 0) ||
#ifdef USE_FOOT_ROCK
        (KeyPush.RockKeyState == 2 &&
         (((DDG_Get_BnCurCircle < ROCK_DEF_BACK && DDG_Get_LnCurCircle < ROCK_DEF_LEG + 9) ||
           DDG_Get_LnCurCircle < ROCK_DEF_LEG) &&
          ((u16)GetLegDataRx()->Rock.CurCircle_H8 << 8) | GetLegDataRx()->Rock.CurCircle_L8) >= ROCKPOS_N2)
#else
        0
#endif
    )
    {
        Buzzer_Ararm = 0x80;
    }
    else
        Buzzer_Ararm &= 0x7f;
}

void KeyFunc(void)
{
    u16 i;
    u8 temp, temp1;
    //		u32 j;
    if (Key.State == 0x00) // 无按键
    {
    }
    else if (Key.State == 0x01) // 按键按下
    {
        // 按键按下时 需要清零肩位置超时时间
        switch (Key.Value)
        {
        case Key_BackTread:

            if (((WorkStateGet() != WorkState_Work) && (WorkStateGet() != WorkState_WaitFunc)) || AutoModeGet())
            {
                Buzzer = SOUND_ALARM;
                return;
            }
#ifdef USE_M2
            if (MoveWhichGet())
            {
                if (Key.Param == 1)
                {
                    KeyPush.M2TreadKeyState = TreadState_Up;
                }
                else if (Key.Param == 2)
                {
                    KeyPush.M2TreadKeyState = TreadState_Dw;
                }
                return;
            }
#endif
            if (Key.Param == 1) // 上
            {
                if (((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc)) && AutoModeGet() == 0)
                {
                    if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
                    {
                        KeyPush.TreadKeyState = TreadState_Up;
                    }
                }
            }
            else if (Key.Param == 2) // 下
            {
                if (((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc)) && AutoModeGet() == 0)
                {
                    if (MassAreaGet(MoveUp) == Area_Fixed || MassAreaGet(MoveUp) == Area_Part)
                    {
                        KeyPush.TreadKeyState = TreadState_Dw;
                    }
                }
            }
            break;

        case Key_Lying:

            FootFlexStateSet(FlexStop); /* 调节推杆时，停止当前找脚动作，调节完成后看情况继续 */
            FootFlexSpeedSet(0);

            KeyPush.LegFlexKeyState = 0;
            KeyPush.BackFlexKeyState = 0;

            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause) // || !nPowerOn)
            {
                KeyPush.FlexKeyState = 0;
                Buzzer = SOUND_ALARM;
                break;
            }

            if (DDGVariable.DDGAutoControl)
            {
                Buzzer = SOUND_ALARM;
                break;
            }

            if (Key.Param == 1) // 起立
            {
                KeyPush.FlexKeyState = DDG_UserState_SitUp;
                DDG_Set_SitUp();
            }
            else if (Key.Param == 2)
            {
                KeyPush.FlexKeyState = DDG_UserState_SitDown;
                DDG_Set_SitDown();
            }
            break;

        case Key_BackLying:

            FootFlexStateSet(FlexStop); /* 调节推杆时，停止当前找脚动作，调节完成后看情况继续 */
            FootFlexSpeedSet(0);

            KeyPush.LegFlexKeyState = 0;
            KeyPush.BackFlexKeyState = 0;

#ifdef AutoLying_Control_BackLeg
            Manual_BackLeg_Flag = 1;
#endif
            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause) // || !nPowerOn)
            {
                KeyPush.FlexKeyState = 0;
                Buzzer = SOUND_ALARM;
                break;
            }

            if (DDGVariable.DDGAutoControl)
            {
                Buzzer = SOUND_ALARM;
                break;
            }

            if (Key.Param == 0) // 起立
            {
                KeyPush.FlexKeyState = DDG_UserState_SitUp;
                DDG_Set_BackUp();
            }
            else
            {
                KeyPush.FlexKeyState = DDG_UserState_SitDown;
                DDG_Set_BackDown();
            }
            break;

        case Key_LegsUpDown:

            FootFlexStateSet(FlexStop); /* 调节推杆时，停止当前找脚动作，调节完成后看情况继续 */
            FootFlexSpeedSet(0);

            KeyPush.FlexKeyState = 0;
            KeyPush.BackFlexKeyState = 0;

#ifdef AutoLying_Control_BackLeg
            Manual_BackLeg_Flag = 1;
#endif
            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause)
            {
                KeyPush.LegFlexKeyState = 0;
                Buzzer = SOUND_ALARM;
                break;
            }

            if (DDGVariable.DDGAutoControl)
            {
                Buzzer = SOUND_ALARM;
                break;
            }

            if (Key.Param == 1) //@ 升降更改 250208 先恢复
            {
                KeyPush.LegFlexKeyState = DDG_UserState_LegDown;
                DDG_Set_LegDown();
            }
            else if (Key.Param == 2)
            {
                KeyPush.LegFlexKeyState = DDG_UserState_LegUp;
                DDG_Set_LegUp();
            }
            break;

        case Key_LegsFlex:

            KeyPush.LegFlexKeyState = 0;
            KeyPush.BackFlexKeyState = 0;
            KeyPush.FlexKeyState = 0;

            // if (DDGVariable.DDGAutoControl)  //* 手动模式高于自动模式 250416
            // {
            //     Buzzer = SOUND_ALARM;
            //     break;
            // }

            DDG_Set_Stop();

            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause)
            {
                FootFlexStateSet(FlexStop);
                KeyPush.FootFlexKeyState = 0;
                Buzzer = SOUND_ALARM;
                break;
            }

            if (Key.Param == 2) // 升
            {
                Leg_Touch_Flag = 0;     // * 增加 250513
                Leg_Touch_Air_Flag = 0; // * 增加 250612
                FootFlexStateSet(FlexUp);
                FootFlexSpeedSet(5);
                KeyPush.FootFlexKeyState = FlexUp;
            }
            else if (Key.Param == 1)
            {
                FootFlexStateSet(FlexDown);
                FootFlexSpeedSet(5);
                KeyPush.FootFlexKeyState = FlexDown;
            }
            break;

        case Key_BackUpDown:

            FootFlexStateSet(FlexStop); /* 调节推杆时，停止当前找脚动作，调节完成后看情况继续 */
            FootFlexSpeedSet(0);

            KeyPush.LegFlexKeyState = 0;
            KeyPush.FlexKeyState = 0;

            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause)
            {
                Buzzer = SOUND_ALARM;
                break;
            }

            if (DDGVariable.DDGAutoControl)
            {
                Buzzer = SOUND_ALARM;
                break;
            }

            if (Key.Param == 1) // 升
            {
                KeyPush.BackFlexKeyState = DDG_UserState_BackUp;
                DDG_Set_BackUp();
            }
            else if (Key.Param == 2)
            {
                KeyPush.BackFlexKeyState = DDG_UserState_BackDown;
                DDG_Set_BackDown();
            }
            break;

        case Key_FootRock:
            if (WorkStateGet() != WorkState_Work)
            {
                KeyPush.RockKeyState = 0;
                Buzzer = SOUND_ALARM;
                break;
            }

#ifdef USE_FOOT_ROCK
            if (Key.Param == 1 || Key.Param == 2)
            {
                KeyPush.RockKeyState = Key.Param;
                LegRockManualStateSet(Key.Param, 5);
            }
#endif
            break;

        default:
            break;
        }
    }
    else if (Key.State == 0x02) // 按键抬起
    {
        switch (Key.Value)
        {
        case Key_MassageReset:
            LegFlexStateSet(5, 5, 0);
            DDG_Set_Zero(0);
            Buzzer = SOUND_ACK;
            Voice_Ack(VC_ZeroOff); // * add 250816
            break;

        case Key_AppAir:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (AirModeGet() == AirModeFull)
                    AirModeSet(AirModeLower);
                else if (AirModeGet() == AirModeLower)
                    AirModeSet(AirModeUpper);
                else if (AirModeGet() == AirModeUpper)
                {
                    AirModeSet(AirModeClose);
                    memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                }
                else
                    AirModeSet(AirModeFull);

                Buzzer = SOUND_ACK;
            }
            break;
#ifdef USE_M2
        case Key_M1_M2: // 机芯选择
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (Key.Param == 1)
                {
                    MoveWhichSet(MoveUp);
                    Buzzer = SOUND_ACK;
                }
                else if (Key.Param == 2)
                {
                    MoveWhichSet(MoveDw);
                    Buzzer = SOUND_ACK;
                }
                else if (Key.Param == 0x10) // * 增加 250522
                {
                    MoveWhichSet(MoveUp);
                }
            }
            break;
#endif

        case Key_O2:

            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                nIonState = ~nIonState;
                if (nIonState)
                {
                    NOI_ON();
                    FS_ON();
                }
                else
                {
                    NOI_OFF();
                    FS_OFF();
                }
                Buzzer = SOUND_ACK;
            }

            break;
#ifdef USE_M2
        case Key_M1M2:                                                                       // 双机芯切换
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) /* &&
                AutoModeGet() == 0*/
            )
            {
                if (MoveWhichGet())
                    MoveWhichSet(MoveUp);
                else
                    MoveWhichSet(MoveDw);
                Buzzer = SOUND_ACK;
            }
            break;
#endif

        case key_DeviceInf:
            DeviceInf_Handle_Update = 1;
            break;
        case key_DevInfOk:
            DeviceInf_Handle_Update = 0;
            break;
        case key_Error:
            DeviceInf_Error = 1;
            break;
        case key_Recovery: // * 增加 250617
            nVolume = 8;
#ifdef USE_SOUNDWAVE
            nVolume_Real = 8;
            nVolume_SoundWave = 9;
#endif
            UnionState_Flash.Strack = 0xff; // TODO 默认正常功耗 260702
            UnionState.Strack = 0xff;       // * 默认全打开
            Dispaly_Language = DefaultLanguage;
            Memorystate = 0;
            Flash_Parameter_Write();
            Buzzer = SOUND_ACK;
            break;
#ifdef USE_SOUNDWAVE
        case Key_SoundWavex:
            if (nSoundWaveSW == 0) // @ add 260424
            {
                Buzzer = SOUND_ALARM;
                break;
            }
            if (Key.Param == 0x01)
            {
                if (nVolume_SoundWave < 15)
                {
                    nVolume_SoundWave += 3; // todo md 260625
                    Buzzer = SOUND_ACK;
                }
                else
                {
                    nVolume_SoundWave = 15;
                    Buzzer = SOUND_ALARM;
                }
            }
            else if (Key.Param == 0x02)
            {
                if (nVolume_SoundWave > 2)
                {
                    nVolume_SoundWave -= 3; // todo md 260625
                    Buzzer = SOUND_ACK;
                }
                else
                {
                    nVolume_SoundWave = 0;
                    Buzzer = SOUND_ALARM;
                }
            }
            nVolume_SoundWave_Real = nVolume_SoundWave;
            Flash_Parameter_Write();
            // Flash_Check_Tick = Flash_Check_Time; // todo 测试调节音量速度 260821
            break;

        case Key_SoundWave:
            Buzzer = SOUND_ACK;
            nVolume_SoundWave = Key.Param;
            nVolume_SoundWave_Real = nVolume_SoundWave;
            Flash_Parameter_Write();
            break;

        case Key_SoundWaveSW:
            Buzzer = SOUND_ACK;
            if (Key.Param == 0x00)
            {
                nSoundWaveSW = 0;
            }
            else // todo md 260707
            {
                nSoundWaveSW = 1;
            }

            Change_Sound();
            // if (AutoModeGet() > 0) // todo md 260821
            Flash_Parameter_Write();
            if (nSoundWaveSW && BlueState_Play == 0) // @ 打开需要处理，关闭不需要 260424 音乐已经打开不切换 260822
                BlueState_Play_Deal();
            break;

        case Key_SoundMusicSW: // todo 增加 260707
            Buzzer = SOUND_ACK;
            if (nSoundMusicSW)
            {
                nSoundMusicSW = 0;
            }
            else
            {
                nSoundMusicSW = 1;
            }

            Change_Sound();
            // if (AutoModeGet() > 0) // todo md 260821
            Flash_Parameter_Write();
            if (nSoundMusicSW && BlueState_Play == 0) // @ 打开需要处理，关闭不需要 260424 音乐已经打开不切换 260822
                BlueState_Play_Deal();
            break;

        case Key_SoundUpDown:
            Buzzer = SOUND_ACK;
            // @ 待确认 260417
            if (BlueState_Play == 0)
                break;
            BlueState_Play = 1;
            if (Key.Param == 0x01)
            {
                if (Blue_Play_ID1 < VC_Music1)
                {
                }
                else if (Blue_Play_ID1 == VC_Music1)
                {
                    Blue_Play_ID1 = VC_YBMusic4;
                    Blue_Play_ID2 = VC_YBMusic4;
                }
                else
                {
                    Blue_Play_ID1--;
                    Blue_Play_ID2 = Blue_Play_ID1;
                }
            }
            else if (Key.Param == 0x02)
            {
                if (Blue_Play_ID1 >= VC_YBMusic4)
                {
                    Blue_Play_ID1 = VC_Music1;
                    Blue_Play_ID2 = VC_Music1;
                }
                else if (Blue_Play_ID1 >= VC_Music1)
                {
                    Blue_Play_ID1++;
                    Blue_Play_ID2 = Blue_Play_ID1;
                }
            }
            break;
#endif
        case key_DiyWork:
            if (nLCState) //* 老化模式 不响应 250401
            {
                Buzzer = SOUND_ALARM;
                break;
            }
            WorkMinSet(20); // * 设定时间 20分钟 250424
            WorkSecSet(0);
            Memorystate &= 0x0f; // * add 250816
            if (Key.Param == 1)
            {
                DiyWork_Deal(&DIY_packet[0]);
            }
            else if (Key.Param == 2)
            {
                DiyWork_Deal(&DIY_packet[5]);
            }

            KeyPush.FootRollKeyState = 1;
            KeyPush.KeyCalfKnead = 1;

            Buzzer = SOUND_ACK;

            if (AutoModeGet() > 0)
            {
                BlueState_Play = 0; // * 关闭背景音乐
                Blue_Play_ID1 = 0;
                Blue_Play_ID2 = 0;

                AT_TableStop(Table_Auto); //* 关闭自动程序进程 251119
                AutoModeSet(0);
                AutoPreModeSet(0);
#ifdef USE_M2
                MassWiseSet(MoveDw, TZ);
#endif
                Manual_Save_AirState = AirModeGet();
                Manual_Save_AirIntensity = AirIntensityGet();
                AirModeSet(AirModeClose);
                memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                TICK_RESET(Manual_Save_tick);
                Manual_Save_Flag = 1;
            }
            break;

        case Key_LogoCompleted:
            if (WorkStateGet() == WorkState_TurnOn)
            {
                //  State_OutTime = 150; // 超时跳过logo
                //								j=WorkTickGet();
                TICK_RESET(Work.Tick);
            }
            break;

        case Key_Volume:
            if (WorkStateGet() == WorkState_Wait)
                break;
            if (Key.Param == 0x01)
            {
                if (nVolume < 15)
                {
                    nVolume++;
                    Buzzer = SOUND_ACK;
                }
                else
                    Buzzer = SOUND_ALARM;
            }
            else if (Key.Param == 0x02)
            {
                if (nVolume > 0)
                {
                    nVolume--;
                    Buzzer = SOUND_ACK;
                }
                else
                    Buzzer = SOUND_ALARM;
            }
#ifdef USE_SOUNDWAVE
            nVolume_Real = nVolume;
#endif
            RedLight_Duty = nVolume * 10; // todo test 261016
            if (RedLight_Duty > 100)
                RedLight_Duty = 100;
#ifdef USE_REDLIGHT
            RedLight_PWM(RedLight_Duty); // todo test 261015
#else
            Heat3_On(RedLight_Duty);
#endif
            Flash_Parameter_Write();
            // Flash_Check_Tick = Flash_Check_Time;
            break;

        case Key_Volume_Set:
            Buzzer = SOUND_ACK;
            if (Key.Param <= 15)
            {
                nVolume = Key.Param;
            }
            else
                Buzzer = SOUND_ALARM;
            Flash_Parameter_Write();
            break;

        case Key_Player:
            if (nVoicePlayer)
            {
                nVoicePlayer = 0;
                Voice_Ack(VC_BbOff); // todo add 261011
            }
            else
            {
                nVoicePlayer = 1;
                Voice_Ack(VC_BbOn); // todo add 261011
            }
            Flash_Parameter_Write();
            Buzzer = SOUND_ACK;
            break;

        case Key_MasMode: // * 三种按摩强度

            if (Key.Param > 3)
                Key.Param = 3;

            UserIntensitySet(Key.Param); // * 只是显示用
            if (AutoModeGet() == 0)
            {
                switch (Key.Param) // * 增加 250228
                {
                case 1:
                    nBackIntensity = 1;
                    AirIntensitySet(2);
                    break;
                case 2:
                    nBackIntensity = 2;
                    AirIntensitySet(3);
                    break;
                case 3:
                    nBackIntensity = 3;
                    AirIntensitySet(4);
                    break;
                default:
                    break;
                }
                MassIntensitySet(nBackIntensity << 1);
            }
            else // * 自动模式时调整强度 250309
            {
                temp = UserIntensityGet();
                nBackIntensity = MassModeGet(AutoModeGet())->_3DIntensity[temp - 1];
                AirIntensitySet(MassModeGet(AutoModeGet())->AirIntensity[temp - 1]);
            }

            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
            }
            Buzzer = SOUND_ACK;
            break;

        case Key_Language:
            DeviceInf_Error = 0;               // * 关闭故障显示 250309
            if (Key.Param != Dispaly_Language) // * 修改
            {
                Dispaly_Language = Key.Param;
                Buzzer = SOUND_ACK;
                Flash_Parameter_Write(); // * 修改
            }
            break;

        case Key_LowPower:
            if (nLowPower)
            {
                nLowPower = 0;
            }
            else
            {
                nLowPower = 1;
            }
            Flash_Parameter_Write();
            Buzzer = SOUND_ACK;
            break;

        case Key_Lying:
        case Key_BackLying:
        case Key_LegsUpDown:
        case Key_BackUpDown:
            if (WorkStateGet() == WorkState_FCT)
                break;
#ifdef AutoLying_Control_BackLeg
            Manual_BackLeg_Flag = 0;
#endif
            KeyPush.FlexKeyState = 0;
            KeyPush.LegFlexKeyState = 0;
            KeyPush.BackFlexKeyState = 0;
            DDG_Set_Stop();
            break;

        case Key_FootRock:
            KeyPush.RockKeyState = 0;
            LegRockManualStateSet(0, 0);
            break;

        case Key_BackTread:
#ifdef USE_M2
            KeyPush.M2TreadKeyState = TreadState_Stop;
#endif
            KeyPush.TreadKeyState = TreadState_Stop;
            break;

        case Key_LegsFlex:
            if (WorkStateGet() == WorkState_FCT)
                break;
#ifdef AutoLying_Control_BackLeg
            Manual_BackLeg_Flag = 0;
#endif
            KeyPush.FootFlexKeyState = FlexStop;
            FootFlexStateSet(FlexStop);
            FootFlexSpeedSet(0);
            break;

        case Key_FCT:

            //            if (WorkStateGet() != WorkState_FCT)
            //            {
            //                if (!Key.Param )
            //                {
            //                    Enter_Fct();
            //                    Buzzer = SOUND_ACK;
            //                }
            //                else
            //                {
            //                    Buzzer = SOUND_ALARM;
            //                }
            //            }
            //            else
            //            {
            //                Fct_Update(Key.Param );
            //            }

            break;

        case Key_BackStrength:

            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {

                if (MoveWhichGet())
                {
#ifndef USE_M2_4D
                    MoveWhichSet(MoveUp); // * 增加 250521
                    Buzzer = SOUND_ALARM; /* 下机芯一般不调4D强度 */
#else
                    if (Key.Param == 0x02) //-
                    {
                        if (nBackIntensity_M2 > 0)
                        {
                            nBackIntensity_M2--;
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    else if (Key.Param == 0x01) //+
                    {
                        if (nBackIntensity_M2 < 4)
                        {
                            nBackIntensity_M2++;
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    if (AutoModeGet() == 0)                     // * 手动模式直接设置3D强度、自动模式还要参考结合动作表
                        MassIntensitySet_M2(nBackIntensity_M2); //* 测试 250807
                    Move_Flag[MoveDw].Inten = 1;                // * add 250901
#endif
                    break;
                }
                else
                {
                    if (Key.Param == 0x02) //-
                    {
                        if (nBackIntensity > 0)
                        {
                            nBackIntensity--;
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    else if (Key.Param == 0x01) //+
                    {
                        if (nBackIntensity < 4)
                        {
                            nBackIntensity++;
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    if (AutoModeGet() == 0)               // * 手动模式直接设置3D强度、自动模式还要参考结合动作表
                        MassIntensitySet(nBackIntensity); //* 测试 250807
                    Move_Flag[MoveUp].Inten = 1;          // * add 250901
                    break;
                }
            }
            break;

        case Key_4DIntensity:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (MoveWhichGet()) // @ 增加 250523
                    MoveWhichSet(MoveUp);

                nBackIntensity = Key.Param - 1;
                Buzzer = SOUND_ACK;
                if (AutoModeGet() == 0) // * 手动模式直接设置3D强度、自动模式还要参考结合动作表
                    MassIntensitySet(nBackIntensity);
                Move_Flag[MoveUp].Inten = 1; // @ add 250901
            }
            break;

        case Key_Blue:
            nBlueState = ~nBlueState;
            Flash_Parameter_Write();
            if (nBlueState) // * 测试关闭蓝牙 背景音乐停止时间过长 250412
            {
                Blue_OnOff(1);
                Voice_Ack(VC_BlueOn);
            }
            else
            {
                Blue_OnOff(0);
                Voice_Ack(VC_BlueOff);
            }

            Buzzer = SOUND_ACK;

            break;

        case Key_BackHeat:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                KeyPush.KeyBackHeat = 1;
                if (HeatStateGet(BackHeat))
                    HeatStateSet(BackHeat, 0, 0);
                else
                    HeatStateSet(BackHeat, 1, 3);
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_LegsHeat:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (HeatStateGet(LegHeat))
                {
                    HeatStateSet(LegHeat, 0, 0);
                }
                else
                {
                    HeatStateSet(LegHeat, 1, 3);
                }
                KeyPush.KeyLegHeat = 1;
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_LegsKnead:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (KneadModeGet())
                {
                    KneadModeSet(0);
                    KneadStateSet(0);
                    // KneadSpeedSet(0);
                }
                else
                {
                    KneadModeSet(1);
                    KneadStateSet(1);
                    // KneadSpeedSet(1);
                }
                KeyPush.KeyCalfKnead = 1;
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_Ion:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                nIonState = ~nIonState;
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_Heat: // * 增加 250122
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (HeatStateGet(BackHeat)) // * 修改循环 250308
                {
                    if (HeatStateGet(LegHeat))
                    {
                        HeatStateSet(BackHeat, 0, 0);
                        HeatStateSet(LegHeat, 0, 0);
                    }
                    else
                    {
                        HeatStateSet(BackHeat, 0, 0);
                        HeatStateSet(LegHeat, 1, 3);
                    }
                }
                else
                {
                    if (HeatStateGet(LegHeat))
                    {
                        HeatStateSet(BackHeat, 1, 3);
                        HeatStateSet(LegHeat, 1, 3);
                    }
                    else
                    {
                        HeatStateSet(BackHeat, 1, 3);
                        HeatStateSet(LegHeat, 0, 0);
                    }
                }
                KeyPush.KeyBackHeat = 1;
                KeyPush.KeyLegHeat = 1;
                Buzzer = SOUND_ACK;
            }

            break;

        case key_SOUND_ACK: // @ 应答声 251206
            Buzzer = SOUND_ACK;
            break;

        case Key_ShoulderHeat: // 披肩加热

            break;

        case Key_Warn:

            break;

        case Key_Power:

            if (WorkStateGet() == WorkState_TurnOn) // LOGO界面不操作
                return;

            Buzzer = SOUND_ACK; // * 增加 250509
            if (nPowerOn)
            {
                if (Leg_Protect_Flag) // * 小腿防夹保护时 不能关机 250401
                    return;
                UserFunc_Set(UserFunc_PowerOff, 0);
#ifdef VIETNAM_VOICE
                Voice_Ack(VC_MassageOff);
#else
                Voice_Ack(VC_PowerOff);
#endif
            }
            else
            {
                UserFunc_Set(UserFunc_PowerOn, 0);
                // Buzzer = SOUND_KJ;
                Voice_Ack(VC_PowerOn);
            }
            break;

        case Key_Pause:
#if (USE_Health_Check)
            if (Health_State > 0)
                break;
#endif
            if (WorkStateGet() == WorkState_Pause)
            {
                Exit_Pause();
                Buzzer = SOUND_ACK;
                if (Leg_Protect_Flag == 0) // * 防夹时 没有按摩继续声音 250513
                    Voice_Ack(VC_MassContinue);
            }
            else
            {
                if (nPowerOn || (WorkStateGet() == WorkState_Reset)) // * 修改 250409
                {
                    Enter_Pause();
                    Buzzer = SOUND_ACK;
                    Voice_Ack(VC_MassPause);
                }
            }
            break;
        case Key_Zero:
            if (WorkStateGet() == WorkState_FCT || WorkStateGet() == WorkState_Pause) // || !nPowerOn)
                break;

            if (0xff == Key.Param)
            {
                if (nCurZeroState < AUTO_SIT_ZERO3)
                    DDG_Set_Zero(nCurZeroState + 1);
                else if (nCurZeroState == AUTO_SIT_ZERO3) // * modify 250819
                    DDG_Set_Zero(AUTO_SIT_NONE);
                else
                    DDG_Set_Zero(AUTO_SIT_ZERO1);
            }
            else
            {
                if (Key.Param > 3)
                    Key.Param = 3;
                DDG_Set_Zero(Key.Param);
            }

            Voice_Ack(VC_ZeroOff + nCurZeroState);
            // Buzzer = SOUND_ACK;
            break;

        case Key_Air:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (Key.Param == 1)
                {
                    if (AirModeGet() == AirModeUpper) //* 已修改
                    {
                        AirModeSet(AirModeClose);
                        memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                    }
                    else
                        AirModeSet(AirModeUpper);
                }
                else if (Key.Param == 2)
                {
                    if (AirModeGet() == AirModeLower)
                    {
                        AirModeSet(AirModeClose);
                        memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                    }
                    else
                        AirModeSet(AirModeLower);
                }
                else if (Key.Param == 3)
                {
                    if (AirModeGet() == AirModeFull)
                    {
                        AirModeSet(AirModeClose);
                        memset(&AirT_Work, 0, sizeof(AirT_Work_S));
                    }
                    else
                        AirModeSet(AirModeFull);
                }
                else
                {
                    AirModeSet(AirModeClose);
                    memset(&AirT_Work, 0, sizeof(AirT_Work_S)); // * 初始化气压参数 251209
                }
                Air_All_Change(AirModeGet()); // * 更新气压程序 251113
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_UpperAir:
            // if (AutoModeGet() > 0)
            // {
            //     Buzzer = SOUND_ALARM;
            //     break;
            // }
            // if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            // {
            //     if (Key.Param == 1)
            //     {
            //         if (AirModeGet() == AirModeFull)
            //             AirModeSet(AirModeClose);
            //         else
            //             AirModeSet(AirModeFull);
            //     }
            //     else if (Key.Param == 2)
            //     {
            //         if (AirModeGet() == AirModeUpper)
            //             AirModeSet(AirModeClose);
            //         else
            //             AirModeSet(AirModeUpper);
            //     }
            //     else if (Key.Param == 3)
            //     {
            //         if (AirModeGet() == AirModeLower)
            //             AirModeSet(AirModeClose);
            //         else
            //             AirModeSet(AirModeLower);
            //     }
            //     Buzzer = SOUND_ACK;
            // }
            break;

        case Key_LowerAir:
            // if (WorkStateGet() == WorkState_Work)
            // {
            //     if (Air_Stre_State > 0)
            //     {
            //         Buzzer = SOUND_ALARM;
            //         break;
            //     }

            //     if (Key.Param  == 1)
            //     {
            //         nCurAirState |= Air_State_Leg;
            //         AirModeSet((Air_State_Enum)nCurAirState);
            //     }
            //     else
            //     {
            //         nCurAirState &= ~Air_State_Leg;
            //         AirModeSet((Air_State_Enum)nCurAirState);
            //     }
            //     Buzzer = SOUND_ACK;
            // }
            return;

        case key_AutoLyJump:
            TICK_RESET(Work.Tick);
            DDG_Set_Stop();

            FootFlexStateSet(FlexStop);
            AutoLyingSkip_Flag = 1; // * 跳过标志
            Buzzer = SOUND_ACK;
            break;

        case Key_UpperAirStrength:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (AirModeGet() != AirModeClose)
                {
                    i = AirIntensityGet();
                    Buzzer = SOUND_ACK; //* 转移
                    if (0x01 == Key.Param)
                    {
                        if (++i > Air_Intensity_Max)
                        {
                            i = Air_Intensity_Max;
                            Buzzer = SOUND_ALARM;
                        }
                        AirIntensitySet(i);
                    }
                    else if (0x02 == Key.Param)
                    {
                        if (--i < 1)
                        {
                            i = 1;
                            Buzzer = SOUND_ALARM;
                        }
                        AirIntensitySet(i);
                    }
                    else
                    {
                        if (++i > Air_Intensity_Max)
                            i = 1;
                        AirIntensitySet(i);
                    }
                }
            }
            break;

        case Key_AirIntensity:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (AirModeGet() != AirModeClose)
                {
                    i = Key.Param;
                    AirIntensitySet(i);
                    Buzzer = SOUND_ACK; //* 增加
                }
            }
            break;

        case Key_Wise:
            if (nLCState) //* 老化模式 不响应 250401
            {
                break;
            }
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                Memorystate &= 0x0f;                                             // * 增加 取消调用显示 250617
                if (AutoModeGet() > 0 || (WorkStateGet() == WorkState_WaitFunc)) // * 修改 250409
                {
                    MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                    MassWiseSet(MoveDw, TZ);
#endif
                    UserFunc_Set(UserFunc_ManualMode, 0);
                    i = 0;
                }

                switch (Key.Param)
                {
                default: // 停止
                    i = TZ;
                    break;
                case 1: // 手法1 停止
                    i = TZ;
                    break;
                case 2: // 揉捏
                    i = RNZ;
                    break;
                case 3: // 拍打
                    i = PD;
                    break;
                case 4: // 揉捏拍打
                    i = RPZ;
                    break;
                case 5: // 敲击
                    i = PDB;
                    break;
#ifdef USE_S0555
                case 6: // 指压
                    i = ZYS;
                    break;
                case 7: //* 指压2
                    i = ZY;
                    break;
                case 8: //* 肩部提抓
                    i = JRZ;
                    break;
                case 9: // 3D1
                    i = D3RNZ;
                    break;
                case 10: // 3D2
                    i = D3PD;
                    break;
                case 11: // 3D3
                    i = D3RPZ;
                    break;
                case 12: // 3D指压
                    i = ZYD;
                    break;
#else
                case 6: // 指压
                    i = ZYS;
                    break;
                case 7: // 3D1
                    i = D3RNZ;
                    break;
                case 8: // 3D2
                    i = D3PD;
                    break;
                case 9: // 3D3
                    i = D3RPZ;
                    break;
                case 10: // 3D指压
                    i = ZYD;
                    break;
#endif
                }
#ifdef USE_M2
                if (MoveWhichGet())
                {
                    if (0 == MassSpeedGet(MoveDw))
                        MassSpeedSet(MoveDw, 3);

                    MassWiseSet(MoveDw, (ManualWise_E)i);
                }
                else
#endif
                {
                    if (0 == MassSpeedGet(MoveUp))
                        MassSpeedSet(MoveUp, 3);

                    MassWiseSet(MoveUp, (ManualWise_E)i);
                    MassIntensitySet(nBackIntensity << 1); // * 增加让 ZYD ZYS 工作 250319
                }
                KeyPush.WiseKeyState = 1;
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_AutoMode:
            if (nLCState) //* 老化模式 不响应 250401
            {
                break;
            }
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                uint8_t AutoModeCur;
                Memorystate &= 0x0f; // * 增加 取消调用显示 250617
                // MoveUpState.ManualArea_ID = 0;
                // MoveUpState.ManualArea_ID = Area_Fixed; // * 修改 250425
                MassAreaSet(MoveUp, Area_Fixed);
                MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                MassAreaSet(MoveDw, Area_Fixed);
                MassWiseSet(MoveDw, TZ);
#endif
                AirModeSet(AirModeAuto);
                AutoModeCur = AutoModeGet();

                if (AutoModeCur != Key.Param && Key.Param <= Max_AutoMode)
                {
                    AutoModeSet(Key.Param);

                    Buzzer = SOUND_ACK;
                }
                else if (0xff == Key.Param)
                {
#ifdef USE_ONLINE_MODE
                    if (AutoModeCur < (Max_AutoMode - 5)) // * 5个在线程序、不能用快捷按键切换
#else
                    if (AutoModeCur < Max_AutoMode)
#endif
                    {
                        AutoModeSet(AutoModeCur + 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        AutoModeSet(1); // * 默认1
                        Buzzer = SOUND_ACK;
                    }
                }
                else
                    Buzzer = SOUND_ALARM;
            }
            break;

        case Key_BackSpeed:
            if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
            {
#ifdef USE_M2
                if (MoveWhichGet())
                {
                    if (MassWiseGet(MoveDw) == TZ)
                    {
                        Buzzer = SOUND_ALARM;
                        return;
                    }
                    i = MassSpeedGet(MoveDw);
                    if (Key.Param == 0x02)
                    {
                        if (i > 1)
                        {
                            MassSpeedSet(MoveDw, i - 1);
                            Buzzer = SOUND_ACK; //* 已增加
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }

                    if (Key.Param == 0x01)
                    {
                        if (i < 5)
                        {
                            MassSpeedSet(MoveDw, i + 1);
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    Move_Flag[MoveDw].Speed = 1; // * add 250924
                    return;
                }
#endif
                if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == TZ || MassWiseGet(MoveUp) == ZY)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }

                i = MassSpeedGet(MoveUp);

                if (Key.Param == 0x02)
                {
                    if (i > 1)
                    {
                        MassSpeedSet(MoveUp, i - 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }

                    else
                        Buzzer = SOUND_ALARM;
                }
                if (Key.Param == 0x01)
                {
                    if (i < 5)
                    {
                        MassSpeedSet(MoveUp, i + 1);
                        Buzzer = SOUND_ACK; //* 已增加
                    }
                    else
                        Buzzer = SOUND_ALARM;
                }
                Move_Flag[MoveUp].Speed = 1; // * add 250901
            }

            break;

        case Key_MassSpeed:
            if (WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
            {
                if (MoveWhichGet())
                {
                    if (MassWiseGet(MoveDw) == TZ)
                    {
                        Buzzer = SOUND_ALARM;
                        return;
                    }
                    i = Key.Param;
                    MassSpeedSet(MoveDw, i);
                    Buzzer = SOUND_ACK; //* 已增加
                    Move_Flag[MoveDw].Speed = 1;
                    return;
                }

                if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == TZ || MassWiseGet(MoveUp) == ZY)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }

                i = Key.Param;
                MassSpeedSet(MoveUp, i);
                Buzzer = SOUND_ACK; //* 已增加
                Move_Flag[MoveUp].Speed = 1;
            }

            break;

        case Key_BackWidth:
            if (((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc)) && AutoModeGet() == 0)
            {
#ifdef USE_M2
                if (MoveWhichGet())
                {
                    if ((RNZ == MassWiseGet(MoveDw)) || (RPZ == MassWiseGet(MoveDw)) || (JRZ == MassWiseGet(MoveDw)))
                    {
                        Buzzer = SOUND_ALARM;
                        break;
                    }

                    i = MassWidthGet(MoveDw);

                    if (Key.Param == 0x02) //-
                    {
                        if (i > 0)
                        {
                            MassWidthSet(MoveDw, i - 1);
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    else if (Key.Param == 0x01) //+
                    {
                        if (i < 4)
                        {
                            MassWidthSet(MoveDw, i + 1);
                            Buzzer = SOUND_ACK;
                        }
                        else
                            Buzzer = SOUND_ALARM;
                    }
                    Move_Flag[MoveDw].Width = 1; // * add 250923
                    return;
                }
#endif
                if ((RNZ == MassWiseGet(MoveUp)) || (RPZ == MassWiseGet(MoveUp)) || (JRZ == MassWiseGet(MoveUp)))
                {
                    Buzzer = SOUND_ALARM;
                    break;
                }

                i = MassWidthGet(MoveUp);

                if (Key.Param == 0x02)
                {
                    if (i > 0) //* 已修改
                    {
                        MassWidthSet(MoveUp, i - 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                        Buzzer = SOUND_ALARM;
                }
                else if (Key.Param == 0x01)
                {
                    if (i < 4) // * 修改 5-4 250213
                    {
                        MassWidthSet(MoveUp, i + 1);
                        Buzzer = SOUND_ACK;
                    }
                    else
                        Buzzer = SOUND_ALARM;
                }
                Move_Flag[MoveUp].Width = 1;
            }
            break;

        case Key_MassWidth:
            if (((WorkStateGet() == WorkState_Work)) && AutoModeGet() == 0)
            {
                if (MoveWhichGet())
                {
                    if ((RNZ == MassWiseGet(MoveDw)) || (RPZ == MassWiseGet(MoveDw)) || (JRZ == MassWiseGet(MoveDw)))
                    {
                        Buzzer = SOUND_ALARM;
                        break;
                    }
                    i = Key.Param; // @ 修改 250505
                    MassWidthSet(MoveDw, i - 1);
                    Buzzer = SOUND_ACK;
                    Move_Flag[MoveDw].Width = 1; // @ add 250923
                    return;
                }

                if ((RNZ == MassWiseGet(MoveUp)) || (RPZ == MassWiseGet(MoveUp)) || (JRZ == MassWiseGet(MoveUp)))
                {
                    Buzzer = SOUND_ALARM;
                    break;
                }

                i = Key.Param; // @ 修改 250505
                MassWidthSet(MoveUp, i - 1);
                Buzzer = SOUND_ACK;
                Move_Flag[MoveUp].Width = 1;
            }
            break;

        case Key_BackArea:
            if (nLCState) //* 老化模式 不响应 250401
            {
                break;
            }
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) || (AutoModeGet() != 0))
            {
                if (AutoModeGet() > 0) // * 增加 250322
                {
                    MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                    MassWiseSet(MoveDw, TZ);
#endif
                    UserFunc_Set(UserFunc_ManualMode, 0);
                }

                if (MassWiseGet(MoveUp) == JRZ)
                {
                    Buzzer = SOUND_ALARM;
                    return;
                }
#ifdef USE_M2
                if (MoveWhichGet())
                {
                    MoveUpState.ManualArea_ID = Key.Param;
                    MassAreaSet(MoveDw, (MassageArea_E)MoveUpState.ManualArea_ID);
                }
                else
#endif
                {
                    MoveUpState.ManualArea_ID = Key.Param;
                    MassAreaSet(MoveUp, (MassageArea_E)MoveUpState.ManualArea_ID);
#ifdef USE_M2
                    if (MassAreaGet(MoveUp) == Area_Full || MassAreaGet(MoveUp) == Area_Lower) // * 250425
                    {
                        Part_Date_Set(MoveDw_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 10);
                    }
#endif
                }
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_Roll:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                i = RollSpeedGet();
                if (i < 3)
                {
                    RollSpeedSet(++i);
                }
                else
                {
                    RollSpeedSet(0);
                }

                if (RollSpeedGet())
                {
                    RollModeSet(1);
                    RollStateSet(1);
                }
                else
                {
                    RollModeSet(0);
                    RollStateSet(0);
                }
                KeyPush.FootRollKeyState = 1;
                Buzzer = SOUND_ACK; //* 增加
            }
            break;

        case Key_Timing:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                i = WorkMinGet();
                i *= 60;
                i += WorkSecGet();
                Buzzer = SOUND_ACK;
                switch (Key.Param)
                {
                case 0:
                    i += 600;
                    if (i > 2400)
                        i = 2400;
                    break;
                case 1:
                    i += 300;
                    if (i > 2400)
                    {
                        i = 2400;
                        Buzzer = SOUND_ALARM;
                    }
                    break;
                case 2:
                    if (i > 300)
                        i -= 300;
                    else
                        Buzzer = SOUND_ALARM;
                    break;
                case 3:
                    if (i > 600)
                        i -= 600;
                    break;
                default:
                    break;
                }
                WorkMinSet(i / 60);
                WorkSecSet(i % 60);
            }
            break;

        case Key_LC:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (!nLCState)
                {
                    nLCState = 1;
                    AirModeSet(AirModeAuto); // * 增加 250401
                    AutoModeSet(A_LC);
#ifdef USE_SOUNDWAVE
                    BlueState_Play_Deal(); // @ add 260326
#endif
                }
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_PACK:
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                MassWiseSet(MoveUp, TZ);
#ifdef USE_M2
                MassWiseSet(MoveDw, TZ);
#endif
                // AirModeSet(AirModeClose); // * 增加 250401
                AirModeSet(AirModeAuto); // * 增加 250401
                AutoModeSet(A_PACK);
                Buzzer = SOUND_ACK;
            }
            break;

        case Key_LcdOK:
            // LCD_OK = 1;
            return;

#if (USE_Health_Check)

        case Key_Health:
            if (nLCState) //* 老化模式 不响应 250401
            {
                break;
            }
            if (Key.Param == 1) // 开始检测
            {
                if (!nPowerOn)
                    return;
                Massage_Flag.HealthPrepare = 1;
                // Enter_Pause();
                Health_Set_State(Health_State_Check);
                Buzzer = SOUND_ACK;
            }
            else
            {
                Exit_Pause();
                if (Key.Param == 3) // 调用专属程序 //* 自动程序需要一一对应 250228
                {
                    WorkMinSet(20); // * 设置按摩时间，防止相同自动程序时间不变 250515
                    WorkSecSet(0);
                    Memorystate &= 0x0f;  // * add 250816
                    switch (Health_Tired) //* 疲劳程度 0-3
                    {
#ifdef VIETNAM_VOICE
                    case 0:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(16);
                        break;
                    case 1:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(1);
                        break;
                    case 2:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(6);
                        break;
                    case 3:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(3);
                        break;
#else
                    case 0:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(4);
                        break;
                    case 1:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(1);
                        break;
                    case 2:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(3);
                        break;
                    case 3:
                        AirModeSet(AirModeAuto);
                        AutoModeSet(5);
                        break;
#endif

                    default:
                        break;
                    }
                }
                Buzzer = SOUND_ACK;
                Health_Set_State(Health_State_Stop);
            }
            break;

#endif // USE_Health_Check

        case Key_TimOutReset:
            if (nTimOutResetSw)
            {
                nTimOutResetSw = 0;
            }
            else
            {
                nTimOutResetSw = 1;
            }
            Flash_Parameter_Write();
            Buzzer = SOUND_ACK;
            break;

        case Key_BodyCheck_Jump:
            // if (WorkStateGet() == WorkState_BodyCheck)
            if (WorkStateGet() == WorkState_BodyCheck || WorkStateGet() == WorkState_AutoLying) // todo 修改为了调试跳过 260701
            {
                Massage_Flag.BodyCheck = 0;
                MassMode_Load(FA_AUTO_CLOSE);
                AT_TableClear(Table_Auto);

                Part_Date_Set(MoveUp_Knead, 1, KNEAD_FIX_CIRCLE, MOVEUPWIDTH1, 0, 3);
                Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, 0, 0, 3);
                Part_Date_Set(MoveUp_Tread, 1, TREAD_MOVE_SDR, 0, 0, 6);

                BodyPlaceCal(DEFAULT_SHOULDER);
                WorkStateSet(WorkState_Work);

                Buzzer = SOUND_ACK;
                Voice_Ack(VC_MassStart); // * 跳过  开始按摩
                TICK_RESET(Work.Tick);
            }
            break;
        case Key_Shoulder: // 肩位置调整
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc) || (WorkStateGet() == WorkState_ShoulderAdj))
            {
                if (Key.Param == 1) // up
                {
                    if (BodyArea[LB_2] >= SHOULDER_MIN)
                    {
                        BodyArea[LB_2] -= SHOULDER_MIN;
                        BodyPlaceCal(BodyArea[LB_2]);
                        Shoulder_Update = 1;
                        if (WorkStateGet() != WorkState_ShoulderAdj)
                            Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
                else if (Key.Param == 2) // down
                {
                    if ((BodyArea[LB_2]) < SHOULDER_MAX)
                    {
                        BodyArea[LB_2] += SHOULDER_MIN;
                        if (BodyArea[LB_2] > SHOULDER_MAX)
                            BodyArea[LB_2] = SHOULDER_MAX;
                        BodyPlaceCal(BodyArea[LB_2]);
                        Shoulder_Update = 1;
                        if (WorkStateGet() != WorkState_ShoulderAdj)
                            Buzzer = SOUND_ACK;
                    }
                    else
                    {
                        Buzzer = SOUND_ALARM;
                    }
                }
            }
            break;
        case Key_ShoulderAdj_Jump:
            if (WorkStateGet() == WorkState_ShoulderAdj)
            {
                WorkStateSet(WorkState_Work);
                //								j=WorkTickGet();
                TICK_RESET(Work.Tick);
                //   Air_Stre_State = 0;
                Buzzer = SOUND_ACK;
                Voice_Ack(VC_MassStart); // * 跳过  开始按摩
            }
            break;
        case Key_Voice:
            if (WorkStateGet() == WorkState_FCT)
                return;
            if (0x10 == Key.Param)
            {
                if (nVoiceControlSw)
                    Voice_Ack(VC_WakeUp);
                else
                    Buzzer = SOUND_ALARM;
            }
            else
            {
                if (nVoiceControlSw)
                {
                    Voice_Swich_Set(0);
                    Voice_Cof(0);
                    Buzzer = SOUND_ACK;
                }
                else
                {
                    Voice_Swich_Set(1);
                    Voice_Cof(1);
                    Buzzer = SOUND_ACK;
                }
            }
            Flash_Parameter_Write();
            break;

            // case Key_language:
            // 	if (WorkStateGet() == WorkState_FCT)
            // 		return;
            // 	return;

        case Key_WakeUp:
            if (WorkStateGet() == WorkState_FCT)
                break;
            if (nVoiceControlSw)
            {
                Voice_Ack(VC_WakeUp);
            }
            else
            {
                Buzzer = SOUND_ALARM;
            }
            break;

        case Key_Memory_Save: // * 修改 250123
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if (User_Memory_Save(Key.Param - 1)) //* 修改
                {
                    Memorystate |= 1 << (Key.Param - 1); //* 显示用了那个保存程序
                    Flash_Parameter_Write();
                    Buzzer = SOUND_ACK;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            break;

        case Key_Memory_Use: // * 修改 250123
            if ((WorkStateGet() == WorkState_Work) || (WorkStateGet() == WorkState_WaitFunc))
            {
                if ((Memorystate & (1 << (Key.Param - 1))) == 0) // * 没有存储不能调用 250419
                {
                    Buzzer = SOUND_ALARM;
                    break;
                }
                if (User_Memory_Use(Key.Param - 1)) //* 配合修改
                {
                    Memorystate &= 0x0f;
                    Memorystate |= 1 << (3 + Key.Param); //* 显示用了那个保存程序
                    Buzzer = SOUND_ACK;
                }
                else
                {
                    Buzzer = SOUND_ALARM;
                }
            }
            break;

        case Key_Sit_coder: // 编码器调整姿势
            if (WorkStateGet() == WorkState_Pause)
                break;

            if (Key.Param == 0)
            {
                Buzzer = SOUND_ACK;
                if (DDGVariable.UserState == DDG_UserState_SitDown_Circle)
                {
                    DDGVariable.BnLocalCircle = 0;
                    DDGVariable.LnLocalCircle = 0;
                }
                DDGVariable.UserState = DDG_UserState_SitUp_Circle;

                //									LegAutoAdj = 1;
                //									LegVoid = 1; // 小腿自动避让

                i = DDGVariable.BnLocalCircle;
#ifdef DDG_Type_NoSignal
                i /= 100;
#endif // DDG_Type_NoSignal
                if (i < DDG_BCircle_Max / 4)
                    i += DDG_BCircle_Max / 10;
#ifdef DDG_Type_NoSignal
                i *= 100;
#endif // DDG_Type_NoSignal
                DDGVariable.BnLocalCircle = i;

                i = DDGVariable.LnLocalCircle;
#ifdef DDG_Type_NoSignal
                i /= 100;
#endif // DDG_Type_NoSignal
                if (i < DDG_LCircle_Max / 4)
                    i += DDG_LCircle_Max / 10;
#ifdef DDG_Type_NoSignal
                i *= 100;
#endif // DDG_Type_NoSignal
                DDGVariable.LnLocalCircle = i;

                nCurZeroState = 0x80;

                if ((DDG_Get_BnCurCircle >= DDG_BCircle_Max && DDG_Get_LnCurCircle >= DDG_LCircle_Max) || (DDG_Get_BnCurCircle == 0 && DDG_Get_LnCurCircle == 0))
                {

                    LegAutoAdj = 0;
                    LegVoid = 0; // 小腿自动避让
                }
                else
                {

                    LegAutoAdj = 1;
                    LegVoid = 1; // 小腿自动避让
                }
            }
            else
            {
                Buzzer = SOUND_ACK;
                if (DDGVariable.UserState == DDG_UserState_SitUp_Circle)
                {
                    DDGVariable.BnLocalCircle = 0;
                    DDGVariable.LnLocalCircle = 0;
                }
                DDGVariable.UserState = DDG_UserState_SitDown_Circle;
                //								LegAutoAdj = 1;
                //								LegVoid = 0; // 小腿自动避让

                i = DDGVariable.BnLocalCircle;
#ifdef DDG_Type_NoSignal
                i /= 100;
#endif // DDG_Type_NoSignal
                if (i < DDG_BCircle_Max / 4)
                    i += DDG_BCircle_Max / 10;
#ifdef DDG_Type_NoSignal
                i *= 100;
#endif // DDG_Type_NoSignal
                DDGVariable.BnLocalCircle = i;

                i = DDGVariable.LnLocalCircle;
#ifdef DDG_Type_NoSignal
                i /= 100;
#endif // DDG_Type_NoSignal
                if (i < DDG_LCircle_Max / 4)
                    i += DDG_LCircle_Max / 10;
#ifdef DDG_Type_NoSignal
                i *= 100;
#endif // DDG_Type_NoSignal
                DDGVariable.LnLocalCircle = i;

                nCurZeroState = 0x80;

                if ((DDG_Get_BnCurCircle >= DDG_BCircle_Max && DDG_Get_LnCurCircle >= DDG_LCircle_Max) || (DDG_Get_BnCurCircle == 0 && DDG_Get_LnCurCircle == 0))
                {

                    LegAutoAdj = 0;
                    LegVoid = 0; // 小腿自动避让
                }
                else
                {

                    LegAutoAdj = 1;
                    LegVoid = 0; // 小腿自动避让
                }
            }
            break;

        case Key_Music:
            if (nMusicSW)
            {
                nMusicSW = 0;
#ifdef USE_SOUNDWAVE
                Change_Sound(); // @ add 260325
#endif
                Flash_Parameter_Write();
            }
            else
            {
                nMusicSW = 1;
#ifdef USE_SOUNDWAVE
                Change_Sound(); // @ add 260325
#endif
                Flash_Parameter_Write();
                BlueState_Play_Deal();
            }
            Buzzer = SOUND_ACK;
            break;

        case key_Light:
            if (nPowerOn)
            {
                nLigntSw = ~nLigntSw;
                Flash_Parameter_Write();
                // Flash_Check_Tick = Flash_Check_Time;
                if (nLigntSw)
                {
                    Light_Params_Set(4, 255, 255, 255, 2);
                    LED1_ON();
#ifdef USE_REDLIGHT
                    RedLight_PWM(RedLight_Duty); // todo test 261015
#else
                    Heat3_On(RedLight_Duty);
#endif
#ifdef LED2_PWM
                    LED2_PWM_Flag = 1;
#else
                    LED2_ON();
#endif
                }
                else
                {
                    Light_Params_Set(0, 0, 0, 0, 0);
                    LED1_OFF();
#ifdef USE_REDLIGHT
                    RedLight_Off(); // todo test 261015
#else
                    Heat3_Off(); // todo test 261015
#endif
#ifdef LED2_PWM
                    LED2_PWM_Flag = 0;
#else
                    LED2_OFF();
#endif
                }
                Buzzer = SOUND_ACK;
            }
            break;

        default:
            break;
        }
        switch (Key.Value)
        {

        case Key_LogoCompleted:
            // return; //* 屏蔽测试 250123
            break;

        default:

            break;
        }
        // 关闭机械手手动调整
        if (KeyPush.TreadKeyState != 0)
        {
            KeyPush.TreadKeyState = 0;
        }
#ifdef USE_M2
        if (KeyPush.M2TreadKeyState != 0)
        {
            KeyPush.M2TreadKeyState = 0;
        }
#endif
        // 关闭机电动杆手动调整
        if (KeyPush.FlexKeyState != 0 || KeyPush.LegFlexKeyState != 0 || KeyPush.BackFlexKeyState != 0)
        {
            if (Key_Lying_Up == 0 && Key_Lying_Dw == 0) // * 扶手旋钮修改 250924
            {
                KeyPush.FlexKeyState = 0;
                KeyPush.LegFlexKeyState = 0;
                KeyPush.BackFlexKeyState = 0;
                DDG_Set_Stop();
            }
        }

        if (KeyPush.FootFlexKeyState != 0)
        {
            KeyPush.FootFlexKeyState = FlexStop;
            FootFlexStateSet(FlexStop);
        }
    }
}

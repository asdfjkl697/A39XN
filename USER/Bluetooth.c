#include "Bluetooth.h"
#include "fmc.h"     //* 增加
#include "FUJIApp.h" //@ add 260303
#include "FUJIOTA.h" //@ OTA Send_Pro

u32 UserCode_data = 0;
#ifdef USE_SOUNDWAVE
bool SoundWave_M1_Paradox_Flag = 0;
u8 nVolume_SoundWave; // 音量
u8 nVolume_SoundWave_Pre;
u8 nVolume_SoundWave_Real;
// @ 最高 122 锚定 260326
// #define nVolume_SoundWave_Cal 92
// todo 最高 110 0db 锚定 系数改变 260605 与薛工最后确定
#define nVolume_SoundWave_Cal 95
// todo 最高 95 锚定 系数改变 260511
// #define nVolume_SoundWave_Cal 88
u8 nVolume_Real;       // @ add 260325
#define nVolume_Cal 70 // @ 70-80 260520
#endif
u8 nVolume; // 音量
u8 Blue_At_Re;
u8 Blue_Over_Time;  // 单位100ms
char Blue_Name[60]; // 蓝牙名称

u8 VoicePlayID; // 播报ID
u8 insertSta = 0;

u8 nWake_time;
u8 nPlay_time;
u8 Voice_DisTime;

u8 nPlay_State;
u8 nPlay_Device;
u8 nPlay_Mode;
u8 nPlay_ID;
u8 nPlay_nVolume;
#ifdef USE_SOUNDWAVE
static u8 system_nVolume = 0;    // @ add 260324
static u8 Wave_Play_nVolume = 0; // 蓝牙板返回的音波振子音量值
#endif
u8 nBlue_Con;

U8 BlueOk; // 蓝牙版就绪标志
static u8 modeflag = 0;
static u8 blue_Update_bit, blue_a3_ok;

static u8 bPause;  // 从播放到暂停的标志
u8 BlueState_Play; // 播放状态 0-停止 1-播放 2-暂停 3- 运行
u8 Blue_Play_ID1;  // 初始ID
u8 Blue_Play_ID2;
Play_Mode_enum User_Play_Mode; // 循环模式

// u8 BT_STATE_READ; // 开始读取蓝牙状态标志
// u8 Blue_Ascii_Cnt;
// u8 Blue_Connect_State;
// UnionBit BT_STATE; //* 测试
// #define BT_OK BT_STATE.StrackBit.BIT0
// #define BT_LE BT_STATE.StrackBit.BIT1
// #define BT_BREDR BT_STATE.StrackBit.BIT2
// #define BT_RECON BT_STATE.StrackBit.BIT3
// #define BT_TONE BT_STATE.StrackBit.BIT4
// #define BT_NAME BT_STATE.StrackBit.BIT5
// #define BT_LE_SET 0	   // 0-关闭数据蓝牙 1- 打开数据蓝牙
// #define BT_RECON_SET 0 // 0-关闭自动重连 1- 打开自动重连
// #define BT_TONE_SET 1  // 0-关闭提示音 1- 打开提示音

extern u8 tx2_checksum;
#if (USE_CSD_MSUIC)
static u8 CSD_BLUE_STATE;
#endif // USE_CSD_MSUIC

/***************
 * 打印32位无符号整型到字符串
 *  *Str目标字符串
 *  nValue需要打印的数据
 *  nlen打印的长度1~10,
 * ******************/
static void User_PrintString5_U32(char *Str, u32 nValue, u8 nlen)
{
    if (nlen == 0 || nlen > 10)
        return;

    Str[nlen] = '\0'; // 增加结束符

    while (nlen > 0)
    {
        nlen--;
        Str[nlen] = nValue % 10 + '0';
        nValue /= 10;
    }
}

static void User_Copy_Str(char *Str1, char *Str2)
{
    while (*Str2 != '\0')
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }

    *Str1 = '\0';
}

static void User_Splicing_Str(char *Str1, char *Str2)
{
    while (*Str1 != '\0')
        Str1++;

    while (*Str2 != '\0')
    {
        *Str1 = *Str2;
        Str1++;
        Str2++;
    }

    *Str1 = '\0';
}

static u8 User_found_strs(char *str, char *_str, char _str1)
{
    u8 i;

    while (*str != '\0' && *str != _str1)
    {
        i = 0;

        while (_str[i] != '\0' && _str[i] != _str1)
        {
            if (_str[i] != str[i])
                break;

            i++;

            if (_str[i] == '\0' || _str[i] == _str1)
                return 1;

            if (str[i] == '\0' || str[i] == _str1)
                return 0;
        }

        str++;
    }

    return 0;
}

static void BlueBoard_Read_Data(void)
{
    EUSART2_Write(0xa3);
    EUSART2_Write(0x00);
    EUSART2_Write(0x00);
    EUSART2_Write(0x5d);
}

#ifdef USE_SOUNDWAVE
static void BlueBoard_Read_Data_ID(u8 Read_ID)
{
    tx2_checksum = 0;
    EUSART2_Write(0xa3);
    EUSART2_Write(Read_ID);
    EUSART2_Write(0x00);
    tx2_checksum = (u8)0 - tx2_checksum;
    EUSART2_Write(tx2_checksum);
}

// 获取音波振子音量值指令
static void BlueBoard_Read_WaveData(void)
{
    EUSART2_Write(0xa3);
    EUSART2_Write(0x0B);
    EUSART2_Write(0x00);
    EUSART2_Write(0x52);
}
#endif

static void BlueBoard_Send_Data1(u8 nCom, u8 nData)
{
    tx2_checksum = 0;
    EUSART2_Write(0xA2);
    EUSART2_Write(nCom);
    EUSART2_Write(0x01);
    EUSART2_Write(nData);
    tx2_checksum = (u8)0 - tx2_checksum;
    EUSART2_Write(tx2_checksum);
}

static void BlueBoard_Send_Data2(u8 nCom, u8 nData)
{
    tx2_checksum = 0;
    EUSART2_Write(0xA2);
    EUSART2_Write(nCom);
    EUSART2_Write(0x02);
    EUSART2_Write(0);
    EUSART2_Write(nData);
    tx2_checksum = 0 - tx2_checksum;
    EUSART2_Write(tx2_checksum);
}

#ifdef USE_SOUNDWAVE
static void BlueBoard_Send_Data3(u8 nCom, u8 nData)
{
    u8 temp1 = 0, temp2 = 0;
    tx2_checksum = 0;
    EUSART2_Write(0xA2);
    EUSART2_Write(nCom);
#if 1
    EUSART2_Write(0x09); // 固定13位
    EUSART2_Write(0x2F); //
    EUSART2_Write(0x41); // * A
#else
    EUSART2_Write(0x0D); // 固定13位
    EUSART2_Write(0x2F); //
    EUSART2_Write(0x41); // * A
    EUSART2_Write(0x33); // * 3
    EUSART2_Write(0x39); // * 9
    EUSART2_Write(0x35); // * 5
    EUSART2_Write(0x53); // * S
#endif
    EUSART2_Write(0x2A); // *
    EUSART2_Write(0x2F); // "/"
    temp1 = nData / 10;
    temp2 = nData % 10;
    EUSART2_Write(0x30 + temp1); // 0
    EUSART2_Write(0x30 + temp2); // 1
    EUSART2_Write(0x2A);         // *
    EUSART2_Write(0x2E);
    EUSART2_Write(0x2A);
    tx2_checksum = ~tx2_checksum + 1;
    EUSART2_Write(tx2_checksum);
}
#endif

static void Blue_Value_Re(u8 nData) // 接收数据
{
    static u8 nBuf[32];
    static u8 nRxCnt = 0, nChecksum = 0;
    static u16 nRxLen;

    if (nRxCnt == 0)
    {
        if (nData == 0xa3) // 接收到头码
        {
            nBuf[nRxCnt++] = nData;
            nChecksum = nData;
            nRxLen = 0;
        }
    }
    else if (nRxCnt <= 2)
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;
#ifdef USE_SOUNDWAVE
        nRxLen = nData; // @ 修改 260325
#else
        nRxLen <<= 8;
        nRxLen |= nData;
#endif
        if (nRxLen > 20) // 超出长度
            nRxCnt = 0;
    }
    else
    {
        nBuf[nRxCnt++] = nData;
        nChecksum += nData;

        if (nRxLen)
            nRxLen--;
        else // 接收完成
        {
            nRxCnt = 0;

            if (nChecksum == 0) // 校验正确
            {
#ifdef USE_SOUNDWAVE
                switch (nBuf[1]) // 判断ID号
                {
                case 0: // ID为0，表示返回的是常规数据
                {
                    nPlay_State = nBuf[3] & 0x03;
                    nPlay_Device = (nBuf[3] >> 2) & 0x03;
                    nPlay_Mode = nBuf[5] & 0x07;
                    insertSta = (nBuf[6] >> 7) & 0x01; // todo add 260630
                    nPlay_ID = nBuf[8];
                    // if (nPlay_ID != 45) // todo rm 260518
                    //     blue_a3_ok = 1;
                    system_nVolume = nBuf[4] & 0x0f;
                    nBlue_Con = nBuf[15] & 0x30; // 蓝牙状态
                    blue_a3_ok = 1;              // @ resume 260325
                }
                break;
                case 0x0b: // ID为0x0b，表示返回的是音波振子的音量值
                {
                    nPlay_nVolume = nBuf[7];
                    Wave_Play_nVolume = nBuf[9];
                }
                break;
                default:

                    break;
                }
#else
                nPlay_State = nBuf[3] & 0x03;
                nPlay_Device = (nBuf[3] >> 2) & 0x03;
                nPlay_Mode = nBuf[5] & 0x07;
                insertSta = nBuf[6];
                nPlay_ID = nBuf[8];
                nPlay_nVolume = nBuf[4] & 0x0f;
                nBlue_Con = nBuf[15] & 0x30; // 蓝牙状态
                blue_a3_ok = 1;
#endif
            }
        }
    }
}

static void Blue_Ascii_Re(u8 nData) // 接收字符串
{
    static u8 nBuf[64];
    static u8 nRxCnt = 0;
    nBuf[nRxCnt++] = nData;

    if (nData == '\n')
    {
        nRxCnt = 0;

        if (User_found_strs((char *)nBuf, "ON!", '\n') == 1)
        {
            if (BlueOk)
                Pt_Reset_ID(TASK_ID_BLUE); // 蓝牙任务【1】复位
        }
        else if (User_found_strs((char *)nBuf, "BT_OK", '\n') == 1)
            Blue_At_Re |= 0x01;
#ifdef USE_SOUNDWAVE
        else if (User_found_strs((char *)nBuf, "BREDR=DIS", '\n') == 1)
#else
        else if (User_found_strs((char *)nBuf, "BREDR_DIS", '\n') == 1)
#endif
        {
            Blue_At_Re |= 0x02;
#ifdef USE_SOUNDWAVE
            Blue_At_Re &= 0xef; // todo test 260825
#endif
        }
        else if (User_found_strs((char *)nBuf, "NA:", '\n') == 1)
        {
            if (User_found_strs((char *)nBuf, Blue_Name, '\n') == 0)
            {
                Blue_At_Re |= 0x08;
            }
        }
        else if (User_found_strs((char *)nBuf, "BREDR=EN", '\n') == 1)
        {
            Blue_At_Re |= 0x10;
#ifdef USE_SOUNDWAVE
            Blue_At_Re &= 0xfd; // todo test 260825
#endif
        }
        else if (User_found_strs((char *)nBuf, "LE=EN", '\n') == 1)
            Blue_At_Re |= 0x20;
        else if (User_found_strs((char *)nBuf, "RECON=EN", '\n') == 1)
            Blue_At_Re |= 0x40;
        else if (User_found_strs((char *)nBuf, "TONE=EN", '\n') == 1)
            Blue_At_Re |= 0x80;
        else if (User_found_strs((char *)nBuf, "LE_CONNECTED", '\n') == 1)
            Blue_At_Re |= 0x04;
    }

    if (nData == 0 || nData > 127 || nRxCnt >= 64)
    {
        nRxCnt = 0;
    }
}

void os_task_BlueRx(void) // 蓝牙串口处理
{
    u8 nData;

    while (COM2.RX_read != COM2.RX_write)
    {
        nData = EUSART2_Read();
        Blue_Value_Re(nData);
        Blue_Ascii_Re(nData);
#if (USE_CSD_MSUIC)
        App_Value_Re(nData);
#endif

#if (USE_BLUE_APP == 1)
        AppCom(nData);
#endif
    }
#if (USE_BLUE_OTA == 1)
    if (WorkStateGet() == WorkState_Pause && nVolume == 1)
    {
        FUJIApp_Recieve(); // 帧解析 (IDLE触发后while可能为空, 此位置确保执行)
    }
    FUJIOTA_Send_Pro(); // OTA响应快速发送: ~3ms vs 原50ms
#endif
}

#ifdef USE_SOUNDWAVE
bool BREDR_Iint_Flag = 0;
u32 BREDR_Iint_Tick = 0;
void BREDR_Iint_Delay(void) // todo 延时修改蓝牙开关 260825
{
    if (TICK_COUNTER(BREDR_Iint_Tick, 3000) && BREDR_Iint_Flag == 0)
    {
        BREDR_Iint_Flag = 1;
        if (nBlueState) // * 测试关闭蓝牙 背景音乐停止时间过长 250412
        {
            Blue_OnOff(1);
        }
        else
        {
            Blue_OnOff(0);
        }
    }
}
#endif
void os_task_blue(u8 nTask_ID)
{
    // KEIL一般分配临时变量在RAM不在堆栈
    // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
    Pt_Start
        BlueOk = 0;
    Pt_Wait_Sig(VoicePlayID > 0);
    Pt_Pause_ID(TASK_ID_VOICE); // 暂停语音播报任务ID号 1

    // 等待启动，成功去到下一步，失败重启
    //  Blue_Over_Time = 0;
    while ((Blue_At_Re & 0x03) == 0)
    {
        Pt_Wait_Tick(3);
        if (++Blue_Over_Time > 250)
        {
            Blue_Over_Time = 0;
            Blue_At_Re = 0;
            PrintString2("AT+RESET\r\n"); // 重启
            Pt_Reset();
        }
    }

    // 查询蓝牙名称和参数(经典蓝牙、数据蓝牙、自动重连、提示音)
    Blue_At_Re = 0;
    PrintString2("AT+NM\r\n");
    Pt_Wait_Tick(10);
    PrintString2("AT+FUN\r\n");
    Pt_Wait_Tick(250);
    blue_Update_bit = 0;
    // 检验蓝牙名称和参数是否正确，不正确则写入

    if (Blue_At_Re & 0x04)
    {
        Timer_1s_Cnt = 0;
        modeflag = 1;
    }

    if (modeflag && Timer_1s_Cnt >= 1)
    {
        modeflag = 0;
    }

    if (Blue_At_Re & 0x08) // 蓝牙名称
    {
        PrintString2("AT+NM=");
        PrintString2(Blue_Name);
        PrintString2("\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    // * 设置密码 250608
#ifdef BLUE_KEY_8888
    PrintString2("AT+MP=8888\r\n");
    Pt_Wait_Tick(10);
    PrintString2("AT+FUN=PIN=EN\r\n");
    Pt_Wait_Tick(50);
#elif defined BLUE_KEY_8392
    PrintString2("AT+MP=8392\r\n");
    Pt_Wait_Tick(10);
    PrintString2("AT+FUN=PIN=EN\r\n");
    Pt_Wait_Tick(50);
#else
    PrintString2("AT+FUN=PIN=DIS\r\n");
    Pt_Wait_Tick(50);
#endif

#ifdef USE_SOUNDWAVE
    if (Blue_At_Re & 0x02 && BREDR_Iint_Flag == 0) // todo 对比测试 260825
    {
        PrintString2("AT+FUN=BREDR=EN\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
        TICK_RESET(BREDR_Iint_Tick);
        // BREDR_Iint_Flag = 1; // todo 初始化完成
    }
#else
    if (Blue_At_Re & 0x10) // 经典蓝牙
    {
        if (nBlueState == 0)
        {
            PrintString2("AT+FUN=BREDR=DIS\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }
    else
    {
        if (nBlueState)
        {
            PrintString2("AT+FUN=BREDR=EN\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }
#endif

    if (Blue_At_Re & 0x20) // 数据蓝牙
    {
        // todo rm for app 不要关闭数据蓝牙 260627
        // if (nBlueState == 0)
        // {
        //     PrintString2("AT+FUN=LE=DIS\r\n");
        //     Pt_Wait_Tick(50);
        //     blue_Update_bit = 1;
        // }
    }
    else
    {
        if (nBlueState)
        {
            PrintString2("AT+FUN=LE=EN\r\n");
            Pt_Wait_Tick(50);
            blue_Update_bit = 1;
        }
    }

    if (Blue_At_Re & 0x40) // 自动连接
    {
    }
    else
    {
        PrintString2("AT+FUN=RECON=EN\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    if (Blue_At_Re & 0x80) // 提示音
    {
    }
    else
    {
        PrintString2("AT+FUN=TONE=EN\r\n");
        Pt_Wait_Tick(50);
        blue_Update_bit = 1;
    }

    if (nBlueState)
        BlueBoard_Send_Data1(0x13, 0x04); // 蓝牙
    else
        BlueBoard_Send_Data1(0x13, 0x01); // SD

    Pt_Wait_Tick(50);

    if (blue_Update_bit) // 蓝牙设置有更新就重启
    {
        // 重启
        Blue_At_Re = 0;
        PrintString2("AT+RESET\r\n");
        Blue_Over_Time = 0;
        Pt_Reset();
    }

//    blue_Update_bit = 1;
#ifdef USE_SOUNDWAVE
    if (nVolume > 0)
        BlueBoard_Send_Data1(0x16, nVolume * 2 + nVolume_Cal);
    else
        BlueBoard_Send_Data1(0x16, 0);
#else
    BlueBoard_Send_Data1(0x10, nVolume); // 音量
#endif
    Pt_Wait_Tick(50);
    Pt_Reset_ID(TASK_ID_VOICE);
    BlueOk = 1;

    Pt_Pause();

    Pt_End
}

void os_task_voice(u8 nTask_ID)
{
    // KEIL一般分配临时变量在RAM不在堆栈
    // 因此为了防止任务之间改写凡是作用域跨越os_delay()应作为static
    Pt_Start

        if (VoicePlayID)
    {
        nPlay_time = nPlay_time_Set;
        Voice_DisTime = nPlay_time_Set;
#ifdef USE_SOUNDWAVE
        // BlueBoard_Send_Data1(0x03, 0x00); //@ rm 260330
        // Pt_Wait_Tick(10);
        Pt_Wait_Tick(20);

        // if (!nSoundWaveSW)
        // {
        //     BlueBoard_Send_Data1(0x17, 0); // 设置音波通道1的音量值
        //     Pt_Wait_Tick(10);
        // }

        BlueBoard_Send_Data3(0x24, VoicePlayID); // 插播指令
        // BlueBoard_Send_Data1(0x16, nVolume * 2 + nVolume_Cal + 15); // @ add 260330
        BlueBoard_Send_Data1(0x16, 100); // todo md 260330
        Pt_Wait_Tick(100);
        // Pt_Wait_Sig(nPlay_ID == VoicePlayID && nPlay_State == Play_State_Run && nPlay_time > 0); // 等待操作完成
        Pt_Wait_Sig(insertSta == 1 && nPlay_time > 0); // todo add 260630
#else
        BlueBoard_Send_Data2(0x23, VoicePlayID);        // 插播指令
        BlueBoard_Send_Data1(0x10, nVolume);            // 音量
        Pt_Wait_Tick(50);                               // 250ms
        blue_a3_ok = 0;                                 // 先清掉轮询残留的应答标志
        Pt_Wait_Sig(blue_a3_ok == 0 && nPlay_time > 0); // 等待操作完成
#endif

        // if (Voice_DisTime) // todo rm 260630
        //     Voice_DisTime = 4;

        if (nPlay_time != 0)
            nPlay_time = 0;
        else
        {
            // 语音未应答时不重启模块：AT+RESET 会掉 BLE，正是「app 按键无反应」的根因。
            // 只放弃本次播报，保持 BLE 连接；模块语音异常留待下次上电自恢复。
            // todo AI rm for app 261005
#if (USE_BLUE_APP == 0)
            Blue_At_Re = 0;
            Blue_Over_Time = 251;
            Pt_Reset_ID(TASK_ID_BLUE); // 重新配置蓝牙
#endif
        }

        VoicePlayID = 0;
#ifdef USE_SOUNDWAVE
        if (WorkStateGet() == WorkState_Reset || WorkStateGet() == WorkState_Wait)
            BlueBoard_Send_Data1(0x16, 0);
        else if ((nMusicSW || nBluePlay) && nSoundMusicSW)
            BlueBoard_Send_Data1(0x16, nVolume * 2 + nVolume_Cal);
        else
            BlueBoard_Send_Data1(0x16, 0);

        Pt_Wait_Sig(nWake_time > 0); // 等待唤醒时间到

        Pt_Wait_Tick(20);
#else
        BlueBoard_Send_Data1(0x10, 0); // 音量设置为0
        Pt_Wait_Sig(nWake_time > 0);   // 等待唤醒时间到
        Pt_Wait_Tick(20);
#endif
    }

    while (1)
    {
        if (nBlueState > 0 && nBlue_Con == 0x30) // 蓝牙开启并且在播放中（蓝牙播放优先）
        {
            if (nPlay_Device != 0x00) // 切换蓝牙
            {
                if (nPlay_State == Play_State_Run) // 记录SD播放状态
                    bPause = 1;
#ifdef USE_SOUNDWAVE
                nBluePlay = 1;
                nVolume_Real = nVolume; // TODO add 260822
#endif
                BlueBoard_Send_Data1(0x13, 0x04);
                Pt_Wait_Tick(10);
            }
        }
        else
        {
            if (nPlay_Device != 0x02) // 切换SD卡
            {
                BlueBoard_Send_Data1(0x13, 0x01);
#ifdef USE_SOUNDWAVE
                nBluePlay = 0;
                Pt_Wait_Tick(200); // @ 测试 260325
#else
                Pt_Wait_Tick(100);
#endif
            }

#ifdef USE_SOUNDWAVE
            if (BlueState_Play > 0 && nMusicSW && (nSoundMusicSW || nSoundWaveSW)) // MP3模式开启
#else
            if (BlueState_Play > 0 && nMusicSW > 0) // MP3模式开启
#endif
            {
                if (WorkStateGet() == WorkState_Pause) // 暂停状态保持暂停
                {
                    if (nPlay_State == Play_State_Run) // 暂停
                    {
                        bPause = 1; // 暂停
                        BlueBoard_Send_Data1(0x03, 0x00);
                        Pt_Wait_Tick(10);
                    }
                }
                else
                {
                    // if (Blue_Play_ID1 != VC_Music1 && Blue_Play_ID1 != VC_Music2 && Blue_Play_ID1 != VC_Music3) // @ 屏蔽其他音乐播放
#ifndef USE_SOUNDWAVE
                    if (Blue_Play_ID1 < VC_Music1 || Blue_Play_ID1 > VC_Music3)
#else
                    if (Blue_Play_ID1 < VC_Music1 || Blue_Play_ID1 > VC_YBMusic4)
#endif
                    {
                        BlueState_Play = 0;
                        Blue_Play_ID1 = 0;
                        Blue_Play_ID2 = 0;
                    }

                    if (nPlay_Mode != User_Play_Mode ||
                        nPlay_ID < Blue_Play_ID1 ||
                        nPlay_ID > Blue_Play_ID2 ||
                        BlueState_Play == 1) // 满足条件从头播放
                    {
                        BlueState_Play = 2; // 播放中
                        bPause = 0;
                        BlueBoard_Send_Data1(0x12, Play_Mode_One);
                        Pt_Wait_Tick(10);
                        BlueBoard_Send_Data2(0x20, Blue_Play_ID1); // 先单曲播放目标音乐
                        Pt_Wait_Tick(50);

                        if (User_Play_Mode != nPlay_Mode)
                        {
                            BlueBoard_Send_Data1(0x12, User_Play_Mode); // 再指定播放模式
                            Pt_Wait_Tick(20);
                        }
                    }

                    if (nPlay_Mode == Play_Mode_One || nPlay_Mode == Play_Mode_All) // 播放一次的模式
                    {
                        if (bPause) // 有播放到暂停标志的才重新播放
                        {
                            bPause = 0;
                            BlueBoard_Send_Data2(0x02, 0x00);
                            Pt_Wait_Tick(10);
                        }
                    }
                    else // 循环模式直接播放
                    {
                        bPause = 0;

                        if (nPlay_State != Play_State_Run) // 直接播放
                        {
                            BlueBoard_Send_Data2(0x02, 0x00);
                            Pt_Wait_Tick(10);
                        }
                    }
                }
            }
            else
            {
#ifdef USE_SOUNDWAVE
                if (nMusicSW == 0 || (nSoundWaveSW == 0 && nSoundMusicSW == 0)) // @ md 260324
#else
                if (nMusicSW == 0) // * 复位 250310
#endif
                {
                    BlueState_Play = 0;
                    Blue_Play_ID1 = 0;
                    Blue_Play_ID2 = 0;
                }

                if (BlueState_Play == 2)
                    BlueState_Play = 1;

                if (nPlay_State == Play_State_Run) // 暂停
                {
                    BlueBoard_Send_Data1(0x03, 0x00);
                    Pt_Wait_Tick(10);
                }
            }
        }

#ifdef USE_SOUNDWAVE
        if (system_nVolume != 15) // @ 系统音量 260326
        {
            BlueBoard_Send_Data1(0x10, 15);
            Pt_Wait_Tick(50);
        }
        if ((nPlay_nVolume != (nVolume_Real * 2 + nVolume_Cal))) // 音量核对
        {
            if ((nMusicSW || nBluePlay) && nSoundMusicSW) // todo md 260821
            {
                if (nVolume_Real == 0)
                    BlueBoard_Send_Data1(0x16, 0);
                else
                    BlueBoard_Send_Data1(0x16, nVolume_Real * 2 + nVolume_Cal);
            }
            else
                BlueBoard_Send_Data1(0x16, 0);
            Pt_Wait_Tick(10);
        }

        if (WorkStateGet() == WorkState_Work && nSoundWaveSW) // @ 处理音波恢复
        {
            if (nVolume_SoundWave_Real == 0)
            {
                if (nVolume_SoundWave > 3 || !SoundWave_M1_Paradox_Flag)
                    nVolume_SoundWave_Real = nVolume_SoundWave;
            }
        }
        else if (nVolume_SoundWave_Real > 0) // @ 非工作状态 音波不开启
        {
            nVolume_SoundWave_Real = 0;
        }

        if ((Wave_Play_nVolume != (nVolume_SoundWave_Real / 3 + nVolume_SoundWave_Cal))) // 音量核对
        {
            if ((nMusicSW || nBluePlay) && nSoundWaveSW) // todo md 260821
            {
                if (nVolume_SoundWave_Real == 0)
                    BlueBoard_Send_Data1(0x17, 0);
                else
                    BlueBoard_Send_Data1(0x17, nVolume_SoundWave_Real / 3 + nVolume_SoundWave_Cal); // todo md 260325
            }
            else
                BlueBoard_Send_Data1(0x17, 0);
            // nVolume_SoundWave_Pre = nVolume_SoundWave_Real;
            Pt_Wait_Tick(10);
        }
        Pt_Wait_Tick(50);
        BlueBoard_Read_Data();
        // BlueBoard_Read_Data_ID(1);

        Pt_Wait_Tick(50);
        BlueBoard_Read_WaveData(); // @ 获取音波振子的音量值指令 260324
#else
        if (nPlay_nVolume != nVolume) // 音量核对
        {
            BlueBoard_Send_Data1(0x10, nVolume);
            Pt_Wait_Tick(10);
        }
        Pt_Wait_Tick(100);
        BlueBoard_Read_Data();
#endif

        Pt_Wait_Tick(100);
    }

    Pt_End
}

void BluePlay_time(void)
{
    if (nPlay_time)
    {
        nPlay_time--;
    }
#ifdef USE_SOUNDWAVE
    BREDR_Iint_Delay(); // todo 增加延时补丁 260825
#endif
}

void Blue_Init(void)
{
    char Str[11];
    // 变量初始化
    UserCode_data = 0;
    Blue_At_Re = 0;
    Blue_Over_Time = 0; // 单位100ms
    VoicePlayID = 0;    // 播报ID
    nWake_time = 0;
    nPlay_time = 0;
    nPlay_State = 0;
    nPlay_Device = 0;
    nPlay_Mode = 0;
    nPlay_ID = 0;
    nPlay_nVolume = 0;
    nBlue_Con = 0;
    BlueOk = 0;
    // nVolume = 6; //* 屏蔽
    bPause = 0;         // 从播放到暂停的标志
    BlueState_Play = 0; // 播放状态
    Blue_Play_ID1 = 0;  // 初始ID
    Blue_Play_ID2 = 0;
    // User_Play_Mode = Play_Mode_FolderLoop; // 循环模式
    User_Play_Mode = Play_Mode_OneLoop; //* 单曲循环

#if (USE_CSD_MSUIC)
    CSD_BLUE_STATE = 0x03;
#endif
    // 序列号和蓝牙名称获取
    // UserCode_data = *(uint32_t *)0x8034000; // 添加编号递增数
    // uint16_t pbuf[2];
    // GDFLASH_Read(0x8030000, pbuf, 2);
    // UserCode_data = pbuf[0];
    // UserCode_data <<= 16;
    // UserCode_data |= pbuf[1];
    UserCode_data = *(uint32_t *)FLASH_SN_ADDR; // 添加编号递增数
    if (UserCode_data == 0xffffffff)            // 如果没有初始则默认
        UserCode_data = 2604255125;
#ifdef A391
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
#elif defined A390_6
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
#elif defined A391_8
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
#elif defined A391S_35
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
#elif defined A391S_33
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
#elif defined A395SY
    UserCode_data = UserCode_data % 10000;
    User_PrintString5_U32(Str, UserCode_data, 4);
    // User_PrintString5_U32(Str, UserCode_data % 10000, 4);
#else
    User_PrintString5_U32(Str, UserCode_data, 10); // 打印递增后的编号到字符串
#endif

#ifdef A390
    User_Copy_Str(Blue_Name, "IMCM-"); // 拷贝蓝牙名称前缀
#elif defined A390_6
    User_Copy_Str(Blue_Name, "Robotouch-"); // 拷贝蓝牙名称前缀
#elif defined A391 || defined A391_8 || defined A391Y
    User_Copy_Str(Blue_Name, "AT1199-"); // 拷贝蓝牙名称前缀
#elif defined A391S_31
    User_Copy_Str(Blue_Name, "Virtus-"); // 拷贝蓝牙名称前缀
#elif defined A391S_35
    User_Copy_Str(Blue_Name, "CHICCEM-"); // 拷贝蓝牙名称前缀
#elif defined A391S_33
    User_Copy_Str(Blue_Name, "020.002299-"); // 拷贝蓝牙名称前缀
#elif defined A390S_8
    User_Copy_Str(Blue_Name, "Poltrona Zenith-"); // 拷贝蓝牙名称前缀
// #elif defined A395SY_2 || defined A395SY
//     User_Copy_Str(Blue_Name, "A395SY-"); // 拷贝蓝牙名称前缀
#elif defined A393SY_2
    User_Copy_Str(Blue_Name, "A393SY-"); // 拷贝蓝牙名称前缀
#elif defined A396SY_2
    User_Copy_Str(Blue_Name, "A396SY-"); // 拷贝蓝牙名称前缀
#elif defined A391S_8
    User_Copy_Str(Blue_Name, "L6SPRO-"); // 拷贝蓝牙名称前缀
#elif defined A396S_1
    User_Copy_Str(Blue_Name, "L6SMAX-"); // 拷贝蓝牙名称前缀
#elif defined A395SY
    User_Copy_Str(Blue_Name, "Mikado-"); // 拷贝蓝牙名称前缀
#elif defined A395SY_6
    User_Copy_Str(Blue_Name, "A89-"); // 拷贝蓝牙名称前缀
#elif defined A396SYN_2
    User_Copy_Str(Blue_Name, "A396SYN-"); // 拷贝蓝牙名称前缀
#else
    User_Copy_Str(Blue_Name, "IMCM-");
#endif
    User_Splicing_Str(Blue_Name, Str); // 拼接完整蓝牙名称
    // PT初始化
    Pt_Reset_ID(TASK_ID_BLUE);  // 蓝牙
    Pt_Pause_ID(TASK_ID_VOICE); // 播报
    Pt_Reset_ID(TASK_ID_APP);   // 蓝牙
}

// * 删除 250222
//  void VoiceControlPlay(u8 nId)
//  {
//      if (MChair.Bluetooth.VoicePlayer)
//      {
//          Voice_Ack(nId);
//      }
//  }

// void Voice_Ack(u8 nId)
// {
//     if (BlueOk == 0)
//         return;

//     Pt_Reset_ID(TASK_ID_VOICE);
//     VoicePlayID = nId;
// }

void VoiceControlPlay(u8 nId)
{
    if (BlueOk == 0 || (nVoicePlayer == 0)) // * 修改 250222
    {
        return;
    }
    Pt_Reset_ID(TASK_ID_VOICE);
    VoicePlayID = nId;
}

void Voice_Ack(u8 nId)
{
    if (BlueOk == 0) // * 语音播报 没有关闭修改
    {
        return;
    }

#ifdef VIETNAM_VOICE
    if ((nVoicePlayer == 0 && nId != VC_BbOff) && nId > VC_OK)
#else
    if ((nVoicePlayer == 0 && nId != VC_BbOff) && nId > VC_SitDown) // todo 修改 261011
#endif
        return;

    Pt_Reset_ID(TASK_ID_VOICE);
    VoicePlayID = nId;
}

void Blue_OnOff(u8 temp) // * 增加 250412
{
    if (temp == 0)
    {
        PrintString2("AT+FUN=BREDR=DIS\r\n");
    }
    else if (temp == 1)
    {
        PrintString2("AT+FUN=BREDR=EN\r\n");
    }
}

void BlueState_Play_Deal(void)
{
    u8 temp = 0;
    u8 temp1 = AutoModeGet(); // * 增加 250326
#ifdef USE_SOUNDWAVE
    switch (temp1)
    {
    case 0:
    case 1:
    case 6 ... 16:
    case 18:
    case 19:
    case 26:
        temp = VC_YBMusic2;
        break;
    case 2:
        temp = VC_YBMusic4;
        break;
    case 3:
        temp = VC_YBMusic3;
        break;
    case 4:
    case 17:
    case 20:
        temp = VC_Music1;
        break;
    case 5:
        temp = VC_YBMusic1;
        break;

    default:
        // nSoundMusicSW = 0;
        // nSoundWaveSW = 0; // todo add test 260707
        BlueState_Play = 0;
        Blue_Play_ID1 = 0;
        Blue_Play_ID2 = 0;
        temp = 0;
        break;
    }

    // if (nMusicSW && nManualPlay) // todo 手动播放 260707
    // {
    //     nManualPlay = 0;
    //     Flash_Parameter_read();
    //     temp = VC_YBMusic2;
    // }

    if ((BlueState_Play == 0 || Blue_Play_ID1 != temp || Blue_Play_ID2 != temp) && temp > 0 && nMusicSW && (nSoundMusicSW || nSoundWaveSW)) // * 增加 nMusicSW>0
    {
        BlueState_Play = 1;
        Blue_Play_ID1 = temp;
        Blue_Play_ID2 = temp;

        if (0 == Blue_Play_ID1)
        {
            BlueState_Play = 0;
        }
    }
#else
    switch (temp1)
    {
    case 1:
    case 26:
        temp = VC_Music1;
        break;
    case 2:
        temp = VC_Music2;
        break;
    case 3:
        temp = VC_Music3;
        break;
#ifdef A390S_6
    case 4:
        temp = VC_Music1;
        break;
    case 5:
        temp = VC_Music2;
        break;
#endif
    default:
        BlueState_Play = 0;
        Blue_Play_ID1 = 0;
        Blue_Play_ID2 = 0;
        temp = 0;
        break;
    }

    if ((BlueState_Play == 0 || Blue_Play_ID1 != temp || Blue_Play_ID2 != temp) && temp > 0 && nMusicSW) // * 增加 nMusicSW>0
    {
        BlueState_Play = 1;
        Blue_Play_ID1 = temp;
        Blue_Play_ID2 = temp;

        if (0 == Blue_Play_ID1)
        {
            BlueState_Play = 0;
        }
    }
#endif
}

#ifdef USE_SOUNDWAVE
void Change_Sound(void) // @ add 251212
{
    if ((nMusicSW == 0 && nBluePlay == 0) || (nSoundMusicSW == 0 && nSoundWaveSW == 0))
    {
        // @ 已经处理，此处待定 260325
        nVolume_Real = 0;
        nVolume_SoundWave_Real = 0;
        return;
    }

    if ((nMusicSW || nBluePlay) && nSoundMusicSW)
    {
        nVolume_Real = nVolume;
        if (nVolume_Real == 0)
            BlueBoard_Send_Data1(0x16, 0);
        else
            BlueBoard_Send_Data1(0x16, nVolume_Real * 2 + nVolume_Cal);
    }
    else
    {
        nVolume_Real = 0;
        BlueBoard_Send_Data1(0x16, 0);
    }

    if ((nMusicSW || nBluePlay) && nSoundWaveSW)
    {
        // nVolume_SoundWave_Real = nVolume_SoundWave;
        if (nVolume_SoundWave_Real == 0)
            BlueBoard_Send_Data1(0x17, 0);
        else
            BlueBoard_Send_Data1(0x17, nVolume_SoundWave_Real / 3 + nVolume_SoundWave_Cal); // todo md 260325
    }
    else
    {
        nVolume_SoundWave_Real = 0;
        BlueBoard_Send_Data1(0x17, 0);
    }
}

void SoundWave_Resume(void) // @ add 260320
{
    if ((nMusicSW || nBluePlay) && nSoundWaveSW)
    {
        nVolume_SoundWave_Real = nVolume_SoundWave;
        if (nVolume_SoundWave_Real == 0)
            BlueBoard_Send_Data1(0x17, 0);
        else
            BlueBoard_Send_Data1(0x17, nVolume_SoundWave_Real / 3 + nVolume_SoundWave_Cal);
    }
    else
    {
        nVolume_SoundWave_Real = 0;
        BlueBoard_Send_Data1(0x17, 0);
    }
}

void SoundWave_Reduce(void) // @ add 260320
{
    if (nVolume_SoundWave > 3)
    {
        nVolume_SoundWave_Real = nVolume_SoundWave - 3;
        BlueBoard_Send_Data1(0x17, nVolume_SoundWave_Real / 3 + nVolume_SoundWave_Cal);
    }
}

void SoundWave_Stop(void) // @ add 260320
{
    nVolume_SoundWave_Real = 0;
    BlueBoard_Send_Data1(0x17, 0);
}
#endif

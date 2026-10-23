/***************************************************************************************************
* @file      S0553_Porting_Template.c
* @brief     【移植骨架】主控侧接入 S0553 显示组件，把每个 TODO 填上即可
* @details   本文件不参与编译。复制到主控工程（建议改名 host_s0553.c），组件侧（Key_S0553/）不用改。
*            快速上手见 S0553_Porting_Guide.md；改页面 / 排错见 S0553_Porting_Advanced.md。
*
*  主控要实现三块（本文件均已给出可填空的参考实现）：
*    - HAL：4 个 UART 真写 + 2 个临界区空写（见下方【步骤 0】）
*    - 定时推送数据点（【步骤 2】）
*    - （可选）指令回调（【步骤 3】）
*
*  组件日志默认全关：无需 printf 重定向，DelayMs / GetTickMs 也不必实现（组件内零调用点）。
* @date      2026-10-22
==================================================================================================*/

#include "S0553_HAL.h"    /* 平台服务（4 UART 真写 + 2 临界区空写） */
#include "S0553_main.h"   /* S0553_Init / S0553_Task / S0553_IsConnected */
#include "S0553_Host.h"   /* 数据点 / 指令 / 状态同步 */


/* ==================== 【步骤 0】HAL 实现（★必做★ 4 个 UART 真写 + 2 个临界区空写） ====================
 * 实现 S0553_HAL.h 声明的平台服务 —— 组件与平台之间唯一的依赖契约。
 * MCU 移植只需把下面每个 <...> 换成你厂商 SDK 的调用，本段不依赖任何具体芯片。
 *
 *   4 个 UART 函数（真写）：
 *     UartInit      —— 开串口时钟 + 引脚复用 + 配置波特率 + 开接收中断
 *     RxAvailable   —— 返回接收环形缓冲内可读字节数（被协议解析轮询调用）
 *     ReadByte      —— 从环形缓冲取 1 字节（组件保证仅在 RxAvailable()>0 时调用）
 *     WriteByte     —— 阻塞发送 1 字节，返回即代表该字节已被平台接收
 *
 *   2 个临界区函数（单任务裸机【空实现】即可；RTOS / 中断里也调组件 API 时才需真正实现）：
 *     Enter/ExitCritical —— 只保护组件内部待发队列，且【必须支持嵌套】
 *
 *   接收参考模型：UART 接收中断只把字节写进环形缓冲（【不】调用任何 S0553_ 函数），
 *   主循环经 RxAvailable/ReadByte 消费。ISR 与主循环共享的接收缓冲同步请自行保证
 *   （单生产者/单消费者索引即可，详见 S0553_HAL.h 的临界区注释）。
 *
 *   【不需要实现】S0553_HAL_DelayMs() / S0553_HAL_GetTickMs()：
 *     - DelayMs  组件内已零调用点（连接由 S0553_Task() 自治）；
 *     - GetTickMs 仅 PC 调试日志用，声明被 S0553_ENABLE_LOG 包裹，MCU 未开日志时连声明都不存在。
 * ==================================================================================================== */

/* ---- 接收环形缓冲（大小取 512，> 组件最大帧 261） ---- */
#define HAL_RX_BUF_SIZE  512u

static volatile uint8_t  s_HalRxBuf[HAL_RX_BUF_SIZE];
static volatile uint16_t s_HalRxRead  = 0;   /* 主循环读索引 */
static volatile uint16_t s_HalRxWrite = 0;   /* 中断写索引 */

/* 接收中断入口：移植者把你平台 UART RX ISR 里收到的每个字节调进来（或改写成批量入队） */
static void Hal_OnRxByte(uint8_t byte)
{
    uint16_t next = (uint16_t)(s_HalRxWrite + 1u);
    if (next >= HAL_RX_BUF_SIZE) { next = 0; }

    if (next != s_HalRxRead)                 /* 未满才写；满了按需丢弃或覆盖 */
    {
        s_HalRxBuf[s_HalRxWrite] = byte;
        s_HalRxWrite = next;
    }
}

uint8_t S0553_HAL_UartInit(void)
{
    /* TODO：换成你的 MCU UART 初始化：
     *   <开串口时钟>；<配置引脚复用>；<波特率 115200/8/N/1>；<开接收中断>；
     * 成功返回 S0553_HAL_OK，失败返回 S0553_HAL_ERROR。 */
    return S0553_HAL_OK;
}

uint16_t S0553_HAL_UartRxAvailable(void)
{
    if (s_HalRxRead == s_HalRxWrite) { return 0u; }
    return (s_HalRxWrite > s_HalRxRead)
           ? (uint16_t)(s_HalRxWrite - s_HalRxRead)
           : (uint16_t)(s_HalRxWrite + HAL_RX_BUF_SIZE - s_HalRxRead);
}

uint8_t S0553_HAL_UartReadByte(void)
{
    const uint8_t byte = s_HalRxBuf[s_HalRxRead];
    uint16_t next = (uint16_t)(s_HalRxRead + 1u);
    if (next >= HAL_RX_BUF_SIZE) { next = 0; }
    s_HalRxRead = next;
    return byte;
}

void S0553_HAL_UartWriteByte(uint8_t byte)
{
    /* TODO：换成你的 MCU 阻塞发送：<等待发送空标志 TXE>；<写数据寄存器>；
     * 或写入平台 TX 队列/FIFO（需保证字节级输出顺序）。 */
    (void)byte;
}

/* ---- 临界区：单任务裸机空实现；RTOS / 中断里也调组件 API 时才需真正实现 ---- */
void S0553_HAL_EnterCritical(void)
{
}

void S0553_HAL_ExitCritical(void)
{
}

// TODO：仅作参考使用，移植完成后删除-----------------------------------------------------------
static uint8_t s_HostTimeMin  = 15;   /**< 按摩剩余时间-分 */
static uint8_t s_HostTimeSec  = 0;    /**< 按摩剩余时间-秒 */
static uint8_t s_HostProgram  = 1;    /**< 当前选中程序：0=手动，1..N=自动程序序号 */
static uint8_t s_HostWise     = 0;    /**< 手动模式下选中的手法序号（PROGRAM==0 时有效） */
static uint8_t s_HostLanguage = 0;    /**< 当前语言（S0553_LANG_CN） */
static uint8_t s_HostZeroGravity = 0; /**< 零重力挡位（0=关闭，1/2/3=三档） */
static uint8_t s_HostWiseLower   = 0; /**< 【下机芯】手法序号（PROGRAM==0 时有效；值域待产品确认，暂复用 S0553WiseId） */
static uint8_t s_WorkState = 0;   /* 主控工作状态：0=关机；5=运行；其他=中间态（组件只显示 Logo） */

/**
 * @brief 手动模式下的手法 ID
 * @details S0553_DP_WISE 只在本枚举内取值：0 = 无手法，1..11 = 11 种手法。
 *          与 Layer/S0553_UI_Config.c 的 s_WiseName 表【逐行对齐】（下标 = 枚举值）。
 *          组件短按手法项时，通过【每页的"游标 → 手法"映射表】把 UI 游标转换为
 *          对应枚举值再下发给主控 —— 主控侧 switch/case 即可用语义名而非裸数字。
 *          增删手法：① 同步改本枚举；② 同步改 s_WiseName 表；③ 改各页映射表。
 */
typedef enum
{
    S0553_WISE_NONE          = 0,  /**< 无手法（手动模式未选具体手法） */
    S0553_WISE_KNEAD         = 1,  /**< 揉捏 */
    S0553_WISE_TAP_1         = 2,  /**< 拍打1 */
    S0553_WISE_TAP_2         = 3,  /**< 拍打2 */
    S0553_WISE_KNEAD_TAP     = 4,  /**< 揉捏拍打 */
    S0553_WISE_SHIATSU_1     = 5,  /**< 指压1 */
    S0553_WISE_SHIATSU_2     = 6,  /**< 指压2 */
    S0553_WISE_SHOULDER_GRAB = 7,  /**< 肩部提抓 */
    S0553_WISE_4D_1          = 8,  /**< 4D I */
    S0553_WISE_4D_2          = 9,  /**< 4D II */
    S0553_WISE_4D_3          = 10, /**< 4D III */
    S0553_WISE_4D_4          = 11, /**< 4D IV */
    S0553_WISE_COUNT              /**< 手法 ID 总数（含"无手法"，作边界勿用） */
} S0553WiseId;

/**
 * @brief UI 语言标识
 * @details 用于动态文本按语言查表。新增语言只需在末尾追加（保持 S0553_LANG_MAX 在最后）。
 */
typedef enum
{
    S0553_LANG_CN      = 0,  /**< 1 简体中文 */
    S0553_LANG_EN,           /**< 2 英语 English */
    S0553_LANG_DE,           /**< 3 德语 Deutsch */
    S0553_LANG_FR,           /**< 4 法语 Français */
    S0553_LANG_MAX           /**< 语言数量上限（当前=4：CN/EN/DE/FR；勿作为语言值使用） */
} S0553_LangId;

/**
 * @brief 零重力挡位
 * @details S0553_DP_ZERO_GRAVITY 只在本枚举内取值：0 = 关闭，1/2/3 = 三档。
 *          与 Layer/S0553_Page_PostureItem2.c 的零重力项对应：1/2/3 档对应其零重力1/2/3项。
 */
typedef enum
{
    S0553_ZERO_G_OFF = 0,  /**< 关闭零重力 */
    S0553_ZERO_G_1,        /**< 零重力 1 档 */
    S0553_ZERO_G_2,        /**< 零重力 2 档 */
    S0553_ZERO_G_3,        /**< 零重力 3 档 */
    S0553_ZERO_G_COUNT     /**< 档位数（含关闭态，共 4），作边界勿用 */
} S0553ZeroGravityLevel;

/**
 * @brief 坐姿动作（S0553_HCMD_POSTURE 命令的 arg 取值，【非数据点】）
 * @details 4 个部件（联动 / 靠背 / 小腿 / 足部）× 2 个方向（升 / 降）= 8 种。
 *          坐姿是【纯动作命令】：用户点按一次 → 下发一次动作，主控执行即可，
 *          不建模为数据点（无需主控回推状态、界面不显示坐姿当前值）。
 *          组件短按坐姿项时把"部件 + 方向"转换为本枚举值，经
 *          S0553_Host_SendCommand(S0553_HCMD_POSTURE, S0553_DP_NONE, <本枚举值>) 下发。
 *          ⚠️ 若产品实际是"按住持续升/降"，可改用 S0553_HCMD_ADJUST（arg=+1/-1），
 *             此时本枚举仅作"部件"的区分，方向由 arg 符号表达。
 */
typedef enum
{
    S0553_POSTURE_LINKAGE_UP   = 0,  /**< 联动-升 */
    S0553_POSTURE_LINKAGE_DOWN,      /**< 联动-降 */
    S0553_POSTURE_BACKREST_UP,       /**< 靠背-升 */
    S0553_POSTURE_BACKREST_DOWN,     /**< 靠背-降 */
    S0553_POSTURE_CALF_UP,           /**< 小腿-升 */
    S0553_POSTURE_CALF_DOWN,         /**< 小腿-降 */
    S0553_POSTURE_FOOT_UP,           /**< 足部-升 */
    S0553_POSTURE_FOOT_DOWN,         /**< 足部-降 */
    S0553_POSTURE_COUNT              /**< 坐姿动作总数（8），作边界勿用 */
} S0553PostureId;

/**
 * @brief 组件下发给主控的操作意图
 * @details 组件只表达"用户想做什么"，不自行改值；由主控裁决（含范围钳制）后回推状态。
 */
typedef enum
{
    S0553_HCMD_NONE = 0,
    S0553_HCMD_ADJUST,          /**< 相对调整：arg = +1 / -1（新值由主控裁决） */
    S0553_HCMD_SET,             /**< 直接设置：arg = 目标值（数据点命令） */
    S0553_HCMD_POSTURE          /**< 坐姿动作命令（非数据点）：dp 传 S0553_DP_NONE，arg = S0553PostureId */
} S0553HostCommand;
typedef enum
{
    S0553_DP_NONE = 0,          /**< 无数据点（该页不显示/不调节数值） */
    S0553_DP_TIME_MIN,          /**< 按摩时间-分（主控下发，组件只显示；倒计时由主控维护） */
    S0553_DP_TIME_SEC,          /**< 按摩时间-秒（主控下发，组件只显示） */
    S0553_DP_PROGRAM,           /**< 当前选中的程序：0 = 手动模式；1..20 = 自动程序序号 */
    S0553_DP_WISE,              /**< 【上机芯】手法序号（仅 PROGRAM==0 时有效，参考 S0553WiseId） */
    S0553_DP_WISE_LOWER,        /**< 【下机芯】手法序号（仅 PROGRAM==0 时有效；值域待产品确认，暂复用 S0553WiseId） */
    S0553_DP_LANGUAGE,          /**< 当前 UI 语言（S0553_LangId），动态文本按此查表 */
    S0553_DP_POWER,             /**< 【工作状态 / 电源】由主控持有并按自身业务逻辑推送，组件据此三态显示：
                                     0     = 关机（黑屏）
                                     5     = 运行（开机正常使用：LOGO → 主页）
                                     其他  = 中间态（只显示 Logo，不进入运行页）
                                     主控【不需要】调用 S0553Display_PowerOn() / PowerOff()。
                                     （早期由"旋钮长按回调"控制开关机，已废弃：工作状态属主控业务，
                                       与其余数据点一样走本条定时推送通道。） */
    S0553_DP_ZERO_GRAVITY,      /**< 零重力挡位（S0553ZeroGravityLevel）：0=关闭，1/2/3=三档 */
    S0553_DP_COUNT              /**< 数据点总数（边界，勿作为数据点使用） */
} S0553DataPoint;
// TODO：仅作参考使用，移植完成后删除-----------------------------------------------------------


/* 【步骤 1】主控自己的业务状态（可选）：组件不保存业务数据，只显示。
 *          可直接在【步骤 2】读你现有的状态，本段留空即可。 */


/* 【步骤 2】★必做★ 定时推送全部数据点 —— 主控→组件的唯一入方向通道，也是界面刷新的唯一触发点。
 *   Host_S0553_Startup调用连接成功后先推一次（否则首屏空），之后每 200ms~1s 推一次。
 *   组件会与镜像比对，值没变就什么都不做，所以放心全推、不会刷屏。 */
void Host_PushAllDataPoints(void)
{
    /* TODO：按下面数据点逐行填，右侧<...>换成主控要发送的数据，格式参考函数定义
     *   S0553_Host_NotifyValue(S0553_DP_TIME_MIN, <按摩剩余时间-分>);
     *   S0553_Host_NotifyValue(S0553_DP_TIME_SEC, <按摩剩余时间-秒>);
     *   S0553_Host_NotifyValue(S0553_DP_PROGRAM,  <程序序号：0=手动，1..20个自动程序>);
     *   S0553_Host_NotifyValue(S0553_DP_WISE,     <【上机芯】手法序号：参考 S0553WiseId>);
     *   S0553_Host_NotifyValue(S0553_DP_WISE_LOWER, <【下机芯】手法序号：值域待产品确认>);
     *   S0553_Host_NotifyValue(S0553_DP_LANGUAGE, <语言：参考 S0553_LangId>);
     *   S0553_Host_NotifyValue(S0553_DP_ZERO_GRAVITY, <零重力挡位：参考 S0553ZeroGravityLevel>);
     *   S0553_Host_NotifyValue(S0553_DP_POWER,    <工作状态：0=关机，5=运行，其他=Logo>);   ★放最后
     *
     *   POWER 由主控把自身工作状态映射后推送：0=关机黑屏，5=运行（组件 LOGO→主页），
     *   其他=中间态（组件只显示 Logo）。主控不必调 S0553Display_PowerOn()/PowerOff()。
     *   POWER 放最后，首屏即显示正确内容。
     */

    //  参考：
    S0553_Host_NotifyValue(S0553_DP_TIME_MIN, s_HostTimeMin);
    S0553_Host_NotifyValue(S0553_DP_TIME_SEC, s_HostTimeSec);
    S0553_Host_NotifyValue(S0553_DP_PROGRAM,  s_HostProgram);
    S0553_Host_NotifyValue(S0553_DP_WISE,     s_HostWise);
    S0553_Host_NotifyValue(S0553_DP_WISE_LOWER, s_HostWiseLower);
    S0553_Host_NotifyValue(S0553_DP_LANGUAGE, s_HostLanguage);
    S0553_Host_NotifyValue(S0553_DP_ZERO_GRAVITY, s_HostZeroGravity);
    /* 【工作状态】映射权在主控：0=关机，5=运行，其他=中间态（组件只显示 Logo） */
    S0553_Host_NotifyValue(S0553_DP_POWER,    s_WorkState);
}


/* 【步骤 3】（可选）执行屏上下发的指令 —— 只有"用户在屏上直接改数值"才需要，否则整段删掉。
 *   定义强符号 S0553Host_OnCommand() 即自动覆盖组件弱符号，无需调 S0553_Host_Register()。
 *   主控裁决（可拒绝/钳制）后只更新自己的状态；结果不用回推，由【步骤 2】定时推送带回。
 *
 *   参数：
 *     cmd  指令类型（S0553HostCommand）：
 *            S0553_HCMD_ADJUST  = 相对调整（暂未使用，arg=±1）
 *            S0553_HCMD_SET     = 直接设置（数据点命令，arg=目标值）
 *            S0553_HCMD_POSTURE = 坐姿动作（非数据点命令，arg=S0553PostureId）
 *     dp   数据点命令时 = 要改哪个数据点（S0553DataPoint）；动作命令时传 S0553_DP_NONE
 * 
 *     arg  指令参数：ADJUST=+1/-1；SET=目标值；POSTURE=坐姿动作（S0553PostureId） */
void S0553Host_OnCommand(S0553HostCommand cmd, S0553DataPoint dp, int16_t arg)
{
    /* ============================================================================
     * 框架只把组件意图（cmd / dp / arg）原样交给你：怎么裁决、怎么更新状态，
     * 全部是主控自己的业务逻辑，下面只给最小参考骨架。
     * 唯一要遵守的约定：【裁决后只更新主控自己的状态，结果不用回推】——
     * 由【步骤 2】Host_PushAllDataPoints() 定时推送带回（值没变组件不刷新）。
     *
     * 数据点命令（cmd = S0553_HCMD_SET）按 dp 分派，统一：
     *   1) 先做范围钳制：越界 = 拒绝（直接 break，不写状态 → 界面不刷新）。
     *   2) 合法值写入主控自己的状态变量，别在这里 NotifyValue。
     * 非数据点命令（坐姿动作：cmd = S0553_HCMD_POSTURE, dp = S0553_DP_NONE）
     * 落在 case S0553_DP_NONE 里，执行动作即可，不落数据点、不回推。
     * ============================================================================ */
    switch (dp)
    {
    /* ---------- 非数据点命令：坐姿动作（联动/靠背/小腿/足部 × 升/降 = 8 种） ---------- */
    case S0553_DP_NONE:
    {
        /* cmd = S0553_HCMD_POSTURE，arg = S0553PostureId（0..7）。纯动作，执行后不落数据点。 */
        if (cmd != S0553_HCMD_POSTURE) break;
        switch (arg)
        {
        case S0553_POSTURE_LINKAGE_UP:    /* 联动-升 */ break;
        case S0553_POSTURE_LINKAGE_DOWN:  /* 联动-降 */ break;
        case S0553_POSTURE_BACKREST_UP:   /* 靠背-升 */ break;
        case S0553_POSTURE_BACKREST_DOWN: /* 靠背-降 */ break;
        case S0553_POSTURE_CALF_UP:       /* 小腿-升 */ break;
        case S0553_POSTURE_CALF_DOWN:     /* 小腿-降 */ break;
        case S0553_POSTURE_FOOT_UP:       /* 足部-升 */ break;
        case S0553_POSTURE_FOOT_DOWN:     /* 足部-降 */ break;
        default: break;                   /* 未知动作：拒绝 */
        }
        break;
    }

    /* ---------- 自动程序 1~20 ---------- */
    case S0553_DP_PROGRAM:
    {
        /* SET：arg = 程序序号。0 = 手动；1..20 = 20 个自动程序。 */
        if (cmd != S0553_HCMD_SET) break;
        if (arg < 1 || arg > 20) break;          /* 越界拒绝（0=手动由主控自身逻辑切换） */
        s_HostProgram = (uint8_t)arg;
        /* 切到自动程序后手法不再生效（PROGRAM!=0 时组件忽略 WISE）；主控按需复位 s_HostWise。 */
        break;
    }

    /* ---------- 上机芯手法 ---------- */
    case S0553_DP_WISE:
    {
        /* SET：arg = S0553WiseId（0=无手法，1..11=11 种手法）。仅在手动模式接受。 */
        if (cmd != S0553_HCMD_SET) break;
        if (s_HostProgram != 0) break;                                /* 非手动模式：拒绝 */
        if (arg < S0553_WISE_NONE || arg >= S0553_WISE_COUNT) break;  /* 范围钳制 */
        s_HostWise = (uint8_t)arg;
        break;
    }

    /* ---------- 下机芯手法 ---------- */
    case S0553_DP_WISE_LOWER:
    {
        /* SET：arg = 下机芯手法序号。值域待产品确认，暂复用 S0553WiseId（0..S0553_WISE_COUNT-1）。 */
        if (cmd != S0553_HCMD_SET) break;
        if (s_HostProgram != 0) break;                                /* 非手动模式：拒绝 */
        if (arg < S0553_WISE_NONE || arg >= S0553_WISE_COUNT) break;
        s_HostWiseLower = (uint8_t)arg;
        break;
    }

    /* ---------- 零重力 3 档 ---------- */
    case S0553_DP_ZERO_GRAVITY:
    {
        /* SET：arg = S0553ZeroGravityLevel（0=关闭，1/2/3=三档）。
           组件在"已选中档位再按一次"时发 0 表示取消。 */
        if (cmd != S0553_HCMD_SET) break;
        if (arg < S0553_ZERO_G_OFF || arg >= S0553_ZERO_G_COUNT) break;   /* 范围钳制 */
        s_HostZeroGravity = (uint8_t)arg;
        break;
    }

    default:
        break;    /* 未实现的数据点：不回推（值没变，组件不刷新） */
    }
}


/* 【步骤 4】启动接线 —— main() 里，硬件初始化之后。
 *   只调一行 S0553_Init()：协议栈 + 显示模块 + 打开串口，立即返回（回调已自动挂接，无需注册）。
 *   连接不用你管：首次 S0553_Task() 自动发起，连不上每 2s 重试，掉线自动重连。
 *   需"连不上就停机"时，在主循环用 S0553_IsConnected()+计数判断。 */
void Host_S0553_Startup(void)
{
    /* TODO：硬件初始化后调 S0553_Init()，并先推一次全量 */
    /* 参考： */
    S0553_Init();               /* 协议栈 + 显示模块 + 打开串口，立即返回 */
    Host_PushAllDataPoints();   /* 先推一次：镜像有值，连接后首屏即正确 */
}


/* 【步骤 5】主循环接线 —— 10ms 周期任务 / while(1) 里，节拍必须严格 10ms。
 *   ① 每 10ms 调一次 S0553_Task()（通信 / UI / 自动连接 / 周期刷新都已内置）。
 *   ② 每 200ms~1s 调一次 Host_PushAllDataPoints()；POWER 非 0 自动开机，0 自动关机。
 * 
 *   ！！此函数不用移植！！ 只作为 调用S0553_Task 和 Host_PushAllDataPoints 的参考
 * 
 *  */
void Host_S0553_MainLoop(void)
{
    /* TODO：严格 10ms 调 S0553_Task()，每 200ms~1s 推一次数据 */

    /* 参考：用 tick 计数（别用 S0553_HAL_GetTickMs()，那仅调试日志） */
    static uint16_t s_Tick = 0;
    S0553_Task();                       /* 通信 / UI / 自动连接 / 周期刷新（必须 10ms） */
    if (++s_Tick >= 20) {               /* 20 × 10ms = 200ms，周期按产品需要每200xs调用一次 */
        s_Tick = 0;
        Host_PushAllDataPoints();
    }
}

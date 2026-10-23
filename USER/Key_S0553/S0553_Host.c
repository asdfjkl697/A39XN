/***************************************************************************************************
* @file      S0553_Host.c
* @brief     页面/组件 与 主控 交互框架实现（主控适配层）
* @details   本文件维护一张"渲染镜像"：s_DpValue[]（值）+ s_DpFresh[]（是否已收到过）。
*            它是主控状态的【只读投影】，不是业务状态副本 —— 组件不允许自行改写业务值，
*            任何变更都走 S0553_Host_SendCommand() 交主控裁决，再由主控 NotifyValue 回推。
*
*            【纯推模式】组件不向主控查询（无 Query 通道）：
*            主控通电期间定时推送全部数据点，本文件负责比对 —— 值没变就什么都不做，
*            值变了才触发刷新回调。因此重复推送不会产生任何重绘与串口流量。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Host.h"
#include "S0553_Display.h"   /* S0553_LangId（S0553_DP_LANGUAGE 值域）：语言归一化用 */

/* ---- 主控命令回调的【默认弱符号实现】：主控定义同名强符号即自动覆盖 ---- */
S0553_WEAK void S0553Host_OnCommand(S0553HostCommand cmd, S0553DataPoint dp, int16_t arg)
{
    (void)cmd; (void)dp; (void)arg;   /* 默认不处理：屏上调节不生效，显示照常 */
}

/* ---- 主控能力接口（默认指向弱符号；S0553_Host_Register 可覆盖，可为 NULL） ---- */
static const S0553HostOps *s_Ops = NULL;

/* ---- 默认主控接口：Command 指向弱符号，组件初始化时自动注册 ---- */
static const S0553HostOps s_DefaultHostOps = {
    .Command = S0553Host_OnCommand
};

/* ---- 数据变更回调（由显示组件注册；值确实变化时触发刷新） ---- */
static S0553HostValueChangedCb s_ValueChangedCb = NULL;

/* ---- 渲染镜像：仅用于渲染，随主控 NotifyValue 更新 ---- */
static uint8_t s_DpValue[S0553_DP_COUNT];
static uint8_t s_DpFresh[S0553_DP_COUNT];   /**< 1=已收到过主控推送的值；0=还没收到过 */

/** @brief 数据点 ID 合法性（NONE 与 COUNT 均视为无效） */
static uint8_t S0553_Host_IsValidDp(S0553DataPoint dp)
{
    return (dp > S0553_DP_NONE && dp < S0553_DP_COUNT) ? 1u : 0u;
}

void S0553_Host_Register(const S0553HostOps *ops)
{
    s_Ops = ops;
    /* 换主控实现 → 旧镜像作废，等待主控下一次定时推送重新填充 */
    for (uint8_t i = 0; i < (uint8_t)S0553_DP_COUNT; i++)
    {
        s_DpFresh[i] = 0;
    }
}

void S0553_Host_Init(void)
{
    /* 自动注册默认接口（弱符号回调）。主控定义强符号 S0553Host_OnCommand 即被覆盖，
       无需自己调 S0553_Host_Register。 */
    S0553_Host_Register(&s_DefaultHostOps);
}

uint8_t S0553_Host_GetValue(S0553DataPoint dp)
{
    if (!S0553_Host_IsValidDp(dp)) { return 0; }

    /* 【纯读】不发起任何查询：数据一律由主控定时推送过来。
       主控还没推过 → 返回 0（页面此时显示空/默认值，收到首次推送后自动刷新）。 */
    return s_DpValue[dp];
}

void S0553_Host_SetValueChangedCb(S0553HostValueChangedCb cb)
{
    s_ValueChangedCb = cb;
}

void S0553_Host_NotifyValue(S0553DataPoint dp, uint8_t value)
{
    if (!S0553_Host_IsValidDp(dp)) { return; }

    /* 【语言归一化】暂只支持中文（S0553_LANG_CN）；
       主控下发的其它语言值（EN/DE/FR 及未开放语言）一律按中文显示。
       在【数据接收入口】统一收敛，下游 ResolveText/ResolveImage/页面查表无需各自判断。 */
    if (dp == S0553_DP_LANGUAGE &&
        value != S0553_LANG_CN)
    {
        value = S0553_LANG_CN;
    }

    const uint8_t oldValue = s_DpValue[dp];
    const uint8_t hadValue = s_DpFresh[dp];

    /* 【纯推模式的"去重"就在这里】值没变 → 直接返回：不重绘、不发帧。
       所以主控【定时推送全部数据点】是安全的，不会刷屏。
       【例外】S0553_DP_POWER 不做"值去重"：它驱动的是"显示状态切换"（关机/开机/仅Logo），
       若首次在【组件连接前】收到（此时 Update 因未连接失败、显示状态未跟上），
       之后主控重推相同值会被这里吞掉、永不重试 → 黑屏。
       故 POWER 每次都交回调，由回调内按"显示状态"去重（s_PowerState != XXX 才动作）。 */
    if (hadValue && oldValue == value && dp != S0553_DP_POWER) { return; }

    s_DpValue[dp] = value;
    s_DpFresh[dp] = 1;

    /* 值确实变了（或首次收到有效值）→ 触发界面刷新。
       这是【唯一的刷新触发点】：组件不同步等待、不做回读。 */
    if (s_ValueChangedCb != NULL)
    {
        s_ValueChangedCb(dp, oldValue, value);
    }
}

void S0553_Host_SendCommand(S0553HostCommand cmd, S0553DataPoint dp, int16_t arg)
{
    /* 【异步·不同步等待】下发后立刻返回，不做回读、不阻塞。
       新值由主控执行完成后调用 S0553_Host_NotifyValue() 回推，届时触发刷新。 */
    if (s_Ops == NULL || s_Ops->Command == NULL) { return; }
    s_Ops->Command(cmd, dp, arg);
}

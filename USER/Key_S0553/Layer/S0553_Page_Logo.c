/***************************************************************************************************
* @file      S0553_Page_Logo.c
* @brief     开机 LOGO 页：UI 图层 + 执行逻辑(钩子) + 页面描述符
* @details   开机流程由【本页自己】掌控：进入本页时（OnEnter）向引擎预约"停留 N tick 后
*            跳到开机目标页"。引擎不再硬编码 Logo 过渡时序，也不再出现具体页面号。
* @date      2026-10-20
================================================================***********************************/
#include "S0553_Pages.h"
#include "S0553_ImageTable.h"

/**
 * @brief Logo 停留时长（tick，10ms/tick）
 * @note  100 = 1s，沿用改造前的实际值（原注释写"2 秒"与实际不符，此处以实际值为准）。
 */
#define LOGO_HOLD_TICKS  100

static const DisplayLayer Logo_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_LOGO_EN_WITH_BG, 0, 0, 0, 0, 0, 0},   // 背景（开机Logo；若素材为透明底请改 IMG_LOGO_EN_NO_BG，待确认）
};

/* ---- 开机页执行逻辑：进入时预约"停留后自动跳到开机目标页" ---- */
static void Logo_OnEnter(uint8_t value)
{
    (void)value;
    const S0553DisplayType target = S0553Display_GetPowerOnTargetPage();
    /* "仅 Logo"中间态：开机目标页被设为 LOGO 自身，此时停留、不预约跳转 */
    if (target == S0553_DISP_TYPE_LOGO)
    {
        return;
    }
    S0553Display_RequestTransition(target,
                                   S0553Display_GetPowerOnTargetValue(),
                                   LOGO_HOLD_TICKS);
}
const PageConfig Logo_Page = {
    .PageType     = S0553_DISP_TYPE_LOGO,
    .Layers       = Logo_Layers,
    .LayerCount   = sizeof(Logo_Layers) / sizeof(DisplayLayer),
    .EventMaps    = NULL,
    .EventCount   = 0,
    .ValueDataPoint = S0553_DP_NONE,   // 开机页无数值
    .OnEnter      = Logo_OnEnter,   // 进入开机页时预约"停留后跳到开机目标页"
};

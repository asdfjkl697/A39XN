/***************************************************************************************************
* @file      S0553_Page_None.c
* @brief     黑屏页（NONE）：UI 图层 + 页面描述符
* @details   黑屏页不配置页面级事件（EventMaps=NULL）：
*            旋钮长按(3秒)由组件统一检测并上报主控，开关机由主控决策后调用
*            S0553Display_PowerOn/PowerOff，组件随之切换；黑屏下不允许页面自行点亮屏幕。
* @date      2026-10-20
================================================================***********************************/
#include "S0553_Pages.h"
#include "S0553_ImageTable.h"

/* 黑屏页：仅一层纯黑背景 */
static const DisplayLayer None_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_BLACK_BG, 0, 0, 0, 0, 0, 0},   // 背景（通用纯黑背景；待确认）
};

/* 黑屏页描述符（对外唯一符号；无事件、无钩子） */
const PageConfig None_Page = {
    .PageType     = S0553_DISP_TYPE_NONE,
    .Layers       = None_Layers,
    .LayerCount   = sizeof(None_Layers) / sizeof(DisplayLayer),
    .EventMaps    = NULL,
    .EventCount   = 0,
    .ValueDataPoint = S0553_DP_NONE,   // 黑屏页无数值
    .OnEnter      = NULL,
};

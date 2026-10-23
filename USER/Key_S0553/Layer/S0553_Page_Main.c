/***************************************************************************************************
* @file      S0553_Page_Main.c
* @brief     主页面（MAIN）：UI 图层 + 事件 + 页面描述符
* @date      2026-10-20
================================================================***********************************/
#include "S0553_Pages.h"
#include "S0553_ImageTable.h"

static const DisplayLayer MainPage_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_MAIN_BG, 0, 0, 0, 0, 0, 0},   // 背景
    // 动态图标：随当前程序变化（程序!=0→自动程序图标；程序==0 手动→按手法选手动图标）
    // 与下方程序名文本同判据；解析失败时回退 Id 字段（IMG_MAIN_AUTO_PROGRAM_ICON_1）
    {LAYER_TYPE_IMAGE, IMG_MAIN_AUTO_PROGRAM_ICON_1, 80, 80, 0, 0, 0, 0, NULL, 0, IMGRES_MAIN_PROGRAM_ICON},
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 150, 0, 0, 24, 0, NULL, TEXTRES_MAIN_PROGRAM_NAME}, // 动态文本：当前自动程序名称（按主板程序序号+语言查表，见 S0553UI_ResolveText）

    // 动态文本：按摩剩余时间 "mm:ss"（分/秒取自主控数据点，倒计时由主控维护，组件只显示）
    // TODO(坐标待调整)：(60,200) 为占位，需按屏幕实际高度与排版校正（建议水平居中）。
    {LAYER_TYPE_TEXT, 0xFFFF, 90, 195, 0, 0, 24, 0, NULL, TEXTRES_TIME_MMSS},
};

static const PageEventMap MainPage_Events[] = {
    // 主页：短按压栈进入菜单页（菜单 ACTION_BACK 时回到这里）
    {S0553_KEY_PHYSICAL_SHORT,     {ACTION_PUSH_PAGE, S0553_DISP_TYPE_MENU, 0, NULL}},
};

const PageConfig MainPage_Page = {
    .PageType     = S0553_DISP_TYPE_MAIN,
    .Layers       = MainPage_Layers,
    .LayerCount   = sizeof(MainPage_Layers) / sizeof(DisplayLayer),
    .EventMaps    = MainPage_Events,
    .EventCount   = sizeof(MainPage_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_NONE,   // 主页数值由文本层的数据点解析（程序/语言），页级无独立数值
    .OnEnter      = NULL,
};

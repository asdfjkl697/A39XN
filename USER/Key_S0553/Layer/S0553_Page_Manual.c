/***************************************************************************************************
* @file      S0553_Page_Manual.c
* @brief     手动程序页（MANUAL）：下方 2 个可选菜单项 + 1 个返回主页项
* @details   菜单页选中"手动程序"后短按进入。
*            每项 = 【圆底层(动态，在下) + 图标层(静态，在上)】，与菜单页同一套机制：
*              · 选中   → IMG_COMMON_ROUND_ICON_SEL_BG
*              · 未选中 → IMG_COMMON_ROUND_ICON_BG
*            末项"返回主页"终点为主页（清栈后切换，不再层层回退）。
*            中间标题 = 当前选中项的名字（TEXTRES_CURSOR_ITEM_NAME，随游标 + 语言变化）。
*
* @note      旋转改【UI 游标】（第一类：组件自治，不经主控）；短按按游标跳转对应页。
* @note      占位实现：手法条目 / 名称 / 坐标待补。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / EnterTargetPage() */
#include "S0553_ImageTable.h"

/* 可选项数量：2 个手法项 + 1 个返回主页 */
#define MANUAL_ITEM_COUNT  3

/* 各菜单项【短按】的目标页：下标 = 图层 Index = UI 游标值，须与下方图层顺序一致。
   TODO(条目待定)：MANUAL_ITEM_1/2 是占位命名，确认具体是什么之后改这里 + 改名枚举。 */
static const S0553DisplayType s_ManualTargets[MANUAL_ITEM_COUNT] = {
    S0553_DISP_TYPE_MANUAL_ITEM_1,   /* 0: 手动第 1 项 */
    S0553_DISP_TYPE_MANUAL_ITEM_2,   /* 1: 手动第 2 项 */
    S0553_DISP_TYPE_MAIN,            /* 2: 返回主页    */
};

/* 子菜单项名字表 [项序][语言]，语言序 = S0553_LangId（CN / EN / DE / FR）。
   TODO(名称待定)："手动 1/2"是占位命名。 */
static const char* s_ManualItemNames[MANUAL_ITEM_COUNT - 1][S0553_LANG_MAX] = {
    /* 0 */ { "上机芯手法",   "Manual 1", "Manuell 1", "Manuel 1" },
    /* 1 */ { "下机芯手法",   "Manual 2", "Manuell 2", "Manuel 2" },
};

static void Manual_OnShortPress(void)
{
    S0553Display_EnterTargetPage(s_ManualTargets, MANUAL_ITEM_COUNT);
}

/* 中间大图表 [项序]：与 s_ManualItemNames 同序 —— 上机芯/下机芯各一张大图，末项用返回按钮大图。
   ⚠️ 大图【占位】：暂用 IMG_MANUAL_TITLE_LARGE_ICON_1..2，真实大图待替换（只改本表即可）。 */
static const uint16_t s_ManualLargeIcons[MANUAL_ITEM_COUNT] = {
    IMG_MANUAL_TITLE_LARGE_ICON_1,        /* 0 上机芯 */
    IMG_MANUAL_TITLE_LARGE_ICON_2,        /* 1 下机芯 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 2 返回主页 */
};

/* 小图标表 [项序]：与 s_ManualItemNames 同序 —— 上机芯/下机芯各一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：暂用 IMG_MANUAL_SMALL_ICON_1_DEFAULT..2_DEFAULT，真实图标待替换（只改本表即可）。 */
static const uint16_t s_ManualItemIcons[MANUAL_ITEM_COUNT] = {
    IMG_MANUAL_TITLE_SMALL_ICON_1,      /* 0 上机芯 */
    IMG_MANUAL_TITLE_SMALL_ICON_2,      /* 1 下机芯 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 2 返回主页 */
};

static const DisplayLayer Manual_Layers[] = {
    /* 背景 / 标题图 */
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},          // 背景
    /* 中间大图：随【当前选中项】切换（52×52 水平居中），图号按游标查 s_ManualLargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_MANUAL_TITLE_LARGE_ICON_1, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 中间标题：当前选中子菜单项的名字（随 UI 游标变化，按当前语言取词）
       位置取【菜单页基准】(70,155)。 */
    {LAYER_TYPE_TEXT, 0xFFFF, 58, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* 底部 2 个可选菜单项 + 1 个返回主页项
       坐标按【弧线布局规范】（见 S0553_UI_Config.h）：N=3，弦长 36（间隙 12px），
       圆底圆心 (85,206) (120,212) (155,206)，左上角 = 圆心-12；图标 +5 居中。 */
    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,      73, 194, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 78, 199, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,     108, 200, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 113, 205, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,     143, 194, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 148, 199, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},
};

static const PageEventMap Manual_Events[] = {
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, Manual_OnShortPress}},
};

const PageConfig Manual_Page = {
    .PageType       = S0553_DISP_TYPE_MANUAL,
    .Layers         = Manual_Layers,
    .LayerCount     = sizeof(Manual_Layers) / sizeof(DisplayLayer),
    .EventMaps      = Manual_Events,
    .EventCount     = sizeof(Manual_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_NONE,
    .UiItemCount    = MANUAL_ITEM_COUNT,
    .ItemNames      = s_ManualItemNames,
    .ItemLargeIcons = s_ManualLargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemIcons      = s_ManualItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = NULL,
};

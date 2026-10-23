/***************************************************************************************************
* @file      S0553_Page_Auto.c
* @brief     自动程序页（AUTO）：下方 4 个可选菜单项 + 1 个返回主页项
* @details   菜单页选中"自动程序"后短按进入。
*            每项 = 【圆底层(动态，在下) + 图标层(静态，在上)】，与菜单页同一套机制：
*              · 选中   → IMG_COMMON_ROUND_ICON_SEL_BG
*              · 未选中 → IMG_COMMON_ROUND_ICON_BG
*            末项"返回主页"终点为主页（清栈后切换，不再层层回退）。
*            中间标题 = 当前选中项的名字（TEXTRES_CURSOR_ITEM_NAME，随游标 + 语言变化）。
*
* @note      旋转改【UI 游标】（第一类：组件自治，不经主控）；短按按游标跳转对应页。
* @note      占位实现：真实程序列表 / 名称 / 坐标待补。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / EnterTargetPage() */
#include "S0553_ImageTable.h"

/* 可选项数量：4 个自动程序 + 1 个返回主页 */
#define AUTO_ITEM_COUNT  5

/* 各菜单项【短按】的目标页：下标 = 图层 Index = UI 游标值，须与下方图层顺序一致。
   末项 S0553_DISP_TYPE_MAIN 会被识别为"返回主页"（清栈后切换）。 */
static const S0553DisplayType s_AutoTargets[AUTO_ITEM_COUNT] = {
    S0553_DISP_TYPE_AUTO_PROGRAM_1,   /* 0: 自动程序 1 */
    S0553_DISP_TYPE_AUTO_PROGRAM_2,   /* 1: 自动程序 2 */
    S0553_DISP_TYPE_AUTO_PROGRAM_3,   /* 2: 自动程序 3 */
    S0553_DISP_TYPE_AUTO_PROGRAM_4,   /* 3: 自动程序 4 */
    S0553_DISP_TYPE_MAIN,             /* 4: 返回主页   */
};

/* 子菜单项名字表 [项序][语言]，语言序 = S0553_LangId（CN / EN / DE / FR）。
   下标须与 s_AutoTargets、图层 Index 一致。
   TODO(名称待定)："自动程序 1~4"是占位命名，真实名称确认后只改这张表。 */
static const char* s_AutoItemNames[AUTO_ITEM_COUNT - 1][S0553_LANG_MAX] = {
    /* 0 */ { "特色程序", "Auto Program 1", "Auto-Programm 1", "Programme Auto 1" },
    /* 1 */ { "健康养护", "Auto Program 2", "Auto-Programm 2", "Programme Auto 2" },
    /* 2 */ { "家庭专属", "Auto Program 3", "Auto-Programm 3", "Programme Auto 3" },
    /* 3 */ { "时间场景", "Auto Program 4", "Auto-Programm 4", "Programme Auto 4" },
};

static void Auto_OnShortPress(void)
{
    S0553Display_EnterTargetPage(s_AutoTargets, AUTO_ITEM_COUNT);
}

/* 中间大图表 [项序]：与 s_AutoItemNames 同序 —— 4 个自动程序各自一张大图，末项用返回按钮大图。
   ⚠️ 大图【占位】：暂用 IMG_AUTO_TITLE_LARGE_ICON_1..4，真实大图待替换（只改本表即可）。 */
static const uint16_t s_AutoLargeIcons[AUTO_ITEM_COUNT] = {
    IMG_AUTO_TITLE_LARGE_ICON_1,          /* 0 自动程序 1 */
    IMG_AUTO_TITLE_LARGE_ICON_2,          /* 1 自动程序 2 */
    IMG_AUTO_TITLE_LARGE_ICON_3,          /* 2 自动程序 3 */
    IMG_AUTO_TITLE_LARGE_ICON_4,          /* 3 自动程序 4 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 4 返回主页 */
};

/* 小图标表 [项序]：与 s_AutoItemNames 同序 —— 4 个自动程序入口各一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：暂用 IMG_AUTO_TITLE_SMALL_ICON_1..4，真实图标待替换（只改本表即可）。 */
static const uint16_t s_AutoItemIcons[AUTO_ITEM_COUNT] = {
    IMG_AUTO_TITLE_SMALL_ICON_1,          /* 0 自动程序 1 */
    IMG_AUTO_TITLE_SMALL_ICON_2,          /* 1 自动程序 2 */
    IMG_AUTO_TITLE_SMALL_ICON_3,          /* 2 自动程序 3 */
    IMG_AUTO_TITLE_SMALL_ICON_4,          /* 3 自动程序 4 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 4 返回主页 */
};

static const DisplayLayer Auto_Layers[] = {
    /* 背景 / 标题图 */
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},          // 背景
    /* 中间大图：随【当前选中项】切换（52×52 水平居中），图号按游标查 s_AutoLargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_AUTO_TITLE_LARGE_ICON_1, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 中间标题：当前选中子菜单项的名字（随 UI 游标变化，按当前语言取词）
       位置取【菜单页基准】(70,155)。 */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* 底部 4 个可选菜单项 + 1 个返回主页项
       坐标按【弧线布局规范】（见 S0553_UI_Config.h）：N=5，弦长 36（间隙 12px），
       圆底圆心 (54,187) (85,206) (120,212) (155,206) (186,187)，左上角 = 圆心-12；图标 +5 居中。

       ⚠️ 图标改用【IMG_AUTO_TITLE_SMALL_ICON_1..4】：
          IMG_AUTO_SMALL_ICON_1_DEFAULT..20_DEFAULT 已整段让给 4 个自动程序子页（每页 5 项），
          本列表页的 4 个"程序入口"图标改用标题小图标，避免与子页编号冲突。 */
    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  42, 175, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 47, 180, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  73, 194, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 78, 199, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 108, 200, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 113, 205, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 143, 194, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0, 148, 199, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 174, 175, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 4},
    {LAYER_TYPE_IMAGE, 0, 179, 180, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 4},
};

static const PageEventMap Auto_Events[] = {
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},  // 右转：选中项 +1
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},  // 左转：选中项 -1
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, Auto_OnShortPress}},  // 短按：进入选中项对应页
};

const PageConfig Auto_Page = {
    .PageType       = S0553_DISP_TYPE_AUTO,
    .Layers         = Auto_Layers,
    .LayerCount     = sizeof(Auto_Layers) / sizeof(DisplayLayer),
    .EventMaps      = Auto_Events,
    .EventCount     = sizeof(Auto_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_NONE,
    .UiItemCount    = AUTO_ITEM_COUNT,
    .ItemNames      = s_AutoItemNames,   // 中间标题取词表（下标 = 游标）
    .ItemLargeIcons = s_AutoLargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemIcons      = s_AutoItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = NULL,
};

/***************************************************************************************************
* @file      S0553_Page_AutoProgram1.c
* @brief     自动程序 1 选择页（AUTO_PROGRAM_1）：5 个子菜单项 + 1 个返回主页按键
* @details   自动程序页第 1 项短按进入（压栈）。
*            本页是【叶子页】：5 个子菜单项短按仅"选中"（暂无更深的下一级页面）；
*            末项为【返回主页】按键，短按清栈回主页。
*            旋转 = 移动本页选中游标（组件本地 UI 状态，不下发主控、不占数据点）。
* @note      圆底坐标按【弧线布局规范】（见 S0553_UI_Config.h）N=6 取用：
*            圆心 (41,174) (68,198) (102,210) (138,210) (172,198) (199,174)，
*            图层左上角 = 圆心-12，小图标再 +5 居中。
*            ⚠️ 图标【占位】：暂复用 IMG_AUTO_SMALL_ICON_1_DEFAULT..5_DEFAULT，真实图标待替换。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / NavReset() / SwitchPage() */
#include "S0553_ImageTable.h"

/* 本页可选项数：5 个子菜单项 + 1 个"返回主页"按键 */
#define AUTO_PROGRAM_1_ITEM_COUNT  6

/* 程序名复用全局 s_MainProgramName（S0553_UI_Config.c）：本页显示程序 1..5（表下标 0..4），
   PageConfig.ItemNames 直接指向 s_MainProgramName，无需本页自持名字表。 */

/**
 * @brief 进入页面：按主控当前程序号同步游标（选中态）
 * @details 本页程序 1..5；主控程序号在本页范围内 → 游标 = 程序号 - 1；
 *          不在本页范围（含 0=手动模式）→ 游标第 0 项。
 */
static void AutoProgram1_OnEnter(uint8_t value)
{
    const uint8_t cursor = (value >= 1u && value <= 5u) ? (uint8_t)(value - 1u) : 0u;
    S0553Display_SetUiCursor(cursor);
}

/**
 * @brief 短按：末项 = 返回主页；其余子项短按"选中"（下发主控，不跳转）
 * @details 选中第 cursor+1 个子程序并下发主控（S0553_DP_PROGRAM），主控裁决后回推刷新。
 */
static void AutoProgram1_OnShortPress(void)
{
    const uint8_t cursor = S0553Display_GetUiCursor();
    if (cursor == (AUTO_PROGRAM_1_ITEM_COUNT - 1))
    {
        /* 返回主页：先清空返回栈再切换 —— 否则从主页按返回会一步步退回本页。 */
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        return;
    }

    /* 选中：短按选中第 cursor+1 个子程序（1..5）并下发主控。
       不区分"再按取消"——连续点击每次都下发选中值，避免中间插 0。 */
    S0553_Host_SendCommand(S0553_HCMD_SET, S0553_DP_PROGRAM, (int16_t)(cursor + 1u));
}

/* 中间大图表 [项序]：与 s_MainProgramName（程序 1..5 段）同序 —— 5 个子菜单项各自一张大图，
   末项（返回主页）用返回按钮大图。
   ⚠️ 大图【占位】：暂用 IMG_AUTO_LARGE_ICON_1_DEFAULT..5_DEFAULT，真实大图待替换（只改本表即可）。 */
static const uint16_t s_AutoProgram1LargeIcons[AUTO_PROGRAM_1_ITEM_COUNT] = {
    IMG_AUTO_LARGE_ICON_1_DEFAULT,        /* 0 选项1 */
    IMG_AUTO_LARGE_ICON_2_DEFAULT,        /* 1 选项2 */
    IMG_AUTO_LARGE_ICON_3_DEFAULT,        /* 2 选项3 */
    IMG_AUTO_LARGE_ICON_4_DEFAULT,        /* 3 选项4 */
    IMG_AUTO_LARGE_ICON_5_DEFAULT,        /* 4 选项5 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 5 返回主页 */
};

/* 中间大图表-选中态 [项序]：与 s_AutoProgram1LargeIcons 同序 —— 5 个程序各自选中态大图，
   末项（返回主页）用返回按钮大图。选中态大图仅在"主控程序号 == 游标选中程序"时叠加显示
   （由 IMGRES_ITEM_LARGE_ICON_SELECTED 按当前页 ItemLargeIconsSelected 查表）。
   ⚠️ 占位：选中态图暂用 IMG_AUTO_LARGE_ICON_1..5_SELECTED，真实图待替换（只改本表即可）。 */
static const uint16_t s_AutoProgram1SelectedLargeIcons[AUTO_PROGRAM_1_ITEM_COUNT] = {
    IMG_AUTO_LARGE_ICON_1_SELECTED,       /* 0 选项1 选中态 */
    IMG_AUTO_LARGE_ICON_2_SELECTED,       /* 1 选项2 选中态 */
    IMG_AUTO_LARGE_ICON_3_SELECTED,       /* 2 选项3 选中态 */
    IMG_AUTO_LARGE_ICON_4_SELECTED,       /* 3 选项4 选中态 */
    IMG_AUTO_LARGE_ICON_5_SELECTED,       /* 4 选项5 选中态 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 5 返回主页 */
};

/* 小图标表 [项序]：与 s_MainProgramName（程序 1..5 段）同序 —— 5 个子菜单项各自一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：暂用 IMG_AUTO_SMALL_ICON_1_DEFAULT..5_DEFAULT，真实图标待替换（只改本表即可）。 */
static const uint16_t s_AutoProgram1ItemIcons[AUTO_PROGRAM_1_ITEM_COUNT] = {
    IMG_AUTO_SMALL_ICON_1_DEFAULT,        /* 0 选项1 */
    IMG_AUTO_SMALL_ICON_2_DEFAULT,        /* 1 选项2 */
    IMG_AUTO_SMALL_ICON_3_DEFAULT,        /* 2 选项3 */
    IMG_AUTO_SMALL_ICON_4_DEFAULT,        /* 3 选项4 */
    IMG_AUTO_SMALL_ICON_5_DEFAULT,        /* 4 选项5 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 5 返回主页 */
};

static const DisplayLayer AutoProgram1_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景

    /* 中间大图：随【当前选中子菜单项】切换（52×52 水平居中），图号按游标查 s_AutoProgram1LargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_AUTO_LARGE_ICON_1_DEFAULT, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 选中态覆盖层：主控 S0553_DP_PROGRAM == 本页游标对应程序号时，在中间大图之上叠加该程序选中态大图；
       组件不存储状态，由主控数据驱动；其余项保持默认图、整层不画。
       选中态图由 IMGRES_ITEM_LARGE_ICON_SELECTED 按 ItemLargeIconsSelected 查表，本层无需 AltId。 */
    {LAYER_TYPE_IMAGE, 0, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON_SELECTED, 0, 0},

    /* 中间标题：当前选中子菜单项的名字（随游标变化），位置取【菜单页基准】(70,155) */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* ---- 5 个子菜单项 + 1 个返回主页按键 = 圆底(动态，先画) + 小图标(动态查表，后画) ----
       圆底层用【通用】IMGRES_ITEM_STATE：选中→AltId，未选中→Id；第几项由末位 Index 指定。 */

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  29, 162, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 34, 167, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  56, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 61, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  90, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 95, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 126, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0, 131, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 160, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 4},
    {LAYER_TYPE_IMAGE, 0, 165, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 4},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 187, 162, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 5},
    {LAYER_TYPE_IMAGE, 0, 192, 167, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 5},
};

static const PageEventMap AutoProgram1_Events[] = {
    /* 旋转：移动本页选中游标（【第一类：组件自治】，引擎在 UiItemCount 内循环） */
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    /* 短按：末项回主页，其余仅选中（见 AutoProgram1_OnShortPress） */
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, AutoProgram1_OnShortPress}},
};

const PageConfig AutoProgram1_Page = {
    .PageType       = S0553_DISP_TYPE_AUTO_PROGRAM_1,
    .Layers         = AutoProgram1_Layers,
    .LayerCount     = sizeof(AutoProgram1_Layers) / sizeof(DisplayLayer),
    .EventMaps      = AutoProgram1_Events,
    .EventCount     = sizeof(AutoProgram1_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_PROGRAM,
    .ValueOffset    = 1,   // 游标 0→程序1（cursor + 1 == 程序号）
    .UiItemCount    = AUTO_PROGRAM_1_ITEM_COUNT,
    .ItemNames      = s_MainProgramName,   // 程序 1..5（表下标 0..4）
    .ItemLargeIcons = s_AutoProgram1LargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemLargeIconsSelected = s_AutoProgram1SelectedLargeIcons,  // 中间大图表-选中态（下标 = 游标）
    .ItemIcons      = s_AutoProgram1ItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = AutoProgram1_OnEnter,
};

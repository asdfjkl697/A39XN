/***************************************************************************************************
* @file      S0553_Page_ManualItem2.c
* @brief     手动程序 第 2 项选择页（MANUAL_ITEM_2）：【下机芯】7 种手法 + 1 个返回主页按键
* @details   手动程序页第 2 项短按进入（压栈）。本页是【叶子页】：
*            7 个手法项短按仅"选中"（暂无更深的下一级页面）；末项为【返回主页】按键。
* @note      圆底坐标按【弧线布局规范】（见 S0553_UI_Config.h）N=8 取用：
*            圆心 (23,143) (41,174) (68,198) (102,210) (138,210) (172,198) (199,174) (217,143)，
*            图层左上角 = 圆心-12，小图标再 +5 居中。
*            ⚠️ 图标【占位】：暂【复用】IMG_MANUAL_SMALL_ICON_1_DEFAULT..7_DEFAULT（与上机芯页重复），
*               下机芯真实图标待补 —— 用户手动替换此处即可。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / NavReset() / SwitchPage() */
#include "S0553_ImageTable.h"

/* 本页可选项数：【下机芯】7 种手法 + 1 个"返回主页"按键 */
#define MANUAL_ITEM_2_ITEM_COUNT  8

/* 手法项数（不含末项"返回主页"） */
#define MANUAL_ITEM_2_WISE_COUNT  (MANUAL_ITEM_2_ITEM_COUNT - 1)

/* 【游标 → 手法 ID】映射表：下标 = 手法项游标，值 = 下发给主控的 S0553WiseId。
   与 s_WiseName（S0553_UI_Config.c）逐项对齐：0=无手法，1..6=前 6 种手法。 */
static const S0553WiseId s_ManualItem2WiseIds[MANUAL_ITEM_2_WISE_COUNT] = {
    S0553_WISE_NONE,           /* 0 手法0 */
    S0553_WISE_KNEAD,          /* 1 手法1 */
    S0553_WISE_TAP_1,          /* 2 手法2 */
    S0553_WISE_TAP_2,          /* 3 手法3 */
    S0553_WISE_KNEAD_TAP,      /* 4 手法4 */
    S0553_WISE_SHIATSU_1,      /* 5 手法5 */
    S0553_WISE_SHIATSU_2,      /* 6 手法6 */
};

/* 手法名复用全局 s_WiseName（S0553_UI_Config.c）：见 PageConfig.ItemNames = s_WiseName。 */

/**
 * @brief 进入页面：按主控当前下机芯手法 ID 同步游标（选中态）
 * @details 手法 ID 0..6 → 游标 0..6；越界 → 游标第 0 项（无手法）。
 */
static void ManualItem2_OnEnter(uint8_t value)
{
    const uint8_t cursor = (value < MANUAL_ITEM_2_WISE_COUNT) ? value : 0u;
    S0553Display_SetUiCursor(cursor);
}

/**
 * @brief 短按：末项 = 返回主页；其余手法项 = 向主控下发"切换到该下机芯手法"
 * @details 查表把游标转换为手法 ID，下发主控（S0553_DP_WISE_LOWER）。
 */
static void ManualItem2_OnShortPress(void)
{
    const uint8_t cursor = S0553Display_GetUiCursor();

    if (cursor == (MANUAL_ITEM_2_ITEM_COUNT - 1))
    {
        /* 返回主页：先清空返回栈再切换 —— 否则从主页按返回会一步步退回本页。 */
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        return;
    }

    /* 其余手法项：查表把游标转换为手法 ID，下发给主控（SET 下机芯手法）。
       ⚠️ 上下机芯手法是【两个独立数据点】：本页是下机芯，发 S0553_DP_WISE_LOWER；
          上机芯页（ManualItem1）才发 S0553_DP_WISE，主控据此区分。 */
    S0553_Host_SendCommand(S0553_HCMD_SET, S0553_DP_WISE_LOWER, (int16_t)s_ManualItem2WiseIds[cursor]);
}

/* 中间大图表 [项序]：与 s_ManualItem2ItemNames 同序 —— 7 种手法各自一张大图，
   末项（返回主页）用返回按钮大图。
   ⚠️ 大图【占位】：下机芯真实大图待补，暂【复用】IMG_MANUAL_LARGE_ICON_1_DEFAULT..7_DEFAULT
   （与上机芯页重复），替换只需改本表。 */
static const uint16_t s_ManualItem2LargeIcons[MANUAL_ITEM_2_ITEM_COUNT] = {
    IMG_MANUAL_LARGE_ICON_1_DEFAULT,      /* 0 手法1 */
    IMG_MANUAL_LARGE_ICON_2_DEFAULT,      /* 1 手法2 */
    IMG_MANUAL_LARGE_ICON_3_DEFAULT,      /* 2 手法3 */
    IMG_MANUAL_LARGE_ICON_4_DEFAULT,      /* 3 手法4 */
    IMG_MANUAL_LARGE_ICON_5_DEFAULT,      /* 4 手法5 */
    IMG_MANUAL_LARGE_ICON_6_DEFAULT,      /* 5 手法6 */
    IMG_MANUAL_LARGE_ICON_7_DEFAULT,      /* 6 手法7 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 7 返回主页 */
};

/* 中间大图表-选中态 [项序]：与 s_ManualItem2LargeIcons 同序 —— 7 种手法各自选中态大图，
   末项（返回主页）用返回按钮大图。选中态大图仅在"主控手法ID == 游标选中手法"时叠加显示
   （由 IMGRES_ITEM_LARGE_ICON_SELECTED 按当前页 ItemLargeIconsSelected 查表）。
   ⚠️ 占位：下机芯真实选中态图待补，暂【复用】IMG_MANUAL_LARGE_ICON_1..7_SELECTED（与上机芯页重复），
   替换只需改本表。 */
static const uint16_t s_ManualItem2SelectedLargeIcons[MANUAL_ITEM_2_ITEM_COUNT] = {
    IMG_MANUAL_LARGE_ICON_1_SELECTED,     /* 0 手法1 选中态 */
    IMG_MANUAL_LARGE_ICON_2_SELECTED,     /* 1 手法2 选中态 */
    IMG_MANUAL_LARGE_ICON_3_SELECTED,     /* 2 手法3 选中态 */
    IMG_MANUAL_LARGE_ICON_4_SELECTED,     /* 3 手法4 选中态 */
    IMG_MANUAL_LARGE_ICON_5_SELECTED,     /* 4 手法5 选中态 */
    IMG_MANUAL_LARGE_ICON_6_SELECTED,     /* 5 手法6 选中态 */
    IMG_MANUAL_LARGE_ICON_7_SELECTED,     /* 6 手法7 选中态 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 7 返回主页 */
};

/* 小图标表 [项序]：与 s_ManualItem2ItemNames 同序 —— 7 种手法各自一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：下机芯真实图标待补，暂【复用】IMG_MANUAL_SMALL_ICON_1_DEFAULT..7_DEFAULT
   （与上机芯页重复），替换只需改本表。 */
static const uint16_t s_ManualItem2ItemIcons[MANUAL_ITEM_2_ITEM_COUNT] = {
    IMG_MANUAL_SMALL_ICON_1_DEFAULT,      /* 0 手法1 */
    IMG_MANUAL_SMALL_ICON_2_DEFAULT,      /* 1 手法2 */
    IMG_MANUAL_SMALL_ICON_3_DEFAULT,      /* 2 手法3 */
    IMG_MANUAL_SMALL_ICON_4_DEFAULT,      /* 3 手法4 */
    IMG_MANUAL_SMALL_ICON_5_DEFAULT,      /* 4 手法5 */
    IMG_MANUAL_SMALL_ICON_6_DEFAULT,      /* 5 手法6 */
    IMG_MANUAL_SMALL_ICON_7_DEFAULT,      /* 6 手法7 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 7 返回主页 */
};

static const DisplayLayer ManualItem2_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景
    /* 中间大图：随【当前选中手法】切换（52×52 水平居中），图号按游标查 s_ManualItem2LargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_MANUAL_LARGE_ICON_1_DEFAULT, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 选中态覆盖层：主控 S0553_DP_WISE_LOWER == 本页游标对应手法ID 时，在中间大图之上叠加该手法选中态大图；
       组件不存储状态，由主控数据驱动；其余项保持默认图、整层不画。
       选中态图由 IMGRES_ITEM_LARGE_ICON_SELECTED 按 ItemLargeIconsSelected 查表，本层无需 AltId。 */
    {LAYER_TYPE_IMAGE, 0, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON_SELECTED, 0, 0},

    /* 中间标题：当前选中手法的名字，位置取【菜单页基准】(70,155) */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* ---- 7 个手法项 + 1 个返回主页按键 = 圆底(动态，先画) + 小图标(动态查表，后画) ---- */

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  11, 131, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 16, 136, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  29, 162, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 34, 167, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  56, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 61, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  90, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0, 95, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 126, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 4},
    {LAYER_TYPE_IMAGE, 0, 131, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 4},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 160, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 5},
    {LAYER_TYPE_IMAGE, 0, 165, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 5},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 187, 162, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 6},
    {LAYER_TYPE_IMAGE, 0, 192, 167, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 6},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 205, 131, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 7},
    {LAYER_TYPE_IMAGE, 0, 210, 136, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 7},
};

static const PageEventMap ManualItem2_Events[] = {
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, ManualItem2_OnShortPress}},
};

const PageConfig ManualItem2_Page = {
    .PageType       = S0553_DISP_TYPE_MANUAL_ITEM_2,
    .Layers         = ManualItem2_Layers,
    .LayerCount     = sizeof(ManualItem2_Layers) / sizeof(DisplayLayer),
    .EventMaps      = ManualItem2_Events,
    .EventCount     = sizeof(ManualItem2_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_WISE_LOWER,
    .ValueOffset    = 0,   // 游标 0→无手法0（cursor + 0 == 手法ID）
    .UiItemCount    = MANUAL_ITEM_2_ITEM_COUNT,
    .ItemNames      = s_WiseName,
    .ItemLargeIcons = s_ManualItem2LargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemLargeIconsSelected = s_ManualItem2SelectedLargeIcons,  // 中间大图表-选中态（下标 = 游标）
    .ItemIcons      = s_ManualItem2ItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = ManualItem2_OnEnter,
};

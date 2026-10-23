/***************************************************************************************************
* @file      S0553_Page_ManualItem1.c
* @brief     手动程序 第 1 项选择页（MANUAL_ITEM_1）：【上机芯】12 种手法 + 1 个返回主页按键
* @details   手动程序页第 1 项短按进入（压栈）。本页是【叶子页】：
*            12 个手法项短按仅"选中"（暂无更深的下一级页面）；末项为【返回主页】按键。
* @note      圆底坐标按【弧线布局规范】（见 S0553_UI_Config.h）N=13 取用。
*            ⚠️ N=13 是规范的【例外】：因项数多到要绕到屏幕上半部，弧心回到屏幕中心
*               (120,120)、半径缩到 97（否则上半部圆底的左上角会被圆形屏幕裁掉）。
*            圆心 (44,60) (27,92) (23,128) (33,162) (54,191) (85,210) (120,217)
*                 (155,210) (186,191) (207,162) (217,128) (213,92) (196,60)，
*            图层左上角 = 圆心-12，小图标再 +5 居中。
*            ⚠️ 图标【占位】：暂复用 IMG_MANUAL_SMALL_ICON_1_DEFAULT..12_DEFAULT，真实图标待替换。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / NavReset() / SwitchPage() */
#include "S0553_ImageTable.h"

/* 本页可选项数：【上机芯】12 种手法 + 1 个"返回主页"按键 */
#define MANUAL_ITEM_1_ITEM_COUNT  13

/* 手法项数（不含末项"返回主页"） */
#define MANUAL_ITEM_1_WISE_COUNT  (MANUAL_ITEM_1_ITEM_COUNT - 1)

/* 【游标 → 手法 ID】映射表：下标 = 手法项游标，值 = 下发给主控的 S0553WiseId。
   与 s_WiseName（S0553_UI_Config.c）逐项对齐：0=无手法，1..11=11 种手法；
   上机芯 12 项 = 无手法 + 全部 11 种手法。 */
static const S0553WiseId s_ManualItem1WiseIds[MANUAL_ITEM_1_WISE_COUNT] = {
    S0553_WISE_NONE,           /*  0 手法0 */
    S0553_WISE_KNEAD,          /*  1 手法1 */
    S0553_WISE_TAP_1,          /*  2 手法2 */
    S0553_WISE_TAP_2,          /*  3 手法3 */
    S0553_WISE_KNEAD_TAP,      /*  4 手法4 */
    S0553_WISE_SHIATSU_1,      /*  5 手法5 */
    S0553_WISE_SHIATSU_2,      /*  6 手法6 */
    S0553_WISE_SHOULDER_GRAB,  /*  7 手法7 */
    S0553_WISE_4D_1,           /*  8 手法8 */
    S0553_WISE_4D_2,           /*  9 手法9 */
    S0553_WISE_4D_3,           /* 10 手法10 */
    S0553_WISE_4D_4,           /* 11 手法11 */
};

/* 手法名复用全局 s_WiseName（S0553_UI_Config.c）：见 PageConfig.ItemNames = s_WiseName。 */

/**
 * @brief 进入页面：按主控当前上机芯手法 ID 同步游标（选中态）
 * @details 手法 ID 0..11 → 游标 0..11；越界 → 游标第 0 项（无手法）。
 */
static void ManualItem1_OnEnter(uint8_t value)
{
    const uint8_t cursor = (value < MANUAL_ITEM_1_WISE_COUNT) ? value : 0u;
    S0553Display_SetUiCursor(cursor);
}

/**
 * @brief 短按：末项 = 返回主页；其余手法项 = 向主控下发"切换到该手法"
 * @details 【第二类：数据修改类】仅下发命令、不同步等待；主控裁决后回推
 *          S0553_DP_WISE，组件据此刷新。
 *          手法序号 1-based：第 X 项（"手法X"，Index = X-1）→ DP_WISE = X。
 */
static void ManualItem1_OnShortPress(void)
{
    const uint8_t cursor = S0553Display_GetUiCursor();

    if (cursor == (MANUAL_ITEM_1_ITEM_COUNT - 1))
    {
        /* 返回主页：先清空返回栈再切换 —— 否则从主页按返回会一步步退回本页。 */
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        return;
    }

    /* 其余手法项：查表把游标转换为手法 ID，下发给主控（SET 手法）。 */
    S0553_Host_SendCommand(S0553_HCMD_SET, S0553_DP_WISE, (int16_t)s_ManualItem1WiseIds[cursor]);
}

/* 中间大图表 [项序]：与 s_ManualItem1ItemNames 同序 —— 12 种手法各自一张大图，
   末项（返回主页）用返回按钮大图。
   ⚠️ 大图【占位】：暂用 IMG_MANUAL_LARGE_ICON_1_DEFAULT..12_DEFAULT，真实大图待替换（只改本表即可）。 */
static const uint16_t s_ManualItem1LargeIcons[MANUAL_ITEM_1_ITEM_COUNT] = {
    IMG_MANUAL_LARGE_ICON_1_DEFAULT,      /*  0 手法1 */
    IMG_MANUAL_LARGE_ICON_2_DEFAULT,      /*  1 手法2 */
    IMG_MANUAL_LARGE_ICON_3_DEFAULT,      /*  2 手法3 */
    IMG_MANUAL_LARGE_ICON_4_DEFAULT,      /*  3 手法4 */
    IMG_MANUAL_LARGE_ICON_5_DEFAULT,      /*  4 手法5 */
    IMG_MANUAL_LARGE_ICON_6_DEFAULT,      /*  5 手法6 */
    IMG_MANUAL_LARGE_ICON_7_DEFAULT,      /*  6 手法7 */
    IMG_MANUAL_LARGE_ICON_8_DEFAULT,      /*  7 手法8 */
    IMG_MANUAL_LARGE_ICON_9_DEFAULT,      /*  8 手法9 */
    IMG_MANUAL_LARGE_ICON_10_DEFAULT,     /*  9 手法10 */
    IMG_MANUAL_LARGE_ICON_11_DEFAULT,     /* 10 手法11 */
    IMG_MANUAL_LARGE_ICON_12_DEFAULT,     /* 11 手法12 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 12 返回主页 */
};

/* 中间大图表-选中态 [项序]：与 s_ManualItem1LargeIcons 同序 —— 12 种手法各自选中态大图，
   末项（返回主页）用返回按钮大图。选中态大图仅在"主控手法ID == 游标选中手法"时叠加显示
   （由 IMGRES_ITEM_LARGE_ICON_SELECTED 按当前页 ItemLargeIconsSelected 查表）。
   ⚠️ 占位：选中态图暂用 IMG_MANUAL_LARGE_ICON_1..12_SELECTED，真实图待替换（只改本表即可）。 */
static const uint16_t s_ManualItem1SelectedLargeIcons[MANUAL_ITEM_1_ITEM_COUNT] = {
    IMG_MANUAL_LARGE_ICON_1_SELECTED,     /*  0 手法1 选中态 */
    IMG_MANUAL_LARGE_ICON_2_SELECTED,     /*  1 手法2 选中态 */
    IMG_MANUAL_LARGE_ICON_3_SELECTED,     /*  2 手法3 选中态 */
    IMG_MANUAL_LARGE_ICON_4_SELECTED,     /*  3 手法4 选中态 */
    IMG_MANUAL_LARGE_ICON_5_SELECTED,     /*  4 手法5 选中态 */
    IMG_MANUAL_LARGE_ICON_6_SELECTED,     /*  5 手法6 选中态 */
    IMG_MANUAL_LARGE_ICON_7_SELECTED,     /*  6 手法7 选中态 */
    IMG_MANUAL_LARGE_ICON_8_SELECTED,     /*  7 手法8 选中态 */
    IMG_MANUAL_LARGE_ICON_9_SELECTED,     /*  8 手法9 选中态 */
    IMG_MANUAL_LARGE_ICON_10_SELECTED,    /*  9 手法10 选中态 */
    IMG_MANUAL_LARGE_ICON_11_SELECTED,    /* 10 手法11 选中态 */
    IMG_MANUAL_LARGE_ICON_12_SELECTED,    /* 11 手法12 选中态 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 12 返回主页 */
};

/* 小图标表 [项序]：与 s_ManualItem1ItemNames 同序 —— 12 种手法各自一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：暂用 IMG_MANUAL_SMALL_ICON_1_DEFAULT..12_DEFAULT，真实图标待替换（只改本表即可）。 */
static const uint16_t s_ManualItem1ItemIcons[MANUAL_ITEM_1_ITEM_COUNT] = {
    IMG_MANUAL_SMALL_ICON_1_DEFAULT,      /*  0 手法1 */
    IMG_MANUAL_SMALL_ICON_2_DEFAULT,      /*  1 手法2 */
    IMG_MANUAL_SMALL_ICON_3_DEFAULT,      /*  2 手法3 */
    IMG_MANUAL_SMALL_ICON_4_DEFAULT,      /*  3 手法4 */
    IMG_MANUAL_SMALL_ICON_5_DEFAULT,      /*  4 手法5 */
    IMG_MANUAL_SMALL_ICON_6_DEFAULT,      /*  5 手法6 */
    IMG_MANUAL_SMALL_ICON_7_DEFAULT,      /*  6 手法7 */
    IMG_MANUAL_SMALL_ICON_8_DEFAULT,      /*  7 手法8 */
    IMG_MANUAL_SMALL_ICON_9_DEFAULT,      /*  8 手法9 */
    IMG_MANUAL_SMALL_ICON_10_DEFAULT,     /*  9 手法10 */
    IMG_MANUAL_SMALL_ICON_11_DEFAULT,     /* 10 手法11 */
    IMG_MANUAL_SMALL_ICON_12_DEFAULT,     /* 11 手法12 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 12 返回主页 */
};

static const DisplayLayer ManualItem1_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景
    /* 中间大图：随【当前选中手法】切换（52×52 水平居中），图号按游标查 s_ManualItem1LargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_MANUAL_LARGE_ICON_1_DEFAULT, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 选中态覆盖层：主控 S0553_DP_WISE == 本页游标对应手法ID 时，在中间大图之上叠加该手法选中态大图；
       组件不存储状态，由主控数据驱动；其余项保持默认图、整层不画。
       选中态图由 IMGRES_ITEM_LARGE_ICON_SELECTED 按 ItemLargeIconsSelected 查表，本层无需 AltId。 */
    {LAYER_TYPE_IMAGE, 0, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON_SELECTED, 0, 0},

    /* 中间标题：当前选中手法的名字，位置取【菜单页基准】(70,155) */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* ---- 12 个手法项 + 1 个返回主页按键 = 圆底(动态，先画) + 小图标(动态查表，后画) ---- */

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  32,  48, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0,  37,  53, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  15,  80, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0,  20,  85, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  11, 116, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0,  16, 121, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  21, 150, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0,  26, 155, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  42, 179, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 4},
    {LAYER_TYPE_IMAGE, 0,  47, 184, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 4},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  73, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 5},
    {LAYER_TYPE_IMAGE, 0,  78, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 5},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 108, 205, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 6},
    {LAYER_TYPE_IMAGE, 0, 113, 210, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 6},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 143, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 7},
    {LAYER_TYPE_IMAGE, 0, 148, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 7},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 174, 179, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 8},
    {LAYER_TYPE_IMAGE, 0, 179, 184, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 8},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 195, 150, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 9},
    {LAYER_TYPE_IMAGE, 0, 200, 155, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 9},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 205, 116, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 10},
    {LAYER_TYPE_IMAGE, 0, 210, 121, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 10},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 201,  80, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 11},
    {LAYER_TYPE_IMAGE, 0, 206,  85, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 11},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 184,  48, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 12},
    {LAYER_TYPE_IMAGE, 0, 189,  53, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 12},
};

static const PageEventMap ManualItem1_Events[] = {
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, ManualItem1_OnShortPress}},
};

const PageConfig ManualItem1_Page = {
    .PageType       = S0553_DISP_TYPE_MANUAL_ITEM_1,
    .Layers         = ManualItem1_Layers,
    .LayerCount     = sizeof(ManualItem1_Layers) / sizeof(DisplayLayer),
    .EventMaps      = ManualItem1_Events,
    .EventCount     = sizeof(ManualItem1_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_WISE,
    .ValueOffset    = 0,   // 游标 0→无手法0（cursor + 0 == 手法ID）
    .UiItemCount    = MANUAL_ITEM_1_ITEM_COUNT,
    .ItemNames      = s_WiseName,
    .ItemLargeIcons = s_ManualItem1LargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemLargeIconsSelected = s_ManualItem1SelectedLargeIcons,  // 中间大图表-选中态（下标 = 游标）
    .ItemIcons      = s_ManualItem1ItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = ManualItem1_OnEnter,
};

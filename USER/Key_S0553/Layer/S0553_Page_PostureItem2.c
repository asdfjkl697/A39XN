/***************************************************************************************************
* @file      S0553_Page_PostureItem2.c
* @brief     坐姿 第 2 项选择页（POSTURE_ITEM_2）：【零重力】3 个主菜单 + 1 个返回主页按键
* @details   坐姿页第 2 项短按进入（压栈）。本页是【叶子页】：
*            3 个主菜单项短按仅"选中"（暂无更深的下一级页面）；末项为【返回主页】按键。
* @note      圆底坐标按【弧线布局规范】（见 S0553_UI_Config.h）N=4 取用：
*            圆心 (68,198) (102,210) (138,210) (172,198)，
*            图层左上角 = 圆心-12，小图标再 +5 居中。
*            ⚠️ 图标【占位】：暂复用 IMG_POSTURE_ZERO_G_SMALL_ICON_1_DEFAULT..3_DEFAULT，真实图标待替换。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / NavReset() / SwitchPage() */
#include "S0553_ImageTable.h"
#include "S0553_Host.h"        /* S0553_Host_SendCommand() / S0553_DP_ZERO_GRAVITY */

/* 本页可选项数：3 个主菜单项 + 1 个"返回主页"按键 */
#define POSTURE_ITEM_2_ITEM_COUNT  4

/* TODO(文案待定)：3 个零重力项的名称待产品确认，现为占位。 */
static const char* s_PostureItem2ItemNames[POSTURE_ITEM_2_ITEM_COUNT - 1][S0553_LANG_MAX] = {
    /* 0 */ { " 零重力1",  "Zero-G 1", "Null-G 1",  "Zéro-G 1" },
    /* 1 */ { " 零重力2",  "Zero-G 2", "Null-G 2",  "Zéro-G 2" },
    /* 2 */ { " 零重力3",  "Zero-G 3", "Null-G 3",  "Zéro-G 3" },
};

/**
 * @brief 进入页面：按主控当前零重力挡位同步游标（选中态）
 * @details 挡位 1/2/3 → 游标 0/1/2（选中零重力1/2/3）；挡位 0 → 游标指向返回键（取消选中）。
 */
static void PostureItem2_OnEnter(uint8_t value)
{
    /* 0=关闭 → 游标到返回键（末项）；1/2/3 → 游标 0/1/2 */
    const uint8_t cursor = (value == 0u) ? (uint8_t)(POSTURE_ITEM_2_ITEM_COUNT - 1u)
                                         : (uint8_t)(value - 1u);
    S0553Display_SetUiCursor(cursor);
}

/**
 * @brief 短按：末项 = 返回主页；零重力项 = 选中 / 取消
 */
static void PostureItem2_OnShortPress(void)
{
    const uint8_t cursor = S0553Display_GetUiCursor();
    if (cursor == (POSTURE_ITEM_2_ITEM_COUNT - 1))
    {
        /* 返回主页：先清空返回栈再切换 —— 否则从主页按返回会一步步退回本页。 */
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        return;
    }

    /* 零重力项（游标 0/1/2 → 挡位 1/2/3）：短按即选中对应档并下发主控。
       不区分"再按取消"——连续点击每次都下发选中值，避免中间插 0 和游标跳动。 */
    const uint8_t gear = (uint8_t)(cursor + 1u);
    S0553_Host_SendCommand(S0553_HCMD_SET, S0553_DP_ZERO_GRAVITY, (int16_t)gear);
}

/* 中间大图表 [项序]：与 s_PostureItem2ItemNames 同序 —— 3 个零重力项各自一张大图，
   末项（返回主页）用返回按钮大图。
   ⚠️ 大图【占位】：暂用 IMG_POSTURE_ZERO_G_LARGE_ICON_1_DEFAULT..3_DEFAULT，真实大图待替换（只改本表即可）。 */
static const uint16_t s_PostureItem2LargeIcons[POSTURE_ITEM_2_ITEM_COUNT] = {
    IMG_POSTURE_ZERO_G_LARGE_ICON_1_DEFAULT, /* 0 零重力1 */
    IMG_POSTURE_ZERO_G_LARGE_ICON_2_DEFAULT, /* 1 零重力2 */
    IMG_POSTURE_ZERO_G_LARGE_ICON_3_DEFAULT, /* 2 零重力3 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT,    /* 3 返回主页 */
};

/* 中间大图表-选中态 [项序]：与 s_PostureItem2LargeIcons 同序 —— 1/2/3 档各自选中态大图，
   末项（返回主页）用返回按钮大图。选中态大图仅在"主控档位 == 游标选中项"时叠加显示
   （由 IMGRES_ITEM_LARGE_ICON_SELECTED 按当前页 ItemLargeIconsSelected 查表）。
   ⚠️ 占位：选中态图暂用 IMG_POSTURE_ZERO_G_LARGE_ICON_1/2/3_SELECTED，真实图待替换（只改本表即可）。 */
static const uint16_t s_PostureItem2SelectedLargeIcons[POSTURE_ITEM_2_ITEM_COUNT] = {
    IMG_POSTURE_ZERO_G_LARGE_ICON_1_SELECTED, /* 0 零重力1 选中态 */
    IMG_POSTURE_ZERO_G_LARGE_ICON_2_SELECTED, /* 1 零重力2 选中态 */
    IMG_POSTURE_ZERO_G_LARGE_ICON_3_SELECTED, /* 2 零重力3 选中态 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT,     /* 3 返回主页 */
};

/* 小图标表 [项序]：与 s_PostureItem2ItemNames 同序 —— 3 个零重力项各自一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：暂用 IMG_POSTURE_ZERO_G_SMALL_ICON_1_DEFAULT..3_DEFAULT，真实图标待替换（只改本表即可）。 */
static const uint16_t s_PostureItem2ItemIcons[POSTURE_ITEM_2_ITEM_COUNT] = {
    IMG_POSTURE_ZERO_G_SMALL_ICON_1_DEFAULT, /* 0 零重力1 */
    IMG_POSTURE_ZERO_G_SMALL_ICON_2_DEFAULT, /* 1 零重力2 */
    IMG_POSTURE_ZERO_G_SMALL_ICON_3_DEFAULT, /* 2 零重力3 */
    IMG_COMMON_SMALL_BACK_BUTTON,            /* 3 返回主页 */
};

static const DisplayLayer PostureItem2_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景
    /* 中间大图：随【当前选中项】切换（52×52 水平居中），图号按游标查 s_PostureItem2LargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_POSTURE_ZERO_G_LARGE_ICON_1_DEFAULT, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 零重力【选中标记】覆盖层：主控下发 S0553_DP_ZERO_GRAVITY==X 档，且子菜单游标选中 X 档项时，
       在中间大图之上叠加显示该档选中态大图（组件不存储该状态，由主控数据驱动）；其余项保持默认图、整层不画。
       选中态图由 IMGRES_ITEM_LARGE_ICON_SELECTED 按当前页 ItemLargeIconsSelected 查表（s_PostureItem2SelectedLargeIcons），本层无需 AltId。 */
    {LAYER_TYPE_IMAGE, 0, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON_SELECTED, 0, 0},

    /* 中间标题：当前选中项的名字，位置取【菜单页基准】(70,155) */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* ---- 3 个主菜单项 + 1 个返回主页按键 = 圆底(动态，先画) + 小图标(动态查表，后画) ---- */

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  56, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 61, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  90, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 95, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 126, 198, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 131, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 160, 186, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0, 165, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},
};

static const PageEventMap PostureItem2_Events[] = {
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, PostureItem2_OnShortPress}},
};

const PageConfig PostureItem2_Page = {
    .PageType       = S0553_DISP_TYPE_POSTURE_ITEM_2,
    .Layers         = PostureItem2_Layers,
    .LayerCount     = sizeof(PostureItem2_Layers) / sizeof(DisplayLayer),
    .EventMaps      = PostureItem2_Events,
    .EventCount     = sizeof(PostureItem2_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_ZERO_GRAVITY,
    .ValueOffset    = 1,   // 游标 0→零重力1 档（cursor + 1 == 挡位）
    .UiItemCount    = POSTURE_ITEM_2_ITEM_COUNT,
    .ItemNames      = s_PostureItem2ItemNames,
    .ItemLargeIcons = s_PostureItem2LargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemLargeIconsSelected = s_PostureItem2SelectedLargeIcons,  // 中间大图表-选中态（下标 = 游标，与标题同序）
    .ItemIcons      = s_PostureItem2ItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = PostureItem2_OnEnter,
};

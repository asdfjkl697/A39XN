/***************************************************************************************************
* @file      S0553_Page_PostureItem1.c
* @brief     坐姿调节页（POSTURE_ITEM_1）：4 个坐姿部件 + 1 个返回主页按键
* @details   坐姿页第 1 项短按进入（压栈）。两级交互：
*              · 浏览态：旋转移动游标选择部件（靠背/联动/小腿/足部），圆底高亮跟随游标；
*                        短按某部件 → 选中并进入调节态；末项短按 → 回主页。
*              · 调节态：中间大图叠加显示该部件选中态；左旋=升、右旋=降，发对应坐姿动作
*                        "开始"命令（持续运行）；再次短按 → 清除选中并退回坐姿页（POSTURE），
*                        并先发"停止"命令让电机立即停下。
*                        停手检测：本页 OnTick 每 10ms 累计"距最后一次旋转的 tick 数"，
*                        超过阈值即视为停手 → 主动发"停止"并退出调节态（交互逻辑归属组件，
*                        不暴露给主控；主控只负责"开始就动 / 停止就停 / 限位自停"）。
*             坐姿是【纯动作命令】（非数据点）：选中态由组件本地"激活选中项"驱动
*             （IMGRES_ITEM_LARGE_ICON_SELECTED 在无数据点页按激活项查表），主控只执行动作。
* @note      圆底坐标按【弧线布局规范】（见 S0553_UI_Config.h）N=5 取用：
*            圆心 (54,187) (85,206) (120,212) (155,206) (186,187)，
*            图层左上角 = 圆心-12，小图标再 +5 居中。
*            ⚠️ 图标【占位】：第 4 项小图标暂复用第 1 个；选中态大图暂用各部件 _SELECTED 图，真实图标待替换。
* @date      2026-10-22
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* GetUiCursor / MoveUiCursor / ActiveCursor / BackPage / NavReset / SwitchPage */
#include "S0553_Host.h"        /* S0553_Host_SendCommand() / S0553_HCMD_POSTURE / S0553PostureId */
#include "S0553_ImageTable.h"

/* 本页可选项数：4 个坐姿部件 + 1 个"返回主页"按键 */
#define POSTURE_ITEM_1_ITEM_COUNT  5

/* TODO(文案待定)：4 个坐姿部件的名称待产品确认，现为占位。 */
static const char* s_PostureItem1ItemNames[POSTURE_ITEM_1_ITEM_COUNT - 1][S0553_LANG_MAX] = {
    /* 0 */ { "  靠背",   "Posture 1", "Sitz 1",   "Posture 1" },
    /* 1 */ { "  联动",   "Posture 2", "Sitz 2",   "Posture 2" },
    /* 2 */ { "  小腿",   "Posture 3", "Sitz 3",   "Posture 3" },
    /* 3 */ { "  足部",   "Posture 4", "Sitz 4",   "Posture 4" },
};

/* 【游标 → 坐姿动作命令】映射表：下标 = 部件游标（靠背/联动/小腿/足部）。
   坐姿是【纯动作命令】（非数据点）：短按选中某部件后，左右旋钮发该部件的升/降动作，
   主控执行即可，无需回推状态（见 S0553_Host.h 的 S0553PostureId 说明）。 */
static const S0553PostureId s_PostureItem1CmdUp[POSTURE_ITEM_1_ITEM_COUNT - 1] = {
    S0553_POSTURE_BACKREST_UP,  /* 0 靠背-升 */
    S0553_POSTURE_LINKAGE_UP,   /* 1 联动-升 */
    S0553_POSTURE_CALF_UP,      /* 2 小腿-升 */
    S0553_POSTURE_FOOT_UP,      /* 3 足部-升 */
};
static const S0553PostureId s_PostureItem1CmdDown[POSTURE_ITEM_1_ITEM_COUNT - 1] = {
    S0553_POSTURE_BACKREST_DOWN, /* 0 靠背-降 */
    S0553_POSTURE_LINKAGE_DOWN,  /* 1 联动-降 */
    S0553_POSTURE_CALF_DOWN,     /* 2 小腿-降 */
    S0553_POSTURE_FOOT_DOWN,     /* 3 足部-降 */
};

/* ---- 坐姿"停手"检测（交互逻辑归属组件，不暴露给主控）----
   旋钮旋转事件是离散的（转一格一个事件、无"停手"信号），电机又是持续运行型，
   因此本页用 OnTick 累计"距最后一次旋转的 tick 数"，超过阈值即视为停手：
   主动发停止命令并退出调节态。阈值只在本页内部，主控无需知道。 */
#define POSTURE_ITEM_1_STOP_TICKS  80u   /**< 停手阈值：80 tick × 10ms = 800ms 无旋转即停手 */
static uint16_t s_PostureItem1IdleTicks = 0;  /**< 距最后一次旋转的 tick 数（仅调节态累计） */

/**
 * @brief 进入页面：清除激活选中项（回到浏览态）
 */
static void PostureItem1_OnEnter(uint8_t value)
{
    (void)value;
    /* 进入页面：保险起见发一次"停止"，避免上次离开时未停止、电机仍在持续运行。 */
    S0553_Host_SendCommand(S0553_HCMD_POSTURE_STOP, S0553_DP_NONE, 0);
    S0553Display_SetActiveCursor(0xFF);
    s_PostureItem1IdleTicks = 0;   /* 复位停手计时（浏览态不累计） */
}

/**
 * @brief 旋转：浏览态移动游标选择部件；调节态发该部件的升降"开始"命令（持续运行）
 * @param delta +1（右旋）/ -1（左旋）
 * @note  方向约定：左旋=升，右旋=降。
 * @note  电机为持续运行型：每格旋转发一次 S0553_HCMD_POSTURE（开始）；"停手自动停"
 *        由本页 OnTick 检测（距最后一次旋转超阈值即发停止），主控无需感知"停手"。
 */
static void PostureItem1_OnRotate(int8_t delta)
{
    const uint8_t active = S0553Display_GetActiveCursor();
    if (active != 0xFF)
    {
        /* 调节态：每格发一次该部件的升降"开始"命令（左旋=升，右旋=降），持续运行。
           每发一次即复位停手计时（表示"用户仍在旋转"）；停手由 OnTick 超时发停止。 */
        const S0553PostureId cmd = (delta < 0) ? s_PostureItem1CmdUp[active]
                                               : s_PostureItem1CmdDown[active];
        S0553_Host_SendCommand(S0553_HCMD_POSTURE, S0553_DP_NONE, (int16_t)cmd);
        s_PostureItem1IdleTicks = 0;   /* 复位停手计时 */
        return;
    }
    /* 浏览态：移动游标 */
    S0553Display_MoveUiCursor(delta);
}

static void PostureItem1_OnRotateCW(void)  { PostureItem1_OnRotate(+1); }
static void PostureItem1_OnRotateCCW(void) { PostureItem1_OnRotate(-1); }

/**
 * @brief 短按：
 *   - 末项（返回主页）      → 回主页；
 *   - 浏览态·坐姿部件项      → 选中该部件，进入调节态（中间大图显示选中态）；
 *   - 调节态（已选中部件）   → 清除选中并退回坐姿页（POSTURE）。
 */
static void PostureItem1_OnShortPress(void)
{
    const uint8_t cursor = S0553Display_GetUiCursor();

    if (cursor == (POSTURE_ITEM_1_ITEM_COUNT - 1))
    {
        /* 返回主页：先清空返回栈再切换 —— 否则从主页按返回会一步步退回本页。
           离开坐姿调节 → 先发"停止"，确保持续运行的坐姿电机立即停下。 */
        S0553_Host_SendCommand(S0553_HCMD_POSTURE_STOP, S0553_DP_NONE, 0);
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        return;
    }

    if (S0553Display_GetActiveCursor() == 0xFF)
    {
        /* 浏览态：短按选中该部件，进入调节态 */
        S0553Display_SetActiveCursor(cursor);
    }
    else
    {
        /* 调节态：再次短按 → 发"停止"、清除激活并退回坐姿页（出栈返回 POSTURE）。
           电机为持续运行型，退出调节态必须显式停止，否则会一直动到限位。 */
        S0553_Host_SendCommand(S0553_HCMD_POSTURE_STOP, S0553_DP_NONE, 0);
        S0553Display_SetActiveCursor(0xFF);
        s_PostureItem1IdleTicks = 0;
        // (void)S0553Display_BackPage();
    }
}

/**
 * @brief 周期钩子（每 10ms）：调节态下累计"距最后一次旋转的 tick 数"，超阈值视为停手
 * @note  停手即主动发"停止"并退出调节态（电机为持续运行型，不显式停止会一直动到限位）。
 *        阈值由本页 POSTURE_ITEM_1_STOP_TICKS 定义，交互逻辑完全归属组件，不暴露给主控。
 */
static void PostureItem1_OnTick(void)
{
    if (S0553Display_GetActiveCursor() == 0xFF) { return; }  /* 浏览态：不累计 */

    if (s_PostureItem1IdleTicks < 0xFFFF) { s_PostureItem1IdleTicks++; }
    if (s_PostureItem1IdleTicks >= POSTURE_ITEM_1_STOP_TICKS)
    {
        /* 停手超时：发"停止"，并退出调节态（回到浏览态） */
        S0553_Host_SendCommand(S0553_HCMD_POSTURE_STOP, S0553_DP_NONE, 0);
        // S0553Display_SetActiveCursor(0xFF);
        s_PostureItem1IdleTicks = 0;
    }
}

/* 中间大图表 [项序]：与 s_PostureItem1ItemNames 同序 —— 4 个坐姿项各自一张大图，
   末项（返回主页）用返回按钮大图。
   ⚠️ 大图【占位】：暂用靠背/联动/小腿/足部四张大图，真实大图待替换（只改本表即可）。 */
static const uint16_t s_PostureItem1LargeIcons[POSTURE_ITEM_1_ITEM_COUNT] = {
    IMG_POSTURE_BACKREST_LARGE_ICON_DEFAULT, /* 0 坐姿1 */
    IMG_POSTURE_LINKAGE_LARGE_ICON_DEFAULT,  /* 1 坐姿2 */
    IMG_POSTURE_CALF_LARGE_ICON_DEFAULT,     /* 2 坐姿3 */
    IMG_POSTURE_FOOT_LARGE_ICON_DEFAULT,     /* 3 坐姿4 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT,    /* 4 返回主页 */
};

/* 中间大图表-选中态 [项序]：与 s_PostureItem1LargeIcons 同序 —— 4 个坐姿项各自选中态大图，
   末项（返回主页）用返回按钮大图。仅在"该部件被短按激活"时叠加显示
   （IMGRES_ITEM_LARGE_ICON_SELECTED 在无数据点页按组件本地"激活选中项"查表）。
   ⚠️ 占位：选中态图暂用靠背/联动/小腿/足部 _SELECTED 图，真实图待替换（只改本表即可）。 */
static const uint16_t s_PostureItem1SelectedLargeIcons[POSTURE_ITEM_1_ITEM_COUNT] = {
    IMG_POSTURE_BACKREST_LARGE_ICON_SELECTED, /* 0 坐姿1 选中态 */
    IMG_POSTURE_LINKAGE_LARGE_ICON_SELECTED,  /* 1 坐姿2 选中态 */
    IMG_POSTURE_CALF_LARGE_ICON_SELECTED,     /* 2 坐姿3 选中态 */
    IMG_POSTURE_FOOT_LARGE_ICON_SELECTED,     /* 3 坐姿4 选中态 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT,     /* 4 返回主页 */
};

/* 小图标表 [项序]：与 s_PostureItem1ItemNames 同序 —— 4 个坐姿项各自一张小图标，
   末项（返回主页）用返回按钮小图标。
   ⚠️ 图标【占位】：现有 ZERO_G_SMALL_ICON 只有 1..3，第 4 项暂复用第 1 个，真实图标待替换（只改本表即可）。 */
static const uint16_t s_PostureItem1ItemIcons[POSTURE_ITEM_1_ITEM_COUNT] = {
    IMG_POSTURE_BACKREST_SMALL_ICON_DEFAULT, /* 0 坐姿1 */
    IMG_POSTURE_LINKAGE_SMALL_ICON_DEFAULT, /* 1 坐姿2 */
    IMG_POSTURE_CALF_SMALL_ICON_DEFAULT, /* 2 坐姿3 */
    IMG_POSTURE_FOOT_SMALL_ICON_DEFAULT, /* 3 坐姿4（占位复用） */
    IMG_COMMON_SMALL_BACK_BUTTON,            /* 4 返回主页 */
};

static const DisplayLayer PostureItem1_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景
    /* 中间大图：随【当前选中项】切换（52×52 水平居中），图号按游标查 s_PostureItem1LargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_POSTURE_BACKREST_LARGE_ICON_DEFAULT, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 选中态覆盖层：坐姿无数据点，选中态由组件本地"激活选中项"驱动 —— 短按激活某部件后，
       在中间大图之上叠加该部件选中态大图；浏览态整层不画。 */
    {LAYER_TYPE_IMAGE, 0, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON_SELECTED, 0, 0},

    /* 中间标题：当前选中项的名字，位置取【菜单页基准】(70,155) */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* ---- 4 个子菜单项 + 1 个返回主页按键 = 圆底(动态，先画) + 小图标(动态查表，后画) ---- */

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

static const PageEventMap PostureItem1_Events[] = {
    /* 旋转统一走回调：浏览态移游标、调节态发升降命令（由回调按运行时状态分流） */
    {S0553_KEY_ROTATE_CW,      {ACTION_EXECUTE_CALLBACK, 0, 0, PostureItem1_OnRotateCW}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_EXECUTE_CALLBACK, 0, 0, PostureItem1_OnRotateCCW}},
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, PostureItem1_OnShortPress}},
};

const PageConfig PostureItem1_Page = {
    .PageType       = S0553_DISP_TYPE_POSTURE_ITEM_1,
    .Layers         = PostureItem1_Layers,
    .LayerCount     = sizeof(PostureItem1_Layers) / sizeof(DisplayLayer),
    .EventMaps      = PostureItem1_Events,
    .EventCount     = sizeof(PostureItem1_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_NONE,
    .UiItemCount    = POSTURE_ITEM_1_ITEM_COUNT,
    .ItemNames      = s_PostureItem1ItemNames,
    .ItemLargeIcons = s_PostureItem1LargeIcons,  // 中间大图表（下标 = 游标，与标题同序）
    .ItemLargeIconsSelected = s_PostureItem1SelectedLargeIcons,  // 中间大图表-选中态（下标 = 游标）
    .ItemIcons      = s_PostureItem1ItemIcons,   // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter        = PostureItem1_OnEnter,
    .OnTick         = PostureItem1_OnTick,
};

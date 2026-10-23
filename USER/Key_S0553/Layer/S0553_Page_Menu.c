/***************************************************************************************************
* @file      S0553_Page_Menu.c
* @brief     菜单页面（MENU）：UI 图层 + 事件 + 页面描述符
* @details   菜单是【纯 UI 页】：旋转只移动本页的"选中游标"（组件本地 UI 状态），
*            不向主控下发任何命令、不占数据点；主控完全不知道当前高亮在哪一项。
*
*            选中态的呈现方式：
*             1) 页面【中间】一层大图标（ImgResId=IMGRES_ITEM_LARGE_ICON 动态层），
*                随游标切换 s_MenuLargeIcons 表里对应的那张大图（表由本页自持）；
*             2) 每个菜单项再配一层"圆底"（ImgResId 动态层，画在图标下层）
*              · 该项被选中 → IMG_COMMON_ROUND_ICON_SEL_BG（高亮圆底）
*              · 未被选中   → IMG_COMMON_ROUND_ICON_BG   （默认圆底）
*            底部三个菜单项小图标是静态层，始终都在；变化的只有中间大图与高亮圆底。
*            背景是静态层，每次整页重发时内容不变，故视觉上"背景不变、只有高亮圆底在动"。
*
* @note      旋转改变的是【UI 游标】，不是业务数据：不下发主控、不占数据点。
*            短按 = 按当前游标进入对应页面（压栈）；长按 = 返回进入菜单前的页面。
* @date      2026-10-21
==================================================================================================*/
#include "S0553_Pages.h"
#include "S0553_Display.h"     /* S0553Display_GetUiCursor() / S0553Display_PushPage() */
#include "S0553_ImageTable.h"

/* 菜单项数量（= PageConfig.UiItemCount）：手动程序 / 自动程序 / 坐姿 / 返回主页 */
#define MENU_ITEM_COUNT  4

/* 各菜单项【短按】进入的目标页：下标 = 图层 Index = UI 游标值，须与上方顺序一致。
   新增菜单项时在这里加一行即可，去向逻辑本身不用改。 */
static const S0553DisplayType s_MenuTargets[MENU_ITEM_COUNT] = {
    S0553_DISP_TYPE_MANUAL,     /* 0: 手动程序 */
    S0553_DISP_TYPE_AUTO,       /* 1: 自动程序 */
    S0553_DISP_TYPE_POSTURE,    /* 2: 坐姿     */
    S0553_DISP_TYPE_MAIN,    /* 3: 运行主页     */
};

/* 子菜单项名字表 [项序][语言]，语言序 = S0553_LangId（CN / EN / DE / FR）。
   下标须与 s_MenuTargets、图层 Index 一致；中间标题即取此表。 */
static const char* s_MenuItemNames[MENU_ITEM_COUNT - 1][S0553_LANG_MAX] = {
    /* 0 */ { "手动程序 ", "Manual Program",         "Manuelles Programm",      "Programme Manuel" },
    /* 1 */ { "自动程序", "Auto Program",           "Automatisches Programm",  "Programme Auto" },
    /* 2 */ { "坐姿调整",     "Posture",                "Sitzposition",            "Posture" },
};

/**
 * @brief 短按：进入【当前选中项】对应的页面
 * @details 【第一类：组件自治】纯 UI 导航：不下发主控、不等待回推、立即生效。
 *          去向由【UI 游标】在运行时决定，故用回调，而不是固定 Param1 的 ACTION_PUSH_PAGE。
 *          压栈进入 → 目标页用 ACTION_BACK 即可回到菜单（游标仍停在原处）。
 */
static void Menu_OnShortPress(void)
{
    /* 列表页通用跳转入口：普通项压栈进入；目标为"主页"时【清栈】后切换，
       否则层层压栈后从主页按返回会一步步退回刚才没走完的子菜单。 */
    S0553Display_EnterTargetPage(s_MenuTargets, MENU_ITEM_COUNT);
}

/* 中间大图表 [项序]：与 s_MenuItemNames 同序 —— 旋转到哪一项，中间就显示那一项的大图。
   下标 = UI 游标 = 项序；末项（返回主页）用返回按钮大图。
   本表由页面自持，故换图只需改这里，不必动引擎与枚举。 */
static const uint16_t s_MenuLargeIcons[MENU_ITEM_COUNT] = {
    IMG_MENU_MANUAL_LARGE_PAGE_ICON,      /* 0 手动程序 */
    IMG_MENU_AUTO_LARGE_PAGE_ICON,        /* 1 自动程序 */
    IMG_MENU_POSTURE_LARGE_PAGE_ICON,     /* 2 坐姿 */
    IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT, /* 3 返回主页 */
};

/* 小图标表 [项序]：与 s_MenuItemNames 同序 —— 手动/自动/坐姿各一张小图标，
   末项（返回主页）用返回按钮小图标。 */
static const uint16_t s_MenuItemIcons[MENU_ITEM_COUNT] = {
    IMG_MENU_MANUAL_SMALL_PAGE_ICON,      /* 0 手动程序 */
    IMG_MENU_AUTO_SMALL_PAGE_ICON,        /* 1 自动程序 */
    IMG_MENU_POSTURE_SMALL_PAGE_ICON,     /* 2 坐姿 */
    IMG_COMMON_SMALL_BACK_BUTTON,         /* 3 返回主页 */
};

static const DisplayLayer Menu_Layers[] = {
    {LAYER_TYPE_IMAGE, IMG_COMMON_MIDDLE_PANEL_BG, 0, 0, 0, 0, 0, 0},   // 背景

    /* 中间大图：随【当前选中项】切换（52×52 水平居中，左上角 (94,65)），
       画在背景之上、圆底之前；图号按游标查 s_MenuLargeIcons。 */
    {LAYER_TYPE_IMAGE, IMG_MENU_MANUAL_LARGE_PAGE_ICON, 94, 65, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_LARGE_ICON},

    /* 中间标题：当前选中子菜单项的名字（随 UI 游标变化，按当前语言取词）
       【本页是各列表页标题位置的基准】(70,155)，24 号字。 */
    {LAYER_TYPE_TEXT, 0xFFFF, 70, 155, 0, 0, 24, 0, NULL, TEXTRES_CURSOR_ITEM_NAME},

    /* 每个菜单项 =【圆底层(动态，先画在下层) + 图标层(静态，后画在上层)】
       圆底层用【通用】IMGRES_ITEM_STATE：选中→AltId，未选中→Id；
       第几项由末位 Index 指定——新增菜单项只需照抄一行改 Index，不用动 ImgResId 枚举。

       坐标按【弧线布局规范】（见 S0553_UI_Config.h）：N=4，弦长 36（间隙 12px），
       圆底圆心 (68,198) (102,210) (138,210) (172,198)，左上角 = 圆心-12；图标 +5 居中。 */
    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  56, 186, 0, 0, 0, 0, NULL, 0,
     IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 0},
    {LAYER_TYPE_IMAGE, 0, 61, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 0},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG,  90, 198, 0, 0, 0, 0, NULL, 0,
     IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 1},
    {LAYER_TYPE_IMAGE, 0, 95, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 1},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 126, 198, 0, 0, 0, 0, NULL, 0,
     IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 2},
    {LAYER_TYPE_IMAGE, 0, 131, 203, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 2},

    {LAYER_TYPE_IMAGE, IMG_COMMON_ROUND_ICON_BG, 160, 186, 0, 0, 0, 0, NULL, 0,
     IMGRES_ITEM_STATE, IMG_COMMON_ROUND_ICON_SEL_BG, 3},
    {LAYER_TYPE_IMAGE, 0, 165, 191, 0, 0, 0, 0, NULL, 0, IMGRES_ITEM_ICON, 0, 3},
};

static const PageEventMap Menu_Events[] = {
    /* 旋转：移动本页选中游标（【第一类：组件自治】不经主控、不等待回推、立即生效）
       Param1 = +1 / -1，引擎会在 UiItemCount 内循环。 */
    {S0553_KEY_ROTATE_CW,      {ACTION_MOVE_CURSOR, +1, 0, NULL}},
    {S0553_KEY_ROTATE_CCW,     {ACTION_MOVE_CURSOR, -1, 0, NULL}},
    /* 短按：进入【当前选中项】对应的页面（去向由 UI 游标决定，见 Menu_OnShortPress） */
    {S0553_KEY_PHYSICAL_SHORT, {ACTION_EXECUTE_CALLBACK, 0, 0, Menu_OnShortPress}},
};

const PageConfig Menu_Page = {
    .PageType     = S0553_DISP_TYPE_MENU,
    .Layers       = Menu_Layers,
    .LayerCount   = sizeof(Menu_Layers) / sizeof(DisplayLayer),
    .EventMaps    = Menu_Events,
    .EventCount   = sizeof(Menu_Events) / sizeof(PageEventMap),
    .ValueDataPoint = S0553_DP_NONE,   // 纯 UI 页：选中项是组件本地游标，不占主控数据点
    .UiItemCount  = MENU_ITEM_COUNT,   // 4 个可选项，游标 0/1/2/3（末项为"返回主页"）
    .ItemNames    = s_MenuItemNames,   // 中间标题取词表（下标 = 游标）
    .ItemLargeIcons = s_MenuLargeIcons, // 中间大图表（下标 = 游标，与标题同序）
    .ItemIcons      = s_MenuItemIcons,  // 小图标表（下标 = 图层 Index，与标题同序）
    .OnEnter      = NULL,
};

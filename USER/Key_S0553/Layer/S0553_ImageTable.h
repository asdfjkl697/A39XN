/***************************************************************************************************
* @file      S0553_ImageTable.h
* @brief     旋钮屏图片编号表（固件烧录顺序 ↔ 代码引用 的唯一映射）
* @details   【核心原则】编号 = 从机 Flash 中预存图片的全局唯一索引（烧录顺序/地址），
*            而本文件里的编号【不是手写的】，是由"烧录顺序列表"自动推出的：
*
*                列表中的行序  ==  固件烧录顺序  ==  图片编号（0 起自动递增）
*
*            因此"改图片数量/位置"的正确做法是：
*              · 新增图片 → 在列表对应位置【加一行】，其后编号自动 +1
*              · 删除图片 → 【删一行】，其后编号自动 -1
*              · 调整顺序 → 【移动一行】，两处编号自动互换
*            无需再手工改任何数字，也不会出现"改一处漏改一片"。
*
*            "复用"指多个页面引用同一编号（同一张图只烧录一次）。
*
* 规则：
*  1. 列表顺序必须与固件烧录工具的 "图片文件 ↔ 编号" 映射表严格一致；改顺序需同步固件，
*     否则从机取到错误/空缺的图片。
*  2. 复用省的是 Flash 存储，不是通信量：从机不跨页缓存图层，每次切页都要把该页
*     所有图重新填进帧缓冲，复用图也会再发一次（见 S0553_Display.c 的批量刷图）。
*  3. 需要预留空号（未来扩展/对齐区间）时，在列表中插入 X(IMG_RESERVED_xxx) 占位行。
*
* @note      本文件只有宏与一个枚举类型，不含任何运行时数据；被 UI_Config.c 引用。
* @date      2026-09-11
* @date      2026-10-21  改为 X-macro 单一列表：编号由顺序自动推导，不再手写绝对编号。
==================================================================================================*/
#ifndef S0553_IMAGE_TABLE_H_
#define S0553_IMAGE_TABLE_H_

/* ================================================================================================
 * 图片烧录顺序表【唯一事实来源】
 * -----------------------------------------------------------------------------------------------
 * · 一行的位置 = 一个烧录槽位；新增/删除/移动图片请只改这里。
 * · 行末注释仅作说明，不影响编号。
 * · 下面的 X 是占位宏名（IMG_X），由使用方在展开时临时定义并立即 #undef，不污染外部。
 * ============================================================================================== */
#define S0553_IMAGE_LIST \
    /* ---- 公共资源（多页复用） ---- */                        \
    IMG_X(IMG_COMMON_LEFT_PANEL_BG)                            \
    IMG_X(IMG_COMMON_MIDDLE_PANEL_BG)                          \
    IMG_X(IMG_COMMON_ROUND_ICON_SEL_BG)                        \
    IMG_X(IMG_COMMON_ROUND_ICON_BG)                            \
    IMG_X(IMG_COMMON_SMALL_BACK_BUTTON)                        \
    IMG_X(IMG_COMMON_LARGE_BACK_BUTTON_SELECTED)               \
    IMG_X(IMG_COMMON_LARGE_BACK_BUTTON_DEFAULT)                \
    IMG_X(IMG_COMMON_PROGRESS_BG)                              \
    /* ---- Logo / 开机 ---- */                                \
    IMG_X(IMG_COMMON_BLACK_BG)                                 \
    IMG_X(IMG_LOGO_EN_WITH_BG)                                 \
    IMG_X(IMG_LOGO_BRAND_NO_BG)                                \
    IMG_X(IMG_LOGO_EN_NO_BG)                                   \
    /* ---- 主页 ---- */                                       \
    IMG_X(IMG_MAIN_BG)                                         \
    /* 主页 自动程序图标（1..20，编号连续，供动态图标按序号取图） */  \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_1)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_2)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_3)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_4)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_5)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_6)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_7)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_8)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_9)                        \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_10)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_11)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_12)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_13)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_14)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_15)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_16)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_17)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_18)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_19)                       \
    IMG_X(IMG_MAIN_AUTO_PROGRAM_ICON_20)                       \
    /* 手动手法图标（1..12，编号连续） */                       \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_1)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_2)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_3)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_4)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_5)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_6)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_7)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_8)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_9)                      \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_10)                     \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_11)                     \
    IMG_X(IMG_MAIN_MANUAL_PROGRAM_ICON_12)                     \
    /* ---- 菜单页 ---- */                                     \
    IMG_X(IMG_MENU_MANUAL_LARGE_PAGE_ICON)                           \
    IMG_X(IMG_MENU_AUTO_LARGE_PAGE_ICON)                             \
    IMG_X(IMG_MENU_POSTURE_LARGE_PAGE_ICON)                          \
    IMG_X(IMG_MENU_MANUAL_SMALL_PAGE_ICON)                           \
    IMG_X(IMG_MENU_AUTO_SMALL_PAGE_ICON)                             \
    IMG_X(IMG_MENU_POSTURE_SMALL_PAGE_ICON)                          \
    /* ---- 自动程序页：大图标（选中/默认 成对，1..20） ---- */  \
    IMG_X(IMG_AUTO_LARGE_ICON_1_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_1_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_2_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_2_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_3_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_3_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_4_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_4_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_5_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_5_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_6_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_6_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_7_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_7_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_8_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_8_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_9_SELECTED)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_9_DEFAULT)                       \
    IMG_X(IMG_AUTO_LARGE_ICON_10_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_10_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_11_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_11_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_12_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_12_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_13_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_13_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_14_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_14_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_15_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_15_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_16_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_16_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_17_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_17_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_18_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_18_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_19_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_19_DEFAULT)                      \
    IMG_X(IMG_AUTO_LARGE_ICON_20_SELECTED)                     \
    IMG_X(IMG_AUTO_LARGE_ICON_20_DEFAULT)                      \
    /* ---- 自动程序页：小图标（默认/选中 成对，1..20） ---- */  \
    IMG_X(IMG_AUTO_SMALL_ICON_1_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_1_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_2_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_2_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_3_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_3_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_4_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_4_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_5_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_5_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_6_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_6_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_7_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_7_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_8_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_8_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_9_DEFAULT)                       \
    IMG_X(IMG_AUTO_SMALL_ICON_9_SELECTED)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_10_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_10_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_11_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_11_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_12_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_12_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_13_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_13_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_14_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_14_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_15_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_15_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_16_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_16_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_17_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_17_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_18_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_18_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_19_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_19_SELECTED)                     \
    IMG_X(IMG_AUTO_SMALL_ICON_20_DEFAULT)                      \
    IMG_X(IMG_AUTO_SMALL_ICON_20_SELECTED)                     \
    IMG_X(IMG_AUTO_TITLE_LARGE_ICON_1)                     \
    IMG_X(IMG_AUTO_TITLE_LARGE_ICON_2)                     \
    IMG_X(IMG_AUTO_TITLE_LARGE_ICON_3)                     \
    IMG_X(IMG_AUTO_TITLE_LARGE_ICON_4)                     \
    IMG_X(IMG_AUTO_TITLE_SMALL_ICON_1)                     \
    IMG_X(IMG_AUTO_TITLE_SMALL_ICON_2)                     \
    IMG_X(IMG_AUTO_TITLE_SMALL_ICON_3)                     \
    IMG_X(IMG_AUTO_TITLE_SMALL_ICON_4)                     \
    /* ---- 手动手法页：大图标（【选中在前、默认在后】，1..12） ---- */ \
    IMG_X(IMG_MANUAL_LARGE_ICON_1_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_1_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_2_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_2_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_3_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_3_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_4_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_4_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_5_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_5_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_6_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_6_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_7_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_7_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_8_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_8_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_9_SELECTED)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_9_DEFAULT)                     \
    IMG_X(IMG_MANUAL_LARGE_ICON_10_SELECTED)                   \
    IMG_X(IMG_MANUAL_LARGE_ICON_10_DEFAULT)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_11_SELECTED)                   \
    IMG_X(IMG_MANUAL_LARGE_ICON_11_DEFAULT)                    \
    IMG_X(IMG_MANUAL_LARGE_ICON_12_SELECTED)                   \
    IMG_X(IMG_MANUAL_LARGE_ICON_12_DEFAULT)                    \
    /* ---- 手动手法页：小图标（【默认在前、选中在后】，1..12） ---- */ \
    IMG_X(IMG_MANUAL_SMALL_ICON_1_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_1_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_2_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_2_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_3_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_3_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_4_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_4_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_5_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_5_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_6_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_6_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_7_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_7_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_8_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_8_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_9_DEFAULT)                     \
    IMG_X(IMG_MANUAL_SMALL_ICON_9_SELECTED)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_10_DEFAULT)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_10_SELECTED)                   \
    IMG_X(IMG_MANUAL_SMALL_ICON_11_DEFAULT)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_11_SELECTED)                   \
    IMG_X(IMG_MANUAL_SMALL_ICON_12_DEFAULT)                    \
    IMG_X(IMG_MANUAL_SMALL_ICON_12_SELECTED)                   \
    IMG_X(IMG_MANUAL_TITLE_LARGE_ICON_1)                     \
    IMG_X(IMG_MANUAL_TITLE_LARGE_ICON_2)                     \
    IMG_X(IMG_MANUAL_TITLE_SMALL_ICON_1)                     \
    IMG_X(IMG_MANUAL_TITLE_SMALL_ICON_2)                     \
    /* ---- 坐姿调节页：大图标 ---- */                          \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_1_SELECTED)            \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_1_DEFAULT)             \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_2_SELECTED)            \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_2_DEFAULT)             \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_3_SELECTED)            \
    IMG_X(IMG_POSTURE_ZERO_G_LARGE_ICON_3_DEFAULT)             \
    IMG_X(IMG_POSTURE_BACKREST_LARGE_ICON_SELECTED)            \
    IMG_X(IMG_POSTURE_BACKREST_LARGE_ICON_DEFAULT)             \
    IMG_X(IMG_POSTURE_LINKAGE_LARGE_ICON_SELECTED)             \
    IMG_X(IMG_POSTURE_LINKAGE_LARGE_ICON_DEFAULT)              \
    IMG_X(IMG_POSTURE_CALF_LARGE_ICON_SELECTED)                \
    IMG_X(IMG_POSTURE_CALF_LARGE_ICON_DEFAULT)                 \
    IMG_X(IMG_POSTURE_FOOT_LARGE_ICON_SELECTED)                \
    IMG_X(IMG_POSTURE_FOOT_LARGE_ICON_DEFAULT)                 \
    /* ---- 坐姿调节页：小图标 ---- */                          \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_1_SELECTED)            \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_1_DEFAULT)             \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_2_DEFAULT)             \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_2_SELECTED)            \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_3_DEFAULT)             \
    IMG_X(IMG_POSTURE_ZERO_G_SMALL_ICON_3_SELECTED)            \
    IMG_X(IMG_POSTURE_BACKREST_SMALL_ICON_DEFAULT)             \
    IMG_X(IMG_POSTURE_BACKREST_SMALL_ICON_SELECTED)            \
    IMG_X(IMG_POSTURE_LINKAGE_SMALL_ICON_DEFAULT)              \
    IMG_X(IMG_POSTURE_LINKAGE_SMALL_ICON_SELECTED)             \
    IMG_X(IMG_POSTURE_CALF_SMALL_ICON_DEFAULT)                 \
    IMG_X(IMG_POSTURE_CALF_SMALL_ICON_SELECTED)                \
    IMG_X(IMG_POSTURE_FOOT_SMALL_ICON_DEFAULT)                  \
    IMG_X(IMG_POSTURE_FOOT_SMALL_ICON_SELECTED)                

/* ================================================================================================
 * 编号展开：值 = 列表行序（0 起自动递增）
 * —— 不要在这里手写数字。
 * ============================================================================================== */
typedef enum
{
#define IMG_X(name)  name,
    S0553_IMAGE_LIST
#undef IMG_X
    IMG_ID_COUNT        /**< 图片总数（自动得出；增删图片后需核对下方 S0553_IMG_TOTAL 断言） */
} S0553ImageId;

/* 注：本表【不做别名】——每张图占一个独立烧录槽位，一行的位置就是一个编号。
   若两个语义名指向同一张图，说明固件里本就是两张（或复用），请在列表中如实各占一行，
   不要用一个名字去 #define 另一个，否则编号与烧录表对不上。 */

/* ================================================================================================
 * 数量：由【相邻段边界】相减得出，改了图标条数会自动跟着变，无需手工同步
 * 自动程序数、手动程序数
 * ============================================================================================== */
#define IMG_MAIN_AUTO_ICON_COUNT \
    (IMG_MAIN_MANUAL_PROGRAM_ICON_1 - IMG_MAIN_AUTO_PROGRAM_ICON_1)    /* = 20 */
#define IMG_MAIN_MANUAL_ICON_COUNT \
    (IMG_MENU_MANUAL_LARGE_PAGE_ICON - IMG_MAIN_MANUAL_PROGRAM_ICON_1)  /* = 12 */

/* ================================================================================================
 * 编译期自检（条件不成立则编译失败，避免"改了列表忘了同步其它地方"）
 * ============================================================================================== */
#define S0553_IMG_CAT_(a, b)  a##b
#define S0553_IMG_CAT(a, b)   S0553_IMG_CAT_(a, b)
#define S0553_IMG_ASSERT(cond)  typedef char S0553_IMG_CAT(s0553_img_chk_, __LINE__)[(cond) ? 1 : -1]

/* 烧录总数：增删图片后【必须同步固件烧录表】，故强制显式更新此数 */
#define S0553_IMG_TOTAL  219
S0553_IMG_ASSERT(IMG_ID_COUNT == S0553_IMG_TOTAL);

/* 动态图标依赖"编号连续"：序列若不连续（中间插入了别的图）会立即编译失败 */
S0553_IMG_ASSERT((IMG_MAIN_AUTO_PROGRAM_ICON_20 - IMG_MAIN_AUTO_PROGRAM_ICON_1 + 1) == IMG_MAIN_AUTO_ICON_COUNT);
S0553_IMG_ASSERT((IMG_MAIN_MANUAL_PROGRAM_ICON_12 - IMG_MAIN_MANUAL_PROGRAM_ICON_1 + 1) == IMG_MAIN_MANUAL_ICON_COUNT);

/* 末项编号 = 总数 - 1（防止列表尾部被误插/误删） */
S0553_IMG_ASSERT(IMG_POSTURE_FOOT_SMALL_ICON_SELECTED == (S0553_IMG_TOTAL - 1));

#endif /* S0553_IMAGE_TABLE_H_ */

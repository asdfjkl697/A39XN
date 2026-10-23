/***************************************************************************************************
* @file      S0553_PageList.h
* @brief     页面单一注册清单（新增页面只改此文件 + 新建一个页面文件）
* @details   本文件是本工程页面的唯一真相源，S0553_PAGE_LIST(X) 被多处展开：
*            - S0553_Protocol.h：生成 S0553DisplayType 枚举（S0553_DISP_TYPE_<ID>）
*            - S0553_Pages.h：生成各页描述符 <SYM>_Page 的 extern 声明
*            - S0553_UI_Config.c：生成 UI_PAGE_MAP 指针表
*
* 参数：X(ID, SYM)
*   ID   页面标识，生成枚举 S0553_DISP_TYPE_<ID>
*   SYM  页面符号前缀，绑定该页的描述符 <SYM>_Page（图层/事件/钩子/数值范围都在其中）
*
* 为什么是"每页一个描述符"：图层数组的 sizeof 只能在定义它的编译单元内计算，
* 跨文件 extern const X[] 是不完整类型、无法 sizeof。故每页在自己文件里把
* LayerCount/EventCount 算好并打包成 <SYM>_Page，中心表只存指针。
*
* 规则：
*   1. 新增页面只在此加一行，并新建 S0553_Page_<SYM>.c，在其中定义
*      static const DisplayLayer <SYM>_Layers[]（可选 <SYM>_Events[] 与 OnEnter 钩子）
*      以及对外唯一的 const PageConfig <SYM>_Page。
*   2. NONE 必须保持首位（=0，表示无页面/黑屏）；MAX 由 Protocol.h 自动追加，勿写入本表。
*   3. UI_PAGE_MAP 由本清单自动生成，勿手工编辑；条目数与 MAX 不一致会触发编译期断言。
*
* @date      2026-10-20
================================================================***********************************/
#ifndef S0553_PAGE_LIST_H_
#define S0553_PAGE_LIST_H_

/*   ID        符号         用途 */
#define S0553_PAGE_LIST(X)                  \
    X(NONE,     None)                       /* 无显示/黑屏页（必须=0，保持首位） */ \
    X(LOGO,     Logo)                       /* 开机 LOGO 页 */ \
    X(MAIN,     MainPage)                   /* 主页面 */ \
    X(MENU,     Menu)                       /* 菜单页面（旋转改选中项） */ \
    X(MANUAL,   Manual)                     /* 菜单项 0：手动程序页（菜单短按进入） */ \
    X(AUTO,     Auto)                       /* 菜单项 1：自动程序页（菜单短按进入） */ \
    X(POSTURE,  Posture)                    /* 菜单项 2：坐姿页（菜单短按进入） */ \
    X(AUTO_PROGRAM_1, AutoProgram1)         /* 自动程序页 第 1 项对应的选择页 */ \
    X(AUTO_PROGRAM_2, AutoProgram2)         /* 自动程序页 第 2 项对应的选择页 */ \
    X(AUTO_PROGRAM_3, AutoProgram3)         /* 自动程序页 第 3 项对应的选择页 */ \
    X(AUTO_PROGRAM_4, AutoProgram4)         /* 自动程序页 第 4 项对应的选择页 */ \
    X(MANUAL_ITEM_1,  ManualItem1)          /* 手动程序页 第 1 项对应的选择页 */ \
    X(MANUAL_ITEM_2,  ManualItem2)          /* 手动程序页 第 2 项对应的选择页 */ \
    X(POSTURE_ITEM_1, PostureItem1)         /* 坐姿页 第 1 项对应的选择页 */ \
    X(POSTURE_ITEM_2, PostureItem2)         /* 坐姿页 第 2 项对应的选择页 */

#endif /* S0553_PAGE_LIST_H_ */

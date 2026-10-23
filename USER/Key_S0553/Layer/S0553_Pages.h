/***************************************************************************************************
* @file      S0553_Pages.h
* @brief     各页面描述符 <SYM>_Page 的 extern 声明（由 S0553_PAGE_LIST 自动生成）
* @details   每个页面在自己的 S0553_Page_<SYM>.c 里把图层/事件/钩子/数值范围打包成一个
*            const PageConfig <SYM>_Page（LayerCount/EventCount 在该文件内用 sizeof 算好），
*            本文件按清单统一声明这些描述符，供 S0553_UI_Config.c 生成指针表。
*            【新增页面】只需：① 在 S0553_PageList.h 加一行；② 新建 S0553_Page_<SYM>.c。
*            本文件无需改动（声明自动跟随清单）。
* @date      2026-10-20
================================================================***********************************/
#ifndef S0553_PAGES_H_
#define S0553_PAGES_H_

#include "S0553_UI_Config.h"   /* PageConfig / DisplayLayer / PageEventMap / S0553PageEnterHook */
#include "S0553_PageList.h"    /* S0553_PAGE_LIST */

/* 每个页面对外只暴露一个描述符 */
#define S0553_PAGE_EXTERN(id, sym)  extern const PageConfig sym##_Page;

S0553_PAGE_LIST(S0553_PAGE_EXTERN)

#undef S0553_PAGE_EXTERN

#endif /* S0553_PAGES_H_ */

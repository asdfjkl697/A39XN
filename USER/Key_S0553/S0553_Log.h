/***************************************************************************************************
* @file      S0553_Log.h
* @brief     组件内部调试日志开关（【零平台依赖】—— 默认关闭）
* @details   MCU 移植【无需任何动作】：不定义 S0553_ENABLE_LOG 时，
*            LOG_DEBUG / LOG_ERROR 一律展开为 ((void)0)：
*              - 不引用 printf     ⇒ 无需串口重定向（retarget）；
*              - 不引用任何 HAL 函数 ⇒ 无需实现 S0553_HAL_GetTickMs()（其声明也被
*                                      S0553_ENABLE_LOG 包裹，未开启日志时根本不存在）；
*              - 不产生任何代码与数据，零存储与运行开销。
*
*            仅在【需要调试跟踪】时定义 S0553_ENABLE_LOG（如 PC 上的联调工程），
*            此时帧日志输出到 stderr（与业务日志的 stdout 分离，便于双终端观察），
*            并需要平台提供 S0553_HAL_GetTickMs() 作为时间戳来源
*            （见 S0553_HAL.h —— 该函数声明同样被此宏包裹）。
*            MCU 量产不定义此宏，故 printf 与 GetTickMs 二者都不需要。
*
*            S0553_LOG_ENABLED：供组件内按条件编译【仅日志使用】的辅助代码
*            （如时间戳格式化函数），避免日志关闭时报"未使用"告警。
*
* @note      日志关闭时，宏的【参数表达式不会被执行】，故勿在其中放入有副作用的调用。
* @date      2026-10-22
==================================================================================================*/
#ifndef S0553_LOG_H_
#define S0553_LOG_H_

#ifdef S0553_ENABLE_LOG
    #include <stdio.h>

    /** @brief 日志是否启用（1=启用；0=关闭，为默认值） */
    #define S0553_LOG_ENABLED   1

    /** @brief 调试日志（仅定义 S0553_ENABLE_LOG 时输出；走 stderr 与业务日志 stdout 分离） */
    #define LOG_DEBUG(...)      fprintf(stderr, __VA_ARGS__)

    /** @brief 错误日志（仅定义 S0553_ENABLE_LOG 时输出；走 stderr） */
    #define LOG_ERROR(...)      fprintf(stderr, __VA_ARGS__)
#else
    /** @brief 日志是否启用（1=启用；0=关闭，为默认值） */
    #define S0553_LOG_ENABLED   0

    /** @brief 调试日志（默认关闭，展开为空） */
    #define LOG_DEBUG(...)      ((void)0)

    /** @brief 错误日志（默认关闭，展开为空） */
    #define LOG_ERROR(...)      ((void)0)
#endif /* S0553_ENABLE_LOG */

#endif /* S0553_LOG_H_ */

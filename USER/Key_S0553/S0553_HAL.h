/***************************************************************************************************
* @file      S0553_HAL.h
* @brief     S0553 组件硬件抽象层（HAL）—— 平台服务接口（纯声明，无实现）
* @details   本头文件是 Key_S0553 组件代码与"所在平台"之间唯一的依赖契约。
*
*            组件代码（S0553_main.c / S0553_Display.c / ...）只允许通过本文件声明的
*            接口访问平台资源，绝不直接调用 OS / 厂商 SDK / 编译内置 API
*           （Windows API、clock/Sleep、__disable_irq、printf 等）。
*
*            移植到新平台 = 按本文件契约实现一份驱动文件并参与编译即可，
*            组件源码无需任何改动：
*              - MCU 平台：自建 hal_mcu_xxx.c（UART 中断收发 + SysTick/RTOS 延时 +
*                           临界区关中断/调度器锁；单任务裸机下临界区可空实现）
*
*            【本组件实际消费的平台资源】（只抽象"用得到的"，避免伪通用）：
*              1. UART 字节流      —— Init/RxAvailable/ReadByte/WriteByte（唯一外设依赖）【必需】
*              2. 单调毫秒时基      —— S0553_HAL_GetTickMs()【★仅 PC 调试；声明本身也被
*                                      S0553_ENABLE_LOG 包裹，MCU 未开启日志时根本看不到】
*              3. 临界区            —— Enter/ExitCritical【单任务裸机【空实现】即可】
*
*            【MCU 最小移植集：真正要写的只有 4 个函数 + 2 个空函数】
*              组件日志【默认全关】（见 S0553_Log.h）：不定义 S0553_ENABLE_LOG 时，
*              LOG_DEBUG / LOG_ERROR 一律展开为 ((void)0)，零代码、零开销，因此：
*                - 【不需要】printf 重定向（retarget）；
*                - 【不需要】实现 S0553_HAL_GetTickMs() —— 其声明被同一宏包裹，
*                  未开启日志时本文件中【不存在】该函数，MCU 侧完全不必实现；
*                - 临界区在【单任务裸机】下写两个空函数即可（RTOS/多中断源才需真正实现）。
*              ⇒ 需要真写的：4 个 UART 函数；外加 2 个空函数（临界区）。
*              ⇒ MCU 移植【无需定义任何宏】；S0553_ENABLE_LOG 仅供 PC 调试工程使用。
*
*            GPIO / I2C / SPI / Timer / 动态内存等外设本组件当前不消费，故不在此预定义
*            空接口（避免"看似通用实则无人调用"的抽象负债）。未来组件若新增外设依赖，
*            按尾部【外设接口扩展指引】以相同模式追加声明即可。
*
* @date      2026-09-04
==================================================================================================*/
#ifndef S0553_HAL_H_
#define S0553_HAL_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== 通用返回值约定 ==================== */
#define S0553_HAL_OK    0u  /**< 操作成功 */
#define S0553_HAL_ERROR 1u  /**< 操作失败 */

/* ==================== 1. UART 字节流 ==================== */
/* 平台侧可用 1 个串口（或任意字节流通道，如模拟串口/DMA 回环）实现下述接口：
 *   - UartInit    ：保证串口就绪（MCU：开时钟+引脚复用+中断；PC：打开 COM 句柄）。
 *                    返回 S0553_HAL_OK / S0553_HAL_ERROR。
 *   - RxAvailable ：当前接收缓冲中可读字节数（无则返回 0），被协议解析轮询调用；
 *                    内部应做好与接收中断/线程的同步（关闭中断或上锁取数）。
 *   - ReadByte    ：取出 1 字节；平台保证仅在 RxAvailable()>0 时被调用，无数据
 *                    时返回 0xFF 兜底亦可。
 *   - WriteByte   ：兼容接口，发送 1 字节；平台不得因 TX 队列满而无限等待。
 *   - WriteFrame  ：原子写入完整帧；空间不足或参数非法时返回 S0553_HAL_ERROR，
 *                    不得部分入队或阻塞。
 */
uint8_t  S0553_HAL_UartInit(void);
uint16_t S0553_HAL_UartRxAvailable(void);
uint8_t  S0553_HAL_UartReadByte(void);
void     S0553_HAL_UartWriteByte(uint8_t byte);
uint8_t  S0553_HAL_UartWriteFrame(const uint8_t *data, uint16_t len);

/* 调试计数：因参数非法或 TX 缓冲区空间不足而被拒绝的完整帧数。 */
extern volatile uint32_t S0553_TxFrameDropCount;

/* ==================== 2. 单调毫秒时基【★仅 PC 调试：MCU 不必实现】 ==================== */
#ifdef S0553_ENABLE_LOG
/* 自平台启动起单调递增的毫秒值，仅作【调试日志时间戳】，无需校准到绝对时间；
 * 建议用 64 位计数器避免溢出回绕（PC 用 GetTickCount64，MCU 用 SysTick 链）。
 *
 * 【MCU 移植不需要本函数】—— 连【声明】也被 S0553_ENABLE_LOG 包裹：未开启日志时
 * （默认，即 MCU 量产）本声明根本不存在，MCU 侧看不到、也不必实现。
 * 组件的所有功能计时（2s 探活窗口、2s fill 看门狗、Logo 过渡、10s 周期维护）
 * 均按【S0553_Task() 的 10ms 调用节拍】计数，【不依赖】毫秒时基。
 * 仅当定义了 S0553_ENABLE_LOG（需要调试跟踪时）才需实现。 */
uint32_t S0553_HAL_GetTickMs(void);
#endif /* S0553_ENABLE_LOG */

/* ==================== 3. 临界区【裸机通常可空实现】 ==================== */
/* 组件内 3 处临界区只保护【同一个对象】：内部待发队列 s_TxQueue（入队 / 清空 / 出队）。
 * 入队只发生在发送类 API（均供主循环调用），出队在 S0553_Process()（主循环调用）。
 *
 * 【可以空实现（裸机的常见情况）】
 *   只要满足：组件的 API【只在主循环里调用】——即 UART 中断里只做
 *   "收字节放进你自己的缓冲"，【不调用任何 S0553_ 开头的函数】。
 *   此时待发队列只有主循环一个访问者，不存在并发，写两个空函数 {} 即可。
 *
 * 【必须真正实现】
 *   - RTOS：S0553_Task() 与别的任务都会调组件 API；
 *   - 裸机但在【中断里】调了组件 API（不推荐这样用）。
 *   实现：关中断（或 taskENTER_CRITICAL），且【必须支持嵌套】——组件内临界区会出现
 *   嵌套，实现需配套计数器。
 *
 * ⚠️ 【组件保护不到】的地方：你自己的 UART 接收环形缓冲。
 *   若 ISR 写缓冲、而 S0553_HAL_UartRxAvailable() / UartReadByte() 在主循环读缓冲，
 *   这是跨上下文共享，本文件的临界区管不到（你的 ISR 不会调它）。
 *   请自行处理，推荐二选一：
 *     ① 单生产者单消费者环形缓冲：ISR 只改写索引、主循环只改读索引
 *        （一方只改一个变量 ⇒ 无需关中断）；
 *     ② 在这两个读函数里短暂关中断。
 *   否则会偶发丢字节 / 帧解析错乱，且极难复现。 */
void S0553_HAL_EnterCritical(void);
void S0553_HAL_ExitCritical(void);

/* 组件沿用旧宏名（DEBUG 与量产模式统一可用），避免大规模改写调用点 */
#define S0553_CRITICAL_ENTER()   S0553_HAL_EnterCritical()
#define S0553_CRITICAL_EXIT()    S0553_HAL_ExitCritical()

#ifdef __cplusplus
}
#endif

/* ==================== 【外设接口扩展指引】 ====================
 * 以下为"未来组件/主控需要更多外设"时的扩展范式（仅示例，勿照抄为死接口）：
 *
 *   // GPIO：如背光/电源使能脚
 *   void     S0553_HAL_GpioSet(uint8_t pin, bool level);
 *   bool     S0553_HAL_GpioGet(uint8_t pin);
 *   // 定时器：组件若需独立硬件定时节拍而非依赖外部周期调用
 *   void     S0553_HAL_TimerStart(uint32_t periodUs, void (*cb)(void));
 *   // I2C/SPI 主模式（扩展传感器/存储）
 *   uint8_t  S0553_HAL_I2C_Write(uint8_t addr, const uint8_t* buf, uint16_t len);
 *   uint8_t  S0553_HAL_SPI_Transfer(const uint8_t* tx, uint8_t* rx, uint16_t len);
 *
 * 约定：每个新接口 = "平台无关语义 + 返回值约定" 写清注释后加进本文件，
 *       平台实现文件补齐对应函数即可，组件内通过宏/函数调用，不直接碰寄存器。
 * 内存管理：本组件为纯静态分配（无 malloc/free/动态表），
 *       如需引入动态内存请优先改用静态池，保证嵌入式可预测性。
 * ==================================================================== */
#endif /* S0553_HAL_H_ */

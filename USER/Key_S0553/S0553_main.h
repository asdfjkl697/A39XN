/***************************************************************************************************
* @file      S0553_main.h
* @brief     S0553旋钮屏模块接口头文件
* @details   对外接口：初始化(S0553_Init)、周期任务(S0553_Task)、按键事件(S0553_PopKeyEvent)、
*            显示控制(ShowImage/ShowText 等)与状态查询(IsConnected/IsTxIdle)。
* @usage     1. 调用 S0553_Init() 初始化；2. 主循环周期调用 S0553_Task()（连接/刷新自治）。
* @note      所有接口函数均为非阻塞设计，可在实时系统中安全调用
* @date      2025-04-10
==================================================================================================*/
#ifndef S0553_MAIN_H_
#define S0553_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

// #include "define.h"
#include "S0553_Protocol.h"
#include "S0553_Display.h"

/* ==================== 配置宏 ==================== */
/** @brief 模块使能开关，定义为1时启用S0553模块功能 */
#define S0553_ENABLE                   1

/** 
 * @brief 连接超时计数阈值
 * @details 单位为S0553_Process()调用次数。假设每10ms调用一次，
 *          则超时时间 = 100 * 10ms = 1000ms = 1秒
 * @note 若超过此时间未收到从机响应，则认为连接失败
 */
#define S0553_CONNECT_TIMEOUT          100

/** 
 * @brief 填图操作超时计数阈值
 * @details 单位为S0553_Process()调用次数。假设每10ms调用一次，
 *          则超时时间 = 500 * 10ms = 5000ms = 5秒
 * @note 若超过此时间未收到填图完成通知，则认为填图失败
 */
#define S0553_FILL_IMAGE_TIMEOUT       500

/**
 * @brief 物理按键长按生效时间默认值（单位：毫秒）
 * @details 范围：1000-10000ms，对应协议命令 0x3A
 * @note 连接建立后会自动配置此值到从设备
 */
#define S0553_DEFAULT_PHYSICAL_LONGPRESS_TIME  3000

/**
 * @brief 触摸按键长按生效时间默认值（单位：毫秒）
 * @details 范围：500-10000ms，对应协议命令 0x3B
 * @note 连接建立后会自动配置此值到从设备
 */
#define S0553_DEFAULT_TOUCH_LONGPRESS_TIME     2000

/* ==================== 外部接口函数 ==================== */

/**
 * @brief 初始化S0553旋钮屏模块
 * @details 执行以下操作：
 *          - 清零接收/发送控制结构
 *          - 初始化UART通信端口
 *          - 初始化显示模块
 *          - 设置初始状态为IDLE
 * @note    必须在其他S0553接口函数调用前执行，且仅调用一次
 * @retval  无
 */
void S0553_Init(void);

/**
 * @brief S0553模块周期处理函数
 * @details 必须在主循环中周期性调用（建议10ms间隔），执行以下任务：
 *          - 解析UART接收数据，处理协议帧
 *          - 更新状态机，处理超时逻辑
 *          - 处理按键消抖和状态转换
 *          - 触发对应的功能回调（通过ChairKey框架）
 *          - 处理编码器旋转事件
 * @note    此函数为非阻塞设计，执行时间短，适合在实时系统中调用
 * @retval  无
 */
void S0553_Process(void);

/**
 * @brief 刷新S0553显示内容
 * @details 周期性调用此函数以维持显示内容的稳定性，内部实现包括：
 *          - 定期重发当前显示页面的图片数据
 *          - 防止因通信干扰导致的显示异常
 * @note    建议放在固定的低频时基（如1秒一次）中调用此函数
 * @retval  无
 */
void S0553_DisplayRefresh(void);

/**
 * @brief 查询S0553是否已成功连接
 * @details 检查当前模块状态是否为S0553_STATE_CONNECTED
 * @return  uint8_t - 连接状态
 *          - 1: 已连接，可以正常通信
 *          - 0: 未连接或连接中断
 * @note    在执行显示控制等操作前，建议先检查此状态
 */
uint8_t S0553_IsConnected(void);
/** @brief 查询发送侧是否空闲（待发队列空且无在飞刷图任务），用于延迟刷新节流 */
uint8_t S0553_IsTxIdle(void);

/**
 * @brief 从事件队列中取出一个事件（FIFO）
 * @details 从队列头部取出最早的事件，如果队列为空则返回全零事件
 * @param   event - 输出参数，用于接收取出的事件
 * @return  uint8_t - 是否成功取出事件
 *          - 1: 成功取出有效事件
 *          - 0: 队列为空，无事件可取
 * @note    此函数会自动将事件从队列中移除，无需手动清除
 */
uint8_t S0553_PopKeyEvent(S0553KeyEvent *event);

/** @brief 建立连接（连接由 S0553_Task() 自治，未连接时自动调用，主控通常无需直接调用） */
void S0553_Connect(void);

/**
 * @brief 发送数据帧
 * @details 封装数据帧并发送给从机，用于发送命令和数据。
 * @param   cmd  - 命令字，对应S0553_CMD_*宏定义
 * @param   data - 数据指针，指向要发送的数据缓冲区
 * @param   len  - 数据长度，单位为字节
 * @note    - 封装数据帧并发送给从机
 *          - 命令字对应S0553_CMD_*宏定义
 *           - 数据指针指向要发送的数据缓冲区
 *           - 数据长度，单位为字节
 * @retval  无
 * @attention 封装数据帧并发送给从机，发送成功后状态会自动流转到WAIT_RSP
 * 
 */
/* 多段文字发送：本从机固件不支持单条 0x43 多段合并，故 S0553_ShowMultiText 内部对每段
 * 各自调用 S0553_ShowText 入队一条单段 0x43（与已调通的单段帧一致）。段内 text 须为
 * 静态/字面量或 S0553UI_ResolveText 返回的持久指针，发送前保持有效。 */
#define S0553_MAX_TEXT_SEG  8
typedef struct {
    const char *text;     // UTF-8 文本
    uint16_t    x, y;     // 视窗起点
    uint8_t     fontSize; // 字号（12/16/24/32）
    uint16_t    color;    // 字体颜色 RGB565
    uint8_t     lineGap;  // 行间隔
} S0553_TextSeg;
uint8_t S0553_ShowMultiText(const S0553_TextSeg *segs, uint8_t count);

uint8_t S0553_SendData(uint8_t cmd, const uint8_t *data, uint16_t len);

/**
 * @brief 在屏幕上显示指定图片
 * @details 发送填图命令（0x34），将从机Flash中存储的图片显示到指定位置。
 *          完整的刷图流程需要配合S0553_RefreshDisplay()使用：
 *          1. 调用 S0553_ShowImage(imageId, x, y) 填充图片数据
 *          2. 等待从机返回 0x3D 填图完成通知
 *          3. 调用 S0553_RefreshDisplay(1) 刷新屏幕显示
 * @param   imageId - 图片序号，对应从机Flash中预存的图片ID
 * @param   x       - 显示位置的X坐标（像素）
 * @param   y       - 显示位置的Y坐标（像素）
 * @note    - 必须在已连接状态下调用
 *          - 若需要显示多个图片组成界面，可连续调用此函数
 *          - 填图完成后会收到0x3D主动上报，状态机会自动处理
 * @return  uint8_t - 发送状态
 *          - 1: 成功发送
 *          - 0: 发送失败（忙碌或未连接）
 */
uint8_t S0553_ShowImage(uint16_t imageId, uint16_t x, uint16_t y);

/**
 * @brief 图片层"该显示哪张图"的解析器（由上层注册，【可选】）
 * @param layer - 图层配置（生命周期稳定的 const）
 * @return 图片序号；返回 S0553_LAYER_IMAGE_SKIP 表示【该层不画】，组帧时会被跳过
 * @details 引入原因：0x34 的组帧发生在【出队时】而非入队时，那时才最终确定每层的图片序号。
 *          有了这个回调，【动态图片层（序号需运行时计算）也能安全并入合批】——
 *          不必再为了"序号未定"而把动态层拆成单图帧单独发送（会导致帧数暴涨、刷屏卡慢）。
 * @note   未注册时组帧按 layer->Id 处理（等价于纯静态页面，行为与引入本机制前一致）。
 */
typedef uint16_t (*S0553LayerImageResolver)(const DisplayLayer *layer);

/** @brief "该层不画"哨兵值：解析器返回此值时，该层不写入 0x34 帧 */
#define S0553_LAYER_IMAGE_SKIP   0xFFFFu

/**
 * @brief 注册图片层解析器（不调用则不启用，按静态 Id 处理）
 * @param resolver - 解析函数；传 NULL 表示注销
 */
void S0553_SetLayerImageResolver(S0553LayerImageResolver resolver);

/**
 * @brief 批量显示一组图片（注册式，非阻塞）
 * @details 将同一页面内连续的图片层合并为单条 0x34 指令发送，减少串口往返。
 *          内部按 S0553_FILL_IMAGE_BATCH_MAX 上限自动分片为多条目入队；
 *          每层【最终显示哪张图】由已注册的解析器在【组帧时】决定（未注册则用 layer->Id）；
 *          解析器返回 S0553_LAYER_IMAGE_SKIP 的层会被跳过（不写入该帧）。
 *          调用方（S0553Display_Update）负责在图片层与非图片层之间正确 flush，
 *          以严格保持图层叠加顺序（先发的在下层）。
 * @param   layers - 指向连续 DisplayLayer（LAYER_TYPE_IMAGE）数组的指针
 * @param   count  - 图层数量
 * @return  uint8_t - 1: 已入队, 0: 失败（未连接/队列满/参数为空）
 * @note    - layers 必须指向生命周期稳定的 const 配置（UI_PAGE_MAP），发送时解引用
 *          - 动态图片层【也可并入本接口】：序号在组帧时经解析器现算，故总能发出最新值
 */
uint8_t S0553_ShowImageBatch(const DisplayLayer *layers, uint8_t count);

/**
 * @brief 在屏幕上填充颜色区域（使用0x35命令）
 * @details 发送填色命令（0x35），在指定矩形区域填充纯色。
 *          完整的刷图流程需要配合S0553_RefreshDisplay()使用：
 *          1. 调用 S0553_FillColor(imageId, x, y, width, height) 填充颜色
 *          2. 等待从机返回 0x3D 填图完成通知
 *          3. 调用 S0553_RefreshDisplay(1) 刷新屏幕显示
 * @param   imageId - 颜色ID/索引（通常用于标识不同的颜色配置）
 * @param   x       - 填充区域的起始X坐标（像素）
 * @param   y       - 填充区域的起始Y坐标（像素）
 * @param   width   - 填充区域的宽度（像素）
 * @param   height  - 填充区域的高度（像素）
 * @note    - 必须在已连接状态下调用
 *          - 填色完成后会收到0x3D主动上报，状态机会自动处理
 * @return  uint8_t - 发送状态
 *          - 1: 成功发送
 *          - 0: 发送失败（忙碌或未连接）
 */
uint8_t S0553_FillColor(uint16_t imageId, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief 在屏幕上播放动画序列（使用0x36命令）
 * @details 发送动画填充命令（0x36），配置轮播速度并播放指定动画。
 *          注册式接口（入队），由 S0553_Process() 闲时按序发送。
 * @param   animId - 动画ID，对应从机Flash中预存的动画序列ID
 * @param   speed  - 播放速度等级
 * @param   x      - 播放区域的起始X坐标（像素）
 * @param   y      - 播放区域的起始Y坐标（像素）
 * @note    - 必须在已连接状态下调用
 * @return  uint8_t - 入队状态
 *          - 1: 已入队
 *          - 0: 入队失败（队列满或未连接）
 */
uint8_t S0553_ShowAnimation(uint8_t animId, uint8_t speed, uint16_t x, uint16_t y);

/**
 * @brief 填充文本（注册式，非阻塞，协议 0x43）
 * @param text     UTF-8 文本（须为静态/字面量，发送前保持有效）
 * @param x,y      视窗起点坐标
 * @param fontSize 字号（12/16/24/32，其余回退 16）
 * @param color    字体颜色（RGB565）
 * @return 1=入队成功, 0=失败（未连接或队列满）
 */
uint8_t S0553_ShowText(const char *text, uint16_t x, uint16_t y, uint8_t fontSize, uint16_t color);
void S0553_QueryFontInfo(void);

/**
 * @brief 控制屏幕显示开关
 * @details 发送显示控制命令（0x37），开启或关闭屏幕显示
 * @param   enable - 显示使能
 *          - 1: 开启显示，刷新当前填充的图片内容
 *          - 0: 关闭显示，屏幕变黑但保留填充的数据
 * @note    - 必须在已连接状态下调用
 *          - 关闭显示后，再次开启时会显示之前填充的内容
 *          - 若未填充任何图片就开启显示，屏幕将显示黑屏
 * @return  uint8_t - 发送状态
 *          - 1: 成功发送
 *          - 0: 发送失败（忙碌或未连接）
 */
uint8_t S0553_RefreshDisplay(uint8_t enable);

/**
 * @brief 设置物理按键长按生效时间
 * @details 发送0x3A命令配置从设备的物理按键长按判定时间
 * @param   time_ms - 长按时间，单位毫秒，范围1000-10000
 * @return  uint8_t - 发送状态
 *          - 1: 命令已发送（需在已连接状态下）
 *          - 0: 发送失败（未连接或参数无效）
 * @note    - 必须在已连接状态下调用
 *          - 从设备断电重启后会恢复默认值
 */
uint8_t S0553_SetPhysicalLongPressTime(uint16_t time_ms);

/**
 * @brief 设置触摸按键长按生效时间
 * @details 发送0x3B命令配置从设备的触摸按键长按判定时间
 * @param   time_ms - 长按时间，单位毫秒，范围500-10000
 * @return  uint8_t - 发送状态
 *          - 1: 命令已发送（需在已连接状态下）
 *          - 0: 发送失败（未连接或参数无效）
 * @note    - 必须在已连接状态下调用
 *          - 从设备断电重启后会恢复默认值
 */
uint8_t S0553_SetTouchLongPressTime(uint16_t time_ms);

/**
 * @brief 配置模块默认参数
 * @details 在连接建立后自动调用，配置以下默认参数：
 *          - 物理按键长按时间（默认3000ms）
 *          - 触摸按键长按时间（默认2000ms）
 * @note    此函数通常在 S0553_Connect() 成功后自动调用
 * @retval  无
 */
void S0553_ConfigDefaults(void);

#ifdef __cplusplus
}
#endif

#endif /* S0553_MAIN_H_ */
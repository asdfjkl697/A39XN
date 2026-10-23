/***************************************************************************************************
* @file      S0553_main.c
* @brief     S0553旋钮屏模块核心实现
* @details   本文件实现了S0553智能旋钮显示屏的完整驱动逻辑，包括：
*            - UART底层通信驱动
*            - 协议帧组包与解析（状态机实现）
*            - 按键/旋转事件处理与消抖
*            - 模块状态机管理（连接、填图、显示等状态流转）
* 
* @note      本模块采用非阻塞设计，所有函数执行时间短，适合在实时系统中周期调用。
*            核心处理函数 S0553_Process() 建议每10ms调用一次。
* 
* @date      2025-04-10
==================================================================================================*/
#include "S0553_main.h"
#include "S0553_Log.h"   // LOG_DEBUG/LOG_ERROR：默认关闭；定义 S0553_ENABLE_LOG 才开启
#include <string.h>

/* ==================== 私有宏定义 ==================== */
#define S0553_FILL_TASK_TIMEOUT_TICKS  80   // 刷图任务超时阈值 (80 * 10ms = 800ms)；缩短以加速应答丢失后的自愈，避免长冻结

// 单条 0x34 批量刷图最多打包的图片层数（方案B：组帧缓冲放 static 不占栈，故可大于原 32）。
// 实际单页图层数通常远小于此，超限时 S0553_ShowImageBatch 自动分片为多条目入队。
#define S0553_FILL_IMAGE_BATCH_MAX     64

// 【方案B】批量刷图组帧缓冲字节数（头 2B + 每图 7B）；缓冲放在 static 数据段，见 s_FillBatchBuf。
#define S0553_FILL_BATCH_BUF_SIZE      (1 + 7 * S0553_FILL_IMAGE_BATCH_MAX)

// 【链路掉线检测】探活帧 = 0x37 刷新(开屏)指令，从机会立即回复 0x37 帧：
//   - 发送 0x37(enable=1) 后开启 S0553_REFRESH_ACK_TIMEOUT_TICKS 应答窗口；
//   - 窗口内收到 0x37 回复 → 从机在线，失败计数清零；
//   - 窗口到期仍无回复 → 本轮刷新指令无应答，计数 +1；若未达阈值且仍在开机
//     显示，组件自动补发一条 0x37 并重新开窗（方向1），探活节奏压缩到 ~2s 一次，
//     不再干等主控下一个 10s 整页刷新周期；
//   - 连续 S0553_LINK_FAIL_THRESHOLD 次无应答 → 判定掉线，回到 IDLE。
#define S0553_LINK_FAIL_THRESHOLD          2u   // 连续无应答判定次数
#define S0553_REFRESH_ACK_TIMEOUT_TICKS   200   // 0x37 应答等待窗口 (200*10ms = 2s)

// 【发送闸门】发送 0x37(开屏) 后，短暂不再下发下一批指令，等从机的 0x37 回复（或本窗口兜底到期）。
// 目的：避免主机在从机"还在按 0x37 刷新整屏"时就压入下一帧的填图指令。协议明示从机
//       不做指令排队（忙时只能回 0xFB BUSY），被抢占的填图指令可能收不到 0x3D，使填充任务
//       白等 80 tick(≈1.3s) 看门狗才解锁（表现为整批延迟）。正常场景 0x37 回复仅数十 ms
//       即到并提前清零本闸门，故实际几乎不产生额外等待；本窗口仅作"回复丢失"时的兜底上限。
#define S0553_REFRESH_GATE_TICKS           20   // 0x37 后发送闸门 (20*10ms = 200ms 上限)

/* ==================== 私有枚举 ==================== */
// 内部刷图任务状态机 (仅在 CONNECTED 状态下有效)
typedef enum {
    FILL_TASK_IDLE,
    FILL_TASK_SENT_CMD,       // 已发送 0x34 填图命令
    FILL_TASK_WAIT_COMPLETE   // 等待 0x3D 填图完成通知
} FillTaskState;

// 新增：待处理指令类型
// S0553 的指令特性：
// 查询类指令（如获取版本）：丢了可以重试，或者周期性发送，不需要严格队列。
// 控制类指令（如亮度、氛围灯）：最新的指令覆盖旧的即可（例如：先设红再设蓝，最终只要蓝）。
// 刷图类指令（Fill + Refresh）：必须严格按顺序执行，且不能并发。
typedef enum {
    PENDING_CMD_NONE = 0,
    PENDING_CMD_SHOW_IMAGE,
    PENDING_CMD_FILL_COLOR,
    PENDING_CMD_ANIMATION,
    PENDING_CMD_FILL_TEXT,
    PENDING_CMD_REFRESH,
    PENDING_CMD_AMBIENT_LIGHT,
    PENDING_CMD_SHOW_IMAGE_BATCH  // 整页/分段图片批量（单条 0x34 多图）
} PendingCmdType;

/* ==================== 私有变量 ==================== */
static S0553RxCtrl s_RxCtrl = {0};
static S0553State s_CurrentState = S0553_STATE_IDLE;

// 刷图任务相关变量
static FillTaskState s_FillTaskState = FILL_TASK_IDLE;
static uint16_t s_FillTaskTimer = 0;      // 刷图任务计时器

// 【链路掉线检测】探活帧 = 0x37 刷新指令。仅两个变量、与刷图(fill)任务完全独立：
//   - s_RefreshAckTimer：发送 0x37(enable=1) 后开启的应答等待窗口（倒计时，10ms/次）。
//     窗口内收到从机 0x37 回复 → 清零（从机在线）；窗口走完仍无回复 → 计一次无应答；
//     未达阈值且开机显示 → 组件自动补发 0x37 并重开窗（见 S0553_Process 2.5 块）。
//   - s_LinkFailCount：连续无应答次数，达 S0553_LINK_FAIL_THRESHOLD → 判定掉线回 IDLE。
// 说明：只统计"0x37 刷新指令有没有收到回复"，与填图指令(0x34/0x35/0x36)及刷图状态机
//       完全无关；关屏/黑屏指令(enable=0)即发即走、不开窗口；黑屏期主机不发刷新指令、
//       从机也无主动上报，无法探活，属本机制固有边界。
static uint16_t s_RefreshAckTimer = 0;  // 0x37 刷新指令应答等待窗口 (0=无等待)
static uint16_t s_RefreshGateTimer = 0; // 0x37 后"等回复"发送闸门 (0=放行)，见 S0553_REFRESH_GATE_TICKS
static uint8_t  s_LinkFailCount = 0;    // 连续"刷新指令无回复"次数

// 【方案3】待发送指令 FIFO 环形队列：
// 刷图类指令（Fill + Refresh）必须严格按顺序执行，且不能并发，
// 单槽位 pending 会被后注册指令覆盖（如 Update 内 ShowImage 被 Refresh 覆盖）。
// 队列保证指令按注册顺序出队发送，且刷图任务未完成前暂停后续出队。
#define S0553_PENDING_QUEUE_SIZE   8

// 缓存待发送指令的参数
typedef struct {
    uint16_t imageId;
    uint16_t x, y;
    uint16_t w, h;      // 仅用于 FillColor
    uint8_t enable;     // 仅用于 Refresh
    uint8_t speed;      // 仅用于 Animation
    const DisplayLayer *layers;  // 仅用于 SHOW_IMAGE_BATCH：连续图片层指针（指向 const 配置）
    uint8_t layerCount;          // 仅用于 SHOW_IMAGE_BATCH：图片层数量
    // 文本填充（0x43）参数
    const char *text;            // UTF-8 文本（须为静态/字面量，发送前保持有效）
    uint16_t    textLen;         // UTF-8 字节长度（0 表示自动 strlen）
    uint8_t     fontSize;        // 字号（12/16/24/32，映射字库特征代码）
    uint16_t    color;           // 字体颜色（RGB565）
    uint8_t     lineGap;         // 行间隔（像素）
} CmdParams;

// 队列元素：指令类型 + 参数
typedef struct {
    PendingCmdType cmd;
    CmdParams params;
} PendingItem;

/* 发送侧状态总控（与接收侧 s_RxCtrl 对称：Rx 攒"帧"、Tx 攒"命令"）
 * 说明：发送是主动且自包含的——S0553_SendFrame 一次调用即在栈上拼完整帧并同步直发，
 *      无需字节级发送缓冲（原 S0553TxCtrl 的 Buffer/Length 即因此被删）；
 *      跨调用需要保存的只有"排队等待"，即下面的环形队列与发送冷却。 */
typedef struct {
    PendingItem Items[S0553_PENDING_QUEUE_SIZE];  /**< 环形队列存储区 */
    uint8_t Head;                                /**< 队头（下一个待出队元素） */
    uint8_t Tail;                                /**< 队尾（下一个入队位置） */
    uint8_t Count;                               /**< 当前队列中指令数 */
    uint8_t SendCooldown;                        /**< 发送间隔保护计数器（防止指令堆叠） */
} S0553TxQueue;

static S0553TxQueue s_TxQueue = {0};

// 【方案B】批量刷图组帧缓冲（static 数据段）：放静态区而非函数栈，避免大数组压栈溢出；
// 当前为单线程串行发送（Process 出队即同步直发），SendData 返回后缓冲即复用，无需互斥。
static uint8_t s_FillBatchBuf[S0553_FILL_BATCH_BUF_SIZE];

#if S0553_LOG_ENABLED
// 调试日志时间基准：首次打印时锁定基准，使日志 ms 从 0 开始（而非系统/程序累计毫秒）。
// 仅在【开启日志】时编译（S0553_ENABLE_LOG）：默认关闭，零占用、且不会产生
// "未使用函数"告警，也不引用 S0553_HAL_GetTickMs()。
static uint32_t s_LogTickBase = 0;
static uint32_t S0553_LogMs(void)
{
    static uint8_t s_LogBaseInited = 0;
    if (!s_LogBaseInited)
    {
        s_LogTickBase = S0553_HAL_GetTickMs();
        s_LogBaseInited = 1;
    }
    return S0553_HAL_GetTickMs() - s_LogTickBase;
}
#endif /* S0553_LOG_ENABLED */

// 新增：按键事件队列
static S0553KeyEventQueue s_KeyEventQueue = {0};
static uint16_t s_StateTimeout = 0;

/* ==================== 私有函数声明 ==================== */
static void S0553Protocol_Reset(void);
static void S0553Queue_Reset(void);
static void S0553Link_Reset(void);
static void S0553_SendFrame(uint8_t cmd, const uint8_t *data, uint16_t dataLen);
static void S0553Protocol_Parse(void);
static uint8_t S0553Protocol_Checksum(const uint8_t *buf, uint16_t len);
static void S0553_StateMachine(void);
static void S0553_SendConnectCmd(uint8_t connect);
static void S0553_SendFillImageCmd(uint16_t imageId, uint16_t x, uint16_t y);
static void S0553_SendFillColorCmd(uint16_t imageId, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
static void S0553_SendFillAnimCmd(uint8_t animId, uint8_t speed, uint16_t x, uint16_t y);
static void S0553_SendFillTextCmd(const char *text, uint16_t textLen, uint8_t fontSize,
                                  uint16_t color, uint8_t lineGap, uint16_t x, uint16_t y);
static void S0553_SendFillImageBatch(const DisplayLayer *layers, uint8_t count);
static void S0553_SendDisplayControlCmd(uint8_t enable);
static uint8_t S0553_PendingEnqueue(PendingCmdType cmd, const CmdParams *params);
static uint8_t S0553_EnqueueEvent(const S0553KeyEvent *event);
static void S0553_SendLongPressTimeCmd(uint8_t cmd, uint16_t time_ms);

/* ==================== 帧发送 ==================== */
/**
 * @brief 组装并发送一帧数据
 * @details 按照协议格式组装帧：HEAD + CMD + LEN(2B) + DATA + CHECKSUM
 *          校验算法：除帧头外所有字节累加，然后异或0xFF
 * 
 * @param cmd     命令字
 * @param data    数据指针
 * @param dataLen 数据长度
 */
static void S0553_SendFrame(uint8_t cmd, const uint8_t *data, uint16_t dataLen)
{
    uint8_t frame[300];
    uint16_t frameLen = 0;
    
    // 填充帧头
    frame[frameLen++] = S0553_FRAME_HEAD;
    // 填充命令字
    frame[frameLen++] = cmd;
    // 填充数据长度（大端序：高字节在前）
    frame[frameLen++] = (uint8_t)(dataLen >> 8);
    frame[frameLen++] = (uint8_t)(dataLen & 0xFF);
    
    // 填充数据内容
    if (dataLen > 0 && data != NULL)
    {
        if (dataLen > S0553_MAX_DATA_LEN)
        {
            dataLen = S0553_MAX_DATA_LEN;
        }
        memcpy(&frame[frameLen], data, dataLen);
        frameLen += dataLen;
    }
    
    // 计算并填充校验码（从命令字开始到数据结束）
    uint8_t checksum = S0553Protocol_Checksum(&frame[1], frameLen - 1);
    frame[frameLen++] = checksum;

    // 【调试】打印发送帧（16进制），格式：[S0553 <运行毫秒>] TX: A5 33 00 02 01 00 5C
    // 便于与协议文档逐字节比对（含帧长信息）；时间戳仅在开启日志时有效
    // 量产模式（不定义DEBUG宏）下 LOG_DEBUG 编译为空，无任何输出开销
    LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] TX(%u): ", (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000), (unsigned)frameLen);
    for (uint16_t i = 0; i < frameLen; i++)
    {
        LOG_DEBUG("%02X ", frame[i]);
    }
    LOG_DEBUG("\n");

    // 逐字节发送
    for (uint16_t i = 0; i < frameLen; i++)
    {
        S0553_HAL_UartWriteByte(frame[i]);
    }
}

/* ==================== 协议解析 ==================== */
/**
 * @brief 计算校验码
 * @details 算法：所有字节累加，然后异或0xFF
 * @param buf 数据缓冲区指针
 * @param len 数据长度
 * @return uint8_t 校验码
 */
static uint8_t S0553Protocol_Checksum(const uint8_t *buf, uint16_t len)
{
    uint16_t sum = 0;
    for (uint16_t i = 0; i < len; i++)
    {
        sum += buf[i];
    }
    return (uint8_t)(sum ^ 0xFF);
}

/**
 * @brief 协议解析状态机
 * @details 采用6状态机解析数据流，处理粘包、断包等情况：
 *          State 0: 等待帧头 (0xA5)
 *          State 1: 接收命令字
 *          State 2: 接收数据长度高字节
 *          State 3: 接收数据长度低字节，解析长度
 *          State 4: 接收数据内容（循环直到收满）
 *          State 5: 接收校验码，验证并分发处理
 */
static void S0553Protocol_Parse(void)
{
    uint8_t data;
    
    // 循环处理接收缓冲区中的所有数据（可读字节数统一由平台 HAL 提供）
    while (S0553_HAL_UartRxAvailable() > 0)
    {
        data = S0553_HAL_UartReadByte();
        
        switch (s_RxCtrl.State)
        {
            case 0: // 等待帧头
                if (data == S0553_FRAME_HEAD)
                {
                    s_RxCtrl.Buffer[0] = data;
                    s_RxCtrl.Count = 1;
                    s_RxCtrl.State = 1;
                }
                break;
                
            case 1: // 接收命令字
                s_RxCtrl.Buffer[s_RxCtrl.Count++] = data;
                s_RxCtrl.State = 2;
                break;
                
            case 2: // 接收数据长度高字节
                s_RxCtrl.Buffer[s_RxCtrl.Count++] = data;
                s_RxCtrl.State = 3;
                break;
                
            case 3: // 接收数据长度低字节，解析长度
                s_RxCtrl.Buffer[s_RxCtrl.Count++] = data;
                
                // 组合2字节长度为16位整数（大端序）
                s_RxCtrl.ExpectedDataLen = ((uint16_t)s_RxCtrl.Buffer[2] << 8) | 
                                            (uint16_t)s_RxCtrl.Buffer[3];
                
                // 检查长度合法性
                if (s_RxCtrl.ExpectedDataLen > S0553_MAX_DATA_LEN)
                {
                    // 长度超限，丢弃并重试
                    s_RxCtrl.State = 0;
                    s_RxCtrl.Count = 0;
                }
                else if (s_RxCtrl.ExpectedDataLen == 0)
                {
                    // 无数据域，直接进入校验码接收
                    s_RxCtrl.State = 5;
                }
                else
                {
                    // 开始接收数据
                    s_RxCtrl.State = 4;
                }
                break;
                
            case 4: // 接收数据内容
                if (s_RxCtrl.Count - 4 < s_RxCtrl.ExpectedDataLen)
                {
                    s_RxCtrl.Buffer[s_RxCtrl.Count++] = data;
                    
                    // 检查是否收满
                    if ((s_RxCtrl.Count - 4) >= s_RxCtrl.ExpectedDataLen)
                    {
                        s_RxCtrl.State = 5;
                    }
                }
                else
                {
                    // 异常：数据溢出，重置
                    s_RxCtrl.State = 0;
                    s_RxCtrl.Count = 0;
                }
                break;
                
            case 5: // 接收校验码并验证
                s_RxCtrl.Buffer[s_RxCtrl.Count++] = data;
                
                // 计算校验码（从命令字到数据结束，不含帧头和校验码本身）
                uint8_t calcChecksum = S0553Protocol_Checksum(
                    &s_RxCtrl.Buffer[1],
                    s_RxCtrl.Count - 2
                );
                
                // 校验成功，分发处理
                if (calcChecksum == s_RxCtrl.Buffer[s_RxCtrl.Count - 1])
                {
                    uint8_t cmd = s_RxCtrl.Buffer[1];
                    uint8_t *payload = &s_RxCtrl.Buffer[4];
                    uint16_t payloadLen = s_RxCtrl.ExpectedDataLen;
                    
                    LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] RX(%u): ", (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000), (unsigned)s_RxCtrl.Count);
                    for (uint16_t i = 0; i < s_RxCtrl.Count; i++)
                    {
                        LOG_DEBUG("%02X ", s_RxCtrl.Buffer[i]);
                    }
                    LOG_DEBUG("\n");
                    switch (cmd)
                    {
                        case S0553_RSP_CONNECT: // 连接响应
                            if (payloadLen >= 2)
                            {
                                uint8_t status = payload[0];
                                if (status == S0553_ACK_SUCCESS)
                                {
                                    s_CurrentState = S0553_STATE_CONNECTED;
                                    s_FillTaskState = FILL_TASK_IDLE; // 重置内部任务
                                    s_RefreshAckTimer = 0;            // 新连接，探活窗口复位
                                    s_LinkFailCount = 0;              // 新连接，掉线计数复位
                                    // 连接成功后，自动配置默认参数
                                    S0553_ConfigDefaults();
                                    S0553_QueryFontInfo();   // 调试：查询从机实际支持的字库特征码
                                }
                                else
                                {
                                    s_CurrentState = S0553_STATE_IDLE;
                                }
                            }
                            break;

                        case S0553_RSP_FILL_IMAGE_ACK: // 步骤 2: 收到 0x34 填图应答
                            if (payload[0] == S0553_ACK_SUCCESS)
                            {
                                if (s_FillTaskState == FILL_TASK_SENT_CMD)
                                {
                                    s_FillTaskState = FILL_TASK_WAIT_COMPLETE;
                                    s_FillTaskTimer = 0; // 重置计时
                                }
                            }else {
                                // 填图命令失败，重置任务
                                s_FillTaskState = FILL_TASK_IDLE;
                                s_FillTaskTimer = 0; // 重置计时
                            }
                            break;

                        case S0553_CMD_FILL_TEXT: // 0x43 填充文本应答（cmd 与请求同号）
                            if (payload[0] == S0553_ACK_SUCCESS)
                            {
                                // 接收成功：与 0x34 对称，转 WAIT_COMPLETE 等 0x3D 解锁；
                                // 若从机对文本不回 0x3D，则 2s 看门狗兜底解锁，不会卡死。
                                if (s_FillTaskState == FILL_TASK_SENT_CMD)
                                {
                                    s_FillTaskState = FILL_TASK_WAIT_COMPLETE;
                                    s_FillTaskTimer = 0; // 重置计时
                                }
                            }
                            else
                            {
                                // 文本命令失败（如参数不支持 F9）：立即解锁，避免卡住队列
                                s_FillTaskState = FILL_TASK_IDLE;
                                s_FillTaskTimer = 0;
                                LOG_ERROR("[S0553] 0x43 rejected by slave (F9): font feature may be unsupported; re-querying font info via 0x42\n");
                                S0553_QueryFontInfo();   // 触发一次字库查询，便于在 F9 附近看到从机真实支持的字库
                            }
                            break;

                        case S0553_RSP_KEY_EVENT: // 按键事件上报
                            if (payloadLen >= 8)
                            {
                                S0553KeyEvent event;
                                event.KeyState = payload[1];
                                event.TouchX = ((uint16_t)payload[2] << 8) | payload[3];
                                event.TouchY = ((uint16_t)payload[4] << 8) | payload[5];
                                event.PhysicalContact = payload[6];
                                event.TouchContact = payload[7];
                                
                                S0553_EnqueueEvent(&event);     // 将事件加入队列
                            }
                            break;
                            
                        case S0553_RSP_FILL_COMPLETE: // 填图完成通知 (0x3D 主动上报)
                            // 实测从机上报 A5 3D 00 02 00 00 C0（payload[1]=0x00），处于填图等待态即视为完成；
                            // 接受 WAIT_COMPLETE / SENT_CMD：兜底 0x3D 先于 ACK 到达或 ACK 丢失。
                            // 收到直接解锁回 IDLE：Update() 队尾已注册唯一 REFRESH(0x37)，
                            // 解锁后调度器立即发出（不重复、不迟发）。切勿在此调 S0553_RefreshDisplay()
                            // 补发（会多一条 0x37），也勿改回 NEED_REFRESH（无解锁点将卡到看门狗 200tick≈3.3s）。
                            if (payloadLen >= 2 &&
                                (s_FillTaskState == FILL_TASK_WAIT_COMPLETE ||
                                 s_FillTaskState == FILL_TASK_SENT_CMD))
                            {
                                s_FillTaskState = FILL_TASK_IDLE;
                                s_FillTaskTimer = 0; // 重置计时
                            }
                            break;

                        case S0553_CMD_DISPLAY_CONTROL: // 0x37 刷新指令回复（链路探活确认）
                            // 从机对 0x37（含开/关屏）都会回复 cmd=0x37 帧（如 A5 37 00 02 FE 00 C8）。
                            // 收到回复即证明从机在线：关闭应答窗口并清零失败计数。
                            s_RefreshAckTimer = 0;   // 刷新指令已得到回复，结束等待
                            s_RefreshGateTimer = 0;  // 从机已画完并回复 → 立即放行下一批指令
                            s_LinkFailCount = 0;     // 从机在线，掉线计数清零
                            break;

                        case S0553_CMD_GET_FONT_INFO: // 0x42 字库信息响应
                        {
                            if (payloadLen >= 2)
                            {
                                uint8_t count = payload[1];
                                LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] FontInfo: count=%u\n",
                                          (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000),
                                          (unsigned)count);
                                for (uint8_t k = 0; k < count && payloadLen >= (uint16_t)(2u + 4u*(k + 1u)); k++)
                                {
                                    uint32_t code = ((uint32_t)payload[2 + 4*k] << 24) |
                                                    ((uint32_t)payload[3 + 4*k] << 16) |
                                                    ((uint32_t)payload[4 + 4*k] << 8)  |
                                                    (uint32_t)payload[5 + 4*k];
                                    /* code 仅供日志使用：量产模式日志展开为 ((void)0)，
                                       故显式消解"未使用变量"告警。 */
                                    (void)code;
                                    LOG_DEBUG("    font[%u] feature=0x%08X\n", (unsigned)k, code);
                                }
                            }
                            break;
                        }

                        default:
                            // 未知命令，忽略
                            break;
                    }
                }
                
                // 重置状态机，准备接收下一帧
                s_RxCtrl.State = 0;
                s_RxCtrl.Count = 0;
                s_RxCtrl.ExpectedDataLen = 0;
                break;
                
            default:
                // 异常状态，强制重置
                s_RxCtrl.State = 0;
                s_RxCtrl.Count = 0;
                s_RxCtrl.ExpectedDataLen = 0;
                break;
        }
    }
}

/* ==================== 状态机 ==================== */
/**
 * @brief 模块状态机处理
 * @details 管理各状态的超时逻辑，防止因通信异常导致的死锁
 *          超时后自动回退到安全状态（IDLE）
 *          仅管理 IDLE <-> CONNECTING <-> CONNECTED 的流转
 */
static void S0553_StateMachine(void)
{
    if (s_StateTimeout > 0) 
    {
        s_StateTimeout--;
    }
    else 
    {
        // 超时处理：连接过程中超时，回退到 IDLE
        if (s_CurrentState == S0553_STATE_CONNECTING)
        {
            s_CurrentState = S0553_STATE_IDLE;
        }
    }

    switch (s_CurrentState)
    {
        case S0553_STATE_IDLE:
            // 空闲状态，无操作
            break;
            
        case S0553_STATE_CONNECTING: // 注意：连接过程中也是一个临时状态

            break;
            
        case S0553_STATE_CONNECTED:
            // 正常工作状态，无需特殊处理
            // 刷图等复杂逻辑由 s_FillTaskState 内部管理
            break;
            
        default:
            s_CurrentState = S0553_STATE_IDLE;
            break;
    }
}

/* ==================== 命令发送封装 ==================== */
/**
 * @brief 发送连接/断开命令
 * @param connect 1=建立连接, 0=断开连接
 */
static void S0553_SendConnectCmd(uint8_t connect)
{
    uint8_t data[2] = {0x00, connect};
    S0553_SendData(S0553_CMD_CONNECT, data, 2);
}

/**
 * @brief 发送填图命令
 * @param imageId 图片ID
 * @param x       X坐标
 * @param y       Y坐标
 */
static void S0553_SendFillImageCmd(uint16_t imageId, uint16_t x, uint16_t y)
{
    // 0x34 协议格式（见 Interaction_cn.md）：[状态代号][图片存储区域(默认0)][图片序号高][图片序号低][X高][X低][Y高][Y低]，共 8 字节。
    // 注意：0x34 与 0x35 一样【没有"保留"字节】；此前多加 1 字节保留，导致从机错位解析图片序号、单图层页（如 Logo）不显示。
    uint8_t data[8] = {
        0x00,                       // 状态代号
        0x00,                       // 图片存储区域（默认 0）
        (uint8_t)(imageId >> 8),    // 图片序号高字节
        (uint8_t)(imageId & 0xFF),  // 图片序号低字节
        (uint8_t)(x >> 8),          // X坐标高字节
        (uint8_t)(x & 0xFF),        // X坐标低字节
        (uint8_t)(y >> 8),          // Y坐标高字节
        (uint8_t)(y & 0xFF)         // Y坐标低字节
    };
    S0553_SendData(S0553_CMD_FILL_IMAGE, data, 8);
}

/**
 * @brief 发送填色命令（0x35）
 * @param imageId 颜色ID/索引
 * @param x       起始X坐标
 * @param y       起始Y坐标
 * @param width   宽度
 * @param height  高度
 * @note  数据格式（11字节，见 Interaction_cn.md 0x35 定义）：
 *        [状态代号][颜色低][颜色高][X高][X低][Y高][Y低][宽度高][宽度低][高度高][高度低]
 *        颜色字段为小端序：S0553_FillColor 入队前已将 imageId 高低字节交换，
 *        因此这里 data[1] 实为颜色低字节、data[2] 为高字节（线上小端，符合从机协议）。
 */
static void S0553_SendFillColorCmd(uint16_t imageId, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint8_t data[11] = {
        0x00,                       // 状态代号
        (uint8_t)(imageId & 0xFF),  // 图片ID低字节
        (uint8_t)(imageId >> 8),    // 图片ID高字节
        (uint8_t)(x >> 8),          // X坐标高字节
        (uint8_t)(x & 0xFF),        // X坐标低字节
        (uint8_t)(y >> 8),          // Y坐标高字节
        (uint8_t)(y & 0xFF),        // Y坐标低字节
        (uint8_t)(width >> 8),      // 宽度高字节
        (uint8_t)(width & 0xFF),    // 宽度低字节
        (uint8_t)(height >> 8),     // 高度高字节
        (uint8_t)(height & 0xFF)    // 高度低字节
    };
    S0553_SendData(S0553_CMD_FILL_COLOR, data, 11);
}

/**
 * @brief 发送动画填充命令（0x36）
 * @param animId 动画ID（从机Flash中预存的动画序列ID）
 * @param speed  播放速度等级
 * @param x      X坐标
 * @param y      Y坐标
 * @note  数据格式：[动画ID][速度等级][起始X高][起始X低][起始Y低]
 */
static void S0553_SendFillAnimCmd(uint8_t animId, uint8_t speed, uint16_t x, uint16_t y)
{
    uint8_t data[5] = {
        animId,
        speed,
        (uint8_t)(x >> 8),
        (uint8_t)(x & 0xFF),
        (uint8_t)(y & 0xFF)
    };
    S0553_SendData(S0553_CMD_FILL_ANIMATION, data, 5);
}

/**
 * @brief 发送显示控制命令
 * @param enable 1=开启显示, 0=关闭显示
 */
static void S0553_SendDisplayControlCmd(uint8_t enable)
{
    uint8_t data[2] = {0x00, enable};
    S0553_SendData(S0553_CMD_DISPLAY_CONTROL, data, 2);
}

/**
 * @brief 将事件加入队列
 * @param event 要加入的事件
 * @return 1=成功, 0=队列已满
 */
static uint8_t S0553_EnqueueEvent(const S0553KeyEvent *event)
{
    if (s_KeyEventQueue.Count >= S0553_KEY_EVENT_QUEUE_SIZE)
    {
        return 0; // 队列已满
    }
    
    s_KeyEventQueue.Events[s_KeyEventQueue.Tail] = *event;
    s_KeyEventQueue.Tail = (s_KeyEventQueue.Tail + 1) % S0553_KEY_EVENT_QUEUE_SIZE;
    s_KeyEventQueue.Count++;
    
    return 1;
}

/**
 * @brief 发送长按时间配置命令
 * @param cmd 命令字（0x3A或0x3B）
 * @param time_ms 时间值（毫秒）
 */
static void S0553_SendLongPressTimeCmd(uint8_t cmd, uint16_t time_ms)
{
    uint8_t data[4] = {
        0x00,                           // 状态代号
        (uint8_t)(time_ms >> 8),        // 时间长字节
        (uint8_t)(time_ms & 0xFF),      // 时间短字节
        0x00                            // 保留
    };
    S0553_SendData(cmd, data, 4);
}

/**
 * @brief 将待发送指令入队（FIFO，带临界区保护）
 * @param cmd    指令类型
 * @param params 指令参数（可为NULL，内部会清零）
 * @return 1=入队成功, 0=队列已满（指令被丢弃）
 */
static uint8_t S0553_PendingEnqueue(PendingCmdType cmd, const CmdParams *params)
{
    uint8_t ret = 0;
    
    S0553_CRITICAL_ENTER();
    if (s_TxQueue.Count < S0553_PENDING_QUEUE_SIZE)
    {
        s_TxQueue.Items[s_TxQueue.Tail].cmd = cmd;
        if (params != NULL)
        {
            s_TxQueue.Items[s_TxQueue.Tail].params = *params;
        }
        else
        {
            memset(&s_TxQueue.Items[s_TxQueue.Tail].params, 0, sizeof(CmdParams));
        }
        s_TxQueue.Tail = (s_TxQueue.Tail + 1) % S0553_PENDING_QUEUE_SIZE;
        s_TxQueue.Count++;
        ret = 1;
    }
    else
    {
        LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] Pending queue full, drop cmd %d\n", (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000), (int)cmd);
    }
    S0553_CRITICAL_EXIT();
    
    return ret;
}

/** @brief 查询发送侧是否空闲（待发队列空且无在飞刷图任务），用于延迟刷新节流 */
uint8_t S0553_IsTxIdle(void)
{
    return (s_TxQueue.Count == 0 && s_FillTaskState == FILL_TASK_IDLE) ? 1u : 0u;
}

/* ==================== 组件内部状态复位（私有，仅 S0553_Init 调用） ==================== */
/* 说明：这些状态全部是组件私有数据（协议帧缓冲 / 待发队列 / 链路计数），
 *      归组件自己所有，故复位留在组件内；HAL 只提供平台服务，不感知上层状态。 */
/**
 * @brief 复位协议接收侧状态（帧解析缓冲 + 按键事件队列）
 */
static void S0553Protocol_Reset(void)
{
    memset(&s_RxCtrl, 0, sizeof(S0553RxCtrl));
    memset(&s_KeyEventQueue, 0, sizeof(S0553KeyEventQueue));
}

/**
 * @brief 复位发送侧状态（整个待发指令队列 + 发送冷却）
 */
static void S0553Queue_Reset(void)
{
    memset(&s_TxQueue, 0, sizeof(S0553TxQueue));
}

/**
 * @brief 复位链路状态（连接状态机 + 探活计数）
 */
static void S0553Link_Reset(void)
{
    s_CurrentState = S0553_STATE_IDLE;
    s_StateTimeout = 0;
    s_RefreshAckTimer = 0;              // 链路探活应答窗口复位
    s_RefreshGateTimer = 0;             // 0x37 后发送闸门复位
    s_LinkFailCount = 0;                // 链路掉线计数复位
}

/* ==================== 公共接口实现 ==================== */
/**
 * @brief 初始化S0553模块
 * @note  新增内部状态时，请补进对应的 S0553*_*_Reset()，避免在 Init 散写导致漏复位
 */
void S0553_Init(void)
{
    // 复位组件内部状态（按职责收口）
    S0553Protocol_Reset();
    S0553Queue_Reset();
    S0553Link_Reset();

    // 初始化UART（HAL 就绪确认，实际打开由 main 的 PC_UART_Init 完成）和显示模块
    (void)S0553_HAL_UartInit();
    S0553Display_Init();

    // 自动注册主控命令回调（弱符号 S0553Host_OnCommand）：主控定义强符号即被覆盖，无需再调 Register
    S0553_Host_Init();
}

/**
 * @brief 周期处理函数（建议10ms调用一次）
 */
void S0553_Process(void)
{
    // 1. 递减发送冷却计数器
    if (s_TxQueue.SendCooldown > 0)
    {
        s_TxQueue.SendCooldown--;
    }

    // 1.1 递减"0x37 后发送闸门"（收到 0x37 回复时会被提前清零，见协议解析 0x37 分支）
    if (s_RefreshGateTimer > 0)
    {
        s_RefreshGateTimer--;
    }

    // 2. 刷图任务看门狗检测（仅用于 fill 流程自身防死锁，与链路掉线检测无关）
    if (s_FillTaskState != FILL_TASK_IDLE)
    {
        s_FillTaskTimer++;
        if (s_FillTaskTimer >= S0553_FILL_TASK_TIMEOUT_TICKS)
        {
            // 超时！强制重置任务状态，防止死锁
            s_FillTaskState = FILL_TASK_IDLE;
            s_FillTaskTimer = 0;
            // 超时后，如果有 pending 指令，可以尝试重发
            // 可选：在此处添加错误日志或指示灯提示
        }
    }

    // 2.5 【链路掉线检测】0x37 刷新指令应答窗口（独立计时，复用 10ms 节拍）
    // 窗口 >0 = 已发 0x37(enable=1)、正等从机回复，每 tick 无条件递减。本块不依赖
    // 刷图(fill)任务状态：只要 S0553_Process() 按 10ms 节拍被调用，
    // 已开启的窗口就必然走完到期——不存在"非刷图场景下定时器不递减"的死区。
    // 窗口自然走完 = 2s 内无回复 → 本轮刷新指令无应答，s_LinkFailCount+1：
    //   - 未达阈值且仍在开机显示 → 立即补发 0x37(enable=1) 并重新开窗（方向1），
    //     探活节奏变为 ~2s/次、2 次无回复即判掉线，不再干等主控下一个 10s 整页刷新；
    //     正常在线时窗口总被回复提前清零，本分支永不执行，正常流量零增加。
    //   - 达阈值 → 判定掉线回 IDLE（清 fill 任务/待发队列/窗口/计数）。
    // 收到 0x37 回复时，协议解析会直接清零本窗口与失败计数（见 S0553_CMD_DISPLAY_CONTROL case）。
    if (s_RefreshAckTimer > 0 && --s_RefreshAckTimer == 0)
    {
        s_LinkFailCount++;
        if (s_LinkFailCount >= S0553_LINK_FAIL_THRESHOLD)
        {
            LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] Link loss (no reply to refresh cmd x%d), back to IDLE\n",
                      (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000),
                      (int)s_LinkFailCount);
            // 回到安全态并清空刷图任务/待发队列，等待主控重新连接
            s_CurrentState = S0553_STATE_IDLE;
            s_FillTaskState = FILL_TASK_IDLE;
            s_FillTaskTimer = 0;
            s_RefreshAckTimer = 0;
            s_LinkFailCount = 0;
            S0553_CRITICAL_ENTER();
            s_TxQueue.Head = 0;
            s_TxQueue.Tail = 0;
            s_TxQueue.Count = 0;
            S0553_CRITICAL_EXIT();
        }
        else if (s_CurrentState == S0553_STATE_CONNECTED &&
                 S0553Display_IsPowerOn() &&
                 S0553Display_GetCurrentPage() != S0553_DISP_TYPE_NONE)
        {
            // 【方向1】本轮 0x37 无回复但尚未达到掉线阈值：仍在开机显示，
            // 组件内立即补发一条 0x37(enable=1) 并重开应答窗口（直发、不进 fill 队列），
            // 把"下一次探活"从依赖主控 10s 周期刷新压缩到 ~2s。只有真掉线才会走到这里。
            LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] No reply to refresh cmd (fail=%d), auto re-probe 0x37\n",
                      (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000),
                      (int)s_LinkFailCount);
            S0553_SendDisplayControlCmd(1);                       // 补发探活帧
            s_RefreshAckTimer = S0553_REFRESH_ACK_TIMEOUT_TICKS;  // 重新开窗计时
            s_TxQueue.SendCooldown = 2;                                   // 保持 20ms 帧间隔
        }
        // else：非连接态 / 关屏(黑屏)时不自行补发——黑屏期主机本就不发刷新指令，
        // 从机也无主动上报，探活在此场景固有失效，交回主控周期刷新或重连逻辑。
    }

    // 2.6 【空闲泵送】若有延迟的文本刷新请求且链路已空闲，现在下发
    //     （由 S0553Display_PumpDeferredRefresh 保证队列最多一批、不溢出丢帧）
    S0553Display_PumpDeferredRefresh();

    // 3. 【核心】指令调度器：闲时自动发送
    // 条件：冷却时间已到 AND 当前没有正在进行的刷图任务 AND 未处于"0x37 后闸门"期 AND 队列非空
    // 【方案3】队列 FIFO 保证刷图类指令（Fill + Refresh）严格按注册顺序串行发送
    if (s_TxQueue.SendCooldown == 0 && s_FillTaskState == FILL_TASK_IDLE &&
        s_RefreshGateTimer == 0 && s_TxQueue.Count > 0)
    {
        PendingItem item;
        
        // 出队（临界区保护，防止中断中入队产生竞态）
        S0553_CRITICAL_ENTER();
        item = s_TxQueue.Items[s_TxQueue.Head];
        s_TxQueue.Head = (s_TxQueue.Head + 1) % S0553_PENDING_QUEUE_SIZE;
        s_TxQueue.Count--;
        S0553_CRITICAL_EXIT();

        switch (item.cmd)
        {
            case PENDING_CMD_SHOW_IMAGE:
                S0553_SendFillImageCmd(item.params.imageId, item.params.x, item.params.y);
                s_FillTaskState = FILL_TASK_SENT_CMD;
                s_FillTaskTimer = 0;
                break;

            case PENDING_CMD_SHOW_IMAGE_BATCH:
                S0553_SendFillImageBatch(item.params.layers, item.params.layerCount);
                s_FillTaskState = FILL_TASK_SENT_CMD;
                s_FillTaskTimer = 0;
                break;
                
            case PENDING_CMD_FILL_COLOR:
                S0553_SendFillColorCmd(item.params.imageId, item.params.x, item.params.y, 
                                       item.params.w, item.params.h);
                s_FillTaskState = FILL_TASK_SENT_CMD;
                s_FillTaskTimer = 0;
                break;
                
            case PENDING_CMD_ANIMATION:
                S0553_SendFillAnimCmd((uint8_t)item.params.imageId, item.params.speed,
                                      item.params.x, item.params.y);
                break;

            case PENDING_CMD_FILL_TEXT:
                S0553_SendFillTextCmd(item.params.text, item.params.textLen,
                                      item.params.fontSize, item.params.color,
                                      item.params.lineGap, item.params.x, item.params.y);
                s_FillTaskState = FILL_TASK_SENT_CMD;
                s_FillTaskTimer = 0;
                break;

            case PENDING_CMD_REFRESH:
                S0553_SendDisplayControlCmd(item.params.enable);
                // 【链路探活】0x37 刷新(开屏)指令 = 探活帧：从机会立即回复 0x37 帧，
                // 发送后开启应答窗口等该回复（仅探活计时，不改动刷图任务状态）；
                // 关屏/黑屏指令(enable=0)即发即走、不参与探活。
                if (item.params.enable)
                {
                    s_RefreshAckTimer = S0553_REFRESH_ACK_TIMEOUT_TICKS;
                    s_RefreshGateTimer = S0553_REFRESH_GATE_TICKS;   // 短暂不再发下一批，等 0x37 回复
                }
                break;
                
            default:
                // 未知/非法条目：直接丢弃，避免卡住队列
                break;
        }
        
        s_TxQueue.SendCooldown = 2; // 20ms 间隔
    }
     
    S0553Protocol_Parse();  // 4. 解析协议（接收应答和主动上报）
    S0553_StateMachine();   // 5. 核心状态机流转
}

/**
 * @brief 刷新显示
 */
void S0553_DisplayRefresh(void)
{
    S0553Display_Refresh();
}

/**
 * @brief 查询连接状态
 * @return 1=已连接, 0=未连接
 */
uint8_t S0553_IsConnected(void)
{
    return (s_CurrentState == S0553_STATE_CONNECTED) ? 1 : 0;
}

/**
 * @brief 从事件队列中取出一个事件（FIFO）
 * @param event 输出参数，用于接收取出的事件
 * @return 1=成功取出, 0=队列为空
 */
uint8_t S0553_PopKeyEvent(S0553KeyEvent *event)
{
    if (s_KeyEventQueue.Count == 0 || event == NULL)
    {
        return 0;
    }
    
    *event = s_KeyEventQueue.Events[s_KeyEventQueue.Head];
    s_KeyEventQueue.Head = (s_KeyEventQueue.Head + 1) % S0553_KEY_EVENT_QUEUE_SIZE;
    s_KeyEventQueue.Count--;
    
    return 1;
}

/**
 * @brief 建立连接
 */
void S0553_Connect(void)
{
    // 仅在空闲状态下允许连接
    if (s_CurrentState == S0553_STATE_IDLE)
    {
        S0553_SendConnectCmd(S0553_CONNECT_ESTABLISH);
        s_CurrentState = S0553_STATE_CONNECTING;
        s_StateTimeout = S0553_CONNECT_TIMEOUT;
    }
}


/**
 * @brief 发送数据帧
 *
 * @param cmd   命令字
 * @param data  指向待发送数据缓冲区的指针
 * @param len   数据长度
 */
void S0553_SendData(uint8_t cmd, const uint8_t *data, uint16_t len)
{
    // 如果冷却时间未到，直接丢弃或返回错误，防止刷屏
    if (s_TxQueue.SendCooldown > 0)
    {
        return; 
    }
    // 调用内部静态函数构建并发送数据帧
    S0553_SendFrame(cmd, data, len);
    // 设置冷却时间（例如 2 个周期，即 20ms）
    // 这个时间取决于你的 S0553_Process 调用频率
    s_TxQueue.SendCooldown = 2; 
}

/**
 * @brief 显示指定图片（注册式，非阻塞）
 * @details 将指令加入待处理队列，由 Process 在闲时自动发送
 * @param imageId 图片ID
 * @param x       X坐标
 * @param y       Y坐标
 * @return 1=成功发送, 0=失败（忙碌或未连接）
 */
uint8_t S0553_ShowImage(uint16_t imageId, uint16_t x, uint16_t y)
{
    CmdParams params;
    
    // 必须在已连接状态下
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }
    
    // 【方案3】入队（FIFO），由 Process 调度器按序发送；
    // 多图层页面不会互相覆盖，先注册的先发送
    memset(&params, 0, sizeof(CmdParams));
    params.imageId = imageId;
    params.x = x;
    params.y = y;
    
    return S0553_PendingEnqueue(PENDING_CMD_SHOW_IMAGE, &params);
}

/**
 * @brief 批量显示一组图片（注册式，非阻塞）
 * @details 将连续图片层合并为尽量少的 0x34 指令入队；单条上限
 *          S0553_FILL_IMAGE_BATCH_MAX，超出自动分片为多条目（每段一条 0x34）。
 *          链接未建立或参数为空直接返回失败。
 * @param   layers - 连续 DisplayLayer（LAYER_TYPE_IMAGE）数组指针
 * @param   count  - 图层数量
 * @return  uint8_t - 1: 已入队, 0: 失败
 */
/**
 * @brief 图片层解析器（由上层注册；NULL = 未启用，组帧按静态 Id 处理）
 * @details 必须是模块级静态：0x34 组帧发生在【出队时】，那时需要能随时取到解析器。
 */
static S0553LayerImageResolver s_LayerImageResolver = NULL;

void S0553_SetLayerImageResolver(S0553LayerImageResolver resolver)
{
    s_LayerImageResolver = resolver;
}

uint8_t S0553_ShowImageBatch(const DisplayLayer *layers, uint8_t count)
{
    if (layers == NULL || count == 0)
    {
        return 0;
    }
    // 必须在已连接状态下
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }

    uint8_t offset = 0;
    uint8_t result = 0;
    while (count > 0)
    {
        uint8_t chunk = (count > S0553_FILL_IMAGE_BATCH_MAX)
                        ? S0553_FILL_IMAGE_BATCH_MAX : count;
        CmdParams params;
        memset(&params, 0, sizeof(CmdParams));
        params.layers = layers + offset;
        params.layerCount = chunk;
        if (!S0553_PendingEnqueue(PENDING_CMD_SHOW_IMAGE_BATCH, &params))
        {
            break;   // 队列满，停止后续分片（已入队的仍会发送）
        }
        result = 1;
        offset += chunk;
        count -= chunk;
    }
    return result;
}

/**
 * @brief 发送整页/分段图片批量填图命令（0x34）
 * @details 将连续图片层拼成单条 0x34：每图子项 = 存储区(1B) + 图片序号(2B) + X(2B) + Y(2B)，
 *          字节序沿用现有单图 0x34（大端：高字节在前），与已调通的从机一致。
 * @note   内部为发送实现，由 Process 在出队时调用；发送后由调用方置 FILL_TASK_SENT_CMD。
 */
static void S0553_SendFillImageBatch(const DisplayLayer *layers, uint8_t count)
{
    if (layers == NULL || count == 0)
    {
        return;
    }
    if (count > S0553_FILL_IMAGE_BATCH_MAX)
    {
        count = S0553_FILL_IMAGE_BATCH_MAX;   // 防御：分片由 ShowImageBatch 保证
    }

    // 头 1 字节（状态代号）+ 每图 7 字节（存储区域1+图片序号2+X2+Y2）；0x34 无"保留"字节（见 Interaction_cn.md）。
    // 缓冲复用模块级 static，不占栈。
    uint8_t *data = s_FillBatchBuf;
    uint16_t len = 1;
    data[0] = 0x00;   // 状态代号
    for (uint8_t i = 0; i < count; i++)
    {
        const DisplayLayer *L = &layers[i];

        /* 【最终显示哪张图】在【组帧时】决定：有解析器就问它，否则用静态 Id。
           这正是动态图片层能够安全合批的原因——序号在出队组帧的这一刻现算，
           发出的永远是当前状态的图（不会出现"合批时算好、发送时已过期"）。 */
        const uint16_t imgId = (s_LayerImageResolver != NULL)
                             ? s_LayerImageResolver(L)
                             : L->Id;
        if (imgId == S0553_LAYER_IMAGE_SKIP)
        {
            continue;   /* 该层不画（如无对应素材）：不占本帧任何字节 */
        }

        data[len++] = 0x00;                    // 图片存储区域（默认 0）
        data[len++] = (uint8_t)(imgId >> 8);   // 图片序号高字节
        data[len++] = (uint8_t)(imgId & 0xFF);  // 图片序号低字节
        data[len++] = (uint8_t)(L->X >> 8);     // X 坐标高字节
        data[len++] = (uint8_t)(L->X & 0xFF);    // X 坐标低字节
        data[len++] = (uint8_t)(L->Y >> 8);     // Y 坐标高字节
        data[len++] = (uint8_t)(L->Y & 0xFF);    // Y 坐标低字节
    }
    S0553_SendData(S0553_CMD_FILL_IMAGE, data, len);
}

/**
 * @brief 填充颜色区域（注册式）
 * @param imageId 颜色ID/索引
 * @param x       起始X坐标
 * @param y       起始Y坐标
 * @param width   宽度
 * @param height  高度
 * @return 1=成功发送, 0=失败（忙碌或未连接）
 */
uint8_t S0553_FillColor(uint16_t imageId, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    CmdParams params;
    
    // 必须在已连接状态下
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }
    
    // 【方案3】入队（FIFO），由 Process 调度器按序发送
    memset(&params, 0, sizeof(CmdParams));
    params.imageId = imageId;
    params.x = x;
    params.y = y;
    params.w = width;
    params.h = height;
    
    return S0553_PendingEnqueue(PENDING_CMD_FILL_COLOR, &params);
}

/**
 * @brief 播放动画序列（注册式，非阻塞）
 * @details 将 0x36 动画指令加入待处理队列，由 Process 在闲时自动发送
 * @param animId 动画ID（从机Flash中预存的动画序列ID）
 * @param speed  播放速度等级
 * @param x      X坐标
 * @param y      Y坐标
 * @return 1=入队成功, 0=失败（未连接或队列满）
 */
uint8_t S0553_ShowAnimation(uint8_t animId, uint8_t speed, uint16_t x, uint16_t y)
{
    CmdParams params;
    
    // 必须在已连接状态下
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }
    
    // 【方案3】入队（FIFO），由 Process 调度器按序发送
    memset(&params, 0, sizeof(CmdParams));
    params.imageId = animId;
    params.speed = speed;
    params.x = x;
    params.y = y;
    
    return S0553_PendingEnqueue(PENDING_CMD_ANIMATION, &params);
}

/**
 * @brief 控制屏幕显示开关
 * @param enable 1=开启, 0=关闭
 * @return 1=成功发送, 0=失败（忙碌或未连接）
 */
uint8_t S0553_RefreshDisplay(uint8_t enable)
{
    CmdParams params;
    
    // 必须在已连接状态下
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }
    
    // 【方案3】入队（FIFO），由 Process 调度器按序发送。
    // 填图解锁已由 0x3D 解析处负责（收到 0x3D 直接回 IDLE），此处无需处理 fill 状态。
    memset(&params, 0, sizeof(CmdParams));
    params.enable = enable;
    
    return S0553_PendingEnqueue(PENDING_CMD_REFRESH, &params);
}

/**
 * @brief UTF-8 -> UTF-16(BMP) 转换（0x43 文本填充用）
 * @details 返回 UTF-16 码元个数；超出 BMP（4 字节 UTF-8，如 emoji）或非法序列降级为空格(0x0020)。
 *          组件内完成编码转换，调用方只需传 UTF-8 文本（与 C 源/主控字符串习惯一致）。
 */
static uint16_t S0553_Utf8ToUtf16(const char *utf8, uint16_t utf8Len, uint8_t *out, uint16_t outCapUnits)
{
    uint16_t units = 0;
    uint16_t i = 0;
    while (i < utf8Len && units < outCapUnits)
    {
        uint8_t c = (uint8_t)utf8[i];
        uint32_t cp;
        uint8_t adv;
        if (c < 0x80)                 { cp = c;        adv = 1; }
        else if ((c & 0xE0) == 0xC0)  { cp = c & 0x1F; adv = 2; }
        else if ((c & 0xF0) == 0xE0)  { cp = c & 0x0F; adv = 3; }
        else if ((c & 0xF8) == 0xF0)  { cp = 0x20;    adv = 4; }  // 四字节(astral)：降级空格
        else                          { cp = 0x20;    adv = 1; }  // 非法首字节：降级空格
        for (uint8_t k = 1; k < adv && (i + k) < utf8Len; k++)
        {
            cp = (cp << 6) | ((uint8_t)utf8[i + k] & 0x3F);
        }
        i += adv;
        if (cp > 0xFFFF) { cp = 0x20; }
        out[units * 2]     = (uint8_t)(cp >> 8);
        out[units * 2 + 1] = (uint8_t)(cp & 0xFF);
        units++;
    }
    return units;
}

/** @brief 字号 -> 字库特征代码（见 S0553_Protocol.h 宏与 Interaction_cn.md 附录9） */
static uint32_t S0553_FontLibCode(uint8_t fontSize)
{
    switch (fontSize)
    {
        case 12: return S0553_FONT_LIB_12;
        case 24: return S0553_FONT_LIB_24;
        case 32: return S0553_FONT_LIB_32;
        case 16:
        default: return S0553_FONT_LIB_16;
    }
}

/**
 * @brief 填充文本（注册式，非阻塞）
 * @details 将指令加入待处理队列，由 Process 在闲时按序发送（与图片/颜色填图同属刷图任务，须等待 0x3D 应答）。
 * @param text     UTF-8 文本（须为静态/字面量，发送前保持有效）
 * @param x,y      视窗起点坐标
 * @param fontSize 字号（12/16/24/32）
 * @param color    字体颜色（RGB565）
 * @return 1=成功入队, 0=失败（未连接或队列满）
 */
uint8_t S0553_ShowText(const char *text, uint16_t x, uint16_t y, uint8_t fontSize, uint16_t color)
{
    // 空串必须拒绝：S0553_SendFillTextCmd 对 0 字符会直接返回不发帧，若仍入队则调度器会
    // 误置 FILL_TASK_SENT_CMD，白等 80 tick 看门狗（见 S0553_Display.c AddTextLayerToSegs）
    if (s_CurrentState != S0553_STATE_CONNECTED || text == NULL || text[0] == '\0')
    {
        return 0;
    }
    CmdParams params;
    memset(&params, 0, sizeof(CmdParams));
    params.text     = text;
    params.textLen  = 0;                 // 0 表示发送时自动 strlen
    params.fontSize = fontSize ? fontSize : 16;
    params.color    = color;
    params.lineGap  = 0;
    params.x = x;
    params.y = y;
    return S0553_PendingEnqueue(PENDING_CMD_FILL_TEXT, &params);
}

/**
 * @brief 发送 0x43 填充文本指令（内部发送实现，由 Process 出队时调用）
 * @details 帧格式（详见 Interaction_cn.md:108），为"两段索引"结构：
 *          索引1(配置段): [状态0x00][索引0x01][内容长度m=16(2B)]
 *                         [字库特征(4B)][字体色(2B)][行间隔(1B)][保留0x00]
 *                         [X1(2B)][Y1(2B)][X2(2B)][Y2(2B)]
 *          索引2(文本段): [索引0x02][内容长度n(2B)][UNICODE 文本(n B, UTF-16)]
 *          m 固定 16(索引1配置总长)；n = 字符数×2(UTF-16 字节数)。
 *          UNICODE 端序：UTF-16BE（高字节在前，与内存中 cp>>8 在前一致）。
 *          视窗宽高按"字符数×字号"自动估算（CJK 占满字号宽，ASCII 估算偏宽属安全）。
 */
static void S0553_SendFillTextCmd(const char *text, uint16_t textLen,
                                  uint8_t fontSize, uint16_t color, uint8_t lineGap,
                                  uint16_t x, uint16_t y)
{
    if (text == NULL) return;
    if (textLen == 0) textLen = (uint16_t)strlen(text);

    uint8_t utf16[280];                       // 容量 = (300-23)/2 ≈ 138 码元
    uint16_t units = S0553_Utf8ToUtf16(text, textLen, utf16, 140);
    uint16_t n = (uint16_t)(units * 2);       // 索引2(文本)内容长度：UTF-16 字节数
    if (n == 0) return;

    // 索引1(配置)内容固定 16 字节：字库特征4 + 字体色2 + 行间隔1 + 保留1 + 坐标8
    const uint16_t m = 16;

    uint8_t data[300];
    uint16_t off = 0;
    // ---- 索引1：配置段 ----
    data[off++] = 0x00;                       // 状态代号
    data[off++] = 0x01;                       // 索引代号 0x01
    data[off++] = (uint8_t)(m >> 8);          // 索引1内容长度 m(2B)
    data[off++] = (uint8_t)(m & 0xFF);
    uint32_t flib = S0553_FontLibCode(fontSize);
    data[off++] = (uint8_t)(flib >> 24);
    data[off++] = (uint8_t)(flib >> 16);
    data[off++] = (uint8_t)(flib >> 8);
    data[off++] = (uint8_t)(flib & 0xFF);     // 字库特征代号
    data[off++] = (uint8_t)(color >> 8);      // 字体颜色(RGB565)
    data[off++] = (uint8_t)(color & 0xFF);
    data[off++] = lineGap;                     // 行间隔
    data[off++] = 0x00;                        // 保留
    uint16_t x2 = (uint16_t)(x + (uint32_t)units * fontSize);  // 视窗右下角（估算宽）
    uint16_t y2 = (uint16_t)(y + fontSize);                     // 视窗右下角（估算高）
    data[off++] = (uint8_t)(x >> 8);  data[off++] = (uint8_t)(x & 0xFF);
    data[off++] = (uint8_t)(y >> 8);  data[off++] = (uint8_t)(y & 0xFF);
    data[off++] = (uint8_t)(x2 >> 8); data[off++] = (uint8_t)(x2 & 0xFF);
    data[off++] = (uint8_t)(y2 >> 8); data[off++] = (uint8_t)(y2 & 0xFF);
    // 此刻 off == 4 + 16 == 20，恰为索引2起点

    // ---- 索引2：文本段 ----
    data[off++] = 0x02;                        // 索引代号 0x02
    data[off++] = (uint8_t)(n >> 8);           // 索引2内容长度 n(2B)
    data[off++] = (uint8_t)(n & 0xFF);
    for (uint16_t i = 0; i < n; i++) data[off++] = utf16[i];

    LOG_DEBUG("[S0553 %02uh %02um %02us.%03ums] TX TEXT(%u B payload, %u chars): \"%.*s\"\n",
              (unsigned)(S0553_LogMs()/3600000), (unsigned)((S0553_LogMs()/60000)%60), (unsigned)((S0553_LogMs()/1000)%60), (unsigned)(S0553_LogMs()%1000),
              (unsigned)off, (unsigned)units, (int)textLen, text);
    S0553_SendData(S0553_CMD_FILL_TEXT, data, off);
}



/**
 * @brief 多段文字发送（逐条 0x43）
 * @details 注意：经上机实测，本从机固件【不支持】单条 0x43 内含多段文本（无论索引代号
 *          按段递增 0x03/0x04 还是复用 0x01/0x02，从机都只渲染首段，且会丢弃先发的背景图）。
 *          因此此处对每段调用 S0553_ShowText 各自入队一条【单段】0x43（与已调通的
 *          "零压舒展"单段帧格式完全一致），从机必能逐条渲染。
 *          故"0x43 合并降低链路占用"在此固件上不可行；旋转刷新链路由已有的 300ms
 *          重绘限频(S0553_REFRESH_MIN_TICKS)保护，不因逐条发送而溢出。
 * @param segs 段数组（text 须为静态/持久指针，发送前有效），count 段数
 * @return 1=至少一段成功入队, 0=失败
 */
uint8_t S0553_ShowMultiText(const S0553_TextSeg *segs, uint8_t count)
{
    if (s_CurrentState != S0553_STATE_CONNECTED || segs == NULL || count == 0)
    {
        return 0;
    }
    uint8_t ok = 0;
    for (uint8_t i = 0; i < count && i < S0553_MAX_TEXT_SEG; i++)
    {
        const S0553_TextSeg *seg = &segs[i];
        if (seg->text == NULL) continue;
        uint8_t fs = seg->fontSize ? seg->fontSize : 16;
        if (S0553_ShowText(seg->text, seg->x, seg->y, fs, seg->color))
        {
            ok = 1;
        }
    }
    return ok;
}

/**
 * @brief 设置物理按键长按生效时间
 * @param time_ms 长按时间，单位毫秒，范围1000-10000
 * @return 1=成功发送, 0=失败
 */
uint8_t S0553_SetPhysicalLongPressTime(uint16_t time_ms)
{
    // 参数校验
    if (time_ms < 1000 || time_ms > 10000)
    {
        return 0;
    }
    
    // 检查连接状态
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }

    S0553_SendLongPressTimeCmd(S0553_CMD_SET_PHYSICAL_LONGPRESS_TIME, time_ms);
    return 1;
}

/**
 * @brief 设置触摸按键长按生效时间
 * @param time_ms 长按时间，单位毫秒，范围500-10000
 * @return 1=成功发送, 0=失败
 */
uint8_t S0553_SetTouchLongPressTime(uint16_t time_ms)
{
    // 参数校验
    if (time_ms < 500 || time_ms > 10000)
    {
        return 0;
    }
    
    // 检查连接状态
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return 0;
    }

    S0553_SendLongPressTimeCmd(S0553_CMD_SET_TOUCH_LONGPRESS_TIME, time_ms);
    return 1;
}

/**
 * @brief 配置模块默认参数
 */
/** @brief 查询从机字库信息（调试诊断：确认 0x43 该用哪个字库特征码）
 *  @details 发送 0x42 指令，从机回显 0x42 响应含字库数目与特征代码；
 *           若从机未加载本组件默认请求的 0xCA000002(16×16)，0x43 会回 F9(参数不支持)。 */
void S0553_QueryFontInfo(void)
{
    if (s_CurrentState != S0553_STATE_CONNECTED) return;
    uint8_t data[1] = { 0x00 };   // 状态代号
    S0553_SendData(S0553_CMD_GET_FONT_INFO, data, 1);
}

void S0553_ConfigDefaults(void)
{
    // 仅在已连接状态下执行
    if (s_CurrentState != S0553_STATE_CONNECTED)
    {
        return;
    }
    
    // 配置物理按键长按时间为默认值（3000ms，旋钮长按3秒触发长按事件）
    // 注意：必须放在最前面，避免被后续指令的发送冷却(s_TxQueue.SendCooldown)丢弃
    S0553_SetPhysicalLongPressTime(S0553_DEFAULT_PHYSICAL_LONGPRESS_TIME);

    RGBColor white = {200, 200, 200};
    S0553Display_SetAmbientLight(AMBIENT_MODE_ROTATE,white,500);

    // 配置触摸按键长按时间为默认值
    // S0553_SetTouchLongPressTime(S0553_DEFAULT_TOUCH_LONGPRESS_TIME);

    // S0553Display_SetBrightness(250);

    // 【默认关机黑屏】连接成功后不再自动显示图片，屏幕保持黑屏。
    // 需要显示画面时，由 UI 事件（如黑屏页长按3秒开机 ACTION_POWER_ON）
    // 调用 S0553Display_PowerOn() / S0553Display_Update() 触发显示。
    // S0553_ShowImage(0, 0, 0); // 已移除：连接后不再自动显示图片

    // 【默认关机黑屏】先通过 0x35 填充黑色全屏纯色背景（底层调用链：
    // S0553Display_FillBlackBackground -> SendLayerToSlave -> S0553_FillColor -> 0x35），
    // 真正覆盖从机显示缓冲，防止从机热启动残留上一帧画面
    S0553Display_FillBlackBackground();

    // 主动关闭显示，确保上电后为黑屏（即使从机热启动残留上一帧画面）
    S0553_RefreshDisplay(0);

}

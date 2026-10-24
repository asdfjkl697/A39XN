/***************************************************************************************************
* @file      S0553_Protocol.h
* @brief     S0553旋钮屏通信协议定义
* @details   本文件定义了S0553智能旋钮显示屏的完整通信协议，包括：
*            - 帧格式定义（帧头、命令字、数据长度、数据域、校验码）
*            - 主机到从机的控制命令（连接、填图、显示控制等）
*            - 从机到主机的响应和事件上报（按键事件、状态反馈等）
*            - 数据结构定义（帧结构、接收控制、按键事件等）
* @date      2025-04-10
==================================================================================================*/
#ifndef S0553_PROTOCOL_H
#define S0553_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "S0553_HAL.h"   // 平台服务契约：UART 字节流 / 延时 / 时基 / 临界区

/* ==================== 通信配置 ==================== */
/**
 * @note UART 字节流统一走硬件抽象层（S0553_HAL.h），本文件不再直接引用平台
 *       串口对象/函数。S0553_HAL_UartRxAvailable/ReadByte/WriteFrame 由平台实现：
 *       嵌入式平台按契约实现（UART 中断接收 + 硬件发送）。
 */
/** @brief 【协议层】帧组装缓冲区容量（字节）
 *  @note  这是组件协议状态机攒"一整帧"用的缓冲，属协议层，与平台侧字节环形缓冲无关；
 *         平台侧另有"UART 接收环形缓冲"（由平台自己的驱动实现），
 *         两者是各司其职的两级缓冲，勿混淆。容量需大于最大协议帧(5+256=261)，取 300 留余量 */
#define S0553_FRAME_BUFFER_SIZE 300


/* ==================== 协议帧格式 ==================== */
/**
 * @brief 协议帧格式说明：
 *        +--------+--------+----------+----------+------+-----------+
 *        | HEAD(1)| CMD(1) | LEN(2)   | DATA(N)  | CS(1)| 总长度    |
 *        +--------+--------+----------+----------+------+-----------+
 *        | 0xA5   | 命令字 | 大端序   | 可变长度 | 校验 | 5+N字节   |
 *        +--------+--------+----------+----------+------+-----------+
 * 
 * @note 校验算法：除帧头外所有字节累加，然后异或0xFF
 *       示例：A5 31 00 02 00 00 CC
 *             校验 = (0x31 + 0x00 + 0x02 + 0x00 + 0x00) ^ 0xFF = 0x33 ^ 0xFF = 0xCC
 */
/** @brief 帧头标识，固定为0xA5 */
#define S0553_FRAME_HEAD             0xA5
/** @brief 单次传输最大数据长度（字节），受限于从机处理能力 */
#define S0553_MAX_DATA_LEN           256

/* ==================== 主机->从机 命令字 (HOST to SLAVE) ==================== */
/** @brief 获取设备软件版本号，返回3字节版本信息 */
#define S0553_CMD_GET_VERSION        0x31
/** @brief 获取有效图片数量，返回4字节图片总数 */
#define S0553_CMD_GET_IMAGE_COUNT    0x32
/** @brief 建立/断开连接控制，数据字节[0]=状态代号, [1]=0x00断开/0x01建立 */
#define S0553_CMD_CONNECT            0x33
/** @brief 填充背景和图标，支持多张图片及坐标设置，最大数据长度65500字节 */
#define S0553_CMD_FILL_IMAGE         0x34
/** @brief 填充颜色区域，设置矩形区域的颜色填充 */
#define S0553_CMD_FILL_COLOR         0x35
/** @brief 填充动画序列，配置轮播速度、播放模式、图片序列等 */
#define S0553_CMD_FILL_ANIMATION     0x36
/** @brief 控制屏幕显示开关，数据字节[1]=0x00关闭/0x01开启 */
#define S0553_CMD_DISPLAY_CONTROL    0x37
/** @brief 控制RGB氛围灯，支持多种灯效模式（呼吸、跑马灯、彩虹等） */
#define S0553_CMD_AMBIENT_LIGHT      0x38
/** @brief 设置屏幕亮度，数值范围[20-250] */
#define S0553_CMD_SET_BRIGHTNESS     0x39
/** @brief 设置物理按键长按生效时间，单位ms，范围[1000-10000] */
#define S0553_CMD_SET_PHYSICAL_LONGPRESS_TIME  0x3A
/** @brief 设置触摸按键长按生效时间，单位ms，范围[500-10000] */
#define S0553_CMD_SET_TOUCH_LONGPRESS_TIME     0x3B
/** @brief 关闭蓝牙模块，断电后恢复 */
#define S0553_CMD_CLOSE_BLE          0x41
/** @brief 获取字库信息，返回字库数量和特征代码 */
#define S0553_CMD_GET_FONT_INFO      0x42
/** @brief 填充文本内容，支持UNICODE编码和多行显示 */
#define S0553_CMD_FILL_TEXT          0x43
/** @brief 字库特征代码（0x43 填充文本用，字号→特征代码映射，详见 Interaction_cn.md 附录9） */
#define S0553_FONT_LIB_12   0xCA000001  /**< 字库 12×12 */
#define S0553_FONT_LIB_16   0xCA000002  /**< 字库 16×16（含中英文） */
#define S0553_FONT_LIB_24   0xCA000004  /**< 字库 24×24（含中英文） */
#define S0553_FONT_LIB_32   0xCA000005  /**< 字库 32×32（含中英文） */
/** @brief 上传错误信息，用于调试和故障诊断 */
#define S0553_CMD_UPLOAD_ERROR       0x44
/** @brief 填充二维码，支持自定义比例和位置 */
#define S0553_CMD_FILL_QRCODE        0x45

/* ==================== 从机->主机 命令字 (SLAVE to HOST) ==================== */
/** @brief 版本号响应，对应CMD 0x31 */
#define S0553_RSP_VERSION            0x31
/** @brief 图片数量响应，对应CMD 0x32 */
#define S0553_RSP_IMAGE_COUNT        0x32
/** @brief 连接状态响应，对应CMD 0x33，返回连接建立/断开确认 */
#define S0553_RSP_CONNECT            0x33
/** @brief 填图应答，对应CMD 0x34，返回0xFE表示成功 */
#define S0553_RSP_FILL_IMAGE_ACK     0x34
/** @brief 按键事件主动上报，包含按键类型、触摸坐标、接触状态等8字节数据 */
#define S0553_RSP_KEY_EVENT          0x3C
/** @brief 填图完成通知，当所有图片填充完毕后主动上报 */
#define S0553_RSP_FILL_COMPLETE      0x3D
/** @brief 动画播放完成通知，单次播放结束后上报 */
#define S0553_RSP_ANIMATION_COMPLETE 0x3E
/** @brief BLE透传数据，来自手机APP的数据通过此命令转发 */
#define S0553_RSP_BLE_DATA           0x3F

/* ==================== 按键事件定义 (0x3C 数据字节1) ==================== */
/** @brief 无按键事件 */
#define S0553_KEY_NONE               0x00
/** @brief 物理旋钮短按事件 */
#define S0553_KEY_PHYSICAL_SHORT     0x01
/** @brief 物理旋钮长按事件（时长由0x3A命令设置） */
#define S0553_KEY_PHYSICAL_LONG      0x02
/** @brief 逆时针旋转事件（左旋） */
#define S0553_KEY_ROTATE_CCW         0x03
/** @brief 顺时针旋转事件（右旋） */
#define S0553_KEY_ROTATE_CW          0x04
/** @brief 触摸屏短按事件 */
#define S0553_KEY_TOUCH_SHORT        0x05
/** @brief 触摸屏长按事件（时长由0x3B命令设置） */
#define S0553_KEY_TOUCH_LONG         0x06
/** @brief 触摸屏左滑手势 */
#define S0553_KEY_SWIPE_LEFT         0x07
/** @brief 触摸屏右滑手势 */
#define S0553_KEY_SWIPE_RIGHT        0x08
/** @brief 触摸屏上滑手势 */
#define S0553_KEY_SWIPE_UP           0x09
/** @brief 触摸屏下滑手势 */
#define S0553_KEY_SWIPE_DOWN         0x0A

/* ==================== 连接状态 ==================== */
/** @brief 建立连接状态 */
#define S0553_CONNECT_ESTABLISH      0x01

/* ==================== 应答状态 ==================== */
/** @brief 操作成功应答码 */
#define S0553_ACK_SUCCESS            0xFE
/** @brief 操作失败应答码（如不支持的命令、参数错误等） */
#define S0553_ACK_FAIL               0xF9

/* ==================== 数据显示类型枚举 ==================== */
/**
 * @brief 显示页面类型枚举
 * @details 定义了系统中所有独立的UI业务页面。
 *          每个页面对应 S0553_UI_Config.c 中 UI_PAGE_MAP 的一个 PageConfig 配置项，
 *          由多个 DisplayLayer（背景、图标、动图、文字等）按顺序叠加而成。
 *
 * @note  - 【单一来源】页面 ID 清单集中在 S0553_PageList.h：新增页面只需在 S0553_PAGE_LIST
 *         中加一行 X(NAME)，本枚举即自动生成 S0553_DISP_TYPE_<NAME>，无需改本文件。
 *        - 每个页面仍须在 S0553_UI_Config.c 提供图层/事件数组与数值范围；
 *          UI_PAGE_MAP 已由本清单自动生成，漏配在编译期即报错（不再静默失效）。
 *        - NONE 必须保持首位(=0，黑屏/无页面)；MAX 由本处自动追加，勿写入清单。
 *        - 枚举值作为 S0553Display_Update() 第一个参数传入。
 */
#include "S0553_PageList.h"   /* 页面 ID 单一清单（S0553DisplayType 的唯一来源） */
#define S0553_PAGE_ENUM_ENTRY(id, sym) S0553_DISP_TYPE_##id,
typedef enum
{
    S0553_PAGE_LIST(S0553_PAGE_ENUM_ENTRY)   /* 展开为 S0553_DISP_TYPE_NONE, ... */
    S0553_DISP_TYPE_MAX           /**< 枚举最大值，用于边界检查 */
} S0553DisplayType;
#undef S0553_PAGE_ENUM_ENTRY

/**
 * @brief 氛围灯效果模式枚举
 * @details 对应协议命令 0x38 中的 Byte[4] 模式参数
 * @note 注意部分模式下，屏幕和呼吸灯一起闪烁
 */
typedef enum {
    AMBIENT_MODE_OFF = 0x00,              /**< 关闭氛围灯 */
    AMBIENT_MODE_ROTATE = 0x01,           /**< 轮播模式 */
    AMBIENT_MODE_BREATH_SINGLE = 0x02,    /**< 单色呼吸灯 */
    AMBIENT_MODE_BREATH_LIST = 0x03,      /**< 列表式呼吸灯（不支持列表修改） */
    AMBIENT_MODE_FIXED_COLOR = 0x04,      /**< 固定常量颜色 */
    AMBIENT_MODE_BLINK = 0x05,            /**< 亮灭交替，周期可改 */
    AMBIENT_MODE_HORSE_RACE = 0x06,       /**< 跑马灯 */
    AMBIENT_MODE_DRAGON = 0x07,           /**< 顺序点亮，反序熄灭，接龙模式 */
    AMBIENT_MODE_RELAY_HORSE = 0x08,      /**< 接力式跑马灯 */
    AMBIENT_MODE_RIPPLE = 0x09,           /**< 渐变呼吸涟漪效果 */
    AMBIENT_MODE_STAR_CHASE = 0x0A,       /**< 星光追逐模式 */
    AMBIENT_MODE_RAINBOW_WAVE = 0x0B,     /**< 彩虹波浪流动 */
    AMBIENT_MODE_FIREWORK = 0x0C,         /**< 烟花绽放特效 */
    AMBIENT_MODE_RELAY_CHAIN = 0x0D       /**< 接龙模式 */
} AmbientLightMode;

/**
 * @brief S0553模块状态机枚举
 * @details 定义模块运行的各个状态，确保操作流程的正确性
 */
typedef enum
{
    S0553_STATE_IDLE = 0,           /**< 1. 未连接状态 */
    S0553_STATE_CONNECTING = 1,     /**< 过渡状态：正在握手连接 */
    S0553_STATE_CONNECTED = 2       /**< 2. 连接通信阶段（核心工作状态） */
} S0553State;

/* ==================== 数据结构 ==================== */

/**
 * @brief UART接收控制结构
 * @details 用于状态机解析接收到的数据流，处理粘包、断包等情况
 */
typedef struct
{
    uint8_t Buffer[S0553_FRAME_BUFFER_SIZE]; /**< 帧组装缓冲区（协议层，非平台 UART 环形缓冲） */
    uint8_t Count;                        /**< 当前已接收字节数 */
    uint8_t State;                        /**< 状态机当前状态（0-5） */
    uint16_t ExpectedDataLen;             /**< 期望的数据长度，从帧中解析 */
} S0553RxCtrl;

/**
 * @brief 按键事件数据结构
 * @details 对应0x3C命令上报的完整按键信息
 */
typedef struct
{
    uint8_t KeyState;           /**< 按键状态码，见S0553_KEY_*定义 */
    uint16_t TouchX;            /**< 触摸点X坐标（仅触摸事件有效） */
    uint16_t TouchY;            /**< 触摸点Y坐标（仅触摸事件有效） */
    uint8_t PhysicalContact;    /**< 物理按键接触状态：0x00无接触, 0x01有接触 */
    uint8_t TouchContact;       /**< 触摸检测状态：0x00无接触, 0x01有接触 */
} S0553KeyEvent;


/**
 * @brief 按键事件队列结构
 * @details 用于缓存多个按键事件，防止事件丢失
 */
#define S0553_KEY_EVENT_QUEUE_SIZE 8  /**< 事件队列大小 */
typedef struct {
    S0553KeyEvent Events[S0553_KEY_EVENT_QUEUE_SIZE];  /**< 事件数组 */
    uint8_t Head;                                        /**< 队头索引 */
    uint8_t Tail;                                        /**< 队尾索引 */
    uint8_t Count;                                       /**< 当前事件数量 */
} S0553KeyEventQueue;

/**
 * @brief RGB颜色结构体
 */
typedef struct {
    uint8_t R; /**< 红色分量 (0-255) */
    uint8_t G; /**< 绿色分量 (0-255) */
    uint8_t B; /**< 蓝色分量 (0-255) */
} RGBColor;

#ifdef __cplusplus
}
#endif

#endif /* S0553_PROTOCOL_H */
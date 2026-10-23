/***************************************************************************************************
* @file      S0553_Display.h
* @brief     S0553显示屏控制接口头文件
* @details   本文件提供S0553智能旋钮显示屏的高级显示控制接口，包括：
*            - 显示模块初始化
*            - 页面内容更新（根据业务逻辑自动映射到对应图片）
*            - 清屏操作
*            - 背光亮度调节
*            - 显示内容周期性刷新
* 
* @note      本模块是对底层通信协议（S0553_main.c）的封装，提供更友好的业务层接口。
*            开发者无需关心具体的图片ID和坐标，只需调用对应的显示类型即可。
* 
* @usage     典型使用场景：
*            // 系统启动时初始化
*            S0553Display_Init();
*            
*            // 当用户切换到加热模式时
*            S0553Display_Update(S0553_DISP_TYPE_HEAT, current_heat_level);
*            
*            // 调节屏幕亮度
*            S0553Display_SetBrightness(150);
*            
*            // 在主循环100ms时基中调用
*            S0553Display_Refresh();
* 
* @date      2025-04-10
==================================================================================================*/
#ifndef S0553_DISPLAY_H_
#define S0553_DISPLAY_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "S0553_Protocol.h"

/**
 * @brief 图层类型枚举
 */
typedef enum
{
    LAYER_TYPE_IMAGE,       /**< 静态图片 */
    LAYER_TYPE_ANIMATION,   /**< 动画序列 */
    LAYER_TYPE_TEXT,        /**< 文本内容 */
    LAYER_TYPE_COLOR        /**< 纯色填充 */
} LayerType;

/**
 * @brief UI 语言标识
 * @details 用于动态文本按语言查表。新增语言只需在末尾追加（保持 S0553_LANG_MAX 在最后）。
 */
typedef enum
{
    S0553_LANG_CN      = 0,  /**< 1 简体中文 */
    S0553_LANG_EN,           /**< 2 英语 English */
    S0553_LANG_DE,           /**< 3 德语 Deutsch */
    S0553_LANG_FR,           /**< 4 法语 Français */
    // TODO： 暂时用不到
    // S0553_LANG_IT,           /**< 5 意大利语 Italiano */
    // S0553_LANG_PT,           /**< 6 葡萄牙语 Português */
    // S0553_LANG_VI,           /**< 7 越南语 Tiếng Việt */
    // S0553_LANG_RO,           /**< 8 罗马尼亚语 Română */
    // S0553_LANG_HU,           /**< 9 匈牙利语 Magyar */
    // S0553_LANG_NL,           /**< 10 荷兰语 Nederlands */
    // S0553_LANG_ES,           /**< 11 西班牙语 Español */
    // S0553_LANG_BG,           /**< 12 保加利亚语 Bulgarian */
    // S0553_LANG_DA,           /**< 13 丹麦语 Danish */
    // S0553_LANG_RU,           /**< 14 俄语 Pусский */
    // S0553_LANG_UZ,           /**< 15 乌兹别克斯坦语 UZ */
    // S0553_LANG_TH,           /**< 16 泰语 ไทย */
    // S0553_LANG_UK,           /**< 17 乌克兰语 Ukrainian */
    // S0553_LANG_PL,           /**< 18 波兰语 Polish */
    // S0553_LANG_HR,           /**< 19 克罗地亚语 Croatian */
    // S0553_LANG_ZH_TW,        /**< 20 中文繁体 中文繁體 */
    // S0553_LANG_JA,           /**< 21 日语 日本語 */
    S0553_LANG_MAX           /**< 语言数量上限（当前=4：CN/EN/DE/FR；勿作为语言值使用） */
} S0553_LangId;

/**
 * @brief 图层数据结构
 */
typedef struct
{
    LayerType Type;         /**< 图层类型 */
    uint16_t Id;            /**< 资源ID：图片序号 / 动画起始序号 / 【文本图层=字体颜色(RGB565)】 */
    uint16_t X;             /**< X坐标（图层起点；文本图层为视窗 X1） */
    uint16_t Y;             /**< Y坐标（图层起点；文本图层为视窗 Y1） */
    uint16_t W;             /**< 宽度（颜色/文本视窗宽；文本填 0 则按字号自动估算） */
    uint16_t H;             /**< 高度（颜色/文本视窗高；文本填 0 则按字号自动估算） */
    uint8_t  Param1;        /**< 扩展参数1：动画速度 / 【文本图层=字号 12/16/24/32，默认16】 */
    uint8_t  Param2;        /**< 扩展参数2：保留 / 【文本图层=行间隔(像素)，默认0】 */
    const char *Text;       /**< 文本内容（UTF-8）：【仅 LAYER_TYPE_TEXT 使用】，须为静态/字面量；
                                 设为 NULL 时由 TextId 动态查表解析（见 S0553UI_ResolveText） */
    uint16_t TextId;        /**< 文本资源ID：【仅 LAYER_TYPE_TEXT 使用】当 Text==NULL 时，
                                 据此 + 当前主板自动程序序号 + 当前语言查表得到实际字符串 */
    uint16_t ImgResId;      /**< 图片资源ID：【仅 LAYER_TYPE_IMAGE 使用】与 TextId 对称；
                                 非 0 时图片序号由 S0553UI_ResolveImage() 动态解析，
                                 Id 字段退化为"另一状态/解析失败时的图片"；为 0 表示静态图片（用 Id）。
                                 @note 动态图片层【与静态层一样参与 0x34 合批】：其序号在协议层
                                       组帧时经 S0553_SetLayerImageResolver 注册的解析器现算，
                                       不会为了合批而拆成单图帧。 */
    uint16_t AltId;         /**< 备用图片序号（配合 ImgResId 的"状态切换"语义）：
                                 例如 IMGRES_ITEM_STATE 在【选中】时显示 AltId、【未选中】显示 Id。
                                 未使用填 0。这样"选中图"由页面自持，引擎无需硬编码素材。 */
    uint8_t  Index;         /**< 本层代表的【列表项序号】（配合 ImgResId 的通用语义）：
                                 例如 IMGRES_ITEM_STATE 据此判断"我是不是当前选中项"。
                                 【不要】把项序号编进 ImgResId 枚举——那会让枚举随项数膨胀。未使用填 0。 */
} DisplayLayer;

/**
 * @brief 设置当前主板自动程序序号（用于动态文本查表）
 * @param idx 主板自动程序序号（0-based）；由主板(host)在切换自动程序时调用
 */
void S0553Display_SetProgramIndex(uint8_t idx);

/**
 * @brief 设置当前 UI 语言（用于动态文本查表）
 * @param lang 语言标识（S0553_LangId）
 */
void S0553Display_SetLanguage(S0553_LangId lang);

/* 注：【页面不再本地保存业务数值】。要在屏上显示或由旋钮调节的量，统一建模为
   "数据点(DataPoint)"由主控持有，组件按需向主控请求（见 S0553_Host.h）。
   原每页多槽位存储(S0553_PAGE_VALUE_SLOTS)与 PageConfig 的 Min/Max/Current 已移除，
   避免"主控状态 + 组件副本 + 页面槽位"三份状态漂移。 */
#include "S0553_Host.h"

/* ================================================================================================
 * 【交互分类 · 暂定方案（后续可能调整）】
 * 按"是否涉及数据更改"把交互划分为两类，二者处理路径严格分离：
 *
 * ┌──────┬────────────────┬──────────────────────┬──────────────────────────────────────┐
 * │ 类别 │ 名称           │ 处理方               │ 时序                                 │
 * ├──────┼────────────────┼──────────────────────┼──────────────────────────────────────┤
 * │ 第一类│ 组件自治交互   │ 组件（依据预设程序） │ 组件自行处理，立即生效；不通知主控   │
 * │      │                │                      │ 典型：UI刷新、上/下页、开关屏、     │
 * │      │                │                      │       屏幕亮度/氛围灯等外设设置     │
 * ├──────┼────────────────┼──────────────────────┼──────────────────────────────────────┤
 * │ 第二类│ 数据修改类交互 │ 主控（唯一真相源）   │ 组件下发命令后即返回【不同步等待】； │
 * │      │                │                      │ 主控执行→数据变动→回推→组件触发刷新 │
 * │      │                │                      │ 典型：数值增减、业务选择、状态设置   │
 * └──────┴────────────────┴──────────────────────┴──────────────────────────────────────┘
 *
 * 要点：
 *  - 第二类【不采用同步等待回调】：组件 SendCommand 后立刻返回，不做回读、不阻塞；
 *    刷新的唯一触发点是"数据确实发生了变动"（主控回推 NotifyValue）。
 *  - 因此第二类的命令必须保证主控最终会回推（无论成功/拒绝/钳制），否则界面不刷新。
 *  - 【外设属性归第一类】屏幕亮度/氛围灯属本组件(显示屏)自身的外设属性，真相源就在
 *    组件自身，无需与主控双向同步，故由组件直接下发从机、不经主控，归入第一类。
 * ==============================================================================================*/

/**
 * @brief 事件动作类型枚举
 * @note  枚举按上述两类分组排列；分组本身即文档，新增动作请归入对应类别。
 */
typedef enum
{
    /* ---------------- 第一类：组件自治（立即生效，不经主控） ---------------- */
    ACTION_NONE,                  /**< [一] 无动作 */
    ACTION_SWITCH_PAGE,           /**< [一] 切换到指定页面 */
    ACTION_PUSH_PAGE,             /**< [一] 压栈并切换页面（进入子页） */
    ACTION_BACK,                  /**< [一] 出栈返回进入前的页面（上一页） */
    ACTION_REFRESH,               /**< [一] 整页重绘当前页面（不改数据，仅重新下发全部图层） */
    ACTION_POWER_ON,              /**< [一] 开机：从黑屏状态恢复显示指定页面 */
    ACTION_POWER_OFF,             /**< [一] 关机：进入黑屏状态 */
    ACTION_SET_BRIGHTNESS,        /**< [一] 设置屏幕亮度（外设属性：组件直发从机，不经主控） */
    ACTION_SET_AMBIENT_COLOR,     /**< [一] 设置氛围灯颜色（外设属性：组件直发从机，不经主控） */
    ACTION_MOVE_CURSOR,           /**< [一] 移动本页选中游标：Param1=+1/-1，在 PageConfig.UiItemCount 内循环 */

    /* ---------------- 第二类：数据修改类（下发主控，不同步等待） ---------------- */
    ACTION_EXECUTE_CALLBACK       /**< [二] 执行自定义回调（页面自定义逻辑，通常为下发命令） */
} ActionType;

/* ---- 页面导航接口 ---- */
void    S0553Display_NavReset(void);
uint8_t S0553Display_NavDepth(void);

/**
 * @brief 压栈进入指定页面：当前页入栈 → 切换到目标页（目标页可用 ACTION_BACK 返回）
 * @param page  目标页面类型
 * @return uint8_t 1=已切换；0=失败（页面类型无效）
 * @note  与 ACTION_PUSH_PAGE 完全等价，只是把"压栈 + 切换"做成可编程接口，
 *        供【去向需按运行时状态决定】的页面在回调里调用（如菜单按游标进入不同子页）。
 */
uint8_t S0553Display_PushPage(S0553DisplayType page);

/**
 * @brief 切换到指定页面（【不】压栈：目标页用 ACTION_BACK 回不到这里）
 * @param page  目标页面类型
 * @return uint8_t 1=已切换；0=失败（页面类型无效）
 * @note  与 ACTION_SWITCH_PAGE 等价，同样做成可编程接口供回调使用。
 */
uint8_t S0553Display_SwitchPage(S0553DisplayType page);

/**
 * @brief 按【目标页表 + 本页 UI 游标】跳转（列表页通用入口）
 * @param targets  目标页表；下标 = 图层 Index = UI 游标值，非空
 * @param count    表项数（应等于 PageConfig.UiItemCount）
 * @details 约定：目标为【主页】时先清空返回栈再切换，避免从主页逐层回退又退回刚才的子菜单；
 *          其余目标一律压栈，目标页用 ACTION_BACK 即可回到本列表页。
 * @note   仅为 UI 导航，属【第一类：组件自治】，不经主控。
 */
void S0553Display_EnterTargetPage(const S0553DisplayType *targets, uint8_t count);

/**
 * @brief 事件动作结构
 */
typedef struct
{
    ActionType Type;              /**< 动作类型 */
    uint8_t Param1;               /**< 参数1（如目标页面类型、亮度值等） */
    uint8_t Param2;               /**< 参数2（预留） */
    void (*Callback)(void);       /**< 自定义回调函数指针（仅当Type=ACTION_EXECUTE_CALLBACK时有效） */
} EventAction;

/**
 * @brief 页面事件映射表
 */
typedef struct
{
    uint8_t KeyEvent;             /**< 按键事件码（S0553_KEY_*） */
    EventAction Action;           /**< 对应的动作 */
} PageEventMap;

/**
 * @brief 页面进入钩子（页面执行逻辑，与 UI 定义集中管理）
 * @details 把原先散落的回调（如 OnMenuChange）收敛到页面自身，避免"UI 与执行函数分离"：
 *          仅在"真正切换页面"时触发一次（周期刷新/同页数值更新不触发）。
 * @note    原 S0553PageHooks 结构体已移除：OnValueChange / OnExit / OnRefresh 三个成员
 *          长期无页面注册，属"看似通用实则无人调用的抽象负债"；仅保留实际在用的 OnEnter，
 *          故由结构体降为单个函数指针。日后确需"离开页面/周期刷新"回调时再按需加回。
 * @note    可为 NULL；未配置即跳过，行为与引入钩子前完全一致（向后兼容）。
 */
typedef void (*S0553PageEnterHook)(uint8_t value);

/**
 * @brief 页面周期钩子（10ms/tick，随 S0553Display_Tick() 调用）
 * @details 供页面做"需要持续计时"的交互判断，如坐姿页的"停手检测"：
 *          距最后一次旋转超过阈值即视为停手、由组件主动发停止命令。
 *          仅在页面处于显示中时调用；可为 NULL（绝大多数页面无周期逻辑，填 NULL 零开销）。
 * @note    组件内所有计时以 10ms/tick 为基准（随 S0553_Task 节拍），不依赖毫秒时基。
 */
typedef void (*S0553PageTickHook)(void);

/**
 * @brief 页面配置结构
 */
typedef struct
{
    S0553DisplayType PageType;      /**< 页面类型 */
    const DisplayLayer *Layers;     /**< 图层数组指针 */
    uint8_t LayerCount;             /**< 图层数量 */
    // 新增：事件映射表
    const PageEventMap *EventMaps;  /**< 事件映射数组指针 */
    uint8_t EventCount;             /**< 事件映射数量 */
    
    // 页面关联的数据点：本页要显示/调节的数值由主控持有（S0553_DP_NONE=本页无数值）
    uint8_t ValueDataPoint;         /**< 本页展示与调节的数据点（见 S0553_Host.h） */

    // 本页【游标 → 数据点值】的线性偏移：游标 0 项对应的数据点值，
    // 即"选中态"判据为 value == cursor + ValueOffset（配合 IMGRES_ITEM_LARGE_ICON_SELECTED）。
    // 例：零重力/自动程序1 cursor0→档1(offset=1)；自动程序2 cursor0→程序6(offset=6)；
    //     手动手法 cursor0→无手法0(offset=0)。
    uint8_t ValueOffset;            /**< 游标 0 项对应的数据点值（默认 0） */

    // 【组件本地 UI 游标】本页"可选项"数量：0 = 本页无选中项（旋转不移动游标）
    // 例：菜单页 4 个入口 → UiItemCount=4，游标 0/1/2/3 表示当前高亮哪一项
    uint8_t UiItemCount;            /**< 本页可选项数量（0=无选中项）；配合 ACTION_MOVE_CURSOR 使用 */

    /* 【子菜单项名字表】[项序][语言]，供 TEXTRES_CURSOR_ITEM_NAME 显示"当前选中项的名字"。
       下标 = UiItemCount 内的游标值 = 图层 Index；语言维度 = S0553_LANG_MAX(CN/EN/DE/FR)。
       本页无可选项（UiItemCount=0）或不需要标题时填 NULL。
       名字表由【页面自持】，故新增列表页只需在页面里填表 + 挂文字层，不必改枚举与解析函数。 */
    const char *(*ItemNames)[S0553_LANG_MAX];

    /* 【子菜单项大图表】[项序]：本页【中间大图】随选中的子菜单项切换
       （配合图层 ImgResId=IMGRES_ITEM_LARGE_ICON 使用）。
       下标 = UiItemCount 内的游标值；本页不需要中间大图时填 NULL。
       与 ItemNames 对称：大图表同样由【页面自持】，故新增列表页或换图
       只需改页面里的表，不必改枚举与解析函数。 */
    const uint16_t *ItemLargeIcons;

    /* 【子菜单项大图表-选中态】[项序]：与 ItemLargeIcons 同序 —— 本页中间大图在
       "该项被主控确认选中"时叠加显示的选中态图（配合 ImgResId=IMGRES_ITEM_LARGE_ICON_SELECTED 使用）。
       下标 = UiItemCount 内的游标值；本页不需要选中态大图时填 NULL。由【页面自持】。 */
    const uint16_t *ItemLargeIconsSelected;

    /* 【子菜单项小图标表】[项序]：本页每个菜单项各自一张小图标
       （配合图层 ImgResId=IMGRES_ITEM_ICON 使用，下标 = 图层 Index）。
       与 ItemNames / ItemLargeIcons 同序、同由【页面自持】；本页不需要时填 NULL。 */
    const uint16_t *ItemIcons;

    // 页面进入钩子（可为 NULL：表示该页无进入逻辑）
    S0553PageEnterHook OnEnter;     /**< 进入页面时调用一次（value=当前值） */

    // 页面周期钩子（可为 NULL）：随 S0553Display_Tick() 每 10ms 调用一次，
    // 供"停手检测"等需要持续计时的交互逻辑使用（见 S0553PageTickHook 说明）。
    S0553PageTickHook  OnTick;      /**< 页面显示期间每 10ms 调用一次；NULL = 无周期逻辑 */
} PageConfig;

/**
 * @brief 电源状态枚举
 * @details 标识屏幕当前的显示状态（由主控工作状态经 S0553_DP_POWER 数据点驱动）：
 *          - S0553_POWER_OFF:  关机/黑屏状态，屏幕不显示任何画面（主控工作状态 = 0）
 *          - S0553_POWER_ON:   开机/正常运行状态（主控工作状态 = 5，走 LOGO → 主页）
 *          - S0553_POWER_LOGO: 仅显示 Logo（中间态：待机/暂停/故障/升级等，主控工作状态为其他值）
 * @note  显示状态随主控工作状态自动联动：
 *         - 工作状态 0      -> 关机（S0553Display_PowerOff）
 *         - 工作状态 5      -> 开机（S0553Display_PowerOn）
 *         - 工作状态 其他值 -> 仅 Logo（内部 ShowLogoOnly）
 */
typedef enum
{
    S0553_POWER_OFF = 0,          /**< 关机/黑屏状态 */
    S0553_POWER_ON,               /**< 开机/正常运行状态 */
    S0553_POWER_LOGO              /**< 仅显示 Logo（中间态） */
} S0553PowerState;

/**
 * @brief 初始化S0553显示模块
 * @details 执行以下操作：
 *          - 重置当前显示类型为NONE
 *          - 清零当前显示值
 *          - 设置默认亮度（通常为100或根据项目需求设定）
 * @note    - 应在S0553_Init()之后调用
 *          - 此函数不发送任何通信指令，仅初始化内部状态
 * @retval  无
 */
void S0553Display_Init(void);


/**
 * @brief 更新显示屏内容（切换页面）
 * @details 根据指定的显示类型，自动查找对应的页面配置（PageConfig），并按顺序发送所有图层数据。
 *          内部流程：
 *          1. 检查模块是否已连接及参数合法性
 *          2. 根据 type 查找 PAGE_CONFIG_MAP 中的页面配置
 *          3. 遍历该页面的所有图层（背景、图标、动图、文字等），依次调用底层接口填充数据
 *          4. 所有图层填充完毕后，调用 S0553_RefreshDisplay(1) 统一刷新屏幕
 *          5. 更新内部状态记录（用于周期性保持显示）
 * 
 * @param   type  - 显示类型枚举，见 S0553DisplayType 定义
 *                  例如：S0553_DISP_TYPE_POWER, S0553_DISP_TYPE_HEAT 等
 * @param   value - 页面相关的动态数值（可选）
 *                  例如：加热等级、强度等级、剩余时间等
 *                  目前主要用于内部记录，未来可扩展为在指定图层上叠加实时数字或进度条
 * 
 * @note    - 必须在模块已连接状态下调用，否则直接返回
 *          - 如果 type 超出有效范围（>= S0553_DISP_TYPE_MAX），直接返回
 *          - **阻塞特性**：此函数会同步发送多个图层指令，每个图层间有微小延时，总耗时取决于图层数量
 *          - **叠加顺序**：在页面配置数组中，先定义的图层在下层，后定义的在上层
 *          - 频繁调用可能导致通信拥堵，建议仅在 UI 状态发生变化时调用
 * 
 * @retval  无
 */
void S0553Display_Update(S0553DisplayType type, uint8_t value);
/**
 * @brief 【限频整页重绘】重发指定页面的所有图层 + 补一条 0x37，并受 300ms 限频保护
 * @details 适用于"内容变了但不必立即上屏"的高频场景（旋转改数值、移动选中游标）：
 *          请求先登记，实际下发由 S0553Display_PumpDeferredRefresh() 在【链路空闲】且
 *          【距上次重绘 ≥300ms】时进行；期间状态仍逐条更新，屏幕最终显示最新值。
 * @note  虽叫"重绘"，【必须连背景图一起重发】：0x34 填图是【整屏替换】语义，
 *        只发变化的图层会把背景清成黑屏。详见 S0553_Display.c 实现处的协议说明。
 * @note  与 S0553Display_Update 的区别：Update 立即下发且【无限频】；本函数延迟 + 限频。
 */
void S0553Display_RefreshThrottled(S0553DisplayType type);
/** @brief 链路空闲时泵送延迟的重绘请求（由 S0553_Process 调用，避免高频更新挤爆队列） */
void S0553Display_PumpDeferredRefresh(void);

/* ---- 组件本地 UI 游标（纯 UI 状态：不经主控、主控不可见） ---- */
/**
 * @brief 取【当前页】的选中游标（0-based）
 * @details 供动态图片解析（DisplayLayer.ImgResId）判断"本层是不是选中项"，
 *          从而切换选中 / 未选中图。与数据点的区别见 S0553_Display.c 中的机制说明。
 */
uint8_t S0553Display_GetUiCursor(void);

/**
 * @brief 设置【当前页】的选中游标并刷新界面
 * @details 超出 UiItemCount 会被钳到末项；值未变化则不重绘。
 * @note   0x34 是【整屏替换】语义，故内部走整页重发——背景内容不变，但必须一起发。
 */
void S0553Display_SetUiCursor(uint8_t index);

/**
 * @brief 相对移动【当前页】的选中游标（循环）并刷新界面
 * @param delta +1 / -1；在 UiItemCount 内循环（0 左移 → 末项）
 * @note  与 ACTION_MOVE_CURSOR 同一实现；供页面回调内按运行时状态决定"是否移动游标"。
 */
void S0553Display_MoveUiCursor(int8_t delta);

/* ---- 组件本地"激活选中项"（坐姿等"短按选中后进入调节态"的交互，按页存储） ---- */
/**
 * @brief 取【当前页】的激活选中项（0xFF = 无激活，即浏览态）
 * @details UI 游标表示"当前高亮浏览到哪一项"，激活项表示"已确认选中哪一项"。
 *          坐姿页短按选中某部件后置为激活态：中间大图显示选中态、旋钮发对应命令。
 *          与 UI 游标同属【第一类：组件自治】状态（不经主控、主控不可见）。
 */
uint8_t S0553Display_GetActiveCursor(void);

/**
 * @brief 设置【当前页】的激活选中项并刷新界面
 * @param index 激活项序号；传 0xFF 清除激活（回到浏览态）
 * @note   非 0xFF 且越界会被钳到末项；值未变化则不重绘（0x34 整屏替换，走整页重发）。
 */
void S0553Display_SetActiveCursor(uint8_t index);

/**
 * @brief 返回上一页（出栈），可编程版（与 ACTION_BACK 同一实现）
 * @return 1=已出栈返回上一页；0=栈空/无效
 */
uint8_t S0553Display_BackPage(void);

/**
 * @brief 清除显示屏内容
 * @details 关闭屏幕显示，使屏幕变黑。内部调用S0553_RefreshDisplay(0)。
 *          同时重置内部记录的显示类型和数值。
 * 
 * @note    - 必须在模块已连接状态下调用
 *          - 清屏后，之前填充的图片数据仍保留在从机内存中
 *          - 再次调用S0553Display_Update()时会重新填充并显示
 *          - 适用于待机、省电或错误提示场景
 * 
 * @retval  无
 */
void S0553Display_Clear(void);

/**
 * @brief 填充黑色全屏纯色背景（关机/默认黑屏用）
 * @details 通过协议 0x35（控制区域填颜色）指令，在从机显示缓冲中生成全屏纯黑背景：
 *          - 真正覆盖从机显示缓冲，防止从机热启动或帧缓存异常时残留上一帧画面；
 *          - 通常与 S0553_RefreshDisplay(0)（0x37 关屏）配合，实现完整的关机黑屏。
 * @note    - 必须在模块已连接状态下调用
 *          - 内部通过 SendLayerToSlave() 走 0x35 填色逻辑
 *            （颜色ID=0 黑色，区域=全屏 240x240）
 *          - 本接口为注册式（非阻塞），实际发送由 S0553_Process() 调度
 * @retval  无
 */
void S0553Display_FillBlackBackground(void);

/**
 * @brief 开机并显示指定页面
 * @details 从黑屏（关机）状态恢复显示。内部等价于 S0553Display_Update(page, value)，
 *          更新成功后电源状态自动切换为开机。
 * 
 * @param   page  - 开机后默认显示的页面类型
 *                  若传 S0553_DISP_TYPE_NONE 或无效值，将回退显示默认电源页面
 * @param   value - 页面初始数值
 * 
 * @note    - 必须在模块已连接状态下调用
 *          - 由主控端调用：主控收到长按上报回调后决策开机，调用此接口使组件随之显示
 *          - 若需要开机后显示空白（保持黑屏），可不调用此函数
 * 
 * @retval  无
 */
void S0553Display_PowerOn(S0553DisplayType page, uint8_t value);

/**
 * @brief 预约一次"延时跳转"：delayTicks 个 tick 后自动显示 targetPage
 * @details 供页面的 OnEnter 钩子使用（如开机页停留 N tick 后跳到首页），
 *          使此类时序逻辑归属页面自身，引擎不再硬编码过渡时序与页面号。
 * @param targetPage 跳转目标页面
 * @param value      目标页面数值
 * @param delayTicks 延时 tick 数（10ms/tick）；0 表示下一个 tick 即跳转
 * @note  同一时刻仅保留一个待执行跳转，后预约的覆盖先前的。
 */
void S0553Display_RequestTransition(S0553DisplayType targetPage, uint8_t value, uint16_t delayTicks);

/**
 * @brief 获取本次开机要进入的目标页面（供开机页 OnEnter 预约跳转用）
 * @retval S0553DisplayType 开机目标页（由 S0553Display_PowerOn 记录）
 */
S0553DisplayType S0553Display_GetPowerOnTargetPage(void);

/**
 * @brief 获取本次开机要进入的页面初始数值
 */
uint8_t S0553Display_GetPowerOnTargetValue(void);

/**
 * @brief 显示层周期节拍（随 S0553_Task / S0553_Process 每 10ms 调用）
 * @details 处理开机 Logo 2 秒过渡：计时到后自动从 Logo 切到目标页。
 *          无过渡进行中时为快速返回，无性能开销。
 */
void S0553Display_Tick(void);

/**
 * @brief 显示文本（填充视窗字库数据，协议 0x43）
 * @details 通过 0x43 指令在指定视窗内绘制 UNICODE 文本（组件内将 UTF-8 转为 UTF-16 发给从机）。
 *          内部为注册式（非阻塞），实际发送由 S0553_Process() 调度。
 * @param text     UTF-8 文本指针（须为静态/字面量或生命周期覆盖发送的字符串）
 * @param x,y      视窗起点坐标
 * @param fontSize 字号（12/16/24/32，映射对应字库特征代码；其它值回退 16）
 * @param color    字体颜色（RGB565，2 字节）
 * @return 1=已入队, 0=失败（未连接或队列满）
 * @note  - 视窗宽高由组件按"字符数×字号"自动估算（CJK 每字占满字号宽，ASCII 估算偏宽属安全）；
 *          - 若需精确视窗，请改用页面图层 LAYER_TYPE_TEXT（DisplayLayer.W/H 显式指定）。
 */
uint8_t S0553Display_ShowText(const char *text, uint16_t x, uint16_t y, uint8_t fontSize, uint16_t color);

/* 注：原"旋钮长按上报回调"(S0553LongPressCallback / S0553Display_SetLongPressCallback) 已移除。
   开关机不再走长按信号通道：主控把自身工作状态映射成【S0553_DP_POWER】数据点定时推送，
   组件收到即自动开关机（详见 S0553_Host.h 的 S0553_DP_POWER 说明）。
   ⚠️ 仍保留 S0553_SetPhysicalLongPressTime() / S0553_SetTouchLongPressTime() ——
   它们是【发给从机】的长按阈值配置命令（0x3A / 0x3B），与本次移除的"上报主控"无关。

   另：原"显示数值变化回调"(S0553ValueChangeCallback) 也已移除。
   数值不再由组件本地修改，故不再有"组件侧数值变化"事件；用户调节统一走
   S0553_Host_SendCommand() 下发主控，由主控裁决后 S0553_Host_NotifyValue() 回推。 */

/**
 * @brief 查询当前电源状态
 * @return S0553_POWER_OFF=关机黑屏，S0553_POWER_ON=正常运行，S0553_POWER_LOGO=仅 Logo
 */
S0553PowerState S0553Display_GetPowerState(void);

/**
 * @brief 查询是否处于开机（有显示）状态
 * @return 1=开机/显示（正常运行或仅 Logo），0=关机黑屏
 */
uint8_t S0553Display_IsPowerOn(void);

/**
 * @brief 查询当前显示的页面类型
 * @return 当前页面（黑屏/关机时为 S0553_DISP_TYPE_NONE）
 */
S0553DisplayType S0553Display_GetCurrentPage(void);

/**
 * @brief 查询当前显示数值（最近一次 Update/Clear 记录的值）
 * @return 当前页面值；Clear/关机后为 0
 */
uint8_t S0553Display_GetCurrentValue(void);

/**
 * @brief 关机（进入黑屏状态）
 * @details 关闭屏幕显示并重置内部显示状态，使屏幕变黑。
 *          内部等价于 S0553Display_Clear()，清屏后电源状态自动切换为关机。
 * 
 * @note    - 必须在模块已连接状态下调用
 *          - 由主控端调用：主控收到长按上报回调后决策关机，调用此接口使组件随之黑屏
 *          - 关机后周期性刷新不会重新点亮屏幕，保持黑屏
 * 
 * @retval  无
 */
void S0553Display_PowerOff(void);

/**
 * @brief 接收主控运行状态改变，使本组件随之切换显示状态
 * @details 【组件职责边界】是否开关机由主控端识别决策。主控运行状态改变后，
 *          调用此接口通知本组件，组件随之切换显示状态：
 *          - S0553_POWER_ON  : 开机正常显示（LOGO → 主页）
 *          - S0553_POWER_LOGO: 仅显示 Logo（中间态）
 *          - S0553_POWER_OFF : 关机黑屏
 * @param state 主控下发的目标电源状态
 * @note  - 必须在模块已连接状态下调用
 *        - 若主控需要指定开机页面，可改用 S0553Display_PowerOn(page, value)
 * @retval 无
 */
void S0553Display_SetPowerState(S0553PowerState state);

/**
 * @brief 设置屏幕背光亮度
 * @details 发送亮度设置命令（0x39）到从机，调节LCD背光强度。
 * 
 * @param   level - 亮度等级，有效范围[20, 250]
 *                  - 20:  最暗（低于此值会被自动修正为20）
 *                  - 250: 最亮（高于此值会被自动修正为250）
 *                  - 推荐常用值：100-150（平衡功耗和可视性）
 * 
 * @note    - 如果模块未连接，亮度值会被记录（供查询）但不发送指令
 *          - 亮度设置会立即生效，无需刷新屏幕
 *          - 较低的亮度有助于降低功耗和延长背光寿命
 *          - 建议在用户设置界面中提供亮度调节功能
 * 
 * @retval  无
 */
void S0553Display_SetBrightness(uint8_t level);

/**
 * @brief 查询当前背光亮度
 * @return 亮度值（主控最近一次 SetBrightness 请求的值，范围[20,250]；未设置过为默认 100）
 * @note   查询的是本组件记录的目标值，实际生效以从机为准
 */
uint8_t S0553Display_GetBrightness(void);

/**
 * @brief 控制氛围灯效果
 * @details 发送0x38命令配置氛围灯的RGB颜色、效果和变换周期
 * @param mode      灯效模式，见AmbientLightMode枚举
 * @param color     RGB颜色结构体（某些模式下可能不使用，如轮播模式）
 * @param period_ms 变换周期，单位毫秒（某些模式下可能不使用，如固定颜色模式）
 * @return  uint8_t - 发送状态
 *          - 1: 成功发送
 *          - 0: 发送失败（未连接或忙碌）
 * @note    - 必须在已连接状态下调用
 *          - 不同模式对颜色和周期的要求不同：
 *            * FIXED_COLOR: 使用color，忽略period_ms
 *            * BREATH_SINGLE: 使用color和period_ms
 *            * ROTATE/RAINBOW_WAVE等: 可能忽略color，使用内部预设
 *          - period_ms 范围建议：100-5000ms
 */
uint8_t S0553Display_SetAmbientLight(AmbientLightMode mode, RGBColor color, uint16_t period_ms);

/**
 * @brief 查询当前氛围灯模式（主控最近一次成功设置的配置）
 * @return 灯效模式，见 AmbientLightMode 枚举
 */
AmbientLightMode S0553Display_GetAmbientMode(void);

/**
 * @brief 查询当前氛围灯颜色（主控最近一次成功设置的配置）
 * @return RGB 颜色
 */
RGBColor S0553Display_GetAmbientColor(void);

/**
 * @brief 查询当前氛围灯变换周期（主控最近一次成功设置的配置）
 * @return 周期（毫秒）
 */
uint16_t S0553Display_GetAmbientPeriod(void);

/**
 * @brief 快速设置氛围灯为固定颜色
 * @param color RGB颜色结构体
 * @return  uint8_t - 发送状态
 * @note    等价于 S0553Display_SetAmbientLight(AMBIENT_MODE_FIXED_COLOR, color, 0)
 */
uint8_t S0553Display_SetAmbientColor(RGBColor color);

/**
 * @brief 关闭氛围灯
 * @return  uint8_t - 发送状态
 * @note    等价于 S0553Display_SetAmbientLight(AMBIENT_MODE_OFF, {0,0,0}, 0)
 */
uint8_t S0553Display_TurnOffAmbientLight(void);

/**
 * @brief 周期性刷新显示内容
 * @details 为防止因通信干扰或从机异常导致的显示丢失，定期重发当前显示页面的数据。
 *          内部实现采用计数器机制，每调用100次才实际执行一次刷新操作。
 * 
 * @note    - 建议在100ms时基中调用此函数（即每10秒刷新一次）
 *          - 如果当前没有显示内容（type为NONE），则不执行任何操作
 *          - 刷新过程会自动调用S0553Display_Update()重发当前页面
 *          - 可根据实际通信稳定性调整刷新频率（修改内部计数器阈值）
 * 
 * @retval  无
 */
void S0553Display_Refresh(void);

/**
 * @brief 处理按键事件并自动执行对应动作
 * @details 这是核心自动化接口，根据当前页面和按键事件自动执行预定义的动作。
 *          支持的动作包括：
 *          - 切换页面（左右旋转）
 *          - 调整数值（左右旋转增减）
 *          - 执行回调函数（短按/长按触发特定逻辑）
 *          - 控制亮度/氛围灯等
 * 
 * @param   event - 按键事件结构指针，来自 S0553_PopKeyEvent() 或回调
 * 
 * @note    - 必须在模块已连接且有显示页面时调用
 *          - 事件与动作的映射关系在 PAGE_CONFIG_MAP 中配置
 *          - 如果某按键未配置动作，则自动忽略
 *          - 建议在 S0553_Process() 之后、主循环中调用
 * 
 * @retval  无
 */
void S0553Display_HandleKeyEvent(const S0553KeyEvent *event);

/**
 * @brief S0553模块统一任务处理函数（主控主循环【只需】周期调用这一个函数）
 * @details 该函数集成了底层通信处理、上层UI事件分发与周期维护。
 *          内部流程：
 *          1. 调用 S0553_Process() 处理串口接收、解析协议帧、维护连接状态。
 *          2. 检查是否有新的按键事件产生。
 *          3. 如果有事件，自动调用 S0553Display_HandleKeyEvent() 执行预定义动作。
 *          4. 开机 Logo 2 秒过渡计时。
 *          5. 连接管理（未连接时每 200 tick = 2s 自动发起 / 重试 S0553_Connect()）。
 *          6. 已连接时每 1000 tick = 10s 做一次 S0553_DisplayRefresh()（0x37 兼作探活帧）。
 * 
 * @note    - 以固定【10ms】周期调用：组件内部所有计时（2s 探活窗口、2s fill 看门狗、
 *             Logo 过渡、连接重试、周期刷新）都以此为基准。
 *          - 【连接已完全自治】主控【不需要】调用 S0553_Connect()：上电后第一次调用
 *            本函数即发起连接，连不上则每 2s 重试直到成功；掉线后下一次调用即重连。
 *            （S0553_Connect() 自带状态守卫，重复调用不会打断握手、不会重复发帧。）
 *          - 调用本函数后，【无需】再单独调用 S0553_Process()、手动处理按键队列，
 *            也【无需】自己实现定时器去做连接 / 重连 / 周期刷新——这些都已内置。
 *          - 主控只需在 S0553_IsConnected() 变为真后，决定【何时开机显示】
 *            （调 S0553Display_PowerOn()）以及【定时推送数据点】。
 * 
 * @retval  无
 */
void S0553_Task(void);

#ifdef __cplusplus
}
#endif

#endif /* S0553_DISPLAY_H_ */
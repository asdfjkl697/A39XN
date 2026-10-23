/***************************************************************************************************
* @file      S0553_Display.c
* @brief     S0553显示屏控制模块实现
* @details   本文件实现了S0553智能旋钮显示屏的高级显示控制逻辑，包括：
*            - 显示类型到图片ID的映射管理
*            - 页面内容更新与刷新
*            - 清屏操作
*            - 背光亮度调节
*            - 周期性显示保持（防止通信干扰导致显示丢失）
* 
* @note      本模块是对底层通信协议（S0553_main.c）的业务层封装。
*            开发者只需关注业务逻辑（如"显示加热页面"），无需关心具体的图片ID和坐标。
* 
* @date      2025-04-10
==================================================================================================*/
#include "S0553_Display.h"
#include "S0553_main.h"
#include "S0553_UI_Config.h"
#include "S0553_Log.h"   // LOG_DEBUG/LOG_ERROR：默认关闭；PC 调试定义 S0553_ENABLE_LOG 才开启
#include <string.h>

/* ==================== 屏幕分辨率 ==================== */
/** @brief S0553 旋钮屏分辨率（宽），用于 0x35 填色全屏背景 */
#define S0553_SCREEN_WIDTH   240
/** @brief S0553 旋钮屏分辨率（高） */
#define S0553_SCREEN_HEIGHT  240

/* ==================== 动态文本上下文 ==================== */
/* 注：原 s_ProgramIndex / s_Lang 两个"主控状态本地副本"已移除，统一收敛为
   S0553_Host 的数据点（S0553_DP_PROGRAM / S0553_DP_LANGUAGE）按需向主控取数，
   消除"主控值 + 组件副本"两份状态漂移。对外 API SetProgramIndex/SetLanguage
   保留并改为主控状态同步（Notify）入口，调用方无感。 */

/* ==================== 开机 Logo 过渡 ==================== */
/** @brief Logo 停留时长（单位：10ms tick），200 = 2 秒 */
/* 注：开机停留时长已移交给开机页自身（Layer/S0553_Page_Logo.c 的 LOGO_HOLD_TICKS），
   引擎不再持有过渡时序常量，也不再硬编码具体页面号。 */

/* ==================== 私有变量 ==================== */
/** @brief 当前显示的页面类型 */
static S0553DisplayType s_CurrentType = S0553_DISP_TYPE_NONE;
/** @brief 当前电源状态（关机黑屏/开机显示），默认关机 */
static S0553PowerState s_PowerState = S0553_POWER_OFF;
/** @brief 旋钮长按上报回调（主控注册，用于决策开关机） */
/* 注：原 s_LongPressCb（旋钮长按上报主控的回调）已删除 —— 开关机改由 S0553_DP_POWER 数据点驱动。 */

/* ---- 当前配置状态镜像（供 S0553Display_Get* 查询；有读者，勿删） ---- */
/** @brief 当前屏幕亮度值（主控最后请求值），范围[20, 250] */
static uint8_t s_Brightness = 100;
/* 注：原"显示数值变化回调" s_ValueChangeCb 已移除（数值不再由组件本地修改）；
   用户调节改走 S0553_Host_SendCommand() → 主控裁决 → S0553_Host_NotifyValue() 回推。 */
/** @brief 当前氛围灯模式 */
static AmbientLightMode s_AmbientMode = AMBIENT_MODE_OFF;
/** @brief 当前氛围灯颜色 */
static RGBColor s_AmbientColor = {0, 0, 0};
/** @brief 当前氛围灯变换周期（毫秒） */
static uint16_t s_AmbientPeriod = 0;

/* 注：原"每页上下文 PageContext{Value[4],MinVal[4],MaxVal[4]}"已整体移除。
   页面要显示/调节的数值统一建模为"数据点"，真相源在主控；组件仅在 S0553_Host
   保留一张渲染镜像（见 S0553_Host.c），不再有"每页多槽位"本地存储。 */

/* ---- 通用"延时跳转"状态：如开机页停留后自动切到目标页（时序由页面自己预约） ---- */
static uint8_t  s_TransActive = 0;                    // 是否有待执行的延时跳转
static uint16_t s_TransTicks  = 0;                    // 计时累加（10ms/tick）
static uint16_t s_TransDelay  = 0;                    // 预约的延时 tick 数
static S0553DisplayType s_TransTarget = S0553_DISP_TYPE_NONE;  // 跳转目标页
static uint8_t  s_TransValue = 0;                     // 跳转目标值
/* ---- 本次开机目标（由 S0553Display_PowerOn 记录，供开机页 OnEnter 读取） ---- */
static S0553DisplayType s_PowerOnTargetPage  = S0553_BOOT_PAGE;
static uint8_t          s_PowerOnTargetValue = 0;

/* ---- 无操作看护：工作状态=5(运行)且非主页时，累计"距最后一次按键"的 tick 数，超时自动返回主页 ---- */
#define S0553_IDLE_HOME_TICKS  800u   /* 8s = 800 tick × 10ms */
static uint16_t s_IdleTicks = 0;       /* 距最后一次按键操作的 tick 数（收到有效按键即清零） */

/* ---- 页面导航栈（ACTION_PUSH_PAGE 压栈 / ACTION_BACK 出栈） ---- */
#define S0553_NAV_STACK_DEPTH  8
static S0553DisplayType s_NavStack[S0553_NAV_STACK_DEPTH];
static uint8_t          s_NavTop = 0;

static void S0553Display_NavPush(S0553DisplayType page)
{
    if (s_NavTop < S0553_NAV_STACK_DEPTH)
    {
        s_NavStack[s_NavTop++] = page;
    }
    else
    {
        // 栈满：丢弃最旧一层，保证最近的返回路径可用
        for (uint8_t i = 1; i < S0553_NAV_STACK_DEPTH; i++) { s_NavStack[i-1] = s_NavStack[i]; }
        s_NavStack[S0553_NAV_STACK_DEPTH - 1] = page;
    }
}

static S0553DisplayType S0553Display_NavPop(void)
{
    if (s_NavTop == 0) { return S0553_DISP_TYPE_NONE; }
    return s_NavStack[--s_NavTop];
}


/* ==================== 私有函数 ==================== */
/* 前向声明：下列助手在 ExecuteAction 中使用，定义位于后文 */
static uint8_t        S0553Display_PageValue(S0553DisplayType page);
static void           S0553Display_OnHostValueChanged(S0553DataPoint dp, uint8_t oldValue, uint8_t newValue);

/**
 * @brief 根据页面类型查找配置
 */
static const PageConfig* FindPageConfig(S0553DisplayType type)
{
    // ⭐ 从外部配置表（PageConfig 指针数组）查找
    const PageConfig *const *map = S0553UI_GetPageConfigTable();
    uint8_t count = S0553UI_GetPageConfigCount();

    for (uint8_t i = 0; i < count; i++)
    {
        if (map[i]->PageType == type)
        {
            return map[i];
        }
    }
    return NULL;
}

/* 注：原"数值变化钩子" S0553Display_InvokeValueChangeHook() 已移除（无页面实际使用，
   唯一注册方 Menu_OnValueChange 是空实现）。数据变动的刷新统一由
   S0553Display_OnHostValueChanged() 处理。 */

/**
 * @brief 查找页面对应的事件动作
 */
static const EventAction* FindEventAction(const PageConfig *config, uint8_t keyEvent)
{
    if (config == NULL || config->EventMaps == NULL)
    {
        return NULL;
    }
    
    for (uint8_t i = 0; i < config->EventCount; i++)
    {
        if (config->EventMaps[i].KeyEvent == keyEvent)
        {
            return &config->EventMaps[i].Action;
        }
    }
    return NULL;
}

/* ==========================================================================
 * 组件本地 UI 游标【通用机制】（菜单 / 列表等"当前高亮第几项"都用它）
 * --------------------------------------------------------------------------
 * 纯 UI 状态，与【数据点】的本质区别：
 *   数据点（DP_PROGRAM 等）：主控持有的【业务】状态，组件只能查询 / 请求修改，异步回推；
 *   UI 游标              ：组件自己持有的【显示】状态，改了立即生效，主控完全不知道。
 *
 * 每个页面各存一份（按页面类型索引）：切页自动切换，返回上一页能恢复上次高亮位置。
 * 页面在 PageConfig.UiItemCount 声明"本页有几个可选项"；0 表示本页无游标概念。
 * ======================================================================== */
static uint8_t s_UiCursor[S0553_DISP_TYPE_MAX];

uint8_t S0553Display_GetUiCursor(void)
{
    if (s_CurrentType >= S0553_DISP_TYPE_MAX) { return 0; }
    return s_UiCursor[s_CurrentType];
}

void S0553Display_SetUiCursor(uint8_t index)
{
    if (s_CurrentType >= S0553_DISP_TYPE_MAX) { return; }

    const PageConfig *cfg = FindPageConfig(s_CurrentType);
    if (cfg == NULL || cfg->UiItemCount == 0)
    {
        return;   /* 本页未声明可选项 → 无游标概念 */
    }
    if (index >= cfg->UiItemCount)
    {
        index = (uint8_t)(cfg->UiItemCount - 1);   /* 越界钳到末项 */
    }
    if (s_UiCursor[s_CurrentType] == index)
    {
        return;   /* 无变化：不重绘 */
    }

    s_UiCursor[s_CurrentType] = index;
    LOG_DEBUG("[UI] Cursor -> %d (items=%d)\n", index, cfg->UiItemCount);

    /* 为什么必须重发：游标变了 → 各"选中框层"的 ImgResId 解析结果变了
       （旧的选中项不再画框、新的选中项开始画框）。图层表本身（顺序/坐标）没变，
       变的是【发送时解析出来的图片序号】。从机不知道游标存在，它只有我们上次填的内容，
       所以要让它显示新内容，就必须重新下发填图指令。
       为什么是整页重发：0x34 是【整屏替换】语义，只发变化的那两层会把背景清成黑屏，
       故静态层（背景）也必须一起发——内容不变，视觉上仍是"背景不动"。

       为什么走 RefreshThrottled 而不是 Update：本函数【无限频】，连续旋转会一封接一封
       淹没链路，冲掉 0x3D/0x37 应答 → FillTask 卡 2s 看门狗 → 整屏冻结。
       RefreshThrottled 走延迟泵送：300ms 限频 + 链路空闲判断，游标仍逐条更新（便宜），
       屏幕以 ≤300ms 的频率显示最新值。 */
    S0553Display_RefreshThrottled(s_CurrentType);
}

void S0553Display_MoveUiCursor(int8_t delta)
{
    if (s_CurrentType >= S0553_DISP_TYPE_MAX) { return; }
    const PageConfig *cfg = FindPageConfig(s_CurrentType);
    if (cfg == NULL || cfg->UiItemCount == 0) { return; }   /* 本页无选中项 */

    const uint8_t count = cfg->UiItemCount;
    const uint8_t cur   = s_UiCursor[s_CurrentType];
    /* 循环移动：先加 count 再取模，避免 uint8_t 下溢（0 - 1 → 末项） */
    const uint8_t next  = (uint8_t)((cur + count + delta) % count);
    S0553Display_SetUiCursor(next);
}

/* ---- 组件本地"激活选中项"（按页存储；0xFF = 无激活/浏览态） ----
   坐姿等"短按选中某部件后进入调节态"的交互：选中后该项为激活态，中间大图显示
   选中态、旋钮发对应命令；再次短按清除激活。与 UI 游标同属第一类组件自治状态
   （不经主控、主控不可见）。 */
static uint8_t s_ActiveCursor[S0553_DISP_TYPE_MAX];

uint8_t S0553Display_GetActiveCursor(void)
{
    if (s_CurrentType >= S0553_DISP_TYPE_MAX) { return 0xFF; }
    return s_ActiveCursor[s_CurrentType];
}

void S0553Display_SetActiveCursor(uint8_t index)
{
    if (s_CurrentType >= S0553_DISP_TYPE_MAX) { return; }
    if (s_ActiveCursor[s_CurrentType] == index) { return; }   /* 无变化：不重绘 */

    if (index != 0xFF)   /* 0xFF = 清除激活，合法值，无需钳制 */
    {
        const PageConfig *cfg = FindPageConfig(s_CurrentType);
        if (cfg == NULL || cfg->UiItemCount == 0) { return; }
        if (index >= cfg->UiItemCount) { index = (uint8_t)(cfg->UiItemCount - 1); }
    }

    s_ActiveCursor[s_CurrentType] = index;
    LOG_DEBUG("[UI] Active -> %d\n", index);
    S0553Display_RefreshThrottled(s_CurrentType);
}

uint8_t S0553Display_PushPage(S0553DisplayType page)
{
    if (page == S0553_DISP_TYPE_NONE || page >= S0553_DISP_TYPE_MAX) { return 0; }

    LOG_DEBUG("[Action] Push page %d -> %d\n", s_CurrentType, page);
    S0553Display_NavPush(s_CurrentType);
    S0553Display_Update(page, S0553Display_PageValue(page));
    return 1;
}

uint8_t S0553Display_SwitchPage(S0553DisplayType page)
{
    if (page == S0553_DISP_TYPE_NONE || page >= S0553_DISP_TYPE_MAX) { return 0; }

    LOG_DEBUG("[Action] Switch to page %d\n", page);
    S0553Display_Update(page, S0553Display_PageValue(page));
    return 1;
}

uint8_t S0553Display_BackPage(void)
{
    const S0553DisplayType prevPage = S0553Display_NavPop();
    LOG_DEBUG("[Action] Back to page %d\n", prevPage);
    if (prevPage != S0553_DISP_TYPE_NONE && prevPage < S0553_DISP_TYPE_MAX)
    {
        S0553Display_Update(prevPage, S0553Display_PageValue(prevPage));
        return 1;
    }
    return 0;
}

void S0553Display_EnterTargetPage(const S0553DisplayType *targets, uint8_t count)
{
    const uint8_t cursor = S0553Display_GetUiCursor();

    if (targets == NULL || cursor >= count) { return; }

    const S0553DisplayType target = targets[cursor];

    if (target == S0553_DISP_TYPE_NONE || target >= S0553_DISP_TYPE_MAX) { return; }

    if (target == S0553_DISP_TYPE_MAIN)
    {
        /* "返回主页"：先清空返回栈再切换。
           否则层层压栈后从主页按返回会一步步退回刚才未走完的子菜单。 */
        S0553Display_NavReset();
        (void)S0553Display_SwitchPage(target);
    }
    else
    {
        (void)S0553Display_PushPage(target);   /* 子页：压栈 → 目标页 ACTION_BACK 可回到本列表页 */
    }
}

/**
 * @brief 执行事件动作
 */
static void ExecuteAction(const EventAction *action)
{
    if (action == NULL)
    {
        return;
    }
    
    switch (action->Type)
    {
        case ACTION_SWITCH_PAGE:
        {
            S0553DisplayType targetPage = (S0553DisplayType)action->Param1;
            LOG_DEBUG("[Action] Switch to page %d\n", targetPage);
            S0553Display_Update(targetPage, S0553Display_PageValue(targetPage));
            break;
        }
        case ACTION_PUSH_PAGE:
        {
            /* 与 S0553Display_PushPage() 是同一实现，避免两处逻辑各自漂移 */
            (void)S0553Display_PushPage((S0553DisplayType)action->Param1);
            break;
        }
        case ACTION_BACK:
        {
            /* 与 S0553Display_BackPage() 同一实现，避免两处逻辑各自漂移 */
            (void)S0553Display_BackPage();
            break;
        }
        case ACTION_REFRESH:
        {
            // 【第一类：非数据类】组件自行【整页重绘】，不涉及数据更改、不通知主控
            LOG_DEBUG("[Action] Refresh page (full redraw, local)\n");
            S0553Display_Update(s_CurrentType, S0553Display_PageValue(s_CurrentType));
            break;
        }
        
        /* 注：原 ACTION_INCREMENT_VALUE / ACTION_DECREMENT_VALUE 已删除。
           数据调整统一走 ACTION_EXECUTE_CALLBACK（页面自定义回调内调用
           S0553_Host_SendCommand(S0553_HCMD_ADJUST, dp, ±1)），语义更明确且可指定目标数据点。 */
        
        /* 注：ACTION_TOGGLE_STATE 已删除（无页面使用，语义不明）。 */
        
        case ACTION_SET_BRIGHTNESS:
        {
            uint8_t brightness = action->Param1;
            LOG_DEBUG("[Action] Set brightness to %d\n", brightness);
            S0553Display_SetBrightness(brightness);
            break;
        }
        
        case ACTION_SET_AMBIENT_COLOR:
        {
            // TODO: 从Param1/Param2解析RGB颜色
            LOG_DEBUG("[Action] Set ambient color\n");
            break;
        }
        
        case ACTION_POWER_ON:
        {
            // 开机：从黑屏状态恢复显示指定页面（Param1=目标页面，Param2=初始值）
            S0553DisplayType targetPage = (S0553DisplayType)action->Param1;
            if (targetPage == S0553_DISP_TYPE_NONE || targetPage >= S0553_DISP_TYPE_MAX)
            {
                targetPage = S0553_BOOT_PAGE;  // 默认开机页面（由 UI 配置决定）
            }
            LOG_DEBUG("[Action] Power ON, show page %d\n", targetPage);
            S0553Display_PowerOn(targetPage, action->Param2);
            break;
        }
        
        case ACTION_POWER_OFF:
        {
            // 关机：进入黑屏状态
            LOG_DEBUG("[Action] Power OFF (black screen)\n");
            S0553Display_PowerOff();
            break;
        }
        
        case ACTION_MOVE_CURSOR:
        {
            /* 【第一类：组件自治】移动本页选中游标——纯 UI 状态，不经主控、不等待回推。
               Param1 是 uint8_t，页面写 +1 / -1，这里用 (int8_t) 还原符号。 */
            S0553Display_MoveUiCursor((int8_t)action->Param1);
            break;
        }

        case ACTION_EXECUTE_CALLBACK:
        {
            if (action->Callback != NULL)
            {
                LOG_DEBUG("[Action] Execute callback\n");
                action->Callback();
            }
            break;
        }
        
        default:
            break;
    }
}

/**
 * @brief 发送单个图层数据到从机
 */
/**
 * @brief 判断图片层是否需要绘制，并给出最终图片序号
 * @details 静态图片（ImgResId==0）照常绘制；动态图片按当前主控数据点解析：
 *          解析为 S0553_IMGRES_INVALID（如程序/手法【超出素材范围】）时【整层不画】，
 *          该区域保留本轮已重发的背景，从而做到"图文都不显示"，避免画出错误图标。
 * @return 1=绘制（*outId 有效）；0=不绘制
 */
static uint8_t S0553Display_LayerImage(const DisplayLayer *layer, uint16_t *outId)
{
    *outId = layer->Id;
    if (layer->ImgResId == 0)
    {
        return 1;   /* 静态图片 */
    }
    /* itemIndex / AltId / Id 由图层自身携带：解析规则与"第几项"解耦，
       新增列表项不用改枚举与解析函数，只在页面多填一层。 */
    const uint16_t id = S0553UI_ResolveImage(layer->ImgResId,
                                             S0553_Host_GetValue(S0553_DP_PROGRAM),
                                             (S0553_LangId)S0553_Host_GetValue(S0553_DP_LANGUAGE),
                                             layer->Index, layer->AltId, layer->Id);
    if (id == S0553_IMGRES_INVALID)
    {
        return 0;   /* 无对应素材 → 不画 */
    }
    *outId = id;
    return 1;
}

/**
 * @brief 注册给协议层的图片解析器：在组帧那一刻告诉它"这一层现在该显示哪张图"
 * @details 这是【动态图片层能够并入合批】的关键。协议层的 0x34 组帧发生在【出队时】，
 *          它只持有 const 图层数组指针，无法自己算出动态序号；由这里在那个时刻现算：
 *            · 静态层 → layer->Id
 *            · 动态层 → S0553UI_ResolveImage() 按当前状态解析
 *            · 无对应素材 → S0553_LAYER_IMAGE_SKIP（该层不写入帧）
 * @note   Layer 层的 S0553_IMGRES_INVALID → 协议层的 S0553_LAYER_IMAGE_SKIP
 *         在此完成转换，避免协议层反向依赖 Layer 目录。
 */
static uint16_t S0553Display_ResolveLayerImage(const DisplayLayer *layer)
{
    uint16_t imgId = 0;
    if (S0553Display_LayerImage(layer, &imgId))
    {
        return imgId;
    }
    return S0553_LAYER_IMAGE_SKIP;   /* 该层不画 */
}

static void SendLayerToSlave(const DisplayLayer *layer)
{
    switch (layer->Type)
    {
        case LAYER_TYPE_IMAGE:
        {
            uint16_t imgId = 0;
            if (S0553Display_LayerImage(layer, &imgId))
            {
                S0553_ShowImage(imgId, layer->X, layer->Y);
            }
            break;
        }
            
        case LAYER_TYPE_ANIMATION:
            // 调用动画填充接口 (0x36命令)，注册式入队，由 Process 调度发送
            // 格式：[动画ID][速度等级][起始X][起始Y]
            S0553_ShowAnimation((uint8_t)(layer->Id & 0xFF), layer->Param1, layer->X, layer->Y);
            break;
            
        case LAYER_TYPE_TEXT:
            // 0x43 填充文本（协议格式见 Interaction_cn.md:108）：
            // 图层字段映射：Id=字体颜色(RGB565)，X/Y=视窗起点，W/H=视窗宽高(0=自动估算)，
            //               Param1=字号(12/16/24/32)，Param2=行间隔(像素)
            // 文本来源：layer->Text 直接字面量；为 NULL 时按 (TextId, 当前程序序号, 当前语言)
            //           由 S0553UI_ResolveText 查表解析（声明式 UI 支持多语言/多程序动态文本）。
            {
                const char *txt = layer->Text;
                if (txt == NULL && layer->TextId != 0)
                {
                    txt = S0553UI_ResolveText(layer->TextId,
                                              S0553_Host_GetValue(S0553_DP_PROGRAM),
                                              (S0553_LangId)S0553_Host_GetValue(S0553_DP_LANGUAGE));
                }
                if (txt != NULL && txt[0] != '\0')   // 空文本不发（见 AddTextLayerToSegs 说明）
                {
                    S0553_ShowText(txt, layer->X, layer->Y,
                                   layer->Param1 ? layer->Param1 : 16,
                                   layer->Id);
                }
            }
            break;

        case LAYER_TYPE_COLOR:
            // 0x35 控制区域填颜色（协议格式见 Interaction_cn.md）：
            // Byte[0]=状态代号, Byte[1-2]=图片序号/颜色ID, Byte[3-4]=X,
            // Byte[5-6]=Y, Byte[7-8]=宽度, Byte[9-10]=高度
            // 图层字段映射：Id=颜色ID，X/Y=矩形起始坐标，W/H=矩形宽高
            S0553_FillColor(layer->Id, layer->X, layer->Y, layer->W, layer->H);
            break;
            
        default:
            break;
    }
    // 【方案2+3】图层指令均为注册式（入队），由 S0553_Process() 按序调度发送，
    // 无需阻塞延时；发送顺序由队列 FIFO 保证（先入队的先发，先发的在下层）
}

/**
 * @brief 将单个文本图层追加到合并段数组（解析动态文本 + 拷贝几何参数）
 * @return 1=已追加, 0=跳过(无文本/段满)
 */
static uint8_t AddTextLayerToSegs(const DisplayLayer *L, S0553_TextSeg *segs, uint8_t *cnt)
{
    if (L == NULL || segs == NULL || cnt == NULL || *cnt >= S0553_MAX_TEXT_SEG)
    {
        return 0;
    }
    const char *txt = L->Text;
    if (txt == NULL && L->TextId != 0)
    {
        txt = S0553UI_ResolveText(L->TextId,
                                  S0553_Host_GetValue(S0553_DP_PROGRAM),
                                  (S0553_LangId)S0553_Host_GetValue(S0553_DP_LANGUAGE));   // 动态文本按主控数据点解析
    }
    // 空串（如某程序在某语言下无翻译、或声明的空文本资源）必须跳过：
    // 否则会入队一条"0 字符"的 0x43，发送时 n==0 直接返回不发帧，但调度器仍置
    // FILL_TASK_SENT_CMD → 等不到 ACK/0x3D → 白等 80 tick(≈1.3s) 看门狗，造成整帧空档。
    if (txt == NULL || txt[0] == '\0')
    {
        return 0;
    }
    segs[*cnt].text     = txt;
    segs[*cnt].x        = L->X;
    segs[*cnt].y        = L->Y;
    segs[*cnt].fontSize = L->Param1 ? L->Param1 : 16;
    segs[*cnt].color    = L->Id;
    segs[*cnt].lineGap  = L->Param2;
    (*cnt)++;
    return 1;
}

/**
 * @brief 发送黑色全屏纯色背景（内部私有实现）
 * @details 构造一个黑色纯色图层（LAYER_TYPE_COLOR）交给 SendLayerToSlave 处理，
 *          通过协议 0x35（控制区域填颜色）指令真正将纯黑背景写入从机显示缓冲，
 *          防止关机黑屏时因从机热启动/帧缓存异常而残留上一帧画面。
 * @note   - 颜色ID=0（按从机 bin 调色板约定，通常 0 为黑色）
 *         - 填充区域为全屏（0,0）~（240,240）
 *         - 内部为注册式调用（S0553_FillColor），非阻塞，实际发送由 Process 调度
 */
static void SendBlackBackground(void)
{
    DisplayLayer blackLayer = {0};   // 整体清零，确保 Text/TextId 等未显式赋值成员确定性为 0
    blackLayer.Type   = LAYER_TYPE_COLOR;
    blackLayer.Id     = 0;                    // 颜色ID：0 为黑色
    blackLayer.X      = 0;
    blackLayer.Y      = 0;
    blackLayer.W      = S0553_SCREEN_WIDTH;   // 全屏宽度
    blackLayer.H      = S0553_SCREEN_HEIGHT;  // 全屏高度
    blackLayer.Param1 = 0;
    blackLayer.Param2 = 0;
    SendLayerToSlave(&blackLayer);
}

/* ==================== 公共接口实现 ==================== */
/**
 * @brief 设置当前主板自动程序序号（用于动态文本查表）
 */
void S0553Display_SetProgramIndex(uint8_t idx)
{
    /* 主控 → 组件 状态同步：写入数据点镜像（刷新交由 S0553Display_OnHostValueChanged） */
    S0553_Host_NotifyValue(S0553_DP_PROGRAM, idx);
}

/**
 * @brief 设置当前 UI 语言（用于动态文本查表）
 */
void S0553Display_SetLanguage(S0553_LangId lang)
{
    if (lang < S0553_LANG_MAX)
    {
        /* 主控 → 组件 状态同步：写入数据点镜像（刷新交由 S0553Display_OnHostValueChanged） */
        S0553_Host_NotifyValue(S0553_DP_LANGUAGE, (uint8_t)lang);
    }
}

/**
 * @brief 初始化显示模块
 * @details 重置所有显示状态变量为默认值
 */
void S0553Display_Init(void)
{
    s_CurrentType = S0553_DISP_TYPE_NONE;
    s_PowerState = S0553_POWER_OFF;   // 默认关机（黑屏）状态
    /* 注：原 s_LongPressCb 清零已随该回调一并删除（开关机改由 S0553_DP_POWER 数据点驱动）。 */
    // 复位延时跳转状态与开机目标
    s_TransActive = 0;
    s_TransTicks  = 0;
    s_TransDelay  = 0;
    s_TransTarget = S0553_DISP_TYPE_NONE;
    s_TransValue  = 0;
    s_PowerOnTargetPage  = S0553_BOOT_PAGE;
    s_PowerOnTargetValue = 0;
    s_NavTop = 0;
    s_Brightness = 100;

    /* 激活选中项复位为"无激活"（0xFF）：坐姿等页进入时应处于浏览态 */
    for (uint8_t i = 0; i < S0553_DISP_TYPE_MAX; i++) { s_ActiveCursor[i] = 0xFF; }

    /* 无页面上下文需初始化：页面数值由主控持有，组件在渲染/调节时按需向主控请求
       （见 S0553_Host_GetValue），Init 阶段不做任何本地值存储。 */

    /* 注册"数据变更"回调：【第二类（数据修改类）交互】下发命令后不同步等待，
       由该回调在数据真正变动时触发刷新。 */
    S0553_Host_SetValueChangedCb(S0553Display_OnHostValueChanged);

    /* 向协议层注册图片解析器：使【动态图片层也能并入 0x34 合批】
       （协议层组帧在出队时进行，届时回调本部件现算每层的图片序号）。
       不注册也能工作，但动态层会被拆成单图帧发送，帧数与延迟都会明显上升。 */
    S0553_SetLayerImageResolver(S0553Display_ResolveLayerImage);
}

/* ==================== 页面数值（经主控数据点，组件不本地存储） ==================== */
/**
 * @brief 取某页面关联数据点的当前值（按需向主控请求）
 * @return 数据点当前值；页面未声明数据点（S0553_DP_NONE）时为 0
 */
static uint8_t S0553Display_PageValue(S0553DisplayType page)
{
    const PageConfig *config = FindPageConfig(page);
    if (config == NULL) { return 0; }
    return S0553_Host_GetValue((S0553DataPoint)config->ValueDataPoint);
}

/**
 * @brief 进入"仅 Logo"中间态：显示 Logo 页并停留，不预约跳转到主页
 * @details 对应主控工作状态为【非 0 且非 5】的中间态（待机/暂停/故障/升级等）。
 *          把开机目标页设为 LOGO 自身 —— LOGO 页 OnEnter 据此跳过跳转预约（见
 *          S0553_Page_Logo.c），从而停留显示 Logo，不进入运行页。
 */
static void S0553Display_ShowLogoOnly(void)
{
    s_PowerOnTargetPage  = S0553_DISP_TYPE_LOGO;   /* LOGO 页 OnEnter 据此不预约跳转 */
    s_PowerOnTargetValue = 0;
    s_NavTop     = 0;                              /* 中间态清空导航栈 */
    s_TransActive = 0;                             /* 取消可能残留的延时跳转 */
    s_CurrentType = S0553_DISP_TYPE_NONE;          /* 确保 LOGO 页 OnEnter 触发 */
    S0553Display_Update(S0553_DISP_TYPE_LOGO, 0);
    /* Update 成功会切到 LOGO 页并把状态置 ON，这里覆盖为"仅 Logo"；
       未连接/失败时 s_CurrentType 仍为 NONE，保持原状态（与 PowerOn 一致），
       等连接后主控重推 POWER 再进入，避免状态被误标为 LOGO。 */
    if (s_CurrentType == S0553_DISP_TYPE_LOGO)
    {
        s_PowerState = S0553_POWER_LOGO;
    }
}

/**
 * @brief 【第二类交互的刷新入口】主控数据变动回推时调用
 * @details 组件下发命令后不同步等待；本函数是"数据确实变动"后触发刷新的唯一入口：
 *          - 变动的是当前页面的主数据点 → 整页刷新；
 *          - 变动的是程序/语言（影响动态文本）→ 仅重发文本层，避免整页重绘；
 *          - 关机状态或未影响当前页面 → 不刷新。
 */
static void S0553Display_OnHostValueChanged(S0553DataPoint dp, uint8_t oldValue, uint8_t newValue)
{
    /* oldValue / newValue 当前仅用于日志：日志关闭时展开为 ((void)0)，
       故显式消解"未使用参数"告警（MCU 工程常开 -Wextra）。 */
    (void)oldValue;
    (void)newValue;

    /* 【工作状态由主控推送（S0553_DP_POWER）】必须放在下面"关机不刷新"检查【之前】处理 ——
       否则关机状态下主控推来的"开机"会被那行 return 挡掉，永远开不了机。
         0     = 关机（黑屏）
         5     = 运行（开机正常使用：LOGO → 主页）
         其他  = 中间态（只显示 Logo，不进入运行页）
       仅在状态【确实切换】时才动作，避免每次推送都重放 LOGO 动画。
       ⇒ 主控【不需要】调用 S0553Display_PowerOn() / PowerOff()。 */
    if (dp == S0553_DP_POWER)
    {
        if (newValue == 0u)
        {
            if (s_PowerState != S0553_POWER_OFF)
            {
                S0553Display_PowerOff();
            }
        }
        else if (newValue == 5u)
        {
            if (s_PowerState != S0553_POWER_ON)
            {
                S0553Display_PowerOn(S0553_BOOT_PAGE, 0);
            }
        }
        else
        {
            if (s_PowerState != S0553_POWER_LOGO)
            {
                S0553Display_ShowLogoOnly();
            }
        }
        return;
    }

    if (s_PowerState != S0553_POWER_ON) { return; }   // 关机不刷新

    const PageConfig *config = FindPageConfig(s_CurrentType);
    if (config == NULL) { return; }

    if (config->ValueDataPoint == (uint8_t)dp)
    {
        LOG_DEBUG("[Host] DataPoint %d changed %d -> %d, refresh page\n", dp, oldValue, newValue);
        S0553Display_Update(s_CurrentType, newValue);
        /* 本页数据点变化 → 同步页面本地状态（如游标选中态）。
           复用 OnEnter 钩子：换页与值变化都走这里，页面据此把主控值映射到游标。 */
        if (config->OnEnter != NULL)
        {
            config->OnEnter(newValue);
        }
    }
    else if (dp == S0553_DP_PROGRAM || dp == S0553_DP_WISE || dp == S0553_DP_WISE_LOWER ||
             dp == S0553_DP_LANGUAGE ||
             dp == S0553_DP_TIME_MIN || dp == S0553_DP_TIME_SEC)
    {
        /* 动态资源依赖这些数据点（程序名/手法名与图标、语言名、按摩时间）：
           重发本页图片层（含动态图标）与文本层，避免整页重绘。
           注：RefreshThrottled 内部也会重发图片层，故动态图标能随之更新。 */
        S0553Display_RefreshThrottled(s_CurrentType);
    }
    else
    {
        // 与当前页面无关的数据点：不刷新（避免无意义重绘）
    }
}

/**
 * @brief 更新显示屏内容（注册式，非阻塞）
 * @details 完整的页面切换流程（异步，由 S0553_Process() 调度发送）：
 *          1. 参数合法性检查
 *          2. 连接状态检查
 *          3. 查找对应的页面配置
 *          4. 更新内部状态记录
 *          5. 按顺序入队所有图层指令（S0553_ShowImage/FillColor/ShowAnimation）
 *          6. 入队刷新指令（S0553_RefreshDisplay）
 *          7. 驱动一次调度，让入队指令尽快发出（不等待刷图完成）
 * 
 *          【方案2+3】不再用 delay 阻塞串等：
 *          - 图层指令全部注册式入队（FIFO），先入队的先发、先发的在下层；
 *          - 刷新指令注册在队尾，由 Process 在填图完成(0x3D)解锁后自动发送；
 *          - 本函数立即返回，不阻塞调用方。
 * 
 * @param type  显示类型枚举
 * @param value 页面初始数值，存入该页上下文，供 +/- 事件使用
 * 
 * @note  - 本函数不阻塞，可安全地频繁调用
 *        - 若type无效或未连接，函数直接返回不执行任何操作
 */
void S0553Display_Update(S0553DisplayType type, uint8_t value)
{
    // 1. 参数合法性检查
    if (type >= S0553_DISP_TYPE_MAX)
    {
        return;
    }
    
    // 2. 连接状态检查
    if (!S0553_IsConnected())
    {
        return;
    }
    
    // 3. 查找页面配置
    const PageConfig *config = FindPageConfig(type);
    if (config == NULL)
    {
        return;
    }
    
    // 3.5 记录切换前的页面：用于判断"是否真正换页"（周期刷新/同页数值更新不算换页）
    const S0553DisplayType prevType = s_CurrentType;
    /* 注：原 OnExit 钩子已移除（无页面使用）。 */
    
    // 4. 更新内部状态记录
    //    注：页面数值不再本地保存（已移除 PageContext）。显示值以页面声明的主控数据点为准，
    //    页面未声明数据点时才用调用方传入的 value 兜底（如纯静态页）。
    s_CurrentType = type;
    {
        const uint8_t pageValue = (config != NULL && config->ValueDataPoint != (uint8_t)S0553_DP_NONE)
                                  ? S0553_Host_GetValue((S0553DataPoint)config->ValueDataPoint)
                                  : value;
        /* 仅供日志：量产模式日志为空，显式消解"未使用变量"告警 */
        (void)pageValue;
        LOG_DEBUG("[Display] Update page=%d value=%d\n", type, pageValue);
    }
    s_PowerState = S0553_POWER_ON;   // 显示内容后进入开机状态
    
    // 4.5 进入新页面钩子（仅换页时触发）
    if (prevType != type && config->OnEnter != NULL)
    {
        config->OnEnter(value);
    }
    
    // 5. 按图层顺序发送；连续的图片层合并为单条 0x34（减少串口往返），
    //    遇到颜色/动画/文本层时先 flush 已累积的图片批次，再发送该非图片层，
    //    以严格保持原叠加顺序（先发的在下层，由队列 FIFO 保证）。
    {
        const DisplayLayer *imgBatch = NULL;
        uint8_t imgCount = 0;
        S0553_TextSeg textSegs[S0553_MAX_TEXT_SEG];
        uint8_t textCount = 0;
        for (uint8_t i = 0; i < config->LayerCount; i++)
        {
            const DisplayLayer *L = &config->Layers[i];
            if (L->Type == LAYER_TYPE_IMAGE)
            {
                if (textCount > 0)   // 文字须在其后图层之前发出，先 flush 文字批次
                {
                    (void)S0553_ShowMultiText(textSegs, textCount);
                    textCount = 0;
                }
                /* 图片层一律并入当前批次——【动态层也不例外】。
                   动态层过去必须单独发，是因为组帧在出队时才发生、而当时无法知道序号；
                   现在协议层会在组帧时回调 S0553Display_ResolveLayerImage() 现算，
                   因此动态层可以安全合批，整页图片合并为一条 0x34，帧数与延迟大幅下降。 */
                if (imgCount == 0)
                {
                    imgBatch = L;   // 记录本批次首张（数组连续，可整体引用）
                }
                imgCount++;
            }
            else if (L->Type == LAYER_TYPE_TEXT)
            {
                if (imgCount > 0)   // 文字前若有未发图片，先 flush 图片批次
                {
                    (void)S0553_ShowImageBatch(imgBatch, imgCount);
                    imgCount = 0;
                }
                (void)AddTextLayerToSegs(L, textSegs, &textCount);  // 累积文字段，稍后合并为单条 0x43
            }
            else
            {
                // 颜色/动画：先 flush 已累积的图片与文字批次，再发该层（严格保持叠加顺序）
                if (imgCount > 0)
                {
                    (void)S0553_ShowImageBatch(imgBatch, imgCount);
                    imgCount = 0;
                }
                if (textCount > 0)
                {
                    (void)S0553_ShowMultiText(textSegs, textCount);
                    textCount = 0;
                }
                SendLayerToSlave(L);   // 颜色/动画 走原命令（0x35/0x36）
            }
        }
        if (imgCount > 0)
        {
            (void)S0553_ShowImageBatch(imgBatch, imgCount);
        }
        if (textCount > 0)
        {
            (void)S0553_ShowMultiText(textSegs, textCount);
        }
    }
    
    // 6. 统一入队刷新指令：由 Process 在全部填图任务完成后自动发送（0x37）
    //    不再用 delay 阻塞等待
    S0553_RefreshDisplay(1);
    
    // 7. 驱动一次调度，让队列中的指令尽快发出（非阻塞，不等待刷图完成）
    S0553_Process();
}

/**
 * @brief 重发指定页面的图片层与文本层 + 补一条 0x37，用于"仅部分内容变化"的更新
 * @details 【为什么必须连背景图一起重发】0x34 填图是【整屏替换】语义：没有新的填图指令前，
 *          从机维持上一次填的内容；一旦来了新的填图指令，上一次的内容被整体换掉。
 *          因此【不存在"只补一张图 / 只发文本"的局部刷新】——只发文本会把背景清成黑屏。
 *          本函数与 S0553Display_Update 的区别仅在于【不重发非图层类内容】（动画/填色等），
 *          图层本身仍按序全部重发（动态文本在发送时按主控数据点解析最新值）。
 *          适用于旋钮旋转等"仅文本/图标变化"的高频场景：配合 S0553_PendingEnqueue 的
 *          "同位置文本合并 / 刷新去重"与 300ms 重绘限频，可消除连续转动卡顿。
 */
/** 延迟重绘请求：仅记录"需要重绘"与页面类型，真正下发在链路空闲时由 Pump 完成 */
static uint8_t s_RefreshPending = 0;
static S0553DisplayType s_RefreshType = S0553_DISP_TYPE_MAX;
/** 泵送节拍计数（每 10ms 由 S0553_Process 调用 +1）与上次重绘节拍，用于重绘限频 */
static uint16_t s_PumpTick = 0;
static uint16_t s_LastRenderTick = 0;
/** 重绘最小间隔（节拍数，10ms/节拍）：连续旋转时限频，避免重绘淹没链路导致冻结 */
#define S0553_REFRESH_MIN_TICKS 30   // 300ms

void S0553Display_RefreshThrottled(S0553DisplayType type)
{
    if (type >= S0553_DISP_TYPE_MAX)
    {
        return;
    }
    // 仅记录"需要刷新"并记住页面类型；真正下发推迟到链路空闲时（S0553_Process 调用
    // S0553Display_PumpDeferredRefresh），避免连续旋转时大量指令挤爆 8 格待发队列导致
    // 丢帧/冻结/元素消失。回调里先更新程序/语言序号，泵送时按最新序号解析动态文本。
    s_RefreshPending = 1;
    s_RefreshType = type;
    S0553Display_PumpDeferredRefresh();   // 若当前链路空闲则立即下发；否则等空闲再发
}

/**
 * @brief 链路空闲时泵送延迟的文本刷新（由 S0553_Process 每节拍调用）
 * @details 仅当：有待刷新请求 + 已连接 + 链路空闲(队列空且无在飞刷图) + 请求页仍是当前页
 *          时，才真正重发该页文本层 + 补刷新。保证队列中最多只有"一批"指令，永不溢出，
 *          且刷新(0x37)始终位于该批末尾，从机必能整屏重显，元素不会丢失。
 */
void S0553Display_PumpDeferredRefresh(void)
{
    s_PumpTick++;   // 每 10ms 节拍 +1（由 S0553_Process 调用），用于重绘限频计时

    if (!s_RefreshPending)
    {
        return;
    }
    if (!S0553_IsConnected())
    {
        s_RefreshPending = 0;
        return;
    }
    if (s_RefreshType != s_CurrentType)   // 页面已切换，丢弃过期请求
    {
        s_RefreshPending = 0;
        return;
    }
    if (!S0553_IsTxIdle())                    // 链路忙：保持 pending，等下一拍空闲再发
    {
        return;
    }
    // 【限频】连续旋转时若不做限频，重绘会一封接一封淹没链路，初始突发易把 0x3D/0x37
    // 应答冲掉 → FillTask 卡在 SENT_CMD/WAIT_COMPLETE 等 2s 看门狗 → 整屏冻结。
    // 这里把"实际重绘"限制为最多每 S0553_REFRESH_MIN_TICKS*10ms 一次：旋转序号仍逐条
    // 更新（便宜），屏幕以≤该频率显示最新值；链路因此保有充分空闲，应答不再丢失，冻结消失。
    if ((uint16_t)(s_PumpTick - s_LastRenderTick) < S0553_REFRESH_MIN_TICKS)
    {
        return;
    }

    const PageConfig *config = FindPageConfig(s_RefreshType);
    if (config == NULL)
    {
        s_RefreshPending = 0;
        return;
    }

    // 【必须】先按序重发本页所有图片层（背景/图标）——不是"为稳妥"，而是【协议要求】：
    // 0x34 填图是【整屏替换】语义（Interaction_cn.md:96：0x37 只是控制屏幕显示/关闭，
    // "没有填图，显示的是原来的图片"；一旦来了新的填图指令，上一次的内容被整体换掉）。
    // 所以不存在"只补文本 / 只补一张图"的局部刷新：只发文本会把背景清成黑屏。
    // 称其为"限频整页重绘"更贴切：内容照发全部图层，靠 300ms 限频与队列合并控制开销。
    uint8_t imgCount = 0;
    const DisplayLayer *imgBatch = NULL;
    for (uint8_t i = 0; i < config->LayerCount; i++)
    {
        const DisplayLayer *L = &config->Layers[i];
        if (L->Type == LAYER_TYPE_IMAGE)
        {
            /* 同 Update：图片层一律并入批次（动态层由协议层组帧时回调解析），不再单独发送 */
            if (imgCount == 0) imgBatch = L;   // 连续图片可整体引用
            imgCount++;
        }
        else if (imgCount > 0)
        {
            (void)S0553_ShowImageBatch(imgBatch, imgCount);
            imgCount = 0;
        }
    }
    if (imgCount > 0)
    {
        (void)S0553_ShowImageBatch(imgBatch, imgCount);
    }

    // 再重发文本层（每段一条单段 0x43；发送时按最新主控数据点(程序/语言)解析动态文本）
    S0553_TextSeg textSegs[S0553_MAX_TEXT_SEG];
    uint8_t textCount = 0;
    for (uint8_t i = 0; i < config->LayerCount; i++)
    {
        const DisplayLayer *L = &config->Layers[i];
        if (L->Type == LAYER_TYPE_TEXT)
        {
            (void)AddTextLayerToSegs(L, textSegs, &textCount);
        }
    }
    if (textCount > 0)
    {
        (void)S0553_ShowMultiText(textSegs, textCount);
    }
    S0553_RefreshDisplay(1);       // 补刷新（位于批末，确保从机整屏重显）
    s_RefreshPending = 0;
    s_LastRenderTick = s_PumpTick; // 记录本次重绘节拍，用于下一次限频
}

/**
 * @brief 显示文本（协议 0x43，注册式，非阻塞）
 * @details 在指定视窗内绘制 UTF-8 文本；组件内部转为 UTF-16 后通过 0x43 指令下发。
 *          实际发送由 S0553_Process() 调度，本函数立即返回。
 * @param text     UTF-8 文本（须为静态/字面量或生命周期覆盖发送的字符串）
 * @param x,y      视窗起点坐标
 * @param fontSize 字号（12/16/24/32，其余回退 16）
 * @param color    字体颜色（RGB565）
 * @return 1=已入队, 0=失败（未连接或队列满）
 */
uint8_t S0553Display_ShowText(const char *text, uint16_t x, uint16_t y, uint8_t fontSize, uint16_t color)
{
    return S0553_ShowText(text, x, y, fontSize, color);
}

/**
 * @brief 填充黑色全屏纯色背景（关机/默认黑屏用）
 * @details 通过协议 0x35（控制区域填颜色）指令在从机显示缓冲中生成全屏纯黑背景：
 *          - 真正覆盖从机显示缓冲，防止从机热启动或帧缓存异常时残留上一帧画面；
 *          - 通常与 S0553_RefreshDisplay(0)（0x37 关屏）配合，实现完整的关机黑屏。
 * @note   - 内部走 SendLayerToSlave() 的 LAYER_TYPE_COLOR 分支（0x35 填色）
 *         - 注册式调用（非阻塞），实际发送由 S0553_Process() 调度
 *         - 必须在模块已连接状态下调用
 * @retval 无
 */
void S0553Display_FillBlackBackground(void)
{
    // 连接状态检查
    if (!S0553_IsConnected())
    {
        return;
    }
    SendBlackBackground();
}

/**
 * @brief 清除显示屏内容
 * @details 关机黑屏流程：
 *          1. 通过 0x35 填充黑色全屏纯色背景（真正覆盖从机显示缓冲）
 *          2. 通过 0x37 关闭显示（保留从机帧缓存，省电）
 *          再次调用S0553Display_Update()时会重新显示。
 * 
 * @note  - 适用于待机、省电或错误提示场景
 *        - 若未连接，函数直接返回
 */
void S0553Display_Clear(void)
{
    // 连接状态检查
    if (!S0553_IsConnected())
    {
        return;
    }
    
    // 【关机黑屏】先通过 0x35 填充黑色全屏纯色背景，
    // 真正覆盖从机显示缓冲，防止热启动/帧缓存残留上一帧画面
    SendBlackBackground();
    
    // 关闭显示
    S0553_RefreshDisplay(0);
    
    // 重置内部状态
    s_CurrentType = S0553_DISP_TYPE_NONE;
    s_PowerState = S0553_POWER_OFF;   // 清屏即进入关机（黑屏）状态
    // 取消可能正在进行的延时跳转（关机打断计时）
    s_TransActive = 0;
    s_TransTicks  = 0;
    s_NavTop = 0;                       // 关机清空导航栈
}

/**
 * @brief 开机并显示指定页面
 * @details 从黑屏（关机）状态恢复显示。
 *          内部等价于 S0553Display_Update(page, value)，更新成功后
 *          电源状态自动切换为开机。
 * @param page  开机后默认显示的页面类型（NONE/无效值将回退到电源页）
 * @param value 页面初始数值
 * @note  必须在已连接状态下调用
 */
void S0553Display_PowerOn(S0553DisplayType page, uint8_t value)
{
    // 页面合法性兜底：不允许开机显示空页，回退到 UI 配置的首页（S0553_BOOT_PAGE）
    if (page == S0553_DISP_TYPE_NONE || page >= S0553_DISP_TYPE_MAX)
    {
        page = S0553_BOOT_PAGE;
    }
    // 记录本次开机目标，供开机页(S0553_SPLASH_PAGE)的 OnEnter 读取并预约停留后跳转
    s_PowerOnTargetPage  = page;
    s_PowerOnTargetValue = value;
    s_NavTop = 0;                       // 开机为全新起点，清空导航栈
    // 确保开机页的 OnEnter 必定触发（若当前已在开机页，重置后可再次"进入"）
    s_CurrentType = S0553_DISP_TYPE_NONE;
    S0553Display_Update(S0553_SPLASH_PAGE, 0);
}

/* ==================== 延时跳转（供页面 OnEnter 预约） ==================== */
void S0553Display_RequestTransition(S0553DisplayType targetPage, uint8_t value, uint16_t delayTicks)
{
    s_TransTarget = targetPage;
    s_TransValue  = value;
    s_TransDelay  = delayTicks;
    s_TransTicks  = 0;
    s_TransActive = 1;
}

S0553DisplayType S0553Display_GetPowerOnTargetPage(void)
{
    return s_PowerOnTargetPage;
}

uint8_t S0553Display_GetPowerOnTargetValue(void)
{
    return s_PowerOnTargetValue;
}

/* 注：原"页面多值槽位接口"(Get/SetPageSlotValue、SetPageSlotRange) 已移除——
   数值改由主控数据点提供，不再有"每页多槽位"概念。 */

/* ==================== 导航栈接口 ==================== */
void S0553Display_NavReset(void)
{
    s_NavTop = 0;
}

uint8_t S0553Display_NavDepth(void)
{
    return s_NavTop;
}

/**
 * @brief 关机（进入黑屏状态）
 * @details 关闭屏幕显示并重置内部状态，屏幕变黑。
 *          内部等价于 S0553Display_Clear()，清屏后电源状态自动切换为关机。
 * @note  必须在已连接状态下调用
 */
void S0553Display_PowerOff(void)
{
    S0553Display_Clear();
}

/**
 * @brief 接收主控运行状态改变，使组件随之开关机
 * @details 主控决策后调用：
 *          - S0553_POWER_ON : 开机显示（当前为黑屏时显示默认电源页）
 *          - S0553_POWER_OFF: 关机黑屏
 */
void S0553Display_SetPowerState(S0553PowerState state)
{
    if (state == S0553_POWER_ON)
    {
        // 开机：正常显示（LOGO → 主页）
        S0553Display_PowerOn(S0553_BOOT_PAGE, 0);
    }
    else if (state == S0553_POWER_LOGO)
    {
        // 中间态：只显示 Logo
        S0553Display_ShowLogoOnly();
    }
    else
    {
        // 关机：进入黑屏
        S0553Display_PowerOff();
    }
}

/**
 * @brief 查询当前电源状态（关机黑屏/开机显示）
 */
S0553PowerState S0553Display_GetPowerState(void)
{
    return s_PowerState;
}

/**
 * @brief 查询是否处于开机（显示）状态
 */
uint8_t S0553Display_IsPowerOn(void)
{
    /* "开机/有显示"含仅 Logo 态：黑屏才算关机 */
    return (s_PowerState != S0553_POWER_OFF) ? 1 : 0;
}

/**
 * @brief 查询当前显示的页面类型（黑屏时为 NONE）
 */
S0553DisplayType S0553Display_GetCurrentPage(void)
{
    return s_CurrentType;
}

/**
 * @brief 查询当前显示数值（最近一次 Update/Clear 记录的值）
 * @return 当前页面值；Clear/关机后为 0
 */
uint8_t S0553Display_GetCurrentValue(void)
{
    // 当前值由页面上下文推导（s_CurrentType 恒为合法枚举，索引安全）
    return S0553Display_PageValue(s_CurrentType);
}

/**
 * @brief 查询当前背光亮度（主控最近一次 SetBrightness 请求的值）
 * @return 亮度值，范围[20, 250]；未设置过为默认 100
 */
uint8_t S0553Display_GetBrightness(void)
{
    return s_Brightness;
}

/**
 * @brief 查询当前氛围灯模式（主控最近一次成功设置的配置）
 */
AmbientLightMode S0553Display_GetAmbientMode(void)
{
    return s_AmbientMode;
}

/**
 * @brief 查询当前氛围灯颜色（主控最近一次成功设置的配置）
 */
RGBColor S0553Display_GetAmbientColor(void)
{
    return s_AmbientColor;
}

/**
 * @brief 查询当前氛围灯变换周期（毫秒，主控最近一次成功设置的配置）
 */
uint16_t S0553Display_GetAmbientPeriod(void)
{
    return s_AmbientPeriod;
}

/**
 * @brief 设置屏幕背光亮度
 * @details 发送亮度设置命令（0x39）到从机，立即生效。
 *          亮度值会自动限制在有效范围[20, 250]内。
 * 
 * @param level 目标亮度值
 *              - < 20:  自动修正为20（最暗）
 *              - > 250: 自动修正为250（最亮）
 *              - 推荐值: 100-150（平衡功耗和可视性）
 * 
 * @note  - 亮度值会被记录（供 S0553Display_GetBrightness 查询）；
 *           若未连接则仅记录、不发送指令
 *        - 亮度设置独立于页面显示，不会触发页面刷新
 */
void S0553Display_SetBrightness(uint8_t level)
{
    // 限制亮度范围
    if (level < 20)
    {
        level = 20;
    }
    if (level > 250)
    {
        level = 250;
    }

    // 更新内部记录（未连接时也记录，供 GetBrightness 查询/断线重连补发）
    s_Brightness = level;
    
    // 若已连接，发送亮度设置命令
    if (S0553_IsConnected())
    {
        uint8_t data[2] = {0x00, level};
        S0553_SendData(S0553_CMD_SET_BRIGHTNESS, data, 2);
    }
}

/**
 * @brief 控制氛围灯效果
 * @param mode      灯效模式
 * @param color     RGB颜色
 * @param period_ms 变换周期（毫秒）
 * @return 1=成功, 0=失败
 */
uint8_t S0553Display_SetAmbientLight(AmbientLightMode mode, RGBColor color, uint16_t period_ms)
{
    // 检查连接状态
    if (!S0553_IsConnected())
    {
        return 0;
    }
    
    // 限制周期范围（100-5000ms）
    if (period_ms > 5000)
    {
        period_ms = 5000;
    }

    // 更新内部记录（供 S0553Display_GetAmbient* 查询；仅连接成功时更新）
    s_AmbientMode = mode;
    s_AmbientColor = color;
    s_AmbientPeriod = period_ms;
    
    // 构建数据帧
    // 格式：[状态代号][R][G][B][模式][周期高][周期低]
    uint8_t data[7] = {
        0x00,                       // 状态代号
        color.R,                    // 红色分量
        color.G,                    // 绿色分量
        color.B,                    // 蓝色分量
        (uint8_t)mode,              // 灯效模式
        (uint8_t)(period_ms >> 8),  // 周期高字节
        (uint8_t)(period_ms & 0xFF) // 周期低字节
    };
    
    S0553_SendData(S0553_CMD_AMBIENT_LIGHT, data, 7);
    
    return 1;
}

/**
 * @brief 快速设置氛围灯为固定颜色
 * @param color RGB颜色
 * @return 1=成功, 0=失败
 */
uint8_t S0553Display_SetAmbientColor(RGBColor color)
{
    return S0553Display_SetAmbientLight(AMBIENT_MODE_FIXED_COLOR, color, 0);
}

/**
 * @brief 关闭氛围灯
 * @return 1=成功, 0=失败
 */
uint8_t S0553Display_TurnOffAmbientLight(void)
{
    RGBColor black = {0, 0, 0};
    return S0553Display_SetAmbientLight(AMBIENT_MODE_OFF, black, 0);
}

/**
 * @brief 周期性刷新显示内容（周期保活/防卡屏刷新）
 * @details 从机帧缓存保存着当前页面的完整图层，因此周期刷新不必整页重发。
 *          这里只重发一条 0x37 开屏刷新指令，让从机把缓存内容重新显示出来；
 *          而这条 0x37 恰好就是链路探活帧（见 S0553_main.c S0553_Process 2.5 块），
 *          一条指令同时完成"屏幕刷新 + 链路保活"，大幅节约通信量与交互时间。
 * 
 * @note  - 建议按 10s 节拍调用（现已由 S0553_Task() 内置，每 1000 个 10ms tick 一次）
 *        - 仅开机且有显示内容时执行；关机/黑屏保持黑屏，不重新点亮
 *        - 代价：不再周期性整页重发图层，若从机帧缓存被干扰/损坏，需等下一次
 *          页面切换(S0553Display_Update)才会自愈，可结合通信环境评估是否保留
 */
void S0553Display_Refresh(void)
{
    // 仅在开机且有显示内容时刷新（关机黑屏时保持黑屏，不重新点亮；仅 Logo 态也需刷新兼探活）
    if (s_PowerState != S0553_POWER_OFF && s_CurrentType != S0553_DISP_TYPE_NONE)
    {
        // 周期刷新只发 0x37(enable=1)：0x37 是"控制屏幕显示/关闭"，【不是刷新内容】；
        // 没有新的填图指令前，从机维持上一次填的内容，故此处无需重发任何图层。
        // （早期为每周期整页重绘 Update(...)，通信量大，已废弃。）
        // 该 0x37 同时兼作探活帧，触发掉线检测窗口。
        /* 注：原 OnRefresh 钩子已移除（无页面使用）。 */
        S0553_RefreshDisplay(1);
    }
}

/**
 * @brief 处理按键事件并自动执行对应动作
 * @details 这是新增的核心接口，应在主循环中调用
 *          当收到按键事件时，自动查找当前页面的事件映射表并执行对应动作
 * 
 * @param event 按键事件结构
 * @note  - 必须在模块已连接状态下调用
 *        - 如果当前页面对应的事件没有配置，则忽略该事件
 *        - 建议在 S0553_Process() 之后调用
 */
void S0553Display_HandleKeyEvent(const S0553KeyEvent *event)
{
    if (event == NULL || !S0553_IsConnected())
    {
        return;
    }

    /* 收到有效按键即视为"用户操作"：清零无操作看护计时（NONE=无按键，不算操作） */
    if (event->KeyState != S0553_KEY_NONE)
    {
        s_IdleTicks = 0;
    }
    
    /* 注：原"旋钮长按(0x02)上报主控"分支已删除 —— 开关机改由【S0553_DP_POWER】数据点驱动。
       长按事件(S0553_KEY_PHYSICAL_LONG)现在与其它事件一样，走【当前页面的事件表】处理；
       黑屏页(NONE)事件表为空，故黑屏下的长按自动忽略。 */
    
    // 按键事件：按当前页面事件表处理
    // 黑屏页(NONE)事件表为空，黑屏下除长按外的事件自动忽略
    
    // 查找当前页面的配置
    const PageConfig *config = FindPageConfig(s_CurrentType);
    if (config == NULL)
    {
        return;
    }
    
    // 查找该按键事件对应的动作（未配置的事件直接忽略）
    const EventAction *action = FindEventAction(config, event->KeyState);
    if (action != NULL)
    {
        ExecuteAction(action);
    }
}

/**
 * @brief S0553模块统一任务处理函数
 * @details 集成通信处理与UI事件分发，简化主控调用
 */
/**
 * @brief 显示层周期节拍（随 S0553_Task / S0553_Process 每 10ms 调用）
 * @details ① 处理开机 Logo 2 秒过渡：计时到后自动从 Logo 切到目标页；
 *          ② 调用当前页的周期钩子 OnTick（如坐姿页的"停手检测"）；
 *          ③ 无操作看护：工作状态=5(运行)且非主页时，累计无按键时间，超时自动返回主页。
 */
void S0553Display_Tick(void)
{
    /* 1. 开机 Logo 过渡计时 */
    if (s_TransActive)
    {
        if (s_TransTicks < 0xFFFF)
        {
            s_TransTicks++;
        }
        if (s_TransTicks >= s_TransDelay)
        {
            s_TransActive = 0;
            S0553Display_Update(s_TransTarget, s_TransValue);  // 切到预约的目标页
        }
    }

    /* 2. 当前页周期钩子（每 10ms）：供"停手检测"等持续计时逻辑使用 */
    const PageConfig *config = FindPageConfig(s_CurrentType);
    if (config != NULL && config->OnTick != NULL)
    {
        config->OnTick();
    }

    /* 3. 无操作看护：工作状态=5(运行)且非主页时累计无按键时间，满 10s 自动返回主页 */
    if (S0553_Host_GetValue(S0553_DP_POWER) == 5u &&
        s_CurrentType != S0553_DISP_TYPE_MAIN &&
        s_CurrentType != S0553_DISP_TYPE_NONE)
    {
        if (s_IdleTicks < S0553_IDLE_HOME_TICKS) { s_IdleTicks++; }
        if (s_IdleTicks >= S0553_IDLE_HOME_TICKS)
        {
            s_IdleTicks = 0;
            S0553Display_NavReset();                        /* 清栈，避免按返回逐层退回 */
            (void)S0553Display_SwitchPage(S0553_DISP_TYPE_MAIN);
        }
    }
    else
    {
        s_IdleTicks = 0;   /* 非运行态或在主页：不计时 */
    }
}

/* 周期刷新间隔：1000 个 tick × 10ms = 10s（【已连接】时周期性刷新显示，0x37 兼作探活帧） */
#define S0553_PERIODIC_TICKS   1000u

/* 连接重试间隔：200 个 tick × 10ms = 2s（【未连接】时自动发起 / 重试连接） */
#define S0553_RECONNECT_TICKS   200u

static uint16_t s_PeriodicTicks = 0;

/* 初值设为"已到期"：使上电后【第一次调用 S0553_Task() 就立即发起连接】，
   主控因此【不必】先调 S0553_Connect()，也不必等一个重试周期。 */
static uint16_t s_ReconnectTicks = S0553_RECONNECT_TICKS;

void S0553_Task(void)
{
    // 1. 处理底层通信（接收、解析、状态机维护）
    S0553_Process();

    // 2. 自动处理所有待处理的按键事件
    S0553KeyEvent event;
    while (S0553_PopKeyEvent(&event))
    {
        S0553Display_HandleKeyEvent(&event);
    }

    // 3. 开机 Logo 2 秒过渡计时（每 10ms tick 调用）
    S0553Display_Tick();

    /* 4. 连接管理（组件自治）：未连接时每 2s 自动发起 / 重试连接；
        S0553_Connect() 自带状态守卫（仅在 IDLE 时发握手帧），可放心周期调用。 */
    if (!S0553_IsConnected())
    {
        if (++s_ReconnectTicks >= S0553_RECONNECT_TICKS)
        {
            s_ReconnectTicks = 0;
            S0553_Connect();
        }
        s_PeriodicTicks = 0;
    }
    else
    {
        /* 已连接：把重试计数器保持在"已到期"，这样一旦掉线，
           下一次调用本函数就会【立即】重连，不必等满 2s。 */
        s_ReconnectTicks = S0553_RECONNECT_TICKS;

        /* 5. 【周期刷新（组件自治）】每 10s 刷新一次显示（0x37 兼作探活帧） */
        if (++s_PeriodicTicks >= S0553_PERIODIC_TICKS)
        {
            s_PeriodicTicks = 0;
            S0553_DisplayRefresh();
        }
    }
    /* 前提：主控须按【10ms】周期调用本函数（MCU 严格准时；PC 受 Sleep 分辨率影响会偏慢，
       但偏慢对周期刷新 / 重连无害，且与组件内部计时同比例）。 */
}

/* 启动只需 S0553_Init()：连接由 S0553_Task() 自治（首次调用即发起，未连接每 2s 重试）。 */

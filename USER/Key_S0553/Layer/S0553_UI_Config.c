/***************************************************************************************************
* @file      S0553_UI_Config.c
* @brief     S0553 页面注册中心 + 动态文本资源
* @details   本文件只做两件事：
*            1) 由 S0553_PAGE_LIST 自动生成 UI_PAGE_MAP（新增页面无需改本文件）；
*            2) 提供动态文本资源解析与页面表访问接口。
*            各页面的 UI 图层 / 事件 / 执行逻辑已按页拆分到 S0553_Page_<Name>.c，
*            本文件通过 S0553_Pages.h（由清单自动生成的 extern 声明）引用它们。
*            要做"夜间模式"/"简易模式"：新建一套页面文件，在初始化时切换配置表指针即可换肤。
================================================================***********************************/
#include "S0553_UI_Config.h"
#include "S0553_Pages.h"        // 各页符号的 extern 声明（由页面清单自动生成）
#include "S0553_ImageTable.h"   // 图片序号（动态图片资源解析用）
#include "S0553_Display.h"      // S0553Display_GetUiCursor()：菜单/列表的选中游标

/* ==================== 全局配置表（由 S0553_PAGE_LIST 自动生成，勿手工编辑） ==================== */
/** @brief 数组元素个数（替代手工 sizeof/除法，杜绝复制粘贴改错数组名） */
#define S0553_ARRAY_SIZE(a)  (sizeof(a) / sizeof((a)[0]))

/* ---- 一条清单行 = 一个页面描述符指针 ---- */
#define S0553_PAGE_PTR(id, sym)  &sym##_Page,

/* 页面指针表：各页在自己文件内把 LayerCount/EventCount 用 sizeof 算好并打包成 <SYM>_Page，
   中心表只存指针（sizeof 无法跨编译单元作用于 extern const X[]）。 */
static const PageConfig *const UI_PAGE_MAP[] = {
    S0553_PAGE_LIST(S0553_PAGE_PTR)
    /* 新增页面请改 S0553_PageList.h 并新建 S0553_Page_<SYM>.c，本表自动生成 */
};
#undef S0553_PAGE_PTR

/* 编译期防漏配：清单条目数必须等于 S0553_DISP_TYPE_MAX，少配/多配直接编译失败 */
typedef char S0553_PageCountCheck[(S0553_ARRAY_SIZE(UI_PAGE_MAP) == S0553_DISP_TYPE_MAX) ? 1 : -1];

/* ==================== 接口实现 ==================== */
const PageConfig *const* S0553UI_GetPageConfigTable(void) {
    return UI_PAGE_MAP;
}

uint8_t S0553UI_GetPageConfigCount(void) {
    return S0553_ARRAY_SIZE(UI_PAGE_MAP);
}

/* ==================== 动态文本资源 ==================== */

/**
 * @brief 主页面"当前自动程序名称"资源表：[程序序号(0-based，=自动程序编号-1)][语言]
 * @note  语言维度 = S0553_LANG_MAX(当前 4 种：CN/EN/DE/FR，顺序对应协议语言序号；
 *        其余 17 种语言在 S0553_Display.h 的 S0553_LangId 中注释保留)。中文名/英/德/法
 *        按用户提供的 1~25 自动程序清单填入（注释数字=自动程序编号）；部分程序在
 *        英/德/法语下无翻译，留空串→运行时按空文本处理、不发送该段。
 *        源数据中的 `[size=20]` 字号标记已剥离（本框架文本字号由图层 Param1 统一控制，
 *        无逐字符串字号机制；如需按程序动态调字号需另行扩展）；
 *        空格占位("   ")按空字符串处理为不显示该语言文本（触发空文本不发送）。
 *        新增语言需在 S0553_LangId 与 S0553_LANG_MAX 同步扩展。
 *        programIndex 越界回退到 0 号程序。
 */
/* 行数不写死（原为 S0553_PROGRAM_MAX）：由表自身 sizeof 得出，避免"常量与表不一致"。
   【非 static】：供自动程序子页（AutoProgram1~4）的 PageConfig.ItemNames 用【指针偏移】
   直接复用（extern 声明见 S0553_UI_Config.h），避免各页复制一份占位名字表。 */
const char* s_MainProgramName[][S0553_LANG_MAX] =
{
    /* 1  */  { "零压舒展", "Total Relax",                 "Tiefenentspannung",       "Relaxation Totale" },
    /* 2  */  { "中式推拿", "Chinese Massage",             "Chinesische Massage",     "Massage Chinois" },
    /* 3  */  { "疲劳恢复", "Fatigue Recovery",            "Regeneration",            "Récupération après Fatigue" },
    /* 4  */  { "海浪轻揉", "Gentle Waves Rub",            "Sanftes Wellenreiben",    "Effleurages en Douces Vagues" },
    /* 5  */  { "泰式拉筋", "Thai Massage",                "Thaimassage",             "Massage Thaïlandais" },
    /* 6  */  { "舒筋活络", "Meridian Treatment",          "Meridian-Therapie",       "Traitement des Méridien" },
    /* 7  */  { "肩颈护理", "Neck & Shoulder Care",         "Nacken- und Schulterpflege","Soin Nuque & Épaules" },
    /* 8  */  { "提臀美腿", "Hips & Legs Fit",             "Hüft- und Beintraining",  "Tonification Hanches & Jambes" },
    /* 9  */  { "腰部护理", "Waist Care",                  "Taillenpflege",           "Soin de la Taille" },
    /* 10 */  { "腿足舒缓", "Legs & Feet Relieve",          "Bein- und Fußentspannung","Soulagement Jambes & Pieds" },
    /* 11 */  { "天伦养生", "Superior Enjoyable",          "Überragendes Vergnügen",  "Confort Supérieur" },
    /* 12 */  { "总裁释压", "President Cosy",              "Präsidenten-Komfort",    "Détente Absolue" },
    /* 13 */  { "女王纤体", "Body Shaping",               "Figurformung",           "Remodelage Corporel" },
    /* 14 */  { "白领专享", "Office Regimen",              "Büro-Entspannungsprogramm","Routine de Bureau" },
    /* 15 */  { "御宅充电", "Health Recharge",             "Gesundheitserholung",     "Recharge Santé" },
    /* 16 */  { "元气唤醒", "Energy Boost",                "Energie-Kick",            "Coup de Boost" },
    /* 17 */  { "午间小憩", "Lunch Break",                 "Mittagspause",            "Pause de Midi" },
    /* 18 */  { "温热祛湿", "Warming and Detox",           "Wärme und Entgiftung",    "Chaleur & Détox" },
    /* 19 */  { "运动辅助", "Sport Recovery",              "Sport-Erholung",          "Récupération Sportive" },
    /* 20 */  { "安神助眠", "Sweet Dream",                 "Süße Träume",             "Sommeil Tranquil" },
    /* 21 */  { "御手解忧", "",                            "",                       "" },
    /* 22 */  { "正脊正骨", "",                            "",                       "" },
    /* 23 */  { "固本培元", "",                            "",                       "" },
    /* 24 */  { "调和阴阳", "",                            "",                       "" },
    /* 25 */  { "酸痛改善", "",                            "",                       "" },
};

/* 实际程序数量 = 程序名表行数（由表自身得出，新增/删除程序只改表即可） */
#define S0553_PROGRAM_NAME_COUNT  ((uint8_t)(sizeof(s_MainProgramName) / sizeof(s_MainProgramName[0])))

/**
 * @brief 语言名（英文）资源表：[语言]，用于测试页显示当前语言标识
 * @note  顺序严格对应 S0553_LangId 枚举，表尺寸跟随 S0553_LANG_MAX（勿写死数字）。
 *        当前枚举仅开放 CN/EN/DE/FR 四种（其余语言在 S0553_Display.h 的 S0553_LangId
 *        中注释保留），故本表只保留对应 4 项；后续启用某语言时，把枚举与下表对应条目
 *        一并取消注释即可（两者顺序必须保持一致）。
 */
static const char* s_LangName[S0553_LANG_MAX] =
{
    "Chinese", "English", "German", "French",
    // 以下与 S0553_LangId 中注释保留的语言按序一一对应，启用时同步取消注释：
    // "Italian", "Portuguese", "Vietnamese",
    // "Romanian", "Hungarian", "Dutch", "Spanish",
    // "Bulgarian", "Danish", "Russian",
    // "Uzbek", "Thai", "Ukrainian", "Polish",
    // "Croatian", "Chinese(TW)", "Japanese"
};

/**
 * @brief 手法名表（0-based，与 S0553_DP_WISE 取值一一对应）
 * @note  与程序名表同构：第 2 维按 S0553_LangId 展开（CN/EN/DE/FR），
 *        当前仅有中文，其余语言留【空串占位】（触发"空文本不发送"），
 *        译文到位后直接填入对应列即可，无需改代码。
 *        项数 12 与手动图标 IMG_MAIN_MANUAL_ICON_COUNT 一致（一手法一图标）。
 *        【非 static】：供手动手法页（ManualItem1/2）的 PageConfig.ItemNames 直接复用，
 *        避免同一份手法名在多处复制导致不一致（extern 声明见 S0553_UI_Config.h）。
 */
const char* s_WiseName[][S0553_LANG_MAX] =
{
    /* 0  */  { " 无手法",    "", "", "" },
    /* 1  */  { "  揉捏",      "", "", "" },
    /* 2  */  { "  拍打1",     "", "", "" },
    /* 3  */  { "  拍打2",     "", "", "" },
    /* 4  */  { "揉捏拍打",  "", "", "" },
    /* 5  */  { " 指压1",     "", "", "" },
    /* 6  */  { " 指压2",     "", "", "" },
    /* 7  */  { "肩部提抓",  "", "", "" },
    /* 8  */  { " 4D I",      "", "", "" },
    /* 9  */  { " 4D II",     "", "", "" },
    /* 10 */  { "4D III",    "", "", "" },
    /* 11 */  { " 4D IV",     "", "", "" },
};

/* 【通用】列表页末项"返回主页"的统一名字。
   各列表页的 ItemNames 不再各自写"返回主页"这一项 —— 解析函数对末项
   （cursor == UiItemCount-1）统一返回本表，消除每页重复且避免措辞不一致。 */
static const char* s_BackHomeName[S0553_LANG_MAX] = {
    "返回主页", "Home", "Startseite", "Accueil"
};

/* 实际手法数量 = 手法名表行数（由表自身得出，增删手法只改表） */
#define S0553_WISE_NAME_COUNT  ((uint8_t)(sizeof(s_WiseName) / sizeof(s_WiseName[0])))

static const char* S0553UI_WiseName(uint8_t wise, S0553_LangId lang)
{
    if (wise >= S0553_WISE_NAME_COUNT) { return ""; }   /* 越界 → 不显示（触发空文本不发送） */
    if (lang >= S0553_LANG_MAX)        { lang = S0553_LANG_CN; }   /* 越界/不支持 → 中文 */
    return s_WiseName[wise][lang];
}

/* 两位数字格式化缓冲（时间分/秒）。返回静态缓冲，调用方无需释放。
   刻意不用 snprintf：避免量产代码引入 stdio 依赖。 */
static char s_TimeMinStr[3];
static char s_TimeSecStr[3];
/** "mm:ss" + '\0'（组合显示用；静态缓冲——返回的指针须长期有效） */
static char s_TimeMmssStr[6];

static const char* S0553UI_Format2Digits(uint8_t v, char buf[3])
{
    buf[0] = (char)('0' + ((v / 10) % 10));
    buf[1] = (char)('0' + (v % 10));
    buf[2] = '\0';
    return buf;
}

/* 前向声明：定义在文件后部。图片解析需按【当前页】取其大图表，
   与文本 TEXTRES_CURSOR_ITEM_NAME 的做法一致。 */
static const PageConfig *S0553UI_FindPageConfig(S0553DisplayType type);

uint16_t S0553UI_ResolveImage(uint16_t imgResId, uint8_t programIndex, S0553_LangId lang,
                              uint8_t itemIndex, uint16_t altImageId, uint16_t defaultImageId)
{
    (void)lang;   /* 当前图片与语言无关；若日后有语言相关图标，在此分支处理 */

    switch (imgResId)
    {
        case IMGRES_MAIN_PROGRAM_ICON:
        {
            /* 与文本 TEXTRES_MAIN_PROGRAM_NAME 同源同判据：
                 程序 != 0 → 自动程序，序号 1..N 对应 ICON_1..（表索引 = 程序号-1）
                 程序 == 0 → 手动模式，按【手法】(S0553_DP_WISE) 选手动图标
               两类图标 ID 各自连续，故直接用"首图 + 序号"计算。 */
            const uint8_t prog = programIndex;
            if (prog == 0)
            {
                const uint8_t w = S0553_Host_GetValue(S0553_DP_WISE);
                /* 手法越界 → 不画（返回 INVALID 让调用方整层跳过） */
                if (w >= IMG_MAIN_MANUAL_ICON_COUNT) { return S0553_IMGRES_INVALID; }
                return (uint16_t)(IMG_MAIN_MANUAL_PROGRAM_ICON_1 + w);
            }
            const uint8_t p = (uint8_t)(prog - 1);
            /* 程序越界（如 21+ 或 255/-1）→ 不画，与文字"不显示"保持一致 */
            if (p >= IMG_MAIN_AUTO_ICON_COUNT) { return S0553_IMGRES_INVALID; }
            return (uint16_t)(IMG_MAIN_AUTO_PROGRAM_ICON_1 + p);
        }
        /* 【通用】本页中间的大图标：显示【当前选中子菜单项】的大图。
           与文本 TEXTRES_CURSOR_ITEM_NAME 完全对称：大图表由【页面】自持
           （PageConfig.ItemLargeIcons），下标 = 本页 UI 游标，
           故旋转移动游标 → 中间大图随之变化；新增列表页不必改枚举与本函数。
           不假设图号连续（烧录表由用户手工维护），故按表逐项查而非"首图+游标"。
           未配大图表 / 越界 → 返回 INVALID（整层不画），与文字"不显示"行为一致。 */
        case IMGRES_ITEM_LARGE_ICON:
        {
            const PageConfig *cfg = S0553UI_FindPageConfig(S0553Display_GetCurrentPage());
            if (cfg == NULL || cfg->ItemLargeIcons == NULL) { return S0553_IMGRES_INVALID; }

            const uint8_t cursor = S0553Display_GetUiCursor();
            if (cursor >= cfg->UiItemCount) { return S0553_IMGRES_INVALID; }

            return cfg->ItemLargeIcons[cursor];
        }
        /* 【通用】本页列表项的【小图标】：显示第 itemIndex 项的小图标。
           与 IMGRES_ITEM_LARGE_ICON 对称：小图标表由【页面】自持
           （PageConfig.ItemIcons），下标 = 图层 Index（项序），
           故新增列表页不必改枚举与本函数；未配表 / 越界 → 不画。 */
        case IMGRES_ITEM_ICON:
        {
            const PageConfig *cfg = S0553UI_FindPageConfig(S0553Display_GetCurrentPage());
            if (cfg == NULL || cfg->ItemIcons == NULL) { return S0553_IMGRES_INVALID; }
            if (itemIndex >= cfg->UiItemCount) { return S0553_IMGRES_INVALID; }
            return cfg->ItemIcons[itemIndex];
        }
        /* 【通用】列表项选中 / 未选中：本层代表第 itemIndex 项。
             选中   → 图层的 AltId（页面自持"选中图"，引擎不硬编码素材）
             未选中 → 图层的 Id（默认图）
           一个 ID 适用于所有列表页：新增列表项【只在页面多填一层】，
           不必改枚举、也不必改本函数。 */
        case IMGRES_ITEM_STATE:
            return (S0553Display_GetUiCursor() == itemIndex) ? altImageId : defaultImageId;
        /* 【通用】中间大图-选中态叠加：
           · 有数据点页（零重力/自动程序/手动手法）：主控数据点值 == 游标对应业务值
             （cursor + ValueOffset）时选中 —— 状态由主控驱动，组件不存储；
             【特例】手动手法（WISE/WISE_LOWER）还需"程序 == 0"（手动模式）才选中：
              自动程序下手动手法不生效、选中态隐藏。
           · 无数据点页（坐姿等纯命令页）：由组件本地"激活选中项"驱动 —— 游标即激活项时选中。
           命中时在中间大图上叠加该游标项的选中态大图（按 ItemLargeIconsSelected 查表），
           否则整层不画、保持默认图。 */
        case IMGRES_ITEM_LARGE_ICON_SELECTED:
        {
            const PageConfig *cfg = S0553UI_FindPageConfig(S0553Display_GetCurrentPage());
            if (cfg == NULL || cfg->ItemLargeIconsSelected == NULL) { return S0553_IMGRES_INVALID; }

            const uint8_t cursor = S0553Display_GetUiCursor();
            if (cursor >= cfg->UiItemCount) { return S0553_IMGRES_INVALID; }

            if (cfg->ValueDataPoint != (uint8_t)S0553_DP_NONE)
            {
                /* 有数据点：主控数据 == 游标对应业务值时选中 */
                const uint8_t value = S0553_Host_GetValue((S0553DataPoint)cfg->ValueDataPoint);
                if (value != (uint8_t)(cursor + cfg->ValueOffset)) { return S0553_IMGRES_INVALID; }

                /* 【手动手法】仅手动模式（程序 == 0）才显示选中态：自动程序下
                   上/下机芯手法不生效，选中态应隐藏（由主控程序数据驱动）。 */
                if ((cfg->ValueDataPoint == (uint8_t)S0553_DP_WISE ||
                     cfg->ValueDataPoint == (uint8_t)S0553_DP_WISE_LOWER) &&
                    S0553_Host_GetValue(S0553_DP_PROGRAM) != 0u)
                {
                    return S0553_IMGRES_INVALID;
                }
            }
            else
            {
                /* 无数据点（坐姿）：组件本地"激活选中项"驱动 */
                if (S0553Display_GetActiveCursor() != cursor) { return S0553_IMGRES_INVALID; }
            }

            return cfg->ItemLargeIconsSelected[cursor];
        }
        default:
            return S0553_IMGRES_INVALID;   /* 未定义 → 调用方回退图层静态 Id */
    }
}

/* ==================== 页面配置查找（动态资源解析按"当前页"取配置） ==================== */
/**
 * @brief 按页面类型取页面配置（本中心表是自动生成的，遍历即可）
 * @param type 页面类型
 * @return const PageConfig* 找到的配置；未找到返回 NULL
 */
static const PageConfig *S0553UI_FindPageConfig(S0553DisplayType type)
{
    const PageConfig *const *map = S0553UI_GetPageConfigTable();
    const uint8_t            cnt = S0553UI_GetPageConfigCount();

    for (uint8_t i = 0; i < cnt; i++)
    {
        if (map[i] != NULL && map[i]->PageType == type) { return map[i]; }
    }
    return NULL;
}

const char* S0553UI_ResolveText(uint16_t textId, uint8_t programIndex, S0553_LangId lang)
{
    if (lang >= S0553_LANG_MAX) lang = S0553_LANG_CN;   /* 越界/不支持 → 中文 */

    switch (textId)
    {
        /* 按摩时间：直接取主控数据点（组件只显示，倒计时由主控维护） */
        case TEXTRES_TIME_MIN:
            return S0553UI_Format2Digits(S0553_Host_GetValue(S0553_DP_TIME_MIN), s_TimeMinStr);
        case TEXTRES_TIME_SEC:
            return S0553UI_Format2Digits(S0553_Host_GetValue(S0553_DP_TIME_SEC), s_TimeSecStr);

        /* 按摩时间"mm:ss"：分 / 秒两个数据点拼成一串（组件只显示，倒计时由主控维护）。
           用一个资源而不是"分 + ':' + 秒"三层的写法：省两帧、也不会因分层位置误差对不齐。 */
        case TEXTRES_TIME_MMSS:
        {
            const uint8_t mm = S0553_Host_GetValue(S0553_DP_TIME_MIN);
            const uint8_t ss = S0553_Host_GetValue(S0553_DP_TIME_SEC);

            s_TimeMmssStr[0] = (char)('0' + ((mm / 10) % 10));
            s_TimeMmssStr[1] = (char)('0' + (mm % 10));
            s_TimeMmssStr[2] = ':';
            s_TimeMmssStr[3] = (char)('0' + ((ss / 10) % 10));
            s_TimeMmssStr[4] = (char)('0' + (ss % 10));
            s_TimeMmssStr[5] = '\0';
            return s_TimeMmssStr;
        }
        case TEXTRES_MAIN_PROGRAM_NAME:
        {
            /* programIndex = 程序数据点：0 = 手动模式，1..N = 自动程序序号。
               而名称表 s_MainProgramName 是 0-based（自动程序号 - 1），故需 -1。 */
            const uint8_t prog = programIndex;
            if (prog == 0)
            {
                /* 手动模式：显示当前选中的【手法名】（取自主控数据点 S0553_DP_WISE） */
                return S0553UI_WiseName(S0553_Host_GetValue(S0553_DP_WISE), lang);
            }
            const uint8_t p = (uint8_t)(prog - 1);
            /* 越界 → 文字也不显示：与图标策略一致（无对应素材的程序，图文双双留空）。
               注：名表保留了 25 条（其他机型备用），但本机型可用程序上限受图标素材数量
               限制（IMG_MAIN_AUTO_ICON_COUNT=20），故 21+ 视为越界。 */
            if (p >= IMG_MAIN_AUTO_ICON_COUNT) { return ""; }
            if (p >= S0553_PROGRAM_NAME_COUNT) { return ""; }
            const char *s = s_MainProgramName[p][lang];
            if (s == NULL) s = s_MainProgramName[p][S0553_LANG_CN]; // 该语言未提供翻译时回退简体中文
            return s;
        }
        case TEXTRES_WISE_NAME:
            return S0553UI_WiseName(S0553_Host_GetValue(S0553_DP_WISE), lang);
        case TEXTRES_TEST_LANG_NAME:
            return s_LangName[lang];   // 当前语言英文名

        /* 【通用】本页【当前选中子菜单项】的名字：
           名字表由【页面】自持（PageConfig.ItemNames），下标 = 本页 UI 游标，
           故旋转移动游标 → 中间标题随之变化；新增列表页不必改枚举与本函数。
           未配名字表 / 越界 → 返回空串（该段文本不发送）。 */
        case TEXTRES_CURSOR_ITEM_NAME:
        {
            const PageConfig *cfg = S0553UI_FindPageConfig(S0553Display_GetCurrentPage());
            if (cfg == NULL || cfg->ItemNames == NULL) { return ""; }

            const uint8_t cursor = S0553Display_GetUiCursor();
            if (cursor >= cfg->UiItemCount) { return ""; }

            /* 末项统一为"返回主页"：名字由框架提供，页面 ItemNames 只存真实项。 */
            if (cursor == cfg->UiItemCount - 1) { return s_BackHomeName[lang]; }

            const char *s = cfg->ItemNames[cursor][lang];
            if (s == NULL || s[0] == '\0')
            {
                s = cfg->ItemNames[cursor][S0553_LANG_CN];   /* 该语言无翻译 → 回退简体中文 */
            }
            return (s != NULL) ? s : "";
        }
        default:
            return "";
    }
}

/*
 *文件描述：功能开关状态定义
 *
 */

#define USE_Health_Check 1
#define Health_Check_Main (USE_Health_Check & 1)        // 通过主板连接心率模块
#define Health_Check_Key_OneLine (USE_Health_Check & 0) // 通过单线通信快捷按键板连接

// #define USART1_DMA

#define USE_CSD_MSUIC 0

//@ 老化配置
// #define J0201_LC
// #define J0201_LC_2
#define A39X_LC // * 新增老化 250423

//* 调试模式
#define Debug_Mode

//* 累加部件运行时间
// #define PART_USE_SUM_TIME
//* 下机芯复位腰部
#define MoveDw_Reset_Waist

/*****************************************************************************************************
 * 配置参数解释 250524
 * USE_BLUE_APP 1                 带irest云app
 * AutoLying_Control_BackLeg      自动躺倒允许手动控制推杆
 * USE_ONLINE_MODE                带irest在线程序
 * USE_M2                         双机芯
 *
 * LEG_BROAD                      有小腿线路板 删除 260512
 *
 *                                无定义 中文
 * VIETNAM_VOICE                  越南泰 陆培训
 * ENGLISH_VOICE                  英文中性
 *
 * A365B_ARMKEY = A391_ARMKEY     A391 扶手按键
 * LED2_PWM                       Led2 带呼吸灯
 * LEG_COM 1                      带电动小腿
 * HEAT_BACK 1                    带靠背加热
 * HEAT_LEG 1                     带小腿加热
 * ROLLER_CALVES_EN 1             带小腿揉搓
 * ROLLER_FOOT_EN 1               带足底滚轮
 *
 * AIR_WEAK                       0-6 气压调节 越大越弱
 * FLEX_WEAK                      0-6 下身顶出调节 越大越弱
 * DEL_FEETAIR                    删除末尾脚部气压
 * FEET_AIR_STRONG                增加脚部气压
 *
 *****************************************************************************************************/

//@ 以下定义机型 新增需要需改 1、配置与2、版本 3、蓝牙名称
// #define A391S_5
// #define A391S_6
// #define A391S_8 // * 样机
// #define A391S_31 //@ 新协议 下发 1.0 升级1.4
// #define A391S_32 //@ 匈牙利鲁迪/傅美琴 下发量产程序V1.0（260106） V1.1
// #define A391S_33 //@ 德国贝格罗斯/许小微 下发 1.0
// #define A391S_35 //@ 印度帕温兹/陈孝源 下发 1.0
// #define A391S_36 // @ 新中性程序 下发 1.1
// #define A391S_37 // @ 瑞典罗伯特/陈凯 1.0
// #define A391S_38 // @ 墨西哥 1.1
// #define A390S_9 // @ 乌兹别克斯坦阿齐兹/陈孝源 下发 1.0
// #define A390S_36 // @ 新中性程序 V1.1
// #define A390S_20 //todo J28 + J28 需要合并 260819
// #define A393S_20
// #define A390S_33 // * 10寸屏
// #define A390S_11 // @ 乌兹别克斯坦阿齐兹/陈孝源 下发 1.0
// #define A390S_10
// #define A393S_2
// #define A393S_5
// #define A395SY_2
// #define A395SY
// #define A395SY_6
// #define A393SY_2
// #define A396SY_2
// #define A396S_1 // * 样机
#define A396SYN_2
// #define A671S_21 // * 单独工程
// #define A390_X // * 测试单机芯 弹簧小腿配置 260513
// #define A390_8 // @ 越南泰
// #define A391_8 // @ 越南泰
// #define A390Y // * 样机
// #define A391Y // * 样机

#define USE_BLUE_OTA 1 // TODO 代替 app=8 260924
#define VOICE_CONTROL_EN 1

#ifdef A391S_5
#define AIR_CLAFBOT 1
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define LEG_COM 0
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 0
#define ROLLER_FOOT_EN 1
// #define LOCK_TO_ZERO2 // * rm 260320
#elif defined A391S_6
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_8
#define PART_USE_SUM_TIME // todo 运行时间 260923
#define USE_BLUE_APP 1
#define NECK_FLEX_ADD 1
#define USE_S0551
#define FEET_AIR_WEAK 18 // @ 要求脚部气压再减少 10-18 260810
#define CALF_AIR_WEAK 10
#define USE_M2
#define A390_ARMKEY
#define LED2_PWM
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_31
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_32
#define USE_ENCODER_4D
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_33
#define BLUE_KEY_8392 // @ 蓝牙有密码
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_35
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_36
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_37
#define FEET_AIR_WEAK 10 // todo P233 开口小腿 减足部气压
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391S_38
#define AIR_WEAK 5
#define MOVEUP_3D_WEAK
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_9
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_36
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_20
#define AIR_WEAK 5
#define FEET_AIR_WEAK 15
#define CALF_AIR_WEAK 15
#define MOVEUP_3D_WEAK
#define USE_S0551
#define USE_M2_4D
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A393S_20
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define MOVEUP_3D_WEAK
#define USE_M2_4D
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_33
#define USE_S0555
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_11
#define USE_S0555
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390S_10
#define USE_HEALTH_AUTO_NUM
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A393S_2
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A393S_5
#define USE_S0551
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 15
#define USE_M2
#define A390_ARMKEY
// #define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A395SY_2
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define USE_SOUNDWAVE
#define USE_M2
#define A395_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A395SY
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define USE_SOUNDWAVE
#define USE_S0551
#define USE_M2
#define A395_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE // todo 没有声控 260803
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#define VOICE_CONTROL_EN 0
#elif defined A395SY_6
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define USE_SOUNDWAVE
#define USE_S0551
#define USE_M2
#define A395_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A393SY_2
// #define USE_AIRSENSOR
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 18 // todo 腿部 10-15-13-18 260701
#define USE_SOUNDWAVE
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A396SY_2
// #define MOTOR_CALF_WEAK
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 18 // todo MOTOR_CALF_WEAK 13-10-18
#define USE_SOUNDWAVE
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A396S_1
#define USE_BLUE_APP 1
#define MOTOR_CALF_WEAK
#define FEET_AIR_WEAK 10
#define CALF_AIR_WEAK 10
#define USE_S0551
#define USE_M2
#define A390_ARMKEY
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A396SYN_2
#define FEET_AIR_WEAK 5
#define CALF_AIR_WEAK 18 // @ 15-18 261019
// #define USE_REDLIGHT // PA7 输出 TIMER2_CH_1 PWM
// #define USE_REDLIGHT_1KHZ // TIMER2 PWM 测试频率切换为 1kHz
// #define USE_REDLIGHT_5KHZ // TIMER2 红光 PWM 频率切换为 5kHz
#define USE_REDLIGHT_2KHZ
#define USE_FOOT_ROCK
#define USE_SOUNDWAVE
#define USE_M2
#define A390_ARMKEY
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A671S_21
#define USE_S0551
#define USE_M2
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
// #define HEAT_PJ 1 // @ 不合并了 260512
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A390_8
// #define CALF_AIR_WEAK 10
#define AUTO_NUM_24
#define A390_ARMKEY
#define VIETNAM_VOICE
#define AIR_SEAT 1 // todo 新增坐部气囊 260518
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391_8
#define AUTO_NUM_24
#define A365B_ARMKEY
#define VIETNAM_VOICE
#define LED2_PWM
#define AIR_SEAT 1
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#define BLUE_KEY_8888
#elif defined A390Y
#define FEET_STRAIR_ADD // TODO P0231 小腿拉伸使用 260707
// #define SEAT_AIR_WEAK 12
#define USE_SOUNDWAVE
#define AUTO_NUM_24
#define A390_ARMKEY
#define VIETNAM_VOICE
#define AIR_SEAT_GONGHE
#define AIR_SEAT 1 // @ 新增坐部气囊 260518
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#elif defined A391Y
// #define SEAT_AIR_WEAK 12
#define USE_SOUNDWAVE
#define AUTO_NUM_24
#define A365B_ARMKEY
#define VIETNAM_VOICE
#define LED2_PWM
#define AIR_SEAT_GONGHE // TODO 宫和座侧气囊 260722
#define AIR_SEAT 1
#define LEG_COM 1
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#define BLUE_KEY_8888
#elif defined A390_X
#define USE_S0551
#define AIR_WEAK 5
#define A365B_ARMKEY
#define LED2_PWM
#define ENGLISH_VOICE
#define LEG_COM 0
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#else
#define FLEX_WEAK 1
#define USE_M2
#define LEG_COM 1
#define A365B_ARMKEY
#define HEAT_BACK 1
#define HEAT_LEG 1
#define ROLLER_CALVES_EN 1
#define ROLLER_FOOT_EN 1
#endif

#ifndef USE_M2
#define M1M2LOSS_NO_PAUSE /* 上下机芯不连接不暂停 单机芯不暂停 */
#endif

// @ 全身气压 减弱 0-10-
#ifndef AIR_WEAK
#define AIR_WEAK 0
#endif
// @ 下身气压 减弱 10
#ifndef ANKLE_AIR_WEAK
#define ANKLE_AIR_WEAK 0
#endif
// @ 足部气压 减弱 10
#ifndef FEET_AIR_WEAK
#define FEET_AIR_WEAK 0
#endif
// @ 腿部气压 减弱 10
#ifndef CALF_AIR_WEAK
#define CALF_AIR_WEAK 0
#endif

// @ 腿部气压 减弱 10
#ifndef SEAT_AIR_WEAK
#define SEAT_AIR_WEAK 0
#endif

// @ 腿部气压 减弱 10
#ifndef NECK_FLEX_ADD
#define NECK_FLEX_ADD 0
#endif

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION 0
#endif

#define SHOULDER_AIR_RNZ_ANTI /* 肩部气压与揉捏互斥 */

/*-----------------------------------加热模块----------------------------------------------------*/
#define HEAT_SHOULDER 0 /* 肩膀加热 */
#define HEAT_ARM 0      /* 手臂加热 */
#if (HEAT_PJ == 0)
#define HEAT_PJ 0 /* 披肩加热 */
#endif

/*-----------------------------------滚轮模块----------------------------------------------------*/
#define ROLLER_ARM_EN 0 /* 手滚 */

/*------------------------------------手控器------------------------------------------------------*/
#define DCI_CTRL_EN 1
#define USE_S0307 (DCI_CTRL_EN & 0)

/*-----------------------------------外设通讯-------------------------------------------------------*/
#define DCI_ARMKEY_EN 1
#define DCI_APP_EN 0
#define DCI_VOICE_EN 1

// * 看门狗使能
#define USE_WDT_ENABLE

/*
 * 文件包含 *************************************************************************************************************************************
 */
#include "Heat.h"
#include "Leg.h"
#include "AT.h"

/*
 * 宏定义 *************************************************************************************************************************************
 */
#define HeatNum 2

/*
 * 变量定义 *************************************************************************************************************************************
 */
HeatInfo_S Heat[HeatNum];

uint8_t RedLight_Duty = 50;

static const uint16_t HeatOnOffTimeTab[][2] =
	{
#if (HEAT_BACK || HEAT_LEG || HEAT_SHOULDER || HEAT_ARM || HEAT_PJ)
		/*开，  关*/
		{60, 60}, /* 单位：秒 */
		{40, 80}, // * 小腿也降点温度 180、60 -> 150, 90 -> 120, 120
		{120, 120},
		{20, 100}, // * 开120S关120S 还是太高 修改 开30S 关90S 250505  -> 20, 100 250506
		{1800, 0},
#endif
};

bool BackHeatOn_Flag = 0; // * 电流检测用 250416
/*
 * 宏定义 *************************************************************************************************************************************
 */
#define HeatBackOnOffTimeTab HeatOnOffTimeTab
#define HeatLegOnOffTimeTab HeatOnOffTimeTab
#define HeatShoulderOnOffTimeTab HeatOnOffTimeTab
#define HeatArmOnOffTimeTab HeatOnOffTimeTab
#define HeatPJOnOffTimeTab HeatOnOffTimeTab
/*
 ************************************************************************************************************************************************
 * 函数定义 *************************************************************************************************************************************
 ************************************************************************************************************************************************
 */
/************************************************************************************************************************************************
 * 描述：背部加热开、关
 * 参数：无
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void BackHeatOn(void)
{
	Heat1_On();
	BackHeatOn_Flag = 1;
}

void BackHeatOff(void)
{
	Heat1_Off();
	BackHeatOn_Flag = 0;
}

/************************************************************************************************************************************************
 * 描述：腿部加热开、关
 * 参数：无
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void LegHeatOn(void)
{
#if (LEG_COM)
	LegHeatStateSet(1, 3);
#else
	Heat2_On(); // * 增加
#endif
}

void LegHeatOff(void)
{
#if (LEG_COM)
	LegHeatStateSet(0, 0);
#else
	Heat2_Off(); // * 增加
#endif
}

/************************************************************************************************************************************************
 * 描述：加热档位获取
 * 参数：whi：选择需要操作的部位
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
u8 HeatLevelGet(u8 whi)
{

	return Heat[whi].Level;
}

/************************************************************************************************************************************************
 * 描述：加热档位设置     预留接口
 * 参数：whi：选择需要操作的部位   level：要设置的档位
 * 返回：无
 * 注意：目前加热不分档位，通过 HeatStateSet一起设置，后续如需分档位，通过该函数单独控制，不用考虑state参数
 *************************************************************************************************************************************************/
void HeatLevelSet(u8 whi, u8 level)
{

	Heat[whi].Level = level;
}

/************************************************************************************************************************************************
 * 描述：加热状态获取
 * 参数：whi：选择需要操作的部位    lev：设置加热等级，在加热状态为3时被使用
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
u8 HeatStateGet(u8 whi)
{

	return Heat[whi].State;
}
/************************************************************************************************************************************************
 * 描述：加热状态设置
 * 参数：whi：选择需要操作的部位   sta：加热状态 0：长关，无需配合加热等级参数 1：长开，无需配合加热等级参数 2：根据加热表格操作，需配合加热等级参数
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void HeatStateSet(u8 whi, u8 sta, u8 level)
{

	Heat[whi].State = sta;
	Heat[whi].Level = level;
}

/************************************************************************************************************************************************
 * 描述：功能的开关处理
 * 参数：ExePeriod       执行周期    一般为100
 *       Func            功能模式    0：长关 1：长开 2：根据时间表格，需配合开关时间参数 *pOnOffTime
 *       Level           功能档位
 *       *pOnOffTime     开关时间    在功能模式为2时被调用
 *       OnOffTimeTab    开关时间表格
 * 返回：ON     功能开
 *       OFF    功能关
 *       其他值 不设置功能
 * 注意：无
 *************************************************************************************************************************************************/
uint8_t AT_Func_OnOffHandle(uint32_t ExePeriod, uint8_t Func, uint8_t Level, uint32_t *pOnOffTime, const uint16_t OnOffTimeTab[][2])
{
	uint16_t OnTime_l;
	uint16_t OffTime_l;
	uint8_t OnOff = FUNC_OFF;

	*pOnOffTime += ExePeriod;

	switch (Func)
	{
	case FUNC_OFF:
	{
		OnOff = FUNC_OFF;
		*pOnOffTime = 0; // * 关闭时计数清零 250305
	}
	break;

	case FUNC_ON:
	{
		OnOff = FUNC_ON;
	}
	break;

	case FUNC_BYTAB: /* 由表格内的值来控制 */
	{
		OnTime_l = OnOffTimeTab[Level - 1][0];
		OffTime_l = OnTime_l + OnOffTimeTab[Level - 1][1];
		if (*pOnOffTime / 1000 < OnTime_l)
		{
			OnOff = FUNC_ON;
		}
		else if (*pOnOffTime / 1000 < OffTime_l)
		{
			OnOff = FUNC_OFF;
		}
		else
		{
			*pOnOffTime = 0;
		}
	}
	break;

	default:
	{
		OnOff = FUNC_OFF;
	}
	break;
	}
	return OnOff;
}
/************************************************************************************************************************************************
 * 描述：功能执行  加热
 * 参数：无
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void HeatProcess(void)
{
	uint8_t Sw = 0;

#if (HEAT_BACK)
	Sw = AT_Func_OnOffHandle(100, Heat[BackHeat].State, Heat[BackHeat].Level, &Heat[BackHeat].TargetOnOffTime, HeatBackOnOffTimeTab);
	// if (FUNC_ON == Sw)
	// {
	// 	BackHeatOn();
	// }
	// else if (FUNC_OFF == Sw)
	// {
	// 	BackHeatOff();
	// }

	if (FUNC_ON == Sw && WorkStateGet() == WorkState_Work) // todo add 暂停不加热 260826
	{
		BackHeatOn();
	}
	else
	{
		BackHeatOff();
	}

	if (Heat[BackHeat].State == 1 && Heat[BackHeat].TargetOnOffTime > 180000) // * 连续长开3分钟 转间歇模式 250305
	{
		// HeatStateSet(BackHeat, 2, 3);
		HeatStateSet(BackHeat, 2, 4); // * 开3停1 -> 开2停2 降低温度 250407
	}
#endif

#if (HEAT_LEG)
	Sw = AT_Func_OnOffHandle(100, Heat[LegHeat].State, Heat[LegHeat].Level, &Heat[LegHeat].TargetOnOffTime, HeatLegOnOffTimeTab);
	// if (FUNC_ON == Sw)
	// {
	// 	LegHeatOn();
	// }
	// else if (FUNC_OFF == Sw)
	// {
	// 	LegHeatOff();
	// }
	if (FUNC_ON == Sw && WorkStateGet() == WorkState_Work) // todo add 暂停不加热 260826
	{
		LegHeatOn();
	}
	else
	{
		LegHeatOff();
	}

	if (Heat[LegHeat].State == 1 && Heat[LegHeat].TargetOnOffTime > 180000) // * 连续长开3分钟 转间歇模式 250305
	{
		HeatStateSet(LegHeat, 2, 2);
	}
#endif

#if (HEAT_SHOULDER)
	Sw = AT_Func_OnOffHandle(100, Heat[ShoulderHeat].State, Heat[ShoulderHeat].Level, &Heat[ShoulderHeat].TargetOnOffTime, HeatShoulderOnOffTimeTab);
	if (FUNC_ON == Sw)
	{
	}
	else if (FUNC_OFF == Sw)
	{
	}
#endif

#if (HEAT_ARM)
	Sw = AT_Func_OnOffHandle(100, Heat[ArmHeat].State, Heat[ArmHeat].Level, &Heat[ArmHeat].TargetOnOffTime, HeatArmOnOffTimeTab);
	if (FUNC_ON == Sw)
	{
	}
	else if (FUNC_OFF == Sw)
	{
	}
#endif

#if (HEAT_PJ)
	Sw = AT_Func_OnOffHandle(100, Heat[PJHeat].State, Heat[PJHeat].Level, &Heat[PJHeat].TargetOnOffTime, HeatPJOnOffTimeTab);
	if (FUNC_ON == Sw)
	{
	}
	else if (FUNC_OFF == Sw)
	{
	}
#endif
}

/************************************************************************************************************************************************
 * 描述：初始化
 * 参数：无
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void HeatInit(void)
{

	memset(Heat, 0, sizeof(HeatInfo_S) * HeatNum); /*清零所有数据*/

	/* 加热片可能需要先加热的一定温度再通过不断开关控制温度，此时将下段函数取消屏蔽，并将开关时间参数改为有符号整型 */
	//	#if(HEAT_BACK)
	//	  Heat[BackHeat].TargetOnOffTime = -180000;      /* 先加热3分钟再做开关控制 */
	//	#endif
	//
	//  #if(HEAT_LEG)
	//	  Heat[LegHeat].TargetOnOffTime = -180000;      /* 先加热3分钟再做开关控制 */
	//	#endif
}

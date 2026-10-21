#include "MassageEXEC.h"
#include "MassageLogic.h"
#include "Leg.h"

MotorT_Work_S MotorT_Work;

void Motor_Work_Iint(void)
{
	memset(&MotorT_Work, 0, sizeof(MotorT_Work_S));
}

void MotorT_100ms_Deal(void) //* 100ms 时基
{
	u8 i, temp, temp1;
	u16 temp16, temp16_1;
	for (i = MotorT_Back; i < MotorT_Max; i++)
	{
#ifdef USE_FOOT_ROCK
		if (MotorT_Work.RunFlag[i] == 0 && i != MotorT_Rock)
			continue;
#else
		if (MotorT_Work.RunFlag[i] == 0)
			continue;
#endif
		switch (i)
		{
		case MotorT_Rock:
#ifndef USE_FOOT_ROCK
			if (KeyPush.RockKeyState != 0)
				break;
			MotorT_Work.RunFlag[i] = 0;
#else
			LegRock_Updata();
#endif
			break;
		case MotorT_Sway:
			if (KeyPush.FlexKeyState != 0 || KeyPush.LegFlexKeyState != 0 || KeyPush.BackFlexKeyState != 0)
				break;
			if (MotorT_Work.Para2[i] == 0 || MotorT_Work.Para3[i] == 0)
			{
				MotorT_Work.RunFlag[i] = 0;
				MotorT_Work.RunTime[i] = 0;
				break;
			}
			temp16 = MotorT_Work.RunTime[i];
			temp16_1 = MotorT_Work.Para2[i] * 20;
			if (temp16 > (temp16_1 * MotorT_Work.Para3[i] - MotorT_Work.Para2[i] * 10)) // todo 修改 260703
			{
				MotorT_Work.RunFlag[i] = 0;
				MotorT_Work.RunTime[i] = 0;
				break;
			}

			if (temp16 % temp16_1 == 0)
			{
				DDG_Set_Zero(MotorT_Work.Cmd[i]);
			}
			else if (temp16 % temp16_1 == (temp16_1 >> 1))
			{
				DDG_Set_Zero(MotorT_Work.Para1[i]);
			}
			MotorT_Work.RunTime[i]++;
			break;

		case MotorT_Zero:
			if (KeyPush.FlexKeyState != 0 || KeyPush.LegFlexKeyState != 0 || KeyPush.BackFlexKeyState != 0)
				break;
			MotorT_Work.RunFlag[i] = 0;
			if (MotorT_Work.Cmd[i] == MotorPart_Zero)
			{
				temp = MotorT_Work.Para1[i];
				DDG_Set_Zero(temp);
			}
			else if (MotorT_Work.Cmd[i] == MotorPart_Stretch)
			{
				DDG_Set_SitCircle(MotorT_Work.Para1[i], MotorT_Work.Para2[i]);
			}
			break;
#if (LEG_COM)
		case MotorT_Flex:
			if (KeyPush.FootFlexKeyState > 0 || Leg_Touch_Flag > 0)
				break;
			if (MChair.Leg.FootFinding > 0)
				break;
			if (MotorT_Work.Cmd[i] == Motor_FOLW_TREAD)
			{
				if (MSG2Move[MoveUp].Tread.Method == TREAD_FWD_CIRCLE)
				{
					FootFlexStateSet(FlexDown);
					FootFlexSpeedSet(MotorT_Work.Para3[i]);
				}
				else if (MSG2Move[MoveUp].Tread.Method == TREAD_REV_CIRCLE)
				{
					FootFlexStateSet(FlexUp);
					FootFlexSpeedSet(MotorT_Work.Para3[i]);
				}
				else
				{
					FootFlexStateSet(FlexStop);
					FootFlexSpeedSet(0);
				}
				break;
			}
			FootFlexStateSet((FootFlexState_E)MotorT_Work.Cmd[i]);
			FootFlexSpeedSet(MotorT_Work.Para3[i]);
			if (GetLegDataRx()->Flex.Place == 2 && MotorT_Work.Para1[i] >= 200) //* 到位结束 250922
			{
				MotorT_Work.RunTime[i] = MotorT_Work.Para1[i];
			}
			if (++MotorT_Work.RunTime[i] > MotorT_Work.Para1[i])
			{
				MotorT_Work.RunFlag[i] = 0;
				MotorT_Work.RunTime[i] = 0;
				FootFlexStateSet(FlexStop);
				FootFlexSpeedSet(0);
			}
			break;
#endif

		case MotorT_Calf:
			if (KeyPush.KeyCalfKnead > 0) /* 未手动调节才通过表格数据控制 */
				break;
			if (MotorT_Work.Para1[i] == 0 && MotorT_Work.Para2[i] > 0) // * 延时启动
			{
				if (++MotorT_Work.RunTime[i] < MotorT_Work.Para2[i])
					break;
			}
			MotorT_Work.RunFlag[i] = 0;
			MotorT_Work.RunTime[i] = 0;
			if (MotorT_Work.Cmd[i] == MotorPart_STOP)
			{
				KneadModeSet(0);
				KneadStateSet(0);
				KneadSpeedSet(0);
			}
			else
			{
				KneadModeSet(1);
				KneadStateSet(1);
				KneadSpeedSet(1);
			}
			break;
		case MotorT_Feet:
			if (KeyPush.FootRollKeyState > 0) /* 未手动调节才通过表格数据控制 */
				break;
			MotorT_Work.RunFlag[i] = 0;
			if (MotorT_Work.Cmd[i] == MotorPart_STOP)
			{
				RollModeSet(0);
				RollStateSet(0);
				RollSpeedSet(0);
			}
			else
			{
				RollModeSet(1);
				RollStateSet(1);
				RollSpeedSet(1);
			}
			break;
		default:
			MotorT_Work.RunFlag[i] = 0;
			break;
		}
	}
}

//* 电机解析函数 Motor_Back  Motor_Leg  Motor_Flex  Motor_Calf  Motor_Feet  Motor_Zero
void MotorPart_Date_Set(u8 motorpart, u8 para0, u8 method, u8 para1, u8 para2, u8 para3)
{
	if (motorpart >= Part_Max || motorpart < Motor_Back)
		return;
#ifdef USE_FOOT_ROCK
	if (motorpart == Motor_Rock && KeyPush.RockKeyState != 0)
		return;
#endif
	MotorT_Work.RunFlag[motorpart - Motor_Back] = 1;
	MotorT_Work.Cmd[motorpart - Motor_Back] = method;
	MotorT_Work.Para1[motorpart - Motor_Back] = para1;
	MotorT_Work.Para2[motorpart - Motor_Back] = para2;
	MotorT_Work.Para3[motorpart - Motor_Back] = para3;
}
// * 100ms 时基  部件执行函数
void Part_EXEC_Process(void)
{
	HeatProcess(); // * 加热处理

	MotorT_100ms_Deal(); // * 电机处理
	FootRollProcess();	 // * 足底滚轮处理
	CalfKneadProcess();	 // * 小腿滚刮处理

	if (WorkStateGet() != WorkState_Work) // * modify 250914
		return;
	AirT_100ms_Deal(); // * 气囊处理
}

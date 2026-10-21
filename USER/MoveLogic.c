#include "MoveLogic.h"

u8 Prevent_Cnt = 0;
u8 Prevent_Cnt_A = 0;
u8 PreventFlag = 0; //* 碰撞标志位 0 正常 1 预警 2 停止 3 碰撞->弹开
bool Prevent_Err = 0;
u8 MoveSelect = 0; //* 机芯选择
u8 ShoulderValue = 0;
u8 BackPos = 0;
u8 FlexSpeed = 0;
u8 nBackIntensity; //* 3D强度 250308
#ifdef USE_M2_4D
u8 nBackIntensity_M2; // todo add 260108
#endif
u16 BodyArea[BodyPara];
u16 BodyAreaDw[BodyPara];

u16 MoveUp_Circle = 0;

MoveUpState_t MoveUpState;
// const u8 _3D_Level[] = {0, 6, 6, 12, 18, 24, 30, 32};

// #define BACK_UP_BASE 6
#define BACK_UP_BASE 3
const u8 BackIntensity_Base[] = {
	BACK_UP_BASE + NECK_FLEX_ADD, // 0
	BACK_UP_BASE + NECK_FLEX_ADD, // 1
	BACK_UP_BASE + NECK_FLEX_ADD, // 2
	BACK_UP_BASE,				  // 3
	BACK_UP_BASE,				  // 4
	BACK_UP_BASE,				  // 5
	BACK_UP_BASE,				  // 6
	3,							  // 7
	4,							  // 8
	6,							  // 9
	8,							  // 10 //todo 单机芯另外调整 251011
	4,							  // 11
	3,							  // 12
	0,							  // 13
	0,							  // 14
	0,							  // 14
	0,							  // 14
	0,							  // 14
	0,							  // 14
	0,							  // 14
};
#ifdef USE_M2_4D
const u8 BackIntensity_Base_M2[] = {
	BACK_UP_BASE, // 0
	BACK_UP_BASE, // 1
	BACK_UP_BASE, // 2
	BACK_UP_BASE, // 3
	BACK_UP_BASE, // 4
	BACK_UP_BASE, // 5
	BACK_UP_BASE, // 6
	BACK_UP_BASE, // 7
	BACK_UP_BASE, // 8
	BACK_UP_BASE, // 9
	BACK_UP_BASE, // 10
	BACK_UP_BASE, // 11
	BACK_UP_BASE, // 12
	0,			  // 13
	0,			  // 14
	0,			  // 14
	0,			  // 14
	0,			  // 14
	0,			  // 14
	0,			  // 14
};
#endif

u8 AutoPro_Flex_Adjust(u8 flexpara) //* 上机芯伸缩矫正 250926
{
	u8 temp, inten;
	u8 flex_weak = 0;
	u16 intentemp;

	inten = flexpara;
	if (inten > 48)
		inten = 48;
#ifdef FLEX_WEAK
	flex_weak = FLEX_WEAK;
#endif
	flex_weak += ASSIGN_Array[ASSIGN_Flex];
	if (inten > 10 && flex_weak > 0)
	{
		if (flex_weak > 10)
			flex_weak = 0;
		intentemp = (inten - 10) * (10 - flex_weak); //* 伸缩调整
		inten = intentemp / 10 + 10;
	}

	temp = BackIntensity_Base[BackPos];
	// temp /= 3;
	temp *= nBackIntensity;
	inten += temp;
	if (inten > 65)
		inten = 65;

	return inten;
}

#ifdef USE_M2_4D
u8 AutoPro_Flex_Adjust_M2(u8 flexpara) //* 上机芯伸缩矫正 250926
{
	u8 temp, inten;
	u8 flex_weak = 0;
	u16 intentemp;

	inten = flexpara;
	if (inten > 60) // TODO 修改限制 261011
		inten = 60;
#ifdef FLEX_WEAK
	flex_weak = FLEX_WEAK;
#endif
	flex_weak += ASSIGN_Array[ASSIGN_Flex];
	if (inten > 10 && flex_weak > 0)
	{
		if (flex_weak > 10)
			flex_weak = 0;
		intentemp = (inten - 10) * (10 - flex_weak); //* 伸缩调整
		inten = intentemp / 10 + 10;
	}

	temp = BackIntensity_Base_M2[MoveDw_Circle / 58 + 1]; // todo md 260109
	// temp /= 3;
	if (AutoModeGet() == 0)
		temp *= nBackIntensity_M2;
	else
		temp *= nBackIntensity; // todo md 260403
	inten += temp;
	if (inten > 65)
		inten = 65;

	return inten;
}
#endif

uint8_t BackPosUpGet(void)
{
	return BackPos;
}

void BackPosUpdate(void) // @ md 260122
{
	u8 i;
	u16 temp;
#ifdef USE_M2
	BackPos = 9;
	for (i = 0; i < 9; i++)
	{
		if (BodyArea[i + 1] < 10)
			temp = 0;
		else
			temp = BodyArea[i + 1] - 10;
		if (MoveUp_Circle <= temp)
		{
			BackPos = i + 1;
			break;
		}
	}
#else
	BackPos = 11;
	for (i = 0; i < 11; i++)
	{
		if (BodyArea[i + 1] < 10)
			temp = 0;
		else
			temp = BodyArea[i + 1] - 10;
		if (MoveUp_Circle <= temp)
		{
			BackPos = i + 1;
			break;
		}
	}
#endif
}

void BodyPlaceCal(u16 circle)
{
	u8 i;
	u16 CheckData;

	ShoulderValue = circle / SHOULDER_MIN;
	if (ShoulderValue > 11)
		ShoulderValue = 11;

	BodyArea[2] = circle;
#ifdef USE_M2
	// CheckData = MoveMaxCircle - MoveDw_UpSpace - 10 - circle;
	CheckData = MoveUp_Limit - circle;
	CheckData /= 7;
#else
	CheckData = MoveMaxCircle - MoveAREA9_11 - circle;
	CheckData /= 7;
#endif

	if (BodyArea[2] >= 55) // * 修改 50-60-55 251107
	{
		BodyArea[1] = BodyArea[2] - 30;
		BodyArea[0] = BodyArea[2] - 55;
	}
	else
	{
		BodyArea[0] = 0;
		if (BodyArea[2] >= 30)
			BodyArea[1] = BodyArea[2] - 30;
		else
			BodyArea[1] = 0;
	}

	if (BodyArea[0] >= 20)
		BodyArea[LB_C0] = BodyArea[0] - 20;
	else
		BodyArea[LB_C0] = 0;

	// if (BodyArea[0] >= 40)
	// 	BodyArea[LB_D0] = BodyArea[0] - 40;
	// else
	// 	BodyArea[LB_D0] = 0;

	// * 修改  250215
#ifdef USE_M2
	BodyArea[9] = MoveUp_Limit;							// * md 250923
	BodyArea[10] = (MoveMaxCircle + MoveUp_Limit) >> 1; //*不可能到达
	BodyArea[11] = MoveMaxCircle;						//*不可能到达
	for (i = 8; i > 2; i--)
	{
		BodyArea[i] = BodyArea[9] - CheckData * (9 - i);
	}
	// * 下机芯使用 转化前的数值
	BodyAreaDw[0] = 0;
	BodyAreaDw[1] = 90; // @ add 260313
	// BodyAreaDw[2] = 180;
	BodyAreaDw[2] = 180 + ((SHOULDER_MIN * 11 - BodyArea[2]) / 6); // todo md 腰部位置调整 260403
																   //* 设置下机芯最高位置 30为额外富裕间距  250928
																   //* BodyAreaDw[4] 相当与腰部 下机芯速度较快
#ifdef USE_M2_4D
	BodyAreaDw[9] = MoveDwMaxCircle - (MoveDw_UpSpace + 30 + BodyArea[0]);
#else
	BodyAreaDw[9] = MoveDwMaxCircle - ((MoveDw_UpSpace + 30 + BodyArea[0]) * 120 / 100);
#endif
	CheckData = BodyAreaDw[9] - BodyAreaDw[2];
	// CheckData = BodyAreaDw[9] / 9;
	// for (i = 8; i > 0; i--)
	// {
	// 	BodyAreaDw[i] = BodyAreaDw[9] - CheckData * (9 - i);
	// }
	CheckData /= 7;
	for (i = 8; i > 2; i--)
	{
		BodyAreaDw[i] = BodyAreaDw[9] - CheckData * (9 - i);
	}
	BodyAreaDw[LB_H0] = (BodyAreaDw[0] + BodyAreaDw[1]) >> 1;
	BodyAreaDw[LB_H1] = (BodyAreaDw[1] + BodyAreaDw[2]) >> 1;
	BodyAreaDw[LB_H2] = (BodyAreaDw[2] + BodyAreaDw[3]) >> 1;
	BodyAreaDw[LB_H3] = (BodyAreaDw[3] + BodyAreaDw[4]) >> 1;
	BodyAreaDw[LB_H5] = (BodyAreaDw[5] + BodyAreaDw[6]) >> 1;
	BodyAreaDw[LB_H6] = (BodyAreaDw[6] + BodyAreaDw[7]) >> 1;
	BodyAreaDw[LB_C0] = (BodyAreaDw[LB_H1] + BodyAreaDw[2]) >> 1;
#else
	BodyArea[9] = MoveMaxCircle - MoveAREA9_11 - (SHOULDER_MIN * 11 - BodyArea[2]) / 6;
	for (i = 8; i > 2; i--)
	{
		BodyArea[i] = BodyArea[9] - CheckData * (9 - i);
	}
	BodyArea[10] = MoveMaxCircle - (MoveAREA9_11 >> 1);
	BodyArea[11] = MoveMaxCircle;
#endif
	BodyArea[LB_H0] = (BodyArea[0] + BodyArea[1]) >> 1;
	BodyArea[LB_H1] = (BodyArea[1] + BodyArea[2]) >> 1;
	BodyArea[LB_H2] = (BodyArea[2] + BodyArea[3]) >> 1;
	BodyArea[LB_H6] = (BodyArea[6] + BodyArea[7]) >> 1;
	BodyArea[LB_H7] = (BodyArea[7] + BodyArea[8]) >> 1;
	BodyArea[LB_H8] = (BodyArea[8] + BodyArea[9]) >> 1;
	BodyArea[LB_H9] = (BodyArea[9] + BodyArea[10]) >> 1;
}

/*****************************************************************************************************
 * 描述：上下机芯间隔计算
 * 参数：无
 * 返回：0：距离安全 1：距离过近
 * 注意：无
 *****************************************************************************************************/
#ifdef USE_M2
uint16_t MoveDw_Circle;
uint16_t MoveSpace;
uint16_t Prevent_Time_Cnt = 0;		// * 碰撞复位计时补丁
uint16_t Prevent_MoveUp_Circle = 0; // * 碰撞记录上机芯
uint16_t Prevent_MoveDw_Circle = 0;
#endif
void Get_M1_M2_AntiFlag(void)
{
	u16 temp;
	MoveUp_Circle = MoveCurTreadCircleGet(MoveUp); //* 上机芯位置
#ifdef USE_M2
	temp = GetMoveDataTx(MoveDw)->Tread.CircleL | GetMoveDataTx(MoveDw)->Tread.CircleH << 8;
#ifdef USE_M2_4D
	MoveDw_Circle = MoveMaxCircle - temp; // todo 下机芯J28 260109
#else
	MoveDw_Circle = MoveMaxCircle - ((temp * MoveMaxCircle) / MoveDwMaxCircle); // * 下机芯位置 按比例变化后
#endif
	MoveSpace = MoveDw_Circle - MoveUp_Circle;
	if (MassAreaGet(MoveUp) == Area_Part) // * 局部增加条件 251216
	{
		MoveSpace -= MoveArea_Space;
	}
#endif

#ifdef USE_M2
	if (KeyPush.TreadKeyState == TreadState_Up && MoveUp_Circle == 0)
#else
	if ((KeyPush.TreadKeyState == TreadState_Up && MoveUp_Circle == 0) ||
		(KeyPush.TreadKeyState == TreadState_Dw && MoveUp_Circle > (MoveMaxCircle - 5)))
#endif
	{
		Buzzer_Ararm = 0x02;
	}
	else
		Buzzer_Ararm &= 0xfd;

#ifdef USE_M2
	// * 增加全身时，下机芯不能往上 250512
	if ((KeyPush.M2TreadKeyState == TreadState_Dw && MoveDw_Circle == MoveMaxCircle) ||
		(KeyPush.M2TreadKeyState == TreadState_Up &&
		 (MassAreaGet(MoveUp) == Area_Full || MassAreaGet(MoveUp) == Area_Lower || (MassAreaGet(MoveUp) == Area_Upper && MoveDw_Circle <= 300))))
	{
		Buzzer_Ararm = 0x04;
	}
	else
		Buzzer_Ararm &= 0xfb;
	if ((KeyPush.TreadKeyState == TreadState_Dw || KeyPush.M2TreadKeyState == TreadState_Up) && PreventFlag > 0)
	{
		Buzzer_Ararm = 0x08;
	}
	else
		Buzzer_Ararm &= 0xf7;

	if (((UpTreadPlace & MassPlace_Hit) == MassPlace_Hit) || (DwTreadPlace & MassPlace_Hit) == MassPlace_Hit)
	{
		PreventFlag = 3; //! 碰撞之后会维持5秒钟 250923
		if (Prevent_MoveUp_Circle == 0)
		{
			Prevent_MoveUp_Circle = MoveUp_Circle; // * add 251121
			Prevent_MoveDw_Circle = MoveDw_Circle; // * add 251122
		}
	}
	// * 碰撞等级>1 只能增加
	if (MoveSpace < (MoveDw_UpSpace - 15))
		PreventFlag = 3;
	else if (MoveSpace < MoveDw_UpSpace && PreventFlag < 3)
		PreventFlag = 2;
	else if (((MoveSpace < (MoveDw_UpSpace + 15)) || ((MoveSpace < (MoveDw_UpSpace + 30)) && MassAreaGet(MoveUp) == Area_Part)) && PreventFlag < 2) // * 手动上下时候用
		PreventFlag = 1;
	else if (PreventFlag < 2)
		PreventFlag = 0;

	if (PreventFlag == 2)
	{
		if ((MoveDw_Circle > (MoveMaxCircle - 10)) && MoveSpace > (MoveDw_UpSpace + 70)) // * 增加 250212
		{
			PreventFlag = 0;
		}
	}
	else if (PreventFlag == 3)
	{
		if (Prevent_Cnt < 6)
		{
			if (MoveDw_Circle == MoveMaxCircle)
			{
				if (MoveUp_Circle == 0 && MoveDw_Circle == MoveMaxCircle)
				{
					Prevent_Cnt = 0;
					Prevent_MoveUp_Circle = 0;
					Prevent_MoveDw_Circle = 0;
					PreventFlag = 0;
					Prevent_Err = 0;
				}
				else if (MoveUp_Circle < (Prevent_MoveUp_Circle - 80) &&
						 (MoveDw_Circle > (Prevent_MoveDw_Circle + 80) || MoveDw_Circle == MoveMaxCircle))
				{
					Prevent_Cnt++;
					Prevent_MoveUp_Circle = 0;
					Prevent_MoveDw_Circle = 0;
					PreventFlag = 0;
				}
			}
		}
		else
		{
			Prevent_Err = 1;
			if (MoveUp_Circle == 0 && MoveDw_Circle == MoveMaxCircle) // * 修改 碰撞恢复条件 250408
			{
				PreventFlag = 0;
				Prevent_Cnt = 0;
				Prevent_Cnt_A++;
				Prevent_Err = 0;
			}
		}

		// if ((MoveSpace > (MoveDw_UpSpace + 50)) && MassAreaGet(MoveUp) != Area_Fixed) // * md 250923 rm 251121
		// {
		// 	PreventFlag = 0; // * rm 251121
		// }
	}

	if (PreventFlag < 2)
	{
		Prevent_Time_Cnt = 0;
		// if (MoveSpace > (MoveDw_UpSpace + 100)) // * rm 251121 * 清除碰撞次数累加 250419
		// 	Prevent_Cnt = 0;
	}
	else if (++Prevent_Time_Cnt > 600) // * 0.1*400 = 20秒 碰撞20S之后
	{
		if (MoveSpace > (MoveDw_UpSpace + 15)) //* 40s后判断条件 防止意外
		{
			Prevent_Time_Cnt = 0;
			PreventFlag = 0;
			Prevent_Cnt = 0;
			// Prevent_Err = 0; //* rm 251121
		}
		else
		{
			Prevent_Time_Cnt = 600;
		}
	}
#endif
}

bool MoveArea_Top_Flag = 0;
bool MoveArea_Bot_Flag = 0;		// * 手动局部到上下限位标志 250923
void MoveArea_TopBot_Deal(void) // * 处理局部到上下限位
{
	if (MassAreaGet(MoveUp) != Area_Part)
		return;

	if (KeyPush.TreadKeyState != TreadState_Stop)
	{
		if (MoveUp_Circle < (MoveArea_Space * 2))
		{
			MoveArea_Top_Flag = 1;
		}
#ifndef USE_M2
		else if (MoveUp_Circle < (MoveMaxCircle - MoveArea_Space * 2))
		{
			MoveArea_Bot_Flag = 1;
		}
#endif
	}
	else if (MoveArea_Top_Flag)
	{
		Part_Date_Set(MoveUp_Tread, 1, TREAD_FIX_CIRCLE, MoveArea_Space * 2, 0, 10);
		if (MoveUp_Circle > (MoveArea_Space * 2 - 10))
		{
			MoveArea_Top_Flag = 0;
			Move_Flag[MoveUp].Area = 1;
		}
	}
#ifndef USE_M2
	else if (MoveArea_Bot_Flag)
	{
		Part_Date_Set(MoveUp_Tread, 1, TREAD_FIX_CIRCLE, (MoveMaxCircle - MoveArea_Space * 2) & 0xff, (MoveMaxCircle - MoveArea_Space * 2) >> 8, 10);
		if (MoveUp_Circle < (MoveMaxCircle + 10 - MoveArea_Space * 2))
		{
			MoveArea_Bot_Flag = 0;
			Move_Flag[MoveUp].Area = 1;
		}
	}
#endif
}

// #ifdef USE_M2
// bool MoveArea_Prevent_Flag = 0;
// u16 MoveUp_Circle_Old = 0;
// u8 MoveArea_Avoid_Delay = 0;
// void MoveArea_Avoid_Deal(void) // * 处理局部到碰撞
// {
// 	if (MassAreaGet(MoveUp) != Area_Part && MassAreaGet(MoveUp) != Area_Full)
// 		return;
// 	if (MoveArea_Top_Flag || MoveArea_Bot_Flag)
// 		return;
// 	if (++MoveArea_Avoid_Delay < 10)
// 		return;
// 	MoveArea_Avoid_Delay = 0;
// 	if (MoveUp_Circle_Old != MoveUp_Circle)
// 	{
// 		MoveUp_Circle_Old = MoveUp_Circle;
// 	}
// 	else
// 	{
// 		Part_Date_Set(MoveUp_Tread, 1, TREAD_FWD_CIRCLE, 100, 0, 10); // * md 251121
// 		if (PreventFlag == 0 && MoveUp_Circle < (MoveUp_Avoid_Position + 20))
// 		{
// 			Move_Flag[MoveUp].Area = 1;
// 		}
// 	}
// }
// #endif

bool MoveArea_Prevent_Flag = 0;
u16 MoveUp_Circle_Old = 0;
u8 MoveArea_Avoid_Delay = 0;
void MoveArea_Avoid_Deal(void) // * 处理局部到碰撞
{
	if (MassAreaGet(MoveUp) != Area_Part && MassAreaGet(MoveUp) != Area_Full)
		return;
	if (MoveArea_Top_Flag || MoveArea_Bot_Flag)
		return;
	if (++MoveArea_Avoid_Delay < 10)
		return;
	MoveArea_Avoid_Delay = 0;
	if (MoveUp_Circle_Old != MoveUp_Circle)
	{
		MoveUp_Circle_Old = MoveUp_Circle;
	}
	else
	{
		Part_Date_Set(MoveUp_Tread, 1, TREAD_FWD_CIRCLE, 100, 0, 10); // * md 251121
#ifdef USE_M2
		if (PreventFlag == 0 && MoveUp_Circle < (MoveUp_Avoid_Position + 20))
#else
		if (MoveUp_Circle < (MoveMaxCircle + 10 - MoveArea_Space * 2))
#endif
		{
			Move_Flag[MoveUp].Area = 1;
		}
	}
}

/*****************************************************************************************************
 * 描述：机芯规避  防撞动作
 * 参数：无
 * 返回：无
 * 注意：简易逻辑，先做调试用，后续需要完善优化
 *****************************************************************************************************/
void M1_M2_void(void)
{
	// uint16_t CurCircle = 0, CurCircle1 = 0;
	Get_M1_M2_AntiFlag();
	MoveArea_TopBot_Deal(); // * 处理局部动作到上下限位 处理 250923
	MoveArea_Avoid_Deal();	// todo 处理局部、全身动作到碰撞处理 250923 处理单机芯局部恢复 260713
#ifdef USE_M2
	if (PreventFlag > 1) // * 修改碰撞 处理
	{
		if (PreventFlag > 3) // * 防止意外 250416
			PreventFlag = 3;
		if (Prevent_Err && PreventFlag == 3) // * 上下机芯复位
		{
			Part_Date_Set(MoveUp_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 10);
			Part_Date_Set(MoveDw_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 10);
			return;
		}

		// if ((MoveDw_Circle > (MoveMaxCircle - 50)) && PreventFlag == 3) // * md 251121
		if (PreventFlag == 3) // * md 251121
		{
			Part_Date_Set(MoveUp_Tread, 1, TREAD_FWD_CIRCLE, 100, 0, 10);
			Part_Date_Set(MoveDw_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 10);
		}
		else
		{
			Part_Date_Set(MoveDw_Tread, 1, TREAD_FIX_CIRCLE, 0, 0, 10);
		}
	}
	else if (PreventFlag == 1 && MoveFolw_Cmd.Tread_cmd == FOLW_SPACE && MoveDw_Circle < MoveMaxCircle) // * 增加跟随碰撞预处理 251023
	{
		Part_Date_Set(MoveDw_Tread, 1, TREAD_FWD, 0, 0, 10);
	}
#endif
}

/*
 * 设置机芯选择，0：上机芯 1：下机芯
 */
void MoveWhichSet(u8 whi)
{
	MoveSelect = whi;
}

/*
 * 获取机芯选择，0：上机芯 1：下机芯
 */
u8 MoveWhichGet(void)
{
	return MoveSelect;
}

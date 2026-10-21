#include "MoveBoth.h"
#include "KeyFunc.h"
#include "Voice.h"
#include "Table.h"
#include "MassageLogic.h"

MSG2Move_S MSG2Move[2];
MSG4Move_R MSG4Move[2];
Move_Flag_T Move_Flag[2];
Massage_Flag_T Massage_Flag;
Move_Follow_Cmd_T MoveFolw_Cmd;
#ifdef USE_M2
u16 Move_Follow_Space = MoveDwMaxCircle;
#endif
void MSGMoveInit(void)
{
	memset(&MSG2Move[MoveUp], 0, sizeof(MSG2Move_S));
	memset(&MSG4Move[MoveUp], 0, sizeof(MSG4Move_R));
	memset(&Move_Flag[MoveUp], 0, sizeof(Move_Flag_T));
	Massage_Flag.BodyCheck = 0;
	// Massage_Flag.DwFinish = 0;
	Massage_Flag.HealthPrepare = 0;
#ifdef USE_M2
	memset(&MSG2Move[MoveDw], 0, sizeof(MSG2Move_S));
	memset(&MSG4Move[MoveDw], 0, sizeof(MSG4Move_R));
	memset(&Move_Flag[MoveDw], 0, sizeof(Move_Flag_T));
	memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T));
#endif
}

MSG4Move_R *GetMoveDataTx(u8 whi)
{
	return &MSG4Move[whi];
}

u16 MoveCurTreadCircleGet(u8 whi)
{
	u16 temp;
	temp = (u16)MSG4Move[whi].Tread.CircleL | (u16)MSG4Move[whi].Tread.CircleH << 8;
	return temp;
}

void MoveUp_Updata_Clear(void)
{
	MSG2Move[MoveUp].Tread.Update = 0;
	MSG2Move[MoveUp].Knead.Update = 0;
	MSG2Move[MoveUp].Tap.Update = 0;
	MSG2Move[MoveUp].Flex.Update = 0;
}

void MoveUp_Updata_Set(u8 code)
{
	if (code & 0x01)
		MSG2Move[MoveUp].Tread.Update = 1;
	if (code & 0x02)
		MSG2Move[MoveUp].Knead.Update = 1;
	if (code & 0x04)
		MSG2Move[MoveUp].Tap.Update = 1;
	if (code & 0x08)
		MSG2Move[MoveUp].Flex.Update = 1;
}
#ifdef USE_M2
void MoveDw_Updata_Clear(void)
{
	MSG2Move[MoveDw].Tread.Update = 0;
	MSG2Move[MoveDw].Knead.Update = 0;
	MSG2Move[MoveDw].Tap.Update = 0;
#ifdef USE_M2_4D
	MSG2Move[MoveDw].Flex.Update = 0;
#endif
}

void MoveDw_Updata_Set(u8 code)
{
	if (code & 0x01)
		MSG2Move[MoveDw].Tread.Update = 1;
	if (code & 0x02)
		MSG2Move[MoveDw].Knead.Update = 1;
	if (code & 0x04)
		MSG2Move[MoveDw].Tap.Update = 1;
#ifdef USE_M2_4D
	if (code & 0x08)
		MSG2Move[MoveDw].Flex.Update = 1;
#endif
}
#endif
//* 机芯指令解析
void Part_Date_Set(u8 part, u8 update, u8 method, u8 para1, u8 para2, u8 speed)
{
	u16 temp, tempdw;
	bool follow_speed_sub = 0;
	bool follow_speed_add = 0;
	switch (part)
	{
	case MoveUp_Tread:
		if (update > 0)
			MSG2Move[MoveUp].Tread.Update = 1;
		else if (PreventFlag < 3) // * md 251121
			Move_Flag[MoveUp].Tread_p = 1;
		MSG2Move[MoveUp].Tread.Method = method;
		MSG2Move[MoveUp].Tread.Para1 = para1;
		MSG2Move[MoveUp].Tread.Para2 = para2;
		if (speed > 0x10) //* 复合速度手法 260119
			MSG2Move[MoveUp].Tread.Speed = speed;
		else if ((ASSIGN_Array[ASSIGN_TreadSpeed] + speed) > 10) // @ add 260119
			MSG2Move[MoveUp].Tread.Speed = 10;
		else
			MSG2Move[MoveUp].Tread.Speed = ASSIGN_Array[ASSIGN_TreadSpeed] + speed;
#ifdef USE_M2
		if (update > 0 || PreventFlag >= 3) //* 自动模式才跟随
			break;
		if (MoveFolw_Cmd.Tread_cmd == FOLW_SPACE)
		{
			Move_Flag[MoveDw].Tread_p = 1;
			if (method == TREAD_FIX_CIRCLE)
			{
				MSG2Move[MoveDw].Tread.Method = method;
				temp = para2;
				temp = temp << 8 | para1;
				if (MoveCurTreadCircleGet(MoveUp) > temp) // * 速度差异修正 251007 251010
					follow_speed_sub = 1;
#ifdef USE_M2_4D
				temp += Move_Follow_Space; // todo md 260130
#else
				temp = (Move_Follow_Space + temp) * 120 / 100;
#endif
				if (temp > MoveDwMaxCircle)
					tempdw = 0;
				else
					tempdw = MoveDwMaxCircle - temp;
				MSG2Move[MoveDw].Tread.Para1 = tempdw & 0xff;
				MSG2Move[MoveDw].Tread.Para2 = tempdw >> 8;
			}
			else if (method == TREAD_REV_CIRCLE)
			{
				MSG2Move[MoveDw].Tread.Method = TREAD_FWD_CIRCLE;
				MSG2Move[MoveDw].Tread.Para1 = para1;
				MSG2Move[MoveDw].Tread.Para2 = para2;
			}
			else if (method == TREAD_FWD_CIRCLE)
			{
				MSG2Move[MoveDw].Tread.Method = TREAD_REV_CIRCLE;
				MSG2Move[MoveDw].Tread.Para1 = para1;
				MSG2Move[MoveDw].Tread.Para2 = para2;
			}
			else
			{
				// @ wait for add 250930
			}
			if (follow_speed_sub)
			{
				if (PreventFlag) // * 测试 251007
					MSG2Move[MoveDw].Tread.Speed = 0;
				else
					MSG2Move[MoveDw].Tread.Speed = speed >> 1;
			}
			else
				MSG2Move[MoveDw].Tread.Speed = speed; // * 速度需要修改 250930
		}
#endif
		break;

	case MoveUp_Knead:
		if (update > 0)
			MSG2Move[MoveUp].Knead.Update = 1;
		else
			Move_Flag[MoveUp].Knead_p = 1;
		MSG2Move[MoveUp].Knead.Method = method;
		MSG2Move[MoveUp].Knead.Para1 = para1;
		MSG2Move[MoveUp].Knead.Para2 = para2;
		if (speed > 0x10) //* 复合速度手法 260119
			MSG2Move[MoveUp].Knead.Speed = speed;
		else if ((ASSIGN_Array[ASSIGN_KneadSpeed] + speed) > 10) // @ add 260119
			MSG2Move[MoveUp].Knead.Speed = 10;
		else
			MSG2Move[MoveUp].Knead.Speed = ASSIGN_Array[ASSIGN_KneadSpeed] + speed;
#ifdef USE_M2
		if (update > 0) //* 自动模式才跟随
			break;
		if (MoveFolw_Cmd.Knead_cmd == FOLW_COPY)
		{
			Move_Flag[MoveDw].Knead_p = 1;
			MSG2Move[MoveDw].Knead.Method = method;
			if (method == KNEAD_FWD_ABS || method == KNEAD_REV_ABS || method == KNEAD_FIX_CIRCLE)
			{
				temp = para1 * MOVEDWWIDTH5;
				temp = temp / MOVEUPWIDTH5;
				MSG2Move[MoveDw].Knead.Para1 = temp;
			}
			else
				MSG2Move[MoveDw].Knead.Para1 = para1;
			MSG2Move[MoveDw].Knead.Para2 = para2;
			MSG2Move[MoveDw].Knead.Speed = speed;
		}
		else if (MoveFolw_Cmd.Knead_cmd == FOLW_WIDTH) //* add 251023
		{
			Move_Flag[MoveDw].Knead_p = 1;
			MSG2Move[MoveDw].Knead.Method = KNEAD_FIX_CIRCLE;
			MSG2Move[MoveDw].Knead.Para1 = para1;
			MSG2Move[MoveDw].Knead.Para2 = para2;
			MSG2Move[MoveDw].Knead.Speed = speed;
		}
		else if (MoveFolw_Cmd.Knead_cmd == FOLW_FIX2KNEAD) // * ADD 251117
		{
			Move_Flag[MoveDw].Knead_p = 1;

			Move_Flag[MoveDw].Knead_p = 1;
			MSG2Move[MoveDw].Knead.Method = method;
			MSG2Move[MoveDw].Knead.Para1 = para1;
			MSG2Move[MoveDw].Knead.Para2 = para2;
			MSG2Move[MoveDw].Knead.Speed = speed;
			if (method == KNEAD_FWD_ABS || method == KNEAD_REV_ABS || method == KNEAD_FIX_CIRCLE)
			{
				MSG2Move[MoveDw].Knead.Method = KNEAD_FWD;
				MSG2Move[MoveDw].Knead.Para1 = 0;
				MSG2Move[MoveDw].Knead.Para2 = 0;
				MSG2Move[MoveDw].Knead.Speed = 4;
			}
		}
#endif
		break;

	case MoveUp_Tap:
		if (update > 0)
			MSG2Move[MoveUp].Tap.Update = 1;
		else
			Move_Flag[MoveUp].Tap_p = 1;
		MSG2Move[MoveUp].Tap.Method = method;
		MSG2Move[MoveUp].Tap.Para1 = para1;
		MSG2Move[MoveUp].Tap.Para2 = para2;
		if (speed > 0x10) //* 复合速度手法 260119
			MSG2Move[MoveUp].Tap.Speed = speed;
		else if ((ASSIGN_Array[ASSIGN_TapSpeed] + speed) > 10) // @ add 260119
			MSG2Move[MoveUp].Tap.Speed = 10;
		else
			MSG2Move[MoveUp].Tap.Speed = ASSIGN_Array[ASSIGN_TapSpeed] + speed;
#ifdef USE_M2
		if (update > 0) //* 自动模式才跟随
			break;
		if (MoveFolw_Cmd.Tap_cmd == FOLW_COPY) // @ md 260309
		{
			Move_Flag[MoveDw].Tap_p = 1;
			MSG2Move[MoveDw].Tap.Method = method;
			MSG2Move[MoveDw].Tap.Para1 = para1;
			MSG2Move[MoveDw].Tap.Para2 = para2;
#ifndef USE_M2_4D
			if (speed <= 8)
				MSG2Move[MoveDw].Tap.Speed = speed + 2; //* 增加下机芯拍打速度
			else
#endif
				MSG2Move[MoveDw].Tap.Speed = speed;
		}
#endif
		break;

	case MoveUp_Flex:
		if (update > 0)
			MSG2Move[MoveUp].Flex.Update = 1;
		else
			Move_Flag[MoveUp].Flex_p = 1;
		MSG2Move[MoveUp].Flex.Method = method;
		if (update > 0)
			MSG2Move[MoveUp].Flex.Para1 = para1;
		else
			MSG2Move[MoveUp].Flex.Para1 = AutoPro_Flex_Adjust(para1); //* 校正 251010

		MSG2Move[MoveUp].Flex.Para2 = para2;
		MSG2Move[MoveUp].Flex.Speed = speed;
#ifdef USE_M2_4D
		if (update > 0) //* 自动模式才跟随
			break;
		if (MoveFolw_Cmd.Flex_cmd == FOLW_COPY) // todo add 260117
		{
			Move_Flag[MoveDw].Flex_p = 1;
			MSG2Move[MoveDw].Flex.Method = method;
			MSG2Move[MoveDw].Flex.Para1 = AutoPro_Flex_Adjust_M2(para1);
			MSG2Move[MoveDw].Flex.Para2 = para2;
			MSG2Move[MoveDw].Flex.Speed = speed;
		}
#endif
		break;
	//* 注意下机芯行走长度 要设定原始的
	case MoveDw_Tread:
		if (update > 0)
			MSG2Move[MoveDw].Tread.Update = 1;
		else if (PreventFlag < 3) // * md 251121
			Move_Flag[MoveDw].Tread_p = 1;
		MSG2Move[MoveDw].Tread.Method = method;
		MSG2Move[MoveDw].Tread.Para1 = para1;
		MSG2Move[MoveDw].Tread.Para2 = para2;
		MSG2Move[MoveDw].Tread.Speed = speed;
		break;

	case MoveDw_Knead:
		if (update > 0)
			MSG2Move[MoveDw].Knead.Update = 1;
		else
			Move_Flag[MoveDw].Knead_p = 1;
		MSG2Move[MoveDw].Knead.Method = method;
		MSG2Move[MoveDw].Knead.Para1 = para1;
		MSG2Move[MoveDw].Knead.Para2 = para2;
		MSG2Move[MoveDw].Knead.Speed = speed;
		break;

	case MoveDw_Tap:
		if (update > 0)
			MSG2Move[MoveDw].Tap.Update = 1;
		else
			Move_Flag[MoveDw].Tap_p = 1;
		MSG2Move[MoveDw].Tap.Method = method;
		MSG2Move[MoveDw].Tap.Para1 = para1;
		MSG2Move[MoveDw].Tap.Para2 = para2;
		MSG2Move[MoveDw].Tap.Speed = speed;
		break;

#ifdef USE_M2_4D
	case MoveDw_Flex:
		if (update > 0)
			MSG2Move[MoveDw].Flex.Update = 1;
		else
			Move_Flag[MoveDw].Flex_p = 1;
		MSG2Move[MoveDw].Flex.Method = method;
		if (update > 0)
			MSG2Move[MoveDw].Flex.Para1 = para1;
		else
			MSG2Move[MoveDw].Flex.Para1 = AutoPro_Flex_Adjust_M2(para1); // todo 待确定 260107

		MSG2Move[MoveDw].Flex.Para2 = para2;
		MSG2Move[MoveDw].Flex.Speed = speed;
		break;
#endif
	default:
		break;
	}
}

void MoveUp_Manual_Pause(void)
{
	Part_Date_Set(MoveUp_Knead, 1, KNEAD_STOP, 0, 0, 0);
	Part_Date_Set(MoveUp_Flex, 1, FLEX_STOP, 0, 0, 0);
	Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
	Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);
}
#ifdef USE_M2
void MoveDw_Manual_Pause(void)
{
	Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
	Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
	Part_Date_Set(MoveDw_Tread, 1, TREAD_STOP, 0, 0, 0);
}
#endif
void Move_Manual_Update_Speed(u8 whi)
{
	u8 speed = MassSpeedGet(whi) << 1;

	MSG2Move[whi].Tread.Speed = speed;
#ifdef USE_S0555
	if (MassWiseGet(whi) == JRZ && whi == MoveUp) // @ add 260115
	{
		MSG2Move[whi].Knead.Speed = (speed << 4) | MassSpeedGet(whi);
	}
	else
		MSG2Move[whi].Knead.Speed = speed;
#else
	MSG2Move[whi].Knead.Speed = speed;
#endif

	if (MassWiseGet(whi) == PDB)
		MSG2Move[whi].Tap.Speed = (speed << 4) | MassSpeedGet(whi);
	else
		MSG2Move[whi].Tap.Speed = speed;
	MSG2Move[whi].Flex.Speed = speed;
}

void Move_Manual_Update_Width(u8 whi)
{
	u8 temp;
	switch (MassWidthGet(whi))
	{
	case 0:
		if (whi == MoveUp)
			temp = MOVEUPWIDTH1;
		else
			temp = MOVEDWWIDTH1;
		break;
	case 1:
		if (whi == MoveUp)
			temp = MOVEUPWIDTH2;
		else
			temp = MOVEDWWIDTH2;
		break;
	case 2:
		if (whi == MoveUp)
			temp = MOVEUPWIDTH3;
		else
			temp = MOVEDWWIDTH3;
		break;
	case 3:
		if (whi == MoveUp)
			temp = MOVEUPWIDTH4;
		else
			temp = MOVEDWWIDTH4;
		break;
	case 4:
		if (whi == MoveUp)
			temp = MOVEUPWIDTH5;
		else
			temp = MOVEDWWIDTH5;
		break;

	default:
		break;
	}
	if (whi == MoveUp)
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FIX_CIRCLE, temp, 0, 3);
	else
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_FIX_CIRCLE, temp, 0, 3);
}

u8 MoveUp_Manual_Inten_Get(void)
{
	u8 temp;
	switch (nBackIntensity)
	{
	default:
	case 0:
#ifdef USE_S0555
		if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == ZYD || MassWiseGet(MoveUp) == ZY ||
			MassWiseGet(MoveUp) == D3RNZ || MassWiseGet(MoveUp) == D3PD || MassWiseGet(MoveUp) == D3RPZ)
			temp = 6;
		else
			temp = 0;
#else
		if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == ZYD ||
			MassWiseGet(MoveUp) == D3RNZ || MassWiseGet(MoveUp) == D3PD || MassWiseGet(MoveUp) == D3RPZ)
			temp = 5;
		else
			temp = 0;
#endif
		break;
	case 1:
		temp = 10;
		break;
	case 2:
		temp = 20;
		break;
	case 3:
		temp = 30;
		break;
	case 4:
		temp = 40; //* 最大 65
		break;
	}
	return temp;
}

void MoveUp_Manual_Update_Inten(void)
{
	u8 temp = MoveUp_Manual_Inten_Get();
	u8 speed;
	if (MassWiseGet(MoveUp) == ZYS || MassWiseGet(MoveUp) == D3RNZ || MassWiseGet(MoveUp) == D3PD || MassWiseGet(MoveUp) == D3RPZ)
	{
		speed = MassSpeedGet(MoveUp) << 1;
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH, temp, 0, speed);
	}
	else if (MassWiseGet(MoveUp) == ZYD)
	{
		speed = MassSpeedGet(MoveUp) << 1;
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH2, temp >> 1, temp, speed);
	}
#ifdef USE_S0555
	else if (MassWiseGet(MoveUp) == ZY) // @ ADD 251203
	{
		speed = MassSpeedGet(MoveUp) << 1;
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH2, temp / 3, (temp * 2) / 3, speed);
	}
#endif
	else
		Part_Date_Set(MoveUp_Flex, 1, FLEX_FIX_CIRCLE, temp, 0, 3);
}

bool Move_Update_Get(u8 whi)
{
	if (Move_Flag[whi].Speed)
		return 1;
	if (Move_Flag[whi].Width)
		return 1;
	if (Move_Flag[whi].Inten)
		return 1;
	if (Move_Flag[whi].Wise == 0xff)
		return 1;
	if (Move_Flag[whi].Area == 0xff)
		return 1;
	return 0;
}

void MoveUp_Manual_Wise_Deal(void)
{
	u8 i, temp, speed;
	if (Move_Flag[MoveUp].Wise == MassWiseGet(MoveUp))
		return;
	Move_Flag[MoveUp].Wise = MassWiseGet(MoveUp);
	speed = MassSpeedGet(MoveUp) << 1;
	temp = MassWiseGet(MoveUp);
	switch (temp)
	{
	case TZ:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		break;

	case RNZ:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		break;

	case PD:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_ACT, 0, 0, speed);
		break;

	case RPZ:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_ACT, 0, 0, speed);
		break;

	case PDB:
		i = speed << 4 | MassSpeedGet(MoveUp);
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_BS, 30, 30, i);
		break;

	case ZYS:
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveUp_Manual_Inten_Get();
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;
#ifdef USE_S0555
	case ZY:
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveUp_Manual_Inten_Get() / 3;
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH2, i, i * 2, speed);
		break;

	case JRZ:
		Part_Date_Set(MoveUp_Tread, 1, TREAD_FIX_CIRCLE, 100, 0, 5);
		// Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, speed);
		// Part_Date_Set(MoveUp_Flex, 1, FLEX_FOLLOW_KNEAD3, 0, 0, 3);
		i = speed << 4 | MassSpeedGet(MoveUp); // @ md 260115
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_1R2R, 0x95, 0xff, i);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		break;
#endif
	case D3RNZ:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveUp_Manual_Inten_Get();
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case D3PD:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_ACT, 0, 0, speed);
		i = MoveUp_Manual_Inten_Get();
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case D3RPZ:
		Part_Date_Set(MoveUp_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveUp_Tap, 1, FLAP_ACT, 0, 0, speed);
		i = MoveUp_Manual_Inten_Get();
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case ZYD:
		Part_Date_Set(MoveUp_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveUp_Manual_Inten_Get();
		Part_Date_Set(MoveUp_Flex, 1, FLEX_BACK_FORTH2, i >> 1, i, speed);
		break;

	default:
		break;
	}

#ifdef USE_S0555
	if (temp == TZ || temp == PD || temp == PDB || temp == D3PD || temp == ZYS || temp == ZYD || temp == ZY)
#else
	if (temp == TZ || temp == PD || temp == PDB || temp == D3PD || temp == ZYS || temp == ZYD)
#endif
	{
		Move_Manual_Update_Width(MoveUp);
	}
	if (temp == TZ || temp == RNZ || temp == PD || temp == RPZ || temp == PDB)
	{
		MoveUp_Manual_Update_Inten(); // *  手动都修改伸缩 250901
	}
}

void MoveUp_Manual_Area_Deal(void)
{
	u8 temp, speed;
#ifdef USE_S0555
	if (MassWiseGet(MoveUp) == JRZ) // @ 肩位提抓 251205
	{
		MassAreaSet(MoveUp, Area_Fixed);
		return;
	}
#endif
	if (Move_Flag[MoveUp].Area == MassAreaGet(MoveUp))
		return;
	Move_Flag[MoveUp].Area = MassAreaGet(MoveUp);
	speed = MassSpeedGet(MoveUp) << 1;
	if (speed == 0)
		speed = 6;
	temp = MassAreaGet(MoveUp);
	switch (temp)
	{
	case Area_Null:
		Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);
		break;
	case Area_Upper:
#ifdef USE_M2
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 15, 40, speed);
#else
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 15, 60, speed);
#endif
		break;
	case Area_Lower:
#ifdef USE_M2
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 40, 65, speed);
#else
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 60, 100, speed);
#endif
		break;
	case Area_Full:
#ifdef USE_M2_4D
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 15, 60, speed);
#elif defined USE_M2
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 15, 65, speed);
#else
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH, 15, 100, speed);
#endif
		break;
	case Area_Fixed:
		Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);
		break;
	case Area_Part:
		Part_Date_Set(MoveUp_Tread, 1, TREAD_BACK_AND_FORTH_CUR_POINT, MoveArea_Space, 0, speed);
		break;
	default:
		break;
	}
}

void MoveUp_Manual_Deal(void) // * add 250901  上机芯手动处理
{
	if (Move_Flag[MoveUp].Speed)
	{
		Move_Flag[MoveUp].Speed = 0;
		Move_Manual_Update_Speed(MoveUp); //* 速度变化更新
		if (Move_Update_Get(MoveUp) == 0)
			return;
	}

	if (Move_Flag[MoveUp].Width)
	{
		Move_Flag[MoveUp].Width = 0;
		Move_Manual_Update_Width(MoveUp); //* 宽度变化更新
		if (Move_Update_Get(MoveUp) == 0)
			return;
	}

	if (Move_Flag[MoveUp].Inten)
	{
		Move_Flag[MoveUp].Inten = 0;
		MoveUp_Manual_Update_Inten(); //* 顶出变化更新
		if (Move_Update_Get(MoveUp) == 0)
			return;
	}
	MoveUp_Manual_Wise_Deal(); //* 手法处理
	MoveUp_Manual_Area_Deal(); //* 区域处理
}

/***************************************************************************************************************************************
 * 描述：手动调节Y轴位置
 * 参数：无
 * 返回：无
 * 注意：当发Part1的数据时，会刷新、覆盖手法动作；只能通过Part0调节Y轴位置，增减的圈数需保证速度最大时动作连贯，不连贯时适量增加或直接给很大
 * 由于通讯和机芯处理动作时存在的时间差，按键抬起到机芯停止动作，机芯位置会稍微超过停止的设置圈数，会有个倒退动作，后续需要可根据当前速度做补偿
 ****************************************************************************************************************************************/
void MoveUp_TreadKeyAdjust(void)
{
	uint16_t CurCircle;
	static u8 StopCmdSend = 0; /* 由运动到停止状态，多一条数据，发送停止的命令 */

	if (PreventFlag > 0 && KeyPush.TreadKeyState == TreadState_Dw) // * md 250923
	{
		if (PreventFlag == 1)
			PreventFlag = 2; // * 下机芯弹开
		StopCmdSend = 0;
		Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);
		return;
	}

	u8 speed = MassSpeedGet(MoveUp) << 1;
	if (speed == 0) // * 不能没有速度
		speed = 6;
	if (TreadState_Up == KeyPush.TreadKeyState)
	{
		StopCmdSend = 1;
		Part_Date_Set(MoveUp_Tread, 1, TREAD_FWD, 0, 0, speed);
	}
	else if (TreadState_Dw == KeyPush.TreadKeyState)
	{
		StopCmdSend = 1;
		Part_Date_Set(MoveUp_Tread, 1, TREAD_REV, 0, 0, speed);
	}
	else if (StopCmdSend) /* 按键抬起后，设置当前的圈数为目的圈数 */
	{
		StopCmdSend = 0;
		Part_Date_Set(MoveUp_Tread, 1, TREAD_STOP, 0, 0, 0);
	}
}

/*****************************************************************************************************
 * 描述：发送数据给上机芯 新协议
 * 参数：无
 * 返回：无
 * 注意：
 *****************************************************************************************************/
void MSG2MoveUp_Deal(void)
{
	u16 temp1;
	u8 MSG2MoveUpdate[sizeof(MSG2Move_S)] = {0};

	MSG2Move[MoveUp].UserMode = 2;

	if (WorkStateGet() == WorkState_Pause || MoveUpDw_Loss_Flag) // * add MoveUpDw_Loss_Flag 251210
	{
		MoveUp_Manual_Pause(); // * 上机芯停止
	}
	else if (WorkStateGet() == WorkState_Work || WorkStateGet() == WorkState_BodyCheck)
	{
		if (AutoModeGet() == 0 && AtTabGet(Table_Auto)->CurAutoTab == FA_AUTO_CLOSE)
		{
			MoveUp_Manual_Deal();	 // * add 250903 上机芯手动处理
			MoveUp_TreadKeyAdjust(); // * move here 250903 上机芯上下调节
		}
	}

	memcpy(&MSG2MoveUpdate, &MSG2Move[MoveUp], sizeof(MSG2MoveUpdate));
	_485_ComTx(MoveUpAddr, MainAddr, MSG2MoveUp_Count, 0x0001, sizeof(MSG2MoveUpdate), MSG2MoveUpdate);
}

#ifdef USE_M2
#ifdef USE_M2_4D
u8 MoveDw_Manual_Inten_Get(void)
{
	u8 temp;
	switch (nBackIntensity_M2) // todo 下机芯强度需要重新定制 260108
	{
	default:
	case 0:
		if (MassWiseGet(MoveDw) == ZYS || MassWiseGet(MoveDw) == ZYD ||
			MassWiseGet(MoveDw) == D3RNZ || MassWiseGet(MoveDw) == D3PD || MassWiseGet(MoveDw) == D3RPZ)
			temp = 5;
		else
			temp = 0;
		break;
	case 1:
		temp = 10;
		break;
	case 2:
		temp = 20;
		break;
	case 3:
		temp = 30;
		break;
	case 4:
		temp = 40; //* 最大 65
		break;
	}
	return temp;
}

void MoveDw_Manual_Update_Inten(void)
{
	u8 temp = MoveDw_Manual_Inten_Get();
	u8 speed;
	if (MassWiseGet(MoveDw) == ZYS || MassWiseGet(MoveDw) == D3RNZ || MassWiseGet(MoveDw) == D3PD || MassWiseGet(MoveDw) == D3RPZ)
	{
		speed = MassSpeedGet(MoveDw) << 1;
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH, temp, 0, speed);
	}
	else if (MassWiseGet(MoveDw) == ZYD)
	{
		speed = MassSpeedGet(MoveDw) << 1;
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH2, temp >> 1, temp, speed);
	}
	else
		Part_Date_Set(MoveDw_Flex, 1, FLEX_FIX_CIRCLE, temp, 0, 3);
}
#endif
void MoveDw_Manual_Wise_Deal(void) //* 下机芯手动手法处理
{
	u8 i, temp, speed;
	if (Move_Flag[MoveDw].Wise == MassWiseGet(MoveDw))
		return;
	Move_Flag[MoveDw].Wise = MassWiseGet(MoveDw);
	speed = MassSpeedGet(MoveDw) << 1;
	temp = MassWiseGet(MoveDw);
	switch (temp)
	{
	case TZ:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
		break;

	case RNZ:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
		break;

	case PD:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, speed);
		break;

	case RPZ:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, speed);
		break;

	case PDB:
		i = speed << 4 | MassSpeedGet(MoveDw);
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_BS, 30, 30, i);
		break;
#ifdef USE_M2_4D
	case ZYS:
		Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveDw_Manual_Inten_Get();
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case D3RNZ:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveDw_Manual_Inten_Get();
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case D3PD:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, speed);
		i = MoveDw_Manual_Inten_Get();
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case D3RPZ:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_FWD, 0, 0, speed);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, speed);
		i = MoveDw_Manual_Inten_Get();
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH, i, 0, speed);
		break;

	case ZYD:
		Part_Date_Set(MoveDw_Tap, 1, FLAP_STOP, 0, 0, 0);
		i = MoveDw_Manual_Inten_Get();
		Part_Date_Set(MoveDw_Flex, 1, FLEX_BACK_FORTH2, i >> 1, i, speed);
		break;
#elif defined USE_S0555
	case ZYS:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, 2);
		break;

	case ZY:
		Part_Date_Set(MoveDw_Knead, 1, KNEAD_STOP, 0, 0, 0);
		Part_Date_Set(MoveDw_Tap, 1, FLAP_ACT, 0, 0, 3);
		break;
#endif
	default:
		break;
	}
#ifdef USE_M2_4D
	if (temp == TZ || temp == PD || temp == PDB || temp == D3PD || temp == ZYS || temp == ZYD)
	{
		Move_Manual_Update_Width(MoveDw);
	}
	if (temp == TZ || temp == RNZ || temp == PD || temp == RPZ || temp == PDB)
	{
		MoveDw_Manual_Update_Inten(); // *  手动都修改伸缩 250901
	}
#else
	if (temp == TZ || temp == PD || temp == PDB)
	{
		Move_Manual_Update_Width(MoveDw);
	}
#endif
}
/*****************************************************************************************************
 * 描述：发送数据给下机芯 新协议
 * 参数：无
 * 返回：无
 * 注意：
 *****************************************************************************************************/
void MoveDw_Manual_Deal(void) // * add 250901
{
	u8 i, temp, speed;
	if (Move_Flag[MoveDw].Speed) //* 速度变化
	{
		Move_Flag[MoveDw].Speed = 0;
		Move_Manual_Update_Speed(MoveDw);
		if (Move_Update_Get(MoveDw) == 0)
			return;
	}

	if (Move_Flag[MoveDw].Width) //* 宽度变化
	{
		Move_Flag[MoveDw].Width = 0;
		Move_Manual_Update_Width(MoveDw);
		if (Move_Update_Get(MoveDw) == 0)
			return;
	}
#ifdef USE_M2_4D
	if (Move_Flag[MoveDw].Inten)
	{
		Move_Flag[MoveDw].Inten = 0;
		MoveDw_Manual_Update_Inten(); //* 顶出变化更新
		if (Move_Update_Get(MoveDw) == 0)
			return;
	}
#endif
	MoveDw_Manual_Wise_Deal();
}

/***************************************************************************************************************************************
 * 描述：手动调节Y轴位置
 * 参数：无
 * 返回：无
 * 注意：当发Part1的数据时，会刷新、覆盖手法动作；只能通过Part0调节Y轴位置，增减的圈数需保证速度最大时动作连贯，不连贯时适量增加或直接给很大
 * 由于通讯和机芯处理动作时存在的时间差，按键抬起到机芯停止动作，机芯位置会稍微超过停止的设置圈数，会有个倒退动作，后续需要可根据当前速度做补偿
 ****************************************************************************************************************************************/
void MoveDw_TreadKeyAdjust(void)
{
	uint16_t CurCircle;
	static u8 StopDwCmdSend = 0; /* 由运动到停止状态，多一条数据，发送停止的命令 */

	if (PreventFlag > 0 && KeyPush.M2TreadKeyState == TreadState_Up) // * 增加
	{
		StopDwCmdSend = 0;
		Part_Date_Set(MoveDw_Tread, 1, TREAD_STOP, 0, 0, 0);
		return;
	}

	u8 speed = MassSpeedGet(MoveDw) << 1;
	if (speed == 0) // * 不能没有速度
		speed = 6;
	if (TreadState_Up == KeyPush.M2TreadKeyState)
	{
		if (MassAreaGet(MoveUp) == Area_Full || MassAreaGet(MoveUp) == Area_Lower)
			return;
		StopDwCmdSend = 1;
		Part_Date_Set(MoveDw_Tread, 1, TREAD_REV, 0, 0, speed);
	}
	else if (TreadState_Dw == KeyPush.M2TreadKeyState)
	{
		StopDwCmdSend = 1;
		Part_Date_Set(MoveDw_Tread, 1, TREAD_FWD, 0, 0, speed);
	}
	else if (StopDwCmdSend) /* 按键抬起后，设置当前的圈数为目的圈数 */
	{
		StopDwCmdSend = 0;
		Part_Date_Set(MoveDw_Tread, 1, TREAD_STOP, 0, 0, 0);
	}
}

/*****************************************************************************************************
 * 描述：发送数据给上机芯 新协议
 * 参数：无
 * 返回：无
 * 注意：
 *****************************************************************************************************/
void MSG2MoveDw_Deal(void)
{
	u16 temp1;
	u8 MSG2MoveDwdate[sizeof(MSG2Move_S)] = {0};

	MSG2Move[MoveDw].UserMode = 2;

	if (AtTabGet(Table_Auto)->CurAutoTab == FA_AUTO_CLOSE && WorkStateGet() == WorkState_Work && AutoModeGet() == 0)
	{
		MoveDw_Manual_Deal();	 // * add 250903 下机芯手动处理
		MoveDw_TreadKeyAdjust(); // * move here 250903 下机芯上下调节
	}

	if (WorkStateGet() == WorkState_Pause || MoveUpDw_Loss_Flag) // * add MoveUpDw_Loss_Flag 251210
	{
		MoveDw_Manual_Pause(); // * 下机芯停止
	}

	memcpy(&MSG2MoveDwdate, &MSG2Move[MoveDw], sizeof(MSG2MoveDwdate));
	_485_ComTx(MoveDwAddr, MainAddr, MSG2MoveDw_Count, 0x0001, sizeof(MSG2MoveDwdate), MSG2MoveDwdate);
}
#endif
// * 机芯复位处理
void MoveUpDw_Rest(void)
{
	if (Massage_Flag.Reset == 0)
	{
		Massage_Flag.Reset = 1;
		MassMode_Load(A_Reset);
	}
	else
	{
		if (AtTabGet(Table_Auto)->PartOverMark == EXEC_Reset)
		{
			PartIf_Para.KneadZero = 1;
			AtTabSet_PartOverMark(Table_Auto, 0); //* 复位标志位清除
			Core_RestSuccess |= 0x03;
			Massage_Flag.Reset = 0;
			MassMode_Load(FA_AUTO_CLOSE);
		}
		else if (AtTabGet(Table_Auto)->CurAutoTab != A_Reset)
		{
			MassMode_Load(A_Reset);
		}
	}
}
// * 机芯复位
void CoreRest(void)
{
	if (MChair.CFG.Bit.CoreReset == 0)
		return;
	if (WorkStateGet() == WorkState_Pause) // * 增加 暂停不复位 250420
		return;
	if ((Core_RestSuccess & 0x03) == 0x00) // * 上下机芯复位合并 250924
	{
		MoveUpDw_Rest();
	}
}

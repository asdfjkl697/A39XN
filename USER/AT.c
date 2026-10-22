#include "AT.h"
#include "Table.h"
#include "MassageEXEC.h"

/*
 * 变量定义 *************************************************************************************************************************************
 */
#ifdef Debug_Mode
AT_CMD_S AT_Table[TableNum];
#else
static AT_CMD_S AT_Table[TableNum];
#endif

extern ActionTabAttr_t Air_Addr[];
extern ActionTabAttr_t MassMode_Addr[];

PartIf_T PartIf_Para;	   // * 指令判断条件 251105
u8 ASSIGN_Array[ASSIGNum]; // * 赋值数组 251126

/*
 ************************************************************************************************************************************************
 * 函数定义 *************************************************************************************************************************************
 ************************************************************************************************************************************************
 */
void AT_Init(void)
{
	u8 i;
	for (i = 0; i < TableNum; i++)
		AT_Table[i].State = 0;

	// memset(AT_Table, 0, sizeof(AT_Cmd_S)*TableNum);
	memset(&PartIf_Para, 0, sizeof(PartIf_T));
}

/************************************************************************************************************************************************
 * 描述：读表处理
 * 参数：cmd_p   操作表格的属性
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
// bool MSG2MoveUp_Knead_Update = 0;
// bool MSG2MoveUp_Tread_Update = 0;
// bool MSG2MoveUp_Tap_Update = 0;
// bool MSG2MoveUp_Flex_Update = 0;
u8 SubPartLoop_Cnt = 1;	 //* 默认循环一次 防止出错
u8 Sub2PartLoop_Cnt = 1; // todo add 260617
void AT_TableHandle(AT_CMD_S *cmd_p, u8 tablenum)
{
	CoreMixOtherTabItem_t *AutoActionTab;
	CoreMixOtherTabItem_t *pAutoExeStepData;

	uint8_t module;
	uint16_t circle_temp;
	bool move_finish = 0;
	bool movedw_finish = 0;

	// 	if (tablenum == Table_Auto)
	// 	{
	// 		if (AtTabGet(Table_Sub)->State > 0 || AtTabGet(Table_Sub2)->State > 0) //* 子表工作 主表停止
	// 			return;
	// 		AutoActionTab = cmd_p->AutoTable; /* 取出要执行的动作表格 */
	// 	}
	// 	else if (tablenum == Table_Sub)
	// 	{

	// 		AutoActionTab = cmd_p->AutoSubTable;
	// 	}
	// 	else if (tablenum == Table_Sub2)
	// 	{
	// 		AutoActionTab = cmd_p->AutoSubTable;
	// 	}
	// 	else if (tablenum == Table_Air)
	// 	{
	// 		AutoActionTab = cmd_p->AirTable;
	// 	}
	// #ifdef USE_M2
	// 	else if (tablenum == Table_MoveDw)
	// 	{
	// 		AutoActionTab = cmd_p->MoveDwTable;
	// 	}
	// #endif

	switch (tablenum)
	{
	case Table_Auto:
		if (AtTabGet(Table_Sub)->State > 0 || AtTabGet(Table_Sub2)->State > 0) //* 子表工作 主表停止
			return;
		AutoActionTab = cmd_p->AutoTable; /* 取出要执行的动作表格 */
		break;
	case Table_Sub:
		if (AtTabGet(Table_Sub2)->State > 0) //* 子表工作 主表停止
			return;
		AutoActionTab = cmd_p->AutoSubTable; /* 取出要执行的动作表格 */
		break;
	case Table_Sub2:
		AutoActionTab = cmd_p->AutoSub2Table; /* 取出要执行的动作表格 */
		break;
	case Table_Air:
		AutoActionTab = cmd_p->AirTable; /* 取出要执行的动作表格 */
		break;
#ifdef USE_M2
	case Table_MoveDw:
		AutoActionTab = cmd_p->MoveDwTable; /* 取出要执行的动作表格 */
		break;
#endif
	default:
		break;
	}

	if (MSG4Move[MoveUp].Knead.Finish == 1 && MSG4Move[MoveUp].Tread.Finish == 1 && MSG4Move[MoveUp].Flex.Finish == 1 && MSG4Move[MoveUp].Tap.Finish == 1)
		move_finish = 1;
#ifdef USE_M2
#ifdef USE_M2_4D
	if (MSG4Move[MoveDw].Knead.Finish == 1 && MSG4Move[MoveDw].Tread.Finish == 1 && MSG4Move[MoveDw].Flex.Finish == 1 && MSG4Move[MoveDw].Tap.Finish == 1)
#else
	if (MSG4Move[MoveDw].Knead.Finish == 1 && MSG4Move[MoveDw].Tread.Finish == 1 && MSG4Move[MoveDw].Tap.Finish == 1)
#endif
		movedw_finish = 1;
	if (tablenum == Table_MoveDw) //* 只判断下机芯
		move_finish = movedw_finish;
	else if (movedw_finish == 0 && Massage_Flag.DwFinish == 1 && (tablenum == Table_Auto || tablenum == Table_Sub || tablenum == Table_Sub2)) //* 需要判断上、下机芯完成
		move_finish = 0;
#endif
	if (tablenum == Table_Air)
	{
		move_finish = 1; //* 不需要判断机芯
	}

	if ((move_finish && cmd_p->RunTime100Ms >= cmd_p->PartRunTime) || cmd_p->RunTime100Ms >= cmd_p->PartOverTime)
	{

		if (tablenum == Table_Auto || tablenum == Table_Sub || tablenum == Table_Sub2) //* 主表与子表 复位下机芯判断开关
			Massage_Flag.DwFinish = 0;
		cmd_p->ReadTBFlag = 1;
		if (cmd_p->TableStep > 0)
		{
			if (cmd_p->LoopCnt == 0) //* 正常运行
				cmd_p->TableStep++;
			else //* 循环处理
			{
				if (cmd_p->TableStep < cmd_p->LoopEnd)
					cmd_p->TableStep++;
				else if (cmd_p->TableStep == cmd_p->LoopEnd)
				{
					if (cmd_p->LoopCnt < 0xfe) // todo 增加死循环 260626
						cmd_p->LoopCnt--;
					cmd_p->TableStep = cmd_p->LoopStart;
				}
				else
					cmd_p->LoopCnt = 0; //* 防止出错
			}
		}
	}

	while (cmd_p->ReadTBFlag) //* 持续读表
	{
		pAutoExeStepData = &AutoActionTab[cmd_p->TableStep];
		module = pAutoExeStepData->Module;
		if (PartIf_Para.Jump2Exec) // * 条件判断语句 251105
		{
			if (module == Part_EXEC)
			{
				PartIf_Para.Jump2Exec = 0;
			}
			module = 0xff; // 跳过
		}
		switch (module)
		{
		default:
			break;
#ifdef USE_M2
		case MoveDw_Tread:
		case MoveDw_Knead:
		case MoveDw_Tap:
		case MoveDw_Flex:
			Part_Date_Set(module, 0, pAutoExeStepData->Method,
						  pAutoExeStepData->Para1, pAutoExeStepData->Para2, pAutoExeStepData->Para3);
			break;
		case MoveDw_TreadB:
			circle_temp = BodyAreaDw[pAutoExeStepData->Para1];
			Part_Date_Set(MoveDw_Tread, 0, pAutoExeStepData->Method,
						  circle_temp & 0xff, circle_temp >> 8, pAutoExeStepData->Para3);
			break;
		case MoveDw_TreadF:
			MoveFolw_Cmd.Tread_cmd = pAutoExeStepData->Method;
			circle_temp = pAutoExeStepData->Para2;
			circle_temp = circle_temp << 8 | pAutoExeStepData->Para1;
			Move_Follow_Space = circle_temp; //* 机芯跟随距离
			break;
		case MoveDw_KneadF:
			Part_Date_Set(MoveDw_Knead, 0, KNEAD_STOP, 0, 0, 0); //* 停止 251104
			MoveFolw_Cmd.Knead_cmd = pAutoExeStepData->Method;
			break;
		case MoveDw_TapF:
			MoveFolw_Cmd.Tap_cmd = pAutoExeStepData->Method;
			break;
		case MoveDw_FlexF:
			MoveFolw_Cmd.Flex_cmd = pAutoExeStepData->Method;
			break;
#endif
		case MoveUp_Tread:
		case MoveUp_Knead:
		case MoveUp_Tap:
		case MoveUp_Flex:
			Part_Date_Set(module, 0, pAutoExeStepData->Method,
						  pAutoExeStepData->Para1, pAutoExeStepData->Para2, pAutoExeStepData->Para3);
			break;
		case MoveUp_TreadB:
			circle_temp = BodyArea[pAutoExeStepData->Para1];
			Part_Date_Set(MoveUp_Tread, 0, pAutoExeStepData->Method,
						  circle_temp & 0xff, circle_temp >> 8, pAutoExeStepData->Para3);
			break;

		case Air_UpDw:
		case Air_SHD:
		case Air_ArmL:
		case Air_ArmR:
		case Air_CalfS:
		case Air_FeetS:
		case Air_Ankle:
#if (AIR_CLAFBOT)
		case Air_CalfB:
#elif (AIR_SEAT)
		case Air_Seat:
#endif
			if (AirModeGet() == AirModeClose) // * md 251113
				break;
			if ((AirModeGet() == AirModeUpper || AirModeGet() == AirModeLower) && tablenum != Table_Air) // * add 251113
				break;
			if (Get_Gas_Keep_State())
				Gas_Keep_Clear(); //* 清除保压
			AirPart_Date_Set(module, 0, pAutoExeStepData->Method,
							 pAutoExeStepData->Para1, pAutoExeStepData->Para2, pAutoExeStepData->Para3);
			break;

		case Air_Del:
			if (AirModeGet() == AirModeClose)
				break;
			if ((AirModeGet() == AirModeUpper || AirModeGet() == AirModeLower) && tablenum != Table_Air)
				break;
			AirPart_Del(pAutoExeStepData->Method);
			break;

		case Air_Refresh:
			if (AirModeGet() < AirModeFull) //* 250917
				break;
			Gas_Keep_Set(pAutoExeStepData->Method);
			break;

		case Air_Keep:
			if (AirModeGet() < AirModeFull) //* 250917
				break;
			AT_TableStop(Table_Air);
			AtTabSet_ID(Table_Air, FA_GAS_CLOSE);
			Gas_Keep_Set(pAutoExeStepData->Method);
			break;

		case Motor_Flex:
		case Motor_Calf:
		case Motor_Feet:
		case Motor_Zero:
		case Motor_Sway:
		case Motor_Rock:
			MotorPart_Date_Set(module, 0, pAutoExeStepData->Method,
							   pAutoExeStepData->Para1, pAutoExeStepData->Para2, pAutoExeStepData->Para3);
			break;
		case Part_AIR:
			if (AirModeGet() == AirModeUpper && AirModeGet() == AirModeLower)
				break;
			if (pAutoExeStepData->Method == PartCon_Run)
				Air_All_Change(pAutoExeStepData->Para1);
			else if (pAutoExeStepData->Method == PartCon_Stop) // * ADD 251120
			{
				AT_TableStop(Table_Air);
				AtTabSet_ID(Table_Air, FA_GAS_CLOSE);
				Gas_Clear();
				memset(&AirT_Work, 0, sizeof(AirT_Work_S));
			}
			break;
#ifdef USE_M2
		case Part_MVDW: //* 调用下机芯程序
			if (pAutoExeStepData->Method == PartCon_Run)
			{
				memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T)); // TODO ADD 260201
				MoveDwMassMode_Load(pAutoExeStepData->Para1);
			}
			else if (pAutoExeStepData->Method == PartCon_Stop) //* 关闭下机芯自动程序及跟随命令
			{
				// MoveFolw_Cmd.Tread_cmd = FOLW_STOP;
				// MoveFolw_Cmd.Knead_cmd = FOLW_STOP;
				// MoveFolw_Cmd.Tap_cmd = FOLW_STOP;
				// MoveFolw_Cmd.Flex_cmd = FOLW_STOP;
				memset(&MoveFolw_Cmd, 0, sizeof(Move_Follow_Cmd_T)); // TODO md 260201
				MoveDw_Manual_Pause();
				MoveDwMassMode_Load(DWTB_CLOSE);
			}
			break;
#endif

		case Part_SUB: //* 调用子程序
			SubMassMode_Load(pAutoExeStepData->Para1);
			if (pAutoExeStepData->Method == ConMth_Loop)
				SubPartLoop_Cnt = pAutoExeStepData->Para2;
			cmd_p->ReadTBFlag = 0; // * 读表结束
			break;

		case Part_SUB2: //* 调用二级子程序
			Sub2MassMode_Load(pAutoExeStepData->Para1);
			if (pAutoExeStepData->Method == ConMth_Loop)
				Sub2PartLoop_Cnt = pAutoExeStepData->Para2;
			cmd_p->ReadTBFlag = 0; // * 读表结束
			break;

		case Part_STOP:
			if (tablenum == Table_Sub)
				SubMassMode_Load(SUBTB_CLOSE);
			else if (tablenum == Table_Sub2)
				Sub2MassMode_Load(SUB2TB_CLOSE);
			break;

		case Part_LOOP:
			if (tablenum == Table_Sub2 && pAutoExeStepData->Method == ConMth_Loop)
			{
				if (Sub2PartLoop_Cnt < 1) //*最少循环一次
					Sub2PartLoop_Cnt = 1;
				cmd_p->LoopCnt = Sub2PartLoop_Cnt - 1;
				Sub2PartLoop_Cnt = 1; //* 次数复位
			}
			else if (tablenum == Table_Sub && pAutoExeStepData->Method == ConMth_Loop)
			{
				if (SubPartLoop_Cnt < 1) //*最少循环一次
					SubPartLoop_Cnt = 1;
				cmd_p->LoopCnt = SubPartLoop_Cnt - 1;
				SubPartLoop_Cnt = 1; //* 次数复位
			}
			else
			{
				if (pAutoExeStepData->Para1 < 1) //*最少循环一次
					pAutoExeStepData->Para1 = 1;
				cmd_p->LoopCnt = pAutoExeStepData->Para1 - 1; //* 循环次数 需要减1
			}
			if (pAutoExeStepData->Para2 < 1) //*最少循环一条
				pAutoExeStepData->Para2 = 1;
			cmd_p->LoopStart = cmd_p->TableStep + 1;
			cmd_p->LoopEnd = cmd_p->LoopStart + pAutoExeStepData->Para2 - 1;
			break;
		case Part_BREAK:
			if (cmd_p->LoopStart > 0 && cmd_p->LoopEnd > 0)
			{
				switch (pAutoExeStepData->Method)
				{
				case ConMth_TreadLess:
					break;
				case ConMth_TreadMore:
					break;
				case ConMth_TreadBLess:
					if (MoveUp_Circle <= BodyArea[pAutoExeStepData->Para1])
					{
						cmd_p->LoopCnt = 0;
						cmd_p->LoopStart = 0;
						cmd_p->LoopEnd = 0;
					}
					break;
				case ConMth_TreadBMore:
					if (MoveUp_Circle >= BodyArea[pAutoExeStepData->Para1])
					{
						cmd_p->LoopCnt = 0;
						cmd_p->LoopStart = 0;
						cmd_p->LoopEnd = 0;
					}
					break;
				default:
					break;
				}
			}
			break;
		case Part_ASSIGN:
			switch (pAutoExeStepData->Method)
			{
			case ASSIGN_Paradox:
				if (pAutoExeStepData->Para1 == (Air_SHD + 0x80))
				{
					AirT_Work.Paradox[AirT_SHD] = 1;
				}
				else if (pAutoExeStepData->Para1 == Air_SHD)
				{
					AirT_Work.Paradox[AirT_SHD] = 0;
				}
				break;
			case ASSIGN_Flex:
			case ASSIGN_KneadSpeed: // @ add 260119
			case ASSIGN_TapSpeed:
			case ASSIGN_TreadSpeed:
				ASSIGN_Array[pAutoExeStepData->Method] = pAutoExeStepData->Para1;
				break;
			case ASSIGN_Mark:
				cmd_p->PartOverMark = pAutoExeStepData->Para3; // @ 增加赋值 260129
			default:
				break;
			}
			break;
		case Part_IF:
			switch (pAutoExeStepData->Method)
			{
			case ConMth_KneadZero:
				if (PartIf_Para.KneadZero)
					PartIf_Para.Jump2Exec = 1;
				break;
			default:
				break;
			}
			break;
		case Part_EXEC:
			switch (pAutoExeStepData->Method)
			{
			default:
				break;
#ifdef USE_M2
			case ConMth_MoveDw:
				cmd_p->RunTime100Ms = 0;
				cmd_p->PartRunTime = pAutoExeStepData->Para1;		//* 运行时间
				cmd_p->PartOverTime = pAutoExeStepData->Para2 * 10; //* 超时时间
				MSG2Move[MoveDw].Tread.Update = Move_Flag[MoveDw].Tread_p;
				MSG2Move[MoveDw].Knead.Update = Move_Flag[MoveDw].Knead_p;
				MSG2Move[MoveDw].Tap.Update = Move_Flag[MoveDw].Tap_p;
				MSG2Move[MoveDw].Flex.Update = Move_Flag[MoveDw].Flex_p;
				Move_Flag[MoveDw].Tread_p = 0;
				Move_Flag[MoveDw].Knead_p = 0;
				Move_Flag[MoveDw].Tap_p = 0;
				Move_Flag[MoveDw].Flex_p = 0;

				MSG4Move[MoveDw].Knead.Finish = 0;
				MSG4Move[MoveDw].Tread.Finish = 0;
				MSG4Move[MoveDw].Tap.Finish = 0;
				MSG4Move[MoveDw].Flex.Finish = 0;
				break;
#endif
			case ConMth_MoveUp:
				cmd_p->RunTime100Ms = 0;
				cmd_p->PartRunTime = pAutoExeStepData->Para1;		//* 运行时间
				cmd_p->PartOverTime = pAutoExeStepData->Para2 * 10; //* 超时时间
				MSG2Move[MoveUp].Tread.Update = Move_Flag[MoveUp].Tread_p;
				MSG2Move[MoveUp].Knead.Update = Move_Flag[MoveUp].Knead_p;
				MSG2Move[MoveUp].Tap.Update = Move_Flag[MoveUp].Tap_p;
				MSG2Move[MoveUp].Flex.Update = Move_Flag[MoveUp].Flex_p;
				Move_Flag[MoveUp].Tread_p = 0;
				Move_Flag[MoveUp].Knead_p = 0;
				Move_Flag[MoveUp].Tap_p = 0;
				Move_Flag[MoveUp].Flex_p = 0;

				MSG4Move[MoveUp].Knead.Finish = 0;
				MSG4Move[MoveUp].Tread.Finish = 0;
				MSG4Move[MoveUp].Tap.Finish = 0;
				MSG4Move[MoveUp].Flex.Finish = 0;

#ifdef USE_M2
				if (MoveFolw_Cmd.Tread_cmd > FOLW_STOP)
				{
					MSG2Move[MoveDw].Tread.Update = Move_Flag[MoveDw].Tread_p;
					Move_Flag[MoveDw].Tread_p = 0;
					MSG4Move[MoveDw].Tread.Finish = 0;
				}
				if (MoveFolw_Cmd.Knead_cmd > FOLW_STOP)
				{
					MSG2Move[MoveDw].Knead.Update = Move_Flag[MoveDw].Knead_p;
					Move_Flag[MoveDw].Knead_p = 0;
					MSG4Move[MoveDw].Knead.Finish = 0;
				}
				if (MoveFolw_Cmd.Tap_cmd > FOLW_STOP)
				{
					MSG2Move[MoveDw].Tap.Update = Move_Flag[MoveDw].Tap_p;
					Move_Flag[MoveDw].Tap_p = 0;
					MSG4Move[MoveDw].Tap.Finish = 0;
				}
				if (MoveFolw_Cmd.Flex_cmd > FOLW_STOP) // todo add 260117
				{
					MSG2Move[MoveDw].Flex.Update = Move_Flag[MoveDw].Flex_p;
					Move_Flag[MoveDw].Flex_p = 0;
					MSG4Move[MoveDw].Flex.Finish = 0;
				}
#endif
				break;
			case ConMth_Move:
				cmd_p->RunTime100Ms = 0;
				cmd_p->PartRunTime = pAutoExeStepData->Para1;		//* 运行时间
				cmd_p->PartOverTime = pAutoExeStepData->Para2 * 10; //* 超时时间
				MSG2Move[MoveUp].Tread.Update = Move_Flag[MoveUp].Tread_p;
				MSG2Move[MoveUp].Knead.Update = Move_Flag[MoveUp].Knead_p;
				MSG2Move[MoveUp].Tap.Update = Move_Flag[MoveUp].Tap_p;
				MSG2Move[MoveUp].Flex.Update = Move_Flag[MoveUp].Flex_p;
				Move_Flag[MoveUp].Tread_p = 0;
				Move_Flag[MoveUp].Knead_p = 0;
				Move_Flag[MoveUp].Tap_p = 0;
				Move_Flag[MoveUp].Flex_p = 0;

				// * finish 清零 251010
				MSG4Move[MoveUp].Knead.Finish = 0;
				MSG4Move[MoveUp].Tread.Finish = 0;
				MSG4Move[MoveUp].Tap.Finish = 0;
				MSG4Move[MoveUp].Flex.Finish = 0;
#ifdef USE_M2
				MSG2Move[MoveDw].Tread.Update = Move_Flag[MoveDw].Tread_p;
				MSG2Move[MoveDw].Knead.Update = Move_Flag[MoveDw].Knead_p;
				MSG2Move[MoveDw].Tap.Update = Move_Flag[MoveDw].Tap_p;
				MSG2Move[MoveDw].Flex.Update = Move_Flag[MoveDw].Flex_p;
				Move_Flag[MoveDw].Tread_p = 0;
				Move_Flag[MoveDw].Knead_p = 0;
				Move_Flag[MoveDw].Tap_p = 0;
				Move_Flag[MoveDw].Flex_p = 0;
				Massage_Flag.DwFinish = 1; //* 判断下机芯完成

				MSG4Move[MoveDw].Knead.Finish = 0;
				MSG4Move[MoveDw].Tread.Finish = 0;
				MSG4Move[MoveDw].Tap.Finish = 0;
				MSG4Move[MoveDw].Flex.Finish = 0;
#endif
				break;
			case ConMth_Air:
				cmd_p->RunTime100Ms = 0;
				cmd_p->PartRunTime = pAutoExeStepData->Para1 * 10;	//* 运行时间 * 10
				cmd_p->PartOverTime = pAutoExeStepData->Para2 * 10; //* 超时时间
				break;
			case ConMth_Motor:
				cmd_p->RunTime100Ms = 0;
				cmd_p->PartRunTime = pAutoExeStepData->Para1;		//* 运行时间
				cmd_p->PartOverTime = pAutoExeStepData->Para2 * 10; //* 超时时间
				break;
			case ConMth_All:
				break;
			case ConMth_AC:
				AT_TableReset(tablenum);
				cmd_p->ReadTBFlag = 1; // * 重新读表
				break;
			case ConMth_AD:
#ifdef USE_M2
				if (tablenum == Table_MoveDw)
					MoveDw_Manual_Pause();
#endif
				cmd_p->PartOverMark = pAutoExeStepData->Para3;
				break;
			}
			cmd_p->ReadTBFlag = 0; // * 读表结束
			break;
		}
		if (cmd_p->ReadTBFlag)
			cmd_p->TableStep++; //* 正常运行
	}
}

/************************************************************************************************************************************************
 * 描述：动作表格读取 处理
 * 参数：cmd_p   操作表格的属性
 *       ID      操作表格的类型
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void AT_TableRead(AT_CMD_S *cmd_p, TableType_E ID)
{
	if (1 == cmd_p->State) /* 开始运行时复位参数 */
	{
		cmd_p->State = 2;
		cmd_p->TableStep = 0;
		cmd_p->RunTime100Ms = 0;
	}

	if (Table_Auto == ID) /* 处理自动程序 */
	{
		AT_TableHandle(cmd_p, Table_Auto);
	}
	else if (Table_Sub == ID)
	{
		AT_TableHandle(cmd_p, Table_Sub);
	}
	else if (Table_Sub2 == ID)
	{
		AT_TableHandle(cmd_p, Table_Sub2);
	}
	else if (Table_Air == ID) /* 处理气囊动作 */
	{
		AT_TableHandle(cmd_p, Table_Air);
	}
#ifdef USE_M2
	else if (Table_MoveDw == ID) /* 处理下机芯动作 */
	{
		AT_TableHandle(cmd_p, Table_MoveDw);
	}
#endif
}

void AT_State_Get(void)
{
}

/************************************************************************************************************************************************
 * 描述：动作表格处理   开始表格   将需要运行的表格地址放到数组的指定位置
 * 参数：cmd_p   操作表格的属性
 *       ID      操作表格的类型
 * 返回：无
 * 注意：无
 *************************************************************************************************************************************************/
void AT_TableStart(u8 ID, const u8 *Table)
{
	if (ID >= TableNum)
		return;

	AT_Table[ID].State = 1;

	if (ID == Table_Auto)
	{
		AT_Table[ID].AutoTable = (CoreMixOtherTabItem_t *)Table;
		AT_Table[ID].ReadTBFlag = 1; //* test 250928
	}
	else if (ID == Table_Sub)
	{
		AT_Table[ID].AutoSubTable = (CoreMixOtherTabItem_t *)Table;
		AT_Table[ID].ReadTBFlag = 1; // todo add 260617
	}
	else if (ID == Table_Sub2)
	{
		AT_Table[ID].AutoSub2Table = (CoreMixOtherTabItem_t *)Table;
	}
	else if (ID == Table_Air)
	{
		AT_Table[ID].AirTable = (CoreMixOtherTabItem_t *)Table;
		AT_Table[ID].ReadTBFlag = 1; //* add 250915
	}
#ifdef USE_M2
	else if (ID == Table_MoveDw)
	{
		AT_Table[ID].MoveDwTable = (CoreMixOtherTabItem_t *)Table;
		AT_Table[ID].ReadTBFlag = 1; //* add 250924
	}
#endif
}

void AT_TableStop(u8 ID)
{
	AT_Table[ID].State = 0;

	if (Table_Auto == ID) /* 停止主表格时，伴随的气囊、下机芯表格也一起停止 */
	{
		AT_Table[Table_Sub2].State = 0;
		AT_Table[Table_Sub].State = 0;
		AT_Table[Table_Air].State = 0;
#ifdef USE_M2
		AT_Table[Table_MoveDw].State = 0;
#endif
	}
	memset(&AT_Table[ID], 0, sizeof(AT_CMD_S)); // * recover 250920
}

void AT_TableContinue(u8 ID)
{
	AT_Table[ID].State = 2;
};

void AT_TableReset(u8 ID)
{
	AT_Table[ID].State = 1;
}

void AT_TableClear(u8 ID) // * add 250905
{
	memset(&AT_Table[ID], 0, sizeof(AT_CMD_S));
}

void AT_Table_TimeRun(void)
{
	if (AT_Table[Table_Auto].State > 0 && AT_Table[Table_Sub].State == 0 && AT_Table[Table_Sub2].State == 0)
		AT_Table[Table_Auto].RunTime100Ms++;

	if (AT_Table[Table_Sub].State > 0 && AT_Table[Table_Sub2].State == 0)
		AT_Table[Table_Sub].RunTime100Ms++;

	if (AT_Table[Table_Sub2].State > 0)
		AT_Table[Table_Sub2].RunTime100Ms++;

	if (AT_Table[Table_Air].State > 0)
		AT_Table[Table_Air].RunTime100Ms++;
#ifdef USE_M2
	if (AT_Table[Table_MoveDw].State > 0)
		AT_Table[Table_MoveDw].RunTime100Ms++;
#endif
}

static uint8_t PreAutoMode = 0;
static uint8_t PreAirMode = 0;
void AT_Process(void)
{
	u8 i;
	AT_Table_TimeRun();

	for (i = 0; i < TableNum; i++)
	{
		if (AT_Table[i].State > 0)
		{
			AT_TableRead((AT_CMD_S *)(AT_Table + i), (TableType_E)i);
		}
	}
}

bool AT_RunGet(void)
{
	u8 i;
	for (i = 0; i < TableNum; i++)
	{
		if (AT_Table[i].State > 0)
		{
			return 1;
		}
	}
	return 0;
}

AT_CMD_S *AtTabGet(u8 whi)
{
	return &AT_Table[whi];
}

void AtTabSet_ID(u8 whi, u8 id)
{
	switch (whi)
	{
	case Table_Auto:
		AT_Table[Table_Auto].CurAutoTab = id;
		break;
	case Table_Sub:
		AT_Table[Table_Sub].CurSubTab = id;
		break;
	case Table_Sub2:
		AT_Table[Table_Sub2].CurSub2Tab = id;
		break;
	case Table_Air:
		AT_Table[Table_Air].CurAirTab = id;
		break;
#ifdef USE_M2
	case Table_MoveDw:
		AT_Table[Table_MoveDw].CurMoveDwTab = id;
		break;
#endif
	default:
		break;
	}
}

void AtTabSet_ReadTBFlag(u8 whi, u8 id)
{
	AT_Table[whi].ReadTBFlag = id;
}
//* 设置自动程序运行标志
void AtTabSet_PartOverMark(u8 whi, u8 id)
{
	AT_Table[whi].PartOverMark = id;
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmDisplayTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-30
 * Descript : 用于测试报警显示部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "AlarmDisplayTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "AlarmDisplayConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"
#include "AlarmInputConfig.h"
#include "GTestConfig.h"
#include "AlarmInfoRes.h"
static Project::HMIProject prj;
void AlarmDisplayTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(1000, 800);
	prjHelper.AddWin(1, 0, 0, 1000, 800, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*OFFX OFFY*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var4");

	/*权限设置*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var8");

	/*报警录入触发通知寄存器*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 98, 4, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 98, 4, "win1.var10");


	/*查询控制查询*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var12");

	/*隐藏信息设置*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var14");

	/*报警触发*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var16");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var17");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var18");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var19");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var20");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var21");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var22");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var23");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var24");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var25");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var26");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 12, 2, "win1.var27");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 12, 2, "win1.var28");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 13, 2, "win1.var29");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 13, 2, "win1.var30");
	
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 14, 2, "win1.var31");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 14, 2, "win1.var32");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 15, 2, "win1.var33");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 15, 2, "win1.var34");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 16, 2, "win1.var35");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 16, 2, "win1.var36");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 17, 2, "win1.var37");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 17, 2, "win1.var38");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 18, 2, "win1.var39");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 18, 2, "win1.var40");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var41");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var42");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var43");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var44");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 21, 2, "win1.var45");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 21, 2, "win1.var46");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 22, 2, "win1.var47");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 22, 2, "win1.var48");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 23, 2, "win1.var49");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 23, 2, "win1.var50");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 24, 2, "win1.var51");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 24, 2, "win1.var52");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 25, 2, "win1.var53");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 25, 2, "win1.var54");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 26, 2, "win1.var55");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 26, 2, "win1.var56");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 27, 2, "win1.var57");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 27, 2, "win1.var58");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 28, 2, "win1.var59");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 28, 2, "win1.var60");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 29, 2, "win1.var61");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 29, 2, "win1.var62");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 30, 2, "win1.var63");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 30, 2, "win1.var64");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 31, 2, "win1.var65");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 31, 2, "win1.var66");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 32, 2, "win1.var67");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 32, 2, "win1.var68");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 33, 2, "win1.var69");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 33, 2, "win1.var70");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 34, 2, "win1.var71");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 34, 2, "win1.var72");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 35, 2, "win1.var73");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 35, 2, "win1.var74");


	/*添加报警录入控件*/
	prjHelper.AddGCtrl(0, "AlarmGControl", "win1.tc6", [&prjHelper](void * param) {
		Project::AlarmInputConfig * cfg = (Project::AlarmInputConfig *)param;
		cfg->CtrlName = "win1.tc6";
		cfg->AlarmNoticeVarId.Vid = prjHelper.GetVarID("win1.var9");

		Project::AlarmInfoRes res1;
		res1.GroupName = 0;
		res1.GroupNo = 1;
		res1.RegType = 0;			//位寄存器
		res1.BtyeCondition = Project::Conditions::LT;
		res1.ComPareValue = 10;
		res1.BitCondition = Project::BitConditions::ON;
		res1.Text.Texts.push_back("Alarm1");
		res1.ReadRegVar = prjHelper.GetVarID("win1.var15");
		cfg->InfoLst.push_back(res1);

		Project::AlarmInfoRes res2;
		res2.GroupName = 0;
		res2.GroupNo = 2;
		res2.RegType = 0;			//位寄存器
		res2.BtyeCondition = Project::Conditions::LT;
		res2.ComPareValue = 10;
		res2.BitCondition = Project::BitConditions::ON;
		res2.Text.Texts.push_back("Alarm2");
		res2.ReadRegVar = prjHelper.GetVarID("win1.var17");
		cfg->InfoLst.push_back(res2);

		Project::AlarmInfoRes res3;
		res3.GroupName = 0;
		res3.GroupNo = 3;
		res3.RegType = 0;			//位寄存器
		res3.BtyeCondition = Project::Conditions::LT;
		res3.ComPareValue = 10;
		res3.BitCondition = Project::BitConditions::ON;
		res3.Text.Texts.push_back("Alarm3");
		res3.ReadRegVar = prjHelper.GetVarID("win1.var19");
		cfg->InfoLst.push_back(res3);

		Project::AlarmInfoRes res4;
		res4.GroupName = 0;
		res4.GroupNo = 4;
		res4.RegType = 0;			//位寄存器
		res4.BtyeCondition = Project::Conditions::LT;
		res4.ComPareValue = 10;
		res4.BitCondition = Project::BitConditions::ON;
		res4.Text.Texts.push_back("Alarm4");
		res4.ReadRegVar = prjHelper.GetVarID("win1.var21");
		cfg->InfoLst.push_back(res4);

		Project::AlarmInfoRes res5;
		res5.GroupName = 1;
		res5.GroupNo = 0;
		res5.RegType = 0;			//位寄存器
		res5.BtyeCondition = Project::Conditions::LT;
		res5.ComPareValue = 10;
		res5.BitCondition = Project::BitConditions::ON;
		res5.Text.Texts.push_back("Alarm5");
		res5.ReadRegVar = prjHelper.GetVarID("win1.var23");
		cfg->InfoLst.push_back(res5);

		Project::AlarmInfoRes res6;
		res6.GroupName = 1;
		res6.GroupNo = 1;
		res6.RegType = 0;			//位寄存器
		res6.BtyeCondition = Project::Conditions::LT;
		res6.ComPareValue = 10;
		res6.BitCondition = Project::BitConditions::ON;
		res6.Text.Texts.push_back("Alarm6");
		res6.ReadRegVar = prjHelper.GetVarID("win1.var25");
		cfg->InfoLst.push_back(res6);

		Project::AlarmInfoRes res7;
		res7.GroupName = 1;
		res7.GroupNo = 2;
		res7.RegType = 0;			//位寄存器
		res7.BtyeCondition = Project::Conditions::LT;
		res7.ComPareValue = 10;
		res7.BitCondition = Project::BitConditions::ON;
		res7.Text.Texts.push_back("Alarm7");
		res7.ReadRegVar = prjHelper.GetVarID("win1.var27");
		cfg->InfoLst.push_back(res7);

		Project::AlarmInfoRes res8;
		res8.GroupName = 1;
		res8.GroupNo = 3;
		res8.RegType = 0;			//位寄存器
		res8.BtyeCondition = Project::Conditions::LT;
		res8.ComPareValue = 10;
		res8.BitCondition = Project::BitConditions::ON;
		res8.Text.Texts.push_back("Alarm8");
		res8.ReadRegVar = prjHelper.GetVarID("win1.var29");
		cfg->InfoLst.push_back(res8);

		Project::AlarmInfoRes res9;
		res9.GroupName = 2;
		res9.GroupNo = 1;
		res9.RegType = 0;			//位寄存器
		res9.BtyeCondition = Project::Conditions::LT;
		res9.ComPareValue = 10;
		res9.BitCondition = Project::BitConditions::ON;
		res9.Text.Texts.push_back("Alarm9");
		res9.ReadRegVar = prjHelper.GetVarID("win1.var31");
		cfg->InfoLst.push_back(res9);

		Project::AlarmInfoRes res10;
		res10.GroupName = 2;
		res10.GroupNo = 2;
		res10.RegType = 0;			//位寄存器
		res10.BtyeCondition = Project::Conditions::LT;
		res10.ComPareValue = 10;
		res10.BitCondition = Project::BitConditions::ON;
		res10.Text.Texts.push_back("Alarm10");
		res10.ReadRegVar = prjHelper.GetVarID("win1.var33");
		cfg->InfoLst.push_back(res10);

		Project::AlarmInfoRes res11;
		res11.GroupName = 2;
		res11.GroupNo = 3;
		res11.RegType = 0;			//位寄存器
		res11.BtyeCondition = Project::Conditions::LT;
		res11.ComPareValue = 10;
		res11.BitCondition = Project::BitConditions::ON;
		res11.Text.Texts.push_back("Alarm11");
		res11.ReadRegVar = prjHelper.GetVarID("win1.var35");
		cfg->InfoLst.push_back(res11);

		Project::AlarmInfoRes res12;
		res12.GroupName = 2;
		res12.GroupNo = 4;
		res12.RegType = 0;			//位寄存器
		res12.BtyeCondition = Project::Conditions::LT;
		res12.ComPareValue = 10;
		res12.BitCondition = Project::BitConditions::ON;
		res12.Text.Texts.push_back("Alarm12");
		res12.ReadRegVar = prjHelper.GetVarID("win1.var37");
		cfg->InfoLst.push_back(res12);

		Project::AlarmInfoRes res13;
		res13.GroupName = 3;
		res13.GroupNo = 1;
		res13.RegType = 0;			//位寄存器
		res13.BtyeCondition = Project::Conditions::LT;
		res13.ComPareValue = 10;
		res13.BitCondition = Project::BitConditions::ON;
		res13.Text.Texts.push_back("Alarm13");
		res13.ReadRegVar = prjHelper.GetVarID("win1.var39");
		cfg->InfoLst.push_back(res13);

		Project::AlarmInfoRes res14;
		res14.GroupName = 3;
		res14.GroupNo = 2;
		res14.RegType = 0;			//位寄存器
		res14.BtyeCondition = Project::Conditions::LT;
		res14.ComPareValue = 10;
		res14.BitCondition = Project::BitConditions::ON;
		res14.Text.Texts.push_back("Alarm14");
		res14.ReadRegVar = prjHelper.GetVarID("win1.var41");
		cfg->InfoLst.push_back(res14);

		Project::AlarmInfoRes res15;
		res15.GroupName = 3;
		res15.GroupNo = 2;
		res15.RegType = 0;			//位寄存器
		res15.BtyeCondition = Project::Conditions::LT;
		res15.ComPareValue = 10;
		res15.BitCondition = Project::BitConditions::ON;
		res15.Text.Texts.push_back("Alarm15");
		res15.ReadRegVar = prjHelper.GetVarID("win1.var43");
		cfg->InfoLst.push_back(res15);

		Project::AlarmInfoRes res16;
		res16.GroupName = 3;
		res16.GroupNo = 3;
		res16.RegType = 0;			//位寄存器
		res16.BtyeCondition = Project::Conditions::LT;
		res16.ComPareValue = 10;
		res16.BitCondition = Project::BitConditions::ON;
		res16.Text.Texts.push_back("Alarm16");
		res16.ReadRegVar = prjHelper.GetVarID("win1.var45");
		cfg->InfoLst.push_back(res16);

		Project::AlarmInfoRes res17;
		res17.GroupName = 3;
		res17.GroupNo = 4;
		res17.RegType = 0;			//位寄存器
		res17.BtyeCondition = Project::Conditions::LT;
		res17.ComPareValue = 10;
		res17.BitCondition = Project::BitConditions::ON;
		res17.Text.Texts.push_back("Alarm17");
		res17.ReadRegVar = prjHelper.GetVarID("win1.var47");
		cfg->InfoLst.push_back(res17);

		Project::AlarmInfoRes res18;
		res18.GroupName = 4;
		res18.GroupNo = 0;
		res18.RegType = 0;			//位寄存器
		res18.BtyeCondition = Project::Conditions::LT;
		res18.ComPareValue = 10;
		res18.BitCondition = Project::BitConditions::ON;
		res18.Text.Texts.push_back("Alarm18");
		res18.ReadRegVar = prjHelper.GetVarID("win1.var49");
		cfg->InfoLst.push_back(res18);

		Project::AlarmInfoRes res19;
		res19.GroupName = 4;
		res19.GroupNo = 1;
		res19.RegType = 0;			//位寄存器
		res19.BtyeCondition = Project::Conditions::LT;
		res19.ComPareValue = 10;
		res19.BitCondition = Project::BitConditions::ON;
		res19.Text.Texts.push_back("Alarm19");
		res19.ReadRegVar = prjHelper.GetVarID("win1.var51");
		cfg->InfoLst.push_back(res19);

		Project::AlarmInfoRes res20;
		res20.GroupName = 4;
		res20.GroupNo = 2;
		res20.RegType = 0;			//位寄存器
		res20.BtyeCondition = Project::Conditions::LT;
		res20.ComPareValue = 10;
		res20.BitCondition = Project::BitConditions::ON;
		res20.Text.Texts.push_back("Alarm20");
		res20.ReadRegVar = prjHelper.GetVarID("win1.var53");
		cfg->InfoLst.push_back(res20);

		Project::AlarmInfoRes res21;
		res21.GroupName = 4;
		res21.GroupNo = 3;
		res21.RegType = 0;			//位寄存器
		res21.BtyeCondition = Project::Conditions::LT;
		res21.ComPareValue = 10;
		res21.BitCondition = Project::BitConditions::ON;
		res21.Text.Texts.push_back("Alarm21");
		res21.ReadRegVar = prjHelper.GetVarID("win1.var55");
		cfg->InfoLst.push_back(res21);

		Project::AlarmInfoRes res22;
		res22.GroupName = 4;
		res22.GroupNo = 4;
		res22.RegType = 0;			//位寄存器
		res22.BtyeCondition = Project::Conditions::LT;
		res22.ComPareValue = 10;
		res22.BitCondition = Project::BitConditions::ON;
		res22.Text.Texts.push_back("Alarm22");
		res22.ReadRegVar = prjHelper.GetVarID("win1.var57");
		cfg->InfoLst.push_back(res22);

		Project::AlarmInfoRes res23;
		res23.GroupName = 5;
		res23.GroupNo = 0;
		res23.RegType = 0;			//位寄存器
		res23.BtyeCondition = Project::Conditions::LT;
		res23.ComPareValue = 10;
		res23.BitCondition = Project::BitConditions::ON;
		res23.Text.Texts.push_back("Alarm23");
		res23.ReadRegVar = prjHelper.GetVarID("win1.var59");
		cfg->InfoLst.push_back(res23);

		Project::AlarmInfoRes res24;
		res24.GroupName = 5;
		res24.GroupNo = 1;
		res24.RegType = 0;			//位寄存器
		res24.BtyeCondition = Project::Conditions::LT;
		res24.ComPareValue = 10;
		res24.BitCondition = Project::BitConditions::ON;
		res24.Text.Texts.push_back("Alarm24");
		res24.ReadRegVar = prjHelper.GetVarID("win1.var61");
		cfg->InfoLst.push_back(res24);

		Project::AlarmInfoRes res25;
		res25.GroupName = 5;
		res25.GroupNo = 2;
		res25.RegType = 0;			//位寄存器
		res25.BtyeCondition = Project::Conditions::LT;
		res25.ComPareValue = 10;
		res25.BitCondition = Project::BitConditions::ON;
		res25.Text.Texts.push_back("Alarm25");
		res25.ReadRegVar = prjHelper.GetVarID("win1.var63");
		cfg->InfoLst.push_back(res25);

		Project::AlarmInfoRes res26;
		res26.GroupName = 5;
		res26.GroupNo = 3;
		res26.RegType = 0;			//位寄存器
		res26.BtyeCondition = Project::Conditions::LT;
		res26.ComPareValue = 10;
		res26.BitCondition = Project::BitConditions::ON;
		res26.Text.Texts.push_back("Alarm26");
		res26.ReadRegVar = prjHelper.GetVarID("win1.var65");
		cfg->InfoLst.push_back(res26);

		Project::AlarmInfoRes res27;
		res27.GroupName = 5;
		res27.GroupNo = 4;
		res27.RegType = 0;			//位寄存器
		res27.BtyeCondition = Project::Conditions::LT;
		res27.ComPareValue = 10;
		res27.BitCondition = Project::BitConditions::ON;
		res27.Text.Texts.push_back("Alarm27");
		res27.ReadRegVar = prjHelper.GetVarID("win1.var67");
		cfg->InfoLst.push_back(res27);

		Project::AlarmInfoRes res28;
		res28.GroupName = 6;
		res28.GroupNo = 0;
		res28.RegType = 0;			//位寄存器
		res28.BtyeCondition = Project::Conditions::LT;
		res28.ComPareValue = 10;
		res28.BitCondition = Project::BitConditions::ON;
		res28.Text.Texts.push_back("Alarm28");
		res28.ReadRegVar = prjHelper.GetVarID("win1.var69");
		cfg->InfoLst.push_back(res28);

		Project::AlarmInfoRes res29;
		res29.GroupName = 6;
		res29.GroupNo = 1;
		res29.RegType = 0;			//位寄存器
		res29.BtyeCondition = Project::Conditions::LT;
		res29.ComPareValue = 10;
		res29.BitCondition = Project::BitConditions::ON;
		res29.Text.Texts.push_back("Alarm29");
		res29.ReadRegVar = prjHelper.GetVarID("win1.var71");
		cfg->InfoLst.push_back(res29);

		Project::AlarmInfoRes res30;
		res30.GroupName = 6;
		res30.GroupNo = 2;
		res30.RegType = 0;			//位寄存器
		res30.BtyeCondition = Project::Conditions::LT;
		res30.ComPareValue = 10;
		res30.BitCondition = Project::BitConditions::ON;
		res30.Text.Texts.push_back("Alarm30");
		res30.ReadRegVar = prjHelper.GetVarID("win1.var73");
		cfg->InfoLst.push_back(res30);
		});

	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var19", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var21", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var23", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var25", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var27", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var29", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var31", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var33", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var35", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var37", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var39", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var41", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var43", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var45", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var47", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var49", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var51", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var53", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var55", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var57", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var59", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var61", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var63", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var65", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var67", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var69", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var71", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var73", "win1.tc6");

	prjHelper.AddCtrl(1, "AlarmDisplayControl", "win1.tc1", [&prjHelper](void* param) {
		Project::AlarmDisplayConfig* cfg = (Project::AlarmDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 200;
		cfg->Y = 250;
		cfg->Width = 496;
		cfg->Height = 300;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->AlarmVarId.Vid = prjHelper.GetVarID("win1.var10");	//通知采样数据变化寄存器
		cfg->StartGroupName = 0;		//采样组
		cfg->EndGroupName = 5;			//采样编号
		cfg->UseTitle = true;			//是否使用标题
		cfg->UseSameStyle = false;		//标题列表是否使用同样式

		cfg->Title.Texts.push_back("报警显示");			//标题文字

		cfg->TitleStringStyle.Align = 0;				//标题字体
		cfg->TitleStringStyle.Font.Name = "Arial";
		cfg->TitleStringStyle.Font.Size = 12;
		cfg->TitleStringStyle.Colors = 39423;

		cfg->ListTitleStyle.Align = 0;					//列标题字体
		cfg->ListTitleStyle.Font.Name = "Arial";
		cfg->ListTitleStyle.Font.Size = 12;
		cfg->ListTitleStyle.Colors = 16776960;

		cfg->ListStringStyle.Align = 0;					//列表字体
		cfg->ListStringStyle.Font.Name = "Arial";	
		cfg->ListStringStyle.Font.Size = 12;
		cfg->ListStringStyle.Colors = 6697881;

		cfg->ListStringStyle.Align = 0;					//相同字体
		cfg->ListStringStyle.Font.Name = "Arial";
		cfg->ListStringStyle.Font.Size = 12;
		cfg->ListStringStyle.Colors = 39423;

		cfg->AlarmMode = Project::AlarmDisMode::HISTORY;		//报警模式	0：实时	1：历史 2：查询
		cfg->AlarmMaxNum = 30;
		cfg->PageNum = 10;
		cfg->IsUseMultiLanguage = false;

		/*设置显示信息项目*/
		Project::ColInfo tmpcolinfo1;
		tmpcolinfo1.ProjectName = "序号";
		tmpcolinfo1.DescribeName = "序号";
		tmpcolinfo1.ColWidth = 80;
		tmpcolinfo1.RowHeight = 30;
		tmpcolinfo1.TitleDescribe.Texts.push_back("序号");
		cfg->AlarmOptions.push_back(tmpcolinfo1);

		Project::ColInfo tmpcolinfo2;
		tmpcolinfo2.ProjectName = "触发日期";
		tmpcolinfo2.DescribeName = "触发日期";
		tmpcolinfo2.ColWidth = 80;
		tmpcolinfo2.RowHeight = 30;
		tmpcolinfo2.TitleDescribe.Texts.push_back("触发日期");
		cfg->AlarmOptions.push_back(tmpcolinfo2);

		Project::ColInfo tmpcolinfo3;
		tmpcolinfo3.ProjectName = "触发时间";
		tmpcolinfo3.DescribeName = "触发时间";
		tmpcolinfo3.ColWidth = 80;
		tmpcolinfo3.RowHeight = 30;
		tmpcolinfo3.TitleDescribe.Texts.push_back("触发时间");
		cfg->AlarmOptions.push_back(tmpcolinfo3);

		Project::ColInfo tmpcolinfo4;
		tmpcolinfo4.ProjectName = "报警信息";
		tmpcolinfo4.DescribeName = "报警信息";
		tmpcolinfo4.ColWidth = 80;
		tmpcolinfo4.RowHeight = 30;
		tmpcolinfo4.TitleDescribe.Texts.push_back("报警信息");
		cfg->AlarmOptions.push_back(tmpcolinfo4);

		Project::ColInfo tmpcolinfo5;
		tmpcolinfo5.ProjectName = "报警次数";
		tmpcolinfo5.DescribeName = "报警次数";
		tmpcolinfo5.ColWidth = 80;
		tmpcolinfo5.RowHeight = 30;
		tmpcolinfo5.TitleDescribe.Texts.push_back("报警次数");
		cfg->AlarmOptions.push_back(tmpcolinfo5);

		Project::ColInfo tmpcolinfo6;
		tmpcolinfo6.ProjectName = "确认时间";
		tmpcolinfo6.DescribeName = "确认时间";
		tmpcolinfo6.ColWidth = 80;
		tmpcolinfo6.RowHeight = 30;
		tmpcolinfo6.TitleDescribe.Texts.push_back("确认时间");
		cfg->AlarmOptions.push_back(tmpcolinfo6);

		cfg->TitleHeight = 20;							//标题高度
		cfg->EachColTitleHeight = 15;					//列标题高度
		cfg->SortMode = 0;								//排序方式	0：时间顺序 1：时间逆序
		cfg->Timemode = Project::TimeMode::HMS;			//时间格式
		cfg->Datemode = Project::DateMode::YMDChinese;	//日期格式
		cfg->TopNotRecover = false;						//是 否置顶未恢复信息 0：不置顶 1：置顶
		cfg->UseConfirmMode = true;						//是否启用确认	0：不启用	1：启用
		cfg->CMode = Project::ConfirmMode::Single;		//确认模式
		cfg->IsInfoHideCtrl = false;							//是否控制隐藏
		//cfg->HideVar.Vid = prjHelper.GetVarID("win1.var13")	//控制隐藏信息的寄存器

		cfg->BackColor = 65535;
		cfg->TitleBgColor = 16751052;
		cfg->UseFrame = true;
		cfg->FrameStyle.Weight = 4;
		cfg->FrameStyle.Type = 0;
		cfg->FrameStyle.Color = 0;

		cfg->UseGrid = true;
		cfg->GridStyle.Weight = 1;
		cfg->GridStyle.Type = 1;
		cfg->GridStyle.Color = 0;

		cfg->AlarmTextColor = 255;				//报警触发时文字颜色
		cfg->AlarmBackColor = 52377;			//报警触发时背景颜色
		cfg->UseConfirm = true;					//是否使用报警确认
		cfg->ConfirmTextColor = 10092543;				//报警确认时文字颜色
		cfg->ConfirmBackColor = 16711935;				//报警确认时背景颜色
		cfg->UseRecover = true;						//是否使用报警恢复
		cfg->RecoverTextColor = 0;				//报警恢复时文字颜色
		cfg->RecoverBackColor = 16777215;				//报警恢复时背景颜色
		/*查询设置*/
		cfg->EnableSearch = true;
		cfg->SearchMode = Project::AlarmSeekMode::AlarmSeekTIME;
		//cfg->SeekModeVar.Vid = prjHelper.GetVarID("win1.var1");
		cfg->SeekCtrlVar.Vid = prjHelper.GetVarID("win1.var11");
		cfg->SearchDate = 0;
		//Project::DataVarId tmpvid;
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);

		cfg->SearchTimeStart = 1611648553408;
		cfg->SearchTimeEnd = 1611648573408;
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchTimeVar.push_back(tmpvid);

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var5");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var6");
		cfg->Perm.EnableLogic = true;
		});
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc1");

	/*OFFX*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc2", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::WordVar;
		cfg->X = 50;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "offx";
		cfg->Value = 0;
		cfg->IncreaseValue = 10;
		cfg->VarId = prjHelper.GetVarID("win1.var3");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc2");

	/*OFFY*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc3", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::WordVar;
		cfg->X = 100;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "offy";
		cfg->Value = 0;
		cfg->IncreaseValue = 10;
		cfg->VarId = prjHelper.GetVarID("win1.var4");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc3");
	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var7");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc4");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var8");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc5");

	/*隐藏控制*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "hide";
		cfg->Max = 7;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc7");
	prjHelper.Update();

	/*查询控制*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "search";
		cfg->VarId = prjHelper.GetVarID("win1.var12");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc8");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc9", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 300;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm1";
		cfg->VarId = prjHelper.GetVarID("win1.var16");
		cfg->CtrlName = "win1.tc9";
		});
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc9");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc10", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 350;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm2";
		cfg->VarId = prjHelper.GetVarID("win1.var18");
		cfg->CtrlName = "win1.tc10";
		});
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc10");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc11", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 400;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm3";
		cfg->VarId = prjHelper.GetVarID("win1.var20");
		cfg->CtrlName = "win1.tc11";
		});
	prjHelper.BindCtrlAndVar("win1.var20", "win1.tc11");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc12", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 450;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		
		cfg->Name = "Alarm4";
		cfg->VarId = prjHelper.GetVarID("win1.var22");
		cfg->CtrlName = "win1.tc12";
		});
	prjHelper.BindCtrlAndVar("win1.var22", "win1.tc12");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc13", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 500;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm5";
		cfg->VarId = prjHelper.GetVarID("win1.var24");
		cfg->CtrlName = "win1.tc13";
		});
	prjHelper.BindCtrlAndVar("win1.var24", "win1.tc13");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc14", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 550;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm6";
		cfg->VarId = prjHelper.GetVarID("win1.var26");
		cfg->CtrlName = "win1.tc14";
		});
	prjHelper.BindCtrlAndVar("win1.var26", "win1.tc14");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc15", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 600;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm7";
		cfg->VarId = prjHelper.GetVarID("win1.var28");
		cfg->CtrlName = "win1.tc15";
		});
	prjHelper.BindCtrlAndVar("win1.var28", "win1.tc15");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc16", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 650;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm8";
		cfg->VarId = prjHelper.GetVarID("win1.var30");
		cfg->CtrlName = "win1.tc16";
		});
	prjHelper.BindCtrlAndVar("win1.var30", "win1.tc16");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc17", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 700;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm9";
		cfg->VarId = prjHelper.GetVarID("win1.var32");
		cfg->CtrlName = "win1.tc17";
		});
	prjHelper.BindCtrlAndVar("win1.var32", "win1.tc17");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc18", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 750;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm10";
		cfg->VarId = prjHelper.GetVarID("win1.var34");
		cfg->CtrlName = "win1.tc18";
		});
	prjHelper.BindCtrlAndVar("win1.var34", "win1.tc18");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc19", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 300;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm11";
		cfg->VarId = prjHelper.GetVarID("win1.var36");
		cfg->CtrlName = "win1.tc19";
		});
	prjHelper.BindCtrlAndVar("win1.var36", "win1.tc19");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc20", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 350;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		cfg->Name = "A1arm12";
		cfg->VarId = prjHelper.GetVarID("win1.var38");
		cfg->CtrlName = "win1.tc20";
		});
	prjHelper.BindCtrlAndVar("win1.var38", "win1.tc20");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc21", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 400;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm13";
		cfg->VarId = prjHelper.GetVarID("win1.var40");
		cfg->CtrlName = "win1.tc21";
		});
	prjHelper.BindCtrlAndVar("win1.var40", "win1.tc21");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc22", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 450;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm14";
		cfg->VarId = prjHelper.GetVarID("win1.var42");
		cfg->CtrlName = "win1.tc22";
		});
	prjHelper.BindCtrlAndVar("win1.var42", "win1.tc22");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc23", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 500;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm15";
		cfg->VarId = prjHelper.GetVarID("win1.var44");
		cfg->CtrlName = "win1.tc23";
		});
	prjHelper.BindCtrlAndVar("win1.var44", "win1.tc23");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc24", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 550;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm16";
		cfg->VarId = prjHelper.GetVarID("win1.var46");
		cfg->CtrlName = "win1.tc24";
		});
	prjHelper.BindCtrlAndVar("win1.var46", "win1.tc24");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc25", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 600;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm17";
		cfg->VarId = prjHelper.GetVarID("win1.var48");
		cfg->CtrlName = "win1.tc25";
		});
	prjHelper.BindCtrlAndVar("win1.var48", "win1.tc25");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc26", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 650;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm18";
		cfg->VarId = prjHelper.GetVarID("win1.var50");
		cfg->CtrlName = "win1.tc26";
		});
	prjHelper.BindCtrlAndVar("win1.var50", "win1.tc26");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc27", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 700;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm19";
		cfg->VarId = prjHelper.GetVarID("win1.var52");
		cfg->CtrlName = "win1.tc27";
		});
	prjHelper.BindCtrlAndVar("win1.var52", "win1.tc27");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc28", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 750;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm20";
		cfg->VarId = prjHelper.GetVarID("win1.var54");
		cfg->CtrlName = "win1.tc28";
		});
	prjHelper.BindCtrlAndVar("win1.var54", "win1.tc28");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc29", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 300;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm21";
		cfg->VarId = prjHelper.GetVarID("win1.var56");
		cfg->CtrlName = "win1.tc29";
		});
	prjHelper.BindCtrlAndVar("win1.var56", "win1.tc29");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc30", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 350;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "A1arm22";
		cfg->VarId = prjHelper.GetVarID("win1.var58");
		cfg->CtrlName = "win1.tc30";
		});
	prjHelper.BindCtrlAndVar("win1.var58", "win1.tc30");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc31", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 400;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm23";
		cfg->VarId = prjHelper.GetVarID("win1.var60");
		cfg->CtrlName = "win1.tc31";
		});
	prjHelper.BindCtrlAndVar("win1.var60", "win1.tc31");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc32", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 450;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm24";
		cfg->VarId = prjHelper.GetVarID("win1.var62");
		cfg->CtrlName = "win1.tc32";
		});
	prjHelper.BindCtrlAndVar("win1.var62", "win1.tc32");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc33", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 500;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm25";
		cfg->VarId = prjHelper.GetVarID("win1.var64");
		cfg->CtrlName = "win1.tc33";
		});
	prjHelper.BindCtrlAndVar("win1.var64", "win1.tc33");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc34", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 550;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm26";
		cfg->VarId = prjHelper.GetVarID("win1.var66");
		cfg->CtrlName = "win1.tc34";
		});
	prjHelper.BindCtrlAndVar("win1.var66", "win1.tc34");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc35", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 600;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm27";
		cfg->VarId = prjHelper.GetVarID("win1.var68");
		cfg->CtrlName = "win1.tc35";
		});
	prjHelper.BindCtrlAndVar("win1.var68", "win1.tc35");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc36", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 650;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm28";
		cfg->VarId = prjHelper.GetVarID("win1.var70");
		cfg->CtrlName = "win1.tc36";
		});
	prjHelper.BindCtrlAndVar("win1.var70", "win1.tc36");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc37", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 700;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm29";
		cfg->VarId = prjHelper.GetVarID("win1.var72");
		cfg->CtrlName = "win1.tc37";
		});
	prjHelper.BindCtrlAndVar("win1.var72", "win1.tc37");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc38", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 750;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Alarm30";
		cfg->VarId = prjHelper.GetVarID("win1.var74");
		cfg->CtrlName = "win1.tc38";
		});
	prjHelper.BindCtrlAndVar("win1.var74", "win1.tc38");


	prjHelper.Update();
}

bool AlarmDisplayTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
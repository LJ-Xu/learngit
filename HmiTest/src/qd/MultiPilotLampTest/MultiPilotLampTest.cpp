/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampTest.cpp
 * Author   : qiaodan
 * Date     : 2020-01-16
 * Descript : 用于测试多状态指示灯部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "MultiPilotLampTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "MultiPilotLampConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
static Project::HMIProject prj;

void MultiPilotLampTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*用于指示灯的寄存器*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");	//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");	//offy
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");	//bit0
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");	//bit1
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");	//bit2
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 5, 4, "win1.var6");	//value
	/*用于显示设置指示灯状态的寄存器*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var10");	//bit1
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var11");	//bit2
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 5, 4, "win1.var12");	//value


	/*用于控制权限*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var14");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var16");


	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/MultiStatusKeyTest/button_a.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/MultiStatusKeyTest/button_b.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/MultiStatusKeyTest/button_c.bmp", buf));
	prjHelper.AddRes("pic4", System::GetFilePath("/../HmiTest/Res/Qd/MultiStatusKeyTest/button_d.bmp", buf));

	prjHelper.AddCtrl(1, "MultiPilotLampControl", "win1.tc1", [&prjHelper](void* param) {
		Project::MultiPilotLampConfig* cfg = (Project::MultiPilotLampConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 200;
		cfg->Width = 60;
		cfg->Height = 60;

		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->DataVarType = 1;		//0:字寄存器  1:位寄存器
		cfg->StatusCnt = 3;

		
		Project::DataVarId tmpvid;
		tmpvid.Vid = prjHelper.GetVarID("win1.var3");
		cfg->RegVars.push_back(tmpvid);
		tmpvid.Vid = prjHelper.GetVarID("win1.var4");
		cfg->RegVars.push_back(tmpvid);
		tmpvid.Vid = prjHelper.GetVarID("win1.var5");
		cfg->RegVars.push_back(tmpvid);
		tmpvid.Vid = prjHelper.GetVarID("win1.var6");
		cfg->RegVars.push_back(tmpvid);

		cfg->BitNum = 3;			//多位组合位数
		cfg->DataFormat = 0;		//0:有符号 1:无符号

		/*状态0*/
		Project::StatusCondition action;
		action.Type = 0;			//0:范围	1:位
		action.ConditionA = Project::Conditions::GT;
		action.ConditionB = Project::Conditions::LT;
		action.LogicCondition = Project::Logic::AND;
		action.DataA.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataA.Value = -4;
		//action.DataA.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.DataB.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataB.Value = -2;
		//action.DataB.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.Offset = 0;			//偏移量
		action.Trigger = 1;			//0:ON  1:OFF	(0-15)
		action.Flash = true;		//是否闪烁
		action.Speed = 300;			//闪烁速度
		action.StatusKey.KeyVal = "pic1";
		action.Css.Align = 0;
		action.Css.Font.Name = "Arial";
		action.Css.Font.Size = 12;
		action.Css.Colors = 0xff0000ff;
		cfg->Action.push_back(action);

		/*状态1*/
		action.Type = 0;			//0:范围	1:位
		action.ConditionA = Project::Conditions::GT;
		action.ConditionB = Project::Conditions::LT;
		action.LogicCondition = Project::Logic::AND;
		action.DataA.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataA.Value = -2;
		//action.DataA.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.DataB.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataB.Value = 0;
		//action.DataB.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.Offset = 0;			//偏移量
		action.Trigger = 0;			//0:ON  1:OFF	(0-15)
		action.Flash = false;		//是否闪烁
		action.Speed = 800;			//闪烁速度
		action.StatusKey.KeyVal = "pic2";
		action.Css.Align = 0;
		action.Css.Font.Name = "Arial";
		action.Css.Font.Size = 12;
		action.Css.Colors = 0xff00ff00;
		cfg->Action.push_back(action);

		/*状态2*/
		action.Type = 0;			//0:范围	1:位
		action.ConditionA = Project::Conditions::GT;
		action.ConditionB = Project::Conditions::LT;
		action.LogicCondition = Project::Logic::AND;
		action.DataA.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataA.Value = 0;
		//action.DataA.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.DataB.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		action.DataB.Value = 2;
		//action.DataB.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		action.Offset = 0;			//偏移量
		action.Trigger = 0;			//0:ON  1:OFF	(0-15)
		action.Flash = false;		//是否闪烁
		action.Speed = 1000;			//闪烁速度
		action.StatusKey.KeyVal = "pic3";
		action.Css.Align = 0;
		action.Css.Font.Name = "Arial";
		action.Css.Font.Size = 12;
		action.Css.Colors = 0xffff0000;
		cfg->Action.push_back(action);

		/*错误状态*/
		cfg->ErrState.Type = 0;			//0:范围	1:位
		cfg->ErrState.ConditionA = Project::Conditions::GT;
		cfg->ErrState.ConditionB = Project::Conditions::LT;
		cfg->ErrState.LogicCondition = Project::Logic::AND;
		cfg->ErrState.DataA.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		cfg->ErrState.DataA.Value = 2;
		//cfg->ErrState.DataA.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid
		cfg->ErrState.DataB.Mode = 0;		//获取方式	0:常数  1:寄存器获取
		cfg->ErrState.DataB.Value = 3;
		//cfg->ErrState.DataB.RegVar.Vid = prjHelper.GetVarID("win1.var6");		//对比寄存器Vid

		cfg->ErrState.Offset = 0;			//偏移量
		cfg->ErrState.Trigger = 0;			//0:ON  1:OFF	(0-15)
		cfg->ErrState.Flash = true;			//是否闪烁
		cfg->ErrState.Speed = 500;			//闪烁速度
		cfg->ErrState.StatusKey.KeyVal = "pic4";
		cfg->ErrState.Css.Align = 0;
		cfg->ErrState.Css.Font.Name = "Arial";
		cfg->ErrState.Css.Font.Size = 12;
		cfg->ErrState.Css.Colors = 0;

		cfg->Text.Texts.push_back("状态0");				//设置文字
		cfg->Text.Texts.push_back("状态1");
		cfg->Text.Texts.push_back("状态2");
		cfg->Text.Texts.push_back("错误");

		cfg->InputErr = 0;				//非法输入处理方式 0:显示错误状态 1:显示当前状态 
		cfg->UseErrNotice = false;		//是否使用错误通知
		//cfg->InputErrVarRef.Vid = prjHelper.GetVarID("win1.var6");		//错误通知寄存器

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var13");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var14");
		cfg->Perm.EnableLogic = true;
		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc1");

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
		cfg->VarId = prjHelper.GetVarID("win1.var7");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var8");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc3");


	/*Bit0  位0*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Bit0";
		cfg->VarId = prjHelper.GetVarID("win1.var9");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc4");

	/*Bit1  位1*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Bit1";
		cfg->VarId = prjHelper.GetVarID("win1.var10");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc5");

	/*Bit2  位2*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 150;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Bit2";
		cfg->VarId = prjHelper.GetVarID("win1.var11");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc6");

	/*字*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 200;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 30;
		cfg->Min = 5;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var12");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc7");

	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var15");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc8");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc9", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var16");
		cfg->CtrlName = "win1.tc9";
		});
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc9");
	prjHelper.Update();
}

bool MultiPilotLampTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
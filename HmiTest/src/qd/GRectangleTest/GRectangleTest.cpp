/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-05
 * Descript : 用于测试圆角矩形部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GRectangleTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "GRectangleConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"

static Project::HMIProject prj;
void GRectangleTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var4");

	/*权限设置*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var8");

	prjHelper.AddCtrl(1, "GRectangleControl", "win1.tc1", [&prjHelper](void* param) {
		Project::GRectangleConfig* cfg = (Project::GRectangleConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 282;
		cfg->Y = 211;
		cfg->Width = 100;
		cfg->Height = 100;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->IsFill = true;					//是否填充
		cfg->FillColor = 0xffff0000;		//填充颜色
		cfg->FillStyle = 0;					//填充样式

		cfg->Line.Color = 255;				//线段颜色
		cfg->Line.Weight = 5;				//线段粗细
		cfg->Line.Type = 0;					//线段样式

		cfg->Radius = 0;
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


	prjHelper.Update();
}

bool GRectangleTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
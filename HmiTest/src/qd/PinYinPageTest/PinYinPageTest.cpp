/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 用于测试拼音界面部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "PinYinPageTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "ChineseDisplayConfig.h"
#include "PinYinPageConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"

static Project::HMIProject prj;
void PinYinPageTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");			//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");			//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 84, 16, "win1.var3");		//read

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 84, 16, "win1.var6");		//read

	/*用于控制权限*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 40, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 41, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 40, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 41, 2, "win1.var10");
	
	/*模拟键盘实时显示数据*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 50, 16, "win1.var11");	//InputKeyCurValue
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 50, 16, "win1.var12");	//InputKeyCurValue

	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DigitalDisplayTest/1.bmp", buf));

	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc1", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 250;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var11");


		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;

		cfg->Text = "中文";
		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var7");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var8");
		cfg->Perm.EnableLogic = true;

		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc1");

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
		cfg->VarId = prjHelper.GetVarID("win1.var4");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var5");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc3");

	/*ReadVar 设置读寄存器数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetString;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 5;
		cfg->Min = 1;
		cfg->VarId = prjHelper.GetVarID("win1.var6");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc4");


	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var9");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc5");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var10");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc6");

	prjHelper.AddCtrl(1, "PinYinPageControl", "win1.tc7", [&prjHelper](void* param) {
		Project::PinYinPageConfig* cfg = (Project::PinYinPageConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 300;
		cfg->Y = 150;
		cfg->Width = 150;
		cfg->Height = 80;
		//cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc7";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var3");
		cfg->WriteVar.Vid = prjHelper.GetVarID("win1.var12");


		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 14;

		/*显示权限*/
		//cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var7");
		//cfg->Perm.ShowLogic = true;
		//cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		//cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var8");
		//cfg->Perm.EnableLogic = true;

		});
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc7");
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc7");

	prjHelper.Update();
}

bool PinYinPageTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
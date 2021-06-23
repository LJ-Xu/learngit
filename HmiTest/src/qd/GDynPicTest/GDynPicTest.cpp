/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicTest.cpp
 * Author   : qiaodan
 * Date     : 2020-12-24
 * Descript : 用于测试动态图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GDynPicTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "GDynPicConfig.h"
#include "ImageFormat.h"
#include "TestBitVarConfig.h"
#include "System.h"

static Project::HMIProject prj;
void GDynPicTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);
	char buf[255];
	//prjHelper.AddRes("pic1", "\\..\\HmiTest\\Res\\1.bmp");
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DynTest/1.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/DynTest/2.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/DynTest/3.bmp", buf));
	prjHelper.AddRes("pic4", System::GetFilePath("/../HmiTest/Res/Qd/DynTest/4.bmp", buf));
	
	//prjHelper.AddStrings("switch", 0, "OFF");
	//prjHelper.AddStrings("switch", 1, "ON");
	//prjHelper.AddStrings("timerLB", 0, "timer bt");
	//prjHelper.AddStrings("opendialogLB", 0, "open dlg bt");
	//prjHelper.AddStrings("closedialogLB", 0, "cls dlg bt");
	//prjHelper.AddStrings("str2", 0, "str2.1");
	//prjHelper.AddStrings("str2", 1, "str2.2");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2,	"win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");
	
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var10");
	/*权限设置*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var14");

	prjHelper.AddCtrl(1, "GDynPicControl", "win1.tc1", [&prjHelper](void* param) {
		Project::GDynPicConfig* cfg = (Project::GDynPicConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 200;
		cfg->Y = 180;
		cfg->Width = 200;
		cfg->Height = 200;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";
		cfg->CycleTime = 0;

		cfg->CycleTimeVarId = prjHelper.GetVarID("win1.var3");
		cfg->SwitchOrder = "1,2,4,3";
		cfg->SwitchMode = Project::PicSwitchMode::Mode_GondBack;	//图片切换模式
		cfg->SwitchSeq = Project::PicSwitchSeq::Seq_Random;			//图片切换模式
		cfg->PicNum = 4;
		//设置图片信息
		Project::ImageFormat imagelst1;
		imagelst1.KeyVal = "pic1";
		imagelst1.UseRotate = true;				//是否旋转
		imagelst1.RotateAngle = 30;					//旋转角度
		cfg->ImageFmtList.push_back(imagelst1);

		Project::ImageFormat imagelst2;
		imagelst2.KeyVal = "pic2";
		imagelst2.UseRotate = true;
		imagelst2.RotateAngle = 40;
		cfg->ImageFmtList.push_back(imagelst2);

		Project::ImageFormat imagelst3;
		imagelst3.KeyVal = "pic3";
		imagelst3.UseRotate = true;
		imagelst3.RotateAngle = 50;
		cfg->ImageFmtList.push_back(imagelst3);

		Project::ImageFormat imagelst4;
		imagelst4.KeyVal = "pic4";
		imagelst4.UseRotate = true;
		imagelst4.RotateAngle = 60;
		cfg->ImageFmtList.push_back(imagelst4);
		
		cfg->UseStartSign = true;
		cfg->StartSignVarId = prjHelper.GetVarID("win1.var4"); 
		cfg->UseStopSign = true ;
		cfg->StopSignVarId = prjHelper.GetVarID("win1.var5");

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var11");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var12");
		cfg->Perm.EnableLogic = true;
		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");

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
		cfg->VarId = prjHelper.GetVarID("win1.var6");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var7");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc3");

	/*循环周期*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::WordVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Cyc";
		cfg->Value = 4000;
		cfg->IncreaseValue = 1000;
		cfg->VarId = prjHelper.GetVarID("win1.var8");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc4");

	/*开始信号*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "start";
		cfg->VarId = prjHelper.GetVarID("win1.var9");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc5");

	/*结束信号*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 150;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "stop";
		cfg->VarId = prjHelper.GetVarID("win1.var10");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc6");

	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var13");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc7");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc8");

	prjHelper.Update();
}

bool GDynPicTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
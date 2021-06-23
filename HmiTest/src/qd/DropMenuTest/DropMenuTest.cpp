/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuTest.cpp
 * Author   : qiaodan
 * Date     : 2020-12-26
 * Descript : 用于测试下拉菜单部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "DropMenuTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "DropMenuConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"

static Project::HMIProject prj;
void DropMenuTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var10");

	/*用于控制权限*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var14");
	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DropTest/1.bmp", buf));

	prjHelper.AddCtrl(1, "DropMenuControl", "win1.tc1", [&prjHelper](void* param) {
		Project::DropMenuConfig* cfg = (Project::DropMenuConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 250;
		cfg->Width = 200;
		cfg->Height = 80;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->MenuMode = 0;									//0:下拉菜单 1:清单选框
		cfg->ReadVar = prjHelper.GetVarID("win1.var3");		//读取写入地址
		cfg->MenuNum = 4;									//下拉条目数量
		cfg->PopMode = 1;									//弹出方式	0：向下 1：向上
		for (unsigned i = 0; i < cfg->MenuNum; i++)			//设置条目信息
		{
			Project::MenuContontLst menuInfo;
			menuInfo.Content.Texts.push_back(to_string(i));
			menuInfo.Index = i;
			menuInfo.Value = i;
			cfg->MenuInfo.push_back(menuInfo);
		}
		//cfg->PicKey.KeyVal = "pic1";		//设置背景图片
		cfg->RowHeight = 20;				//设置行高（自动根据字体大小调节）
		cfg->HscollWidth = 16;				//设置滚动条宽度
		cfg->SelectColor = 0xff0000ff;		//设置被选中条目颜色
		cfg->BgColor = 0xffff0000;			//设置背景颜色
		cfg->FrameColor = 0xff0000ff;		//设置边框颜色
		
		Project::StringStyle menustyle1;	//设置条目格式
		menustyle1.Colors = 0xff000000;		//字体颜色
		menustyle1.Font.Name = "Arial";		//字体名称
		menustyle1.Font.Size = 24;			//字体大小
		menustyle1.Align = 0;				//对齐方式
		cfg->MenuStyle.push_back(menustyle1);

		Project::StringStyle menustyle2;
		menustyle2.Colors = 0xff000000;
		menustyle2.Font.Name = "Arial";
		menustyle2.Font.Size = 12;
		menustyle2.Align = 0;
		cfg->MenuStyle.push_back(menustyle2);

		Project::StringStyle menustyle3;
		menustyle3.Colors = 0xff000000;
		menustyle3.Font.Name = "Arial";
		menustyle3.Font.Size = 12;
		menustyle3.Align = 0;
		cfg->MenuStyle.push_back(menustyle3);

		Project::StringStyle menustyle4;
		menustyle4.Colors = 0xff000000;
		menustyle4.Font.Name = "Arial";
		menustyle4.Font.Size = 12;
		menustyle4.Align = 0;
		cfg->MenuStyle.push_back(menustyle4);

		/*设置错误通知*/
		cfg->ErrorNotice.Bit.DataBitVar = prjHelper.GetVarID("win1.var4");
		cfg->ErrorNotice.Bit.State = Project::NotifyBitMode::SetOFF;

		cfg->ErrorNotice.Word.DataWordVar = prjHelper.GetVarID("win1.var5");
		cfg->ErrorNotice.Word.Val = 8;

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var11");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var12");
		cfg->Perm.EnableLogic = true;

		});


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

	/*ReadVar 设置读写寄存器数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 150;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 5;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var8");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc4");

	/*ErrorNotice-Bit  错误位通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateBitVar;
		cfg->X = 200;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var9");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc5");
	/*ErrorNotice-Word 错误字通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateWordVar;
		cfg->X = 250;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var10");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc6");

	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

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
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc8");

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");
	prjHelper.Update();
}

bool DropMenuTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
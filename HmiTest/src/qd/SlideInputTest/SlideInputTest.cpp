/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputTest.cpp
 * Author   : qiaodan
 * Date     : 2020-12-28
 * Descript : 用于测试滑动输入部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "SlideInputTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "SlideInputConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "ScaleInfo.h"
static Project::HMIProject prj;

void SlideInputTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*用于滑动输入的寄存器*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 4, 4, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 6, 4, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var9");


	/*用于更改或显示滑动寄存器状态的*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var10");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 4, 4, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 6, 4, "win1.var14");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var16");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var17");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var18");

	/*用于控制权限*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var19");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var20");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var21");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var22");

	
	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/SlideInputTest/horizontal_rail.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/SlideInputTest/horizontal_block.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/SlideInputTest/vertical_rail.bmp", buf));
	prjHelper.AddRes("pic4", System::GetFilePath("/../HmiTest/Res/Qd/SlideInputTest/vertical_block.bmp", buf));

	prjHelper.AddCtrl(1, "SlideInputControl", "win1.tc1", [&prjHelper](void* param) {
		Project::SlideInputConfig* cfg = (Project::SlideInputConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;		
		cfg->Y = 250;
		
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->ReadVar = prjHelper.GetVarID("win1.var3");			//读取写入地址
		cfg->AttrMax = 100.0;		//最大值
		cfg->AttrMaxVar = prjHelper.GetVarID("win1.var4");		//最大值寄存器获取地址
		cfg->AttrMin = 0.0;			//最小值
		cfg->AttrMinVar = prjHelper.GetVarID("win1.var5");		//最小值寄存器获取地址

		cfg->MinScale = 1;						//最小刻度
		cfg->UseIncDecPerClick = false;			//是否使用每次点击增减最小刻度倍数
		cfg->IncDecPerClickScale = 1;			//每次点击增减最小刻度倍数
		cfg->UseChangeInput = false;				//滑动过程中是否实时改变写入值

		cfg->ShowAxis = true;					//是否显示轴线
		cfg->ShowScale = true;					//是否显示标注
		cfg->PrimaryScaleNum = 5;				//主刻度段数
		cfg->PrimaryScaleLen = 7;				//主刻度标记长度
		cfg->SecondaryScaleNum = 2;				//副刻度段数
		cfg->SecondaryScaleLen = 5;				//副刻度标记长度
		cfg->ScaleLineStyle.Color = 0xffffffff;	//刻度线颜色
		cfg->ScaleLineStyle.Type = 0;			//刻度线样式
		cfg->ScaleLineStyle.Weight = 0;			//刻度线宽度

		cfg->ShowMark = true;					//是否显示数字标识
		cfg->Act = 3;							//数字标识整数位
		cfg->Bct = 1;							//数字标识小数位
		cfg->MarkStyle.Align = 5;				//数字标识对齐方式
		cfg->MarkStyle.Colors = 0xffffffff;		//数字标识颜色	ABGR
		cfg->MarkStyle.Font.Name = "Arial";		//数字标识字体
		cfg->MarkStyle.Font.Size = 12;			//数字标识字体大小

		cfg->ShowPer = ~cfg->ShowMark;			//是否使用百分比标识
		cfg->PerStyle.Align = 5;				//百分比标识对齐方式
		cfg->PerStyle.Colors = 0xffffffff;		//百分比标识字体颜色
		cfg->PerStyle.Font.Name = "Arial";		//百分比标识字体样式
		cfg->PerStyle.Font.Size = 12;			//百分比标识字体大小

		/*设置各个模式下坐标位置*/
		cfg->Dir = Project::SlideDir::SildeDown;	//滑动输入方向（上下左右）
		cfg->ScalePos = 0;							//刻度位置	0（上方 / 左方） 1（下方 / 右方）
		if (cfg->Dir == Project::SlideDir::SildeRight ||			//水平滑动
			cfg->Dir == Project::SlideDir::SildeLeft)		
		{
			cfg->Width = 200;
			cfg->Height = 80;

			cfg->SlideRailWidth = cfg->Width;
			cfg->SlideRailHeight = 20;
			cfg->SlideRailBackColor = 0xff0000ff;
			cfg->SlideRailBorderColor = 0xffff0000;
			//cfg->SlideRailKey.KeyVal = "pic1";

			cfg->SlideWidth = 26;
			cfg->SlideHeight = 18;
			cfg->SlideBackColor = 0xff00ff00;
			cfg->SlideBorderColor = 0xff55ff55;
			//cfg->SlideKey.KeyVal = "pic2";

			if (cfg->ScalePos == 0)					//坐标位于滑动轨道上方
			{
				cfg->SlideRailPos.X = cfg->X;
				cfg->SlideRailPos.Y = cfg->Y + cfg->Height - cfg->SlideRailHeight;
				/*设置标注位置*/
				int interval = (cfg->SlideRailWidth - cfg->SlideWidth) / cfg->PrimaryScaleNum;
				for (int i = 0; i <= cfg->PrimaryScaleNum; i++)
				{
					Project::ScaleInfo tmpInfo;
					tmpInfo.Height = 10;
					tmpInfo.Width = 20;
					tmpInfo.Coordinate.X = (cfg->SlideRailPos.X + cfg->SlideWidth/2  - tmpInfo.Height/2) + i * interval;
					tmpInfo.Coordinate.Y = cfg->SlideRailPos.Y  - cfg->SlideRailHeight - cfg->PrimaryScaleLen - 2;
					tmpInfo.ScaleContent = "000.0";
					cfg->ScaleInfo.push_back(tmpInfo);
				}
			}
			else									//坐标位于滑动轨道下方
			{
				cfg->SlideRailPos.X = cfg->X;
				cfg->SlideRailPos.Y = cfg->Y;
				/*设置标注位置*/
				int interval = (cfg->SlideRailWidth - cfg->SlideWidth) / cfg->PrimaryScaleNum;
				for (int i = 0; i <= cfg->PrimaryScaleNum; i++)
				{
					Project::ScaleInfo tmpInfo;
					tmpInfo.Height = 10;
					tmpInfo.Width = 26;
					tmpInfo.Coordinate.X = (cfg->SlideRailPos.X + cfg->SlideWidth / 2 - tmpInfo.Height / 2) + i * interval;
					tmpInfo.Coordinate.Y = cfg->SlideRailPos.Y + cfg->SlideRailHeight + cfg->PrimaryScaleLen + 2;
					tmpInfo.ScaleContent = "000.0";
					cfg->ScaleInfo.push_back(tmpInfo);
				}
			}
		}
		else			//垂直滑动
		{
			cfg->Width = 80;
			cfg->Height = 200;

			cfg->SlideRailWidth = 20;
			cfg->SlideRailHeight = cfg->Height;
			cfg->SlideRailBackColor = 0xff0000ff;
			cfg->SlideRailBorderColor = 0xffff0000;
			cfg->SlideRailKey.KeyVal = "pic3";

			cfg->SlideWidth = 18;
			cfg->SlideHeight = 20;
			cfg->SlideBackColor = 0xff00ff00;
			cfg->SlideBorderColor = 0xff55ff55;
			cfg->SlideKey.KeyVal = "pic4";

			if (cfg->ScalePos == 0)			//坐标位于滑动轨道左方
			{
				cfg->SlideRailPos.X = cfg->X + cfg->Width - cfg->SlideRailWidth;
				cfg->SlideRailPos.Y = cfg->Y;
				/*设置标注位置*/
				int interval = (cfg->SlideRailHeight - cfg->SlideHeight) / cfg->PrimaryScaleNum;
				for (int i = 0; i <= cfg->PrimaryScaleNum; i++)
				{
					Project::ScaleInfo tmpInfo;
					tmpInfo.Height = 10;
					tmpInfo.Width = 20;
					tmpInfo.Coordinate.X = cfg->SlideRailPos.X - cfg->SlideRailWidth - cfg->PrimaryScaleLen - 4;
					tmpInfo.Coordinate.Y = cfg->SlideRailPos.Y  + cfg->SlideRailHeight - cfg->SlideHeight / 2 - tmpInfo.Height / 2 - interval * i;
					tmpInfo.ScaleContent = "000.0";
					cfg->ScaleInfo.push_back(tmpInfo);
				}
			}
			else
			{
				cfg->SlideRailPos.X = cfg->X;
				cfg->SlideRailPos.Y = cfg->Y;
				/*设置标注位置*/
				int interval = (cfg->SlideRailHeight - cfg->SlideHeight) / cfg->PrimaryScaleNum;
				for (int i = 0; i <= cfg->PrimaryScaleNum; i++)
				{
					Project::ScaleInfo tmpInfo;
					tmpInfo.Height = 10;
					tmpInfo.Width = 20;
					tmpInfo.Coordinate.X = cfg->SlideRailPos.X + cfg->SlideRailWidth + cfg->PrimaryScaleLen + 4;
					tmpInfo.Coordinate.Y = cfg->SlideRailPos.Y + cfg->SlideRailHeight - cfg->SlideHeight / 2 - tmpInfo.Height / 2 - interval * i;
					tmpInfo.ScaleContent = "000.0";
					cfg->ScaleInfo.push_back(tmpInfo);
				}
			}
		}
		/*设置写入前通知*/
		cfg->ResBef.Bit.DataBitVar = prjHelper.GetVarID("win1.var6");
		cfg->ResBef.Bit.State = Project::NotifyBitMode::SetOFF;

		cfg->ResBef.Word.DataWordVar = prjHelper.GetVarID("win1.var7");
		cfg->ResBef.Word.Val = 1;

		/*设置写入后通知*/
		cfg->ResAft.Bit.DataBitVar = prjHelper.GetVarID("win1.var8");
		cfg->ResAft.Bit.State = Project::NotifyBitMode::SetON;

		cfg->ResAft.Word.DataWordVar = prjHelper.GetVarID("win1.var9");
		cfg->ResAft.Word.Val = 0;

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var19");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var20");
		cfg->Perm.EnableLogic = true;
		});

		prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var7", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var8", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var9", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var19", "win1.tc1");
		prjHelper.BindCtrlAndVar("win1.var20", "win1.tc1");
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
		cfg->VarId = prjHelper.GetVarID("win1.var10");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var11");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc3");

	/*ReadVar 设置读写寄存器数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 150;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var12");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc4");


	/*AttrMaxVar 设置最大值寄存器数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 200;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 80;
		cfg->VarId = prjHelper.GetVarID("win1.var13");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc5");

	/*AttrMinVar 设置最小值寄存器数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 250;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 20;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc6");

	/*ResBef-Bit  写入前位通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateBitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var15");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc7");
	/*ResBef-Word 写入前字通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateWordVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var16");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc8");

	/*ResBef-Bit  写入后位通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc9", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateBitVar;
		cfg->X = 150;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var17");
		cfg->CtrlName = "win1.tc9";
		});
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc9");
	/*ResBef-Word 写入后字通知*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc10", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateWordVar;
		cfg->X = 200;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var18");
		cfg->CtrlName = "win1.tc10";
		});
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc10");

	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc11", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var21");
		cfg->CtrlName = "win1.tc11";
		});
	prjHelper.BindCtrlAndVar("win1.var21", "win1.tc11");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc12", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var22");
		cfg->CtrlName = "win1.tc12";
		});
	prjHelper.BindCtrlAndVar("win1.var22", "win1.tc12");
	prjHelper.Update();
}

bool SlideInputTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
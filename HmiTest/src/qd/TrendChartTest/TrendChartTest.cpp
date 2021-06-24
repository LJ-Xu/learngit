/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-14
 * Descript : 用于测试趋势图部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "TrendChartTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "TrendChartConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"
#include "DataSampleConfig.h"
#include "GTestConfig.h"
static Project::HMIProject prj;
void TrendChartTest::Init()
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

	/*数据采样寄存器*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var10");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var14");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 99, 4, "win1.var15");

	/*缩放、翻页*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var16");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var17");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var18");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var19");
	
	/*控制查询*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var20");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var21");

	// 添加数据采样控件
	//prjHelper.AddGCtrl(0, "SampleGCtrl", "win1.tc6", [&prjHelper](void * param) {
	//	Project::DataSampleConfig * cfg = (Project::DataSampleConfig *)param;
	//	cfg->CtrlName = "win1.tc6";
	//	cfg->SampleNoticeVarId = prjHelper.GetVarID("win1.var15");
	//	Project::SampleInfoRes res;
	//	res.SimpleGroupName = 111;
	//	res.SimpleGroupNo = 123;
	//	res.SimpleDescribe = "Sample";
	//	res.SimpleCtrlVarId = prjHelper.GetVarID("win1.var9");
	//	res.SimpleStyle = 0;
	//	res.CollectMode = Project::SampleMode::Cycle;
	//	// res.SimpleCycleVarId = prjHelper.GetVarID("win1.var10");
	//	res.SimpleCycle = 500;
	//	res.TriggerBitMode = 1;
	//	res.TriggerReadAddr = prjHelper.GetVarID("win1.var11");
	//	res.BtyeCondition = Project::Conditions::LT;
	//	res.BitCondition = 0;
	//	res.BitCond = Project::BitConditions::ON;
	//	res.ComPareValue = 10;
	//	// res.ComPareVar = prjHelper.GetVarID("win1.var12");
	//	res.CollectTimeFrom = 0;
	//	res.CollectTimeTo = 1;
	//	res.IsContinueAddr = true;
	//	Project::SampleChannel channel;
	//	channel.Channel = 1;
	//	channel.SampleAddr = prjHelper.GetVarID("win1.var13");
	//	channel.WordNum = 1;
	//	channel.DataStyle = "Word";
	//	channel.DataFormat = "Unsigned";
	//	channel.Describe = "Channel1";
	//	res.SimpleChannelLst.push_back(channel);

	//	channel.Channel = 2;
	//	channel.SampleAddr = prjHelper.GetVarID("win1.var13");
	//	channel.WordNum = 1;
	//	channel.DataStyle = "Word";
	//	channel.DataFormat = "Unsigned";
	//	channel.Describe = "Channel2";
	//	res.SimpleChannelLst.push_back(channel);
	//	cfg->InfoLst.push_back(res);
	//	});

	prjHelper.AddCtrl(1, "GTestControl", "win1.tc7", [&prjHelper](void* param) {
		Project::GTestConfig* cfg = (Project::GTestConfig*)param;
		cfg->FuncID = Project::FuncType::StringF;
		cfg->Val = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc7";
		cfg->Title = "switch";
		cfg->Height = 50;
		cfg->Width = 50;
		cfg->X = 300;
		cfg->Y = 100;
		});

	//prjHelper.BindCtrlAndVar("win1.var9", "win1.tc6");
	//// prjHelper.BindCtrlAndVar("win1.var10", "win1.tc1");
	//prjHelper.BindCtrlAndVar("win1.var11", "win1.tc6");
	//// prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");
	//prjHelper.BindCtrlAndVar("win1.var13", "win1.tc6");
	//prjHelper.BindCtrlAndVar("win1.var14", "win1.tc7");

	prjHelper.AddCtrl(1, "TrendChartControl", "win1.tc1", [&prjHelper](void* param) {
		Project::TrendChartConfig* cfg = (Project::TrendChartConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 273;
		cfg->Y = 265;
		cfg->Width = 486;
		cfg->Height = 407;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->DataCapacity = 1000;		//数据容量
		cfg->PerSceenViewMode = 0;		//屏幕显示内容	0:点数 1:时间段
		cfg->PerScreendot = 10;			//每屏数据容量
		cfg->PerScreenPeriod = 10000;	//每屏时间段(ms)
		cfg->SimpleGroup = 0;
		cfg->SimpleNo = 0;
		cfg->SampleVarId.Vid = prjHelper.GetVarID("win1.var15");
		Project::TrendChannel channelinfo;
		channelinfo.ChannelNo = 1;
		channelinfo.TrendMax = 100.0;
		channelinfo.TrendMin = 0.0;
		//channelinfo.TrendMaxVarId.Vid = prjHelper.GetVarID("win1.var1");
		//channelinfo.TrendMinVarId.Vid = prjHelper.GetVarID("win1.var1");
		channelinfo.TrendStyle.Color = 255;
		channelinfo.TrendStyle.Type = 0;
		channelinfo.TrendStyle.Weight = 0;
		cfg->SetChannel.push_back(channelinfo);

		channelinfo.ChannelNo = 2;
		channelinfo.TrendMax = 100.0;
		channelinfo.TrendMin = 0.0;
		//channelinfo.TrendMaxVarId.Vid = prjHelper.GetVarID("win1.var1");
		//channelinfo.TrendMinVarId.Vid = prjHelper.GetVarID("win1.var1");
		channelinfo.TrendStyle.Color = 255;
		channelinfo.TrendStyle.Type = 0;
		channelinfo.TrendStyle.Weight = 0;
		cfg->SetChannel.push_back(channelinfo);

		channelinfo.ChannelNo = 3;
		channelinfo.TrendMax = 100.0;
		channelinfo.TrendMin = 0.0;
		//channelinfo.TrendMaxVarId.Vid = prjHelper.GetVarID("win1.var1");
		//channelinfo.TrendMinVarId.Vid = prjHelper.GetVarID("win1.var1");
		channelinfo.TrendStyle.Color = 255;
		channelinfo.TrendStyle.Type = 0;
		channelinfo.TrendStyle.Weight = 0;
		cfg->SetChannel.push_back(channelinfo);

		cfg->BgColor = 10092543;
		cfg->ScaleAreabgColor = 16751052;
		cfg->IsGriddisplay = false;
		cfg->XUniformNum = 4;
		cfg->YUniformNum = 4;
		cfg->GridStyle.Color = 0;
		cfg->GridStyle.Type = 0;
		cfg->GridStyle.Weight = 0;


		cfg->AxisX.IsUseTimeScale = true;
		cfg->AxisX.IsdiaplayDate = true;
		cfg->AxisX.DateStyle = Project::DateMode::YMDSlash;
		cfg->AxisX.IsdisplayTime = true;
		cfg->AxisX.TimeStyle = Project::TimeMode::HMS;
		cfg->AxisX.IsdisplayScale = true;
		cfg->AxisX.ScaleColor = 0;
		cfg->AxisX.MainScaleNum = 3;
		cfg->AxisX.MainScaleLen = 10;
		cfg->AxisX.IsViceScaleNum = true;
		cfg->AxisX.ViceScaleNum = 1;
		cfg->AxisX.ViceScaleLen = 6;
		cfg->AxisX.ScaleStyle.Colors = 0;
		cfg->AxisX.ScaleStyle.Align = 0;
		cfg->AxisX.ScaleStyle.Font.Name = "Arial";
		cfg->AxisX.ScaleStyle.Font.Size = 12;


		cfg->AxisY.ScaleTag = Project::ScaleMarkType::NumSacle;
		cfg->AxisY.YIntegerNum = 5;
		cfg->AxisY.YDecimalnNum = 0;
		cfg->AxisY.ScaleRange = Project::YScaleMode::Custom;	//刻度范围	0：自定义 1：使用通道最大最小值 2：显示所有通道范围
		cfg->AxisY.UseRangeChanbelNo = false;
		cfg->AxisY.YScaleMax = 100.0;
		//cfg->AxisY.YScalemaxVarId.Vid = prjHelper.GetVarID("win1.var1");
		cfg->AxisY.YScaleMin = 0.0;
		//cfg->AxisY.YScaleminVarId.Vid = prjHelper.GetVarID("win1.var1");
		cfg->AxisY.IsdisplayScale = true;
		cfg->AxisY.ScaleColor = 0;
		cfg->AxisY.MainScaleNum = 3;
		cfg->AxisY.MainScaleLen = 10;
		cfg->AxisY.IsViceScaleNum = true;
		cfg->AxisY.ViceScaleNum = 1;
		cfg->AxisY.ViceScaleLen = 6;
		cfg->AxisY.ScaleStyle.Colors = 0;
		cfg->AxisY.ScaleStyle.Align = 0;
		cfg->AxisY.ScaleStyle.Font.Name = "Arial";
		cfg->AxisY.ScaleStyle.Font.Size = 12;

		cfg->OriginPos.X = 397;
		cfg->OriginPos.Y = 632;
		cfg->XaxisLen = 333;
		cfg->YaxisLen = 350;
		cfg->ScrollHeight = 10;

		Project::ScaleInfo scaleinfo;
		scaleinfo.Coordinate.X = 368;
		scaleinfo.Coordinate.Y = 642;
		scaleinfo.Height = 30;
		scaleinfo.Width = 58;
		scaleinfo.ScaleContent = "YY/MM/DD\nHH:MM:SS";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 479;
		scaleinfo.Coordinate.Y = 642;
		scaleinfo.Height = 30;
		scaleinfo.Width = 58;
		scaleinfo.ScaleContent = "YY/MM/DD\nHH:MM:SS";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 590;
		scaleinfo.Coordinate.Y = 642;
		scaleinfo.Height = 30;
		scaleinfo.Width = 58;
		scaleinfo.ScaleContent = "YY/MM/DD\nHH:MM:SS";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 701;
		scaleinfo.Coordinate.Y = 642;
		scaleinfo.Height = 30;
		scaleinfo.Width = 58;
		scaleinfo.ScaleContent = "YY/MM/DD\nHH:MM:SS";
		cfg->XScaleInfo.push_back(scaleinfo);

		vector<Project::ScaleInfo> tmpscale;
		scaleinfo.Coordinate.X = 349;
		scaleinfo.Coordinate.Y = 615;
		scaleinfo.Height = 15;
		scaleinfo.Width = 38;
		scaleinfo.ScaleContent = "00000.";
		tmpscale.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 349;
		scaleinfo.Coordinate.Y = 499;
		scaleinfo.Height = 15;
		scaleinfo.Width = 38;
		scaleinfo.ScaleContent = "00000.";
		tmpscale.push_back(scaleinfo);


		scaleinfo.Coordinate.X = 349;
		scaleinfo.Coordinate.Y = 383;
		scaleinfo.Height = 15;
		scaleinfo.Width = 38;
		scaleinfo.ScaleContent = "00000.";
		tmpscale.push_back(scaleinfo);


		scaleinfo.Coordinate.X = 349;
		scaleinfo.Coordinate.Y = 267;
		scaleinfo.Height = 15;
		scaleinfo.Width = 38;
		scaleinfo.ScaleContent = "00000.";
		tmpscale.push_back(scaleinfo);

		cfg->YScaleInfo.push_back(tmpscale);

		/*显示信息设置*/
		cfg->IsDataDisplay = true;
		Project::DataDisplay displayinfo;
		displayinfo.DescribeName = "日期";
		displayinfo.ProjectName = "日期";
		cfg->DisplayContent.push_back(displayinfo);

		displayinfo.DescribeName = "时间";
		displayinfo.ProjectName = "时间";
		cfg->DisplayContent.push_back(displayinfo);

		displayinfo.DescribeName = "曲线1";
		displayinfo.ProjectName = "1";
		cfg->DisplayContent.push_back(displayinfo);

		displayinfo.DescribeName = "曲线2";
		displayinfo.ProjectName = "2";
		cfg->DisplayContent.push_back(displayinfo);

		cfg->DisplayDateStyle = Project::DateMode::DMYSlash;
		cfg->DisplayTimeStyle = Project::TimeMode::HMS;
		cfg->InfoBgColor = 0xffff0055;
		cfg->InfoFontColor = 0;
		cfg->InfoLineColor = 0;

		cfg->ZoomVarId.Vid = prjHelper.GetVarID("win1.var16");
		cfg->PageCtrlVarId.Vid = prjHelper.GetVarID("win1.var17");
		/*查询设置*/
		cfg->EnableSearch = false;
		cfg->SearchMode = Project::TrendSeekMode::TIME;
		//cfg->SeekModeVar.Vid = prjHelper.GetVarID("win1.var1");
		cfg->SeekCtrlVar.Vid = prjHelper.GetVarID("win1.var20");
		cfg->SearchDate = 0;
		//Project::DataVarId tmpvid;
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);
		//tmpvid.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->SearchDateVarId.push_back(tmpvid);

		cfg->SearchTimeStart = 1610966818275;
		cfg->SearchTimeEnd =   1610966838275;
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

		cfg->SearchChannelNo = 1;
		//cfg->SearchChannelNoVar.Vid = prjHelper.GetVarID("win1.var1");

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
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc1");
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

	/*缩放*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 150;
		cfg->Min = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var18");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc8");

	/*翻页*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc9", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 10;
		cfg->Min = 1;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var19");
		cfg->CtrlName = "win1.tc9";
		});
	prjHelper.BindCtrlAndVar("win1.var19", "win1.tc9");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc10", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "search";
		cfg->VarId = prjHelper.GetVarID("win1.var21");
		cfg->CtrlName = "win1.tc10";
		});
	prjHelper.BindCtrlAndVar("win1.var21", "win1.tc10");
	prjHelper.Update();
}

bool TrendChartTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
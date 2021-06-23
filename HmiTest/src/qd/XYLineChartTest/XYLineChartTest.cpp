/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYLineChartTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-27
 * Descript : 用于测试XY折线图部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "XYLineChartTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "XYLineChartConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"
#include "GTestConfig.h"
static Project::HMIProject prj;
void XYLineChartTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(1300, 800);
	prjHelper.AddWin(1, 0, 0, 1300, 800, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");	//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");	//offy
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");	//触发地址
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");	//暂停
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var5");	//清除
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var6");	//翻页
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var7");	//采样点数
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 7, 4, "win1.var8");		//参考线1
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 9, 4, "win1.var9");		//参考线2
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 11, 4, "win1.var10");		//X上限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 13, 4, "win1.var11");	//X下限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 15, 4, "win1.var12");	//Y上限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 17, 4, "win1.var13");	//Y下限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var14");	//显示权限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var15");	//使能权限

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var16");	//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var17");	//offy
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var18");	//触发地址
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var19");	//暂停
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var20");	//清除
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var21");	//翻页
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var22");	//采样点数
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 7, 4, "win1.var23");		//参考线1
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 9, 4, "win1.var24");		//参考线2
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 11, 4, "win1.var25");		//X上限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 13, 4, "win1.var26");	//X下限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 15, 4, "win1.var27");	//Y上限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 17, 4, "win1.var28");	//Y下限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var29");	//显示权限
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var30");	//使能权限
	
																																						
	/*XY数据设置*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 25, 0, "win1.var31");	//通道1X数据
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 45, 0, "win1.var32");	//通道1Y数据
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 65, 0, "win1.var33");	//通道2X数据
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 85, 0, "win1.var34");	//通道2Y数据

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 25, 4, "win1.var37");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 27, 4, "win1.var38");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 29, 4, "win1.var39");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 31, 4, "win1.var40");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 33, 4, "win1.var41");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 35, 4, "win1.var42");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 37, 4, "win1.var43");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 39, 4, "win1.var44");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 41, 4, "win1.var45");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 43, 4, "win1.var46");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 65, 4, "win1.var47");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 67, 4, "win1.var48");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 69, 4, "win1.var49");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 71, 4, "win1.var50");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 73, 4, "win1.var51");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 75, 4, "win1.var52");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 77, 4, "win1.var53");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 79, 4, "win1.var54");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 81, 4, "win1.var55");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 83, 4, "win1.var56");

	prjHelper.AddCtrl(1, "XYLineChartControl", "win1.tc1", [&prjHelper](void* param) {
		Project::XYLineChartConfig* cfg = (Project::XYLineChartConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 263;
		cfg->Y = 156;
		cfg->Width = 941;
		cfg->Height = 527;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->RefreshMode = 0;												//刷新模式	0：周期式 1：触发式
		cfg->SamplingCycle = 1000;											//刷新周期(ms)
		cfg->TriggerTypeVar.Vid = prjHelper.GetVarID("win1.var3");			//触发地址
		cfg->TriggerCond = 0;												//触发条件	0：ON->OFF 1：OFF->ON
		cfg->PauseControlVar.Vid = prjHelper.GetVarID("win1.var4");			//暂停控制
		cfg->ClearControlVar.Vid = prjHelper.GetVarID("win1.var5");			//清除控制
		cfg->UseZoom = false;
		cfg->ZoomPercent = 100;					//缩放百分比
		cfg->PageControlStyle = 0;				//翻页模式 0：滚动条 1：寄存器
		//cfg->TurnControlVar.Vid = prjHelper.GetVarID("win1.var6");			//翻页寄存器
		cfg->SamplePoints = 5;
		//cfg->SamplePointsVar.Vid = prjHelper.GetVarID("win1.var7");			//采样点数寄存器
		cfg->PointsPerScreen = 10;
		cfg->ChannelNum = 2;

		Project::XYLineChannel channelinfo;
		channelinfo.Index = 1;
		channelinfo.IsXYDataFromSameSource = true;							//XY是否使用同寄存器
		channelinfo.StartXAddr.Vid = prjHelper.GetVarID("win1.var31");		//X数据起始寄存器
		//channelinfo.StartYAddr.Vid = prjHelper.GetVarID("win1.var32");		//Y数据起始寄存器
		channelinfo.CurveStyle = Project::XYLineMode::LineDot;				//曲线样式 线、点线、点、x投影、y投影
		channelinfo.LineColor = 255;
		channelinfo.LineStyle = 0;
		channelinfo.LineSize = 0;
		channelinfo.DrawDotColor = 0;
		channelinfo.DrawDotSize = 6;
		channelinfo.DrawDotStyle = Project::DotStyle::FilledTriangle;
		cfg->DataChanel.push_back(channelinfo);

		channelinfo.Index = 2;
		channelinfo.IsXYDataFromSameSource = true;							//XY是否使用同寄存器
		channelinfo.StartXAddr.Vid = prjHelper.GetVarID("win1.var33");		//X数据起始寄存器
		//channelinfo.StartYAddr.Vid = prjHelper.GetVarID("win1.var34");		//Y数据起始寄存器
		channelinfo.CurveStyle = Project::XYLineMode::LineDot;				//曲线样式 线、点线、点、x投影、y投影
		channelinfo.LineColor = 0xffff00ff;
		channelinfo.LineStyle = 0;
		channelinfo.LineSize = 0;
		channelinfo.DrawDotColor = 0xff00ff00;
		channelinfo.DrawDotSize = 4;
		channelinfo.DrawDotStyle = Project::DotStyle::Cricle;
		cfg->DataChanel.push_back(channelinfo);

		//channelinfo.Index = 3;
		//channelinfo.IsXYDataFromSameSource = true;							//XY是否使用同寄存器
		//channelinfo.StartXAddr.Vid = prjHelper.GetVarID("win1.var35");		//X数据起始寄存器
		//channelinfo.StartYAddr.Vid = prjHelper.GetVarID("win1.var36");		//Y数据起始寄存器
		//channelinfo.CurveStyle = Project::XYLineMode::LineDot;				//曲线样式 线、点线、点、x投影、y投影
		//channelinfo.LineColor = 255;
		//channelinfo.LineStyle = 0;
		//channelinfo.LineSize = 0;
		//channelinfo.DrawDotColor = 0xff00ff00;
		//channelinfo.DrawDotSize = 4;
		//channelinfo.DrawDotStyle = Project::DotStyle::Cricle;
		//cfg->DataChanel.push_back(channelinfo);

		cfg->ReferenceLineNum = 2;//参考线数量
		Project::ReferenceLine tmpline;
		tmpline.LineColor = 0xffff0000;
		tmpline.LineValue = 50.0;
		tmpline.LineValueVar.Vid = prjHelper.GetVarID("win1.var8");
		cfg->ReferenceLines.push_back(tmpline);

		tmpline.LineColor = 0xffffff00;
		tmpline.LineValue = 70.0;
		tmpline.LineValueVar.Vid = prjHelper.GetVarID("win1.var9");
		cfg->ReferenceLines.push_back(tmpline);

		cfg->LineChartBackColor = 10092543;
		cfg->ScaleAreaBackColor = 16751052;
		cfg->UseGrid = false;
		cfg->XGridEqualNum = 4;
		cfg->YGridEqualNum = 4;
		cfg->GridStyle.Color = 0;
		cfg->GridStyle.Type = 0;
		cfg->GridStyle.Weight = 0;

		cfg->UseXScale = true;		//是否使用X标注
		cfg->UseYScale = true;		//是否使用Y标注

		cfg->XScaleSet.UpperLimit = 100.0;
		//cfg->XScaleSet.UpperLimitVar.Vid = prjHelper.GetVarID("win1.var10");
		cfg->XScaleSet.LowerLimit = 0.0;
		//cfg->XScaleSet.LowerLimitVar.Vid = prjHelper.GetVarID("win1.var11");
		cfg->XScaleSet.ScaleColor = 0;
		cfg->XScaleSet.PrimaryScaleNum = 10;
		cfg->XScaleSet.PrimaryScaleLen = 10;
		cfg->XScaleSet.UseSecondaryScaleNum = true;
		cfg->XScaleSet.SecondaryScaleNum = 10;
		cfg->XScaleSet.SecondaryScaleLen = 10;
		cfg->XScaleSet.MarkType = true;
		cfg->XScaleSet.ScaleMarkFont = "Arial";
		cfg->XScaleSet.ScaleMarkColor = 0;
		cfg->XScaleSet.ScaleMarkSize = 12;
		cfg->XScaleSet.IntegerNum = 2;
		cfg->XScaleSet.DecimalnNum = 2;

		cfg->YScaleSet.UpperLimit = 100.0;
		//cfg->YScaleSet.UpperLimitVar.Vid = prjHelper.GetVarID("win1.var12");
		cfg->YScaleSet.LowerLimit = 0.0;
		//cfg->YScaleSet.LowerLimitVar.Vid = prjHelper.GetVarID("win1.var13");
		cfg->YScaleSet.ScaleColor = 0;
		cfg->YScaleSet.PrimaryScaleNum = 10;
		cfg->YScaleSet.PrimaryScaleLen = 10;
		cfg->YScaleSet.UseSecondaryScaleNum = true;
		cfg->YScaleSet.SecondaryScaleNum = 10;
		cfg->YScaleSet.SecondaryScaleLen = 10;
		cfg->YScaleSet.MarkType = true;
		cfg->YScaleSet.ScaleMarkFont = "Arial";
		cfg->YScaleSet.ScaleMarkColor = 0;
		cfg->YScaleSet.ScaleMarkSize = 12;
		cfg->YScaleSet.IntegerNum = 2;
		cfg->YScaleSet.DecimalnNum = 2;

		cfg->OriginPos.X = 304;
		cfg->OriginPos.Y = 668;
		cfg->XaxisLen = 900;
		cfg->YaxisLen = 512;
		cfg->ScrollHeight = 10;

		Project::ScaleInfo scaleinfo;
		scaleinfo.Coordinate.X = 289;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 379;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 469;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 559;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

	
		scaleinfo.Coordinate.X = 649;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 739;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 829;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 919;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);
		
		scaleinfo.Coordinate.X = 1009;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 1099;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 1189;
		scaleinfo.Coordinate.Y = 671;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->XScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 661;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);


		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 559;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);


		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 508;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 457;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 406;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 355;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 304;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 253;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 202;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		scaleinfo.Coordinate.X = 263;
		scaleinfo.Coordinate.Y = 151;
		scaleinfo.Height = 15;
		scaleinfo.Width = 31;
		scaleinfo.ScaleContent = "00.00";
		cfg->YScaleInfo.push_back(scaleinfo);

		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var14");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var15");
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
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var31", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var32", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var33", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var34", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var35", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var36", "win1.tc1");

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
		cfg->VarId = prjHelper.GetVarID("win1.var16");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var17");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc3");
	/*显示权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var29");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var29", "win1.tc4");

	/*使能权限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var30");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var30", "win1.tc5");

	/*触发*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "trigger";
		cfg->VarId = prjHelper.GetVarID("win1.var18");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc6");

	/*暂停*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "stop";
		cfg->VarId = prjHelper.GetVarID("win1.var19");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var19", "win1.tc7");

	/*清除*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 150;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "clear";
		cfg->VarId = prjHelper.GetVarID("win1.var20");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var20", "win1.tc8");

	/*采样点数*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc10", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 10;
		cfg->Min = 1;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var22");
		cfg->CtrlName = "win1.tc10";
		});
	prjHelper.BindCtrlAndVar("win1.var22", "win1.tc10");

	/*参考线1*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc11", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 100;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var23");
		cfg->CtrlName = "win1.tc11";
		});
	prjHelper.BindCtrlAndVar("win1.var23", "win1.tc11");

	/*参考线2*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc12", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 150;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var24");
		cfg->CtrlName = "win1.tc12";
		});
	prjHelper.BindCtrlAndVar("win1.var24", "win1.tc12");



	/*X上限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc13", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 50;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 80;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var25");
		cfg->CtrlName = "win1.tc13";
		});
	prjHelper.BindCtrlAndVar("win1.var25", "win1.tc13");

	/*X下限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc14", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 100;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 20;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var26");
		cfg->CtrlName = "win1.tc14";
		});
	prjHelper.BindCtrlAndVar("win1.var26", "win1.tc14");


	/*Y上限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc15", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 150;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 80;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var27");
		cfg->CtrlName = "win1.tc15";
		});
	prjHelper.BindCtrlAndVar("win1.var27", "win1.tc15");

	/*Y下限*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc16", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 200;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 20;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var28");
		cfg->CtrlName = "win1.tc16";
		});
	prjHelper.BindCtrlAndVar("win1.var28", "win1.tc16");

	/*通道1数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc17", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 300;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var37");
		cfg->CtrlName = "win1.tc17";
		});
	prjHelper.BindCtrlAndVar("win1.var37", "win1.tc17");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc18", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 350;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var38");
		cfg->CtrlName = "win1.tc18";
		});
	prjHelper.BindCtrlAndVar("win1.var38", "win1.tc18");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc19", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 400;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var39");
		cfg->CtrlName = "win1.tc19";
		});
	prjHelper.BindCtrlAndVar("win1.var39", "win1.tc19");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc20", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 450;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var40");
		cfg->CtrlName = "win1.tc20";
		});
	prjHelper.BindCtrlAndVar("win1.var40", "win1.tc20");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc21", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 500;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var41");
		cfg->CtrlName = "win1.tc21";
		});
	prjHelper.BindCtrlAndVar("win1.var41", "win1.tc21");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc22", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 550;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var42");
		cfg->CtrlName = "win1.tc22";
		});
	prjHelper.BindCtrlAndVar("win1.var42", "win1.tc22");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc23", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 600;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var43");
		cfg->CtrlName = "win1.tc23";
		});
	prjHelper.BindCtrlAndVar("win1.var43", "win1.tc23");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc24", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 650;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var44");
		cfg->CtrlName = "win1.tc24";
		});
	prjHelper.BindCtrlAndVar("win1.var44", "win1.tc24");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc25", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 700;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var45");
		cfg->CtrlName = "win1.tc25";
		});
	prjHelper.BindCtrlAndVar("win1.var45", "win1.tc25");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc26", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 750;
		cfg->Y = 50;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var46");
		cfg->CtrlName = "win1.tc26";
		});
	prjHelper.BindCtrlAndVar("win1.var46", "win1.tc26");



	/*通道2数据*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc27", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 300;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var47");
		cfg->CtrlName = "win1.tc27";
		});
	prjHelper.BindCtrlAndVar("win1.var47", "win1.tc27");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc28", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 350;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var48");
		cfg->CtrlName = "win1.tc28";
		});
	prjHelper.BindCtrlAndVar("win1.var48", "win1.tc28");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc29", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 400;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var49");
		cfg->CtrlName = "win1.tc29";
		});
	prjHelper.BindCtrlAndVar("win1.var49", "win1.tc29");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc30", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 450;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var50");
		cfg->CtrlName = "win1.tc30";
		});
	prjHelper.BindCtrlAndVar("win1.var50", "win1.tc30");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc31", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 500;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var51");
		cfg->CtrlName = "win1.tc31";
		});
	prjHelper.BindCtrlAndVar("win1.var51", "win1.tc31");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc32", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 550;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var52");
		cfg->CtrlName = "win1.tc32";
		});
	prjHelper.BindCtrlAndVar("win1.var52", "win1.tc32");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc33", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 600;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var53");
		cfg->CtrlName = "win1.tc33";
		});
	prjHelper.BindCtrlAndVar("win1.var53", "win1.tc33");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc34", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 650;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var54");
		cfg->CtrlName = "win1.tc34";
		});
	prjHelper.BindCtrlAndVar("win1.var54", "win1.tc34");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc35", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 700;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var55");
		cfg->CtrlName = "win1.tc35";
		});
	prjHelper.BindCtrlAndVar("win1.var55", "win1.tc35");

	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc36", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 750;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Max = 100;
		cfg->Min = 0;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var56");
		cfg->CtrlName = "win1.tc36";
		});
	prjHelper.BindCtrlAndVar("win1.var56", "win1.tc36");
	prjHelper.Update();
}

bool XYLineChartTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
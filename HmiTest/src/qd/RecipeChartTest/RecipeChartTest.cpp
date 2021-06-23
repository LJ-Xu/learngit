/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartTest.cpp
 * Author   : qiaodan
 * Date     : 2021-02-18
 * Descript : 用于测试配方部件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "RecipeChartTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "RecipeChartConfig.h"
#include "CharacterDisplayConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "KeyBtnConfig.h"
#include "RecipeGControl.h"
static Project::HMIProject prj;
void RecipeChartTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(1000, 600);
	prjHelper.AddWin(1, 0, 0, 1000, 600, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*用于显示部分*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");			//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");			//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");			//search

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var4");			//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var5");			//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var6");			//search

	/*用于控制权限*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var10");
	/*模拟键盘实时显示数据*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 50, 16, "win2.var1");	//InputKeyCurValue

	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/curvalue.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button1.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button2.bmp", buf));

	/*添加配方编辑配置*/
	prjHelper.AddCtrl(1, "RecipeGControl", "win1.tc1", [&prjHelper](void* param) {
		Project::DataRecipeConfig* cfg = (Project::DataRecipeConfig*)param;
		string name = "Recipe_0";
		//int i = 0;
		vector<Project::RecipeInfoRes> vectorinfo;
		Project::RecipeInfoRes info;
		info.RecipeName = "名称";
		info.DataType = Project::VarDataType::DT_String;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		info.RecipeData.push_back("甜面包");
		info.RecipeData.push_back("普通");
		info.RecipeData.push_back("无糖面包");
		info.RecipeData.push_back("低糖面包");
		info.RecipeData.push_back("多糖面包");
		info.RecipeData.push_back("一般口味");
		info.RecipeData.push_back("无盐面包");
		info.RecipeData.push_back("小面包");
		vectorinfo.push_back(info);

		info.RecipeName = "糖";
		info.DataType = Project::VarDataType::DT_DWORD;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		vector<string>().swap(info.RecipeData);
		info.RecipeData.push_back("25.00");
		info.RecipeData.push_back("55.00");
		info.RecipeData.push_back("22.00");
		info.RecipeData.push_back("12.00");
		info.RecipeData.push_back("32.00");
		info.RecipeData.push_back("4.00");
		info.RecipeData.push_back("11.00");
		info.RecipeData.push_back("23.00");
		vectorinfo.push_back(info);

		info.RecipeName = "水";
		info.DataType = Project::VarDataType::DT_DWORD;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		vector<string>().swap(info.RecipeData);
		info.RecipeData.push_back("16.00");
		info.RecipeData.push_back("12.00");
		info.RecipeData.push_back("44.00");
		info.RecipeData.push_back("23.00");
		info.RecipeData.push_back("12.00");
		info.RecipeData.push_back("6.00");
		info.RecipeData.push_back("77.00");
		info.RecipeData.push_back("45.00");
		vectorinfo.push_back(info);

		info.RecipeName = "面粉";
		info.DataType = Project::VarDataType::DT_DWORD;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		vector<string>().swap(info.RecipeData);
		info.RecipeData.push_back("26.00");
		info.RecipeData.push_back("45.00");
		info.RecipeData.push_back("66.00");
		info.RecipeData.push_back("43.00");
		info.RecipeData.push_back("78.00");
		info.RecipeData.push_back("7.00");
		info.RecipeData.push_back("89.00");
		info.RecipeData.push_back("45.00");
		vectorinfo.push_back(info);

		info.RecipeName = "盐";
		info.DataType = Project::VarDataType::DT_DWORD;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		vector<string>().swap(info.RecipeData);
		info.RecipeData.push_back("33.00");
		info.RecipeData.push_back("5.00");
		info.RecipeData.push_back("77.00");
		info.RecipeData.push_back("45.00");
		info.RecipeData.push_back("34.00");
		info.RecipeData.push_back("8.00");
		info.RecipeData.push_back("78.00");
		info.RecipeData.push_back("20.00");
		vectorinfo.push_back(info);

		info.RecipeName = "蜂蜜";
		info.DataType = Project::VarDataType::DT_DWORD;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		vector<string>().swap(info.RecipeData);
		info.RecipeData.push_back("45.00");
		info.RecipeData.push_back("34.00");
		info.RecipeData.push_back("44.00");
		info.RecipeData.push_back("23.00");
		info.RecipeData.push_back("89.00");
		info.RecipeData.push_back("12.00");
		info.RecipeData.push_back("56.00");
		info.RecipeData.push_back("12.00");
		vectorinfo.push_back(info);
		cfg->TbList.insert(pair<string, vector<Project::RecipeInfoRes>>(name, vectorinfo));
		});

	/*添加配方表配置*/
	prjHelper.AddCtrl(1, "RecipeChartControl", "win1.tc2", [&prjHelper](void* param) {
		Project::RecipeChartConfig* cfg = (Project::RecipeChartConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 200;
		cfg->Y = 200;
		cfg->Width = 596;
		cfg->Height = 300;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc2";

		cfg->RecipeGroupId = "Recipe_0";	//配方组名称
		cfg->ArrangMode = 0;				//排序方式 0:顺序 1:逆序
		cfg->TotalRowCount = 30;			//最大行数
		cfg->PerPageRowCount = 10;			//每页行数

		cfg->TitleUsed = true;				//是否显示标题
		cfg->TitleHeight = 20;				//标题高度
		cfg->TxtTitle.Texts.push_back("配方表");		//标题内容
		cfg->TitleStyle.Align = 0;						//标题样式
		cfg->TitleStyle.Font.Name = "Arial";
		cfg->TitleStyle.Font.Size = 14;
		cfg->TitleStyle.Colors = 16777164;

		cfg->ColTitleShown = true;			//是否显示列标题
		cfg->EachColTitleHeight = 15;
		cfg->ColTitleStyle.Align = 0;		//列标题样式
		cfg->ColTitleStyle.Font.Name = "Arial";
		cfg->ColTitleStyle.Font.Size = 12;
		cfg->ColTitleStyle.Colors = 0;


		/*设置显示信息项目*/
		Project::DisplayReportLst tmplst1;
		tmplst1.ProjectName = "序号";
		tmplst1.DescribeName = "序号";
		tmplst1.TitleDescribe.Texts.push_back("序号");
		tmplst1.ColWidth = 80;
		tmplst1.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst1);

		Project::DisplayReportLst tmplst2;
		tmplst2.ProjectName = "名称";
		tmplst2.DescribeName = "名称";
		tmplst2.TitleDescribe.Texts.push_back("名称");
		tmplst2.ColWidth = 100;
		tmplst2.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst2);

		Project::DisplayReportLst tmplst3;
		tmplst3.ProjectName = "糖";
		tmplst3.DescribeName = "糖";
		tmplst3.TitleDescribe.Texts.push_back("糖");
		tmplst3.ColWidth = 80;
		tmplst3.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst3);

		Project::DisplayReportLst tmplst4;
		tmplst4.ProjectName = "水";
		tmplst4.DescribeName = "水";
		tmplst4.TitleDescribe.Texts.push_back("水");
		tmplst4.ColWidth = 80;
		tmplst4.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst4);

		Project::DisplayReportLst tmplst5;
		tmplst5.ProjectName = "面粉";
		tmplst5.DescribeName = "面粉";
		tmplst5.TitleDescribe.Texts.push_back("面粉");
		tmplst5.ColWidth = 80;
		tmplst5.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst5);

		Project::DisplayReportLst tmplst6;
		tmplst6.ProjectName = "盐";
		tmplst6.DescribeName = "盐";
		tmplst6.TitleDescribe.Texts.push_back("盐");
		tmplst6.ColWidth = 80;
		tmplst6.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst6);

		Project::DisplayReportLst tmplst7;
		tmplst7.ProjectName = "蜂蜜";
		tmplst7.DescribeName = "蜂蜜";
		tmplst7.TitleDescribe.Texts.push_back("蜂蜜");
		tmplst7.ColWidth = 80;
		tmplst7.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst7);

		cfg->SerialNoShown = true;		//是否显示序号
		cfg->SerialNumStyle = 0;		//序号样式 0：数字 1：中文
		cfg->IsUseMultiLan = false;			//是否使用多语言
		cfg->AutoSize = false;

		cfg->ListFontStyle.Align = 0;		//列表文字样式
		cfg->ListFontStyle.Font.Name = "Arial";
		cfg->ListFontStyle.Font.Size = 12;
		cfg->ListFontStyle.Colors = 0;
		/*设置键盘*/
		cfg->KeyboardNum.KeypadIdx = 2;						//键盘 id
		cfg->KeyboardNum.KeypadPos.X = 100;
		cfg->KeyboardNum.KeypadPos.Y = 100;

		cfg->KeyboardChar.KeypadIdx = 3;						//键盘 id
		cfg->KeyboardChar.KeypadPos.X = 100;
		cfg->KeyboardChar.KeypadPos.Y = 100;

		cfg->AppearMode = 1;		//外观样式 0：使用图片样式 1：使用自定义外观
		//cfg->Key.KeyVal = "pic1";
		cfg->TableBgColor = 65535;
		cfg->TitleBackColor = 16751052;
		cfg->ColTitleBackColor = 16751052;
		cfg->ParityDiffColor = true;
		cfg->OddBackColor = 0xccffcc;
		cfg->EvenBackColor = 0xcc33cc;

		cfg->IsUseBorder = true;
		cfg->TableBorderStyle.Weight = 4;
		cfg->TableBorderStyle.Type = 0;
		cfg->TableBorderStyle.Color = 0;

		cfg->GridBorderStyle.Weight = 1;
		cfg->GridBorderStyle.Type = 1;
		cfg->GridBorderStyle.Color = 0;

		cfg->IsRowDividLine = true;
		cfg->IsColDividLine = true;

		cfg->FocusFontColor = 0xff3366;
		cfg->FocusCellColor = 0x66ff33;
		cfg->FocusRowColor = 0xffff00;

		/*查询设置*/
		cfg->EnableSearch = false;
		cfg->SearchMode = Project::RecipeSeekMode::RecipeKey;
		//cfg->SeekModeVar.Vid = prjHelper.GetVarID("win1.var1");
		cfg->SeekCtrlVar.Vid = prjHelper.GetVarID("win1.var3");
		/*显示权限*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var7");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*使能权限*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var8");
		cfg->Perm.EnableLogic = true;

		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc2");		//OFFX
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc2");		//OFFY
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc2");		//search
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc2");		//show
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc2");		//enable

	/*OFFX*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc3", [&prjHelper](void* param) {
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
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc3");

	/*OFFY*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
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
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc4");


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

	/*数字键盘*/
	prjHelper.AddWin(2, 300, 130, 167, 295, 0xffffffff);
	/*显示上限*/

	/*当前输入数据*/
	prjHelper.AddCtrl(2, "CharacterDisplayControl", "win2.tc16", [&prjHelper](void* param) {
		Project::CharacterDisplayConfig* cfg = (Project::CharacterDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 305;
		cfg->Y = 135;
		cfg->Width = 160;
		cfg->Height = 30;
		cfg->CtrlName = "win2.tc16";
		cfg->PswMode = 0;	//是否使用密码样式

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var1");


		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->FmtStr = "AA";

		});

	prjHelper.BindCtrlAndVar("win2.var1", "win2.tc16");


	/*模拟键盘7*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc1", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 170;

		cfg->Width = 48;
		cfg->Height = 48;
		cfg->CtrlName = "win2.tc1";
		cfg->AsciiNum = '7';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("7");					//设置文字
		cfg->Txt.Texts.push_back("7");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘8*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc2", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 170;

		cfg->Width = 48;
		cfg->Height = 48;
		cfg->CtrlName = "win2.tc2";
		cfg->AsciiNum = '8';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("8");					//设置文字
		cfg->Txt.Texts.push_back("8");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘9*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc3", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 170;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc3";
		cfg->AsciiNum = '9';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("9");					//设置文字
		cfg->Txt.Texts.push_back("9");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘4*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc4", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc4";
		cfg->AsciiNum = '4';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("4");					//设置文字
		cfg->Txt.Texts.push_back("4");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘5*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc5", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc5";
		cfg->AsciiNum = '5';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("5");					//设置文字
		cfg->Txt.Texts.push_back("5");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘6*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc6", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc6";
		cfg->AsciiNum = '6';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("6");					//设置文字
		cfg->Txt.Texts.push_back("6");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘1*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc7", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc7";
		cfg->AsciiNum = '1';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("1");					//设置文字
		cfg->Txt.Texts.push_back("1");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘2*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc8", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc8";
		cfg->AsciiNum = '2';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("2");					//设置文字
		cfg->Txt.Texts.push_back("2");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘3*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc9", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc9";
		cfg->AsciiNum = '3';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("3");					//设置文字
		cfg->Txt.Texts.push_back("3");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘0*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc10", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 325;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc10";
		cfg->AsciiNum = '0';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("0");					//设置文字
		cfg->Txt.Texts.push_back("0");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘 . */
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc11", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 325;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc11";
		cfg->AsciiNum = 190;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back(".");					//设置文字
		cfg->Txt.Texts.push_back(".");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘 BS*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc12", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 325;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc12";
		cfg->AsciiNum = 8;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("Bs");					//设置文字
		cfg->Txt.Texts.push_back("Bs");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘 Left*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc13", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 376;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc13";
		cfg->AsciiNum = 37;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("Left");				//设置文字
		cfg->Txt.Texts.push_back("Left");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘 Right*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc14", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 376;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc14";
		cfg->AsciiNum = 39;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("Right");				//设置文字
		cfg->Txt.Texts.push_back("Right");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*模拟键盘 Enter*/
	prjHelper.AddCtrl(2, "KeyBtnControl", "win2.tc15", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 376;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc15";
		cfg->AsciiNum = 13;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键松开时图片
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//设置按键按下时图片

		cfg->Txt.Texts.push_back("Ent");				//设置文字
		cfg->Txt.Texts.push_back("Ent");

		Project::StringStyle tmpstyle;					//设置按键松开时文字样式
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 20;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//设置按键按下时文字样式
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 20;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});


	prjHelper.Update();
}

bool RecipeChartTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
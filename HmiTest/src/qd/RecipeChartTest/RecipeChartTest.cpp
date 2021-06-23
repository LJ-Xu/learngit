/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartTest.cpp
 * Author   : qiaodan
 * Date     : 2021-02-18
 * Descript : ���ڲ����䷽����
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

	/*������ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");			//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");			//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");			//search

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var4");			//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var5");			//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var6");			//search

	/*���ڿ���Ȩ��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var10");
	/*ģ�����ʵʱ��ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 50, 16, "win2.var1");	//InputKeyCurValue

	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/curvalue.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button1.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button2.bmp", buf));

	/*����䷽�༭����*/
	prjHelper.AddCtrl(1, "RecipeGControl", "win1.tc1", [&prjHelper](void* param) {
		Project::DataRecipeConfig* cfg = (Project::DataRecipeConfig*)param;
		string name = "Recipe_0";
		//int i = 0;
		vector<Project::RecipeInfoRes> vectorinfo;
		Project::RecipeInfoRes info;
		info.RecipeName = "����";
		info.DataType = Project::VarDataType::DT_String;
		info.DataFmt = Project::VarNumberType::NT_Float;
		info.IntegerNum = 4;
		info.DecimalNum = 2;
		info.RecipeData.push_back("�����");
		info.RecipeData.push_back("��ͨ");
		info.RecipeData.push_back("�������");
		info.RecipeData.push_back("�������");
		info.RecipeData.push_back("�������");
		info.RecipeData.push_back("һ���ζ");
		info.RecipeData.push_back("�������");
		info.RecipeData.push_back("С���");
		vectorinfo.push_back(info);

		info.RecipeName = "��";
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

		info.RecipeName = "ˮ";
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

		info.RecipeName = "���";
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

		info.RecipeName = "��";
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

		info.RecipeName = "����";
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

	/*����䷽������*/
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

		cfg->RecipeGroupId = "Recipe_0";	//�䷽������
		cfg->ArrangMode = 0;				//����ʽ 0:˳�� 1:����
		cfg->TotalRowCount = 30;			//�������
		cfg->PerPageRowCount = 10;			//ÿҳ����

		cfg->TitleUsed = true;				//�Ƿ���ʾ����
		cfg->TitleHeight = 20;				//����߶�
		cfg->TxtTitle.Texts.push_back("�䷽��");		//��������
		cfg->TitleStyle.Align = 0;						//������ʽ
		cfg->TitleStyle.Font.Name = "Arial";
		cfg->TitleStyle.Font.Size = 14;
		cfg->TitleStyle.Colors = 16777164;

		cfg->ColTitleShown = true;			//�Ƿ���ʾ�б���
		cfg->EachColTitleHeight = 15;
		cfg->ColTitleStyle.Align = 0;		//�б�����ʽ
		cfg->ColTitleStyle.Font.Name = "Arial";
		cfg->ColTitleStyle.Font.Size = 12;
		cfg->ColTitleStyle.Colors = 0;


		/*������ʾ��Ϣ��Ŀ*/
		Project::DisplayReportLst tmplst1;
		tmplst1.ProjectName = "���";
		tmplst1.DescribeName = "���";
		tmplst1.TitleDescribe.Texts.push_back("���");
		tmplst1.ColWidth = 80;
		tmplst1.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst1);

		Project::DisplayReportLst tmplst2;
		tmplst2.ProjectName = "����";
		tmplst2.DescribeName = "����";
		tmplst2.TitleDescribe.Texts.push_back("����");
		tmplst2.ColWidth = 100;
		tmplst2.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst2);

		Project::DisplayReportLst tmplst3;
		tmplst3.ProjectName = "��";
		tmplst3.DescribeName = "��";
		tmplst3.TitleDescribe.Texts.push_back("��");
		tmplst3.ColWidth = 80;
		tmplst3.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst3);

		Project::DisplayReportLst tmplst4;
		tmplst4.ProjectName = "ˮ";
		tmplst4.DescribeName = "ˮ";
		tmplst4.TitleDescribe.Texts.push_back("ˮ");
		tmplst4.ColWidth = 80;
		tmplst4.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst4);

		Project::DisplayReportLst tmplst5;
		tmplst5.ProjectName = "���";
		tmplst5.DescribeName = "���";
		tmplst5.TitleDescribe.Texts.push_back("���");
		tmplst5.ColWidth = 80;
		tmplst5.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst5);

		Project::DisplayReportLst tmplst6;
		tmplst6.ProjectName = "��";
		tmplst6.DescribeName = "��";
		tmplst6.TitleDescribe.Texts.push_back("��");
		tmplst6.ColWidth = 80;
		tmplst6.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst6);

		Project::DisplayReportLst tmplst7;
		tmplst7.ProjectName = "����";
		tmplst7.DescribeName = "����";
		tmplst7.TitleDescribe.Texts.push_back("����");
		tmplst7.ColWidth = 80;
		tmplst7.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst7);

		cfg->SerialNoShown = true;		//�Ƿ���ʾ���
		cfg->SerialNumStyle = 0;		//�����ʽ 0������ 1������
		cfg->IsUseMultiLan = false;			//�Ƿ�ʹ�ö�����
		cfg->AutoSize = false;

		cfg->ListFontStyle.Align = 0;		//�б�������ʽ
		cfg->ListFontStyle.Font.Name = "Arial";
		cfg->ListFontStyle.Font.Size = 12;
		cfg->ListFontStyle.Colors = 0;
		/*���ü���*/
		cfg->KeyboardNum.KeypadIdx = 2;						//���� id
		cfg->KeyboardNum.KeypadPos.X = 100;
		cfg->KeyboardNum.KeypadPos.Y = 100;

		cfg->KeyboardChar.KeypadIdx = 3;						//���� id
		cfg->KeyboardChar.KeypadPos.X = 100;
		cfg->KeyboardChar.KeypadPos.Y = 100;

		cfg->AppearMode = 1;		//�����ʽ 0��ʹ��ͼƬ��ʽ 1��ʹ���Զ������
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

		/*��ѯ����*/
		cfg->EnableSearch = false;
		cfg->SearchMode = Project::RecipeSeekMode::RecipeKey;
		//cfg->SeekModeVar.Vid = prjHelper.GetVarID("win1.var1");
		cfg->SeekCtrlVar.Vid = prjHelper.GetVarID("win1.var3");
		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var7");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
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


	/*��ʾȨ��*/
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

	/*ʹ��Ȩ��*/
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

	/*���ּ���*/
	prjHelper.AddWin(2, 300, 130, 167, 295, 0xffffffff);
	/*��ʾ����*/

	/*��ǰ��������*/
	prjHelper.AddCtrl(2, "CharacterDisplayControl", "win2.tc16", [&prjHelper](void* param) {
		Project::CharacterDisplayConfig* cfg = (Project::CharacterDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 305;
		cfg->Y = 135;
		cfg->Width = 160;
		cfg->Height = 30;
		cfg->CtrlName = "win2.tc16";
		cfg->PswMode = 0;	//�Ƿ�ʹ��������ʽ

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var1");


		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->FmtStr = "AA";

		});

	prjHelper.BindCtrlAndVar("win2.var1", "win2.tc16");


	/*ģ�����7*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("7");					//��������
		cfg->Txt.Texts.push_back("7");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����8*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("8");					//��������
		cfg->Txt.Texts.push_back("8");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����9*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("9");					//��������
		cfg->Txt.Texts.push_back("9");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����4*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("4");					//��������
		cfg->Txt.Texts.push_back("4");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����5*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("5");					//��������
		cfg->Txt.Texts.push_back("5");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����6*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("6");					//��������
		cfg->Txt.Texts.push_back("6");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����1*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("1");					//��������
		cfg->Txt.Texts.push_back("1");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����2*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("2");					//��������
		cfg->Txt.Texts.push_back("2");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����3*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("3");					//��������
		cfg->Txt.Texts.push_back("3");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ�����0*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("0");					//��������
		cfg->Txt.Texts.push_back("0");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ����� . */
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back(".");					//��������
		cfg->Txt.Texts.push_back(".");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ����� BS*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Bs");					//��������
		cfg->Txt.Texts.push_back("Bs");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ����� Left*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Left");				//��������
		cfg->Txt.Texts.push_back("Left");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ����� Right*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Right");				//��������
		cfg->Txt.Texts.push_back("Right");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 14;
		cfg->StrStyles.push_back(tmpstyle);
		cfg->Perm.SafeTime = 100;

		});

	/*ģ����� Enter*/
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
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Ent");				//��������
		cfg->Txt.Texts.push_back("Ent");

		Project::StringStyle tmpstyle;					//���ð����ɿ�ʱ������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 0;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 20;
		cfg->StrStyles.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;					//���ð�������ʱ������ʽ
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
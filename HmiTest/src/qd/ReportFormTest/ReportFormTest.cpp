/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ReportFormTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-25
 * Descript : ���ڲ��Ա�����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ReportFormTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "ReportFormConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"
#include "DataSampleConfig.h"
#include "GTestConfig.h"
static Project::HMIProject prj;
void ReportFormTest::Init()
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

	/*Ȩ������*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var8");

	/*���ݲ����Ĵ���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var10");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var14");


	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 99, 4, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 99, 4, "win1.var16");


	/*���Ʋ�ѯ*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var20");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var21");

	// ������ݲ����ؼ�
	prjHelper.AddGCtrl(0, "SampleGCtrl", "win1.tc6", [&prjHelper](void * param) {
		Project::DataSampleConfig * cfg = (Project::DataSampleConfig *)param;
		cfg->CtrlName = "win1.tc6";
		cfg->SampleNoticeVarId = prjHelper.GetVarID("win1.var15");
		Project::SampleInfoRes res;
		res.SimpleGroupName = 111;
		res.SimpleGroupNo = 123;
		res.SimpleDescribe = "Sample";
		res.SimpleCtrlVarId = prjHelper.GetVarID("win1.var9");
		res.SimpleStyle = 0;
		res.CollectMode = Project::SampleMode::Cycle;
		// res.SimpleCycleVarId = prjHelper.GetVarID("win1.var10");
		res.SimpleCycle = 1000;
		res.TriggerBitMode = 1;
		res.TriggerReadAddr = prjHelper.GetVarID("win1.var11");
		res.BtyeCondition = Project::Conditions::LT;
		res.BitCondition = Project::BitConditions::OFF;
		res.ComPareValue = 10;
		// res.ComPareVar = prjHelper.GetVarID("win1.var12");
		res.CollectTimeFrom = 0;
		res.CollectTimeTo = 1;
		res.IsContinueAddr = true;
		Project::SampleChannel channel;
		channel.Channel = 1;
		channel.SampleAddr = prjHelper.GetVarID("win1.var13");
		channel.WordNum = 1;
		channel.DataStyle = Project::VarDataType::DT_WORD;
		channel.DataFormat = Project::VarNumberType::NT_Unsigned;
		channel.Describe = "Channel1";
		res.SimpleChannelLst.push_back(channel);

		channel.Channel = 2;
		channel.SampleAddr = prjHelper.GetVarID("win1.var13");
		channel.WordNum = 1;
		channel.DataStyle = Project::VarDataType::DT_WORD;
		channel.DataFormat = Project::VarNumberType::NT_Unsigned;
		channel.Describe = "Channel2";
		res.SimpleChannelLst.push_back(channel);
		cfg->InfoLst.push_back(res);
		});

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

	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc6");
	// prjHelper.BindCtrlAndVar("win1.var10", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc6");
	// prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc6");
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc7");

	prjHelper.AddCtrl(1, "ReportFormControl", "win1.tc1", [&prjHelper](void* param) {
		Project::ReportFormConfig* cfg = (Project::ReportFormConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 200;
		cfg->Y = 250;
		cfg->Width = 496;
		cfg->Height = 300;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->SampleVarId.Vid = prjHelper.GetVarID("win1.var16");	//֪ͨ�������ݱ仯�Ĵ���
		cfg->SimpleGroupName = 0;		//������
		cfg->SimpleGroupNo = 0;			//�������

		/*����ͨ������*/
		Project::ReportChannel tmpchannel;
		tmpchannel.SimpleGroupName = 0;
		tmpchannel.SimpleGroupNo = 0;
		tmpchannel.Channel = 1;
		tmpchannel.IntegerNum = 2;
		tmpchannel.DecimalNum = 1;
		tmpchannel.IsLead = false;
		tmpchannel.AlignStyle = 0;
		tmpchannel.ViewColor = 26367;
		cfg->ReportChannelLst.push_back(tmpchannel);

		tmpchannel.SimpleGroupName = 0;
		tmpchannel.SimpleGroupNo = 0;
		tmpchannel.Channel = 2;
		tmpchannel.IntegerNum = 4;
		tmpchannel.DecimalNum = 1;
		tmpchannel.IsLead = true;
		tmpchannel.AlignStyle = 0;
		tmpchannel.ViewColor = 0xff0000ff;
		cfg->ReportChannelLst.push_back(tmpchannel);

		tmpchannel.SimpleGroupName = 0;
		tmpchannel.SimpleGroupNo = 0;
		tmpchannel.Channel = 1;
		tmpchannel.IntegerNum = 3;
		tmpchannel.DecimalNum = 2;
		tmpchannel.IsLead = false;
		tmpchannel.AlignStyle = 5;
		tmpchannel.ViewColor = 16711935;
		cfg->ReportChannelLst.push_back(tmpchannel);

		cfg->IsViewSerialNo = true;				//�Ƿ���ʾ���
		cfg->SerialNum = 3;						//���λ��
		cfg->SerialColor = 26367;				//�����ɫ
		cfg->IsViewDate = true;					//�Ƿ���ʾ����
		cfg->DateColor = 52377;					//������ɫ
		cfg->IsViewTime = true;					//�Ƿ���ʾʱ��
		cfg->TimeColor = 6723891;				//ʱ����ɫ
		cfg->MaxRows = 30;						//��������
		cfg->PerPageRows = 10;					//ÿҳ����
		cfg->ViewSort = 1;						//��ʾ˳��0��˳�� 1������
		cfg->IsModifyCellValue = false;			//�Ƿ���޸ĵ�Ԫ������
		cfg->IsViewTitle = true;				//�Ƿ���ʾ����
		cfg->TitleHeight = 20;					//����߶�
		cfg->TitleRes.Texts.push_back("���");	//������������
		cfg->ElementTitleStyle.Align = 0;		//������ʽ
		cfg->ElementTitleStyle.Font.Name = "Arial";
		cfg->ElementTitleStyle.Font.Size = 14;
		cfg->ElementTitleStyle.Colors = 16777164;
		cfg->IsViewEachColTitle = true;			//�Ƿ�ʹ���б���
		cfg->EachColTitleHeight = 15;
		cfg->IsUseMultiLan = false;
		cfg->IsAutoRowColWidth = false;

		/*������ʾ��Ϣ��Ŀ*/
		Project::DisplayReportLst tmplst1;
		tmplst1.ProjectName = "���";
		tmplst1.DescribeName = "���";
		tmplst1.TitleDescribe.Texts.push_back("���");
		tmplst1.ColWidth = 80;
		tmplst1.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst1);

		Project::DisplayReportLst tmplst2;
		tmplst2.ProjectName = "ʱ��";
		tmplst2.DescribeName = "ʱ��";
		tmplst2.TitleDescribe.Texts.push_back("ʱ��");
		tmplst2.ColWidth = 80;
		tmplst2.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst2);

		Project::DisplayReportLst tmplst3;
		tmplst3.ProjectName = "����";
		tmplst3.DescribeName = "����";
		tmplst3.TitleDescribe.Texts.push_back("����");
		tmplst3.ColWidth = 80;
		tmplst3.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst3);

		Project::DisplayReportLst tmplst4;
		tmplst4.ProjectName = "1";
		tmplst4.DescribeName = "ͨ��1";
		tmplst4.TitleDescribe.Texts.push_back("ͨ��1");
		tmplst4.ColWidth = 80;
		tmplst4.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst4);

		Project::DisplayReportLst tmplst5;
		tmplst5.ProjectName = "2";
		tmplst5.DescribeName = "ͨ��2";
		tmplst5.TitleDescribe.Texts.push_back("ͨ��2");
		tmplst5.ColWidth = 80;
		tmplst5.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst5);

		Project::DisplayReportLst tmplst6;
		tmplst6.ProjectName = "3";
		tmplst6.DescribeName = "ͨ��3";
		tmplst6.TitleDescribe.Texts.push_back("ͨ��3");
		tmplst6.ColWidth = 80;
		tmplst6.RowHeight = 30;
		cfg->InfoLst.push_back(tmplst6);

		cfg->TitleBarStyle.Align = 0;
		cfg->TitleBarStyle.Font.Name = "Arial";
		cfg->TitleBarStyle.Font.Size = 12;
		cfg->TitleBarStyle.Colors = 0;

		cfg->ListFontStyle.Align = 0;
		cfg->ListFontStyle.Font.Name = "Arial";
		cfg->ListFontStyle.Font.Size = 12;
		cfg->ListFontStyle.Colors = 0;

		cfg->Appearance = 1;
		//cfg->Key.KeyVal = "pic1";
		cfg->TableBgColor = 65535;
		cfg->TitleBgColor = 16751052;
		cfg->IsUseBorder = true;
		cfg->TableBorderStyle.Weight = 4;
		cfg->TableBorderStyle.Type = 0;
		cfg->TableBorderStyle.Color = 0;

		cfg->GridBorderStyle.Weight = 1;
		cfg->GridBorderStyle.Type = 1;
		cfg->GridBorderStyle.Color = 0;

		cfg->IsRowDividLine = true;
		cfg->IsColDividLine = true;

		/*��ѯ����*/
		cfg->EnableSearch = true;
		cfg->SearchMode = Project::ReportSeekMode::ReportTIME;
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

		cfg->SearchTimeStart = 1611648553408;
		cfg->SearchTimeEnd = 1611648573408;
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

		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var5");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var6");
		cfg->Perm.EnableLogic = true;
		});
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc1");
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
	/*��ʾȨ��*/
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

	/*ʹ��Ȩ��*/
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

	/*��ѯ*/
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

bool ReportFormTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
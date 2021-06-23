/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableTest.cpp
 * Author   : qiaodan
 * Date     : 2020-12-24
 * Descript : ���ڲ��Ա�񲿼�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "TableTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "TableConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "Point.h"

static Project::HMIProject prj;
void TableTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var8");
	/*Ȩ�޿���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 3, 2, "win1.var6");

	prjHelper.AddCtrl(1, "TableControl", "win1.tc1", [&prjHelper](void* param) {
		Project::TableConfig* cfg = (Project::TableConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 250;
		cfg->Width = 200;
		cfg->Height = 100;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->OuterBorder.Color = 0xff00ff00;		//�����ɫ
		cfg->OuterBorder.Type = 0;					//���������
		cfg->OuterBorder.Weight = 0;				//����߿�

		cfg->HaveRowBorder = true;					//�Ƿ�ʹ���ڲ�����
		cfg->InnerBorderRow.Color = 0xff0000ff;		//������ɫ
		cfg->InnerBorderRow.Type = 0;				//��������
		cfg->InnerBorderRow.Weight = 0;				//�����߿�

		cfg->HaveColBorder = true;					//�Ƿ�ʹ���ڲ�����
		cfg->InnerBorderCol.Color = 0xff0000ff;		//������ɫ
		cfg->InnerBorderCol.Type = 0;				//��������
		cfg->InnerBorderCol.Weight = 0;				//�����߿�

		cfg->UseFillin = true;						//�Ƿ���䱳��
		cfg->FillColor = 0xffff0000;				//������ɫ

		int rowheight = 30;							//����ÿ���и�
		cfg->RowHeight.push_back(rowheight);
		rowheight = 70;
		cfg->RowHeight.push_back(rowheight);

		int colwidth = 90;							//����ÿ���п�
		cfg->ColWidth.push_back(colwidth);
		colwidth = 110;
		cfg->ColWidth.push_back(colwidth);

		cfg->Rows = 2;								//����
		cfg->Cols = 2;								//����

		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var3");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var4");
		cfg->Perm.EnableLogic = true;
		});
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");

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
		cfg->VarId = prjHelper.GetVarID("win1.var7");
		cfg->CtrlName = "win1.tc2";
		});
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc2");

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
		cfg->VarId = prjHelper.GetVarID("win1.var8");
		cfg->CtrlName = "win1.tc3";
		});
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc3");

	/*��ʾȨ��*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var5");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc4");

	/*ʹ��Ȩ��*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var6");
		cfg->CtrlName = "win1.tc5";
		});
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc5");

	prjHelper.Update();
}

bool TableTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
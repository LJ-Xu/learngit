/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-04
 * Descript : ���ڲ���ָʾ�Ʋ���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "PilotLampTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "PilotLampConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
static Project::HMIProject prj;

void PilotLampTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*����ָʾ�ƵļĴ���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");	//OFFX
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");	//OFFY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var3");	//Read

	/*������ʾ����ָʾ��״̬�ļĴ���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var6");


	/*���ڿ���Ȩ��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var10");


	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/BtnPilotLampTest/lampbutton1.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/BtnPilotLampTest/lampbutton2.bmp", buf));

	static bool UseSameAddr = false;		//��д�Ƿ�ʹ��ͬ��ַ	
	prjHelper.AddCtrl(1, "PilotLampControl", "win1.tc1", [&prjHelper](void* param) {
		Project::PilotLampConfig* cfg = (Project::PilotLampConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 200;
		cfg->Width = 60;
		cfg->Height = 60;

		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->Flash = 1;									//�Ƿ���˸	0:����˸
														//			1:ON״̬��˸
														//			2:OFF״̬��˸
		cfg->Logic = 0;									//��Ȧ״̬�����߼� 0:���߼� 1:���߼�
		cfg->Speed = 500;								//��˸�ٶ�

		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic1";
		cfg->StatusKeys.push_back(tmpImageKey);				//����OFF״̬ͼƬ
		tmpImageKey.KeyVal = "pic2";
		cfg->StatusKeys.push_back(tmpImageKey);				//����ON״̬ͼƬ

		cfg->Text.Texts.push_back("OFF");					//��������
		cfg->Text.Texts.push_back("ON");

		Project::StringStyle tmpstyle;						//����OFF״̬������ʽ
		tmpstyle.Colors = 0xff0000ff;
		tmpstyle.Align = 5;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->Csses.push_back(tmpstyle);

		tmpstyle.Colors = 0xff00ff00;						//����ON״̬������ʽ
		tmpstyle.Align = 5;
		tmpstyle.Font.Name = "Arial";
		tmpstyle.Font.Size = 24;
		cfg->Csses.push_back(tmpstyle);

		cfg->Status = prjHelper.GetVarID("win1.var3");	//��ȡ״̬�Ĵ���

		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var7");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var8");
		cfg->Perm.EnableLogic = true;
		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");
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

	/*ReadVar  д��Ĵ�����ʾ*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "Write";
		cfg->VarId = prjHelper.GetVarID("win1.var6");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc4");


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
	prjHelper.Update();
}

bool PilotLampTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
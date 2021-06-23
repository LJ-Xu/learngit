/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputTest.cpp
 * Author   : qiaodan
 * Date     : 2021-01-07
 * Descript : ���ڲ���������ʾ����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "DigitalInputTest.h"
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "DigitalInputConfig.h"
#include "DigitalDisplayConfig.h"
#include "CharacterDisplayConfig.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "KeyBtnConfig.h"

static Project::HMIProject prj;
void DigitalInputTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(800, 480);
	prjHelper.AddWin(1, 0, 0, 800, 480, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*������ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");	//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");	//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var3");		//read
	/*��ʾ���������޾���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var4");	//warnupper
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var5");	//warnlower
	/*��ʾ��������ת��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var6");	//novuppersrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var7");	//novlowersrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var8");	//novupperres
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var9");	//novupperres

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var10");	//offx
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var11");	//offY
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var12");	//read
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 4, 2, "win1.var13");	//warnupper
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 5, 2, "win1.var14");	//warnlower
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 6, 2, "win1.var15");	//novuppersrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 7, 2, "win1.var16");	//novlowersrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var17");	//novupperres
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var18");	//novupperres

	/*���ڿ���Ȩ��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var19");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var20");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 10, 2, "win1.var21");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 11, 2, "win1.var22");

	/*�������벿�� --- д�Ĵ���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var23");	//write same
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 12, 4, "win1.var24");	//write diff

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var25");	//��ʾд���� write same
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 14, 4, "win1.var26");	//��ʾд���� write diff
	/*����ת����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 16, 2, "win1.var27");	//novupperinputsrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 17, 2, "win1.var28");	//novlowerinputsrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 18, 2, "win1.var29");	//novupperinputres
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var30");	//novupperinputres

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 16, 2, "win1.var31");	//novupperinputsrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 17, 2, "win1.var32");	//novlowerinputsrc
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 18, 2, "win1.var33");	//novupperinputres
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 19, 2, "win1.var34");	//novupperinputres

	/*д��ǰ��λ��֪ͨ*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var35");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 21, 2, "win1.var36");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 22, 2, "win1.var37");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 23, 2, "win1.var38");

	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 20, 2, "win1.var39");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 21, 2, "win1.var40");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 22, 2, "win1.var41");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 23, 2, "win1.var42");


	/*ģ�������ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 82, 4, "win2.var1");		//InputKeyLowerLmt
	/*ģ�������ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 86, 4, "win2.var2");		//InputKeyUpperLmt
	/*ģ�����ʵʱ��ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_SPSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 50, 16, "win2.var3");	//InputKeyCurValue

	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/1.bmp", buf));
	prjHelper.AddRes("pic2", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button1.bmp", buf));
	prjHelper.AddRes("pic3", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/button2.bmp", buf));
	prjHelper.AddRes("pic4", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/input.bmp", buf));
	prjHelper.AddRes("pic5", System::GetFilePath("/../HmiTest/Res/Qd/DigitalInputTest/curvalue.bmp", buf));

	static bool UseWRSameAddr = true;		//��д�Ƿ�ʹ��ͬ��ַ	

	prjHelper.AddCtrl(1, "DigitalInputControl", "win1.tc1", [&prjHelper](void* param) {
		Project::DigitalInputConfig* cfg = (Project::DigitalInputConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 100;
		cfg->Y = 400;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win1.tc1";

		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;		//������ʾ����λ
		cfg->Accuracy.Bct = 2;		//������ʾС��λ

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var3");			//���Ĵ���
		if(UseWRSameAddr)
			cfg->WriteVar.Vid = prjHelper.GetVarID("win1.var23");	//д�Ĵ���
		else
			cfg->WriteVar.Vid = prjHelper.GetVarID("win1.var24");


		/*���ü���*/
		cfg->KeyMethod.Method = 0;									//����
		//cfg->KeyMethod.KeypadVar =								//λ���ƼĴ���
		cfg->KeyMethod.KeypadSrc = 0;								//0����������	1��ϵͳ����
		cfg->KeyMethod.Keypad.KeypadIdx = 2;						//���� id
		cfg->KeyMethod.Keypad.KeypadPos.X = 100;
		cfg->KeyMethod.Keypad.KeypadPos.Y = 100;

		/*�������ת��*/
		cfg->IsInputConvertUsed = true;
		cfg->NovUpperSrcInput.Num = 100;
		//cfg->NovUpperSrcInput.DataVarLmt.Vid = prjHelper.GetVarID("win1.var27");
		cfg->NovLowerSrcInput.Num = 0;
		//cfg->NovLowerSrcInput.DataVarLmt.Vid = prjHelper.GetVarID("win1.var28");
		cfg->NovUpperResInput.Num = 50;
		//cfg->NovUpperResInput.DataVarLmt.Vid = prjHelper.GetVarID("win1.var29");
		cfg->NovLowerResInput.Num = 0;
		//cfg->NovLowerResInput.DataVarLmt.Vid = prjHelper.GetVarID("win1.var30");

		/*��ʾ����������*/
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 80;
		cfg->WarnUpper.DataVarLmt.Vid = prjHelper.GetVarID("win1.var4");
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 40;
		cfg->WarnLower.DataVarLmt.Vid = prjHelper.GetVarID("win1.var5");

		/*������������ʽ*/
		cfg->IsWarnStyleUsed = true;
		cfg->WarnStyleUpper.Color = 0xff0000ff;
		cfg->WarnStyleUpper.Flash = 1;
		cfg->WarnStyleLower.Color = 0xff00ffff;
		cfg->WarnStyleLower.Flash = 1;

		/*��ʾ����ת��*/
		cfg->IsConvertUsed = true;
		cfg->NovUpperSrc.Num = 100;
		//cfg->NovUpperSrc.DataVarLmt.Vid = prjHelper.GetVarID("win1.var6");
		cfg->NovLowerSrc.Num = 0;
		//cfg->NovLowerSrc.DataVarLmt.Vid = prjHelper.GetVarID("win1.var7");
		cfg->NovUpperRes.Num = 50;
		//cfg->NovUpperRes.DataVarLmt.Vid = prjHelper.GetVarID("win1.var8");
		cfg->NovLowerRes.Num = 0;
		//cfg->NovLowerRes.DataVarLmt.Vid = prjHelper.GetVarID("win1.var9");

		cfg->Key.KeyVal = "pic1";			//�����ͼƬ
		cfg->TextStyle.Align = 0;			//������뷽ʽ
		cfg->TextStyle.Colors = 0xffffffff;	//������ɫ
		cfg->TextStyle.Font.Name = "Arial";	//������ʽ
		cfg->TextStyle.Font.Size = 18;		//�����С
		cfg->FmtStr = "0000";				//Ĭ���ַ���

		/*����д��ǰ֪ͨ*/
		cfg->ResBef.Bit.DataBitVar = prjHelper.GetVarID("win1.var35");
		cfg->ResBef.Bit.State = Project::NotifyBitMode::SetOFF;

		cfg->ResBef.Word.DataWordVar = prjHelper.GetVarID("win1.var36");
		cfg->ResBef.Word.Val = 1;

		/*����д���֪ͨ*/
		cfg->ResAft.Bit.DataBitVar = prjHelper.GetVarID("win1.var37");
		cfg->ResAft.Bit.State = Project::NotifyBitMode::SetON;

		cfg->ResAft.Word.DataWordVar = prjHelper.GetVarID("win1.var38");
		cfg->ResAft.Word.Val = 0;
		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var19");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var20");
		cfg->Perm.EnableLogic = true;

		});

	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");		//OFFX
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");		//OFFY
	prjHelper.BindCtrlAndVar("win1.var3", "win1.tc1");		//READ
	prjHelper.BindCtrlAndVar("win1.var4", "win1.tc1");		//warnupper
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");		//warnlower
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");		//novuppersrc
	prjHelper.BindCtrlAndVar("win1.var7", "win1.tc1");		//novlowersrc
	prjHelper.BindCtrlAndVar("win1.var8", "win1.tc1");		//novupperres
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc1");		//novlowerres
	prjHelper.BindCtrlAndVar("win1.var19", "win1.tc1");		//show
	prjHelper.BindCtrlAndVar("win1.var20", "win1.tc1");		//enable
	if(UseWRSameAddr)
		prjHelper.BindCtrlAndVar("win1.var23", "win1.tc1");	//write same
	else
		prjHelper.BindCtrlAndVar("win1.var24", "win1.tc1");	//write diff
	prjHelper.BindCtrlAndVar("win1.var27", "win1.tc1");		//novupperinputsrc
	prjHelper.BindCtrlAndVar("win1.var28", "win1.tc1");		//novlowerinputsrc
	prjHelper.BindCtrlAndVar("win1.var29", "win1.tc1");		//novupperinputres
	prjHelper.BindCtrlAndVar("win1.var30", "win1.tc1");		//novlowerinputres

	prjHelper.BindCtrlAndVar("win1.var35", "win1.tc1");		//д��ǰλ֪ͨ
	prjHelper.BindCtrlAndVar("win1.var36", "win1.tc1");		//д��ǰ��֪ͨ
	prjHelper.BindCtrlAndVar("win1.var37", "win1.tc1");		//д���λ֪ͨ
	prjHelper.BindCtrlAndVar("win1.var38", "win1.tc1");		//д�����֪ͨ
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

	/*ReadVar ���ö��Ĵ�������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc4", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 50;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 20;
		cfg->VarId = prjHelper.GetVarID("win1.var12");
		cfg->CtrlName = "win1.tc4";
		});
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc4");

	/*WriteVar ���ö��Ĵ�������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc5", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetFloat;
		cfg->X = 100;
		cfg->Y = 100;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 20;
		if(UseWRSameAddr)
			cfg->VarId = prjHelper.GetVarID("win1.var25");
		else
			cfg->VarId = prjHelper.GetVarID("win1.var26");

		cfg->CtrlName = "win1.tc5";
		});
	if(UseWRSameAddr)
		prjHelper.BindCtrlAndVar("win1.var25", "win1.tc5");
	else
		prjHelper.BindCtrlAndVar("win1.var26", "win1.tc5");


	/*WarnMax ���ñ�������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc6", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 70;
		cfg->Min = 60;
		cfg->VarId = prjHelper.GetVarID("win1.var13");
		cfg->CtrlName = "win1.tc6";
		});
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc6");

	/*WarnMin ���ñ�������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc7", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 100;
		cfg->Y = 150;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 40;
		cfg->Min = 30;
		cfg->VarId = prjHelper.GetVarID("win1.var14");
		cfg->CtrlName = "win1.tc7";
		});
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc7");

	/*NovUpperSrc ��ʾ����Դ����*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc8", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 80;
		cfg->VarId = prjHelper.GetVarID("win1.var15");
		cfg->CtrlName = "win1.tc8";
		});
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc8");

	/*NovLowerSrc ������ʾ����Դ����*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc9", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 100;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 20;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var16");
		cfg->CtrlName = "win1.tc9";
		});
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc9");

	/*NovUpperRes ������ʾת������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc10", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 150;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 80;
		cfg->VarId = prjHelper.GetVarID("win1.var17");
		cfg->CtrlName = "win1.tc10";
		});
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc10");

	/*NovLowerRes ������ʾת������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc11", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 200;
		cfg->Y = 200;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 20;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var18");
		cfg->CtrlName = "win1.tc11";
		});
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc11");

	/*NovUpperSrc ��������Դ����*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc12", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 50;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 80;
		cfg->VarId = prjHelper.GetVarID("win1.var31");
		cfg->CtrlName = "win1.tc12";
		});
	prjHelper.BindCtrlAndVar("win1.var31", "win1.tc12");

	/*NovLowerSrc ������������Դ����*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc13", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 100;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 20;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var32");
		cfg->CtrlName = "win1.tc13";
		});
	prjHelper.BindCtrlAndVar("win1.var32", "win1.tc13");

	/*NovUpperRes ��������ת������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc14", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 150;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 100;
		cfg->Min = 80;
		cfg->VarId = prjHelper.GetVarID("win1.var33");
		cfg->CtrlName = "win1.tc14";
		});
	prjHelper.BindCtrlAndVar("win1.var33", "win1.tc14");

	/*NovLowerRes ��������ת������*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc15", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::SetRandVar;
		cfg->X = 200;
		cfg->Y = 250;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "offy";
		cfg->Max = 20;
		cfg->Min = 0;
		cfg->VarId = prjHelper.GetVarID("win1.var34");
		cfg->CtrlName = "win1.tc15";
		});
	prjHelper.BindCtrlAndVar("win1.var34", "win1.tc15");

	/*ResBef-Bit  д��ǰλ֪ͨ*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc16", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateBitVar;
		cfg->X = 50;
		cfg->Y = 300;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var39");
		cfg->CtrlName = "win1.tc16";
		});
	prjHelper.BindCtrlAndVar("win1.var39", "win1.tc16");
	/*ResBef-Word д��ǰ��֪ͨ*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc17", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateWordVar;
		cfg->X = 100;
		cfg->Y = 300;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var40");
		cfg->CtrlName = "win1.tc17";
		});
	prjHelper.BindCtrlAndVar("win1.var40", "win1.tc17");

	/*ResBef-Bit  д���λ֪ͨ*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc18", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateBitVar;
		cfg->X = 150;
		cfg->Y = 300;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var41");
		cfg->CtrlName = "win1.tc18";
		});
	prjHelper.BindCtrlAndVar("win1.var41", "win1.tc18");
	/*ResBef-Word д�����֪ͨ*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc19", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::StateWordVar;
		cfg->X = 200;
		cfg->Y = 300;

		cfg->Width = 50;
		cfg->Height = 50;
		//cfg->Name = "EBit";
		cfg->VarId = prjHelper.GetVarID("win1.var42");
		cfg->CtrlName = "win1.tc19";
		});
	prjHelper.BindCtrlAndVar("win1.var42", "win1.tc19");

	/*��ʾȨ��*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc20", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 50;
		cfg->Y = 350;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "show";
		cfg->VarId = prjHelper.GetVarID("win1.var21");
		cfg->CtrlName = "win1.tc20";
		});
	prjHelper.BindCtrlAndVar("win1.var21", "win1.tc20");

	/*ʹ��Ȩ��*/
	prjHelper.AddCtrl(1, "TestBitVarControl", "win1.tc21", [&prjHelper](void* param) {
		Project::TestBitVarConfig* cfg = (Project::TestBitVarConfig*)param;
		cfg->Type = Project::VarType::BitVar;
		cfg->X = 100;
		cfg->Y = 350;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->Name = "enable";
		cfg->VarId = prjHelper.GetVarID("win1.var22");
		cfg->CtrlName = "win1.tc21";
		});
	prjHelper.BindCtrlAndVar("win1.var22", "win1.tc21");


	
	prjHelper.AddWin(2, 300, 100, 167, 325, 0xffffffff);
	/*��ʾ����*/
	prjHelper.AddCtrl(2, "DigitalDisplayControl", "win2.tc16", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 305;
		cfg->Y = 102;
		cfg->Width = 76;
		cfg->Height = 30;
		//cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win2.tc16";

		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var1");

		cfg->IsWarnUpperUsed = false;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;

		cfg->Key.KeyVal = "pic4";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;

		cfg->FmtStr = "0000";
		///*��ʾȨ��*/
		//cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var19");
		//cfg->Perm.ShowLogic = true;
		//cfg->Perm.DisplayMode = '1';
		///*ʹ��Ȩ��*/
		//cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var20");
		//cfg->Perm.EnableLogic = true;

		});

	prjHelper.BindCtrlAndVar("win2.var1", "win2.tc16");
	
	/*��ʾ����*/
	prjHelper.AddCtrl(2, "DigitalDisplayControl", "win2.tc17", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 388;
		cfg->Y = 102;
		cfg->Width = 76;
		cfg->Height = 30;
		//cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		//cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		cfg->CtrlName = "win2.tc17";

		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var2");

		cfg->IsWarnUpperUsed = false;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;

		cfg->Key.KeyVal = "pic4";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;

		cfg->FmtStr = "0000";

		});

	prjHelper.BindCtrlAndVar("win2.var2", "win2.tc17");

	/*��ǰ��������*/
	prjHelper.AddCtrl(2, "CharacterDisplayControl", "win2.tc18", [&prjHelper](void* param) {
		Project::CharacterDisplayConfig* cfg = (Project::CharacterDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 305;
		cfg->Y = 135;
		cfg->Width = 160;
		cfg->Height = 30;
		cfg->CtrlName = "win2.tc18";
		cfg->PswMode = 0;	//�Ƿ�ʹ��������ʽ

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var3");


		cfg->Key.KeyVal = "pic5";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->FmtStr = "AA";
	
		});

	prjHelper.BindCtrlAndVar("win2.var3", "win2.tc18");


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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

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
		cfg->Perm.DelayTime = 100;

		});


	prjHelper.Update();
}

bool DigitalInputTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
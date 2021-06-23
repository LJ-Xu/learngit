/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputTest.cpp
 * Author   : qiaodan
 * Date     : 2020-12-28
 * Descript : ���ڲ��Ի������벿��
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

	/*���ڻ�������ļĴ���*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 4, 4, "win1.var4");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 6, 4, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var8");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var9");


	/*���ڸ��Ļ���ʾ�����Ĵ���״̬��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 0, 2, "win1.var10");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 1, 2, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 2, 4, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 4, 4, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 6, 4, "win1.var14");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var16");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 8, 2, "win1.var17");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 9, 2, "win1.var18");

	/*���ڿ���Ȩ��*/
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

		cfg->ReadVar = prjHelper.GetVarID("win1.var3");			//��ȡд���ַ
		cfg->AttrMax = 100.0;		//���ֵ
		cfg->AttrMaxVar = prjHelper.GetVarID("win1.var4");		//���ֵ�Ĵ�����ȡ��ַ
		cfg->AttrMin = 0.0;			//��Сֵ
		cfg->AttrMinVar = prjHelper.GetVarID("win1.var5");		//��Сֵ�Ĵ�����ȡ��ַ

		cfg->MinScale = 1;						//��С�̶�
		cfg->UseIncDecPerClick = false;			//�Ƿ�ʹ��ÿ�ε��������С�̶ȱ���
		cfg->IncDecPerClickScale = 1;			//ÿ�ε��������С�̶ȱ���
		cfg->UseChangeInput = false;				//�����������Ƿ�ʵʱ�ı�д��ֵ

		cfg->ShowAxis = true;					//�Ƿ���ʾ����
		cfg->ShowScale = true;					//�Ƿ���ʾ��ע
		cfg->PrimaryScaleNum = 5;				//���̶ȶ���
		cfg->PrimaryScaleLen = 7;				//���̶ȱ�ǳ���
		cfg->SecondaryScaleNum = 2;				//���̶ȶ���
		cfg->SecondaryScaleLen = 5;				//���̶ȱ�ǳ���
		cfg->ScaleLineStyle.Color = 0xffffffff;	//�̶�����ɫ
		cfg->ScaleLineStyle.Type = 0;			//�̶�����ʽ
		cfg->ScaleLineStyle.Weight = 0;			//�̶��߿��

		cfg->ShowMark = true;					//�Ƿ���ʾ���ֱ�ʶ
		cfg->Act = 3;							//���ֱ�ʶ����λ
		cfg->Bct = 1;							//���ֱ�ʶС��λ
		cfg->MarkStyle.Align = 5;				//���ֱ�ʶ���뷽ʽ
		cfg->MarkStyle.Colors = 0xffffffff;		//���ֱ�ʶ��ɫ	ABGR
		cfg->MarkStyle.Font.Name = "Arial";		//���ֱ�ʶ����
		cfg->MarkStyle.Font.Size = 12;			//���ֱ�ʶ�����С

		cfg->ShowPer = ~cfg->ShowMark;			//�Ƿ�ʹ�ðٷֱȱ�ʶ
		cfg->PerStyle.Align = 5;				//�ٷֱȱ�ʶ���뷽ʽ
		cfg->PerStyle.Colors = 0xffffffff;		//�ٷֱȱ�ʶ������ɫ
		cfg->PerStyle.Font.Name = "Arial";		//�ٷֱȱ�ʶ������ʽ
		cfg->PerStyle.Font.Size = 12;			//�ٷֱȱ�ʶ�����С

		/*���ø���ģʽ������λ��*/
		cfg->Dir = Project::SlideDir::SildeDown;	//�������뷽���������ң�
		cfg->ScalePos = 0;							//�̶�λ��	0���Ϸ� / �󷽣� 1���·� / �ҷ���
		if (cfg->Dir == Project::SlideDir::SildeRight ||			//ˮƽ����
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

			if (cfg->ScalePos == 0)					//����λ�ڻ�������Ϸ�
			{
				cfg->SlideRailPos.X = cfg->X;
				cfg->SlideRailPos.Y = cfg->Y + cfg->Height - cfg->SlideRailHeight;
				/*���ñ�עλ��*/
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
			else									//����λ�ڻ�������·�
			{
				cfg->SlideRailPos.X = cfg->X;
				cfg->SlideRailPos.Y = cfg->Y;
				/*���ñ�עλ��*/
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
		else			//��ֱ����
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

			if (cfg->ScalePos == 0)			//����λ�ڻ��������
			{
				cfg->SlideRailPos.X = cfg->X + cfg->Width - cfg->SlideRailWidth;
				cfg->SlideRailPos.Y = cfg->Y;
				/*���ñ�עλ��*/
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
				/*���ñ�עλ��*/
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
		/*����д��ǰ֪ͨ*/
		cfg->ResBef.Bit.DataBitVar = prjHelper.GetVarID("win1.var6");
		cfg->ResBef.Bit.State = Project::NotifyBitMode::SetOFF;

		cfg->ResBef.Word.DataWordVar = prjHelper.GetVarID("win1.var7");
		cfg->ResBef.Word.Val = 1;

		/*����д���֪ͨ*/
		cfg->ResAft.Bit.DataBitVar = prjHelper.GetVarID("win1.var8");
		cfg->ResAft.Bit.State = Project::NotifyBitMode::SetON;

		cfg->ResAft.Word.DataWordVar = prjHelper.GetVarID("win1.var9");
		cfg->ResAft.Word.Val = 0;

		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var19");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
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

	/*ReadVar ���ö�д�Ĵ�������*/
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


	/*AttrMaxVar �������ֵ�Ĵ�������*/
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

	/*AttrMinVar ������Сֵ�Ĵ�������*/
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

	/*ResBef-Bit  д��ǰλ֪ͨ*/
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
	/*ResBef-Word д��ǰ��֪ͨ*/
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

	/*ResBef-Bit  д���λ֪ͨ*/
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
	/*ResBef-Word д�����֪ͨ*/
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

	/*��ʾȨ��*/
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

	/*ʹ��Ȩ��*/
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
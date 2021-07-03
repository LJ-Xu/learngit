/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataTableTest.cpp
 * Author   : qiaodan
 * Date     : 2021-03-27
 * Descript : ���ڲ������ݱ�񲿼�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "HMIProject.h"
#include "HmiCore.h"
#include "../../TestHelper/ProjectTestHelper.h"
#include "ControlFactory.h"
#include "TestBitVarConfig.h"
#include "System.h"
#include "GTestConfig.h"
#include "PinYinPageConfig.h"
#include "DataTableTest.h"
#include "KeyBtnConfig.h"
#include "DataTableConfig.h"
#include "DigitalDisplayConfig.h"
#include "ChineseDisplayConfig.h"
#include "CodeFormatUtility.h"
static Project::HMIProject prj;

void DataTableTest::Init()
{
	ProjectTestHelper prjHelper(&prj);
	/*************************************************/
	prjHelper.SettingScreen(1000, 800);
	prjHelper.AddWin(1, 0, 0, 1000, 800, 0x55f0550F0);
	prjHelper.SetStartWin(1);

	/*OFFX OFFY*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 100, 2, "win1.var1");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 101, 2, "win1.var2");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 100, 2, "win1.var3");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 101, 2, "win1.var4");

	/*Ȩ������*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 102, 2, "win1.var5");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 103, 2, "win1.var6");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 102, 2, "win1.var7");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 103, 2, "win1.var8");

	/*��ֵ�Ĵ�������*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 0, 4, "win1.var9");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var10");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 3, 16, "win1.var11");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 19, 4, "win1.var12");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 21, 4, "win1.var13");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 23, 16, "win1.var14");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 39, 4, "win1.var15");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 41, 2, "win1.var16");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 42, 16, "win1.var17");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 58, 4, "win1.var18");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 60, 4, "win1.var19");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 62, 2, "win1.var20");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 63, 16, "win1.var21");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 79, 4, "win1.var22");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 81, 4, "win1.var23");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 83, 2, "win1.var24");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 84, 16, "win1.var25");


	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 0, 4, "win1.var29");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 2, 2, "win1.var30");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 3, 16, "win1.var31");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 19, 4, "win1.var32");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 21, 4, "win1.var33");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 23, 16, "win1.var34");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 39, 4, "win1.var35");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 41, 2, "win1.var36");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 42, 16, "win1.var37");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 58, 4, "win1.var38");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 60, 4, "win1.var39");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 62, 2, "win1.var40");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 63, 16, "win1.var41");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 79, 4, "win1.var42");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_DWORD, Project::VarNumberType::NT_Float, 81, 4, "win1.var43");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_WORD, Project::VarNumberType::NT_Unsigned, 83, 2, "win1.var44");
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 84, 16, "win1.var45");

	/*ģ�����ʵʱ��ʾ����*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 170, 32, "win2.var1");		//InputKeyCurValue
	/*ƴ������ƴ��*/
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 130, 32, "win3.var1");		//read
	prjHelper.AddDataVar(0, 0, Project::LocalRegType::TP_PSW, Project::VarDataType::DT_String, Project::VarNumberType::NT_Unsigned, 170, 32, "win3.var2");		//InputKeyCurValue

	/*����ͼƬ��Դ*/
	char buf[255];
	prjHelper.AddRes("pic1", System::GetFilePath("/../HmiTest/Res/Qd/DigitalDisplayTest/1.bmp", buf));

	// ������ݱ��ؼ�
	prjHelper.AddCtrl(1, "DataTableControl", "win1.tc1", [&prjHelper](void * param) {
		Project::DataTableConfig * cfg = (Project::DataTableConfig *)param;
		cfg->CtrlName = "win1.tc1";
		cfg->X = 200;
		cfg->Y = 250;
		cfg->Width = 496;
		cfg->Height = 300;
		cfg->VOffX.Vid = prjHelper.GetVarID("win1.var1");
		cfg->VOffY.Vid = prjHelper.GetVarID("win1.var2");
		/*��ʾȨ��*/
		cfg->Perm.ShowVID = prjHelper.GetVarID("win1.var5");
		cfg->Perm.ShowLogic = true;
		cfg->Perm.DisplayMode = '1';
		/*ʹ��Ȩ��*/
		cfg->Perm.EnableVID = prjHelper.GetVarID("win1.var6");
		cfg->Perm.EnableLogic = true;

		cfg->UseContineAddr = false;		//�Ƿ�ʹ��������ַ
		cfg->RowsNum = 5;
		cfg->PerPageRows = 3;
		cfg->ColsNum = 4;
		cfg->UseTitleBar = true;
		cfg->ColTitleHeight = 30;						//�б����

		/*������ʾ��Ϣ��Ŀ*/
		Project::DisplayReportLst tmplst1;
		tmplst1.ProjectName = "���";
		tmplst1.DescribeName = "���";
		tmplst1.TitleDescribe.Texts.push_back("���");
		tmplst1.ColWidth = 80;
		tmplst1.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst1);

		Project::DisplayReportLst tmplst2;
		tmplst2.ProjectName = "��̬��";
		tmplst2.DescribeName = "��̬��";
		tmplst2.TitleDescribe.Texts.push_back("��̬��");
		tmplst2.ColWidth = 80;
		tmplst2.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst2);

		Project::DisplayReportLst tmplst3;
		tmplst3.ProjectName = "ˮ";
		tmplst3.DescribeName = "ˮ";
		tmplst3.TitleDescribe.Texts.push_back("ˮ");
		tmplst3.ColWidth = 80;
		tmplst3.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst3);

		Project::DisplayReportLst tmplst4;
		tmplst4.ProjectName = "��";
		tmplst4.DescribeName = "��";
		tmplst4.TitleDescribe.Texts.push_back("��");
		tmplst4.ColWidth = 80;
		tmplst4.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst4);

		Project::DisplayReportLst tmplst5;
		tmplst5.ProjectName = "���";
		tmplst5.DescribeName = "���";
		tmplst5.TitleDescribe.Texts.push_back("���");
		tmplst5.ColWidth = 80;
		tmplst5.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst5);

		Project::DisplayReportLst tmplst6;
		tmplst6.ProjectName = "��ע";
		tmplst6.DescribeName = "��ע";
		tmplst6.TitleDescribe.Texts.push_back("��ע");
		tmplst6.ColWidth = 80;
		tmplst6.RowHeight = 30;
		cfg->ColInfoLst.push_back(tmplst6);

		cfg->SerialNum = 2;							//���λ��
		Project::StatusRes name1;
		name1.Texts.push_back("С���");
		cfg->StaticColName.push_back(name1);
		Project::StatusRes name2;
		name2.Texts.push_back("ȫ�����");
		cfg->StaticColName.push_back(name2);
		Project::StatusRes name3;
		name3.Texts.push_back("�������");
		cfg->StaticColName.push_back(name3);
		Project::StatusRes name4;
		name4.Texts.push_back("��ɳ���");
		cfg->StaticColName.push_back(name4);
		Project::StatusRes name5;
		name5.Texts.push_back("��˾���");
		cfg->StaticColName.push_back(name5);

		cfg->UseSameStyle = false;						//�Ƿ�ʹ��ͬһ����
		cfg->TitleBarStyle.Align = 0;
		cfg->TitleBarStyle.Font.Name = "Arial";
		cfg->TitleBarStyle.Font.Size = 12;
		cfg->TitleBarStyle.Colors = 0;

		cfg->ListFontStyle.Align = 0;
		cfg->ListFontStyle.Font.Name = "Arial";
		cfg->ListFontStyle.Font.Size = 12;
		cfg->ListFontStyle.Colors = 0;

		cfg->SameFontStyle.Align = 0;
		cfg->SameFontStyle.Font.Name = "Arial";
		cfg->SameFontStyle.Font.Size = 12;
		cfg->SameFontStyle.Colors = 0;

		Project::DataCell cellinfo;
		cellinfo.CellType = 1;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "����";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var9");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 2;
		cellinfo.DecimalNum = 1;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule = UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var10");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 5;
		cellinfo.DecimalNum = 0;
		cellinfo.RegCount = 1;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 1;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var11");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 8;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var12");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var13");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 2;
		cellinfo.DecimalNum = 1;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 1;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "ȫ�����";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 1;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var14");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 8;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var15");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 1;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "����Ҫ��";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var16");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 5;
		cellinfo.DecimalNum = 0;
		cellinfo.RegCount = 1;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);

		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 1;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var17");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 8;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var18");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var19");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 2;
		cellinfo.DecimalNum = 1;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var20");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 5;
		cellinfo.DecimalNum = 0;
		cellinfo.RegCount = 1;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 1;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var21");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 8;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var22");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var23");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 2;
		cellinfo.DecimalNum = 1;
		cellinfo.RegCount = 2;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 0;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var24");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 5;
		cellinfo.DecimalNum = 0;
		cellinfo.RegCount = 1;						//�Ĵ�����
		cellinfo.Rule = UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cellinfo.CellType = 0;						//������� 0:��ַ��� 1:�ı�
		cellinfo.Text = "";
		if (cellinfo.CellType)
			cellinfo.IsEditable = false;			//�Ƿ�ɱ༭
		else
			cellinfo.IsEditable = true;
		cellinfo.AddrType = 1;						//��ַ���� 0:��ֵ	1:�ַ���
		cellinfo.DataAddrVar = prjHelper.GetVarID("win1.var25");				//��ȡ�Ĵ�����ַ
		cellinfo.IsFillZero = false;				//�Ƿ�ʹ��ǰ��0
		cellinfo.IntegerNum = 4;
		cellinfo.DecimalNum = 2;
		cellinfo.RegCount = 8;						//�Ĵ�����
		cellinfo.Rule =UI::CodeFormatUtility::UTF_8;					//�����ʽ
		cfg->DataCellInfoLst.push_back(cellinfo);


		cfg->Appearance = 1;
		//cfg->Key.KeyVal = "pic1";
		cfg->TableBgColor = 65535;
		cfg->TitleBgColor = 16751052;
		cfg->IsUseBorder = true;
		cfg->TableBorderStyle.Weight = 2;
		cfg->TableBorderStyle.Type = 0;
		cfg->TableBorderStyle.Color = 0;

		cfg->GridBorderStyle.Weight = 1;
		cfg->GridBorderStyle.Type = 1;
		cfg->GridBorderStyle.Color = 0;

		cfg->IsRowDividLine = true;
		cfg->IsColDividLine = true;
		});
	prjHelper.BindCtrlAndVar("win1.var1", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var2", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var5", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var6", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var9", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var10", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var11", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var12", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var13", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var14", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var15", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var16", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var17", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var18", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var19", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var20", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var21", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var22", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var23", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var24", "win1.tc1");
	prjHelper.BindCtrlAndVar("win1.var25", "win1.tc1");

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

	/*PSW0 DDWORD 2/1*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc6", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 300;
		cfg->Y = 50;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc6";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 2;
		cfg->Accuracy.Bct = 1;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var29");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var29", "win1.tc6");
	/*PSW2 DWORD 5/0*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc7", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 380;
		cfg->Y = 50;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc7";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 5;
		cfg->Accuracy.Bct = 0;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var30");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var30", "win1.tc7");
	/*PSW3 String 16*/
	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc8", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 460;
		cfg->Y = 50;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc8";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var31");

		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";
		cfg->Rule = UI::CodeFormatUtility::CodeRule::UTF_8;
		});
	prjHelper.BindCtrlAndVar("win1.var31", "win1.tc8");
	/*PSW19 DDWORD 4/2*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc9", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 540;
		cfg->Y = 50;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc9";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var32");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var32", "win1.tc9");
	/*PSW21 DDWORD 2/1*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc10", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 620;
		cfg->Y = 50;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc10";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 2;
		cfg->Accuracy.Bct = 1;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var33");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var33", "win1.tc10");
	/*PSW23 String 16*/
	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc11", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 300;
		cfg->Y = 100;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc11";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var34");

		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";
		cfg->Rule = UI::CodeFormatUtility::CodeRule::UTF_8;
		});
	prjHelper.BindCtrlAndVar("win1.var34", "win1.tc11");
	/*PSW39 DDWORD 4/2*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc12", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 380;
		cfg->Y = 100;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc12";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var35");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var35", "win1.tc12");
	/*PSW41 DWORD 5*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc13", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 460;
		cfg->Y = 100;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc13";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 5;
		cfg->Accuracy.Bct = 0;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var36");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var36", "win1.tc13");
	/*PSW42 String 16*/
	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc14", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 540;
		cfg->Y = 100;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc14";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var37");

		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";
		cfg->Rule = UI::CodeFormatUtility::CodeRule::UTF_8;
		});
	prjHelper.BindCtrlAndVar("win1.var37", "win1.tc14");
	/*PSW58 DDWORD 4/2*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc15", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 620;
		cfg->Y = 100;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc15";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var38");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var38", "win1.tc15");
	/*PSW60 DDWORD 2/1*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc16", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 300;
		cfg->Y = 150;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc16";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 2;
		cfg->Accuracy.Bct = 1;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var39");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var39", "win1.tc16");
	/*PSW62 DWORD 5*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc17", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 380;
		cfg->Y = 150;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc17";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 5;
		cfg->Accuracy.Bct = 0;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var40");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var40", "win1.tc17");
	/*PSW63 String 16*/
	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc18", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 460;
		cfg->Y = 150;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc18";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var41");

		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";
		cfg->Rule = UI::CodeFormatUtility::CodeRule::UTF_8;
		});
	prjHelper.BindCtrlAndVar("win1.var41", "win1.tc18");
	/*PSW79 DDWORD 4/2*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc19", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 540;
		cfg->Y = 150;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc19";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 4;
		cfg->Accuracy.Bct = 2;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var42");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var42", "win1.tc19");
	/*PSW81 DDWORD 2/1*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc20", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 620;
		cfg->Y = 150;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc20";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 2;
		cfg->Accuracy.Bct = 1;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var43");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var43", "win1.tc20");
	/*PSW83 DWORD 5*/
	prjHelper.AddCtrl(1, "DigitalDisplayControl", "win1.tc21", [&prjHelper](void* param) {
		Project::DigitalDisplayConfig* cfg = (Project::DigitalDisplayConfig*)param;
		cfg->X = 300;
		cfg->Y = 200;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc21";
		cfg->PswMode = 0;			//�Ƿ�ʹ��������ʽ
		cfg->IsFillZero = false;	//�Ƿ�ʹ��ǰ��0
		cfg->Accuracy.Act = 5;
		cfg->Accuracy.Bct = 0;
		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.var44");
		cfg->IsWarnUpperUsed = true;
		cfg->WarnUpper.Num = 100;
		cfg->IsWarnLowerUsed = true;
		cfg->WarnLower.Num = 0;
		cfg->IsWarnStyleUsed = false;
		cfg->IsConvertUsed = false;
		cfg->Key.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		});
	prjHelper.BindCtrlAndVar("win1.var44", "win1.tc21");
	/*PSW84 String 16*/
	prjHelper.AddCtrl(1, "ChineseDisplayControl", "win1.tc22", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 380;
		cfg->Y = 200;
		cfg->Width = 76;
		cfg->Height = 40;
		cfg->CtrlName = "win1.tc22";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win1.va45");

		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";
		cfg->Rule = UI::CodeFormatUtility::CodeRule::UTF_8;
		});
	prjHelper.BindCtrlAndVar("win1.var45", "win1.tc22");

	/*����ҳ��*/
	prjHelper.AddWin(25001, 300, 130, 167, 300, 0xffffffff);
	/*��ǰ��������*/
	prjHelper.AddCtrl(25001, "ChineseDisplayControl", "win2.tc12", [&prjHelper](void* param) {
		Project::ChineseDisplayConfig* cfg = (Project::ChineseDisplayConfig*)param;
		//cfg->FuncID = Project::FuncType::StringF;
		cfg->X = 305;
		cfg->Y = 135;
		cfg->Width = 160;
		cfg->Height = 30;
		cfg->CtrlName = "win2.tc16";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win2.var1");


		cfg->PicKey.KeyVal = "pic1";
		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xffffffff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 18;
		cfg->Text = "����";

		});
	prjHelper.BindCtrlAndVar("win2.var1", "win2.tc12");
	/*ģ�����1*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc1", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 170;

		cfg->Width = 48;
		cfg->Height = 48;
		cfg->CtrlName = "win2.tc1";
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
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc2", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 170;

		cfg->Width = 48;
		cfg->Height = 48;
		cfg->CtrlName = "win2.tc2";
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
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc3", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 170;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc3";
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
	/*ģ�����a*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc4", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc4";
		cfg->AsciiNum = 'A';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("a");					//��������
		cfg->Txt.Texts.push_back("a");

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
	/*ģ�����b*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc5", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc5";
		cfg->AsciiNum = 'B';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("b");					//��������
		cfg->Txt.Texts.push_back("b");

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
	/*ģ�����c*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc6", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 221;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc6";
		cfg->AsciiNum = 'C';
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("c");					//��������
		cfg->Txt.Texts.push_back("c");

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
	/*ģ�����.*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc7", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc7";
		cfg->AsciiNum = 110;
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
	/*ģ�������Ӣ��*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc8", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc8";
		cfg->AsciiNum = 16;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Ӣ��");					//��������
		cfg->Txt.Texts.push_back("����");

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
	/*ģ����̴�Сд*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc9", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 413;
		cfg->Y = 274;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc9";
		cfg->AsciiNum = 20;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Сд");					//��������
		cfg->Txt.Texts.push_back("��д");

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
	/*ģ�����Bs*/
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc10", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 305;
		cfg->Y = 325;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc10";
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
	/*ģ�����Enter */
	prjHelper.AddCtrl(25001, "KeyBtnControl", "win2.tc11", [&prjHelper](void* param) {
		Project::KeyBtnConfig* cfg = (Project::KeyBtnConfig*)param;
		cfg->X = 358;
		cfg->Y = 325;

		cfg->Width = 50;
		cfg->Height = 50;
		cfg->CtrlName = "win2.tc11";
		cfg->AsciiNum = 13;
		Project::ImageResId tmpImageKey;
		tmpImageKey.KeyVal = "pic2";
		cfg->PicKey.push_back(tmpImageKey);				//���ð����ɿ�ʱͼƬ
		tmpImageKey.KeyVal = "pic3";
		cfg->PicKey.push_back(tmpImageKey);				//���ð�������ʱͼƬ

		cfg->Txt.Texts.push_back("Ent");					//��������
		cfg->Txt.Texts.push_back("Ent");

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
	
	/*ƴ������*/
	prjHelper.AddWin(25900, 0, 0, 360, 80, 0xffffffff);
	prjHelper.AddCtrl(25900, "PinYinPageControl", "win3.tc1", [&prjHelper](void* param) {
		Project::PinYinPageConfig* cfg = (Project::PinYinPageConfig*)param;
		cfg->X = 0;
		cfg->Y = 0;
		cfg->Width = 360;
		cfg->Height = 80;
		cfg->CtrlName = "win3.tc1";

		cfg->ReadVar.Vid = prjHelper.GetVarID("win3.var1");
		cfg->WriteVar.Vid = prjHelper.GetVarID("win3.var2");


		cfg->TextStyle.Align = 0;
		cfg->TextStyle.Colors = 0xff0000ff;
		cfg->TextStyle.Font.Name = "Arial";
		cfg->TextStyle.Font.Size = 14;
		});
	prjHelper.BindCtrlAndVar("win3.var1", "win3.tc1");
	prjHelper.BindCtrlAndVar("win3.var2", "win3.tc1");


	prjHelper.Update();
}

bool DataTableTest::Run(int argc, char ** argv)
{
	Core::HmiCore core;
	Init();
	core.TestRun(&prj);
	return true;
}
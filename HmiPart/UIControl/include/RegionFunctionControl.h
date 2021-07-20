/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RegionFunctionControl.h
 * Author   : qiaodan
 * Date     : 2020-11-2
 * Descript : ���ڹ�����Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "RegionFunctionView.h"
#include "RegionFunctionModel.h"
namespace UI
{
	class RegionFunctionControl : public BaseControl
	{
	public:
		RegionFunctionControl() :RegionFunctionControl(nullptr) {}
		RegionFunctionControl(HMIPage*);
		bool HandleSysChange(SysChangeEM catogray);
		void CreateView() override;
		void OnReady();
		void OnClose();
		bool HavePerm();
		void StartRegionFunc();	//��ʼ���ö�Ӧ������
		void OnStartPage();		//���濪ʼ
		void OnClosePage();		//�������
		int TimeCount = 0;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<RegionFunctionModel> mode_;

		void HandleSetCoil(Project::FunctionSetup param);
		void HandleSetData(Project::FunctionSetup param);
		void HandleArithmetic(Project::FunctionSetup param);
		void HandleSendData(Project::FunctionSetup param);
		void HandleSwitchScreen(Project::FunctionSetup param);
		void HandleOpenWin(Project::FunctionSetup param);
		void HandleCloseWin(Project::FunctionSetup param);
		void HandleImportCSVdata(Project::BtnFunctionRes res);
		void HandleExportCSVdata(Project::BtnFunctionRes res);
		void HandleDownloadRecipe(Project::BtnFunctionRes res);
		void HandleUploadRecipe(Project::BtnFunctionRes res);
		void HandleCallbackFunc(Project::BtnFunctionRes res);
		void HandlePrintScreen(Project::FunctionSetup param);

		bool precoilstatus_ = false;
		bool stopfunc_ = false;
		bool hastimer_ = false;
		bool meetcond_ = false;
		bool switchwin_ = false;

		bool needswitchwin_ = false;
		int switchwinno_ = 0;
	};
}
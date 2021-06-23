/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڹ��ܰ����Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "BtnFunctionView.h"
#include "BtnFunctionModel.h"
namespace UI
{
	class BtnFunctionControl : public BaseControl
	{
	public:
		BtnFunctionControl() :BtnFunctionControl(nullptr) {}
		BtnFunctionControl(HMIPage*);
		void SetOffSetPos(int offx, int offy);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		bool HandlePermLimit(int region, bool popwin);
		void OnReady();
		/*******************************************************************************
		 * Name     : HandleBtnFunc
		 * Descript : ����״̬���ö�Ӧ������
		 * Input    : action - ����״̬
		 *******************************************************************************/
		bool HandleBtnFunc(int action);
		vector<Project::BtnFunctionRes> TimerTask;			//��ʱ������
		void HandleSetData(Project::FunctionSetup &param);

		bool StartTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<BtnFunctionModel> mode_;
		bool switchwin_ = false;

		void HandleSetCoil(Project::FunctionSetup param);
		void HandleArithmetic(Project::FunctionSetup param);
		void HandleSendData(Project::FunctionSetup param);
		void HandleSwitchScreen(Project::FunctionSetup param);//�л�����
		void HandleOpenWin(Project::FunctionSetup param);		//�򿪴���
		void HandleCloseWin(Project::FunctionSetup param);		//�رմ���
		void HandleImportCSVdata(Project::FunctionSetup param);
		void HandleExportCSVdata(Project::FunctionSetup param);
		void HandleDownloadRecipe(Project::FunctionSetup param);
		void HandleUploadRecipe(Project::FunctionSetup param);
		void HandleCallbackFunc(Project::FunctionSetup param);
		void HandlePrintScreen(Project::FunctionSetup param);

		bool needswitchwin_ = false;
		int switchwinno_ = 0;
	};
}


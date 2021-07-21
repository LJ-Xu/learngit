#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ReportFormControl.h
 * Author   : qiaodan
 * Date     : 2020-11-30
 * Descript : ���ڱ�����Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "ReportFormView.h"
#include "ReportFormModel.h"
namespace UI
{
	class ReportFormControl : public BaseControl
	{
	public:
		ReportFormControl() :ReportFormControl(nullptr) {}
		ReportFormControl(HMIPage*);
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int pageoffx, int pageoffy);
		void OnReady();
		void GetSearchRegData();

	protected:
		void CreateView()override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<ReportFormModel> mode_;
	};
}


#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorChartControl.h
 * Author   : qiaodan
 * Date     : 2021-05-11
 * Descript : ���ڲ�����¼��ʾ���Ƶ�ͷ�ļ������ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "OperatorChartView.h"
#include "OperatorChartModel.h"
namespace UI
{
	class OperatorChartControl : public BaseControl
	{
	public:
		OperatorChartControl() :OperatorChartControl(nullptr) {}
		OperatorChartControl(HMIPage*);
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int pageoffx, int pageoffy);
		void OnReady();
		void GetSearchRegData();

	protected:
		void CreateView()override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<OperatorChartModel> mode_;
	};
}

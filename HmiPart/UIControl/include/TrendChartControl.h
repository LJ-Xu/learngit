#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartControl.h
 * Author   : qiaodan
 * Date     : 2020-11-19
 * Descript : ��������ͼ�ؼ��Ŀ��Ƶ�ͷ�ļ�����������ͼ���ƣ����ݴ�����Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "TrendChartModel.h"
#include "TrendChartView.h"
#include <deque>
namespace UI
{
	class TrendChartControl : public BaseControl
	{
	public:
		TrendChartControl() :TrendChartControl(nullptr) {}
		TrendChartControl(HMIPage*);
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();
		void SetOffSetPos(int offx, int offy);
		void GetSearchRegData();

		/*������ݲ���*/
		//vector<deque<Project::SampleChannel::SampleContent>> Contents;
	protected:
		void CreateView();
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<TrendChartModel> mode_;
	};

}

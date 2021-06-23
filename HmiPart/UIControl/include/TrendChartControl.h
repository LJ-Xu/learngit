#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartControl.h
 * Author   : qiaodan
 * Date     : 2020-11-19
 * Descript : 关于趋势图控件的控制的头文件，创建趋势图绘制，传递处理消息
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

		/*存放数据采样*/
		//vector<deque<Project::SampleChannel::SampleContent>> Contents;
	protected:
		void CreateView();
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<TrendChartModel> mode_;
	};

}

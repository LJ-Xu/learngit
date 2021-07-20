#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYLineChartControl.h
 * Author   : qiaodan
 * Date     : 2020-12-09
 * Descript : 关于XY折线图控件的控制的头文件，创建趋势图绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "XYLineChartModel.h"
#include "XYLineChartView.h"

namespace UI
{
	struct XYLineChartData
	{
		XYLineChartView *View;
		XYLineChartModel *Model;
		int DataNo = 0;
		int DataType = 0;	//0:X数据，1:Y数据
	};
	class XYLineChartControl : public BaseControl
	{
	public:
		XYLineChartControl() :XYLineChartControl(nullptr) {}
		XYLineChartControl(HMIPage*);
		void OnReady();
		void GetSampleData();
		void HandleSampleData(BYTE* str, int len, int channelno,bool Isxdata);
		bool HandleSysChange(SysChangeEM catogray);
		XYLineChartData TimerData;
		int DataNo;			//以获取的数据个数
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:
		void CreateView();
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<XYLineChartModel> mode_;
		bool isSample_ = false;
		bool isStop_ = false;
		bool preSampleStatus_ = false;
		bool preClearStatus_ = false;
	};
	
}
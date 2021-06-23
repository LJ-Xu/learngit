#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataSampleControl.h
 * Author   : qiaodan
 * Date     : 2020-11-3
 * Descript : 关于数据采样的控制的头文件，创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DataSampleView.h"
#include "DataSampleModel.h"
#include "SampleInfoRes.h"
namespace UI
{
	struct TrendChartInfo
	{
		Project::WinCtrlID CtrlId;				//趋势图控件ID
		int SimpleGroupName;				//采样组别
		int SimpleGroupNo;					//采样编号
		string SimpleDescribe;				//采样组描述
		vector<int> Channel;						//通道编号
	};
	
	class DataSampleControl : public BaseControl
	{
	public:
		DataSampleControl() :DataSampleControl(nullptr) {}
		DataSampleControl(HMIPage*);
		void CreateView() override;
		vector<TrendChartInfo> Trend;
		vector<Project::SampleChannel::SampleContent> Contents;

		/*******************************************************************************
		 * Name     : MeetByteCondition
		 * Descript : 判断是否满足位数据报警触发条件
		 * Input    : cond  - 判断条件
		 *			  src   - 原数据
		 *			  dst   - 对比数据
		 * Output   : true  - 满足报警条件
		 *			  false - 不满足报警条件
		 * Note	    :
		 *******************************************************************************/
		bool MeetByteCondition(Project::Conditions cond, int src, int dst);
		/*******************************************************************************
		 * Name     : StoreSampleInfo
		 * Descript : 存储报警信息
		 * Input    : info - 触发报警的信息
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void HandleFileNameChange(Project::SampleInfoRes &);
		void HandleStorePosChange(Project::SampleInfoRes &);
		void RegisterCollectTime(time_t start, time_t end);

		void StartSample(Project::SampleInfoRes &info);
		bool MeetFixedSampleTime(Project::SampleInfoRes &info);
		void SetSampleContent(Project::SampleInfoRes &info);
		bool SampleIsFull(Project::SampleInfoRes &info);
		void ClearSampleData(Project::SampleInfoRes &info);
		void EndSample(Project::SampleInfoRes &info);
		string GetSampleData(Project::SampleChannel &channelinfo);


	protected:
		void AddVarData(vector<Project::DataVarId*> vars) override;
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DataSampleModel> mode_;
		int prevbitValue_;		//记录上一个位寄存器值
	};
	struct TimerData
	{
		DataSampleControl *ctrl;
		Project::SampleInfoRes *info;
	};

}


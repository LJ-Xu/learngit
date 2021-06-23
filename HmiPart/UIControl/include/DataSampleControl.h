#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataSampleControl.h
 * Author   : qiaodan
 * Date     : 2020-11-3
 * Descript : �������ݲ����Ŀ��Ƶ�ͷ�ļ����������̰������ƣ����ݴ�����Ϣ
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
		Project::WinCtrlID CtrlId;				//����ͼ�ؼ�ID
		int SimpleGroupName;				//�������
		int SimpleGroupNo;					//�������
		string SimpleDescribe;				//����������
		vector<int> Channel;						//ͨ�����
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
		 * Descript : �ж��Ƿ�����λ���ݱ�����������
		 * Input    : cond  - �ж�����
		 *			  src   - ԭ����
		 *			  dst   - �Ա�����
		 * Output   : true  - ���㱨������
		 *			  false - �����㱨������
		 * Note	    :
		 *******************************************************************************/
		bool MeetByteCondition(Project::Conditions cond, int src, int dst);
		/*******************************************************************************
		 * Name     : StoreSampleInfo
		 * Descript : �洢������Ϣ
		 * Input    : info - ������������Ϣ
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
		int prevbitValue_;		//��¼��һ��λ�Ĵ���ֵ
	};
	struct TimerData
	{
		DataSampleControl *ctrl;
		Project::SampleInfoRes *info;
	};

}


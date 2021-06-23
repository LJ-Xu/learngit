/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleGCtrl.h
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : 数据采样控制
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "SampleGModel.h"
#include "BaseGControl.h"
#include <BaseControl.h>
#include <vector>

using namespace std;

namespace UI
{
	void SampleValue_CB(void * param);
	void AddFixedTimer_CB(void * param);
	void SampleFlush_CB(void * param);
	class SampleGCtrl : public BaseGControl
	{
	public:
		SampleGCtrl() : SampleGCtrl(nullptr) { }
		SampleGCtrl(HMIPage *);
		~SampleGCtrl();

	public:
		void CreateView() override;
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataNotify(Project::DataVarId vid);
		void OnReady() override;
		void SampleValue(vector<Project::SampleChannel> &, DDWORD,int);
		void DealStroeCount(Project::SampleInfoRes& spIfRs);

	public:
		vector<Project::SampleInfoRes> FixedSampleGroups;
		vector<BOOL> FixedSampleTimer;
	private:
		shared_ptr<SampleGModel> model_;
		BOOL currentState_ = false;
		BOOL updateFlag_ = false;
	};
}


/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorGControl.h
 * Author   : qiaodan
 * Date     : 2021-05-12
 * Descript : 操作记录控件头文件
 * Version  : 0.1.6
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseGControl.h"
#include "OperatorGModel.h"
#include <BaseControl.h>
#include "OperatorRecord.h"
#include <vector>
using namespace std;

namespace UI
{
	class OperatorGControl : public BaseGControl
	{
	public:
		OperatorGControl() : OperatorGControl(nullptr) { }
		OperatorGControl(HMIPage *);
		~OperatorGControl();
		bool Changeflag = false;
	public:
		void CreateView() override;
		void OnReady();
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		void DeleteAllRecord();
		//template<typename T>
		void AddOperatorRecord(size_t win, string ctrlName, int action, Project::DataVarId vid, double orgVal, double newVal);
		void AddOperatorRecord(size_t win, string ctrlName, Project::DataVarId vid, char * orgVal, char * newVal);
		void AddOperatorRecord(size_t win, string ctrlName, int action, int startWin, int endWin);
		void AddOperatorRecord(size_t win, string ctrlName, Project::DataVarId vid, int action, string recipename, int count);
		void AddOperatorRecord(size_t win, string ctrlName, int action, vector<string> funcname);
		//void AddOperatorRecord(Storage::OperatorRecord & record);
		static OperatorGControl *Ins();
	private:
		shared_ptr<OperatorGModel> model_;
		static OperatorGControl *ctrl_;
		bool startrecord_ = false;
		bool preprintState_ = false;

	};
}

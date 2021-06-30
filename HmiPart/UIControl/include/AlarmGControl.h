/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmGControl.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 报警控件头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseGControl.h"
#include "AlarmGModel.h"
#include "AlarmGView.h"
#include <BaseControl.h>
#include <vector>
#include <thread>

using namespace std;

namespace UI
{
	class AlarmGControl : public BaseGControl
	{
	public:
		AlarmGControl() : AlarmGControl(nullptr) { }
		AlarmGControl(HMIPage *);
		~AlarmGControl();
		bool Changeflag = false;
	public:
		void CreateView() override;
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		void AddAlarmRecord(int groupname, int groupno, std::string& str, int times, Project::AlarmLevel level);
		string GetStringFromLibrary(Project::StatusRes & res);
		void HandleAlarmAction(Project::AlarmInfoRes& alarmres, bool triggeralarm);
		//std::string GetContextString(const AlarmInfoContext& almctxt);
		static AlarmGControl *Ins();
	private:
		//void AnalyseNumberStr(const string& ssrc,string& sdst,int itgDgt,int dmcDgt,bool isPre);
	private:
		shared_ptr<AlarmGModel> model_;
		//BOOL currentState = false;
		int index = 0;
		static AlarmGControl *ctrl_;

		std::thread* ThrBeep;
		std::thread* ThrPopWin;
		bool RunBeep;
		bool RunPop;
	};
}

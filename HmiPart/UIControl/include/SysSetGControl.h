#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SysSetGControl.h
 * Author   : qiaodan
 * Date     : 2021-05-12
 * Descript : 操作记录控件头文件
 * Version  : 0.1.6
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseGControl.h"
#include "SysSetGModel.h"
#include <BaseControl.h>
#include <vector>
using namespace std;
extern unsigned long long ScreenSaverClickTime;
namespace UI
{
	class SysSetGControl : public BaseGControl
	{
	public:
		SysSetGControl() : SysSetGControl(nullptr) { }
		SysSetGControl(HMIPage *);
		~SysSetGControl();
	public:
		void CreateView() override;
		void OnReady();
		void HandleDataVar(Project::DataVarId & id);
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);

		bool AchieveScreenSaverTime();
		void TriggerScreenSaver();
		void CloseScreenSaver(int mode = 0, int winno = 0);

		/*mode: 时钟格式 0-十进制 1-十六进制*/
		void GetSysTime(int mode, vector<int>& time);
		void GetExternalTime(vector<int>& time);
		bool WriteTimeToPlc();


		void ReportWinno(int winno);
	private:
		shared_ptr<SysSetGModel> mode_;
		bool preclockflag_;
		bool hasScreenTime = false;
	};
}



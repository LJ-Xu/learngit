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
	private:
		shared_ptr<SysSetGModel> mode_;
	};
}



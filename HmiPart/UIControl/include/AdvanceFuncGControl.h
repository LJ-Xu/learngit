/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AdvanceFuncGControl.h
 * Author   : qiaodan
 * Date     : 2021/3/23
 * Descript : 高级设置头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseGControl.h"
#include "AdvanceFuncGModel.h"
#include <BaseControl.h>
#include <vector>

using namespace std;

namespace UI
{
	class AdvanceFuncGControl : public BaseGControl
	{
	public:
		AdvanceFuncGControl() : AdvanceFuncGControl(nullptr) { }
		AdvanceFuncGControl(HMIPage *);
	public:
		void CreateView() override;
		int PeekHMIMessage(Message::Msg * msg);
		void HandleDataNotify(Project::DataVarId id);
		void HandleDataVar(Project::DataVarId &id);
	private:
		void HandleCommunicationSet(Project::AdvFuncSetup param);
		void HandleFlashDiskSet(Project::AdvFuncSetup param);
		void HandleSDCardSet(Project::AdvFuncSetup param);
		void HandleSystemParam(Project::AdvFuncSetup param);
		void HandleRestart(Project::AdvFuncSetup param);
		void HandleCallMacros(Project::AdvFuncSetup param);
		void HandleCallWindow(Project::AdvFuncSetup param);
		void HandleCloseWindow(Project::AdvFuncSetup param);
		void HandleDataTransfer(Project::AdvFuncSetup param);
		void HandleCallSystemCommand(Project::AdvFuncSetup param);
	
		shared_ptr<AdvanceFuncGModel> mode_;
	};
}

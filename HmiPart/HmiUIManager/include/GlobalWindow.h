#pragma once
#include "stdafx.h"
#include "HMIWindow.h"
#include "UIManager.h"
#include "DeleteHelper.h"
#include "BaseGControl.h"
namespace UI
{
	
	class GlobalWindow :public HMIWindow
	{
	public:
		GlobalWindow(int w, int h, const char* title = 0);
		void PeekUnHandleMessage(Message::Msg* msg);
		//不在UI线程执行的函数
		void HandleDataNotify(int ctrlidx, Project::DataVarId id);
	private:
		std::shared_ptr<BaseGControl> ctrls_[GCTRL_SIZE];
		//void AddGCtrl(GWinCtrlEnum ctrlEnum, std::unique_ptr<BaseGControl> ctrl);
	};
}

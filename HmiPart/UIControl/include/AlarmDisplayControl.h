/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmDisplayControl.h
 * Author   : qiaodan
 * Date     : 2020-11-5
 * Descript : 关于报警显示控制的头文件，创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "AlarmDisplayView.h"
#include "AlarmDisplayModel.h"
#include "Permission.h"

namespace UI
{
	class AlarmDisplayControl : public BaseControl
	{
	public:
		AlarmDisplayControl() : AlarmDisplayControl(nullptr) {}
		AlarmDisplayControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
		void GetSearchRegData();
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<AlarmDisplayModel> mode_;
	};
}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于数值显示框的控制的头文件，创建数值显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "DigitalDisplayModel.h"
#include "DigitalDisplayView.h"
namespace UI
{
	struct DigitalDisplayData
	{
		DigitalDisplayView *View;
		DigitalDisplayModel *Model;
	};
	class DigitalDisplayControl : public BaseControl
	{
	public:
		DigitalDisplayControl() :DigitalDisplayControl(nullptr) {}
		DigitalDisplayControl(HMIPage*);
		void CreateView()override;
		void HandleSysChange(SysChangeEM catogray);
		void OnReady();		//开始
		void SetOffSetPos(int offx, int offy);

		DigitalDisplayData TimerData;
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<DigitalDisplayModel> mode_;
	};
}

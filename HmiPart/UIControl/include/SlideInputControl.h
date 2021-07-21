/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputControl.h
 * Author   : qiaodan
 * Date     : 2020-12-18
 * Descript : 关于滑动输入控件的控制的头文件，创建滑动输入绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "SlideInputModel.h"
#include "SlideInputView.h"
namespace UI
{
	class SlideInputControl : public BaseControl
	{
	public:
		SlideInputControl() :SlideInputControl(nullptr) {}
		SlideInputControl(HMIPage*);
		void CreateView()override;
		bool HandleSysChange(SysChangeEM catogray);
		void SetOffSetPos(int offx, int offy);
		void OnReady();
	protected:
		int PeekHMIMessage(Message::Msg* msg);
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<SlideInputModel> mode_;
	};

}

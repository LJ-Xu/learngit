/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightControl.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 关于定时器的控制的头文件，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "BackLightView.h"
#include "BackLightModel.h"
namespace UI
{
	class BackLightControl : public BaseControl
	{
	public:
		BackLightControl() :BackLightControl(nullptr) {}
		BackLightControl(HMIPage*);
		void CreateView()override;
		void OnReady();//开始
		void SetOffSetPos(int offx, int offy);
		bool HandleSysChange(SysChangeEM catogray);

	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varIds);
	private:
		std::shared_ptr<BackLightModel> mode_;

	};
}


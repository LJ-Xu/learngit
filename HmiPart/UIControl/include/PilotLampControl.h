/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 关于指示灯的控制的头文件，创建指示灯绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "PilotLampModel.h"
#include "PilotLampView.h"
namespace UI
{
	struct PilotLampData
	{
		PilotLampModel *Model;
		PilotLampView *View;
	};
	class PilotLampControl : public BaseControl
	{
	public:
		PilotLampControl() :PilotLampControl(nullptr) {}
		PilotLampControl(HMIPage*  );
		void CreateView()override;
		void OnReady();//开始
		void HandleSysChange(SysChangeEM catogray);
		PilotLampData LampTimerData;
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr <PilotLampModel> mode_;
	};
}


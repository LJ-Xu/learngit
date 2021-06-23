/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于指示灯按键按键的控制的头文件，创建指示灯按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include "BtnPilotLampModel.h"
#include "BtnPilotLampView.h"
#include "PermUtility.h"
#include "BaseControl.h"
namespace UI
{
	struct BtnPilotLampData
	{
		BtnPilotLampModel *Model;
		BtnPilotLampView *View;
	};
	class BtnPilotLampControl : public BaseControl
	{
	public:
		BtnPilotLampControl() :BtnPilotLampControl(nullptr) {}
		BtnPilotLampControl(HMIPage*  );
		void CreateView()override;
		void OnReady();//开始
		void HandleSysChange(SysChangeEM catogray);
		//bool HandleOperatePush();
		BtnPilotLampData BtnLampData;
		void SetOffSetPos(int offx, int offy);
		bool HasTimer = false;
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
		void HandleDataVar(Project::DataVarId &varId);
	private:
		std::shared_ptr<BtnPilotLampModel> mode_;

	};
}

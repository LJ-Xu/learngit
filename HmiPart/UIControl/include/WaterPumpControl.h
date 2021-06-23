/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : WaterPumpControl.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 水泵控件控制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "WaterPumpModel.h"
#include <iostream>
#include <cstdio>

using namespace std;

namespace UI
{
	class WaterPumpControl : public BaseControl
	{
	public:
		WaterPumpControl() : WaterPumpControl(nullptr) { }
		WaterPumpControl(HMIPage *);
		~WaterPumpControl();

	public:
		void CreateView();
		void OnReady() override;
		void HandleSysChange(SysChangeEM catogray);

	protected:
		int PeekHMIMessage(Message::Msg * msg) override;
		void HandleDataVar(Project::DataVarId &varId);

	private:
		shared_ptr<WaterPumpModel> model_;
	public:
		int FluidSpeedMarkWidth;//流标宽度像素数
	};

}


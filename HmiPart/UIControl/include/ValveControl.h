/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveControl.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件控制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "ValveModel.h"
#include <iostream>
#include <cstdio>

using namespace std;

namespace UI
{
	class ValveControl : public BaseControl
	{
	public:
		ValveControl() : ValveControl(nullptr) { }
		ValveControl(HMIPage *);
		~ValveControl();

	public:
		void CreateView() override;
		void OnReady() override;
		bool HandleSysChange(SysChangeEM catogray);

	protected:
		int PeekHMIMessage(Message::Msg * msg) override;
		void HandleDataVar(Project::DataVarId &varId);

	private:
		shared_ptr<ValveModel> model_;
	public:
		int FluidSpeedMarkWidth;//流标宽度像素数
	};
}


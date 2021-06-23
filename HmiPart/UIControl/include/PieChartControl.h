/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerControl.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 饼图控件控制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "PieChartModel.h"
#include <iostream>
#include <cstdio>

using namespace std;

namespace UI
{
	class PieChartControl : public BaseControl
	{
	public:
		PieChartControl() : PieChartControl(nullptr){ }
		PieChartControl(HMIPage*);
		~PieChartControl();

	public:
		void CreateView();
		void OnReady() override;
		float GetPieValue(int index);

		void HandleSysChange(SysChangeEM catogray);
	protected:
		int PeekHMIMessage(Message::Msg * msg) override;

	private:
		shared_ptr<PieChartModel> model_;
	};
}


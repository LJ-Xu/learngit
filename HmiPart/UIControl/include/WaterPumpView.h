/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : WaterPumpView.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 水泵控件绘制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>

using namespace std;
#define MARKCOUNT 10//流速位标记数

namespace UI
{
	class WaterPumpControl;
	class WaterPumpModel;

	class WaterPumpView : public HMIBaseView
	{
	public:
		WaterPumpView(int, int, int, int);
		~WaterPumpView();

	protected:
		void draw() override;

	public :
		int FinX;
		int FinY;
		BasicView BaseView;
		int CurrentSpeendMark;
		int MarkCount = MARKCOUNT;
	};
}

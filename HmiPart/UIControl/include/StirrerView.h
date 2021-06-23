/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerView.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件绘制头文件
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

namespace UI
{
	class StirrerControl;
	class StirrerModel;

	class StirrerView : public HMIBaseLabel
	{
	public:
		StirrerView(int, int, int, int);
		~StirrerView();

	protected:
		void draw() override;

	public:
		BasicView BaseView;
		int ResIndex = 0;
	};
}

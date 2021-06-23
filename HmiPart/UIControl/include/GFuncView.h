/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncControl.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件绘制类
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
	class GFuncModel;
	class GFuncControl;

	class GFuncView : public HMIBaseView
	{
	public:
		GFuncView(int, int, int, int);
		~GFuncView();

	protected:
		void draw() override;

	public:
		BasicView BaseView;
	};
}
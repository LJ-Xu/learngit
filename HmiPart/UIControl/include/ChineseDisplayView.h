/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnView.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 关于中文显示的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalInputView.h"

namespace UI
{
	class ChineseDisplayView : public InputView
	{
	public:
		ChineseDisplayView(int X, int Y, int W, int H, const char* l);
		~ChineseDisplayView();

		BasicView BaseView;
		string DisplayValue;

		/*重写Fl_Input中关于事件响应处理函数*/
		int handle(int event);
	protected:
		void draw() override;
	};
}
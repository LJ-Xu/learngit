/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符显示框的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalDisplayView.h"

namespace UI
{
	class CharacterDisplayView : public InputView
	{
	public:
		CharacterDisplayView(int X, int Y, int W, int H, const char* l);
		~CharacterDisplayView();

		BasicView BaseView;
		string DisplayValue;

		/*******************************************************************************
		 * Name     : handle
		 * Descript : 重写Fl_Input中关于事件响应处理函数
		 * Input    : event	- 事件标号
		 * Output   : 1 - 处理事件成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
	protected:
		void draw() override;
	};
}

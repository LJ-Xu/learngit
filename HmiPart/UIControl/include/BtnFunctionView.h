/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionView.h
 * Author   : qiaodan
 * Date     : 2020-10-28
 * Descript : 关于功能按键的绘制头文件，绘制键盘按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "BtnFunctionModel.h"
#include "BtnFunctionControl.h"

using namespace std;
namespace UI
{
	enum FunctionAct { PRESS, RELEASE };
	class BtnFunctionView : public HMIBaseButton
	{
	public:
		BtnFunctionView(int X, int Y, int W, int H, const char *L);
		~BtnFunctionView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : 用于处理按键响应事件
		 * Input    : event - 事件类型
		 * Output   : 1	- 处理成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		BasicView BaseView;
		bool IsReleased = true;
		bool HandleOperatePush();
		bool HaveOperateLimit = false;
		int CurrentStatus = 0;

	protected:
		void draw() override;

	};
}



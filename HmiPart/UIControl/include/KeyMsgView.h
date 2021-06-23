/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于按键消息的绘制头文件，绘制键盘按键消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <BasicView.h>
#include "KeyMsgModel.h"

namespace UI
{
	class KeyMsgView : public HMIBaseView
	{
	public:
		KeyMsgView(int X, int Y, int W, int H, const char *L);
		~KeyMsgView();
		BasicView BaseView;

	protected:
		void draw() {};

	};
}

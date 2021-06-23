/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 关于按键的绘制头文件，绘制键盘按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "KeypadModel.h"
#include "KeypadControl.h"

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	//void BtnshortON_cb(void *t);

	class KeypadView : public HMIBaseButton
	{
		/*******************************************************************************
		 * Name     : time_cb
		 * Descript : 处理按键响应事件回调函数
		 * Input    : t - 可传递数据内容
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
	public:
		KeypadView(int X, int Y, int W, int H, const char *L);
		~KeypadView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : 用于处理按键响应事件
		 * Input    : event - 事件类型
		 * Output   : 1	- 处理成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		/*******************************************************************************
		 * Name     : HandleClick
		 * Descript : 用于处理点击后根据指示灯按键设置的行为来设置当前状态
		 * Input    :
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void HandleClick();
		BasicView BaseView;
		//bool HasShortOnEvent = false;
		bool IsReleased = true;
		bool HaveOperateLimit = false;
		bool HandleOperatePush();
		int CurrentStatus = 0;
	protected:
		void draw() override;

	private:
		//high_resolution_clock::time_point beginTime_;	//记录按键按下时间
		//high_resolution_clock::time_point endTime_;		//记录按键松开时间

	};
}

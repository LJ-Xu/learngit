/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 关于多状态按键的绘制头文件，绘制多状态按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "MultiStatusKeypadModel.h"
#include "MultiStatusKeypadControl.h"
using namespace std;
namespace UI
{
	class MultiStatusKeypadView : public HMIBaseButton
	{
	public:
		MultiStatusKeypadView(int X, int Y, int W, int H, const char *L);
		~MultiStatusKeypadView();
		BasicView BaseView;

		/*事件处理函数*/
		int handle(int event);

		/*用于处理点击后根据指示灯按键设置的行为来设置当前状态*/
		void HandleClick();
		bool HandleOperatePush();

		/*点击按键后，设置当前状态执行的动作*/
		void SetStatusAction();
		int CurrentStatus = 0;
		//int CurrentBtnStatus = 0;
		bool IsReleased = true;
		bool IsAchieveSafeTime = false;
	protected:
		void draw() override;

	};

}


#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmWinView.h
 * Author   : qiaodan
 * Date     : 2020-11-10
 * Descript : 关于报警弹窗的绘制头文件，绘制报警弹窗
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "AlarmWinModel.h"
#include "AlarmWinControl.h"
#include "AlarmInfoRes.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	class AlarmWinView : public HMIBaseWindow
	{
	public:
		AlarmWinView(int X, int Y, int W, int H, const char *L);
		~AlarmWinView();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : 用于处理按键响应事件
		 * Input    : event - 事件类型
		 * Output   : 1	- 处理成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		BasicView BaseView;
		Project::AlarmContent AlarmInfo;
		bool GetCloseMode(int &mode,int &time);
		
	protected:
		void draw() override;

	private:
		high_resolution_clock::time_point beginTime_;	//记录按键按下时间
		high_resolution_clock::time_point endTime_;		//记录按键松开时间

	};
}

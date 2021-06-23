/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveModel.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件绘制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Button.H>
#include <FL/Fl.H>
//#include <chrono>

using namespace std;
#define MARKCOUNT 10//流速位标记数
//typedef std::chrono::high_resolution_clock::time_point TimePoint;
//typedef std::chrono::milliseconds MilliSecs;

namespace UI 
{
	class ValveControl;
	class ValveModel;
	//void ShortON_CB(void * data);

	class ValveView : public HMIBaseButton 
	{
	public:
		ValveView(int, int, int, int, const char*);
		~ValveView();

	public:
		int handle(int event);
		void HandleClick();
		bool HandleOperatePush();

	protected:
		void draw() override;

	public:
		int FinX;
		int FinY;
		BasicView BaseView;
		bool IsReleased;
		bool HaveOperateLimit = false;
		int CurrentSpeendMark;
		int MarkCount = MARKCOUNT;
	};
}

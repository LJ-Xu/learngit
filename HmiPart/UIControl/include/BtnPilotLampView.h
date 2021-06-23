/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于指示灯按键的绘制头文件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
#include "BtnPilotLampModel.h"
using namespace std;
namespace UI
{
	class BtnPilotLampView : public HMIBaseButton
	{
	public:
		BtnPilotLampView(int X, int Y, int W, int H, const char *L);
		~BtnPilotLampView();
		BasicView BaseView;

	    /*******************************************************************************
		 * Name     : handle
		 * Descript : 用于处理按键响应事件
		 * Input    : event - 事件类型
		 * Output   : 1	- 处理成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		/*根据按键动作模式，处理按键*/
		void HandleClick();
		int CurrentStatus = 0;		//0:OFF状态	1:ON状态
		int CurrentFlash = 1;		//0:隐藏	1:显示
		bool IsReleased = true;
		bool HaveOperateLimit = false;
		bool HandleOperatePush();
	protected:
		void draw() override;
	};

}

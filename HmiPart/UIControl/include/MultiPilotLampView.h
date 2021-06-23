/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 关于多状态指示灯的绘制头文件，绘制多状态指示灯
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <stdafx.h>
#include <FL/Fl_Box.H>
#include <BasicView.h>
#include "MultiPilotLampModel.h"
namespace UI
{
	class MultiPilotLampView : public HMIBaseLabel
	{
	public:
		MultiPilotLampView(int X, int Y, int W, int H);
		~MultiPilotLampView();
		BasicView BaseView;
		int CurrentStatus = 0;		//0:OFF状态	1:ON状态
		int CurrentFlash = 1;		//0:隐藏	1:显示
	protected:
		void draw() override;
	};

}


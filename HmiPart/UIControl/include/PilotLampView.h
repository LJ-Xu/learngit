/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampView.h
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 关于指示灯的绘制头文件，绘制指示灯
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "stdafx.h"
#include <BasicView.h>
#include <FL/Fl_Box.H>
#include "PilotLampModel.h"
namespace UI
{
	class PilotLampView : public HMIBaseLabel
	{
	public:
		PilotLampView(int X, int Y, int W, int H);
		~PilotLampView();
		BasicView BaseView;
		int CurrentStatus = 0;		//0:OFF状态	1:ON状态
		int CurrentFlash = 1;		//0:隐藏	1:显示
	protected:
		void draw() override;
	};
}


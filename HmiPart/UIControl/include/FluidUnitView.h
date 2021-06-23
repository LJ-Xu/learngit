/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于流体绘制头文件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <Point.h>
namespace UI
{
	class FluidUnitView : public HMIBaseView
	{
	public:
		FluidUnitView(int X, int Y, int W, int H);
		~FluidUnitView();
		BasicView BaseView;
		void InitDraw();
		int SliderOffx = 0;
		int SliderOffy = 0;

		vector<Project::Point> Coordinates;
		Fl_Color Slidercolor;

	protected:
		void draw() override;
	private:
		int pointnum_;
		int borderwidth_;
		Fl_Color bordercolor_;
		int pipewidth_;
		Fl_Color pipecolor_;
		int sliderwidth_;
		int sliderinterval_;
		int sliderlength_;
		int arrowangle_;
	};

}
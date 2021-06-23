/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PieChartView.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 饼图控件绘制头文件
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "BasicView.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Chart.H>
#include "PieChartConfig.h"


namespace UI
{
	class PieChartView : public HMIBaseLabel
	{
	public:
		PieChartView(int, int, int, int);
		~PieChartView();
		
		void InitView(void* p);
	private:
		void DrawPie(Project::PieChartChannelStyle& style, Rectangle& rect, int dir, Project::StringStyle& textStyle);

	protected:
		void draw() override;

	public:
		BasicView BaseView;
		std::vector<int> ValueSnapList;
		int TotalValue;
		std::vector<std::pair<int, int>> TxtPosList;
	};
}



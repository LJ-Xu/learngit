/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于折线的绘制头文件，绘制折线
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "BasicView.h"

namespace UI
{
	class GPolyLineView : public HMIBaseView
	{
	public:
		GPolyLineView(int X, int Y, int W, int H);
		~GPolyLineView();
		BasicView BaseView;

	protected:
		void draw() override;
		int handle(int event);
		void DrawFill();
		double GetArrowAngle(int startx, int endx, int starty, int endy, int mode);
	};
}

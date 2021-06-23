/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleView.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于矩形的绘制头文件，绘制矩形
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
	class GRectangleView : public HMIBaseView
	{
	public:
		GRectangleView(int X, int Y, int W, int H);
		~GRectangleView();
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		void DrawFill();
	private:
		int drawRectRadius;//绘制的直角尺寸
	};
}


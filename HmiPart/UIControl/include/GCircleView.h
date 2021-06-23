/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆的绘制头文件，绘制圆
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "BasicView.h"
#include "GCircleControl.h"
#include "GCircleModel.h"

namespace UI
{
	class GCircleView : public HMIBaseView
	{
	public:
		GCircleView(int X, int Y, int W, int H);
		~GCircleView();
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		void DrawFill();
	};
}
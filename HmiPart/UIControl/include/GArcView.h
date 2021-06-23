/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcView.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆弧的绘制头文件，绘制圆弧
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
	class GArcView : public HMIBaseView
	{
	public:
		GArcView(int X, int Y, int W, int H);
		~GArcView();
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		void DrawFill();
	};


}

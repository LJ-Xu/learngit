/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RegionFunctionView.cpp
 * Author   : qiaodan
 * Date     : 2020-11-2
 * Descript : ªÊ÷∆π¶ƒ‹”Ú
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "RegionFunctionView.h"
namespace UI
{
	RegionFunctionView::RegionFunctionView(int X, int Y, int W, int H, const char *L) : HMIBaseLabel(X, Y, W, H, L)
	{
		box(FL_NO_BOX);
	}
	RegionFunctionView::~RegionFunctionView() {}

	void RegionFunctionView::draw()
	{
		shared_ptr<RegionFunctionModel> model = BaseView.GetModel<RegionFunctionModel>();
		fl_font(0, 14);
		fl_color(FL_BLACK);
		draw_box();
		fl_draw(Text.c_str(), model->FuncRegionConfig.X + model->FuncRegionConfig.OffX,
			model->FuncRegionConfig.Y + model->FuncRegionConfig.OffY,
			model->FuncRegionConfig.Width, model->FuncRegionConfig.Height, FL_ALIGN_CENTER);
	}
}

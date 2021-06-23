/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerView.cpp
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 绘制定时器
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "TimerView.h"
#include "TimerModel.h"
#include "CodeFormatUtility.h"
#include "IResourceService.h"
namespace UI
{
	TimerView::TimerView(int X, int Y, int W, int H, const char *L) : HMIBaseLabel(X, Y, W, H, L)
	{
		box(FL_NO_BOX);
	}
	TimerView::~TimerView() {}

	void TimerView::draw()
	{
		shared_ptr<TimerModel> model = BaseView.GetModel<TimerModel>();
		if (UI::IResourceService::Ins()->IsRenderMode())
		{
			fl_font(0, 14);
			fl_color(FL_BLACK);
			draw_box();
			string text = "定时器";
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);
			fl_draw(text.c_str(), model->TimerConfig.X + model->TimerConfig.OffX,
				model->TimerConfig.Y + model->TimerConfig.OffY,
				model->TimerConfig.Width, model->TimerConfig.Height, FL_ALIGN_CENTER);
		}
	}
}

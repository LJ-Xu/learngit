/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 绘制中文按键文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "ChineseBtnView.h"
#include "ChineseBtnControl.h"
#include "ChineseBtnModel.h"
#include "ResourceService.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <iostream>
#include <chrono>
#include <FL/fl_draw.H>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
namespace UI
{
	ChineseBtnView::ChineseBtnView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	ChineseBtnView::~ChineseBtnView() {}
	
	//void ChineseBtn_cb(void *data)
	//{
	//	ChineseBtnView * view = (ChineseBtnView *)data;
	//	view->achieveTime = true;
	//}
	int ChineseBtnView::handle(int event)
	{
		shared_ptr<ChineseBtnModel> model = BaseView.GetModel<ChineseBtnModel>();
		shared_ptr<ChineseBtnControl> ctrl = BaseView.GetControl<ChineseBtnControl>();

		switch (event)
		{
		case FL_PUSH:
			//Fl::add_timeout(2, count_cb, (void*)this);
			beginTime = high_resolution_clock::now();
			return HMIBaseButton::handle(event);
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				endTime = high_resolution_clock::now();
				milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
				//Fl::remove_timeout(count_cb, NULL);
				if (timeInterval.count() > model->BtnConfig.AcceptTime)
				{
					if (model->BtnConfig.text == "@4->")
						ctrl->SendMsg(BTN_PREV, 0);
					else if (model->BtnConfig.text == "@6->")
						ctrl->SendMsg(BTN_NEXT, 1);
					else
						ctrl->SendMsg(BTN_VALUE, 2);
					achieveTime = false;
				}
			}
		default:
			return HMIBaseButton::handle(event);
		}
	}
	void ChineseBtnView::draw()
	{
		shared_ptr<ChineseBtnModel> model = BaseView.GetModel<ChineseBtnModel>();
		shared_ptr<ChineseBtnControl> ctrl = BaseView.GetControl<ChineseBtnControl>();

		int fontstyle = UI::IResourceService::GetFontIdx(model->BtnConfig.font.Name);
		labelfont(fontstyle);
		labelsize(24);
		label(model->BtnConfig.text.c_str());
		//labelcolor((model->m_config.font.color));
		//fl_color(RGBColor(model->m_config.font.color));
		HMIBaseButton::draw();
		/*绘制文本*/
		//fl_draw(model->BtnConfig.text.data(), model->BtnConfig.X, model->BtnConfig.Y,
		//		model->BtnConfig.Width, model->BtnConfig.Height, 0);	//FL_ALIGN_CENTER
		//color(model->m_config.backColor);
	}
}

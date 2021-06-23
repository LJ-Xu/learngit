/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : WaterPumpView.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 水泵控件绘制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "WaterPumpView.h"
#include "WaterPumpModel.h"
#include "WaterPumpControl.h"
#include "ResourceService.h"

namespace UI
{
	WaterPumpView::WaterPumpView(int x, int y, int w, int h)
		: HMIBaseView(x, y, w, h) {

	}

	WaterPumpView::~WaterPumpView() {

	}

	// 绘制控件
	void WaterPumpView::draw() {
		shared_ptr<WaterPumpModel> model = BaseView.GetModel<WaterPumpModel>();
		shared_ptr<WaterPumpControl> ctl = BaseView.GetControl<WaterPumpControl>();
		FinX = model->WaterPumpUnit.X + model->WaterPumpUnit.OffX;
		FinY = model->WaterPumpUnit.Y + model->WaterPumpUnit.OffY;
		// 获取图片信息
		//vector<Project::ImageResId> model->WaterPumpUnit.Keys = model->WaterPumpUnit.Keys;
		// 获取当前状态资源图片
		unique_ptr<Project::ImageResId> key;
		IResourceService::Ins()->SetRenderStatus(model->WaterPumpUnit.CurrentStatus, model->WaterPumpUnit.Keys.size());
		
		switch (model->WaterPumpUnit.CurrentStatus)
		{
		case 0:									// Close
			if (model->WaterPumpUnit.Keys.size() > 0) {
				key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[0]));
			}
			break;
		case 1:									// Open
			if (model->WaterPumpUnit.Keys.size() > 1) {
				key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[1]));
			}
			break;
		default:
			break;
		}
		
		//key.reset(new Project::ImageResId(model->WaterPumpUnit.Keys[model->WaterPumpUnit.CurrentStatus]));
		if (!key) {
			return;
		}
		// 获取当前图片
		Fl_Image * img = IResourceService::Ins()->GetImage(key->KeyVal);
		if (img) {
			// 设置控件图片
			image(img);
			// 设置图片为背景
			align(FL_ALIGN_IMAGE_BACKDROP);
			// 设置绘制样式
			box(FL_NO_BOX);
		}
		// 绘制背景图片
		draw_box();
		//流动状态
		if (model->WaterPumpUnit.CurrentStatus)
		{
			//表现流速
			//坐标起点
			int curmk = model->WaterPumpUnit.Dir ? (MarkCount - CurrentSpeendMark) : CurrentSpeendMark;
			int mkx = FinX + model->WaterPumpUnit.InnerRectX + model->WaterPumpUnit.InnerRectW / MarkCount * curmk;
			int mky = FinY + model->WaterPumpUnit.InnerRectY;
			fl_color(FL_WHITE);
			fl_rectf(mkx,
				mky,
				ctl->FluidSpeedMarkWidth,
				model->WaterPumpUnit.InnerRectH);
		}
	}
}

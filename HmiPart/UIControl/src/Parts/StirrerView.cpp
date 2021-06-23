/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerView.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件绘制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StirrerView.h"
#include "StirrerModel.h"
#include "StirrerControl.h"
#include "ResourceService.h"

namespace UI
{
	StirrerView::StirrerView(int x, int y, int w, int h)
		: HMIBaseLabel(x, y, w, h) {

	}

	StirrerView::~StirrerView() {

	}
	// 绘制控件
	void StirrerView::draw() {
		shared_ptr<StirrerModel> model = BaseView.GetModel<StirrerModel>();
		// 获取图片信息
		//vector<Project::ImageResId> resIds = model->StirrerUnit.Keys;
		// 获取当前状态资源图片
		unique_ptr<Project::ImageResId> key;
		IResourceService::Ins()->SetRenderStatus(ResIndex, model->StirrerUnit.Keys.size());
		switch (ResIndex)
		{
		case 0:
			if (model->StirrerUnit.Keys.size() > 0) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[0]));
			}
			break;
		case 1:
			if (model->StirrerUnit.Keys.size() > 1) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[1]));
			}
			break;
		case 2:
			if (model->StirrerUnit.Keys.size() > 2) {
				key.reset(new Project::ImageResId(model->StirrerUnit.Keys[2]));
			}
			break;
		default:
			break;
		}
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
		Fl_Box::draw();
	}
}

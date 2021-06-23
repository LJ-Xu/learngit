/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : ªÊ÷∆≤‚ ‘∞¥º¸
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ResourceService.h"
#include "UIComm.h"
#include "TestBitVarView.h"
#include "TestBitVarControl.h"
#include "TestBitVarModel.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>
#include "UIData.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
namespace UI
{
	TestBitVarView::TestBitVarView(int X, int Y, int W, int H, const char* L = 0) : HMIBaseButton(X, Y, W, H, L)
	{

	}
	TestBitVarView::~TestBitVarView() {}
	int TestBitVarView::handle(int event)
	{
		shared_ptr<TestBitVarModel> model = BaseView.GetModel<TestBitVarModel>();

		switch (event)
		{
		case FL_PUSH:
			if (model->BitConfig.Type == Project::VarType::BitVar)
			{
				UI::UIData::Bit(model->BitConfig.VarId, model->BitConfig.BitValue);
				if(model->BitConfig.BitValue)
					color(FL_BLUE);
				else
					color(FL_RED);
				model->BitConfig.BitValue = !model->BitConfig.BitValue;
			}
			return HMIBaseButton::handle(event);
		//case FL_RELEASE:
		//	if (model->BitConfig.Type == Project::VarType::BitVar)
		//		UI::UIData::Bit(model->BitConfig.VarId, false);
		//	return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}
	
	void TestBitVarView::draw()
	{
		shared_ptr<TestBitVarModel> model = BaseView.GetModel<TestBitVarModel>();

		if (!model->BitConfig.BGKey.empty())
		{

			Fl_Image* img = IResourceService::Ins()->GetImage(model->BitConfig.BGKey);
			image(img);
			align(FL_ALIGN_IMAGE_BACKDROP);
			fl_color(FL_RED);
		}
		label(model->BitConfig.Name.c_str());
		HMIBaseButton::draw();
	}
}

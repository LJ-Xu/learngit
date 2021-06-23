/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseDisplayView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 绘制中文显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseDisplayView.h"
#include "ChineseDisplayControl.h"
#include "ChineseDisplayModel.h"
#include <FL/Fl_Input_.H>
#include "ResourceService.h"
#include "KeyMsgControl.h"
#include "Message.h"
namespace UI
{
	ChineseDisplayView::ChineseDisplayView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	ChineseDisplayView::~ChineseDisplayView() {}

	void ChineseDisplayView::draw()
	{
		shared_ptr<ChineseDisplayModel> model = BaseView.GetModel<ChineseDisplayModel>();
		/*绘制框体*/
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->DisChineseConfig.PicKey.KeyVal);
		if (!rgbImage) 		
			LOG_INFO_("Chinese Display Pic is NULL\n");
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}

		Fl_Boxtype b = box();
		if (damage() & FL_DAMAGE_ALL) {
			draw_box();
		}

		value(DisplayValue.c_str());
		textfont(UI::IResourceService::GetFontIdx(model->DisChineseConfig.TextStyle.Font.Name));
		textsize(model->DisChineseConfig.TextStyle.Font.Size);
		Fl_Color tcolor = fl_rgb_color(RGBColor(model->DisChineseConfig.TextStyle.Colors));
		textcolor(tcolor);
		
		drawtext(model->DisChineseConfig.X + model->DisChineseConfig.OffX,
			model->DisChineseConfig.Y + model->DisChineseConfig.OffY,
			model->DisChineseConfig.Width, model->DisChineseConfig.Height,
			model->DisChineseConfig.TextStyle.Align, rgbImage,model->DisChineseConfig.PswMode);
	}

	int ChineseDisplayView::handle(int event)
	{
		shared_ptr<ChineseDisplayModel> model = BaseView.GetModel<ChineseDisplayModel>();
		shared_ptr<ChineseDisplayControl> ctrl = BaseView.GetControl<ChineseDisplayControl>();
		return 1;
	}
}

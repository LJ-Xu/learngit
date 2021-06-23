/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterDisplayView.h"
#include "CharacterDisplayControl.h"
#include "CharacterDisplayModel.h"
#include "ResourceService.h"
#include "Message.h"
namespace UI
{
	CharacterDisplayView::CharacterDisplayView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	CharacterDisplayView::~CharacterDisplayView() {}

	void CharacterDisplayView::draw()
	{
		shared_ptr<CharacterDisplayModel> model = BaseView.GetModel<CharacterDisplayModel>();
		//绘制框体
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->DisStrConfig.Key.KeyVal);
		if (!rgbImage) 
			LOG_INFO_("Character Display Pic is NULL\n");
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		/*获取数据*/
		//string codestring = CodeFormatUtility::CodeConversion(DisplayValue, model->DisStrConfig.Rule);
		string codestring = DisplayValue;
		if (model->DisStrConfig.ByteConversion == 1 
			&& model->DisStrConfig.Rule != CodeFormatUtility::UTF_16)		//高低字节转换
			codestring = CodeFormatUtility::EndianSwap(DisplayValue);
		value(codestring.c_str());
		Fl_Boxtype b = box();
		if (damage() & FL_DAMAGE_ALL) {
			draw_box();
		}
		/*设置字体*/
		textfont(UI::IResourceService::GetFontIdx(model->DisStrConfig.TextStyle.Font.Name));
		textsize(model->DisStrConfig.TextStyle.Font.Size);
		/*设置字体颜色和文字*/
		Fl_Color tcolor = fl_rgb_color(RGBColor(model->DisStrConfig.TextStyle.Colors));
		textcolor(tcolor);
		drawtext(model->DisStrConfig.X + model->DisStrConfig.OffX,
			model->DisStrConfig.Y + model->DisStrConfig.OffY,
			model->DisStrConfig.Width, model->DisStrConfig.Height,
			model->DisStrConfig.TextStyle.Align, rgbImage, model->DisStrConfig.PswMode);
	}

	int CharacterDisplayView::handle(int event)
	{
		shared_ptr<CharacterDisplayModel> model = BaseView.GetModel<CharacterDisplayModel>();
		shared_ptr<CharacterDisplayControl> ctrl = BaseView.GetControl<CharacterDisplayControl>();
		return 1;
	}
}

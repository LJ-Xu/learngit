/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 绘制拼音界面
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "PinYinPageControl.h"
#include "PinYinPageView.h"
#include "PinYinPageModel.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iomanip>
#include "UIDataService.h"
#include "IResourceService.h"
#include "LocalData.h"
#include "UIData.h"
#include "HMIWindow.h"
#include "SysSetApi.h"
namespace UI
{
	PinYinPageView *PinYinPageView::staticview_ = nullptr;
	void PinYinPageView::SetIns(PinYinPageView *view)
	{
		staticview_ = view;
	}
	PinYinPageView *PinYinPageView::Ins()
	{
		return staticview_;
	}

	PinYinPageView::PinYinPageView(int X, int Y, int W, int H, const char* L) : HMIBaseGroup(X, Y, W, H, L) {
		Fl_Group::end();		// end the group's begin()
	}
	PinYinPageView::~PinYinPageView() {}
	int PinYinPageView::handle(int event)
	{
		shared_ptr<PinYinPageModel> model = BaseView.GetModel<PinYinPageModel>();
		shared_ptr<PinYinPageControl> ctrl = BaseView.GetControl<PinYinPageControl>();
		switch (event)
		{
		case FL_PUSH:
		{
			SysSetApi::TriggerBeep();
			int linex = Fl::event_x();
			int liney = Fl::event_y();
			if (linex >= model->PinYinConfig.X + model->PinYinConfig.OffX &&
				linex <= model->PinYinConfig.X + model->PinYinConfig.OffX + pinyinwidth_ &&
				liney >= model->PinYinConfig.Y + model->PinYinConfig.OffY &&
				liney <= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2)
			{
				if (!ChineseString.empty())
				{
					ctrl->Page()->Win()->SendKey(0 + '1');

					int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
					char *inputchar = new char[len + 1];
					memset(inputchar, '\0', len + 1);
					LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
					string inputstring = inputchar;
					inputstring += ChineseString[0];
					char *data = new char[len + 1];
					memset(data, '\0', len + 1);
					memcpy(data, inputstring.c_str(), inputstring.size() > len ? len : inputstring.size());
					LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
					delete[] data;
					delete[] inputchar;
				}
				//hide();
				redraw();
				return 1;
			}
			int btnwidth = model->PinYinConfig.Width / 9;
			//前一页
			if (linex >= model->PinYinConfig.X + model->PinYinConfig.OffX &&
				linex <= model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth &&
				liney >= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2 &&
				liney <= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height)
			{
				if(EnablePreBtn)
					ctrl->SearchChineseCharacters(-1, PinYin);
				redraw();
				return 1;
			}
			//后一页
			if (linex >= model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * 8 &&
				linex <= model->PinYinConfig.X + model->PinYinConfig.OffX + model->PinYinConfig.Width &&
				liney >= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2 &&
				liney <= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height)
			{
				if(EnableNextBtn)
					ctrl->SearchChineseCharacters(1, PinYin);
				redraw();
				return 1;
			}
			for (int i = 0; i < (int)ChineseString.size(); i++)
			{
				if (linex >= model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * (i + 1) &&
					linex <= model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * (i + 2) &&
					liney >= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2 &&
					liney <= model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height)
				{
					int len = LocalData::GetLocalVarLen(SYS_PSW_INPUTKEY_CURVAL);
					char *inputchar = new char[len + 1];
					memset(inputchar, '\0', len + 1);
					LocalData::GetBytes(SYS_PSW_INPUTKEY_CURVAL, inputchar);
					string inputstring = inputchar;
					inputstring += ChineseString[i];
					char *data = new char[len + 1];
					memset(data, '\0', len + 1);
					memcpy(data, inputstring.c_str(), inputstring.size() > len ? len : inputstring.size());
					LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, data);
					delete[] data;
					delete[] inputchar;
					//LocalData::SetString(SYS_PSW_INPUTKEY_CURVAL, inputstring.c_str());
					/*发送消息告知以选择文字*/
					ctrl->Page()->Win()->SendKey(i + '1');
					//hide();
					redraw();
					return 1;
				}
			}
			return 1;
			break;
		}
		default:
			return Fl_Group::handle(event);
		}
	}
	void PinYinPageView::draw()
	{
		shared_ptr<PinYinPageModel> model = BaseView.GetModel<PinYinPageModel>();
		shared_ptr<PinYinPageControl> ctrl = BaseView.GetControl<PinYinPageControl>();
		//绘制框体
		draw_box((Fl_Boxtype)1, model->PinYinConfig.X + model->PinYinConfig.OffX,
			model->PinYinConfig.Y + model->PinYinConfig.OffY,
			model->PinYinConfig.Width, model->PinYinConfig.Height, FL_BACKGROUND2_COLOR);

		/*设置字体和大小*/
		int fontSize = model->PinYinConfig.TextStyle.Font.Size;
		//int fontStyle = UI::IResourceService::GetFontIdx(model->PinYinConfig.TextStyle.Font.Name);
		int fontStyle = UI::IResourceService::GetFontIdx("xxx");
		fl_font(fontStyle, fontSize);
		
		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->PinYinConfig.TextStyle.Colors));

		//绘制拼音框
		/*获取拼音框体宽度*/
		pinyinwidth_ = (int)fl_width(PinYin.c_str(), (int)PinYin.size()) + 10;
		draw_box((Fl_Boxtype)FL_UP_BOX, model->PinYinConfig.X + model->PinYinConfig.OffX,
			model->PinYinConfig.Y + model->PinYinConfig.OffY,
			pinyinwidth_, model->PinYinConfig.Height / 2, FL_GRAY);
		/*绘制文本*/
		fl_color(active() ? textcolor : fl_inactive(textcolor));
		fl_draw(PinYin.data(), model->PinYinConfig.X + model->PinYinConfig.OffX,
			model->PinYinConfig.Y + model->PinYinConfig.OffY,
			pinyinwidth_, model->PinYinConfig.Height / 2,
			model->PinYinConfig.TextStyle.Align);	//FL_ALIGN_CENTER
		//绘制按键框
		/*绘制前后翻页按键*/
		int btnwidth = model->PinYinConfig.Width / 9;
		draw_box((Fl_Boxtype)FL_UP_BOX, model->PinYinConfig.X + model->PinYinConfig.OffX,
			model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
			btnwidth, model->PinYinConfig.Height / 2, FL_GRAY);
		fl_color(EnablePreBtn ? textcolor : fl_inactive(textcolor));
		fl_draw("@4->", model->PinYinConfig.X + model->PinYinConfig.OffX,
			model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
			btnwidth, model->PinYinConfig.Height / 2,
			model->PinYinConfig.TextStyle.Align);	//FL_ALIGN_CENTER

		draw_box((Fl_Boxtype)FL_UP_BOX, model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * 8,
			model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
			btnwidth, model->PinYinConfig.Height / 2, FL_GRAY);
		fl_color(EnableNextBtn ? textcolor : fl_inactive(textcolor));
		fl_draw("@6->", model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * 8,
			model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
			btnwidth, model->PinYinConfig.Height / 2,
			model->PinYinConfig.TextStyle.Align);	//FL_ALIGN_CENTER

		/*绘制中间中文选择按键*/
		for (size_t i = 0; i < ChineseString.size(); i++)
		{
			draw_box((Fl_Boxtype)FL_UP_BOX, model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * (i + 1),
				model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
				btnwidth, model->PinYinConfig.Height / 2, FL_GRAY);
			fl_color(active() ? textcolor : fl_inactive(textcolor));
			string text = std::to_string(i + 1) + '.' + ChineseString[i];
			fl_draw(text.c_str(), model->PinYinConfig.X + model->PinYinConfig.OffX + btnwidth * (i + 1),
				model->PinYinConfig.Y + model->PinYinConfig.OffY + model->PinYinConfig.Height / 2,
				btnwidth, model->PinYinConfig.Height / 2,
				model->PinYinConfig.TextStyle.Align);	//FL_ALIGN_CENTER
		}
	}

}

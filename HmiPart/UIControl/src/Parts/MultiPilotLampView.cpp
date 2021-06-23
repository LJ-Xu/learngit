/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 绘制多状态指示灯
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "UIComm.h"
#include "MultiPilotLampView.h"
#include "MultiPilotLampControl.h"
#include "MultiPilotLampModel.h"
#include "StringUtility.h"
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "Message.h"
namespace UI
{
	MultiPilotLampView::MultiPilotLampView(int X, int Y, int W, int H) :HMIBaseLabel(X, Y, W, H) {}
	MultiPilotLampView::~MultiPilotLampView() {}

	void MultiPilotLampView::draw()
	{
		shared_ptr<MultiPilotLampModel> model = BaseView.GetModel<MultiPilotLampModel>();
		//获取关于字体的数据
		if (CurrentFlash == 0)
			return;
		IResourceService::Ins()->SetRenderStatus(CurrentStatus, model->MPLampconfig.Action.size());
		if (CurrentStatus >= (int)model->MPLampconfig.Action.size() )
			return;
		if (CurrentStatus == -1)	//错误状态
		{
			Fl_Image *lampImage = IResourceService::Ins()->GetImage(model->MPLampconfig.ErrState.StatusKey.KeyVal);
			if (!lampImage) {
				LOG_INFO_("Multi Pilot Lamp ErrState Pic is NULL\n");
			}
			else {
				image(lampImage);
				align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
				box(FL_NO_BOX);
			}
			//绘制图片框体
			draw_box();
			//获取当前状态下文字
			string text = StringUtility::GetDrawString(IResourceService::Ins(),
				model->MPLampconfig.Text, model->MPLampconfig.StatusCnt);
			UI::IResourceService::GB2312toUtf8(text);
			/*设置字体样式及大小*/
			fl_font(UI::IResourceService::GetFontIdx(model->MPLampconfig.ErrState.Css.Font.Name),
				model->MPLampconfig.ErrState.Css.Font.Size);

			/*设置字体颜色*/
			Fl_Color textcolor = fl_rgb_color(RGBColor(model->MPLampconfig.ErrState.Css.Colors));
			fl_color(active() ? textcolor : fl_inactive(textcolor));
			/*绘制文本*/
			fl_draw(text.data(), model->MPLampconfig.X + model->MPLampconfig.OffX,
				model->MPLampconfig.Y + model->MPLampconfig.OffY,
				model->MPLampconfig.Width,
				model->MPLampconfig.Height,
				model->MPLampconfig.ErrState.Css.Align);	//FL_ALIGN_CENTER
		}
		else
		{
			Fl_Image *lampImage = IResourceService::Ins()->GetImage(model->MPLampconfig.Action[CurrentStatus].StatusKey.KeyVal);
			if (!lampImage) {
				LOG_INFO("Multi Pilot Lamp Stats%d Pic is NULL\n", CurrentStatus);
			}
			else {
				image(lampImage);
				align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
				box(FL_NO_BOX);
			}
			//绘制图片框体
			draw_box();
			//获取当前状态下文字
			string text = StringUtility::GetDrawString(IResourceService::Ins(),
				model->MPLampconfig.Text, CurrentStatus);
			UI::IResourceService::GB2312toUtf8(text);
			/*设置字体样式及大小*/
			fl_font(UI::IResourceService::GetFontIdx(model->MPLampconfig.Action[CurrentStatus].Css.Font.Name),
				model->MPLampconfig.Action[CurrentStatus].Css.Font.Size);

			/*设置字体颜色*/
			Fl_Color textcolor = fl_rgb_color(RGBColor(model->MPLampconfig.Action[CurrentStatus].Css.Colors));
			fl_color(active() ? textcolor : fl_inactive(textcolor));
			/*绘制文本*/
			fl_draw(text.data(), model->MPLampconfig.X + model->MPLampconfig.OffX,
				model->MPLampconfig.Y + model->MPLampconfig.OffY,
				model->MPLampconfig.Width,
				model->MPLampconfig.Height,
				model->MPLampconfig.Action[CurrentStatus].Css.Align);	//FL_ALIGN_CENTER
		}
	}
}

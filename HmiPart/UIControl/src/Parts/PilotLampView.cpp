/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 绘制指示灯
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "UIComm.h"
#include "PilotLampView.h"
#include "PilotLampControl.h"
#include "StringUtility.h"
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "Message.h"
namespace UI
{
	PilotLampView::PilotLampView(int X, int Y, int W, int H) :HMIBaseLabel(X, Y, W, H) {}
	PilotLampView::~PilotLampView() {}

	void PilotLampView::draw()
	{
		shared_ptr<PilotLampControl> ctrl = BaseView.GetControl<PilotLampControl>();
		shared_ptr<PilotLampModel> model = BaseView.GetModel<PilotLampModel>();
		//获取关于字体的数据
		if (CurrentFlash == 0)
			return;
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,
			model->PLampconfig.StatusKeys.size() < model->PLampconfig.Csses.size()
			? model->PLampconfig.StatusKeys.size() : model->PLampconfig.Csses.size());

		if ((size_t)CurrentStatus >= model->PLampconfig.StatusKeys.size() || (size_t)CurrentStatus >= model->PLampconfig.Csses.size())
			return;
		//获取当前状态图片key值
		//if (model->PLampconfig.StatusKeys.size() == 0 || model->PLampconfig.Csses.size() == 0)
		//	return;

		Fl_Image *lampImage = IResourceService::Ins()->GetImage(model->PLampconfig.StatusKeys[CurrentStatus].KeyVal);
		if (!lampImage) {
			LOG_INFO_("lampImage is NULL\n");
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
			model->PLampconfig.Text, CurrentStatus);
		UI::IResourceService::GB2312toUtf8(text);
		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->PLampconfig.Csses[CurrentStatus].Font.Name),
			model->PLampconfig.Csses[CurrentStatus].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->PLampconfig.Csses[CurrentStatus].Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));
		/*绘制文本*/
		fl_draw(text.data(), model->PLampconfig.X + model->PLampconfig.OffX,
			model->PLampconfig.Y + model->PLampconfig.OffY,
			model->PLampconfig.Width,
			model->PLampconfig.Height,
			model->PLampconfig.Csses[CurrentStatus].Align);	//FL_ALIGN_CENTER
	}
}

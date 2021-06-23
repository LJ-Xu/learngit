/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmWinView.cpp
 * Author   : qiaodan
 * Date     : 2020-11-10
 * Descript : 绘制报警窗口
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "AlarmWinView.h"
#include "ResourceService.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	AlarmWinView::AlarmWinView(int X, int Y, int W, int H, const char *L) : HMIBaseWindow(X, Y, W, H, L) {}
	AlarmWinView::~AlarmWinView() {}
	bool AlarmWinView::GetCloseMode(int &mode, int &time)
	{
		shared_ptr<AlarmWinModel> model = BaseView.GetModel<AlarmWinModel>();
		mode = model->WinAlarmConfig.ModePop;
		time = model->WinAlarmConfig.CycleTime;
		return true;
	}
	void CycleOpenWin_cb(void *data)
	{
		AlarmWinView * view = (AlarmWinView *)data;
		if (view->AlarmInfo.Recovered)
			view->hide();
		else if (!view->AlarmInfo.Recovered && !view->visible())
			view->show();
	}
	void WinClose_cb(Fl_Widget*, void *v)
	{
		AlarmWinView *view = (AlarmWinView *)v;
		int mode, time;
		view->GetCloseMode(mode,time);
		if (mode == Project::PopMode::ONCE)
			view->hide();
		else
		{
			view->hide();
			Fl::add_timeout(time, CycleOpenWin_cb, (void*)view);
		}
		printf("jdkfrg\n");
		Fl_Widget::default_callback(view, v);
	}
	void AlarmWinView::draw()
	{
		shared_ptr<AlarmWinControl> ctrl = BaseView.GetControl<AlarmWinControl>();
		shared_ptr<AlarmWinModel> model = BaseView.GetModel<AlarmWinModel>();
		AlarmInfo = ctrl->AlarmInfo;
		//设置背景
		if (model->WinAlarmConfig.BgMode && model->WinAlarmConfig.BgPicKey.KeyVal.size() != 0)
		{
			Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->WinAlarmConfig.BgPicKey.KeyVal);
			if (!rgbImage) {
				printf("Can't get image\n");
			}
			else {
				image(rgbImage);
				align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
				box(FL_NO_BOX);
				draw_box();
			}
		}
		if(model->WinAlarmConfig.BgMode == 0)
			color(fl_rgb_color(RGBColor(model->WinAlarmConfig.BgColor)));
		//设置窗口模式
		if(model->WinAlarmConfig.AloneMode)
			set_modal();
		//设置窗口回调函数
		callback((Fl_Callback *)WinClose_cb, (void *)this);
	}
}

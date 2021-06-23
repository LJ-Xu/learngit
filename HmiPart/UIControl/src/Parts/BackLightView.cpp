/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightView.cpp
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : ���Ʊ�����
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BackLightView.h"
#include "TimerModel.h"
#include "CodeFormatUtility.h"
#include "IResourceService.h"
#include "SysSetApi.h"
namespace UI
{
	BackLightView::BackLightView(int X, int Y, int W, int H) : HMIBaseWindow(X, Y, W, H)
	{
		box(FL_NO_BOX);
		end();
	}
	BackLightView::~BackLightView() {}
	
	int BackLightView::handle(int event)
	{
		switch (event) {
		case FL_PUSH:				//�ؼ�����
		{
			SysSetApi::TriggerBeep();
			bool IsPutOutStatus = false;		//�Ƿ���Ϩ��״̬
			hide();
			redraw();
		}
		}
		//return 0;
		return 1;			//��һ���Ƿ�����Ϣ
	}
	
	void BackLightView::draw()
	{
		shared_ptr<TimerModel> model = BaseView.GetModel<TimerModel>();
		if (UI::IResourceService::Ins()->IsRenderMode())
		{
			fl_font(0, 14);
			fl_color(FL_BLACK);
			draw_box();
			string text = "������";
			if (!UI::CodeFormatUtility::IsStrUtf8(text.c_str()))
				UI::IResourceService::GB2312toUtf8(text);
			fl_draw(text.c_str(), model->TimerConfig.X + model->TimerConfig.OffX,
				model->TimerConfig.Y + model->TimerConfig.OffY,
				model->TimerConfig.Width, model->TimerConfig.Height, FL_ALIGN_CENTER);
		}
		else
		{
			if (!IsPutOutStatus)
				return;
			fl_color(FL_BLACK);
			fl_rectf(x(),y(),w(),h());
		}
	}
}

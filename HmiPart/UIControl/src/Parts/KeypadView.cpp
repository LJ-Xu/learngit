/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 绘制按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "KeypadView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "UIData.h"
#include "PermUtility.h"
#include "HMIWindow.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	KeypadView::KeypadView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	KeypadView::~KeypadView() {}

	//按键满足瞬时ON时间后，将状态置OFF
	//void BtnshortON_cb(void *data)
	//{
	//	KeypadView * view = (KeypadView *)data;
	//	shared_ptr<KeypadModel> model = view->BaseView.GetModel<KeypadModel>();
	//	//置为OFF状态
	//	UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
	//	view->HasShortOnEvent = false;
	//}

	void KeypadView::HandleClick()
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();

		switch (model->KeypadConfig.Action)
		{
		case Project::BtnAction::BtnSETON:			//置为ON状态
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), true);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, true);
			break;
		}
		case Project::BtnAction::BtnSETOFF:		//置为OFF状态
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), false);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
			break;
		}
		case Project::BtnAction::BtnOPPOSITE:		//取反
		{
			bool perstatus = UI::UIData::Bit(model->KeypadConfig.WriteVar);
			bool status = !perstatus;
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_TOGGLE,
					model->KeypadConfig.WriteVar, perstatus, status);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, status);
			break;
		}
		case Project::BtnAction::BtnSHORTON:				//瞬时ON状态
		{
			if (model->KeypadConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
					model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), true);
			UI::UIData::Bit(model->KeypadConfig.WriteVar, true);
			break;
		}
		default:
			break;
		}
		PermUtility::QuitLimitPerm(model->KeypadConfig.Perm);
	}

	//按键满足安全时间
	void BtnSafeTimerCb(void *data)
	{
		KeypadView * view = (KeypadView *)data;
		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			view->HandleClick();
		}
	}

	bool KeypadView::HandleOperatePush()
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();
		IsReleased = false;
		if ((model->KeypadConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->KeypadConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			HaveOperateLimit = false;
			if (model->KeypadConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->KeypadConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->KeypadConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->KeypadConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->KeypadConfig.Perm.DelayTime * 100, BtnSafeTimerCb, (void*)this, false);
			HaveOperateLimit = false;
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->KeypadConfig.Perm, ctrl->CtrlId()))
		{
			HaveOperateLimit = false;
			return false;
		}
		HaveOperateLimit = true;
		HandleClick();
		return true;
	}
	int KeypadView::handle(int event)
	{
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();

		switch (event)
		{
		case FL_PUSH:
		{
			char dataflag = DataApi::GetDataFlag(model->KeypadConfig.WriteVar);
			if (dataflag == 0)		return 1;
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (HandleOperatePush())
			{
				//beginTime_ = high_resolution_clock::now();
				redraw();
				return HMIBaseButton::handle(event);
			}
			redraw();
			return 1;
		}
		case FL_RELEASE:
		{
			if (when() & FL_WHEN_RELEASE)
			{
				CurrentStatus = 0;
				ctrl->Page()->RemoveTimeout(BtnSafeTimerCb, (void*)this);		//移除定时器
				//char dataflag = DataApi::GetDataFlag(model->KeypadConfig.WriteVar);
				//if (dataflag == 0)
				//{
				//	redraw();
				//	return 1;
				//}
				IsReleased = true;
				if (HaveOperateLimit && model->KeypadConfig.Action == Project::BtnAction::BtnSHORTON)
				{
					HaveOperateLimit = false;
					if (model->KeypadConfig.IsRecord)
						OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->KeypadConfig.CtrlName, Storage::OA_SET,
							model->KeypadConfig.WriteVar,UIData::Bit(model->KeypadConfig.WriteVar), false);
					UI::UIData::Bit(model->KeypadConfig.WriteVar, false);
				}
				//endTime_ = high_resolution_clock::now();
				//milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime_ - beginTime_);
				//if (timeInterval.count() > model->KeypadConfig.Perm.SafeTime)
				//	HandleClick();		//确定按键按下后松开
					redraw();			//重绘
			}
			return HMIBaseButton::handle(event);
		}
		default:
			return HMIBaseButton::handle(event);
		}
	}

	void KeypadView::draw()
	{
		shared_ptr<KeypadControl> ctrl = BaseView.GetControl<KeypadControl>();
		shared_ptr<KeypadModel> model = BaseView.GetModel<KeypadModel>();
		//获取当前状态
		//int status = value();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus, 
			model->KeypadConfig.PicKey.size() < model->KeypadConfig.StrStyles.size()
			? model->KeypadConfig.PicKey.size() : model->KeypadConfig.StrStyles.size());

		int status = CurrentStatus;
		//获取当前状态图片key值
		Fl_Image * btnImage = nullptr;
		if((size_t)status < model->KeypadConfig.PicKey.size())
			btnImage = IResourceService::Ins()->GetImage(model->KeypadConfig.PicKey[status].KeyVal);
		//Fl_Image * btnImage = IResourceService::Ins()->GetImage(model->KeypadConfig.PicKey[status].KeyVal);
		if (!btnImage) {
			LOG_INFO_("btn Image is NULL\n");
		}
		else {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		//绘制图片框体
		draw_box();
		//判断当前状态是否越界
		if ((size_t)status >= model->KeypadConfig.StrStyles.size())
			return;
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->KeypadConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->KeypadConfig.StrStyles[status].Font.Name), 
				model->KeypadConfig.StrStyles[status].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->KeypadConfig.StrStyles[status].Colors));
		//fl_color(textcolor);
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*绘制文本*/
		fl_draw(text.data(), model->KeypadConfig.X + model->KeypadConfig.OffX,
			model->KeypadConfig.Y + model->KeypadConfig.OffY,
			model->KeypadConfig.Width, model->KeypadConfig.Height,
			model->KeypadConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	}
}

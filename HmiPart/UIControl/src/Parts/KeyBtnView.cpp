/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制键盘按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "KeyBtnView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "UIData.h"
#include "KeyInputUtility.h"
#include "HMIWindow.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
#include "SysSetApi.h"
using namespace std;

namespace UI
{
	KeyBtnView::KeyBtnView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	KeyBtnView::~KeyBtnView() {}


	//按键满足安全时间
	void BtnKeySafeTimerCb(void *data)
	{
		KeyBtnView * view = (KeyBtnView *)data;
		if (!view->IsReleased)
			view->HandleKeyMsg();
	}

	bool KeyBtnView::HandleOperatePush()
	{
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		IsReleased = false;
		if ((model->KeyBtnConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->KeyBtnConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->KeyBtnConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->KeyBtnConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->KeyBtnConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->KeyBtnConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->KeyBtnConfig.Perm.DelayTime * 100, BtnKeySafeTimerCb, (void*)this, false);
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->KeyBtnConfig.Perm, ctrl->CtrlId()))
			return false;

		HandleKeyMsg();
		return true;
	}
	void KeyBtnView::HandleKeyMsg()
	{
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		/*发送键值*/
		ctrl->Page()->Win()->SendKey(model->KeyBtnConfig.AsciiNum);
		if (model->KeyBtnConfig.Perm.HasShowPerm && model->KeyBtnConfig.Perm.HasLimitShowPerm)
			redraw();
		PermUtility::QuitLimitPerm(model->KeyBtnConfig.Perm);
	}

	int KeyBtnView::handle(int event)
	{
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		switch (event)
		{
		case FL_PUSH:
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (model->KeyBtnConfig.IsCloseBtn)			//关闭按钮
			{
				ctrl->Win()->ClosePage(ctrl->Page()->Winno());
				return 1;
			}
			if (HandleOperatePush())
			{
				redraw();
				return HMIBaseButton::handle(event);
			}			
			redraw();
			return 1;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				ctrl->Page()->RemoveTimeout(BtnKeySafeTimerCb, (void*)this);		//移除定时器
				CurrentStatus = 0;
				IsReleased = true;
			}
			redraw();
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}
		 
	void KeyBtnView::draw()
	{
		shared_ptr<KeyBtnControl> ctrl = BaseView.GetControl<KeyBtnControl>();
		shared_ptr<KeyBtnModel> model = BaseView.GetModel<KeyBtnModel>();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,
			model->KeyBtnConfig.PicKey.size() < model->KeyBtnConfig.StrStyles.size()
			? model->KeyBtnConfig.PicKey.size() : model->KeyBtnConfig.StrStyles.size());		//获取当前状态
		int status = CurrentStatus;;
		if (model->KeyBtnConfig.AsciiNum == 16)	//shift按键
			status = KeyInputUtility::IsChinese();
		if(model->KeyBtnConfig.AsciiNum == 20)	//caps按键 
			status = KeyInputUtility::IsUpper();
		
		//获取当前状态图片key值
		Fl_Image * btnImage = nullptr;
		if((size_t)status < model->KeyBtnConfig.PicKey.size())
			btnImage = IResourceService::Ins()->GetImage(model->KeyBtnConfig.PicKey[status].KeyVal);
		if (btnImage) {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
		}
		else
		{
			box(FL_NO_BOX);
			LOG_INFO_("Key Btn Pic is NULL\n");
		}

		//绘制图片框体
		//box(FL_FLAT_BOX);
		draw_box();

		//判断当前状态是否越界
		if ((size_t)status >= model->KeyBtnConfig.StrStyles.size())
			return;
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->KeyBtnConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->KeyBtnConfig.StrStyles[status].Font.Name),
			model->KeyBtnConfig.StrStyles[status].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->KeyBtnConfig.StrStyles[status].Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*绘制文本*/
		fl_draw(text.data(), model->KeyBtnConfig.X + model->KeyBtnConfig.OffX,
			model->KeyBtnConfig.Y + model->KeyBtnConfig.OffY,
			model->KeyBtnConfig.Width, model->KeyBtnConfig.Height,
			model->KeyBtnConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	}
}

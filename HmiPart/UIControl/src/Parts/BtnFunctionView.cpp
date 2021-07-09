/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnFunctionView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制功能按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "BtnFunctionView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "PermUtility.h"
#include "HMIWindow.h"
#include "SysSetApi.h"
using namespace std;
namespace UI
{
	BtnFunctionView::BtnFunctionView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	BtnFunctionView::~BtnFunctionView() {}
	
	//按键满足安全时间
	void BtnFuncSafeTimerCb(void *data)
	{
		BtnFunctionView * view = (BtnFunctionView *)data;
		shared_ptr<BtnFunctionControl> ctrl = view->BaseView.GetControl<BtnFunctionControl>();
		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			ctrl->HandleBtnFunc(FunctionAct::PRESS);
		}
	}
	bool BtnFunctionView::HandleOperatePush()
	{
		shared_ptr<BtnFunctionModel> model = BaseView.GetModel<BtnFunctionModel>();
		shared_ptr<BtnFunctionControl> ctrl = BaseView.GetControl<BtnFunctionControl>();
		IsReleased = false;
		if ((model->FuncBtnConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->FuncBtnConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			HaveOperateLimit = false;
			if (model->FuncBtnConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->FuncBtnConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->FuncBtnConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->FuncBtnConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->FuncBtnConfig.Perm.DelayTime * 100, BtnFuncSafeTimerCb, (void*)this, false);
			HaveOperateLimit = false;
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->FuncBtnConfig.Perm, ctrl->CtrlId()))
		{
			HaveOperateLimit = false;
			return false;
		}
		HaveOperateLimit = true;
		return true;
		//return true;
	}

	int BtnFunctionView::handle(int event)
	{
		shared_ptr<BtnFunctionModel> model = BaseView.GetModel<BtnFunctionModel>();
		shared_ptr<BtnFunctionControl> ctrl = BaseView.GetControl<BtnFunctionControl>();
		switch (event)
		{
		case FL_PUSH:
			SysSetApi::TriggerBeep();
			CurrentStatus = 1;
			if (HandleOperatePush())
			{
				if (ctrl->HandleBtnFunc(PRESS))				//按下返回true，窗口仍存在
				{
					redraw();
					return HMIBaseButton::handle(event);
				}
				else										//窗口不存在
					return 1;
			}
			else
				redraw();
			return 1;
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				ctrl->Page()->RemoveTimeout(BtnFuncSafeTimerCb, (void*)this);		//移除定时器
				CurrentStatus = 0;
				IsReleased = true;
				if (HaveOperateLimit)
				{
					if (ctrl->HandleBtnFunc(RELEASE))
					{
						redraw();
						return HMIBaseButton::handle(event);
					}
					else
						return 0;
				}
				else
					redraw();
			}
			return HMIBaseButton::handle(event);;
		default:
			return HMIBaseButton::handle(event);
		}
	}

	

	void BtnFunctionView::draw()
	{
		shared_ptr<BtnFunctionControl> ctrl = BaseView.GetControl<BtnFunctionControl>();
		shared_ptr<BtnFunctionModel> model = BaseView.GetModel<BtnFunctionModel>();
		//获取当前状态
		int status = CurrentStatus;
		IResourceService::Ins()->SetRenderStatus(status,
			model->FuncBtnConfig.PicKey.size() < model->FuncBtnConfig.StrStyles.size()
			? model->FuncBtnConfig.PicKey.size() : model->FuncBtnConfig.StrStyles.size());		//获取当前状态

		//获取当前状态图片key值
		Fl_Image * btnImage = nullptr;
		if ((size_t)status < model->FuncBtnConfig.PicKey.size())
			btnImage = IResourceService::Ins()->GetImage(model->FuncBtnConfig.PicKey[status].KeyVal);
		if (!btnImage) {
			LOG_INFO_("Btn Function Pic is NULL\n");
		}
		else {
			image(btnImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		//绘制图片框体
		draw_box();
		//判断当前状态是否越界
		if ((size_t)status >= model->FuncBtnConfig.StrStyles.size())
			return;
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->FuncBtnConfig.Txt, model->FuncBtnConfig.CurrentStatus);
		//string text = GetDrawString(model.get());
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->FuncBtnConfig.StrStyles[status].Font.Name),
			model->FuncBtnConfig.StrStyles[status].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->FuncBtnConfig.StrStyles[status].Colors));
		//fl_color(textcolor);
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*绘制文本*/
		fl_draw(text.data(), model->FuncBtnConfig.X + model->FuncBtnConfig.OffX,
			model->FuncBtnConfig.Y + model->FuncBtnConfig.OffY,
			model->FuncBtnConfig.Width,
			model->FuncBtnConfig.Height,
			model->FuncBtnConfig.StrStyles[status].Align);	//FL_ALIGN_CENTER
	
	}
}

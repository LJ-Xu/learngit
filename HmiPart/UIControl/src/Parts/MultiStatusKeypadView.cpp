/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiStatusKeypadView.cpp
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 绘制多状态按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "MultiStatusKeypadView.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include "IResourceService.h"
#include "UIData.h"
#include "HMIWindow.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace UI
{
	MultiStatusKeypadView::MultiStatusKeypadView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	MultiStatusKeypadView::~MultiStatusKeypadView() {}

	void MultiStatusKeypadView::SetStatusAction()
	{
		unsigned i;
		shared_ptr<MultiStatusKeypadControl> ctrl = BaseView.GetControl<MultiStatusKeypadControl>();
		shared_ptr<MultiStatusKeypadModel> model = BaseView.GetModel<MultiStatusKeypadModel>();
		//获取当前状态设置值
		if ((size_t)CurrentStatus >= model->MultiStatusKeypadConfig.Action.size())
			return;
		
		if (model->MultiStatusKeypadConfig.DataVarType == 0)	//位寄存器
		{
			int value = atoi(model->MultiStatusKeypadConfig.Action[CurrentStatus].Val.c_str());
			for (i = 0; i < model->MultiStatusKeypadConfig.RegVars.size() - 1; i++)
			{
				if (model->MultiStatusKeypadConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->MultiStatusKeypadConfig.CtrlName, Storage::OA_SET,
						model->MultiStatusKeypadConfig.RegVars[i],UIData::Bit(model->MultiStatusKeypadConfig.RegVars[i]), (bool)((value >> i) & 0x1));
				UI::UIData::Bit(model->MultiStatusKeypadConfig.RegVars[i], (value >> i) & 0x1);
			}
		}
		else {					//字寄存器
			int num = model->MultiStatusKeypadConfig.RegVars.size() - 1;
			XJDataType tp = UIDataService::Ins().GetDataType(model->MultiStatusKeypadConfig.RegVars[num]);
			DDWORD value = Utility::DataFormate::NumParse(model->MultiStatusKeypadConfig.Action[CurrentStatus].Val, tp);
			double dvalue = Utility::DataFormate::GetNumber<double>(value, tp);
			if (model->MultiStatusKeypadConfig.RegVars.size() > (size_t)model->MultiStatusKeypadConfig.BitNum)
			{
				if (model->MultiStatusKeypadConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->MultiStatusKeypadConfig.CtrlName, Storage::OA_SET,
						model->MultiStatusKeypadConfig.RegVars[num], UI::UIData::Number<double>(model->MultiStatusKeypadConfig.RegVars[num]), value);
				UIDataService::Ins().SetNumber(model->MultiStatusKeypadConfig.RegVars[num], value);

				//UI::UIData::Number(model->MultiStatusKeypadConfig.RegVars[num], value);
			}
		}
	}

	void MultiStatusKeypadView::HandleClick()
	{
		shared_ptr<MultiStatusKeypadModel> model = BaseView.GetModel<MultiStatusKeypadModel>();
		//增加状态
		if (CurrentStatus == model->MultiStatusKeypadConfig.StatusCnt - 1)
			CurrentStatus = 0;
		else
			CurrentStatus++;
		//设置寄存器（动作）
		SetStatusAction();
		PermUtility::QuitLimitPerm(model->MultiStatusKeypadConfig.Perm);

		redraw();
	}

	//按键满足安全时间
	void MultiBtnSafeTimerCb(void *data)
	{
		MultiStatusKeypadView * view = (MultiStatusKeypadView *)data;
		if (!view->IsReleased)
		{
			view->IsAchieveSafeTime = true;
			view->HandleClick();
		}
	}
	//按键满足安全时间
	void MultBtnSafeTimerCb(void *data)
	{
		MultiStatusKeypadView * view = (MultiStatusKeypadView *)data;
		if (!view->IsReleased)
			view->HandleClick();
	}
	bool MultiStatusKeypadView::HandleOperatePush()
	{
		shared_ptr<MultiStatusKeypadModel> model = BaseView.GetModel<MultiStatusKeypadModel>();
		shared_ptr<MultiStatusKeypadControl> ctrl = BaseView.GetControl<MultiStatusKeypadControl>();
		IsReleased = false;
		if ((model->MultiStatusKeypadConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->MultiStatusKeypadConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->MultiStatusKeypadConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->MultiStatusKeypadConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->MultiStatusKeypadConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->MultiStatusKeypadConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->MultiStatusKeypadConfig.Perm.DelayTime * 100, MultBtnSafeTimerCb, (void*)this, false);
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->MultiStatusKeypadConfig.Perm, ctrl->CtrlId()))
			return false;
		HandleClick();
		return true;
	}
	int MultiStatusKeypadView::handle(int event)
	{
		shared_ptr<MultiStatusKeypadModel> model = BaseView.GetModel<MultiStatusKeypadModel>();
		shared_ptr<MultiStatusKeypadControl> ctrl = BaseView.GetControl<MultiStatusKeypadControl>();

		switch (event)
		{
		case FL_PUSH:
		{
			SysSetApi::TriggerBeep();
			if (HandleOperatePush())
				return HMIBaseButton::handle(event);
			return 1;
		}
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				ctrl->Page()->RemoveTimeout(MultBtnSafeTimerCb, (void*)this);		//移除定时器
				IsReleased = true;
			}
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}


	void MultiStatusKeypadView::draw()
	{
		shared_ptr<MultiStatusKeypadControl> ctrl = BaseView.GetControl<MultiStatusKeypadControl>();
		shared_ptr<MultiStatusKeypadModel> model = BaseView.GetModel<MultiStatusKeypadModel>();
		//判断当前状态是否越界
		IResourceService::Ins()->SetRenderStatus(CurrentStatus, model->MultiStatusKeypadConfig.Action.size());
		int status = CurrentStatus;
		//判断当前状态是否越界
		if ((size_t)status >= model->MultiStatusKeypadConfig.Action.size())
			return;

		//if ((size_t)CurrentStatus >= model->MultiStatusKeypadConfig.Action.size())
			//return;
		//获取当前图片
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->MultiStatusKeypadConfig.Action[status].PicKey.KeyVal);
		if (!rgbImage) {
			LOG_INFO_("Multi Status Keypad Pic is NULL\n");
		}
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		//绘制图片框体
		draw_box();
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->MultiStatusKeypadConfig.Txt, status);
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(IResourceService::Ins()->GetFontIdx(model->MultiStatusKeypadConfig.Action[status].StrStyles.Font.Name), 
			model->MultiStatusKeypadConfig.Action[status].StrStyles.Font.Size);
		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->MultiStatusKeypadConfig.Action[status].StrStyles.Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*绘制文本*/
		fl_draw(text.data(), model->MultiStatusKeypadConfig.X + model->MultiStatusKeypadConfig.OffX,
			model->MultiStatusKeypadConfig.Y + model->MultiStatusKeypadConfig.OffY,
			model->MultiStatusKeypadConfig.Width,
			model->MultiStatusKeypadConfig.Height,
			model->MultiStatusKeypadConfig.Action[status].StrStyles.Align);	//FL_ALIGN_CENTER
	}
}

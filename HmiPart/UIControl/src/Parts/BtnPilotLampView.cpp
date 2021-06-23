/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 绘制指示灯按键
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "UIComm.h"
#include "BtnPilotLampView.h"
#include "BtnPilotLampControl.h"
#include "ResourceService.h"
#include "Message.h"
#include "StringUtility.h"
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "HMIWindow.h"
#include "UIData.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
namespace UI
{
	BtnPilotLampView::BtnPilotLampView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	BtnPilotLampView::~BtnPilotLampView() {}

	void BtnPilotLampView::HandleClick()
	{
		shared_ptr<BtnPilotLampModel> model = BaseView.GetModel<BtnPilotLampModel>();
		shared_ptr<BtnPilotLampControl> ctrl = BaseView.GetControl<BtnPilotLampControl>();
		switch (model->BtnPilotLampConfig.Action)
		{
		case Project::BtnAction::BtnSETON:			//置为ON状态
		{
			if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.WriteVar, UIData::Bit(model->BtnPilotLampConfig.WriteVar), true);
				UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar, true);
			}
			else
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.ReadVar, UIData::Bit(model->BtnPilotLampConfig.ReadVar), true);
				UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar, true);

			}
			break;
		}
		case Project::BtnAction::BtnSETOFF:		//置为OFF状态
		{
			if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.WriteVar, UIData::Bit(model->BtnPilotLampConfig.WriteVar), false);
				UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar, false);
			}
			else
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.ReadVar, UIData::Bit(model->BtnPilotLampConfig.ReadVar), false);
				UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar, false);
				break;

			}
		}
		case Project::BtnAction::BtnOPPOSITE:		//取反
		{
			if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
			{
				bool perstatus = UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar);
				bool status = !perstatus;
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_TOGGLE,
						model->BtnPilotLampConfig.WriteVar, perstatus, status);
				UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar, status);
			}
			else
			{
				bool perstatus = UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar);
				bool status = !perstatus;
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_TOGGLE,
						model->BtnPilotLampConfig.ReadVar, perstatus, status);
				UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar, status);
			}
			break;
		}
		case Project::BtnAction::BtnSHORTON:				//瞬时ON状态
		{
			/*寄存器写入ON*/
			if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.WriteVar, UIData::Bit(model->BtnPilotLampConfig.WriteVar), true);
				UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar, true);
			}
			else
			{
				if (model->BtnPilotLampConfig.IsRecord)
					OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
						model->BtnPilotLampConfig.ReadVar, UIData::Bit(model->BtnPilotLampConfig.ReadVar), true);
				UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar, true);
			}
			break;
		}
		default:
			break;
		}
		PermUtility::QuitLimitPerm(model->BtnPilotLampConfig.Perm);
	}
	//按键满足安全时间
	void BtnLampSafeTimerCb(void *data)
	{
		BtnPilotLampView * view = (BtnPilotLampView *)data;
		if (!view->IsReleased)
		{
			view->HaveOperateLimit = true;
			view->HandleClick();
		}
	}
	bool BtnPilotLampView::HandleOperatePush()
	{
		shared_ptr<BtnPilotLampModel> model = BaseView.GetModel<BtnPilotLampModel>();
		shared_ptr<BtnPilotLampControl> ctrl = BaseView.GetControl<BtnPilotLampControl>();
		IsReleased = false;
		if ((model->BtnPilotLampConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->BtnPilotLampConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			HaveOperateLimit = false;
			if (model->BtnPilotLampConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->BtnPilotLampConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->BtnPilotLampConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		if (model->BtnPilotLampConfig.Perm.IsKeydelay)
		{
			ctrl->Page()->AddTimeout(model->BtnPilotLampConfig.Perm.DelayTime * 100, BtnLampSafeTimerCb, (void*)this, false);
			HaveOperateLimit = false;
			return false;
		}
		else if (!PermUtility::HandleConfirmPerm(model->BtnPilotLampConfig.Perm, ctrl->CtrlId()))
		{
			HaveOperateLimit = false;
			return false;
		}
		HaveOperateLimit = true;
		HandleClick();
		return true;
	}
	int BtnPilotLampView::handle(int event)
	{
		shared_ptr<BtnPilotLampModel> model = BaseView.GetModel<BtnPilotLampModel>();
		shared_ptr<BtnPilotLampControl> ctrl = BaseView.GetControl<BtnPilotLampControl>();
		switch (event)
		{
		case FL_PUSH:
		{
			char dataflag;
			if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
				dataflag = DataApi::GetDataFlag(model->BtnPilotLampConfig.WriteVar);
			else
				dataflag = DataApi::GetDataFlag(model->BtnPilotLampConfig.ReadVar);
			if (dataflag == 0)		//没有权限则返回
				return 1;
			SysSetApi::TriggerBeep();
			if (HandleOperatePush())
				return HMIBaseButton::handle(event);
			return 1;
		}
		case FL_RELEASE:
			if (when() & FL_WHEN_RELEASE)
			{
				ctrl->Page()->RemoveTimeout(BtnLampSafeTimerCb, (void*)this);		//移除定时器
				IsReleased = true;
				char dataflag;
				if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
					dataflag = DataApi::GetDataFlag(model->BtnPilotLampConfig.WriteVar);
				else
					dataflag = DataApi::GetDataFlag(model->BtnPilotLampConfig.ReadVar);
				if (dataflag == 0)		//没有权限则返回
					return 1;
				if (HaveOperateLimit && model->BtnPilotLampConfig.Action == Project::BtnAction::BtnSHORTON)
				{
					HaveOperateLimit = false;
					if (model->BtnPilotLampConfig.RwDiffAddrUsed)  //读写使用不同寄存器
					{
						if (model->BtnPilotLampConfig.IsRecord)
							OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
								model->BtnPilotLampConfig.WriteVar, UIData::Bit(model->BtnPilotLampConfig.WriteVar), false);
						UI::UIData::Bit(model->BtnPilotLampConfig.WriteVar, false);
					}
					else
					{
						if (model->BtnPilotLampConfig.IsRecord)
							OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->BtnPilotLampConfig.CtrlName, Storage::OperatorAction::OA_SET,
								model->BtnPilotLampConfig.ReadVar, UIData::Bit(model->BtnPilotLampConfig.ReadVar), false);
						UI::UIData::Bit(model->BtnPilotLampConfig.ReadVar, false);
					}
				}
			}
			return HMIBaseButton::handle(event);
		default:
			return HMIBaseButton::handle(event);
		}
	}

	void BtnPilotLampView::draw()
	{
		shared_ptr<BtnPilotLampControl> ctrl = BaseView.GetControl<BtnPilotLampControl>();
		shared_ptr<BtnPilotLampModel> model = BaseView.GetModel<BtnPilotLampModel>();
		//获取当前状态
		//判断当前状态是否越界
		if (CurrentFlash == 0)
			return;
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,
			model->BtnPilotLampConfig.PicKey.size() < model->BtnPilotLampConfig.StrStyles.size()
			? model->BtnPilotLampConfig.PicKey.size() : model->BtnPilotLampConfig.StrStyles.size());		//获取当前状态

		if ((size_t)CurrentStatus >= model->BtnPilotLampConfig.PicKey.size() || (size_t)CurrentStatus >= model->BtnPilotLampConfig.StrStyles.size())
			return;
		//获取当前状态图片key值
		//if (model->BtnPilotLampConfig.PicKey.size() == 0 || model->BtnPilotLampConfig.StrStyles.size() == 0)
		//	return;

		Fl_Image *btnlampImage = IResourceService::Ins()->GetImage(model->BtnPilotLampConfig.PicKey[CurrentStatus].KeyVal);

		if (!btnlampImage) {
			LOG_INFO_("btn lamp Image is NULL\n");
			//box(FL_FLAT_BOX);
		}
		else {
			image(btnlampImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		//绘制图片框体
		draw_box();
		//获取当前状态下文字
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->BtnPilotLampConfig.Txt, CurrentStatus);
		UI::IResourceService::GB2312toUtf8(text);
		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->BtnPilotLampConfig.StrStyles[CurrentStatus].Font.Name),
			model->BtnPilotLampConfig.StrStyles[CurrentStatus].Font.Size);

		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->BtnPilotLampConfig.StrStyles[CurrentStatus].Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));
		/*绘制文本*/
		fl_draw(text.data(), model->BtnPilotLampConfig.X + model->BtnPilotLampConfig.OffX,
			model->BtnPilotLampConfig.Y + model->BtnPilotLampConfig.OffY,
			model->BtnPilotLampConfig.Width,
			model->BtnPilotLampConfig.Height,
			model->BtnPilotLampConfig.StrStyles[CurrentStatus].Align);	//FL_ALIGN_CENTER
	}
	
}

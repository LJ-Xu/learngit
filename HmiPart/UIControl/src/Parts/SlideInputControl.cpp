/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-18
 * Descript : 创建滑动输入绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "SlideInputControl.h"
#include "HMIWindow.h"
#include "UIData.h"
#include "Message.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"

namespace UI
{
	SlideInputControl::SlideInputControl(UI::HMIPage* w) : BaseControl(w)
	{
		//创建SlideInputModel
		mode_ = std::shared_ptr < SlideInputModel>(new SlideInputModel());
		InitMVCModel(mode_);
	}

	void SlideInputControl::CreateView()
	{
		//初始化VIEW
		SlideInputView *view = new SlideInputView(
			mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX,
			mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY,
			mode_->SlideInputConfig.Width,
			mode_->SlideInputConfig.Height,nullptr);
		InitMVCView(view);
		view->InitDraw();
//		view->deactivate();
	}
	void SlideInputControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void SlideInputControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->SlideInputConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->SlideInputConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->SlideInputConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->SlideInputConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->SlideInputConfig.Perm, mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX, mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void SlideInputControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->SlideInputConfig.VOffX != Project::DataVarId::NullId)
			mode_->SlideInputConfig.OffX = UI::UIData::Number<int>(mode_->SlideInputConfig.VOffX) + pageoffx;
		else
			mode_->SlideInputConfig.OffX = pageoffx;


		if (mode_->SlideInputConfig.VOffY != Project::DataVarId::NullId)
			mode_->SlideInputConfig.OffY = UI::UIData::Number<int>(mode_->SlideInputConfig.VOffY) + pageoffy;
		else
			mode_->SlideInputConfig.OffY = pageoffy;
	}
	void SlideInputControl::HandleDataVar(Project::DataVarId &varId)
	{
		SlideInputView* pView = static_cast<SlideInputView*>(pView_);
		if (mode_->SlideInputConfig.VOffX.Cmp(varId))
		{
			mode_->SlideInputConfig.OffX = UI::UIData::Number<int>(mode_->SlideInputConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->SlideInputConfig.Perm, mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX, mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
		}
		if (mode_->SlideInputConfig.VOffY.Cmp(varId))
		{
			mode_->SlideInputConfig.OffY = UI::UIData::Number<int>(mode_->SlideInputConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->SlideInputConfig.Perm, mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX, mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
		}
		if (mode_->SlideInputConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->SlideInputConfig.Perm, pView_);
		if (mode_->SlideInputConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->SlideInputConfig.Perm, pView_);
		if (mode_->SlideInputConfig.ReadVar.Cmp(varId)) 
		{
			DOUBLE readvalue = UI::UIData::Number<DOUBLE>(mode_->SlideInputConfig.ReadVar);
			pView->value(readvalue);
			if (mode_->SlideInputConfig.Perm.HasShowPerm && mode_->SlideInputConfig.Perm.HasLimitShowPerm)
				UI::ViewShowUtility::ShowView(pView, mode_->SlideInputConfig.Perm,
					mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX,
					mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
		}
		if (mode_->SlideInputConfig.AttrMaxVar.Cmp(varId)) 
		{
			mode_->SlideInputConfig.AttrMax = UI::UIData::Number<DOUBLE>(mode_->SlideInputConfig.AttrMaxVar);
			pView->maximum(mode_->SlideInputConfig.AttrMax);
			if (mode_->SlideInputConfig.Dir == Project::SlideDir::SildeUp ||
				mode_->SlideInputConfig.Dir == Project::SlideDir::SildeLeft)
			{
				pView->minimum(mode_->SlideInputConfig.AttrMax);
				pView->maximum(mode_->SlideInputConfig.AttrMin);
			}
			else
			{
				pView->minimum(mode_->SlideInputConfig.AttrMin);
				pView->maximum(mode_->SlideInputConfig.AttrMax);
			}
			UI::ViewShowUtility::ShowView(pView, mode_->SlideInputConfig.Perm,
				mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX,
				mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
		}
		if (mode_->SlideInputConfig.AttrMinVar.Cmp(varId))
		{
			mode_->SlideInputConfig.AttrMin = UI::UIData::Number<DOUBLE>(mode_->SlideInputConfig.AttrMinVar);
			pView->minimum(mode_->SlideInputConfig.AttrMin);
			if (mode_->SlideInputConfig.Dir == Project::SlideDir::SildeUp ||
				mode_->SlideInputConfig.Dir == Project::SlideDir::SildeLeft)
			{
				pView->minimum(mode_->SlideInputConfig.AttrMax);
				pView->maximum(mode_->SlideInputConfig.AttrMin);
			}
			else
			{
				pView->minimum(mode_->SlideInputConfig.AttrMin);
				pView->maximum(mode_->SlideInputConfig.AttrMax);
			}
			UI::ViewShowUtility::ShowView(pView, mode_->SlideInputConfig.Perm,
				mode_->SlideInputConfig.X + mode_->SlideInputConfig.OffX,
				mode_->SlideInputConfig.Y + mode_->SlideInputConfig.OffY);
		}
	}

	int SlideInputControl::PeekHMIMessage(Message::Msg* msg)
	{
		SlideInputView* pView = static_cast<SlideInputView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}

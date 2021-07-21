/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiStatusKeypadControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-23
 * Descript : 创建多状态按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "MultiStatusKeypadControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	MultiStatusKeypadControl::MultiStatusKeypadControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeypadModel
		mode_ = std::shared_ptr<MultiStatusKeypadModel>(new MultiStatusKeypadModel());
		InitMVCModel(mode_);
	}

	void MultiStatusKeypadControl::CreateView()
	{
		//初始化VIEW
		MultiStatusKeypadView* view  = new MultiStatusKeypadView(
			mode_->MultiStatusKeypadConfig.X + mode_->MultiStatusKeypadConfig.OffX,
			mode_->MultiStatusKeypadConfig.Y + mode_->MultiStatusKeypadConfig.OffY,
			mode_->MultiStatusKeypadConfig.Width,
			mode_->MultiStatusKeypadConfig.Height, nullptr);
		InitMVCView(view);
	}
	void MultiStatusKeypadControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool MultiStatusKeypadControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->MultiStatusKeypadConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->MultiStatusKeypadConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->MultiStatusKeypadConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->MultiStatusKeypadConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->MultiStatusKeypadConfig.Perm, 
					mode_->MultiStatusKeypadConfig.X + mode_->MultiStatusKeypadConfig.OffX, 
					mode_->MultiStatusKeypadConfig.Y + mode_->MultiStatusKeypadConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void MultiStatusKeypadControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->MultiStatusKeypadConfig.VOffX != Project::DataVarId::NullId)
			mode_->MultiStatusKeypadConfig.OffX = UI::UIData::Number<int>(mode_->MultiStatusKeypadConfig.VOffX) + pageoffx;
		else
			mode_->MultiStatusKeypadConfig.OffX = pageoffx;


		if (mode_->MultiStatusKeypadConfig.VOffY != Project::DataVarId::NullId)
			mode_->MultiStatusKeypadConfig.OffY = UI::UIData::Number<int>(mode_->MultiStatusKeypadConfig.VOffY) + pageoffy;
		else
			mode_->MultiStatusKeypadConfig.OffY = pageoffy;
	}
	void MultiStatusKeypadControl::HandleDataVar(Project::DataVarId &varId)
	{
		MultiStatusKeypadView* pView = static_cast<MultiStatusKeypadView*>(pView_);
		if (mode_->MultiStatusKeypadConfig.VOffX.Cmp(varId))
		{
			mode_->MultiStatusKeypadConfig.OffX = UI::UIData::Number<int>(mode_->MultiStatusKeypadConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->MultiStatusKeypadConfig.Perm, mode_->MultiStatusKeypadConfig.X + mode_->MultiStatusKeypadConfig.OffX, mode_->MultiStatusKeypadConfig.Y + mode_->MultiStatusKeypadConfig.OffY);
		}
		if (mode_->MultiStatusKeypadConfig.VOffY.Cmp(varId))
		{
			mode_->MultiStatusKeypadConfig.OffY = UI::UIData::Number<int>(mode_->MultiStatusKeypadConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->MultiStatusKeypadConfig.Perm, mode_->MultiStatusKeypadConfig.X + mode_->MultiStatusKeypadConfig.OffX, mode_->MultiStatusKeypadConfig.Y + mode_->MultiStatusKeypadConfig.OffY);
		}
		if (mode_->MultiStatusKeypadConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->MultiStatusKeypadConfig.Perm, pView_);
		if (mode_->MultiStatusKeypadConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->MultiStatusKeypadConfig.Perm, pView_);
	}

	int MultiStatusKeypadControl::PeekHMIMessage(Message::Msg* msg)
	{
		MultiStatusKeypadView* pView = static_cast<MultiStatusKeypadView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->HandleClick();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
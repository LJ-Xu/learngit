/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-22
 * Descript : 创建按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeypadControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	KeypadControl::KeypadControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeypadModel
		mode_ = std::shared_ptr<KeypadModel>(new KeypadModel());
		InitMVCModel(mode_);
	}

	void KeypadControl::CreateView()
	{
		//初始化VIEW
		KeypadView* view = new KeypadView(
			mode_->KeypadConfig.X + mode_->KeypadConfig.OffX,
			mode_->KeypadConfig.Y + mode_->KeypadConfig.OffY,
			mode_->KeypadConfig.Width,
			mode_->KeypadConfig.Height, nullptr);
		InitMVCView(view);
	}
	void KeypadControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool KeypadControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->KeypadConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->KeypadConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->KeypadConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->KeypadConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->KeypadConfig.Perm, 
					mode_->KeypadConfig.X + mode_->KeypadConfig.OffX, 
					mode_->KeypadConfig.Y + mode_->KeypadConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void KeypadControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->KeypadConfig.VOffX != Project::DataVarId::NullId)
			mode_->KeypadConfig.OffX = UI::UIData::Number<int>(mode_->KeypadConfig.VOffX) + pageoffx;
		else
			mode_->KeypadConfig.OffX = pageoffx;


		if (mode_->KeypadConfig.VOffY != Project::DataVarId::NullId)
			mode_->KeypadConfig.OffY = UI::UIData::Number<int>(mode_->KeypadConfig.VOffY) + pageoffy;
		else
			mode_->KeypadConfig.OffY = pageoffy;
	}
	void KeypadControl::HandleDataVar(Project::DataVarId &varId)
	{
		KeypadView* pView = static_cast<KeypadView*>(pView_);
		if (mode_->KeypadConfig.VOffX.Cmp(varId))
		{
			mode_->KeypadConfig.OffX = UI::UIData::Number<int>(mode_->KeypadConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->KeypadConfig.Perm, mode_->KeypadConfig.X + mode_->KeypadConfig.OffX, mode_->KeypadConfig.Y + mode_->KeypadConfig.OffY);
		}
		if (mode_->KeypadConfig.VOffY.Cmp(varId))
		{
			mode_->KeypadConfig.OffY = UI::UIData::Number<int>(mode_->KeypadConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->KeypadConfig.Perm, mode_->KeypadConfig.X + mode_->KeypadConfig.OffX, mode_->KeypadConfig.Y + mode_->KeypadConfig.OffY);
		}
		if (mode_->KeypadConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->KeypadConfig.Perm, pView_);
		if (mode_->KeypadConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->KeypadConfig.Perm, pView_);

	}

	int KeypadControl::PeekHMIMessage(Message::Msg* msg)
	{
		KeypadView* pView = static_cast<KeypadView*>(pView_);
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

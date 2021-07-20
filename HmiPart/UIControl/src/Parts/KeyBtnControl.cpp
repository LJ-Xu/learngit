/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建键盘按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeyBtnControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	KeyBtnControl::KeyBtnControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<KeyBtnModel>(new KeyBtnModel());
		InitMVCModel(mode_);
	}

	void KeyBtnControl::CreateView()
	{
		//初始化VIEW
		KeyBtnView* view = new KeyBtnView(
						mode_->KeyBtnConfig.X + mode_->KeyBtnConfig.OffX,
						mode_->KeyBtnConfig.Y + mode_->KeyBtnConfig.OffY,
						mode_->KeyBtnConfig.Width,
						mode_->KeyBtnConfig.Height,nullptr);
		InitMVCView(view);
		view->clear_visible_focus();
	}
	void KeyBtnControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool KeyBtnControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->KeyBtnConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->KeyBtnConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->KeyBtnConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->KeyBtnConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->KeyBtnConfig.Perm, mode_->KeyBtnConfig.X + mode_->KeyBtnConfig.OffX, mode_->KeyBtnConfig.Y + mode_->KeyBtnConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void KeyBtnControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->KeyBtnConfig.VOffX != Project::DataVarId::NullId)
			mode_->KeyBtnConfig.OffX = UI::UIData::Number<int>(mode_->KeyBtnConfig.VOffX) + pageoffx;
		else
			mode_->KeyBtnConfig.OffX = pageoffx;


		if (mode_->KeyBtnConfig.VOffY != Project::DataVarId::NullId)
			mode_->KeyBtnConfig.OffY = UI::UIData::Number<int>(mode_->KeyBtnConfig.VOffY) + pageoffy;
		else
			mode_->KeyBtnConfig.OffY = pageoffy;
	}

	void KeyBtnControl::HandleDataVar(Project::DataVarId &varId)
	{
		KeyBtnView* pView =static_cast<KeyBtnView*>(pView_);
		if (mode_->KeyBtnConfig.VOffX.Cmp(varId))
		{
			mode_->KeyBtnConfig.OffX = UI::UIData::Number<int>(mode_->KeyBtnConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->KeyBtnConfig.Perm, mode_->KeyBtnConfig.X + mode_->KeyBtnConfig.OffX, mode_->KeyBtnConfig.Y + mode_->KeyBtnConfig.OffY);
		}
		if (mode_->KeyBtnConfig.VOffY.Cmp(varId))
		{
			mode_->KeyBtnConfig.OffY = UI::UIData::Number<int>(mode_->KeyBtnConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->KeyBtnConfig.Perm, mode_->KeyBtnConfig.X + mode_->KeyBtnConfig.OffX, mode_->KeyBtnConfig.Y + mode_->KeyBtnConfig.OffY);
		}
		if (mode_->KeyBtnConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->KeyBtnConfig.Perm, pView_);
		if (mode_->KeyBtnConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->KeyBtnConfig.Perm, pView_);
	}

	int KeyBtnControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		KeyBtnView* pView = static_cast<KeyBtnView*>(pView_);

		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->HandleKeyMsg();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}

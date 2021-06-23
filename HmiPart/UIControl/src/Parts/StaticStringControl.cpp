/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建静态文字绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StaticStringControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	StaticStringControl::StaticStringControl(HMIPage* w) : BaseControl(w)
	{
		//创建StaticStringModel
		mode_ = std::shared_ptr < StaticStringModel>(new StaticStringModel());
		InitMVCModel(mode_);
	}

	void StaticStringControl::CreateView()
	{
		//初始化VIEW
		StaticStringView* view = new StaticStringView(
				mode_->StringConfig.X + mode_->StringConfig.OffX - mode_->StringConfig.Border.BorderSize / 2,
				mode_->StringConfig.Y + mode_->StringConfig.OffY - mode_->StringConfig.Border.BorderSize / 2,
				mode_->StringConfig.Width + mode_->StringConfig.Border.BorderSize,
				mode_->StringConfig.Height + mode_->StringConfig.Border.BorderSize);
		InitMVCView(view);
	}
	void StaticStringControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->StringConfig.VOffX != Project::DataVarId::NullId)
			mode_->StringConfig.OffX = UI::UIData::Number<int>(mode_->StringConfig.VOffX) + pageoffx;
		else
			mode_->StringConfig.OffX = pageoffx;


		if (mode_->StringConfig.VOffY != Project::DataVarId::NullId)
			mode_->StringConfig.OffY = UI::UIData::Number<int>(mode_->StringConfig.VOffY) + pageoffy;
		else
			mode_->StringConfig.OffY = pageoffy;
	}
	void StaticStringControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void StaticStringControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->StringConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->StringConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->StringConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->StringConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->StringConfig.Perm,
					mode_->StringConfig.X + mode_->StringConfig.OffX,
					mode_->StringConfig.Y + mode_->StringConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void StaticStringControl::HandleDataVar(Project::DataVarId &varId)
	{
		StaticStringView* pView =  static_cast<StaticStringView*>(pView_);
		if (mode_->StringConfig.VOffX.Cmp(varId))
		{
			mode_->StringConfig.OffX = UI::UIData::Number<int>(mode_->StringConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->StringConfig.Perm, 
				mode_->StringConfig.X + mode_->StringConfig.OffX - mode_->StringConfig.Border.BorderSize / 2, 
				mode_->StringConfig.Y + mode_->StringConfig.OffY - mode_->StringConfig.Border.BorderSize / 2);
		}
		if (mode_->StringConfig.VOffY.Cmp(varId))
		{
			mode_->StringConfig.OffY = UI::UIData::Number<int>(mode_->StringConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->StringConfig.Perm, 
				mode_->StringConfig.X + mode_->StringConfig.OffX - mode_->StringConfig.Border.BorderSize / 2,
				mode_->StringConfig.Y + mode_->StringConfig.OffY - mode_->StringConfig.Border.BorderSize / 2);
		}
		if (mode_->StringConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->StringConfig.Perm, pView_);
		if (mode_->StringConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->StringConfig.Perm, pView_);

	}

	int StaticStringControl::PeekHMIMessage(Message::Msg* msg)
	{
		StaticStringView* pView = static_cast<StaticStringView*>(pView_);
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

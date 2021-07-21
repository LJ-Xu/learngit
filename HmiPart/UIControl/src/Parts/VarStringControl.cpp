/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建动态文字绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "VarStringControl.h"
#include "VarStringView.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	VarStringControl::VarStringControl(HMIPage* w) : BaseControl(w)
	{
		//创建VarStringModel
		mode_ = std::shared_ptr<VarStringModel>(new VarStringModel());
		InitMVCModel(mode_);
	}

	void VarStringControl::CreateView()
	{
		//初始化VIEW
		VarStringView*  view = new VarStringView(
						mode_->VarStringConfig.X + mode_->VarStringConfig.OffX,
						mode_->VarStringConfig.Y + mode_->VarStringConfig.OffY,
						mode_->VarStringConfig.Width,
						mode_->VarStringConfig.Height);
		InitMVCView(view);
 
	}
	void VarStringControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->VarStringConfig.VOffX != Project::DataVarId::NullId)
			mode_->VarStringConfig.OffX = UI::UIData::Number<int>(mode_->VarStringConfig.VOffX) + pageoffx;
		else
			mode_->VarStringConfig.OffX = pageoffx;


		if (mode_->VarStringConfig.VOffY != Project::DataVarId::NullId)
			mode_->VarStringConfig.OffY = UI::UIData::Number<int>(mode_->VarStringConfig.VOffY) + pageoffy;
		else
			mode_->VarStringConfig.OffY = pageoffy;
	}
	void VarStringControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool VarStringControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->VarStringConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->VarStringConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->VarStringConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->VarStringConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->VarStringConfig.Perm,
					mode_->VarStringConfig.X + mode_->VarStringConfig.OffX,
					mode_->VarStringConfig.Y + mode_->VarStringConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void VarStringControl::HandleDataVar(Project::DataVarId &varId)
	{
		VarStringView* pView = static_cast<VarStringView*>(pView_);

		if (mode_->VarStringConfig.VOffX.Cmp(varId))
		{
			mode_->VarStringConfig.OffX = UI::UIData::Number<int>(mode_->VarStringConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->VarStringConfig.Perm,
				mode_->VarStringConfig.X + mode_->VarStringConfig.OffX, mode_->VarStringConfig.Y + mode_->VarStringConfig.OffY);
		}
		if (mode_->VarStringConfig.VOffY.Cmp(varId))
		{
			mode_->VarStringConfig.OffY = UI::UIData::Number<int>(mode_->VarStringConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->VarStringConfig.Perm,
				mode_->VarStringConfig.X + mode_->VarStringConfig.OffX, mode_->VarStringConfig.Y + mode_->VarStringConfig.OffY);
		}
		if (mode_->VarStringConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->VarStringConfig.Perm, pView_);
		if (mode_->VarStringConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->VarStringConfig.Perm, pView_);
		if (mode_->VarStringConfig.SrcIdRef.Cmp(varId))
		{
			double status = UI::UIData::Number<double>(mode_->VarStringConfig.SrcIdRef);
			for (size_t i = 0; i < mode_->VarStringConfig.StatusStyle.size(); i++)
			{
				if (mode_->VarStringConfig.StatusStyle[i].SrcId == status)
				{
					pView->CurrentStatus = i;
					UI::ViewShowUtility::ShowView(pView_, mode_->VarStringConfig.Perm,
						mode_->VarStringConfig.X + mode_->VarStringConfig.OffX, mode_->VarStringConfig.Y + mode_->VarStringConfig.OffY);
					break;
				}
			}
		}
	}

	int VarStringControl::PeekHMIMessage(Message::Msg* msg)
	{
		VarStringView* pView = static_cast<VarStringView*>(pView_);
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

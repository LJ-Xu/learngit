/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineControl.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 创建直线绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GLineControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "UIData.h"
#include "PermUtility.h"
namespace UI
{
	GLineControl::GLineControl(HMIPage* w) :BaseControl(w)
	{
		//创建GLineModel
		mode_ = std::shared_ptr <GLineModel>(new GLineModel());
		InitMVCModel(mode_);
	}

	void GLineControl::CreateView()
	{
		//初始化VIEW
		GLineView* view = new GLineView(
						mode_->LineConfig.X + mode_->LineConfig.OffX,
						mode_->LineConfig.Y + mode_->LineConfig.OffY,
						mode_->LineConfig.Width, 
						mode_->LineConfig.Height);
		InitMVCView(view);
	}
	void GLineControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->LineConfig.VOffX != Project::DataVarId::NullId)
			mode_->LineConfig.OffX = UI::UIData::Number<int>(mode_->LineConfig.VOffX) + pageoffx;
		else
			mode_->LineConfig.OffX = pageoffx;


		if (mode_->LineConfig.VOffY != Project::DataVarId::NullId)
			mode_->LineConfig.OffY = UI::UIData::Number<int>(mode_->LineConfig.VOffY) + pageoffy;
		else
			mode_->LineConfig.OffY = pageoffy;
	}
	void GLineControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool GLineControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->LineConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->LineConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->LineConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->LineConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->LineConfig.Perm,
					mode_->LineConfig.X + mode_->LineConfig.OffX,
					mode_->LineConfig.Y + mode_->LineConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void GLineControl::HandleDataVar(Project::DataVarId &varId)
	{
		if (mode_->LineConfig.VOffX.Cmp(varId))
		{
			mode_->LineConfig.OffX = UI::UIData::Number<int>(mode_->LineConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->LineConfig.Perm, mode_->LineConfig.X + mode_->LineConfig.OffX, mode_->LineConfig.Y + mode_->LineConfig.OffY);
		}
		if (mode_->LineConfig.VOffY.Cmp(varId))
		{
			mode_->LineConfig.OffY = UI::UIData::Number<int>(mode_->LineConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->LineConfig.Perm, mode_->LineConfig.X + mode_->LineConfig.OffX, mode_->LineConfig.Y + mode_->LineConfig.OffY);
		}
		if (mode_->LineConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->LineConfig.Perm, pView_);
		if (mode_->LineConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->LineConfig.Perm, pView_);

	}
	int GLineControl::PeekHMIMessage(Message::Msg* msg)
	{
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

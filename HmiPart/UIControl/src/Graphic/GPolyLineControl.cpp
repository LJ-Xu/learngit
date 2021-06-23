/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 创建折线多边形绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GPolyLineControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "UIData.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
namespace UI
{
	GPolyLineControl::GPolyLineControl(UI::HMIPage* w) :BaseControl(w)
	{
		//创建GPolyLineModel
		mode_ = std::shared_ptr < GPolyLineModel>(new GPolyLineModel());
		InitMVCModel(mode_);
	}

	void GPolyLineControl::CreateView()
	{
		//初始化VIEW
		GPolyLineView *view  = new GPolyLineView(
			mode_->PolyLineConfig.X + mode_->PolyLineConfig.OffX,
			mode_->PolyLineConfig.Y + mode_->PolyLineConfig.OffY,
			mode_->PolyLineConfig.Width,
			mode_->PolyLineConfig.Height);
		InitMVCView(view);
	}
	void GPolyLineControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	void GPolyLineControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->PolyLineConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->PolyLineConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->PolyLineConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->PolyLineConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->PolyLineConfig.Perm,
					mode_->PolyLineConfig.X + mode_->PolyLineConfig.OffX,
					mode_->PolyLineConfig.Y + mode_->PolyLineConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void GPolyLineControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->PolyLineConfig.VOffX != Project::DataVarId::NullId)
			mode_->PolyLineConfig.OffX = UI::UIData::Number<int>(mode_->PolyLineConfig.VOffX) + pageoffx;
		else
			mode_->PolyLineConfig.OffX = pageoffx;


		if (mode_->PolyLineConfig.VOffY != Project::DataVarId::NullId)
			mode_->PolyLineConfig.OffY = UI::UIData::Number<int>(mode_->PolyLineConfig.VOffY) + pageoffy;
		else
			mode_->PolyLineConfig.OffY = pageoffy;
	}
	void GPolyLineControl::HandleDataVar(Project::DataVarId &varId)
	{
		GPolyLineView* pView = static_cast<GPolyLineView*>(pView_);
		if (mode_->PolyLineConfig.VOffX.Cmp(varId))
		{
			mode_->PolyLineConfig.OffX = UI::UIData::Number<int>(mode_->PolyLineConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->PolyLineConfig.Perm, mode_->PolyLineConfig.X + mode_->PolyLineConfig.OffX, mode_->PolyLineConfig.Y + mode_->PolyLineConfig.OffY);
		}
		if (mode_->PolyLineConfig.VOffY.Cmp(varId))
		{
			mode_->PolyLineConfig.OffY = UI::UIData::Number<int>(mode_->PolyLineConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->PolyLineConfig.Perm, mode_->PolyLineConfig.X + mode_->PolyLineConfig.OffX, mode_->PolyLineConfig.Y + mode_->PolyLineConfig.OffY);
		}
		if (mode_->PolyLineConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->PolyLineConfig.Perm, pView_);
		if (mode_->PolyLineConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->PolyLineConfig.Perm, pView_);
	}

	int GPolyLineControl::PeekHMIMessage(Message::Msg* msg)
	{
		GPolyLineView* pView = static_cast<GPolyLineView*>(pView_);
		Project::DataVarId varId;

		switch (msg->Code)
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

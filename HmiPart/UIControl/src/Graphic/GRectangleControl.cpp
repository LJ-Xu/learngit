/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleControl.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 创建矩形绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GRectangleView.h"
#include "HMIWindow.h"
#include "GRectangleControl.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	GRectangleControl::GRectangleControl(HMIPage* w) :BaseControl(w)
	{
		//创建GRectangleModel
		mode_ = std::shared_ptr<GRectangleModel>(new GRectangleModel());
		InitMVCModel(mode_);
	}
	void GRectangleControl::CreateView()
	{
		//初始化VIEW
		GRectangleView* view = new GRectangleView(
					mode_->RectangleConfig.X + mode_->RectangleConfig.OffX,
					mode_->RectangleConfig.Y + mode_->RectangleConfig.OffY,
					mode_->RectangleConfig.Width, 
					mode_->RectangleConfig.Height);
		InitMVCView(view);
	}
	void GRectangleControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->RectangleConfig.VOffX != Project::DataVarId::NullId)
			mode_->RectangleConfig.OffX = UI::UIData::Number<int>(mode_->RectangleConfig.VOffX) + pageoffx;
		else
			mode_->RectangleConfig.OffX = pageoffx;


		if (mode_->RectangleConfig.VOffY != Project::DataVarId::NullId)
			mode_->RectangleConfig.OffY = UI::UIData::Number<int>(mode_->RectangleConfig.VOffY) + pageoffy;
		else
			mode_->RectangleConfig.OffY = pageoffy;
	}
	void GRectangleControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	void GRectangleControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->RectangleConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->RectangleConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->RectangleConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->RectangleConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->RectangleConfig.Perm,
					mode_->RectangleConfig.X + mode_->RectangleConfig.OffX,
					mode_->RectangleConfig.Y + mode_->RectangleConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	
	void GRectangleControl::HandleDataVar(Project::DataVarId &varId)
	{
		GRectangleView* pView = static_cast<GRectangleView*>(pView_);
		
		if (mode_->RectangleConfig.VOffX.Cmp(varId))
		{
			mode_->RectangleConfig.OffX = UI::UIData::Number<int>(mode_->RectangleConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->RectangleConfig.Perm, 
				mode_->RectangleConfig.X + mode_->RectangleConfig.OffX, 
				mode_->RectangleConfig.Y + mode_->RectangleConfig.OffY);
		}
		if (mode_->RectangleConfig.VOffY.Cmp(varId))
		{
			mode_->RectangleConfig.OffY = UI::UIData::Number<int>(mode_->RectangleConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->RectangleConfig.Perm,
				mode_->RectangleConfig.X + mode_->RectangleConfig.OffX,
				mode_->RectangleConfig.Y + mode_->RectangleConfig.OffY);
		}
		if (mode_->RectangleConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->RectangleConfig.Perm, pView_);
		if (mode_->RectangleConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->RectangleConfig.Perm, pView_);

	}
	int GRectangleControl::PeekHMIMessage(Message::Msg* msg)
	{
		GRectangleView* pView = static_cast<GRectangleView*>(pView_);

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

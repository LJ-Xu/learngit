/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleControl.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 创建圆绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GCircleControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{

	GCircleControl::GCircleControl(HMIPage *w) :BaseControl(w)
	{
		//创建GCircleModel
		mode_ = std::shared_ptr < GCircleModel>(new GCircleModel());
		InitMVCModel(mode_);
	}
	void GCircleControl::CreateView()
	{
		//初始化VIEW
		GCircleView* view = new GCircleView(
						mode_->CircleConfig.X + mode_->CircleConfig.OffX,
						mode_->CircleConfig.Y + mode_->CircleConfig.OffY,
						mode_->CircleConfig.Width,
						mode_->CircleConfig.Height);
		InitMVCView(view);
	}
	void GCircleControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->CircleConfig.VOffX != Project::DataVarId::NullId)
			mode_->CircleConfig.OffX = UI::UIData::Number<int>(mode_->CircleConfig.VOffX) + pageoffx;
		else
			mode_->CircleConfig.OffX = pageoffx;


		if (mode_->CircleConfig.VOffY != Project::DataVarId::NullId)
			mode_->CircleConfig.OffY = UI::UIData::Number<int>(mode_->CircleConfig.VOffY) + pageoffy;
		else
			mode_->CircleConfig.OffY = pageoffy;
	}

	void GCircleControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	void GCircleControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->CircleConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->CircleConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->CircleConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->CircleConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->CircleConfig.Perm,
					mode_->CircleConfig.X + mode_->CircleConfig.OffX,
					mode_->CircleConfig.Y + mode_->CircleConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void GCircleControl::HandleDataVar(Project::DataVarId &varId)
	{
		if (mode_->CircleConfig.VOffX.Cmp(varId))
		{
			mode_->CircleConfig.OffX = UI::UIData::Number<int>(mode_->CircleConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->CircleConfig.Perm, 
				mode_->CircleConfig.X + mode_->CircleConfig.OffX,
				mode_->CircleConfig.Y + mode_->CircleConfig.OffY);
		}
		if (mode_->CircleConfig.VOffY.Cmp(varId))
		{
			mode_->CircleConfig.OffY = UI::UIData::Number<int>(mode_->CircleConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->CircleConfig.Perm, 
				mode_->CircleConfig.X + mode_->CircleConfig.OffX,
				mode_->CircleConfig.Y + mode_->CircleConfig.OffY);
		}
		if (mode_->CircleConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->CircleConfig.Perm, pView_);
		if (mode_->CircleConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->CircleConfig.Perm, pView_);

	}
	int GCircleControl::PeekHMIMessage(Message::Msg* msg)
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

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcControl.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 圆弧的控制源码，创建圆弧绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GArcControl.h"
#include "Message.h"
#include "BaseControl.h"
#include "HMIWindow.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "Logger.h"
namespace UI
{
	GArcControl::GArcControl(HMIPage* w): BaseControl(w)
	{
		mode_ = std::shared_ptr<GArcModel>(new GArcModel());
		InitMVCModel(mode_);
    }
	void GArcControl::CreateView()
	{
		//初始化VIEW
		GArcView* view = new GArcView(
					mode_->ArcConfig.X + mode_->ArcConfig.OffX,
					mode_->ArcConfig.Y + mode_->ArcConfig.OffY,
					mode_->ArcConfig.Width, 
					mode_->ArcConfig.Height);
		InitMVCView(view);
	}
	void GArcControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->ArcConfig.VOffX != Project::DataVarId::NullId)
			mode_->ArcConfig.OffX = UI::UIData::Number<int>(mode_->ArcConfig.VOffX) + pageoffx;
		else
			mode_->ArcConfig.OffX = pageoffx;


		if (mode_->ArcConfig.VOffY != Project::DataVarId::NullId)
			mode_->ArcConfig.OffY = UI::UIData::Number<int>(mode_->ArcConfig.VOffY) + pageoffy;
		else
			mode_->ArcConfig.OffY = pageoffy;
	}
	void GArcControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool GArcControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			LOG_INFO_("GArc Change Perm\n");
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ArcConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ArcConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ArcConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->ArcConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ArcConfig.Perm,
					mode_->ArcConfig.X + mode_->ArcConfig.OffX,
					mode_->ArcConfig.Y + mode_->ArcConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void GArcControl::HandleDataVar(Project::DataVarId &varId)
	{
		GArcView* pView = static_cast<GArcView*>(pView_);
		if (mode_->ArcConfig.VOffX.Cmp(varId))
		{
			mode_->ArcConfig.OffX = UI::UIData::Number<int>(mode_->ArcConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->ArcConfig.Perm, 
				mode_->ArcConfig.X + mode_->ArcConfig.OffX, mode_->ArcConfig.Y + mode_->ArcConfig.OffY);
		}
		if (mode_->ArcConfig.VOffY.Cmp(varId))
		{
			mode_->ArcConfig.OffY = UI::UIData::Number<int>(mode_->ArcConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->ArcConfig.Perm, 
				mode_->ArcConfig.X + mode_->ArcConfig.OffX, mode_->ArcConfig.Y + mode_->ArcConfig.OffY);
		}
		if (mode_->ArcConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->ArcConfig.Perm, pView_);
		if (mode_->ArcConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->ArcConfig.Perm, pView_);

	}
	int GArcControl::PeekHMIMessage(Message::Msg* msg)
	{
		GArcView* pView = static_cast<GArcView*>(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			LOG_INFO("GArc WM_EVENT_DATEUPDATE vid = %d\n", varId.Vid);
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}

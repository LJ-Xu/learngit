/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : 创建Gif图片绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GifPicControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	GifPicControl::GifPicControl(UI::HMIPage* w) : BaseControl(w)
	{
		//创建GifPicModel
		mode_ = std::shared_ptr <GifPicModel>(new GifPicModel());
		InitMVCModel(mode_);
	}
	void GifTimerCb(void* data)
	{
		GifPicControl* ctrl = (GifPicControl*)data;
		ctrl->GifTimerData.View->CurrentPicNo += 1;
		if (ctrl->GifTimerData.View->CurrentPicNo >= ctrl->GifTimerData.View->PicNum)
			ctrl->GifTimerData.View->CurrentPicNo = 0;
		UI::ViewShowUtility::ShowView(ctrl->GifTimerData.View, ctrl->GifTimerData.Model->GifConfig.Perm, 
			                          ctrl->GifTimerData.Model->GifConfig.X + ctrl->GifTimerData.Model->GifConfig.OffX, ctrl->GifTimerData.Model->GifConfig.Y + ctrl->GifTimerData.Model->GifConfig.OffY);
		//ctrl->GifTimerData.View->redraw();
		if((size_t)ctrl->GifTimerData.View->CurrentPicNo < ctrl->GifTimerData.Model->GifConfig.DelayTime.size())
			ctrl->Page()->AddTimeout((size_t)ctrl->GifTimerData.Model->GifConfig.DelayTime[ctrl->GifTimerData.View->CurrentPicNo],
									 GifTimerCb, (void *)data, false);
	}

	void PicTimerCb(void* data)
	{
		GifPicControl* ctrl = (GifPicControl*)data;
		if (ctrl->GifTimerData.Model->GifConfig.Perm.HasShowPerm || 
			ctrl->GifTimerData.Model->GifConfig.Perm.HasLimitShowPerm)
		{
			if (ctrl->GifTimerData.View->CurrentFlash == 0)	//隐藏
			{
				ctrl->GifTimerData.View->CurrentFlash = 1;
				ctrl->GifTimerData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->GifTimerData.View, ctrl->GifTimerData.Model->GifConfig.Perm,
					ctrl->GifTimerData.Model->GifConfig.X + ctrl->GifTimerData.Model->GifConfig.OffX,
					ctrl->GifTimerData.Model->GifConfig.Y + ctrl->GifTimerData.Model->GifConfig.OffY);
			}
			else
			{
				ctrl->GifTimerData.View->CurrentFlash = 0;
				ctrl->GifTimerData.View->hide();
				ctrl->GifTimerData.View->redraw();
				ctrl->GifTimerData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->GifTimerData.View, ctrl->GifTimerData.Model->GifConfig.Perm,
					ctrl->GifTimerData.Model->GifConfig.X + ctrl->GifTimerData.Model->GifConfig.OffX,
					ctrl->GifTimerData.Model->GifConfig.Y + ctrl->GifTimerData.Model->GifConfig.OffY);
			}
		}
	}
	void GifPicControl::CreateView()
	{
		//初始化VIEW
		GifPicView *view = new GifPicView(
			mode_->GifConfig.X + mode_->GifConfig.OffX,
			mode_->GifConfig.Y + mode_->GifConfig.OffY,
			mode_->GifConfig.Width,
			mode_->GifConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
		//OnReady();
	}

	void GifPicControl::OnReady()
	{
		GifPicView* pView = static_cast<GifPicView*>(pView_);
		GifTimerData.Model = mode_.get();
		GifTimerData.View = pView;
		if (mode_->GifConfig.PicKeys.size() > 1)
		{
			if ((size_t)pView->CurrentPicNo < mode_->GifConfig.DelayTime.size())
				Page()->AddTimeout((size_t)mode_->GifConfig.DelayTime[pView->CurrentPicNo], GifTimerCb, (void *)this, false);
		}
		else
		{
			if(mode_->GifConfig.UseFlashing)
				Page()->AddTimeout((size_t)mode_->GifConfig.Frequency, PicTimerCb, (void *)this, true);
		}
		HandleSysChange(Permission);
	}
	void GifPicControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->GifConfig.VOffX != Project::DataVarId::NullId)
			mode_->GifConfig.OffX = UI::UIData::Number<int>(mode_->GifConfig.VOffX) + pageoffx;
		else
			mode_->GifConfig.OffX = pageoffx;


		if (mode_->GifConfig.VOffY != Project::DataVarId::NullId)
			mode_->GifConfig.OffY = UI::UIData::Number<int>(mode_->GifConfig.VOffY) + pageoffy;
		else
			mode_->GifConfig.OffY = pageoffy;
	}
	void GifPicControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->GifConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->GifConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->GifConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->GifConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->GifConfig.Perm, 
					mode_->GifConfig.X + mode_->GifConfig.OffX, 
					mode_->GifConfig.Y + mode_->GifConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void GifPicControl::HandleDataVar(Project::DataVarId &varId)
	{
		GifPicView* pView = static_cast<GifPicView*>(pView_);
		if (mode_->GifConfig.VOffX.Cmp(varId))
		{
			mode_->GifConfig.OffX = UI::UIData::Number<int>(mode_->GifConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->GifConfig.Perm, mode_->GifConfig.X + mode_->GifConfig.OffX, mode_->GifConfig.Y + mode_->GifConfig.OffY);
		}
		if (mode_->GifConfig.VOffY.Cmp(varId))
		{
			mode_->GifConfig.OffY = UI::UIData::Number<int>(mode_->GifConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->GifConfig.Perm, mode_->GifConfig.X + mode_->GifConfig.OffX, mode_->GifConfig.Y + mode_->GifConfig.OffY);
		}
		if (mode_->GifConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->GifConfig.Perm, pView_);
		if (mode_->GifConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->GifConfig.Perm, pView_);
	}

	int GifPicControl::PeekHMIMessage(Message::Msg* msg)
	{
		GifPicView* pView = static_cast<GifPicView*>(pView_);
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

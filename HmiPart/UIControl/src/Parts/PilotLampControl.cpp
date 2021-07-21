/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 创建指示灯绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "PilotLampControl.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{

	PilotLampControl::PilotLampControl(HMIPage* w) : BaseControl(w)
	{
		//创建PilotLampModel
		mode_ = std::shared_ptr < PilotLampModel>(new PilotLampModel());
		InitMVCModel(mode_);
	}

	void PilotLampControl::CreateView()
	{
		//初始化VIEW
		PilotLampView* view = new PilotLampView(
					mode_->PLampconfig.X + mode_->PLampconfig.OffX,
					mode_->PLampconfig.Y + mode_->PLampconfig.OffY,
					mode_->PLampconfig.Width,
					mode_->PLampconfig.Height);
		InitMVCView(view);
		if (!mode_->PLampconfig.Logic)		//负逻辑
			view->CurrentStatus = 1;	//ON状态
		else	//正逻辑
			view->CurrentStatus = 0;	//OFF状态
	}

	void PilotLampControl::OnReady()
	{
		PilotLampView* pView = static_cast<PilotLampView*>(pView_);
		LampTimerData.Model = mode_.get();
		LampTimerData.View = pView;
		HandleSysChange(Permission);

	}
	void LampFlashTimerCb(void *data)
	{
		PilotLampControl *ctrl = (PilotLampControl *)data;
		if (ctrl->LampTimerData.Model->PLampconfig.Perm.HasShowPerm)
		{
			if (ctrl->LampTimerData.View->CurrentFlash == 0)	//隐藏
			{
				ctrl->LampTimerData.View->CurrentFlash = 1;
				ctrl->LampTimerData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->LampTimerData.View, ctrl->LampTimerData.Model->PLampconfig.Perm,
					ctrl->LampTimerData.Model->PLampconfig.X + ctrl->LampTimerData.Model->PLampconfig.OffX,
					ctrl->LampTimerData.Model->PLampconfig.Y + ctrl->LampTimerData.Model->PLampconfig.OffY);
			}
			else
			{
				ctrl->LampTimerData.View->CurrentFlash = 0;
				ctrl->LampTimerData.View->hide();
				ctrl->LampTimerData.View->redraw();
				ctrl->LampTimerData.View->show();
				ctrl->LampTimerData.View->redraw();
				UI::ViewShowUtility::ShowView(ctrl->LampTimerData.View, ctrl->LampTimerData.Model->PLampconfig.Perm,
					ctrl->LampTimerData.Model->PLampconfig.X + ctrl->LampTimerData.Model->PLampconfig.OffX,
					ctrl->LampTimerData.Model->PLampconfig.Y + ctrl->LampTimerData.Model->PLampconfig.OffY);

			}
		}
	}

	bool PilotLampControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->PLampconfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->PLampconfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->PLampconfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->PLampconfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->PLampconfig.Perm,
					mode_->PLampconfig.X + mode_->PLampconfig.OffX,
					mode_->PLampconfig.Y + mode_->PLampconfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void PilotLampControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->PLampconfig.VOffX != Project::DataVarId::NullId)
			mode_->PLampconfig.OffX = UI::UIData::Number<int>(mode_->PLampconfig.VOffX) + pageoffx;
		else
			mode_->PLampconfig.OffX = pageoffx;


		if (mode_->PLampconfig.VOffY != Project::DataVarId::NullId)
			mode_->PLampconfig.OffY = UI::UIData::Number<int>(mode_->PLampconfig.VOffY) + pageoffy;
		else
			mode_->PLampconfig.OffY = pageoffy;
	}
	void PilotLampControl::HandleDataVar(Project::DataVarId &varId)
	{
		PilotLampView* pView = static_cast<PilotLampView*>(pView_);
		if (mode_->PLampconfig.VOffX.Cmp(varId))
		{
			mode_->PLampconfig.OffX = UI::UIData::Number<int>(mode_->PLampconfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->PLampconfig.Perm, mode_->PLampconfig.X + mode_->PLampconfig.OffX, mode_->PLampconfig.Y + mode_->PLampconfig.OffY);
		}
		if (mode_->PLampconfig.VOffY.Cmp(varId))
		{
			mode_->PLampconfig.OffY = UI::UIData::Number<int>(mode_->PLampconfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->PLampconfig.Perm, mode_->PLampconfig.X + mode_->PLampconfig.OffX, mode_->PLampconfig.Y + mode_->PLampconfig.OffY);
		}
		if (mode_->PLampconfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->PLampconfig.Perm, pView_);
		if (mode_->PLampconfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->PLampconfig.Perm, pView_);

		if (mode_->PLampconfig.Status.Cmp(varId))
		{
			bool status = UI::UIData::Bit(mode_->PLampconfig.Status);
			/*确定当前状态*/
			if (!mode_->PLampconfig.Logic)		//负逻辑
			{
				if (status)  //获取为true
					pView->CurrentStatus = 0;	//OFF状态
				else
					pView->CurrentStatus = 1;	//ON状态
			}
			else	//正逻辑
			{
				if (status)  //获取为true
					pView->CurrentStatus = 1;	//ON状态
				else
					pView->CurrentStatus = 0;	//OFF状态
			}
			if ((pView->CurrentStatus && mode_->PLampconfig.Flash == 1)
				|| (pView->CurrentStatus == 0 && mode_->PLampconfig.Flash == 2))	//ON或OFF状态闪烁
			{
				if (HasTimer)
				{
					Page()->RemoveTimeout(LampFlashTimerCb, (void*)this);
					if(mode_->PLampconfig.Speed != 0)
						Page()->AddTimeout((int)(mode_->PLampconfig.Speed), LampFlashTimerCb, (void*)this, true);
				}
				else
				{
					if (mode_->PLampconfig.Speed != 0)
						Page()->AddTimeout((int)(mode_->PLampconfig.Speed), LampFlashTimerCb, (void*)this, true);
					HasTimer = true;
				}
			}
			else
			{
				pView->CurrentFlash = 1;
				if (HasTimer)
					Page()->RemoveTimeout(LampFlashTimerCb, (void*)this);

			}
			UI::ViewShowUtility::ShowView(pView, mode_->PLampconfig.Perm,
				mode_->PLampconfig.X + mode_->PLampconfig.OffX,
				mode_->PLampconfig.Y + mode_->PLampconfig.OffY);

		}
		
	}

	int PilotLampControl::PeekHMIMessage(Message::Msg* msg)
	{
		PilotLampView* pView = static_cast<PilotLampView*>(pView_);
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
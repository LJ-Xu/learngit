/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 创建指示灯按键绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "BtnPilotLampControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "UIData.h"
namespace UI
{
	BtnPilotLampControl::BtnPilotLampControl(HMIPage* w) :BaseControl(w)
	{
		//创建BtnPilotLampModel
		mode_ = std::shared_ptr<BtnPilotLampModel>(new BtnPilotLampModel());
		InitMVCModel(mode_);
	}

	void BtnPilotLampControl::CreateView()
	{
		//初始化VIEW
		BtnPilotLampView* view = new BtnPilotLampView(
					mode_->BtnPilotLampConfig.X + mode_->BtnPilotLampConfig.OffX,
					mode_->BtnPilotLampConfig.Y + mode_->BtnPilotLampConfig.OffY,
					mode_->BtnPilotLampConfig.Width,
					mode_->BtnPilotLampConfig.Height, nullptr);
		InitMVCView(view);
		if (!mode_->BtnPilotLampConfig.Logic)		//负逻辑
			view->CurrentStatus = 1;	//ON状态
		else	//正逻辑
			view->CurrentStatus = 0;	//OFF状态
		//OnReady();
	}

	bool BtnPilotLampControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->BtnPilotLampConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->BtnPilotLampConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->BtnPilotLampConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->BtnPilotLampConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->BtnPilotLampConfig.Perm, mode_->BtnPilotLampConfig.X + mode_->BtnPilotLampConfig.OffX, mode_->BtnPilotLampConfig.Y + mode_->BtnPilotLampConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void BtnPilotLampControl::OnReady()
	{
		BtnPilotLampView* pView = static_cast<BtnPilotLampView*>(pView_);
		BtnLampData.Model = mode_.get();
		BtnLampData.View = pView;
		HandleSysChange(Permission);
	}
	void BtnLampFlashTimerCb(void *data)
	{
		BtnPilotLampControl *ctrl = (BtnPilotLampControl *)data;
		if (ctrl->BtnLampData.Model->BtnPilotLampConfig.Perm.HasShowPerm)
		{
			if (ctrl->BtnLampData.View->CurrentFlash == 0)	//隐藏
			{
				ctrl->BtnLampData.View->CurrentFlash = 1;
				ctrl->BtnLampData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->BtnLampData.View, ctrl->BtnLampData.Model->BtnPilotLampConfig.Perm,
					ctrl->BtnLampData.Model->BtnPilotLampConfig.X + ctrl->BtnLampData.Model->BtnPilotLampConfig.OffX,
					ctrl->BtnLampData.Model->BtnPilotLampConfig.Y + ctrl->BtnLampData.Model->BtnPilotLampConfig.OffY);

				//ctrl->BtnLampData.View->redraw();
			}
			else
			{
				ctrl->BtnLampData.View->CurrentFlash = 0;
				ctrl->BtnLampData.View->hide();
				ctrl->BtnLampData.View->redraw();
				ctrl->BtnLampData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->BtnLampData.View, ctrl->BtnLampData.Model->BtnPilotLampConfig.Perm,
					ctrl->BtnLampData.Model->BtnPilotLampConfig.X + ctrl->BtnLampData.Model->BtnPilotLampConfig.OffX,
					ctrl->BtnLampData.Model->BtnPilotLampConfig.Y + ctrl->BtnLampData.Model->BtnPilotLampConfig.OffY);
			}
		}
	}
	void BtnPilotLampControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->BtnPilotLampConfig.VOffX != Project::DataVarId::NullId)
			mode_->BtnPilotLampConfig.OffX = UI::UIData::Number<int>(mode_->BtnPilotLampConfig.VOffX) + pageoffx;
		else
			mode_->BtnPilotLampConfig.OffX = pageoffx;


		if (mode_->BtnPilotLampConfig.VOffY != Project::DataVarId::NullId)
			mode_->BtnPilotLampConfig.OffY = UI::UIData::Number<int>(mode_->BtnPilotLampConfig.VOffY) + pageoffy;
		else
			mode_->BtnPilotLampConfig.OffY = pageoffy;
	}
	void BtnPilotLampControl::HandleDataVar(Project::DataVarId &varId)
	{
		BtnPilotLampView* pView = static_cast<BtnPilotLampView*>(pView_);
		if (mode_->BtnPilotLampConfig.VOffX.Cmp(varId))
		{
			mode_->BtnPilotLampConfig.OffX = UI::UIData::Number<int>(mode_->BtnPilotLampConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->BtnPilotLampConfig.Perm, mode_->BtnPilotLampConfig.X + mode_->BtnPilotLampConfig.OffX, mode_->BtnPilotLampConfig.Y + mode_->BtnPilotLampConfig.OffY);

		}
		if (mode_->BtnPilotLampConfig.VOffY.Cmp(varId))
		{
			mode_->BtnPilotLampConfig.OffY = UI::UIData::Number<int>(mode_->BtnPilotLampConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->BtnPilotLampConfig.Perm, mode_->BtnPilotLampConfig.X + mode_->BtnPilotLampConfig.OffX, mode_->BtnPilotLampConfig.Y + mode_->BtnPilotLampConfig.OffY);
		}
		if (mode_->BtnPilotLampConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->BtnPilotLampConfig.Perm, pView_);
		if (mode_->BtnPilotLampConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->BtnPilotLampConfig.Perm, pView_);
		if (mode_->BtnPilotLampConfig.ReadVar.Cmp(varId))
		{
			bool status = UI::UIData::Bit(mode_->BtnPilotLampConfig.ReadVar);
			/*确定当前状态*/
			if (!mode_->BtnPilotLampConfig.Logic)		//负逻辑
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
			if ((pView->CurrentStatus && mode_->BtnPilotLampConfig.Flash == 1)
				|| (pView->CurrentStatus == 0 && mode_->BtnPilotLampConfig.Flash == 2))	//ON或OFF状态闪烁
			{
				if (!HasTimer)
				{
					HasTimer = true;
					if(mode_->BtnPilotLampConfig.Speed != 0)
						Page()->AddTimeout((int)(mode_->BtnPilotLampConfig.Speed), BtnLampFlashTimerCb, (void*)this, true);
				}
				else
				{
					Page()->RemoveTimeout(BtnLampFlashTimerCb, (void*)this);
					if (mode_->BtnPilotLampConfig.Speed != 0)
						Page()->AddTimeout((int)(mode_->BtnPilotLampConfig.Speed), BtnLampFlashTimerCb, (void*)this, true);
				}
			}
			else
			{
				pView->CurrentFlash = 1;
				if (HasTimer)
				{
					Page()->RemoveTimeout(BtnLampFlashTimerCb, (void*)this);
					HasTimer = false;
				}
			}
			UI::ViewShowUtility::ShowView(pView, mode_->BtnPilotLampConfig.Perm,
				mode_->BtnPilotLampConfig.X + mode_->BtnPilotLampConfig.OffX,
				mode_->BtnPilotLampConfig.Y + mode_->BtnPilotLampConfig.OffY);
			//pView->redraw();
		}
	}
	int BtnPilotLampControl::PeekHMIMessage(Message::Msg* msg)
	{
		BtnPilotLampView* pView = static_cast<BtnPilotLampView*>(pView_);
		Project::DataVarId varId;

		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->HandleClick();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
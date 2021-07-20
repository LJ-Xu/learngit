/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : ����ָʾ�ƻ��ƣ����ݴ�����Ϣ
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
		//����PilotLampModel
		mode_ = std::shared_ptr < PilotLampModel>(new PilotLampModel());
		InitMVCModel(mode_);
	}

	void PilotLampControl::CreateView()
	{
		//��ʼ��VIEW
		PilotLampView* view = new PilotLampView(
					mode_->PLampconfig.X + mode_->PLampconfig.OffX,
					mode_->PLampconfig.Y + mode_->PLampconfig.OffY,
					mode_->PLampconfig.Width,
					mode_->PLampconfig.Height);
		InitMVCView(view);
		if (!mode_->PLampconfig.Logic)		//���߼�
			view->CurrentStatus = 1;	//ON״̬
		else	//���߼�
			view->CurrentStatus = 0;	//OFF״̬
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
			if (ctrl->LampTimerData.View->CurrentFlash == 0)	//����
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
			/*�ж��Ƿ���Ȩ��*/
			if (!UI::PermUtility::HasUserPerm(mode_->PLampconfig.Perm.RequireRegion))	//û��Ȩ�޲�������
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
			/*ȷ����ǰ״̬*/
			if (!mode_->PLampconfig.Logic)		//���߼�
			{
				if (status)  //��ȡΪtrue
					pView->CurrentStatus = 0;	//OFF״̬
				else
					pView->CurrentStatus = 1;	//ON״̬
			}
			else	//���߼�
			{
				if (status)  //��ȡΪtrue
					pView->CurrentStatus = 1;	//ON״̬
				else
					pView->CurrentStatus = 0;	//OFF״̬
			}
			if ((pView->CurrentStatus && mode_->PLampconfig.Flash == 1)
				|| (pView->CurrentStatus == 0 && mode_->PLampconfig.Flash == 2))	//ON��OFF״̬��˸
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
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 创建多状态指示灯绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "MultiPilotLampControl.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{

	MultiPilotLampControl::MultiPilotLampControl(HMIPage* w) : BaseControl(w)
	{
		//创建PilotLampModel
		mode_ = std::shared_ptr <MultiPilotLampModel>(new MultiPilotLampModel());
		InitMVCModel(mode_);
	}

	void MultiPilotLampControl::CreateView()
	{
		//初始化VIEW
		MultiPilotLampView* view = new MultiPilotLampView(
			mode_->MPLampconfig.X + mode_->MPLampconfig.OffX,
			mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY,
			mode_->MPLampconfig.Width,
			mode_->MPLampconfig.Height);
		InitMVCView(view);
	}

	void MultiPilotLampControl::OnReady()
	{
		MultiPilotLampView* pView = static_cast<MultiPilotLampView*>(pView_);
		MultiLampTimerData.Model = mode_.get();
		MultiLampTimerData.View = pView;
		HandleSysChange(Permission);
	}

	
	void MultiLampFlashTimerCb(void *data)
	{
		MultiPilotLampControl *ctrl = (MultiPilotLampControl *)data;
		if (ctrl->MultiLampTimerData.Model->MPLampconfig.Perm.HasShowPerm)
		{
			if (ctrl->MultiLampTimerData.View->CurrentFlash == 0)	//隐藏
			{
				ctrl->MultiLampTimerData.View->CurrentFlash = 1;
				ctrl->MultiLampTimerData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->MultiLampTimerData.View, ctrl->MultiLampTimerData.Model->MPLampconfig.Perm,
					ctrl->MultiLampTimerData.Model->MPLampconfig.X + ctrl->MultiLampTimerData.Model->MPLampconfig.OffX,
					ctrl->MultiLampTimerData.Model->MPLampconfig.Y + ctrl->MultiLampTimerData.Model->MPLampconfig.OffY);
			}
			else
			{
				ctrl->MultiLampTimerData.View->CurrentFlash = 0;
				ctrl->MultiLampTimerData.View->hide();
				ctrl->MultiLampTimerData.View->redraw();
				ctrl->MultiLampTimerData.View->show();
				UI::ViewShowUtility::ShowView(ctrl->MultiLampTimerData.View, ctrl->MultiLampTimerData.Model->MPLampconfig.Perm,
					ctrl->MultiLampTimerData.Model->MPLampconfig.X + ctrl->MultiLampTimerData.Model->MPLampconfig.OffX,
					ctrl->MultiLampTimerData.Model->MPLampconfig.Y + ctrl->MultiLampTimerData.Model->MPLampconfig.OffY);
			}
		}
	}

	bool MultiPilotLampControl::MeetCondition(Project::StatusCondition cond, XJDataType* tp, int decimals)
	{
		if (cond.Type == 0)		//范围
		{
			switch (cond.LogicCondition)
			{
			case Project::NONE:
				if (JudgeCondition::MeetCondition(cond.ConditionA, readvalue_, cond.DataA.Value, *tp, decimals))
					return true;
				break;
			case Project::AND:
				if (JudgeCondition::MeetCondition(cond.ConditionA, readvalue_, cond.DataA.Value, *tp, decimals) &&
					JudgeCondition::MeetCondition(cond.ConditionB, readvalue_, cond.DataB.Value, *tp, decimals))
					return true;
				break;
			case Project::OR:
				if (JudgeCondition::MeetCondition(cond.ConditionA, readvalue_, cond.DataA.Value, *tp, decimals) ||
					JudgeCondition::MeetCondition(cond.ConditionB, readvalue_, cond.DataB.Value, *tp, decimals))
					return true;
				break;
			default:
				break;
			}
		}
		else
		{
			//int bitvalue = ((int)readvalue_ >> cond.Offset) & 1;//(1 << cond.Offset);
			if ((readbit_ == 1 && cond.Trigger == 0) || (readbit_ == 0 && cond.Trigger == 1))
				return true;
		}
		return false;
	}

	void MultiPilotLampControl::HandleRegChange()
	{
		MultiPilotLampView* pView = static_cast<MultiPilotLampView*>(pView_);
		//对比数据是否满足条件
		if (mode_->MPLampconfig.DataVarType == 0)		//字寄存器
		{
			if (mode_->MPLampconfig.RegVars.size() <= mode_->MPLampconfig.BitNum)
				return;
			for (size_t i = 0; i < mode_->MPLampconfig.Action.size(); i++)
			{
				int num = i + mode_->MPLampconfig.BitNum;
				if (mode_->MPLampconfig.Action[i].Type)		//位
					readbit_ = UI::UIData::Bit(mode_->MPLampconfig.RegVars[num]);
				else
					readvalue_ = UI::UIData::Number<DOUBLE>((mode_->MPLampconfig.RegVars[num]));
				XJDataType tp = UIDataService::Ins().GetDataType(mode_->MPLampconfig.RegVars[num]);
				if (MeetCondition(mode_->MPLampconfig.Action[i], &tp, mode_->MPLampconfig.Decimals))
				{
					HandleMeetCondition(i);
					return;
				}
			}
		}
		else
		{
			int value = 0;
			for (size_t i = 0; i < (size_t)mode_->MPLampconfig.BitNum; i++)
			{
				if (mode_->MPLampconfig.RegVars.size() <= i)
					return;
				if (i == mode_->MPLampconfig.BitNum - 1 && mode_->MPLampconfig.DataFormat == 0)
				{
					if (UI::UIData::Bit(mode_->MPLampconfig.RegVars[i]))
						value = 0 - value;
				}
				else
					value |= (UI::UIData::Bit(mode_->MPLampconfig.RegVars[i]) << i);
			}
			readvalue_ = (float)value;
			XJDataType tp;
			tp.Cls = VarDataType::DT_WORD;
			tp.Type = VarNumberType::NT_Unsigned;
			for (size_t i = 0; i < mode_->MPLampconfig.Action.size(); i++)
			{
				if (MeetCondition(mode_->MPLampconfig.Action[i],&tp, mode_->MPLampconfig.Decimals))
				{
					HandleMeetCondition(i);
					return;
				}
			}
		}
		XJDataType tperr;
		tperr.Cls = VarDataType::DT_WORD;
		tperr.Type = VarNumberType::NT_Unsigned;
		if (mode_->MPLampconfig.DataVarType == 0)		//字寄存器
		{
			if (mode_->MPLampconfig.RegVars.size() <= mode_->MPLampconfig.BitNum)
				return;
			int num = mode_->MPLampconfig.RegVars.size() - 1;
			//int actnum = mode_->MPLampconfig.Action.size() - 1;
			if (num < 0) return;
			if (mode_->MPLampconfig.ErrState.Type)		//位
				readbit_ = UI::UIData::Bit(mode_->MPLampconfig.RegVars[num]);
			else
				readvalue_ = UI::UIData::Number<DOUBLE>((mode_->MPLampconfig.RegVars[num]));
			tperr = UIDataService::Ins().GetDataType(mode_->MPLampconfig.RegVars[num]);
		}

		if (MeetCondition(mode_->MPLampconfig.ErrState, &tperr, mode_->MPLampconfig.Decimals))
		{
			pView->CurrentStatus = -1;
			pView->CurrentFlash = 1;
			if (HasTimer)
			{
				Page()->RemoveTimeout(MultiLampFlashTimerCb, (void*)this);
				HasTimer = false;
			}
			if (mode_->MPLampconfig.ErrState.Flash)
			{
				if (mode_->MPLampconfig.ErrState.Speed != 0)
					Page()->AddTimeout((int)mode_->MPLampconfig.ErrState.Speed, MultiLampFlashTimerCb, (void*)this, true);
			}
			if (mode_->MPLampconfig.UseErrNotice)
				UI::UIData::Bit(mode_->MPLampconfig.InputErrVarRef, true);
			UI::ViewShowUtility::ShowView(pView, mode_->MPLampconfig.Perm,
				mode_->MPLampconfig.X + mode_->MPLampconfig.OffX,
				mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);
			return;
		}
		if (mode_->MPLampconfig.InputErr == 0)
		{
			pView->CurrentStatus = -1;
			pView->CurrentFlash = 1;
			if (HasTimer)
			{
				Page()->RemoveTimeout(MultiLampFlashTimerCb, (void*)this);
				HasTimer = false;
			}
			if (mode_->MPLampconfig.ErrState.Flash)
			{
				HasTimer = true;
				if (mode_->MPLampconfig.ErrState.Speed != 0)
					Page()->AddTimeout((int)mode_->MPLampconfig.ErrState.Speed, MultiLampFlashTimerCb, (void*)this, true);
			}
			if (mode_->MPLampconfig.UseErrNotice)
				UI::UIData::Bit(mode_->MPLampconfig.InputErrVarRef, true);
			UI::ViewShowUtility::ShowView(pView, mode_->MPLampconfig.Perm,
				mode_->MPLampconfig.X + mode_->MPLampconfig.OffX,
				mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);
			return;
		}
		else
		{
			if (mode_->MPLampconfig.UseErrNotice)
				UI::UIData::Bit(mode_->MPLampconfig.InputErrVarRef, true);
		}
	}
	void MultiPilotLampControl::HandleMeetCondition(int i)
	{
		MultiPilotLampView* pView = static_cast<MultiPilotLampView*>(pView_);
		pView->CurrentStatus = i;
		pView->CurrentFlash = 1;
		if (HasTimer)
		{
			HasTimer = false;
			Page()->RemoveTimeout(MultiLampFlashTimerCb, (void*)this);
		}

		if (mode_->MPLampconfig.Action[i].Flash)
		{
			HasTimer = true;
			if (mode_->MPLampconfig.Action[i].Speed != 0)
				Page()->AddTimeout((int)mode_->MPLampconfig.Action[i].Speed, MultiLampFlashTimerCb, (void*)this, true);
		}
		if (mode_->MPLampconfig.UseErrNotice)		//取消错误通知
			UI::UIData::Bit(mode_->MPLampconfig.InputErrVarRef, false);
		UI::ViewShowUtility::ShowView(pView, mode_->MPLampconfig.Perm,
			mode_->MPLampconfig.X + mode_->MPLampconfig.OffX,
			mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);
	}
		
	
	void MultiPilotLampControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->MPLampconfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->MPLampconfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->MPLampconfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->MPLampconfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->MPLampconfig.Perm,
					mode_->MPLampconfig.X + mode_->MPLampconfig.OffX,
					mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void MultiPilotLampControl::HandleDataVar(Project::DataVarId &varId)
	{
		MultiPilotLampView* pView = static_cast<MultiPilotLampView*>(pView_);
		bool changestatus = false;
		if (mode_->MPLampconfig.VOffX.Cmp(varId))
		{
			mode_->MPLampconfig.OffX = UI::UIData::Number<int>(mode_->MPLampconfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->MPLampconfig.Perm, mode_->MPLampconfig.X + mode_->MPLampconfig.OffX, mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);
		}

		if (mode_->MPLampconfig.VOffY.Cmp(varId))
		{
			mode_->MPLampconfig.OffY = UI::UIData::Number<int>(mode_->MPLampconfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->MPLampconfig.Perm, mode_->MPLampconfig.X + mode_->MPLampconfig.OffX, mode_->MPLampconfig.Y + mode_->MPLampconfig.OffY);

		}
		if (mode_->MPLampconfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->MPLampconfig.Perm, pView_);
		if (mode_->MPLampconfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->MPLampconfig.Perm, pView_);
		if (Project::DataVarId::Cmp(mode_->MPLampconfig.RegVars, varId))
			changestatus = true;
		for (size_t i = 0; i < (size_t)mode_->MPLampconfig.StatusCnt; i++)
		{
			if (mode_->MPLampconfig.Action[i].DataA.RegVar.Cmp(varId))
			{
				changestatus = true;
				mode_->MPLampconfig.Action[i].DataA.Value = UI::UIData::Number<DOUBLE>
					(mode_->MPLampconfig.Action[i].DataA.RegVar);
			}
				
			if (mode_->MPLampconfig.Action[i].DataB.RegVar.Cmp(varId))
			{
				changestatus = true;
				mode_->MPLampconfig.Action[i].DataB.Value = UI::UIData::Number<DOUBLE>
					(mode_->MPLampconfig.Action[i].DataB.RegVar);
			}
		}
		if (mode_->MPLampconfig.ErrState.DataA.RegVar.Cmp(varId))
		{
			changestatus = true;
			mode_->MPLampconfig.ErrState.DataA.Value = UI::UIData::Number<DOUBLE>
				(mode_->MPLampconfig.ErrState.DataA.RegVar);
		}
		if (mode_->MPLampconfig.ErrState.DataB.RegVar.Cmp(varId))
		{
			changestatus = true;
			mode_->MPLampconfig.ErrState.DataB.Value = UI::UIData::Number<DOUBLE>
				(mode_->MPLampconfig.ErrState.DataB.RegVar);
		}
		if (changestatus)
			HandleRegChange();
	}
	void MultiPilotLampControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->MPLampconfig.VOffX != Project::DataVarId::NullId)
			mode_->MPLampconfig.OffX = UI::UIData::Number<int>(mode_->MPLampconfig.VOffX) + pageoffx;
		else
			mode_->MPLampconfig.OffX = pageoffx;


		if (mode_->MPLampconfig.VOffY != Project::DataVarId::NullId)
			mode_->MPLampconfig.OffY = UI::UIData::Number<int>(mode_->MPLampconfig.VOffY) + pageoffy;
		else
			mode_->MPLampconfig.OffY = pageoffy;
	}
	int MultiPilotLampControl::PeekHMIMessage(Message::Msg* msg)
	{

		MultiPilotLampView* pView = static_cast<MultiPilotLampView*>(pView_);
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
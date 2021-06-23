/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerControl.cpp
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 创建定时器部件，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "TimerControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
namespace UI
{
	TimerControl::TimerControl(UI::HMIPage* w) :BaseControl(w)
	{
		//创建TimerModel
		mode_ = std::shared_ptr <TimerModel>(new TimerModel());
		InitMVCModel(mode_);
	}

	void TimerControl::CreateView()
	{
		//初始化VIEW
		TimerView *view = new TimerView(
			mode_->TimerConfig.X + mode_->TimerConfig.OffX,
			mode_->TimerConfig.Y + mode_->TimerConfig.OffY,
			mode_->TimerConfig.Width, mode_->TimerConfig.Height,nullptr);
		InitMVCView(view);
		//OnReady();
	}
	void TimerControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->TimerConfig.VOffX != Project::DataVarId::NullId)
			mode_->TimerConfig.OffX = UI::UIData::Number<int>(mode_->TimerConfig.VOffX) + pageoffx;
		else
			mode_->TimerConfig.OffX = pageoffx;


		if (mode_->TimerConfig.VOffY != Project::DataVarId::NullId)
			mode_->TimerConfig.OffY = UI::UIData::Number<int>(mode_->TimerConfig.VOffY) + pageoffy;
		else
			mode_->TimerConfig.OffY = pageoffy;
	}
	void TimerControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Language:
			break;
		default:
			break;
		}
	}

	void TimerCountCb(void *data)
	{
		TimerControl *ctrl = (TimerControl *)data;
		ctrl->HandleTime();
	}
	

	void TimerControl::OnReady()
	{
		if (mode_->TimerConfig.PreconditionVar != Project::DataVarId::NullId)
			mode_->TimerConfig.PreconditionConstant = UI::UIData::Number<int>(mode_->TimerConfig.PreconditionVar);
		if (mode_->TimerConfig.StartConditions == Project::TimerStartCond::StartBitChange
			&& mode_->TimerConfig.StartVar != Project::DataVarId::NullId)
			perstartbitstatus_ = UI::UIData::Bit(mode_->TimerConfig.StartVar);
		if (mode_->TimerConfig.StartConditions == Project::TimerStartCond::StartValueChange
			&& mode_->TimerConfig.StartVar != Project::DataVarId::NullId)
			pervalue_ = UI::UIData::Number<double>(mode_->TimerConfig.StartVar);
		if (mode_->TimerConfig.EndConditions == Project::TimerStopCond::StopBitChange
			&& mode_->TimerConfig.EndVar != Project::DataVarId::NullId)
			perstopbitstatus_ = UI::UIData::Bit(mode_->TimerConfig.EndVar);
		if (mode_->TimerConfig.UseReset && mode_->TimerConfig.ResetVar != Project::DataVarId::NullId)
		{
			bool state = UI::UIData::Bit(mode_->TimerConfig.ResetVar);
			if ((state && mode_->TimerConfig.ResetCond == 0) ||
				(!state && mode_->TimerConfig.ResetCond == 1))
				isresettimer_ = true;
		}
		if (mode_->TimerConfig.Perm.EnableVID != Project::DataVarId::NullId)
			UI::PermUtility::HandleEnablePerm(mode_->TimerConfig.Perm, pView_);

		TimeCount = 0;
		OnStartPage();
	}
	
	void TimerControl::OnStartPage()
	{
		TimerView* pView = static_cast<TimerView*>(pView_);
		if (!hastimer_ && !isresettimer_ && mode_->TimerConfig.StartConditions == Project::TimerStartCond::TimerStartWin)		//启动定时
		{
			if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
			{
				hastimer_ = true;
				TimeCount = 0;
				Page()->AddTimeout(100, TimerCountCb, (void *)this, true);
				if (!mode_->TimerConfig.UseDelay)							//不使用延时
				{
					UI::UIData::Bit(mode_->TimerConfig.MarkVar, true);		//开始定时
					starttimer_ = true;
				}
			}
		}
	}
	void TimerControl::OnClose()
	{
		OnClosePage();
	}
	void TimerControl::OnClosePage()
	{
		TimerView* pView = static_cast<TimerView*>(pView_);
		if (mode_->TimerConfig.StartConditions == Project::TimerStartCond::StartCloseWin)	//启动定时
		{
			if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
			{
				if (!mode_->TimerConfig.UseDelay)							//不使用延时
				{
					starttimer_ = true;
					UI::UIData::Bit(mode_->TimerConfig.MarkVar, true);		//开始定时
				}
			}
		}
		if (mode_->TimerConfig.EndConditions == Project::TimerStopCond::StopCloseWin)		//结束定定时
		{
			if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
			{
				starttimer_ = false;
				UI::UIData::Bit(mode_->TimerConfig.MarkVar, false);
			}
		}
		Page()->RemoveTimeout(TimerCountCb, (void *)this);
		TimeCount = 0;
	}

	void TimerControl::HandleTime()
	{
		TimeCount++;
		if (mode_->TimerConfig.UseDelay && !starttimer_)
		{
			if (TimeCount * 100 == mode_->TimerConfig.DelayExecutionCycle * mode_->TimerConfig.CycTime)	//开始定时
			{
				UI::UIData::Bit(mode_->TimerConfig.MarkVar, true);		//开始定时
				TimeCount = 0;
				starttimer_ = true;
			}
		}

		if (starttimer_ && TimeCount * 100 == mode_->TimerConfig.CycTime * mode_->TimerConfig.PreconditionConstant)		//满足时间
		{
			if (mode_->TimerConfig.EndConditions == Project::TimerStopCond::StopReachTime)		//停止定时
			{
				hastimer_ = false;
				Page()->RemoveTimeout(TimerCountCb,(void *)this);
				UI::UIData::Bit(mode_->TimerConfig.MarkVar, false);
				starttimer_ = false;TimeCount = 0;
			}
			if (mode_->TimerConfig.PresetTimeNotifyVar != Project::DataVarId::NullId)
			{
				if (mode_->TimerConfig.NotifyMode == 0)
					UI::UIData::Bit(mode_->TimerConfig.PresetTimeNotifyVar, true);
				else
					UI::UIData::Bit(mode_->TimerConfig.PresetTimeNotifyVar, false);
			}
		}
		if (starttimer_ && mode_->TimerConfig.AlreadTimeVar != Project::DataVarId::NullId)
			UI::UIData::Number(mode_->TimerConfig.AlreadTimeVar, (int)((TimeCount * 100) / mode_->TimerConfig.CycTime));
	}
	void TimerControl::HandleDataVar(Project::DataVarId &varId)
	{
		TimerView* pView = static_cast<TimerView*>(pView_);
		if (mode_->TimerConfig.VOffX.Cmp(varId))
		{
			mode_->TimerConfig.OffX = UI::UIData::Number<int>(mode_->TimerConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->TimerConfig.Perm, mode_->TimerConfig.X + mode_->TimerConfig.OffX, mode_->TimerConfig.Y + mode_->TimerConfig.OffY);
		}
		if (mode_->TimerConfig.VOffY.Cmp(varId))
		{
			mode_->TimerConfig.OffY = UI::UIData::Number<int>(mode_->TimerConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->TimerConfig.Perm, mode_->TimerConfig.X + mode_->TimerConfig.OffX, mode_->TimerConfig.Y + mode_->TimerConfig.OffY);
		}
		if (mode_->TimerConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->TimerConfig.Perm, pView_);
		if (mode_->TimerConfig.Perm.EnableVID.Cmp(varId))
		{
			UI::PermUtility::HandleEnablePerm(mode_->TimerConfig.Perm, pView_);
			if (!pView->active())	//不使能
			{
				starttimer_ = false;
				hastimer_ = false;
				TimeCount = 0;
				Page()->RemoveTimeout(TimerCountCb, (void *)this);			//停止定时
				UI::UIData::Bit(mode_->TimerConfig.MarkVar, false);			//停止标志位
			}
		}
		if (mode_->TimerConfig.PreconditionVar.Cmp(varId))		//预设时间地址
			mode_->TimerConfig.PreconditionConstant = UI::UIData::Number<int>(mode_->TimerConfig.PreconditionVar);
		if (mode_->TimerConfig.StartVar.Cmp(varId))				//触发地址
		{
			if (mode_->TimerConfig.StartConditions == Project::TimerStartCond::StartBitChange)
			{
				bool status = UI::UIData::Bit(mode_->TimerConfig.StartVar);
				if (!isresettimer_ && !hastimer_ &&((mode_->TimerConfig.BitTriggerCond == 1 && status == false && perstartbitstatus_ == true)
					|| (mode_->TimerConfig.BitTriggerCond == 0 && status == true && perstartbitstatus_ == false)))
				{
					if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
					{
						Page()->AddTimeout(100, TimerCountCb, (void *)this, true);
						TimeCount = 0;
						hastimer_ = true;
						if (!mode_->TimerConfig.UseDelay)							//不使用延时
						{
							UI::UIData::Bit(mode_->TimerConfig.MarkVar, true);		//开始定时
							starttimer_ = true;
						}
					}
				}
				perstartbitstatus_ = status;
			}
			else if (mode_->TimerConfig.StartConditions == Project::TimerStartCond::StartValueChange)
			{
				double value = UI::UIData::Number<double>(mode_->TimerConfig.StartVar);
				bool meetcond = false;
				if (mode_->TimerConfig.UseValueEqual)
				{
					if(value == mode_->TimerConfig.ComPareValue)
						meetcond = true;
				}
				else
				{
					if(value != pervalue_)
						meetcond = true;
				}
				pervalue_ = value;
				if (!isresettimer_ && !hastimer_ && meetcond)
				{
					if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
					{
						if (!mode_->TimerConfig.UseDelay)							//不使用延时
						{
							UI::UIData::Bit(mode_->TimerConfig.MarkVar, true);		//开始定时
							starttimer_ = true;
						}
						TimeCount = 0;
						Page()->AddTimeout(100, TimerCountCb, (void *)this, true);
						hastimer_ = true;
					}
				}
			}
		}
		if (mode_->TimerConfig.EndVar.Cmp(varId))
		{
			if (mode_->TimerConfig.EndConditions == Project::TimerStopCond::StopBitChange)
			{
				bool status = UI::UIData::Bit(mode_->TimerConfig.EndVar);
				if ((mode_->TimerConfig.BitEndCond == 1 && status == false && perstopbitstatus_ == true)
					|| (mode_->TimerConfig.BitEndCond == 0 && status == true && perstopbitstatus_ == false))
				{
					Page()->RemoveTimeout(TimerCountCb, (void *)this);
					if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
						UI::UIData::Bit(mode_->TimerConfig.MarkVar, false);			//停止
					hastimer_ = false;
					starttimer_ = false;
					TimeCount = 0;
				} 
				perstopbitstatus_ = status;
			}
		}
		if (mode_->TimerConfig.ResetVar.Cmp(varId))
		{
			bool state = UI::UIData::Bit(mode_->TimerConfig.ResetVar);
			if ((state && mode_->TimerConfig.ResetCond == 0) ||
				(!state && mode_->TimerConfig.ResetCond == 1))
				isresettimer_ = true;
			else
				isresettimer_ = false;

			if (isresettimer_)
			{
				starttimer_ = false;
				hastimer_ = false;
				TimeCount = 0;
				Page()->RemoveTimeout(TimerCountCb, (void *)this);			//停止定时
				if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
					UI::UIData::Bit(mode_->TimerConfig.MarkVar, false);			//停止标志位
				if (mode_->TimerConfig.PresetTimeNotifyVar != Project::DataVarId::NullId)	//满足条件标志位复位
				{
					if (mode_->TimerConfig.NotifyMode == 0)
						UI::UIData::Bit(mode_->TimerConfig.PresetTimeNotifyVar, false);
					else
						UI::UIData::Bit(mode_->TimerConfig.PresetTimeNotifyVar, true);
				}
				if(mode_->TimerConfig.AlreadTimeVar != Project::DataVarId::NullId)		//定时时间复位
					UI::UIData::Number(mode_->TimerConfig.AlreadTimeVar, 0);
			}
			/*if (starttimer_)
			{
				bool status = UI::UIData::Bit(mode_->TimerConfig.EndVar);
				if ((mode_->TimerConfig.BitTriggerCond == 1 && status == false && perstopbitstatus_ == true)
					|| (mode_->TimerConfig.BitTriggerCond == 0 && status == true && perstopbitstatus_ == false))
				{
					if (mode_->TimerConfig.UseDelay)
						TimeCount = (mode_->TimerConfig.DelayExecutionCycle * mode_->TimerConfig.CycTime) / 100;
					else
						TimeCount = 0;
				}
			}*/
		}
		if (mode_->TimerConfig.MarkVar.Cmp(varId))
		{
			if (!mode_->TimerConfig.Perm.IsEnableVerity || pView->active())
				UI::UIData::Bit(mode_->TimerConfig.MarkVar, starttimer_);
		}
	}
	int TimerControl::PeekHMIMessage(Message::Msg* msg)
	{
		TimerModel* pMode = static_cast<TimerModel*>(pModel_.get());
		TimerView* pView = static_cast<TimerView*>(pView_);
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

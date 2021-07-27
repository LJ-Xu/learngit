/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightControl.cpp
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : 创建背景灯部件，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "BackLightControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
#include "Logger.h"
namespace UI
{
	BackLightControl::BackLightControl(UI::HMIPage* w) :BaseControl(w)
	{
		//创建BackLightModel
		mode_ = std::shared_ptr <BackLightModel>(new BackLightModel());
		InitMVCModel(mode_);
	}

	void BackLightControl::CreateView()
	{
		//初始化VIEW
		BackLightView *view = new BackLightView(
			mode_->BackLightConfig.X + mode_->BackLightConfig.OffX,
			mode_->BackLightConfig.Y + mode_->BackLightConfig.OffY,
			mode_->BackLightConfig.Width, mode_->BackLightConfig.Height);
		InitMVCView(view);
		//OnReady();
	}
	void BackLightControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->BackLightConfig.VOffX != Project::DataVarId::NullId)
			mode_->BackLightConfig.OffX = UI::UIData::Number<int>(mode_->BackLightConfig.VOffX) + pageoffx;
		else
			mode_->BackLightConfig.OffX = pageoffx;


		if (mode_->BackLightConfig.VOffY != Project::DataVarId::NullId)
			mode_->BackLightConfig.OffY = UI::UIData::Number<int>(mode_->BackLightConfig.VOffY) + pageoffy;
		else
			mode_->BackLightConfig.OffY = pageoffy;
	}
	bool BackLightControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			switch (mode_->BackLightConfig.Perm.Mode)
			{
			case '0':
			{
				/*弹出窗体*/
				break;
			}
			case '1':
			{
				/*隐藏*/
			//	pView_->hide();
			//	pView_->redraw();
				break;
			}
			default:
				break;
			}
			break;
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void BackLightControl::OnReady()
	{
		BackLightView* pView = static_cast<BackLightView*>(pView_);
		pView->resize(Page()->x(), Page()->y(), Page()->w(), Page()->h());
		pView->override();
		pView->type();
		//pView->set_modal();
	}
	
	void BackLightControl::HandleDataVar(Project::DataVarId &varId)
	{
		BackLightView* pView = static_cast<BackLightView*>(pView_);
		if (mode_->BackLightConfig.VOffX.Cmp(varId))
		{
			mode_->BackLightConfig.OffX = UI::UIData::Number<int>(mode_->BackLightConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->BackLightConfig.Perm, mode_->BackLightConfig.X + mode_->BackLightConfig.OffX, mode_->BackLightConfig.Y + mode_->BackLightConfig.OffY);
		}
		if (mode_->BackLightConfig.VOffY.Cmp(varId))
		{
			mode_->BackLightConfig.OffY = UI::UIData::Number<int>(mode_->BackLightConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->BackLightConfig.Perm, mode_->BackLightConfig.X + mode_->BackLightConfig.OffX, mode_->BackLightConfig.Y + mode_->BackLightConfig.OffY);
		}
		if (mode_->BackLightConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->BackLightConfig.Perm, pView_);
		if (mode_->BackLightConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->BackLightConfig.Perm, pView_);
		if (mode_->BackLightConfig.TrigVar.Cmp(varId))				//触发地址
		{
			bool meetcond = false;
			if (mode_->BackLightConfig.Reg == 1)			//位寄存器
			{
				bool status = UI::UIData::Bit(mode_->BackLightConfig.TrigVar);
				if ((status && mode_->BackLightConfig.BitTriggerCond == 0) ||
					(!status && mode_->BackLightConfig.BitTriggerCond == 1))		//触发
					meetcond = true;
			}
			else 
			{
				double value = UI::UIData::Number<double>(mode_->BackLightConfig.TrigVar);
				XJDataType tp = UIDataService::Ins().GetDataType(mode_->BackLightConfig.TrigVar);
				if (JudgeCondition::MeetCondition(mode_->BackLightConfig.ValueTrigCond,
					value, mode_->BackLightConfig.ComPareValue, tp))
					meetcond = true;
			}
			if (meetcond && (!mode_->BackLightConfig.Perm.IsEnableVerity || pView->active()))		//满足条件
			{
				if (mode_->BackLightConfig.Act)		//唤醒
				{
					LOG_INFO_("Wake up BackLight\n");
					pView->IsPutOutStatus = false;
				}
				else
				{
					LOG_INFO_("Put out BackLight\n");
					pView->IsPutOutStatus = true;
				}
				if (mode_->BackLightConfig.Perm.HasShowPerm && mode_->BackLightConfig.Perm.HasLimitShowPerm)
				{
					pView->show();
					pView->redraw();
				}
			}
		}
	}
	int BackLightControl::PeekHMIMessage(Message::Msg* msg)
	{
		BackLightView* pView = static_cast<BackLightView*>(pView_);
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

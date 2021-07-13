/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SysSetGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-3-23
 * Descript : 创建高级功能，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "SysSetGControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "XJMacro.h"
#include "System.h"
#include "Logger.h"
#include "SysSetHandle.h"
#include "SysCtrlApi.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#endif // WIN32

namespace UI
{
	SysSetGControl::SysSetGControl(HMIPage* w) :BaseGControl(w)
	{
		mode_ = shared_ptr<SysSetGModel>(new SysSetGModel());
		InitMVCModel(mode_);
		SysSetHandle::SetIns(this);
	}

	void SysSetGControl::CreateView() {	}

	SysSetGControl::~SysSetGControl() {	}

	void SysSetGControl::HandleDataNotify(Project::DataVarId id)
	{
		Page()->Win()->SendUserMessage((void*)CtrlId(), WM_EVENT_DATEUPDATE, id.Vid);
	}

	bool SysSetGControl::AchieveScreenSaverTime()
	{
		Project::HMIProject* prj = Win()->HMIProject();
		if(prj->SysSetting.Param.WaitTime == 0)
			return false;
		unsigned long long currenttime = System::GetCurrentTimeStampMs();
		if (currenttime - ScreenSaverClickTime >= prj->SysSetting.Param.WaitTime * 60 * 1000)
			return true;
		return false;
	}

	void SysSetGControl::TriggerScreenSaver()
	{
		Project::HMIProject* prj = Win()->HMIProject();
		if (prj->SysSetting.Param.IsViewPage)		//弹出屏保画面
		{
			LOG_INFO("Enter ScreenSaver %d\n", prj->SysSetting.Param.ViewPageIndex);
			Win()->OpenDialogPage(prj->SysSetting.Param.ViewPageIndex);
		}
		else if (prj->SysSetting.Param.IsOffBgLed)
			SysCtrlApi::CloseBacklight();
	}

	void SysSetGControl::CloseScreenSaver(int mode, int winno)
	{
		Project::HMIProject* prj = Win()->HMIProject();
		if (prj->SysSetting.Param.IsViewPage)							//弹出屏保画面
		{
			LOG_INFO("Exit ScreenSaver %d\n", prj->SysSetting.Param.ViewPageIndex);
			Win()->ClosePage(prj->SysSetting.Param.ViewPageIndex);
		}
		else if (prj->SysSetting.Param.IsOffBgLed)
			SysCtrlApi::OpenBacklight();
		ScreenSaverClickTime = System::GetCurrentTimeStampMs();			//记录时间
		if (mode)
			Win()->SwitchPage(winno);
	}

	void SysSetGControl::GetSysTime(int mode,vector<int>& time)
	{
		if (mode)	//十六进制
		{
			for (size_t i = 0; i < 6; i++)
				time.push_back(LocalData::GetNumberData<int>(SYS_PSW_TIME_DEC+1));
		}
		else
		{
			for (size_t i = 0; i < 6; i++)
				time.push_back(LocalData::GetNumberData<int>(SYS_PSW_TIME_HEX + 1));
		}
	}

	void SysSetGControl::GetExternalTime(vector<int>& time)
	{
		int size = mode_->SysSetGUnit.Clock.OutDeviceVarId.size() > 6 ? 6 : mode_->SysSetGUnit.Clock.OutDeviceVarId.size();
		for (size_t i = 0; i < size; i++)
			time.push_back(DataApi::AppNumber<int>(mode_->SysSetGUnit.Clock.OutDeviceVarId[i]));
	}

	bool SysSetGControl::WriteTimeToPlc()
	{
		for (size_t i = 0; i < mode_->SysSetGUnit.Clock.WriteInPlcDevices.size(); i++)
		{
			vector<int> time;
			GetSysTime(mode_->SysSetGUnit.Clock.WriteInPlcDevices[i].ClockWriteViewFormat, time);
			for (size_t j = 0; j < mode_->SysSetGUnit.Clock.WriteInPlcDevices[i].TimeVids.size(); j++)
			{
				if(time.size() > j)
					DataApi::AppNumber(mode_->SysSetGUnit.Clock.WriteInPlcDevices[i].TimeVids[j], time[j]);
			}
		}
		return true;
	}
	void SysSetGControl::ReportWinno(int winno)
	{
		if (mode_->SysSetGUnit.Exchange.IsReportCurPageNo
			&&mode_->SysSetGUnit.Exchange.ReportCurPageNoVarId != Project::DataVarId::NullId)
			UI::UIData::Number(mode_->SysSetGUnit.Exchange.ReportCurPageNoVarId, winno);
		LocalData::SetNumberData<int>(SYS_PSW_CURRENT_WINNO, winno);
	}
	void ScreenSaverTimer(void *param)
	{
		SysSetGControl *ctrl = (SysSetGControl *)param;
		if(ctrl->AchieveScreenSaverTime())
			ctrl->TriggerScreenSaver();	//触发屏保
	}
	void HandleSetTimer(void *param)
	{
		SysSetGControl *ctrl = (SysSetGControl *)param;
			ctrl->WriteTimeToPlc();	//写入时间
	}
	void SysSetGControl::OnReady()
	{
		Project::HMIProject* prj = Win()->HMIProject();
		ScreenSaverClickTime  = System::GetCurrentTimeStampMs();
		/*设置鼠标*/
		if (prj->SysSetting.Param.IsHideCursor)		//隐藏鼠标
		{
#ifndef WIN32
			int root_window = DefaultRootWindow(fl_display);        //获取当前窗
			XFixesShowCursor(fl_display, root_window);
			XFixesHideCursor(fl_display, root_window);
#endif // WIN32

		}
		/*设置蜂鸣器*/
		if (prj->SysSetting.Param.DisableBuzzer)			//不使能蜂鸣器
			LocalData::SetBit(SYS_PSB_BEEP_DISABLE, true);
		else
			LocalData::SetBit(SYS_PSB_BEEP_DISABLE, false);
		/*设置时间*/
		if (!mode_->SysSetGUnit.Clock.IsbanClockSet)
		{
			/*获取时钟*/
			if (mode_->SysSetGUnit.Clock.ClockSource)	//外部时钟
			{
				vector<int> time;
				GetExternalTime(time);
				SysCtrlApi::SwitchSysTime(mode_->SysSetGUnit.Clock.ClockSource, mode_->SysSetGUnit.Clock.ClockViewFormat, &time);
			}
			else //内部时钟
				SysCtrlApi::SwitchSysTime(mode_->SysSetGUnit.Clock.ClockSource, mode_->SysSetGUnit.Clock.ClockViewFormat);

			/*写入时钟*/
			if (mode_->SysSetGUnit.Clock.IsClockWriteinPlc &&
				mode_->SysSetGUnit.Clock.WriteInPlcDevicesNum != 0)		//写入外部
			{
				switch (mode_->SysSetGUnit.Clock.WriteMode)
				{
				case Project::ClockWriteMode::ClockWriteContinue:
					Page()->AddTimeout(500, HandleSetTimer, (void *)this, true);
					break;
				case Project::ClockWriteMode::ClockWriteCycle:
					if (mode_->SysSetGUnit.Clock.CycleVarId != Project::DataVarId::NullId)
						mode_->SysSetGUnit.Clock.CycleUnit = UI::DataApi::AppNumber<int>(mode_->SysSetGUnit.Clock.CycleVarId);
					if(mode_->SysSetGUnit.Clock.CycleUnit > 0)
						Page()->AddTimeout(mode_->SysSetGUnit.Clock.CycleUnit, HandleSetTimer, (void *)this, true);
					break;
				case Project::ClockWriteMode::ClockWriteTrigger:
					preclockflag_ = UI::DataApi::AppBit(mode_->SysSetGUnit.Clock.TriReadAddrVarId);
					break;
				default:
					break;
				}
			}
		}
		/*设置屏保*/
		if (prj->SysSetting.Param.WaitTime != 0)
			Page()->AddTimeout(1000, ScreenSaverTimer, (void *)this, true);
	}
	void SysSetGControl::HandleDataVar(Project::DataVarId &id)
	{
		if (mode_->SysSetGUnit.Exchange.CtrlPageExchangeVarId.Cmp(id)
			&& mode_->SysSetGUnit.Exchange.IsCtrlPageExchange)
		{
			int winno = UI::UIData::Number<int>(mode_->SysSetGUnit.Exchange.CtrlPageExchangeVarId);
			Win()->SwitchPage(winno);
		}
		if (mode_->SysSetGUnit.Clock.CycleVarId.Cmp(id))
		{
			mode_->SysSetGUnit.Clock.CycleUnit = DataApi::AppNumber<int>(mode_->SysSetGUnit.Clock.CycleVarId);
			if (mode_->SysSetGUnit.Clock.CycleUnit > 0)
			{
				Page()->RemoveTimeout(HandleSetTimer, (void *)this);
				Page()->AddTimeout(mode_->SysSetGUnit.Clock.CycleUnit, ScreenSaverTimer, (void *)this, true);
			}
		}
		if (mode_->SysSetGUnit.Clock.TriReadAddrVarId.Cmp(id))
		{
			bool state = DataApi::AppBit(mode_->SysSetGUnit.Clock.TriReadAddrVarId);
			if ((state == false && preclockflag_ == true && mode_->SysSetGUnit.Clock.TriPattern == 1) ||
				(state == true && preclockflag_ == false && mode_->SysSetGUnit.Clock.TriPattern == 0))	 //上升沿
				WriteTimeToPlc();
			preclockflag_ = state;
		}
	}
	int SysSetGControl::PeekHMIMessage(Message::Msg* msg)
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

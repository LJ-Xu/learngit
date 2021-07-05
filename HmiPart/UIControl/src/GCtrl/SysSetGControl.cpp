/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SysSetGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-3-23
 * Descript : �����߼����ܣ����ݴ�����Ϣ
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
#include "ScreenSaver.h"
#ifdef WIN32
#include <Windows.h>
#else
#include <X11/Xcursor/Xcursor.h>
#endif // WIN32

namespace UI
{
	SysSetGControl::SysSetGControl(HMIPage* w) :BaseGControl(w)
	{
		mode_ = shared_ptr<SysSetGModel>(new SysSetGModel());
		InitMVCModel(mode_);
		ScreenSaver::SetIns(this);
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
		if (prj->SysSetting.Param.IsViewPage)		//������������
		{
			LOG_INFO("Enter ScreenSaver %d\n", prj->SysSetting.Param.ViewPageIndex);
			Win()->OpenDialogPage(prj->SysSetting.Param.ViewPageIndex);
		}
	}

	void SysSetGControl::CloseScreenSaver(int mode, int winno)
	{
		Project::HMIProject* prj = Win()->HMIProject();
		if (prj->SysSetting.Param.IsViewPage)							//������������
		{
			LOG_INFO("Exit ScreenSaver %d\n", prj->SysSetting.Param.ViewPageIndex);
			Win()->ClosePage(prj->SysSetting.Param.ViewPageIndex);
		}
		ScreenSaverClickTime = System::GetCurrentTimeStampMs();			//��¼ʱ��
		if (mode)
			Win()->SwitchPage(winno);
	}

	void ScreenSaverTimer(void *param)
	{
		SysSetGControl *ctrl = (SysSetGControl *)param;
		if(ctrl->AchieveScreenSaverTime())
			ctrl->TriggerScreenSaver();	//��������
	}
	void SysSetGControl::OnReady()
	{
		Project::HMIProject* prj = Win()->HMIProject();
		ScreenSaverClickTime  = System::GetCurrentTimeStampMs();
		/*�������*/
		if (prj->SysSetting.Param.IsHideCursor)		//�������
		{
#ifndef WIN32
			int root_window = DefaultRootWindow(fl_display);        //��ȡ��ǰ��
			XFixesShowCursor(fl_display, root_window);
			XFixesHideCursor(fl_display, root_window);
#endif // WIN32

		}
		/*���÷�����*/
		if (prj->SysSetting.Param.DisableBuzzer)			//��ʹ�ܷ�����
			LocalData::SetBit(SYS_PSB_BEEP_DISABLE, true);
		else
			LocalData::SetBit(SYS_PSB_BEEP_DISABLE, false);
		/*��������*/
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

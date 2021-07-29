/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AdvanceFuncGControl.cpp
 * Author   : qiaodan
 * Date     : 2021-3-23
 * Descript : 创建高级功能，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "AdvanceFuncGControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ProjectDataTables.h"
#include "XJMacro.h"
#include "System.h"
#include "UICommand.h"
#include "Logger.h"
namespace UI
{
	AdvanceFuncGControl::AdvanceFuncGControl(HMIPage* w) :BaseGControl(w)
	{
		mode_ = shared_ptr<AdvanceFuncGModel>(new AdvanceFuncGModel());
		InitMVCModel(mode_);
	}
	void AdvanceFuncGControl::CreateView()	{	}


	void AdvanceFuncGControl::HandleCommunicationSet(Project::AdvFuncSetup param)
	{
		
	}
	void AdvanceFuncGControl::HandleFlashDiskSet(Project::AdvFuncSetup param)
	{
		
	}
	void AdvanceFuncGControl::HandleSDCardSet(Project::AdvFuncSetup param)
	{
		
	}
	
	void AdvanceFuncGControl::HandleSystemParam(Project::AdvFuncSetup param)
	{
		
	}
	void AdvanceFuncGControl::HandleRestart(Project::AdvFuncSetup param)
	{

	}

	void AdvanceFuncGControl::HandleCallMacros(Project::AdvFuncSetup param)
	{
		UI::XJMacro::Trig_Macro(param.CallMacros.MacroName);
	}
	void AdvanceFuncGControl::HandleCallWindow(Project::AdvFuncSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		switch (param.CallWindow.CallWindowMode)
		{
		case Project::WinAction::SWITCH:
		{
			Win()->SwitchPage(param.CallWindow.WindowNumber);
			break;
		}
		case Project::WinAction::RETURN:
		{
			Win()->GobackPage();
			break;
		}
		case Project::WinAction::OPEN:
			Win()->OpenDialogPage(param.CallWindow.WindowNumber);
			break;
		default:
			break;
		}
		if (param.CallWindow.IsShowPasswordWindow)		//打开密码页面
			Win()->OpenDialogPage(2);
	}
	void AdvanceFuncGControl::HandleCloseWindow(Project::AdvFuncSetup param)
	{
		int curwinno = Page()->Winno();			//获取当前页面
		if (param.CloseWindow.CloseWindowMode)
			Win()->ClosePage(param.CloseWindow.AssignCloseWindowNumber);
		else
			Win()->CloseAllDialog();
	}
	void AdvanceFuncGControl::HandleDataTransfer(Project::AdvFuncSetup param)
	{
		UI::UIDataService::Ins().MoveDataVars(param.DataTransfer.SrcVar, param.DataTransfer.DstVar, param.DataTransfer.RegNum);
	}
	void AchieveConfirmTimerCb(void *data)			//达到定时
	{
		LOG_INFO_("Achieve Confirm Time\n");
		LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::OverTime);
		//AdvanceFuncGControl *ctrl = (AdvanceFuncGControl *)data;
		//ctrl->Win()->ClosePage(SafeConfirmWinNum);
	}
	void AdvanceFuncGControl::HandleCallSystemCommand(Project::AdvFuncSetup param)
	{
		vector<string> data(0);
		string name = param.CallSystemCommand.CommandName;
		if (name == "LogInUser")
			GetCommandDatas[param.CallSystemCommand.CommandName]("", data);
		if (name == "LogOutUser")
			GetCommandDatas[param.CallSystemCommand.CommandName]("", data);
		if (name == "LogChanged")
		{
			LOG_INFO_("Log Changed\n");
			bool bit = LocalData::GetBit(SYS_PSB_HASLOGINUSER);
			int status = LocalData::GetNumberData<int>(SYS_PSW_LOGINFlag);
			if (status == 5)	//退出登陆
			{
				Win()->NotifySysChange(SysChangeEM::ExitLog);
				return;
			}
			if (status != 0)		//有状态改变
			{
				char login[32] = { 0 };
				//LocalData::SetString(SYS_PSW_LOGINName, login);	//设置登陆账户为空
				LocalData::SetString(SYS_PSW_LOGINPwd, login);		//设置登陆密码为空
				Win()->NotifySysChange(SysChangeEM::Permission);
			}
		}
		if (name == "ConfirmOperate")
		{
			PremConfirm status = (PremConfirm)LocalData::GetNumberData<int>(SYS_PSW_ConfirmOperate);
			switch (status)
			{
			case UI::TriggerOperate:
			{
				LOG_INFO("Open Condirm Win %d\n", SafeConfirmWinNum);
				//LocalData::SetNumberData<int>(SYS_PSW_ConfirmOperate, 6);
				Win()->OpenDialogPage(SafeConfirmWinNum);			//打开操作确认页面
				int time = LocalData::GetNumberData<int>(SYS_PSW_ConfirmTime);
				Page()->AddTimeout(time * 1000, AchieveConfirmTimerCb, (void *)this);
				break;
			}
			case UI::ConfirmOperate:
			{
				//LocalData::SetNumberData<int>(SYS_PSW_ConfirmOperate, 6);
				Page()->RemoveTimeout(AchieveConfirmTimerCb, (void *)this);
				Project::WinCtrlID ctrlid = LocalData::GetNumberData<unsigned int>(SYS_PSW_OperateCtrlID);
				Win()->SendUserMessage((void*)ctrlid, WM_EVENT_CLICKED, 0);
				Win()->ClosePage(SafeConfirmWinNum);
				break;
			}
			case UI::CancelOperate:
			{
				//LocalData::SetNumberData<int>(SYS_PSW_ConfirmOperate, 6);
				Page()->RemoveTimeout(AchieveConfirmTimerCb, (void *)this);
				Win()->ClosePage(SafeConfirmWinNum);
				break;
			}
			case UI::OverTime:
				Win()->ClosePage(SafeConfirmWinNum);
				break;
			case UI::ConfirmParam:
				LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::TriggerOperate);		//设置触发
				break;
			default:
				break;
			}

			//GetCommandDatas[param.CallSystemCommand.CommandName]("", data);
		}
		if (name == "PopCommuFailWin")
		{
			LocalData::SetBit(SYS_PSB_COMMPORT_FAIL, false);
			Win()->OpenDialogPage(COMMUFailWinNum);
		}
	}

	
	void AdvanceFuncGControl::HandleDataNotify(Project::DataVarId id) 
	{
		Page()->Win()->SendUserMessage((void*)CtrlId(), WM_EVENT_DATEUPDATE, id.Vid);
	}
	void AdvanceFuncGControl::HandleDataVar(Project::DataVarId &id)
	{
		for (size_t i = 0; i < mode_->AdvFuncGConfig.Functions.size(); i++)
		{
			bool meetcond = false;
			if (mode_->AdvFuncGConfig.Functions[i].TrigDatavrIdRef.Cmp(id))
			{
				if (mode_->AdvFuncGConfig.Functions[i].Trigmode)		//字触发
				{
					double value = UI::UIData::Number<DOUBLE>(mode_->AdvFuncGConfig.Functions[i].TrigDatavrIdRef);
					/*判断是否满足条件*/
					XJDataType tp = UIDataService::Ins().GetDataType(mode_->AdvFuncGConfig.Functions[i].TrigDatavrIdRef);
					if(JudgeCondition::MeetCondition(mode_->AdvFuncGConfig.Functions[i].WordTrigCondition,
						value, mode_->AdvFuncGConfig.Functions[i].WordTrigValue,tp))
						meetcond = true;
				}
				else
				{
					bool coilView = UI::UIData::Bit(mode_->AdvFuncGConfig.Functions[i].TrigDatavrIdRef);
					if ((coilView == true && mode_->AdvFuncGConfig.Functions[i].Precoilstatus == false && mode_->AdvFuncGConfig.Functions[i].BitTrigCondition == 1) ||
						(coilView == false && mode_->AdvFuncGConfig.Functions[i].Precoilstatus == true && mode_->AdvFuncGConfig.Functions[i].BitTrigCondition == 0) ||
						(coilView != mode_->AdvFuncGConfig.Functions[i].Precoilstatus && mode_->AdvFuncGConfig.Functions[i].BitTrigCondition == 2))
						meetcond = true;
					mode_->AdvFuncGConfig.Functions[i].Precoilstatus = coilView;
				}
				if (meetcond)
				{
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "CommunicationSetting")
						HandleCommunicationSet(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "FlashDiskSetting")
						HandleFlashDiskSet(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "SDCardSetting")
						HandleSDCardSet(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "SystemParam")
						HandleSystemParam(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "Restart")
						HandleRestart(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "CallMacros")
						HandleCallMacros(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "CallWindow")
						HandleCallWindow(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "CloseWindow")
						HandleCloseWindow(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "DataTransfer")
						HandleDataTransfer(mode_->AdvFuncGConfig.Functions[i].FunctionParam);
					if (mode_->AdvFuncGConfig.Functions[i].FunctionName == "CallSystemCommand")
						HandleCallSystemCommand(mode_->AdvFuncGConfig.Functions[i].FunctionParam);

				}
			}
		}
	}
	int AdvanceFuncGControl::PeekHMIMessage(Message::Msg* msg)
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

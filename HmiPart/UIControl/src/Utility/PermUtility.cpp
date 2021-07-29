/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PermUtility.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 权限处理
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "PermUtility.h"
#include "UIData.h"
#include "UICommand.h"
#include "BaseControl.h"
#include "HMIWindow.h"
namespace UI
{
	void PermUtility::HandleShowPerm(Project::Permission& perm, HMIBaseView *view)
	{
		if (!perm.IsShowVerity)
			return;
		bool isShow = UI::UIData::Bit(perm.ShowVID);
		if ((isShow && perm.ShowLogic == 1) || (!isShow && perm.ShowLogic == 0))
		{
			//行为模式 0:关闭时任然显示  1:关闭时隐藏   
			switch (perm.DisplayMode)
			{
			case 0:
			{
				if(perm.HasLimitShowPerm)
					view->show();
				view->redraw();
				perm.HasShowPerm = true;
				break;
			}
			case 1:
			{
				view->hide();
				view->redraw();
				perm.HasShowPerm = false;
				break;
			}
			case 2:
			{
				perm.HasShowPerm = true;
				view->deactivate();
				view->redraw();
				perm.HasShowEnablePerm = false;
				break;
			}
			default:
				break;
			}
		}
		else
		{
			perm.HasShowPerm = true;
			perm.HasShowEnablePerm = true;
			if(perm.HasEnablePerm)
				view->activate();
			if(perm.HasLimitShowPerm)
				view->show();
			view->redraw();
		}
		
	}
	void PermUtility::HandleEnablePerm(Project::Permission& perm, HMIBaseView *view)
	{
		if (!perm.IsEnableVerity)
			return;
		int a = UI::UIData::Number<int>(perm.EnableVID);
		bool isEnable = UI::UIData::Number<int>(perm.EnableVID);
		if ((isEnable && perm.EnableLogic == 1) || (!isEnable && perm.EnableLogic == 0))
		{
			view->deactivate();
			view->redraw();
			perm.HasEnablePerm = false;
		}
		else
		{
			if (perm.HasShowEnablePerm)
				view->activate();
			view->redraw();
			perm.HasEnablePerm = true;
		}
	}
	bool PermUtility::HasUserPerm(int LimitRegion)
	{
		/*获取当前登陆用户名*/
		if (LimitRegion == 0)
			return true;
		//if (!LocalData::GetBit(SYS_PSW_HASLOGINUSER))	//没有用户登陆
		//	return false;
		int len = LocalData::GetLocalVarLen(SYS_PSW_LoginedName);
		char *username = new char[len + 1];
		memset(username, '\0', len + 1);
		LocalData::GetString(SYS_PSW_LoginedName, username);
		if (!username)
		{
			delete[] username;
			return false;
		}
		else
		{
			if (!UICommand::Ins())
			{
				delete[] username;
				return true;
			}
			int region = UICommand::Ins()->GetUserLimitRegion(username);
			if (region & (1 << LimitRegion))
			{
				delete[] username;
				return true;
			}
			else
			{
				delete[] username;
				return false;
			}
		}
	}
	bool PermUtility::HandleConfirmPerm(Project::Permission& perm, Project::WinCtrlID ctrlid)
	{
		if (!perm.IsOperatecfm)
			return true;		//不需要操作确认返回true
		//ctr->Win()->OpenDialogPage(SafeConfirmWinNum);
		int status = LocalData::GetNumberData<int>(SYS_PSW_ConfirmOperate);
		if(status == (int)PremConfirm::TriggerOperate)
			LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::ConfirmParam);		//设置触发
		else
			LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::TriggerOperate);		//设置触发

		//LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::OverTime);		//设置触发
		//LocalData::SetNumberData(SYS_PSW_ConfirmOperate, (int)PremConfirm::TriggerOperate);		//设置触发
		LocalData::SetNumberData(SYS_PSW_OperateCtrlID, ctrlid);							//绑定ctrid
		LocalData::SetNumberData(SYS_PSW_ConfirmTime, perm.WaitTime);								//设置时间
		LocalData::SetNumberData(SYS_PSW_ConfirmOperateTime, perm.WaitTime);								//设置时间
		
		return false;
	}

	void PermUtility::QuitLimitPerm(Project::Permission& perm)
	{
		if (perm.RequireRegion != 0 && perm.QuitLimit)					//退出限制
			perm.RequireRegion = 0;
	}

}
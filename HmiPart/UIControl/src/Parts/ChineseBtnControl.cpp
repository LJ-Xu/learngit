/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnControl.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 创建中文按键，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ChineseBtnControl.h"
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "PinYinWindow.h"
namespace UI
{
	ChineseBtnControl::ChineseBtnControl(HMIPage* w) :BaseControl(w)
	{
		//创建buttonmodel
		mode_ = std::shared_ptr < ChineseBtnModel>(new ChineseBtnModel());
		InitMVCModel(mode_);
	}
	void ChineseBtnControl::CreateView()
	{
		//创建buttonview
		ChineseBtnView* view = new ChineseBtnView(mode_->BtnConfig.X, mode_->BtnConfig.Y,
			mode_->BtnConfig.Width, mode_->BtnConfig.Height, nullptr);
		InitMVCView(view);
	}

	
	void ChineseBtnControl::AddVarData(vector<Project::DataVarId*> vars)
	{

	}

	void ChineseBtnControl::SendMsg(int message, int lparam)
	{
		ChineseBtnView* pView = static_cast<ChineseBtnView*>(pView_);
		PinYinWindow *win;
		switch (message)
		{
		case BTN_PREV:
			win = (PinYinWindow *)Win();
			win->SearchChineseCharacters(-1,win->PinYin);
			//Win()->SendUserMessage((void*)this->CtrlId(), message, (int)pView->label());
			break;
		case BTN_NEXT:
			win = (PinYinWindow *)Win();
			win->SearchChineseCharacters(1, win->PinYin);
			//Win()->SendUserMessage((void*)this->CtrlId(), message, (int)pView->label());
			break;
		case BTN_VALUE:
			//Win()->SendUserMessage((void*)Win()->KeyboardId(), BTN_SET_CHINESE, (int)pView->label());
		default:
			break;
		}
	}
	ChineseBtnControl::DataVarType ChineseBtnControl::GetDataVarType(Project::DataVarId &varId)
	{
		if (mode_->BtnConfig.VOffX.Cmp(varId))
			return (DataVarType)OFFX;
		else if (mode_->BtnConfig.VOffY.Cmp(varId))
			return (DataVarType)OFFY;
		else if (mode_->BtnConfig.Perm.Permission.PermData.Cmp(varId))
			return (DataVarType)PERM;
		else
			return (DataVarType)NO;
	}
	void ChineseBtnControl::HandleDataVar(DataVarType type, Project::DataVarId &varId)
	{
		ChineseBtnView* pView = static_cast<ChineseBtnView*>(pView_);
		switch (type)
		{
		case UI::ChineseBtnControl::OFFX:
			mode_->BtnConfig.OffX = UIDataService::Ins().GetNumber<int>(mode_->BtnConfig.VOffX);
			RedrawMove(mode_->BtnConfig.X + mode_->BtnConfig.OffX, mode_->BtnConfig.Y + mode_->BtnConfig.OffY);
			break;
		case UI::ChineseBtnControl::OFFY:
			mode_->BtnConfig.OffY = UIDataService::Ins().GetNumber<int>(mode_->BtnConfig.VOffY);
			RedrawMove(mode_->BtnConfig.X + mode_->BtnConfig.OffX, mode_->BtnConfig.Y + mode_->BtnConfig.OffY);
			break;
		case UI::ChineseBtnControl::PERM:
			if (mode_->BtnConfig.Perm.Permission.GetDisplayPms())	//可以显示
			{
				pView_->show();
				pView_->redraw();
			}
			else {
				pView_->hide();
				pView_->redraw();
			}
			if (mode_->BtnConfig.Perm.Permission.GetUsePms())		//可以使用
			{
				pView_->activate();
				pView_->redraw();
			}
			else
			{
				pView_->deactivate();
				pView_->redraw();
			}
			break;
		case UI::ChineseBtnControl::NO:
			break;
		default:
			break;
		}

	}

	int ChineseBtnControl::PeekHMIMessage(Message::Msg* msg)
	{
		ChineseBtnView* pView = static_cast<ChineseBtnView*>(pView_);
		Project::DataVarId varId;
		DataVarType type;
		bool permission;
		ChineseMsg *label;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			type = GetDataVarType(varId);
			HandleDataVar(type, varId);
			break;
		case WM_CHANGE_PERM:
			permission = IPermission::Ins()->HasPerm(mode_->BtnConfig.Perm.RequireRegion);
			HandlePermChange(mode_->BtnConfig.Perm.ActionMode, permission);
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
		case BTN_GET_CHINESE:
			label = (ChineseMsg*)msg->LParam;
			mode_->BtnConfig.text = label->chinese;
			pView->redraw();
			//pView->label(label->chinese.c_str());
			break;
		case BTN_SET_ENABLE:
			if (msg->LParam == 0)
				pView->deactivate();
			else
				pView->activate();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
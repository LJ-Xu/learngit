/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmDisplayControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-05
 * Descript : 创建报警显示绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "AlarmDisplayControl.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "HMIWindow.h"
#include "Message.h"
#include "AlarmInputControl.h"
#include "UIComm.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
#include "Logger.h"
namespace UI
{
	AlarmDisplayControl::AlarmDisplayControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<AlarmDisplayModel>(new AlarmDisplayModel());
		InitMVCModel(mode_);
	}

	void AlarmDisplayControl::CreateView()
	{
		//初始化VIEW
		AlarmDisplayView* view;
			//绘制标题
		view = new AlarmDisplayView(
			mode_->AlarmDisConfig.X + mode_->AlarmDisConfig.OffX,
			mode_->AlarmDisConfig.Y + mode_->AlarmDisConfig.OffY,
			mode_->AlarmDisConfig.Width,
			mode_->AlarmDisConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}
	void AlarmDisplayControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->AlarmDisConfig.VOffX != Project::DataVarId::NullId)
			mode_->AlarmDisConfig.OffX = UI::UIData::Number<int>(mode_->AlarmDisConfig.VOffX) + pageoffx;
		else
			mode_->AlarmDisConfig.OffX = pageoffx;


		if (mode_->AlarmDisConfig.VOffY != Project::DataVarId::NullId)
			mode_->AlarmDisConfig.OffY = UI::UIData::Number<int>(mode_->AlarmDisConfig.VOffY) + pageoffy;
		else
			mode_->AlarmDisConfig.OffY = pageoffy;
	}
	void AlarmDisplayControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void AlarmDisplayControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->AlarmDisConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->AlarmDisConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->AlarmDisConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->AlarmDisConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->AlarmDisConfig.Perm, mode_->AlarmDisConfig.X + mode_->AlarmDisConfig.OffX, mode_->AlarmDisConfig.Y + mode_->AlarmDisConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}	
	void AlarmDisplayControl::GetSearchRegData()
	{
		AlarmDisplayView* pView = static_cast<AlarmDisplayView*>(pView_);
		switch (mode_->AlarmDisConfig.SearchMode)
		{
		case Project::AlarmSeekDATE:
		{
			int time[3] = { 0 };
			tm searchlocal;
			int size = 3 < mode_->AlarmDisConfig.SearchRegVarId.size() ? 3 : mode_->AlarmDisConfig.SearchRegVarId.size();
			for (unsigned i = 0; i < size;i++)
				time[i] = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchRegVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->AlarmDisConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			break;
		}
		case Project::AlarmSeekTIME:
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->AlarmDisConfig.SearchRegVarId.size();i++)
				time[i] =UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchRegVarId[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->AlarmDisConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->AlarmDisConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			break;
		}
		case Project::AlarmSeekGROUP:
		{
			if (mode_->AlarmDisConfig.SearchRegVarId.empty())
				return;
			mode_->AlarmDisConfig.SearchGroup = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchRegVarId[0]);
			break;
		}
		case Project::AlarmSeekNUM:
		{
			if (mode_->AlarmDisConfig.SearchRegVarId.empty())
				return;
			mode_->AlarmDisConfig.SearchNo = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchRegVarId[0]);
			break;
		}
		case Project::AlarmSeekLEVEL:
			if (mode_->AlarmDisConfig.SearchRegVarId.empty())
				return;
			mode_->AlarmDisConfig.SearchLevel = (Project::AlarmLevel)UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchRegVarId[0]);
			break;
		default:
			break;
		}
	}

	void AlarmDisplayControl::HandleDataVar(Project::DataVarId &varId)
	{
		AlarmDisplayView* pView = static_cast<AlarmDisplayView*>(pView_);
		if (mode_->AlarmDisConfig.VOffX.Cmp(varId))
		{
			mode_->AlarmDisConfig.OffX = UI::UIData::Number<int>(mode_->AlarmDisConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->AlarmDisConfig.Perm, mode_->AlarmDisConfig.X + mode_->AlarmDisConfig.OffX, mode_->AlarmDisConfig.Y + mode_->AlarmDisConfig.OffY);
		}
		if (mode_->AlarmDisConfig.VOffY.Cmp(varId))
		{
			mode_->AlarmDisConfig.OffY = UI::UIData::Number<int>(mode_->AlarmDisConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->AlarmDisConfig.Perm, mode_->AlarmDisConfig.X + mode_->AlarmDisConfig.OffX, mode_->AlarmDisConfig.Y + mode_->AlarmDisConfig.OffY);
		}
		if (mode_->AlarmDisConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->AlarmDisConfig.Perm, pView_);
		if (mode_->AlarmDisConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->AlarmDisConfig.Perm, pView_);
		bool needredraw = false;
		if (mode_->AlarmDisConfig.HideVar.Cmp(varId))
		{
			if (mode_->AlarmDisConfig.AlarmMode == Project::AlarmDisMode::HISTORY)
			{
				pView->HideFlag = UI::UIData::Number<int>(mode_->AlarmDisConfig.HideVar);
				needredraw = true;
			}
		}
		if (mode_->AlarmDisConfig.AlarmVarId.Cmp(varId))
			needredraw = true;
		if (mode_->AlarmDisConfig.SeekCtrlVar.Cmp(varId))
		{
			bool searchstatus = UI::UIData::Bit(mode_->AlarmDisConfig.SeekCtrlVar);
			if (searchstatus && mode_->AlarmDisConfig.EnableSearch)	//ON状态查询模式
				pView->DisplayMode = Project::AlarmDisMode::SEARCH;
			else
				pView->DisplayMode = mode_->AlarmDisConfig.AlarmMode;
			needredraw = true;
		}
		if (mode_->AlarmDisConfig.SeekModeVar.Cmp(varId))
		{
			int searchmode = UI::UIData::Number<int>(mode_->AlarmDisConfig.SeekModeVar);
			if ((Project::AlarmSeekMode)searchmode != mode_->AlarmDisConfig.SearchMode)
				mode_->AlarmDisConfig.SearchMode = (Project::AlarmSeekMode)searchmode;
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH)
				needredraw = true;
		}
		/*日期*/
		if (Project::DataVarId::Cmp(mode_->AlarmDisConfig.SearchDateVarId, varId))
		{
			int time[3] = { 0 };
			tm searchlocal;
			for (unsigned i = 0; i < mode_->AlarmDisConfig.SearchTimeVar.size();i++)
				time[i] = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchTimeVar[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->AlarmDisConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH
				&& mode_->AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekDATE)
				needredraw = true;

		}
		/*时间*/
		if (Project::DataVarId::Cmp(mode_->AlarmDisConfig.SearchTimeVar, varId))
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->AlarmDisConfig.SearchTimeVar.size();i++)
				time[i] =UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchTimeVar[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->AlarmDisConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->AlarmDisConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH 
				&& mode_->AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekTIME)
				needredraw = true;

		}
		if (mode_->AlarmDisConfig.SearchGroupVar.Cmp(varId))
		{
			mode_->AlarmDisConfig.SearchGroup = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchGroupVar);
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH 
				&& mode_->AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekGROUP)
				needredraw = true;
		}
		if (mode_->AlarmDisConfig.SearchNoVar.Cmp(varId))
		{
			mode_->AlarmDisConfig.SearchNo = UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchNoVar);
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH 
				&& mode_->AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekNUM)
				needredraw = true;
		}
		if (mode_->AlarmDisConfig.SearchLevelVar.Cmp(varId))
		{
			mode_->AlarmDisConfig.SearchLevel = (Project::AlarmLevel)UI::UIData::Number<int>(mode_->AlarmDisConfig.SearchLevelVar);
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH && 
				mode_->AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekLEVEL)
				needredraw = true;
		}
		if (Project::DataVarId::Cmp(mode_->AlarmDisConfig.SearchRegVarId, varId))
		{
			GetSearchRegData();
			if (pView->DisplayMode == Project::AlarmDisMode::SEARCH
				&& mode_->AlarmDisConfig.RegMode == true)
				needredraw = true;
		}
		if(needredraw && mode_->AlarmDisConfig.Perm.HasShowPerm && mode_->AlarmDisConfig.Perm.HasLimitShowPerm)
			pView->redraw();
	}

	int AlarmDisplayControl::PeekHMIMessage(Message::Msg* msg)
	{
		AlarmDisplayView* pView = static_cast<AlarmDisplayView*>(pView_);
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
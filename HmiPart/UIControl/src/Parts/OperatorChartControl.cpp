/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorChartControl.cpp
 * Author   : qiaodan
 * Date     : 2021-05-11
 * Descript : 创建操作记录显示绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "OperatorChartControl.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include "HMIWindow.h"
#include "Message.h"
#include "UIComm.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	OperatorChartControl::OperatorChartControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<OperatorChartModel>(new OperatorChartModel());
		InitMVCModel(mode_);
	}


	void OperatorChartControl::CreateView()
	{
		//初始化VIEW
		OperatorChartView* view;
		view = new OperatorChartView(
			mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
			mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY,
			mode_->OperatorConfig.Width,
			mode_->OperatorConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}
	void OperatorChartControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->OperatorConfig.VOffX != Project::DataVarId::NullId)
			mode_->OperatorConfig.OffX = UI::UIData::Number<int>(mode_->OperatorConfig.VOffX) + pageoffx;
		else
			mode_->OperatorConfig.OffX = pageoffx;


		if (mode_->OperatorConfig.VOffY != Project::DataVarId::NullId)
			mode_->OperatorConfig.OffY = UI::UIData::Number<int>(mode_->OperatorConfig.VOffY) + pageoffy;
		else
			mode_->OperatorConfig.OffY = pageoffy;
	}

	void OperatorChartControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void OperatorChartControl::GetSearchRegData()
	{
		switch (mode_->OperatorConfig.SearchMode)
		{
		case  Project::OperatorSeekMode::OperatorByTime:
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->OperatorConfig.SearchRegVarId.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->OperatorConfig.SearchRegVarId[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->OperatorConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->OperatorConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			break;
		}
		case Project::OperatorSeekMode::OperatorByDate:
		{
			int time[3] = { 0 };
			tm searchlocal;
			int size = 3 < mode_->OperatorConfig.SearchRegVarId.size() ? 3 : mode_->OperatorConfig.SearchRegVarId.size();
			for (unsigned i = 0; i < size;i++)
				time[i] = UI::UIData::Number<int>(mode_->OperatorConfig.SearchRegVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->OperatorConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			break;
		}
		case Project::OperatorSeekMode::OperatorByName:
		{
			if(!mode_->OperatorConfig.SearchRegVarId.empty())
				mode_->OperatorConfig.SearchKey = UI::UIDataService::Ins().GetString(mode_->OperatorConfig.SearchRegVarId[0]);
			break;
		}
		default:
			break;
		}

	}

	void OperatorChartControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->OperatorConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->OperatorConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->OperatorConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->OperatorConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void OperatorChartControl::HandleDataVar(Project::DataVarId &varId)
	{
		OperatorChartView* pView = (OperatorChartView*)(pView_);
		if (mode_->OperatorConfig.VOffX.Cmp(varId))
		{
			mode_->OperatorConfig.OffX = UI::UIData::Number<int>(mode_->OperatorConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->OperatorConfig.Perm, mode_->OperatorConfig.X + mode_->OperatorConfig.OffX, mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		if (mode_->OperatorConfig.VOffY.Cmp(varId))
		{
			mode_->OperatorConfig.OffY = UI::UIData::Number<int>(mode_->OperatorConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->OperatorConfig.Perm, mode_->OperatorConfig.X + mode_->OperatorConfig.OffX, mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		if (mode_->OperatorConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->OperatorConfig.Perm, pView_);
		if (mode_->OperatorConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->OperatorConfig.Perm, pView_);
		if (mode_->OperatorConfig.RecordVarId.Cmp(varId))
			UI::ViewShowUtility::ShowView(pView_, mode_->OperatorConfig.Perm, mode_->OperatorConfig.X + mode_->OperatorConfig.OffX, mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		if (mode_->OperatorConfig.SeekCtrlVar.Cmp(varId))
		{
			bool searchstatus = UI::UIData::Bit(mode_->OperatorConfig.SeekCtrlVar);
			if (searchstatus && mode_->OperatorConfig.EnableSearch)	//ON状态查询模式
				pView->DisplayMode = 1;
			else
				pView->DisplayMode = 0;
			UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
				mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
				mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		if (mode_->OperatorConfig.SeekModeVar.Cmp(varId))
		{
			int searchmode = UI::UIData::Number<int>(mode_->OperatorConfig.SeekModeVar);
			mode_->OperatorConfig.SearchMode = (Project::OperatorSeekMode)searchmode;

			if (pView->DisplayMode && (Project::OperatorSeekMode)searchmode != mode_->OperatorConfig.SearchMode)
			{
				UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
			}
		}
		/*日期*/
		if (Project::DataVarId::Cmp(mode_->OperatorConfig.SearchDateVarId, varId))
		{
			int time[3] = { 0 };
			tm searchlocal;
			for (unsigned i = 0; i < mode_->OperatorConfig.SearchDateVarId.size();i++)
				time[i] = UI::UIData::Number<int>(mode_->OperatorConfig.SearchDateVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->OperatorConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByDate)
				UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		/*时间*/
		if (Project::DataVarId::Cmp(mode_->OperatorConfig.SearchTimeVar, varId))
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->OperatorConfig.SearchTimeVar.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->OperatorConfig.SearchTimeVar[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->OperatorConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->OperatorConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByTime)
				UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		/*用户名*/
		if (mode_->OperatorConfig.SearchKeyVar.Cmp(varId))
		{
			mode_->OperatorConfig.SearchKey = UI::UIDataService::Ins().GetString(mode_->OperatorConfig.SearchChannelNoVar);
			if (pView->DisplayMode && mode_->OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByName)
				UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}
		if (Project::DataVarId::Cmp(mode_->OperatorConfig.SearchRegVarId, varId))
		{
			GetSearchRegData();
			if (pView->DisplayMode && mode_->OperatorConfig.RegMode == true)
				UI::ViewShowUtility::ShowView(pView, mode_->OperatorConfig.Perm,
					mode_->OperatorConfig.X + mode_->OperatorConfig.OffX,
					mode_->OperatorConfig.Y + mode_->OperatorConfig.OffY);
		}

	}

	int OperatorChartControl::PeekHMIMessage(Message::Msg* msg)
	{
		OperatorChartView* pView = (OperatorChartView*)(pView_);
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
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ReportFormControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-30
 * Descript : 创建报表显示绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ReportFormControl.h"
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
	ReportFormControl::ReportFormControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<ReportFormModel>(new ReportFormModel());
		InitMVCModel(mode_);
	}


	void ReportFormControl::CreateView()
	{
		//初始化VIEW
		ReportFormView* view;
		view = new ReportFormView(
			mode_->ReportConfig.X + mode_->ReportConfig.OffX,
			mode_->ReportConfig.Y + mode_->ReportConfig.OffY,
			mode_->ReportConfig.Width,
			mode_->ReportConfig.Height,nullptr);
		InitMVCView(view);
		view->InitDraw();
	}
	void ReportFormControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->ReportConfig.VOffX != Project::DataVarId::NullId)
			mode_->ReportConfig.OffX = UI::UIData::Number<int>(mode_->ReportConfig.VOffX) + pageoffx;
		else
			mode_->ReportConfig.OffX = pageoffx;


		if (mode_->ReportConfig.VOffY != Project::DataVarId::NullId)
			mode_->ReportConfig.OffY = UI::UIData::Number<int>(mode_->ReportConfig.VOffY) + pageoffy;
		else
			mode_->ReportConfig.OffY = pageoffy;
	}

	void ReportFormControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void ReportFormControl::GetSearchRegData()
	{
		if (mode_->ReportConfig.SearchMode == Project::ReportSeekMode::ReportDATE)
		{
			int time[3] = { 0 };
			tm searchlocal;
			int size = 3 < mode_->ReportConfig.SearchRegVarId.size() ? 3 : mode_->ReportConfig.SearchRegVarId.size();
			for (int i = 0; i < size;i++)
				time[i] = UI::UIData::Number<int>(mode_->ReportConfig.SearchRegVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->ReportConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
		}
		if (mode_->ReportConfig.SearchMode == Project::ReportSeekMode::ReportTIME)
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->ReportConfig.SearchRegVarId.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->ReportConfig.SearchRegVarId[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->ReportConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->ReportConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
		}
	}

	void ReportFormControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ReportConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ReportConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ReportConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->ReportConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ReportConfig.Perm,
					mode_->ReportConfig.X + mode_->ReportConfig.OffX,
					mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}


	void ReportFormControl::HandleDataVar(Project::DataVarId &varId)
	{
		ReportFormView* pView = (ReportFormView*)(pView_);
		if (mode_->ReportConfig.VOffX.Cmp(varId))
		{
			mode_->ReportConfig.OffX = UI::UIData::Number<int>(mode_->ReportConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->ReportConfig.Perm, mode_->ReportConfig.X + mode_->ReportConfig.OffX, mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		if (mode_->ReportConfig.VOffY.Cmp(varId))
		{
			mode_->ReportConfig.OffY = UI::UIData::Number<int>(mode_->ReportConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->ReportConfig.Perm, mode_->ReportConfig.X + mode_->ReportConfig.OffX, mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		if (mode_->ReportConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->ReportConfig.Perm, pView_);
		if (mode_->ReportConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->ReportConfig.Perm, pView_);
		if (mode_->ReportConfig.SampleVarId.Cmp(varId))
			UI::ViewShowUtility::ShowView(pView_, mode_->ReportConfig.Perm, mode_->ReportConfig.X + mode_->ReportConfig.OffX, mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		if (mode_->ReportConfig.SeekCtrlVar.Cmp(varId))
		{
			bool searchstatus = UI::UIData::Bit(mode_->ReportConfig.SeekCtrlVar);
			if (searchstatus && mode_->ReportConfig.EnableSearch)	//ON状态查询模式
				pView->DisplayMode = 1;
			else
				pView->DisplayMode = 0;
			UI::ViewShowUtility::ShowView(pView, mode_->ReportConfig.Perm,
				mode_->ReportConfig.X + mode_->ReportConfig.OffX,
				mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		if (mode_->ReportConfig.SeekModeVar.Cmp(varId))
		{
			int searchmode = UI::UIData::Number<int>(mode_->ReportConfig.SeekModeVar);
			if ((Project::ReportSeekMode)searchmode != mode_->ReportConfig.SearchMode)
				mode_->ReportConfig.SearchMode = (Project::ReportSeekMode)searchmode;
			if (pView->DisplayMode)
				UI::ViewShowUtility::ShowView(pView, mode_->ReportConfig.Perm,
						mode_->ReportConfig.X + mode_->ReportConfig.OffX,
						mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		/*日期*/
		if (Project::DataVarId::Cmp(mode_->ReportConfig.SearchDateVarId, varId))
		{
			int time[3] = { 0 };
			tm searchlocal;
			for (unsigned i = 0; i < mode_->ReportConfig.SearchTimeVar.size();i++)
				time[i] = UI::UIData::Number<int>(mode_->ReportConfig.SearchTimeVar[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->ReportConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->ReportConfig.SearchMode == Project::ReportSeekMode::ReportDATE)
				UI::ViewShowUtility::ShowView(pView, mode_->ReportConfig.Perm,
					mode_->ReportConfig.X + mode_->ReportConfig.OffX,
					mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		/*时间*/
		if (Project::DataVarId::Cmp(mode_->ReportConfig.SearchTimeVar, varId))
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->ReportConfig.SearchTimeVar.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->ReportConfig.SearchTimeVar[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->ReportConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->ReportConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->ReportConfig.SearchMode == Project::ReportSeekMode::ReportTIME)
				UI::ViewShowUtility::ShowView(pView, mode_->ReportConfig.Perm,
					mode_->ReportConfig.X + mode_->ReportConfig.OffX,
					mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		if (Project::DataVarId::Cmp(mode_->ReportConfig.SearchRegVarId, varId))
		{
			GetSearchRegData();
			if (pView->DisplayMode && mode_->ReportConfig.RegMode == true)
				UI::ViewShowUtility::ShowView(pView, mode_->ReportConfig.Perm,
					mode_->ReportConfig.X + mode_->ReportConfig.OffX,
					mode_->ReportConfig.Y + mode_->ReportConfig.OffY);
		}
		///*通道*/
		//if (mode_->ReportConfig.SearchChannelNoVar.Cmp(varId))
		//{
		//	mode_->ReportConfig.SearchChannelNo = UI::UIData::Number<int>(mode_->ReportConfig.SearchChannelNoVar);
		//	if (pView->DisplayMode && mode_->ReportConfig.SearchMode == Project::ReportSeekMode::CHANNELNO)
		//		pView->redraw();
		//}
	}

	int ReportFormControl::PeekHMIMessage(Message::Msg* msg)
	{
		ReportFormView* pView = (ReportFormView*)(pView_);
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
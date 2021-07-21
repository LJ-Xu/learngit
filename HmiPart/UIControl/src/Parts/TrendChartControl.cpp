/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartControl.cpp
 * Author   : qiaodan
 * Date     : 2020-11-14
 * Descript : 创建趋势图绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "TrendChartControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	TrendChartControl::TrendChartControl(HMIPage* w) : BaseControl(w)
	{
		//创建TrendChartModel
		mode_ = std::shared_ptr<TrendChartModel>(new TrendChartModel);
		InitMVCModel(mode_);
	}

	void TrendChartControl::CreateView()
	{
		//初始化VIEW
		TrendChartView* view = new TrendChartView(
			mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
			mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY,
			mode_->ChartTrendConfig.Width,
			mode_->ChartTrendConfig.Height,nullptr);
		InitMVCView(view);
		//view->UseInfoDisplay = false;
		//OnReady();
		view->InitDraw();
	}
	void TrendChartControl::OnReady()
	{
		TrendChartView* pView = static_cast<TrendChartView*>(pView_);
		pView->UseInfoDisplay = mode_->ChartTrendConfig.IsDataDisplay;
		HandleSysChange(Permission);
		if (mode_->ChartTrendConfig.AxisY.YScalemaxVarId != Project::DataVarId::NullId)
			mode_->ChartTrendConfig.AxisY.YScaleMax = UI::UIData::Number<DOUBLE>(mode_->ChartTrendConfig.AxisY.YScalemaxVarId);
		if (mode_->ChartTrendConfig.AxisY.YScaleminVarId != Project::DataVarId::NullId)
			mode_->ChartTrendConfig.AxisY.YScaleMin = UI::UIData::Number<DOUBLE>(mode_->ChartTrendConfig.AxisY.YScaleminVarId);

	}
	void TrendChartControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->ChartTrendConfig.VOffX != Project::DataVarId::NullId)
			mode_->ChartTrendConfig.OffX = UI::UIData::Number<int>(mode_->ChartTrendConfig.VOffX) + pageoffx;
		else
			mode_->ChartTrendConfig.OffX = pageoffx;


		if (mode_->ChartTrendConfig.VOffY != Project::DataVarId::NullId)
			mode_->ChartTrendConfig.OffY = UI::UIData::Number<int>(mode_->ChartTrendConfig.VOffY) + pageoffy;
		else
			mode_->ChartTrendConfig.OffY = pageoffy;
	}
	void TrendChartControl::GetSearchRegData()
	{
		switch (mode_->ChartTrendConfig.SearchMode)
		{
		case Project::TrendSeekMode::CHANNELNO:
		{
			if(!mode_->ChartTrendConfig.SearchRegVarId.empty())
				mode_->ChartTrendConfig.SearchChannelNo = UI::UIData::Number<int>(mode_->ChartTrendConfig.SearchRegVarId[0]);
			break;
		}
		case Project::TrendSeekMode::TIME:
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->ChartTrendConfig.SearchRegVarId.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->ChartTrendConfig.SearchRegVarId[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->ChartTrendConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->ChartTrendConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			break;
		}
		case Project::TrendSeekMode::DATE:
		{
			int time[3] = { 0 };
			tm searchlocal;
			int size = 3 < mode_->ChartTrendConfig.SearchRegVarId.size() ? 3 : mode_->ChartTrendConfig.SearchRegVarId.size();
			for (int i = 0; i < size;i++)
				time[i] = UI::UIData::Number<int>(mode_->ChartTrendConfig.SearchRegVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->ChartTrendConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			break;
		}
		default:
			break;
		}

	}
	bool TrendChartControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ChartTrendConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ChartTrendConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ChartTrendConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->ChartTrendConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void TrendChartControl::HandleDataVar(Project::DataVarId &varId)
	{
		TrendChartView* pView = (TrendChartView*)(pView_);
		if (mode_->ChartTrendConfig.VOffX.Cmp(varId))
		{
			mode_->ChartTrendConfig.OffX = UI::UIData::Number<int>(mode_->ChartTrendConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->ChartTrendConfig.Perm, mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX, mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);

		}
		if (mode_->ChartTrendConfig.VOffY.Cmp(varId))
		{
			mode_->ChartTrendConfig.OffY = UI::UIData::Number<int>(mode_->ChartTrendConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->ChartTrendConfig.Perm, mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX, mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
		if (mode_->ChartTrendConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->ChartTrendConfig.Perm, pView_);
		if (mode_->ChartTrendConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->ChartTrendConfig.Perm, pView_);
		if (mode_->ChartTrendConfig.SampleVarId.Cmp(varId))
		//{
			//bool changeData = UI::UIData::Bit(mode_->ChartTrendConfig.SampleVarId);
			//if(changeData)
			UI::ViewShowUtility::ShowView(pView_, mode_->ChartTrendConfig.Perm, mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX, mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		//}
		if (mode_->ChartTrendConfig.PageCtrlVarId.Cmp(varId))	//翻页
			pView->SetCurrentPage(UI::UIData::Number<int>(mode_->ChartTrendConfig.PageCtrlVarId));
		if (mode_->ChartTrendConfig.ZoomVarId.Cmp(varId))		//缩放
			pView->SetZoomParam(UI::UIData::Number<int>(mode_->ChartTrendConfig.ZoomVarId));
		if (mode_->ChartTrendConfig.AxisY.YScalemaxVarId.Cmp(varId))
			mode_->ChartTrendConfig.AxisY.YScaleMax = UI::UIData::Number<DOUBLE>(mode_->ChartTrendConfig.AxisY.YScalemaxVarId);
		if (mode_->ChartTrendConfig.AxisY.YScaleminVarId.Cmp(varId))
			mode_->ChartTrendConfig.AxisY.YScaleMin = UI::UIData::Number<DOUBLE>(mode_->ChartTrendConfig.AxisY.YScaleminVarId);
		if (mode_->ChartTrendConfig.SeekCtrlVar.Cmp(varId))
		{
			bool searchstatus = UI::UIData::Bit(mode_->ChartTrendConfig.SeekCtrlVar);
			if (searchstatus && mode_->ChartTrendConfig.EnableSearch)	//ON状态查询模式
				pView->DisplayMode = 1;
			else
				pView->DisplayMode = 0;
			UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
				mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
				mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
		if (mode_->ChartTrendConfig.SeekModeVar.Cmp(varId))
		{
			int searchmode = UI::UIData::Number<int>(mode_->ChartTrendConfig.SeekModeVar);
			mode_->ChartTrendConfig.SearchMode = (Project::TrendSeekMode)searchmode;

			if (pView->DisplayMode && (Project::TrendSeekMode)searchmode != mode_->ChartTrendConfig.SearchMode)
			{
				UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
			}
		}
		/*日期*/
		if (Project::DataVarId::Cmp(mode_->ChartTrendConfig.SearchDateVarId, varId))
		{
			int time[3] = {0};
			tm searchlocal;
			int size = 3 < mode_->ChartTrendConfig.SearchRegVarId.size() ? 3 : mode_->ChartTrendConfig.SearchRegVarId.size();
			for (int i = 0; i < size;i++)
				time[i] = UI::UIData::Number<int>(mode_->ChartTrendConfig.SearchDateVarId[i]);
			searchlocal.tm_sec = 0;
			searchlocal.tm_min = 0;
			searchlocal.tm_hour = 0;
			searchlocal.tm_mday = time[2];
			searchlocal.tm_mon = time[1] - 1;
			searchlocal.tm_year = time[0] - 1900;
			mode_->ChartTrendConfig.SearchDate = (DDWORD)mktime(&searchlocal) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->ChartTrendConfig.SearchMode == Project::TrendSeekMode::DATE)
				UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
		/*时间*/
		if (Project::DataVarId::Cmp(mode_->ChartTrendConfig.SearchTimeVar, varId))
		{
			int time[12] = { 0 };
			tm tmDate;
			for (unsigned i = 0; i < mode_->ChartTrendConfig.SearchTimeVar.size();i++)
				time[i] = UIDataService::Ins().GetNumber<int>(mode_->ChartTrendConfig.SearchTimeVar[i]);
			tmDate.tm_sec = time[5];
			tmDate.tm_min = time[4];
			tmDate.tm_hour = time[3];
			tmDate.tm_mday = time[2];
			tmDate.tm_mon = time[1] - 1;
			tmDate.tm_year = time[0] - 1900;
			mode_->ChartTrendConfig.SearchTimeStart = (DDWORD)mktime(&tmDate) * (DDWORD)1000;

			tmDate.tm_sec = time[11];
			tmDate.tm_min = time[10];
			tmDate.tm_hour = time[9];
			tmDate.tm_mday = time[8];
			tmDate.tm_mon = time[7] - 1;
			tmDate.tm_year = time[6] - 1900;
			mode_->ChartTrendConfig.SearchTimeEnd = (DDWORD)mktime(&tmDate) * (DDWORD)1000;
			if (pView->DisplayMode && mode_->ChartTrendConfig.SearchMode == Project::TrendSeekMode::TIME)
				UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
		/*通道*/
		if (mode_->ChartTrendConfig.SearchChannelNoVar.Cmp(varId))
		{
			mode_->ChartTrendConfig.SearchChannelNo = UI::UIData::Number<int>(mode_->ChartTrendConfig.SearchChannelNoVar);
			if (pView->DisplayMode && mode_->ChartTrendConfig.SearchMode == Project::TrendSeekMode::CHANNELNO)
				UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
		if (Project::DataVarId::Cmp(mode_->ChartTrendConfig.SearchRegVarId, varId))
		{
			GetSearchRegData();
			if (pView->DisplayMode && mode_->ChartTrendConfig.RegMode == true)
				UI::ViewShowUtility::ShowView(pView, mode_->ChartTrendConfig.Perm,
					mode_->ChartTrendConfig.X + mode_->ChartTrendConfig.OffX,
					mode_->ChartTrendConfig.Y + mode_->ChartTrendConfig.OffY);
		}
	}

	int TrendChartControl::PeekHMIMessage(Message::Msg* msg)
	{
		TrendChartView* pView = (TrendChartView*)(pView_);
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

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeChartControl.cpp
 * Author   : qiaodan
 * Date     : 2021-02-04
 * Descript : 创建配方表显示绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "RecipeChartControl.h"
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
	RecipeChartControl::RecipeChartControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<RecipeChartModel>(new RecipeChartModel());
		InitMVCModel(mode_);
	}


	void RecipeChartControl::CreateView()
	{
		//初始化VIEW
		RecipeChartView* view = new RecipeChartView(
			mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
			mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY,
			mode_->RecipeConfig.Width,
			mode_->RecipeConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}

	void RecipeChartControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool RecipeChartControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->RecipeConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->RecipeConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->RecipeConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->RecipeConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->RecipeConfig.Perm, mode_->RecipeConfig.X + mode_->RecipeConfig.OffX, mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void RecipeChartControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->RecipeConfig.VOffX != Project::DataVarId::NullId)
			mode_->RecipeConfig.OffX = UI::UIData::Number<int>(mode_->RecipeConfig.VOffX) + pageoffx;
		else
			mode_->RecipeConfig.OffX = pageoffx;


		if (mode_->RecipeConfig.VOffY != Project::DataVarId::NullId)
			mode_->RecipeConfig.OffY = UI::UIData::Number<int>(mode_->RecipeConfig.VOffY) + pageoffy;
		else
			mode_->RecipeConfig.OffY = pageoffy;
	}
	void RecipeChartControl::GetSearchRegData()
	{
		RecipeChartView* pView = (RecipeChartView*)(pView_);
		if (mode_->RecipeConfig.SearchRegVarId.empty())
			return;
		if(mode_->RecipeConfig.SearchMode == Project::RecipeSeekMode::RecipeKey)
			mode_->RecipeConfig.SearchKey = UIDataService::Ins().GetString(mode_->RecipeConfig.SearchRegVarId[0]);
		if (mode_->RecipeConfig.SearchMode == Project::RecipeSeekMode::RecipeData)
			mode_->RecipeConfig.SearchData = UI::UIData::Number<double>(mode_->RecipeConfig.SearchRegVarId[0]);
	}
	void RecipeChartControl::HandleDataVar(Project::DataVarId &varId)
	{
		RecipeChartView* pView = (RecipeChartView*)(pView_);
		if (mode_->RecipeConfig.VOffX.Cmp(varId))
		{
			mode_->RecipeConfig.OffX = UI::UIData::Number<int>(mode_->RecipeConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->RecipeConfig.Perm, mode_->RecipeConfig.X + mode_->RecipeConfig.OffX, mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}
		if (mode_->RecipeConfig.VOffY.Cmp(varId))
		{
			mode_->RecipeConfig.OffY = UI::UIData::Number<int>(mode_->RecipeConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->RecipeConfig.Perm, mode_->RecipeConfig.X + mode_->RecipeConfig.OffX, mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}
		if (mode_->RecipeConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->RecipeConfig.Perm, pView_);
		if (mode_->RecipeConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->RecipeConfig.Perm, pView_);
		if (mode_->RecipeConfig.SeekCtrlVar.Cmp(varId))
		{
			bool searchstatus = UI::UIData::Bit(mode_->RecipeConfig.SeekCtrlVar);
			if (searchstatus && mode_->RecipeConfig.EnableSearch)	//ON状态查询模式
				pView->DisplayMode = 1;
			else
				pView->DisplayMode = 0;
			UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
				mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
				mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}
		if (mode_->RecipeConfig.SeekModeVar.Cmp(varId))
		{
			int searchmode = UI::UIData::Number<int>(mode_->RecipeConfig.SeekModeVar);
			mode_->RecipeConfig.SearchMode = (Project::RecipeSeekMode)searchmode;

			if (pView->DisplayMode && (Project::RecipeSeekMode)searchmode != mode_->RecipeConfig.SearchMode)
			{
				UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
					mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
					mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
			}
		}
		/*关键字*/
		if (mode_->RecipeConfig.SearchKeyVar.Cmp(varId))
		{
			mode_->RecipeConfig.SearchKey = UIDataService::Ins().GetString(mode_->RecipeConfig.SearchKeyVar);
			if (pView->DisplayMode && mode_->RecipeConfig.SearchMode == Project::RecipeSeekMode::RecipeKey)
				UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
					mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
					mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}
		/*数据*/
		if (mode_->RecipeConfig.SearchDataVar.Cmp(varId))
		{
			mode_->RecipeConfig.SearchData = UI::UIData::Number<double>(mode_->RecipeConfig.SearchDataVar);
			if (pView->DisplayMode && mode_->RecipeConfig.SearchMode == Project::RecipeSeekMode::RecipeData)
				UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
					mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
					mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}
		if (Project::DataVarId::Cmp(mode_->RecipeConfig.SearchRegVarId, varId))
		{
			GetSearchRegData();
			if (pView->DisplayMode && mode_->RecipeConfig.RegMode == true)
				UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
					mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
					mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
		}

		if (mode_->RecipeConfig.UpdateNotice.Cmp(varId))
			UI::ViewShowUtility::ShowView(pView, mode_->RecipeConfig.Perm,
				mode_->RecipeConfig.X + mode_->RecipeConfig.OffX,
				mode_->RecipeConfig.Y + mode_->RecipeConfig.OffY);
	}

	int RecipeChartControl::PeekHMIMessage(Message::Msg* msg)
	{
		RecipeChartView* pView = (RecipeChartView*)(pView_);
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
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataTableControl.cpp
 * Author   : qiaodan
 * Date     : 2021-03-26
 * Descript : 创建数据表格显示绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DataTableControl.h"
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
	DataTableControl::DataTableControl(HMIPage* w) :BaseControl(w)
	{
		//创建KeyBtnModel
		mode_ = std::shared_ptr<DataTableModel>(new DataTableModel());
		InitMVCModel(mode_);
	}


	void DataTableControl::CreateView()
	{
		//初始化VIEW
		DataTableView* view;
		view = new DataTableView(
			mode_->DataConfig.X + mode_->DataConfig.OffX,
			mode_->DataConfig.Y + mode_->DataConfig.OffY,
			mode_->DataConfig.Width,
			mode_->DataConfig.Height, nullptr);
		InitMVCView(view);
		view->InitDraw();
	}

	void DataTableControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->DataConfig.VOffX != Project::DataVarId::NullId)
			mode_->DataConfig.OffX = UI::UIData::Number<int>(mode_->DataConfig.VOffX) + pageoffx;
		else
			mode_->DataConfig.OffX = pageoffx;


		if (mode_->DataConfig.VOffY != Project::DataVarId::NullId)
			mode_->DataConfig.OffY = UI::UIData::Number<int>(mode_->DataConfig.VOffY) + pageoffy;
		else
			mode_->DataConfig.OffY = pageoffy;
	}

	void DataTableControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	bool DataTableControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DataConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DataConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DataConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->DataConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DataConfig.Perm,
					mode_->DataConfig.X + mode_->DataConfig.OffX,
					mode_->DataConfig.Y + mode_->DataConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void DataTableControl::HandleDataVar(Project::DataVarId &varId)
	{
		DataTableView* pView = (DataTableView*)(pView_);
		if (mode_->DataConfig.VOffX.Cmp(varId))
		{
			mode_->DataConfig.OffX = UI::UIData::Number<int>(mode_->DataConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->DataConfig.Perm, mode_->DataConfig.X + mode_->DataConfig.OffX, mode_->DataConfig.Y + mode_->DataConfig.OffY);
		}
		if (mode_->DataConfig.VOffY.Cmp(varId))
		{
			mode_->DataConfig.OffY = UI::UIData::Number<int>(mode_->DataConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->DataConfig.Perm, mode_->DataConfig.X + mode_->DataConfig.OffX, mode_->DataConfig.Y + mode_->DataConfig.OffY);
		}
		if (mode_->DataConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->DataConfig.Perm, pView_);
		if (mode_->DataConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->DataConfig.Perm, pView_);
		for (size_t i = 0; i < mode_->DataConfig.DataCellInfoLst.size(); i++)
		{
			if (mode_->DataConfig.DataCellInfoLst[i].DataAddrVar.Cmp(varId))
			{
				if (mode_->DataConfig.Perm.HasShowPerm && mode_->DataConfig.Perm.HasLimitShowPerm)
					pView_->redraw();
				return;
			}
		}
	}

	int DataTableControl::PeekHMIMessage(Message::Msg* msg)
	{
		DataTableView* pView = (DataTableView*)(pView_);
		Project::DataVarId varId;
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->EnterValue();
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}
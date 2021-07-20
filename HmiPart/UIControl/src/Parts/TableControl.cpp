/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableControl.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 表格的控制源码，创建表格绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "TableControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
namespace UI
{

	TableControl::TableControl(HMIPage* w) : BaseControl(w)
	{
		mode_ = std::shared_ptr<TableModel>(new TableModel);
		InitMVCModel(mode_);
	}
	void TableControl::CreateView()
	{
		//初始化VIEW
		TableView* view = new TableView(
			mode_->TableConfig.X + mode_->TableConfig.OffX,
			mode_->TableConfig.Y + mode_->TableConfig.OffY,
			mode_->TableConfig.Width,
			mode_->TableConfig.Height,nullptr);
		InitMVCView(view);
		view->InitDraw();
	}
	void TableControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->TableConfig.VOffX != Project::DataVarId::NullId)
			mode_->TableConfig.OffX = UI::UIData::Number<int>(mode_->TableConfig.VOffX) + pageoffx;
		else
			mode_->TableConfig.OffX = pageoffx;


		if (mode_->TableConfig.VOffY != Project::DataVarId::NullId)
			mode_->TableConfig.OffY = UI::UIData::Number<int>(mode_->TableConfig.VOffY) + pageoffy;
		else
			mode_->TableConfig.OffY = pageoffy;
	}
	void TableControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool TableControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->TableConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->TableConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->TableConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->TableConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->TableConfig.Perm, mode_->TableConfig.X + mode_->TableConfig.OffX, mode_->TableConfig.Y + mode_->TableConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void TableControl::HandleDataVar(Project::DataVarId &varId)
	{
		TableView* pView = (TableView*)(pView_);
		if (mode_->TableConfig.VOffX.Cmp(varId))
		{
			mode_->TableConfig.OffX = UI::UIData::Number<int>(mode_->TableConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->TableConfig.Perm, mode_->TableConfig.X + mode_->TableConfig.OffX, mode_->TableConfig.Y + mode_->TableConfig.OffY);
		}
		if (mode_->TableConfig.VOffY.Cmp(varId))
		{
			mode_->TableConfig.OffY = UI::UIData::Number<int>(mode_->TableConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->TableConfig.Perm, mode_->TableConfig.X + mode_->TableConfig.OffX, mode_->TableConfig.Y + mode_->TableConfig.OffY);
		}
		if (mode_->TableConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->TableConfig.Perm, pView_);
		if (mode_->TableConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->TableConfig.Perm, pView_);
	}
	int TableControl::PeekHMIMessage(Message::Msg* msg)
	{
		TableView* pView = (TableView*)(pView_);
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

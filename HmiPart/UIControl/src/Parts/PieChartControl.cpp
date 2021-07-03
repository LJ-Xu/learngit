/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PieChartControl.cpp
 * Author   : TangYao
 * PieChart     : 2020/12/19
 * Descript : 饼图控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "PieChartView.h"
#include "PieChartControl.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"

namespace UI
{
	PieChartControl::PieChartControl(HMIPage* pg) : BaseControl(pg) {
		// 创建model对象
		model_ = shared_ptr<PieChartModel>(new PieChartModel());
		InitMVCModel(model_);
	}

	PieChartControl::~PieChartControl() {

	}

	void PieChartControl::CreateView() {
		// 初始化View
		PieChartView * view = new PieChartView(
			model_->PieChartUnit.X + model_->PieChartUnit.OffX,
			model_->PieChartUnit.Y + model_->PieChartUnit.OffY,
			model_->PieChartUnit.Width,
			model_->PieChartUnit.Height
		);
		InitMVCView(view);
		view->InitView(model_.get());
	}

	void PieChartControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void PieChartControl::HandleSysChange(SysChangeEM catogray)
	{
		PieChartModel* mode_ = static_cast<PieChartModel*>(pModel_.get());
		PieChartView* pView = static_cast<PieChartView*>(pView_);
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->PieChartUnit.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->PieChartUnit.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->PieChartUnit.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				pView->ValueSnapList.clear();
				pView->TotalValue = 0;
				for (int i = 0; i < mode_->PieChartUnit.ReadVars.size(); ++i)
				{
					if (DataApi::GetDataFlag(mode_->PieChartUnit.ReadVars[i]))
					{
						int value = UIData::Number<int>(mode_->PieChartUnit.ReadVars[i]);
						pView->ValueSnapList.push_back(value);
						pView->TotalValue += value;
					}
					else
					{
						return;
					}
				}
				for (int i = 0; i < mode_->PieChartUnit.ReadVars.size(); ++i)
				{
					mode_->PieChartUnit.Channels[i].Percent = (float)pView->ValueSnapList[i] / (float)pView->TotalValue;
				}
				mode_->PieChartUnit.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->PieChartUnit.Perm, mode_->PieChartUnit.X + mode_->PieChartUnit.OffX, mode_->PieChartUnit.Y + mode_->PieChartUnit.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	int PieChartControl::PeekHMIMessage(Message::Msg * msg) {
		shared_ptr<PieChartModel> model = static_pointer_cast<PieChartModel>(model_);
		PieChartView * view = (PieChartView *)pView_;
		
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
		{
			for (int i = 0; i < model->PieChartUnit.ReadVars.size(); ++i)
			{
				if ((msg->LParam == model->PieChartUnit.ReadVars[i].Vid) && (view->ValueSnapList.size() > i))
				{
					float value = UIData::Number<float>(model->PieChartUnit.ReadVars[i]);
					//目前默认筛选大于零的值
					if (value < 0) value = 0;
					view->TotalValue = view->TotalValue - view->ValueSnapList[i] + value;
					view->ValueSnapList[i] = value;
				}
			}
			if (view->ValueSnapList.size()< model->PieChartUnit.ReadVars.size())
			{
				view->ValueSnapList.clear();
				view->TotalValue = 0;
				//初始化值快照队列
				for (int i = 0; i < model->PieChartUnit.ReadVars.size(); ++i)
				{
					if (DataApi::GetDataFlag(model->PieChartUnit.ReadVars[i]))
					{
						float value = UIData::Number<float>(model->PieChartUnit.ReadVars[i]);
						view->ValueSnapList.push_back(value);
						view->TotalValue += value;
					}
					else
					{
						return 1;
					}
				}
			}
			for (int i = 0; i < model->PieChartUnit.ReadVars.size(); ++i)
			{
				model->PieChartUnit.Channels[i].Percent = view->TotalValue?((float)view->ValueSnapList[i] / (float)view->TotalValue):0;
			}
				
			if (msg->LParam == model->PieChartUnit.Perm.ShowVID.Vid) {	// 控件显示
				PermUtility::HandleShowPerm(model->PieChartUnit.Perm, view);
			}
			if (msg->LParam == model->PieChartUnit.Perm.EnableVID.Vid) {	// 控件使能
				PermUtility::HandleEnablePerm(model->PieChartUnit.Perm, view);
			}
			view->position(view->x(), view->y());
			view->redraw();
			}
			break;
		default:
			break;
				}
		return 1;
		}
	// 获取饼图扇区
	float PieChartControl::GetPieValue(int index) {

		return 0;
	}

}
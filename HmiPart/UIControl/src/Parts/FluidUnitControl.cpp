/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 创建流体绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "FluidUnitControl.h"
#include "FluidUnitView.h"
#include "HMIWindow.h"
#include "Message.h"
#include "UIData.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
namespace UI
{
	FluidUnitControl::FluidUnitControl(HMIPage* w) : BaseControl(w)
	{
		//创建FluidUnitModel
		mode_ = std::shared_ptr < FluidUnitModel>(new FluidUnitModel());
		InitMVCModel(mode_);
	}

	void FluidUnitControl::CreateView()
	{
		//初始化VIEW
		FluidUnitView* view = new FluidUnitView(
			mode_->FluidConfig.X + mode_->FluidConfig.OffX,
			mode_->FluidConfig.Y + mode_->FluidConfig.OffY,
			mode_->FluidConfig.Width,
			mode_->FluidConfig.Height);
		InitMVCView(view);
		view->InitDraw();
		//OnReady();
	}
	void FluidUnitControl::OnReady()
	{
		FluidUnitView* pView = static_cast<FluidUnitView*>(pView_);
		FluidTimerData.Model = mode_.get();
		FluidTimerData.View = pView;
		if (mode_->FluidConfig.SpeedVarId != Project::DataVarId::NullId)
			mode_->FluidConfig.FlowSpeed = UI::UIData::Number<double>(mode_->FluidConfig.SpeedVarId);
		if (mode_->FluidConfig.FlowSpeed < 1)		mode_->FluidConfig.FlowSpeed = 1;
		if (mode_->FluidConfig.FlowSpeed > 25)		mode_->FluidConfig.FlowSpeed = 25;
		if (mode_->FluidConfig.ActionMode == 0)
		{
			if(mode_->FluidConfig.FlowSpeed != 0)
				Page()->AddTimeout((size_t)(1500.0 / mode_->FluidConfig.FlowSpeed), FlowTimerCb, (void *)this, true);
			isflow_ = true;
		}
		HandleSysChange(Permission);
	}

	void FlowTimerCb(void *data)
	{
		FluidUnitControl *res = (FluidUnitControl *)data;
		FluidUnitView *view = res->FluidTimerData.View;
		FluidUnitModel *model = res->FluidTimerData.Model;
		if (view->SliderOffx >= model->FluidConfig.SliderParam.SliderLength + model->FluidConfig.SliderParam.SliderIntervalWidget)
		{
			view->SliderOffx = 0;
			view->SliderOffy = 0;
		}
		view->SliderOffy += 4;
		view->SliderOffx += 4;
		if (model->FluidConfig.Perm.HasShowPerm && model->FluidConfig.Perm.HasLimitShowPerm)
			view->redraw();
		//UI::ViewShowUtility::ShowView(view, model->FluidConfig.Perm, 
		//							  model->FluidConfig.X + model->FluidConfig.OffX, 
		//							  model->FluidConfig.Y + model->FluidConfig.OffY);
	}
	void FluidUnitControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->FluidConfig.VOffX != Project::DataVarId::NullId)
			mode_->FluidConfig.OffX = UI::UIData::Number<int>(mode_->FluidConfig.VOffX) + pageoffx;
		else
			mode_->FluidConfig.OffX = pageoffx;


		if (mode_->FluidConfig.VOffY != Project::DataVarId::NullId)
			mode_->FluidConfig.OffY = UI::UIData::Number<int>(mode_->FluidConfig.VOffY) + pageoffy;
		else
			mode_->FluidConfig.OffY = pageoffy;
	}
	void FluidUnitControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->FluidConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->FluidConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->FluidConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->FluidConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->FluidConfig.Perm, mode_->FluidConfig.X + mode_->FluidConfig.OffX, mode_->FluidConfig.Y + mode_->FluidConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void FluidUnitControl::HandleDataVar(Project::DataVarId &varId)
	{
		FluidUnitView* pView = (FluidUnitView*)(pView_);
		if (mode_->FluidConfig.VOffX.Cmp(varId))
		{
			mode_->FluidConfig.OffX = UI::UIData::Number<int>(mode_->FluidConfig.VOffX) + Page()->GetPageOffX();
			for (size_t i = 0; i < pView->Coordinates.size(); i++)
				pView->Coordinates[i].X = mode_->FluidConfig.Coordinates[i].X + mode_->FluidConfig.OffX;
			UI::ViewShowUtility::ShowView(pView_, mode_->FluidConfig.Perm, mode_->FluidConfig.X + mode_->FluidConfig.OffX, mode_->FluidConfig.Y + mode_->FluidConfig.OffY);
		}
		if (mode_->FluidConfig.VOffY.Cmp(varId))
		{
			mode_->FluidConfig.OffY = UI::UIData::Number<int>(mode_->FluidConfig.VOffY) + Page()->GetPageOffY();
			for (size_t i = 0; i < pView->Coordinates.size(); i++)
				pView->Coordinates[i].Y = mode_->FluidConfig.Coordinates[i].Y + mode_->FluidConfig.OffY;
			UI::ViewShowUtility::ShowView(pView_, mode_->FluidConfig.Perm, mode_->FluidConfig.X + mode_->FluidConfig.OffX, mode_->FluidConfig.Y + mode_->FluidConfig.OffY);
		}
		if (mode_->FluidConfig.FlowVarId.Cmp(varId))
		{
			bool flowcondition = UI::UIData::Bit(mode_->FluidConfig.FlowVarId);
			if (flowcondition)		//设置ON状态
				pView->Slidercolor = fl_rgb_color(RGBColor(mode_->FluidConfig.SliderParam.SliderColorON));
			else					//设置OFF状态
				pView->Slidercolor = fl_rgb_color(RGBColor(mode_->FluidConfig.SliderParam.SliderColorOFF));
			if ((flowcondition == 1 && mode_->FluidConfig.FlowCondition == 0 && isflow_ == 0)	//开始流动
				|| (flowcondition == 0 && mode_->FluidConfig.FlowCondition == 1 && isflow_ == 0))
			{
				Page()->RemoveTimeout(FlowTimerCb, (void *)this);
				if (mode_->FluidConfig.FlowSpeed != 0)
					Page()->AddTimeout((size_t)(1500.0 / mode_->FluidConfig.FlowSpeed), FlowTimerCb, (void *)this, true);
				isflow_ = true;
			}
			else	//停止流动
			{
				Page()->RemoveTimeout(FlowTimerCb, (void*)this);
				isflow_ = false;
			}
			if (mode_->FluidConfig.Perm.HasShowPerm && mode_->FluidConfig.Perm.HasLimitShowPerm)
				pView->redraw();
			//UI::ViewShowUtility::ShowView(pView, mode_->FluidConfig.Perm,
			//	mode_->FluidConfig.X + mode_->FluidConfig.OffX,
			//	mode_->FluidConfig.Y + mode_->FluidConfig.OffY);
		}
		if (mode_->FluidConfig.SpeedVarId.Cmp(varId))
		{
			mode_->FluidConfig.FlowSpeed = UI::UIData::Number<double>(mode_->FluidConfig.SpeedVarId);
			if (mode_->FluidConfig.FlowSpeed < 1)		mode_->FluidConfig.FlowSpeed = 1;
			if (mode_->FluidConfig.FlowSpeed > 25)		mode_->FluidConfig.FlowSpeed = 25;
			if (isflow_ == true)
			{
				Page()->RemoveTimeout(FlowTimerCb, (void*)this);
				if (mode_->FluidConfig.FlowSpeed != 0)
					Page()->AddTimeout((size_t)(1500.0 / mode_->FluidConfig.FlowSpeed), FlowTimerCb, (void *)this, true);
			}
		}
		if (mode_->FluidConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->FluidConfig.Perm, pView_);
		if (mode_->FluidConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->FluidConfig.Perm, pView_);
	}

	int FluidUnitControl::PeekHMIMessage(Message::Msg* msg)
	{
		FluidUnitView* pView = static_cast<FluidUnitView*>(pView_);
		Project::DataVarId varId;
		switch (msg->Code)
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

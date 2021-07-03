/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveControl.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "ValveView.h"
#include "ValveControl.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ViewShowUtility.h"

void FluidValveCB(void* data)
{
	UI::ValveView * view = static_cast<UI::ValveView*>(data);
	shared_ptr<UI::ValveModel> model = view->BaseView.GetModel<UI::ValveModel>();
	view->CurrentSpeendMark = (view->CurrentSpeendMark + 1) % view->MarkCount;
	view->redraw();
}

namespace UI
{
	ValveControl::ValveControl(HMIPage * win) : BaseControl(win) {
		// 创建ValveModel
		model_ = shared_ptr <ValveModel>(new ValveModel());
		InitMVCModel(model_);
	}

	ValveControl::~ValveControl() {

	}

	void ValveControl::CreateView() {
		// 初始化View
		ValveView * view = new ValveView(
			model_->ValveUnit.X + model_->ValveUnit.OffX, 
			model_->ValveUnit.Y + model_->ValveUnit.OffY, 
			model_->ValveUnit.Width,
			model_->ValveUnit.Height, "");
		InitMVCView(view);
	}

	void ValveControl::OnReady()
	{
		HandleSysChange(Permission);
		//ValveModel* pMode = static_cast<ValveModel*>(pModel_.get());
		//ValveView* pView = static_cast<ValveView*>(pView_);
		////StartTick(true);
		////pMode->ValveUnit.CurrentStatus = pMode->ValveUnit.Logic;
		//
		//pView_->damage();
		//pView_->redraw();
	}
	void ValveControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(model_->ValveUnit.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (model_->ValveUnit.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					model_->ValveUnit.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				if (DataApi::GetDataFlag(model_->ValveUnit.ReadVar))
					model_->ValveUnit.CurrentStatus = UIData::Number<int>(model_->ValveUnit.ReadVar) ^ model_->ValveUnit.Logic;
				else
					model_->ValveUnit.CurrentStatus = 0;
				model_->ValveUnit.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, model_->ValveUnit.Perm, model_->ValveUnit.X + model_->ValveUnit.OffX, model_->ValveUnit.Y + model_->ValveUnit.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	// 控件处理HMI消息函数
	int ValveControl::PeekHMIMessage(Message::Msg * msg) {
		shared_ptr<ValveModel> model = static_pointer_cast<ValveModel>(model_);
		ValveView * view = (ValveView *)pView_;
		Project::DataVarId varId = Project::DataVarId(msg->LParam);
		switch (msg->Code) 
		{
		case WM_EVENT_DATEUPDATE:
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			view->HandleClick();
		default:
			break;
		}
		return 1;
	}
	void ValveControl::HandleDataVar(Project::DataVarId & varId)
	{
		shared_ptr<ValveModel> model = static_pointer_cast<ValveModel>(model_);
		ValveView * view = (ValveView *)pView_;
		if (model->ValveUnit.VOffX.Cmp(varId))
		{
			model->ValveUnit.OffX = UI::UIData::Number<int>(model->ValveUnit.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, model->ValveUnit.Perm,
				model->ValveUnit.X + model->ValveUnit.OffX,
				model->ValveUnit.Y + model->ValveUnit.OffY);
		}
		if (model->ValveUnit.VOffY.Cmp(varId))
		{
			model->ValveUnit.OffY = UI::UIData::Number<int>(model->ValveUnit.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, model->ValveUnit.Perm,
				model->ValveUnit.X + model->ValveUnit.OffX,
				model->ValveUnit.Y + model->ValveUnit.OffY);
		}
		if (model->ValveUnit.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(model->ValveUnit.Perm, pView_);

		if (varId == model->ValveUnit.Perm.EnableVID.Vid) {	// 控件使能
			PermUtility::HandleEnablePerm(model->ValveUnit.Perm, view);
		}
		if (varId == model->ValveUnit.ReadVar.Vid) {	// 读取数据

			model->ValveUnit.Speed = UIData::Number<WORD>(model->ValveUnit.SpVar);
			if (model->ValveUnit.Speed < 10)
				model->ValveUnit.Speed = 10;
			if (model->ValveUnit.Speed > 100)
				model->ValveUnit.Speed = 100;
			model->ValveUnit.CurrentStatus = (UIData::Number<int>(model->ValveUnit.ReadVar) ^ model->ValveUnit.Logic);
			if (model->ValveUnit.CurrentStatus)
			{
				Page()->RemoveTimeout(FluidValveCB, (void*)view);
				if (model->ValveUnit.InnerRectW > view->MarkCount)
				{
					FluidSpeedMarkWidth = model->ValveUnit.InnerRectW / view->MarkCount;
				}
				else
				{
					view->MarkCount = model->ValveUnit.InnerRectW;
					FluidSpeedMarkWidth = 1;
				}
				Page()->AddTimeout(10000.0 / (double)model->ValveUnit.Speed, FluidValveCB, (void*)view, true);
			}
			else
				Page()->RemoveTimeout(FluidValveCB, (void*)view);

		}
		if (varId == model->ValveUnit.SpVar.Vid) {	
			// 流动速度
			model->ValveUnit.Speed = UIData::Number<WORD>(model->ValveUnit.SpVar);
			if (model->ValveUnit.Speed < 10)
				model->ValveUnit.Speed = 10;
			if (model->ValveUnit.Speed > 100)
				model->ValveUnit.Speed = 100;
			Page()->RemoveTimeout(FluidValveCB, (void*)view);
			Page()->AddTimeout(10000.0 / (double)model->ValveUnit.Speed, FluidValveCB, (void*)view, true);
		}
		view->position(view->x(), view->y());
		view->redraw();
	}
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : WaterPumpControl.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 水泵控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "WaterPumpView.h"
#include "WaterPumpControl.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ViewShowUtility.h"

void FluidCB(void* data)
{
	UI::WaterPumpView * view = static_cast<UI::WaterPumpView*>(data);
	shared_ptr<UI::WaterPumpModel> model = view->BaseView.GetModel<UI::WaterPumpModel>();
	view->CurrentSpeendMark = (view->CurrentSpeendMark + 1) % view->MarkCount;
	view->redraw();
}

namespace UI
{
	WaterPumpControl::WaterPumpControl(HMIPage * win) : BaseControl(win) {
		// 创建Model对象
		model_ = shared_ptr<WaterPumpModel>(new WaterPumpModel());
		InitMVCModel(model_);
	}

	WaterPumpControl::~WaterPumpControl() {

	}

	void WaterPumpControl::CreateView() {
		// 初始化View
		WaterPumpView * view = new WaterPumpView(
			model_->WaterPumpUnit.X + model_->WaterPumpUnit.OffX, 
			model_->WaterPumpUnit.Y + model_->WaterPumpUnit.OffY, 
			model_->WaterPumpUnit.Width, 
			model_->WaterPumpUnit.Height);
		InitMVCView(view);
	}

	void WaterPumpControl::OnReady()
	{
		HandleSysChange(Permission);
	}

	void WaterPumpControl::HandleSysChange(SysChangeEM catogray)
	{
		WaterPumpModel* mode_ = static_cast<WaterPumpModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->WaterPumpUnit.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->WaterPumpUnit.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->WaterPumpUnit.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				if (DataApi::GetDataFlag(mode_->WaterPumpUnit.ReadVar))
					mode_->WaterPumpUnit.CurrentStatus = (UIData::Number<int>(mode_->WaterPumpUnit.ReadVar) == mode_->WaterPumpUnit.Logic) ? 1 : 0;
				/*pView_->damage();
				pView_->redraw();*/
				mode_->WaterPumpUnit.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->WaterPumpUnit.Perm, mode_->WaterPumpUnit.X + mode_->WaterPumpUnit.OffX, mode_->WaterPumpUnit.Y + mode_->WaterPumpUnit.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}

	int WaterPumpControl::PeekHMIMessage(Message::Msg * msg) {
		shared_ptr<WaterPumpModel> model = static_pointer_cast<WaterPumpModel>(model_);
		WaterPumpView * view = (WaterPumpView*)pView_;
		Project::DataVarId varId = Project::DataVarId(msg->LParam);
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;
	}
	void WaterPumpControl::HandleDataVar(Project::DataVarId & varId)
	{
		shared_ptr<WaterPumpModel> model = static_pointer_cast<WaterPumpModel>(model_);
		WaterPumpView * view = (WaterPumpView *)pView_;
		if (model->WaterPumpUnit.VOffX.Cmp(varId))
		{
			model->WaterPumpUnit.OffX = UI::UIData::Number<int>(model->WaterPumpUnit.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, model->WaterPumpUnit.Perm,
				model->WaterPumpUnit.X + model->WaterPumpUnit.OffX,
				model->WaterPumpUnit.Y + model->WaterPumpUnit.OffY);
		}
		if (model->WaterPumpUnit.VOffY.Cmp(varId))
		{
			model->WaterPumpUnit.OffY = UI::UIData::Number<int>(model->WaterPumpUnit.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, model->WaterPumpUnit.Perm,
				model->WaterPumpUnit.X + model->WaterPumpUnit.OffX,
				model->WaterPumpUnit.Y + model->WaterPumpUnit.OffY);
		}
		if (model->WaterPumpUnit.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(model->WaterPumpUnit.Perm, pView_);

		if (NULL_VID_VALUE != model->WaterPumpUnit.SpVar.Vid) {
			model->WaterPumpUnit.Speed = UIData::Number<WORD>(model->WaterPumpUnit.SpVar);
			if (model->WaterPumpUnit.Speed < 10)
				model->WaterPumpUnit.Speed = 10;
			if (model->WaterPumpUnit.Speed > 100)
				model->WaterPumpUnit.Speed = 100;
		}
		if (varId == model->WaterPumpUnit.ReadVar.Vid) {
			//model->WaterPumpUnit.CurrentStatus = UIData::Number<WORD>(model->WaterPumpUnit.ReadVar);
			model->WaterPumpUnit.CurrentStatus = (UIData::Number<int>(model->WaterPumpUnit.ReadVar) ^ model->WaterPumpUnit.Logic);
			if (model->WaterPumpUnit.CurrentStatus)
			{
				Page()->RemoveTimeout(FluidCB, (void*)view);
				//Page()->AddTimeout(model->StirrerUnit.Speed, StirrerCB, (void*)view, true);
				if (model->WaterPumpUnit.InnerRectW > view->MarkCount)
				{
					FluidSpeedMarkWidth = model->WaterPumpUnit.InnerRectW / view->MarkCount;
				}
				else
				{
					view->MarkCount = model->WaterPumpUnit.InnerRectW;
					FluidSpeedMarkWidth = 1;
				}
				Page()->AddTimeout(10000.0 / (double)model->WaterPumpUnit.Speed, FluidCB, (void*)view, true);
			}
			else
				Page()->RemoveTimeout(FluidCB, (void*)view);
			
		}
		
		if (varId == model->WaterPumpUnit.Perm.ShowVID.Vid) {	// 控件显示
			PermUtility::HandleShowPerm(model->WaterPumpUnit.Perm, view);
		}
		if (varId == model->WaterPumpUnit.Perm.EnableVID.Vid) {	// 控件使能
			PermUtility::HandleEnablePerm(model->WaterPumpUnit.Perm, view);
		}
		view->position(view->x(), view->y());
		view->redraw();
	}


}

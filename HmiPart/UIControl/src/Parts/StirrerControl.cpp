/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerControl.cpp
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件控制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "Message.h"
#include "HMIWindow.h"
#include "StirrerView.h"
#include "StirrerControl.h"
#include "PermUtility.h"
#include "UIData.h"
#include "ViewShowUtility.h"

namespace UI
{
	StirrerControl::StirrerControl(HMIPage * win) : BaseControl(win) {
		// 创建Model对象
		model_ = shared_ptr<StirrerModel>(new StirrerModel());
		InitMVCModel(model_);
	}

	StirrerControl::~StirrerControl() {

	}

	void StirrerControl::CreateView() {
		// 初始化View
		StirrerView * view = new StirrerView(
			model_->StirrerUnit.X + model_->StirrerUnit.OffX,
			model_->StirrerUnit.Y + model_->StirrerUnit.OffY,
			model_->StirrerUnit.Width,
			model_->StirrerUnit.Height);
		InitMVCView(view);
	}
	void StirrerControl::OnReady()
	{
		HandleSysChange(Permission);
		/*StirrerModel* pMode = static_cast<StirrerModel*>(pModel_.get());
		StirrerView* pView = static_cast<StirrerView*>(pView_);*/
		//StartTick(true);
		
	}
	void StirrerControl::HandleSysChange(SysChangeEM catogray)
	{
		StirrerModel* mode_ = static_cast<StirrerModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->StirrerUnit.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->StirrerUnit.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->StirrerUnit.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				if (DataApi::GetDataFlag(mode_->StirrerUnit.ReadVar))
					mode_->StirrerUnit.CurrentStatus = (UIData::Number<int>(mode_->StirrerUnit.ReadVar) == mode_->StirrerUnit.Logic) ? 1 : 0;
				/*pView_->damage();
				pView_->redraw();*/
				mode_->StirrerUnit.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->StirrerUnit.Perm, mode_->StirrerUnit.X + mode_->StirrerUnit.OffX, mode_->StirrerUnit.Y + mode_->StirrerUnit.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	int StirrerControl::PeekHMIMessage(Message::Msg * msg) {
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

	void StirrerControl::HandleDataVar(Project::DataVarId & varId)
	{
		shared_ptr<StirrerModel> model = static_pointer_cast<StirrerModel>(model_);
		StirrerView * view = (StirrerView *)pView_;
		if (model->StirrerUnit.VOffX.Cmp(varId))
		{
			model->StirrerUnit.OffX = UI::UIData::Number<int>(model->StirrerUnit.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, model->StirrerUnit.Perm,
				model->StirrerUnit.X + model->StirrerUnit.OffX,
				model->StirrerUnit.Y + model->StirrerUnit.OffY);
		}
		if (model->StirrerUnit.VOffY.Cmp(varId))
		{
			model->StirrerUnit.OffY = UI::UIData::Number<int>(model->StirrerUnit.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, model->StirrerUnit.Perm,
				model->StirrerUnit.X + model->StirrerUnit.OffX,
				model->StirrerUnit.Y + model->StirrerUnit.OffY);
		}
		if (model->StirrerUnit.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(model->StirrerUnit.Perm, pView_);

		if (varId == model->StirrerUnit.ReadVar.Vid|| varId == model->StirrerUnit.SpVar.Vid) {

			model->StirrerUnit.CurrentStatus = (UIData::Number<int>(model->StirrerUnit.ReadVar) ^ model->StirrerUnit.Logic);

			if (model->StirrerUnit.CurrentStatus == 0) {
				Page()->RemoveTimeout(StirrerCB, (void*)view);
			}
			else if (model->StirrerUnit.CurrentStatus == 1) {
				//Page()->AddTimeout(10000.0 / (double)model->StirrerUnit.Speed, StirrerCB, (void*)view, true);
				if (NULL_VID_VALUE != model->StirrerUnit.SpVar.Vid)
				{
					model->StirrerUnit.Speed = UIData::Number<WORD>(model->StirrerUnit.SpVar);
					if (model->StirrerUnit.Speed < MINRUNSPEED)
						model->StirrerUnit.Speed = MINRUNSPEED;
					if (model->StirrerUnit.Speed > MAXRUNSPEED)
						model->StirrerUnit.Speed = MAXRUNSPEED;
				}
				Page()->RemoveTimeout(StirrerCB, (void*)view);
				//Page()->AddTimeout(model->StirrerUnit.Speed, StirrerCB, (void*)view, true);
				Page()->AddTimeout(10000.0 / (double)model->StirrerUnit.Speed, StirrerCB, (void*)view, true);

			}
		}
		if (varId == model->StirrerUnit.Perm.ShowVID.Vid) {	// 控件显示
			PermUtility::HandleShowPerm(model->StirrerUnit.Perm, view);
		}
		if (varId == model->StirrerUnit.Perm.EnableVID.Vid) {	// 控件使能
			PermUtility::HandleEnablePerm(model->StirrerUnit.Perm, view);
		}
		view->position(view->x(), view->y());
		view->redraw();
	}

	void StirrerCB(void * data) {
		StirrerView * view = static_cast<StirrerView*>(data);
		shared_ptr<StirrerModel> model = view->BaseView.GetModel<StirrerModel>();
		if (model->StirrerUnit.Dir == 0) {
			view->ResIndex = (view->ResIndex + 1) % 3;
		} else {
			view->ResIndex = 2 - (view->ResIndex % 3);
		}
		view->redraw();
	}
}

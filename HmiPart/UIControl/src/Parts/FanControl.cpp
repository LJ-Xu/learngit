/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanControl.cpp
 * Author   : qiaodan
 * Date     : 2021-03-31
 * Descript : 创建风扇绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "FanControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
namespace UI
{
	FanControl::FanControl(UI::HMIPage* w) :BaseControl(w)
	{
		//创建FanModel
		mode_ = std::shared_ptr <FanModel>(new FanModel());
		InitMVCModel(mode_);
	}

	void FanControl::CreateView()
	{
		//初始化VIEW
		FanView *view = new FanView(
			mode_->FanUnitConfig.X + mode_->FanUnitConfig.OffX,
			mode_->FanUnitConfig.Y + mode_->FanUnitConfig.OffY,
			mode_->FanUnitConfig.Width, mode_->FanUnitConfig.Height);
		InitMVCView(view);
		view->InitDraw();
		//OnReady();
	}

	void FanControl::OnReady()
	{
		if (mode_->FanUnitConfig.CycleTimeVarId != Project::DataVarId::NullId)
			mode_->FanUnitConfig.CycleTime = UI::UIData::Number<double>(mode_->FanUnitConfig.CycleTimeVarId);
		if (mode_->FanUnitConfig.CycleTime < 10) mode_->FanUnitConfig.CycleTime = 10;
		if (mode_->FanUnitConfig.CycleTime > 100)mode_->FanUnitConfig.CycleTime = 100;
		mode_->FanUnitConfig.CycleTime = (((int)mode_->FanUnitConfig.CycleTime + 5) / 10) * 10;

		if (mode_->FanUnitConfig.ActionMode == 0)
		{
			isswitch_ = true;
			if(mode_->FanUnitConfig.CycleTime != 0)
				Page()->AddTimeout((size_t)(10000.0 / (double)mode_->FanUnitConfig.CycleTime), FanTimerCb, (void *)this, true);
		}
		HandleSysChange(Permission);
	}
	void FanTimerCb(void *data)
	{
		FanControl *ctrl = (FanControl *)data;
		ctrl->HandleFanTimeOut();
	}
	void FanControl::HandleFanTimeOut()
	{
		FanView* pView = static_cast<FanView*>(pView_);
		if (pView->CurrentPic == mode_->FanUnitConfig.PicKeys.size() - 1)
			pView->CurrentPic = 0;
		else
			pView->CurrentPic++;
		if (mode_->FanUnitConfig.Perm.HasShowPerm && mode_->FanUnitConfig.Perm.HasLimitShowPerm)
			pView->redraw();

	}
	void FanControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->FanUnitConfig.VOffX != Project::DataVarId::NullId)
			mode_->FanUnitConfig.OffX = UI::UIData::Number<int>(mode_->FanUnitConfig.VOffX) + pageoffx;
		else
			mode_->FanUnitConfig.OffX = pageoffx;


		if (mode_->FanUnitConfig.VOffY != Project::DataVarId::NullId)
			mode_->FanUnitConfig.OffY = UI::UIData::Number<int>(mode_->FanUnitConfig.VOffY) + pageoffy;
		else
			mode_->FanUnitConfig.OffY = pageoffy;
	}

	void FanControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->FanUnitConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->FanUnitConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->FanUnitConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->FanUnitConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->FanUnitConfig.Perm,
					mode_->FanUnitConfig.X + mode_->FanUnitConfig.OffX,
					mode_->FanUnitConfig.Y + mode_->FanUnitConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
	void FanControl::HandleDataVar(Project::DataVarId &varId)
	{
		FanView* pView = static_cast<FanView*>(pView_);
		if (mode_->FanUnitConfig.VOffX.Cmp(varId))
		{
			mode_->FanUnitConfig.OffX = UI::UIData::Number<int>(mode_->FanUnitConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->FanUnitConfig.Perm, mode_->FanUnitConfig.X + mode_->FanUnitConfig.OffX, mode_->FanUnitConfig.Y + mode_->FanUnitConfig.OffY);
		}
		if (mode_->FanUnitConfig.VOffY.Cmp(varId))
		{
			mode_->FanUnitConfig.OffY = UI::UIData::Number<int>(mode_->FanUnitConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->FanUnitConfig.Perm, mode_->FanUnitConfig.X + mode_->FanUnitConfig.OffX, mode_->FanUnitConfig.Y + mode_->FanUnitConfig.OffY);
		}
		if (mode_->FanUnitConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->FanUnitConfig.Perm, pView_);
		if (mode_->FanUnitConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->FanUnitConfig.Perm, pView_);
		if (mode_->FanUnitConfig.CycleTimeVarId.Cmp(varId))
		{
			mode_->FanUnitConfig.CycleTime = UI::UIData::Number<double>(mode_->FanUnitConfig.CycleTimeVarId);
			
			if (mode_->FanUnitConfig.CycleTime < 10) mode_->FanUnitConfig.CycleTime = 10;
			if (mode_->FanUnitConfig.CycleTime > 100)mode_->FanUnitConfig.CycleTime = 100;
			mode_->FanUnitConfig.CycleTime = (((int)mode_->FanUnitConfig.CycleTime + 5) / 10) * 10;
			Page()->RemoveTimeout(FanTimerCb, (void*)this);
			if (isswitch_ == true && mode_->FanUnitConfig.CycleTime != 0)
				Page()->AddTimeout((size_t)(10000.0 / (double)mode_->FanUnitConfig.CycleTime), FanTimerCb, (void *)this, true);
		}
		if (mode_->FanUnitConfig.ActTrigVar.Cmp(varId))
		{
			bool meetcond = false;
			if (mode_->FanUnitConfig.RegMode == 0)
			{
				double value = UI::UIData::Number<double>(mode_->FanUnitConfig.ActTrigVar);
				XJDataType tp = UIDataService::Ins().GetDataType(mode_->FanUnitConfig.ActTrigVar);
				if(JudgeCondition::MeetCondition(mode_->FanUnitConfig.WordTrigCondition,
					value, mode_->FanUnitConfig.WordTrigValue, tp))
					meetcond = true;

				//switch (mode_->FanUnitConfig.WordTrigCondition)
				//{
				//case Project::Conditions::LT:
				//	if (value < mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//case Project::Conditions::GT:
				//	if (value > mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//case Project::Conditions::LE:
				//	if (value <= mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//case Project::Conditions::GE:
				//	if (value >= mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//case Project::Conditions::EQ:
				//	if (value == mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//case Project::Conditions::NE:
				//	if (value == mode_->FanUnitConfig.WordTrigValue)
				//		meetcond = true;
				//	break;
				//default:
				//	break;
				//}
				if (meetcond && isswitch_ == false)
				{
					Page()->RemoveTimeout(FanTimerCb, (void*)this);
					if(mode_->FanUnitConfig.CycleTime != 0)
						Page()->AddTimeout((size_t)(10000.0 / (double)mode_->FanUnitConfig.CycleTime), FanTimerCb, (void *)this, true);
					isswitch_ = true;
				}
				else
				{
					Page()->RemoveTimeout(FanTimerCb, (void*)this);
					isswitch_ = false;
				}
			}
			else
			{
				bool flag = UI::UIData::Bit(mode_->FanUnitConfig.ActTrigVar);
				if ((flag && mode_->FanUnitConfig.BitTrigCondition == 0) || 
					(!flag && mode_->FanUnitConfig.BitTrigCondition == 1) && 
					isswitch_ == false )		//满足开始旋转
				{
					Page()->RemoveTimeout(FanTimerCb, (void*)this);
					//Fl::add_timeout(((double)mode_->FanUnitConfig.CycleTime / 1000.0) / ((double)pView->PicImages.size()), FanTimerCb, (void *)&picdata_);
					if (mode_->FanUnitConfig.CycleTime != 0)
						Page()->AddTimeout((size_t)(10000.0 / (double)mode_->FanUnitConfig.CycleTime), FanTimerCb, (void *)this, true);
					isswitch_ = true;
				}
				else
				{
					Page()->RemoveTimeout(FanTimerCb, (void*)this);
					isswitch_ = false;
				}
			}
			
		}
	}
	int FanControl::PeekHMIMessage(Message::Msg* msg)
	{
		FanModel* pMode = static_cast<FanModel*>(pModel_.get());
		FanView* pView = static_cast<FanView*>(pView_);
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

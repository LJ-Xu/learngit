/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicControl.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 创建动态图片绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "HMIWindow.h"
#include "Message.h"
#include "GDynPicControl.h"
#include "UIData.h"
#include "PermUtility.h"
#include "ViewShowUtility.h"
#include "Logger.h"
namespace UI
{
	GDynPicControl::GDynPicControl(UI::HMIPage* w) :BaseControl(w)
	{
		//创建GDynPicModel
		mode_ = std::shared_ptr <GDynPicModel>(new GDynPicModel());
		InitMVCModel(mode_);
	}

	void GDynPicControl::CreateView()
	{
		//初始化VIEW
		GDynPicView *view = new GDynPicView(
			mode_->PicDynConfig.X + mode_->PicDynConfig.OffX,
			mode_->PicDynConfig.Y + mode_->PicDynConfig.OffY,
			mode_->PicDynConfig.Width, mode_->PicDynConfig.Height);
		InitMVCView(view);
		view->InitDraw();
		//OnReady();
	}

	void GDynPicControl::OnReady()
	{
		GDynPicView* pView = static_cast<GDynPicView*>(pView_);
		PicTimerData.Model = mode_.get();
		PicTimerData.View = pView;
		if (mode_->PicDynConfig.UseStartSign)
			prestartflag_ = UI::UIData::Bit(mode_->PicDynConfig.StartSignVarId);
		if (mode_->PicDynConfig.UseStopSign)
			prestopflag_ = UI::UIData::Bit(mode_->PicDynConfig.StopSignVarId);
		if (mode_->PicDynConfig.CycleTimeVarId != Project::DataVarId::NullId)
			mode_->PicDynConfig.CycleTime = (int)UI::UIData::Number<double>(mode_->PicDynConfig.CycleTimeVarId);
		if (!mode_->PicDynConfig.UseStartSign)
		{
			isswitch_ = true;
			if (mode_->PicDynConfig.CycleTime != 0)
			{
				LOG_INFO_("Start Switch DynPic\n");
				Page()->AddTimeout((size_t)((double)mode_->PicDynConfig.CycleTime / (double)mode_->PicDynConfig.ImageFmtList.size()), DynPicTimerCb, (void *)this, true);
			}
			LOG_INFO("PicDynConfig.CycleTime == %d\n", mode_->PicDynConfig.CycleTime);
		}
		HandleSysChange(Permission);
	}
	void DynPicTimerCb(void *data)
	{
		GDynPicControl *res = (GDynPicControl *)data;

		if (res->PicTimerData.Model->PicDynConfig.SwitchMode == Project::Mode_GondBack)
		{
			if (res->PicTimerData.Model->PicDynConfig.CurrentPic == res->PicTimerData.Model->PicDynConfig.ImageFmtList.size() - 1)
				res->PicTimerData.Model->PicDynConfig.Direction = 1;
			else if (res->PicTimerData.Model->PicDynConfig.CurrentPic == 0)
				res->PicTimerData.Model->PicDynConfig.Direction = 0;
			if (res->PicTimerData.Model->PicDynConfig.Direction)
				res->PicTimerData.Model->PicDynConfig.CurrentPic--;
			else
				res->PicTimerData.Model->PicDynConfig.CurrentPic++;
		}
		else if (res->PicTimerData.Model->PicDynConfig.SwitchMode == Project::Mode_OneWay)
		{
			if (res->PicTimerData.Model->PicDynConfig.CurrentPic == res->PicTimerData.Model->PicDynConfig.ImageFmtList.size() - 1)
				res->PicTimerData.Model->PicDynConfig.CurrentPic = 0;
			else
				res->PicTimerData.Model->PicDynConfig.CurrentPic++;
		}
		UI::ViewShowUtility::ShowView(res->PicTimerData.View, res->PicTimerData.Model->PicDynConfig.Perm,
			res->PicTimerData.Model->PicDynConfig.X + res->PicTimerData.Model->PicDynConfig.OffX,
			res->PicTimerData.Model->PicDynConfig.Y + res->PicTimerData.Model->PicDynConfig.OffY);
		//res->PicTimerData.View->redraw();
	}
	void GDynPicControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->PicDynConfig.VOffX != Project::DataVarId::NullId)
			mode_->PicDynConfig.OffX = UI::UIData::Number<int>(mode_->PicDynConfig.VOffX) + pageoffx;
		else
			mode_->PicDynConfig.OffX = pageoffx;


		if (mode_->PicDynConfig.VOffY != Project::DataVarId::NullId)
			mode_->PicDynConfig.OffY = UI::UIData::Number<int>(mode_->PicDynConfig.VOffY) + pageoffy;
		else
			mode_->PicDynConfig.OffY = pageoffy;
	}
	bool GDynPicControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->PicDynConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->PicDynConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->PicDynConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->PicDynConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->PicDynConfig.Perm,
					mode_->PicDynConfig.X + mode_->PicDynConfig.OffX,
					mode_->PicDynConfig.Y + mode_->PicDynConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	void GDynPicControl::HandleDataVar(Project::DataVarId &varId)
	{
		GDynPicView* pView = static_cast<GDynPicView*>(pView_);
		if (mode_->PicDynConfig.VOffX.Cmp(varId))
		{
			mode_->PicDynConfig.OffX = UI::UIData::Number<int>(mode_->PicDynConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_,mode_->PicDynConfig.Perm, mode_->PicDynConfig.X + mode_->PicDynConfig.OffX, mode_->PicDynConfig.Y + mode_->PicDynConfig.OffY);
		}
		if (mode_->PicDynConfig.VOffY.Cmp(varId))
		{
			mode_->PicDynConfig.OffY = UI::UIData::Number<int>(mode_->PicDynConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->PicDynConfig.Perm, mode_->PicDynConfig.X + mode_->PicDynConfig.OffX, mode_->PicDynConfig.Y + mode_->PicDynConfig.OffY);
		}
		if (mode_->PicDynConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->PicDynConfig.Perm, pView_);
		if (mode_->PicDynConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->PicDynConfig.Perm, pView_);
		if (mode_->PicDynConfig.CycleTimeVarId.Cmp(varId))
		{
			mode_->PicDynConfig.CycleTime = (int)UI::UIData::Number<double>(mode_->PicDynConfig.CycleTimeVarId);
			if (isswitch_ == true && mode_->PicDynConfig.CycleTime != 0)
			{
				Page()->RemoveTimeout(DynPicTimerCb, (void*)this);
				Page()->AddTimeout((size_t)((double)mode_->PicDynConfig.CycleTime / (double)mode_->PicDynConfig.ImageFmtList.size()), DynPicTimerCb, (void *)this, true);
			}
			else		//为0则停止
			{
				LOG_INFO("PicDynConfig.CycleTime == %d, Stop Switch\n", mode_->PicDynConfig.CycleTime);
				Page()->RemoveTimeout(DynPicTimerCb, (void*)this);
				isswitch_ = false;
			}
		}
		if (mode_->PicDynConfig.StartSignVarId.Cmp(varId) && mode_->PicDynConfig.UseStartSign)
		{
			bool flag = UI::UIData::Bit(mode_->PicDynConfig.StartSignVarId);
			if ((flag && prestartflag_ == false && mode_->PicDynConfig.StartTrigCondition == 0) ||
				(!flag && prestartflag_ == true && mode_->PicDynConfig.StartTrigCondition == 1))
			{
				if (isswitch_ == false)		//未开始切换图片
				{
					//Page()->RemoveTimeout(DynPicTimerCb, (void*)this);
					//Fl::add_timeout(((double)mode_->PicDynConfig.CycleTime / 1000.0) / ((double)pView->PicImages.size()), DynPicTimerCb, (void *)&picdata_);
					if (mode_->PicDynConfig.CycleTime != 0)
					{
						Page()->AddTimeout((size_t)((double)mode_->PicDynConfig.CycleTime / (double)mode_->PicDynConfig.ImageFmtList.size()), DynPicTimerCb, (void *)this, true);
						LOG_INFO_("PicDyn Start Switch\n");
					}
					isswitch_ = true;
				}
			}
			prestartflag_ = flag;
		}
		if (mode_->PicDynConfig.StopSignVarId.Cmp(varId) && mode_->PicDynConfig.UseStopSign)
		{
			bool flag = UI::UIData::Bit(mode_->PicDynConfig.StopSignVarId);
			if ((flag && prestopflag_ == false && mode_->PicDynConfig.EndTrigCondition == 0) ||
				(!flag && prestopflag_ == true && mode_->PicDynConfig.EndTrigCondition == 1))
			{
				Page()->RemoveTimeout(DynPicTimerCb, (void*)this);
				//Fl::remove_timeout(DynPicTimerCb);
				isswitch_ = false;
				LOG_INFO_("PicDyn Stop Switch\n");
			}
			prestopflag_ = flag;
		}
	}
	int GDynPicControl::PeekHMIMessage(Message::Msg* msg)
	{
		GDynPicView* pView = static_cast<GDynPicView*>(pView_);
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

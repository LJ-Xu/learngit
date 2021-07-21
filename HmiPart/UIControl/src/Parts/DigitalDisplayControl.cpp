/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建数值显示框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DigitalDisplayControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "UIComm.h"
#include "KeyMsgControl.h"
#include "KeyBtnControl.h"
#include <FL/Fl_Button.H>
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	DigitalDisplayControl::DigitalDisplayControl(HMIPage* w) :BaseControl(w)
	{
		//创建DigitalDisplayModel
		mode_ = std::shared_ptr <DigitalDisplayModel>(new DigitalDisplayModel());
		InitMVCModel(mode_);
	}

	void DigitalDisplayControl::CreateView()
	{
		//初始化VIEW
		DigitalDisplayView* view = new DigitalDisplayView(
							mode_->DisNumConfig.X + mode_->DisNumConfig.OffX,
							mode_->DisNumConfig.Y + mode_->DisNumConfig.OffY,
							mode_->DisNumConfig.Width,
							mode_->DisNumConfig.Height, nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->DisNumConfig.FmtStr;
		else
			view->DisplayValue = "";
		InitMVCView(view);
	}
	void DigitalDisplayControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->DisNumConfig.VOffX != Project::DataVarId::NullId)
			mode_->DisNumConfig.OffX = UI::UIData::Number<int>(mode_->DisNumConfig.VOffX) + pageoffx;
		else
			mode_->DisNumConfig.OffX = pageoffx;


		if (mode_->DisNumConfig.VOffY != Project::DataVarId::NullId)
			mode_->DisNumConfig.OffY = UI::UIData::Number<int>(mode_->DisNumConfig.VOffY) + pageoffy;
		else
			mode_->DisNumConfig.OffY = pageoffy;
	}
	bool DigitalDisplayControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DisNumConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DisNumConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DisNumConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->DisNumConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DisNumConfig.Perm,
					mode_->DisNumConfig.X + mode_->DisNumConfig.OffX,
					mode_->DisNumConfig.Y + mode_->DisNumConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}


	void DigitalDisplayControl::OnReady()
	{
		DigitalDisplayView* pView = static_cast<DigitalDisplayView*>(pView_);
		TimerData.Model = mode_.get();
		TimerData.View = pView;
		DOUBLE value = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.ReadVar);
		DDWORD da;
		//memset(&da, 0, sizeof(da));
		Utility::NumberFmtInfo fmt;
		fmt.Num1 = mode_->DisNumConfig.Accuracy.Act;
		fmt.Num2 = mode_->DisNumConfig.Accuracy.Bct;
		fmt.IsFillZero = mode_->DisNumConfig.IsFillZero;
		memcpy(&da, &value, sizeof(DOUBLE));
		XJDataType tp = UIDataService::Ins().GetDataType(mode_->DisNumConfig.ReadVar);
		pView->DisplayValue = Utility::DataFormate::NumFmt(da, tp, fmt);
		HandleSysChange(Permission);
	}

	void DigitalDisplayTimerCb(void *data)
	{
		DigitalDisplayControl *ctrl = (DigitalDisplayControl*)data;
		ctrl->TimerData.View->IsHideText = !ctrl->TimerData.View->IsHideText;
		UI::ViewShowUtility::ShowView(ctrl->TimerData.View, ctrl->TimerData.Model->DisNumConfig.Perm,
			ctrl->TimerData.Model->DisNumConfig.X + ctrl->TimerData.Model->DisNumConfig.OffX,
			ctrl->TimerData.Model->DisNumConfig.Y + ctrl->TimerData.Model->DisNumConfig.OffY);
	}
	void DigitalDisplayControl::HandleDataVar(Project::DataVarId &varId)
	{
		DigitalDisplayView* pView = static_cast<DigitalDisplayView*>(pView_);
		bool ischangevalue = false;
		if (mode_->DisNumConfig.VOffX.Cmp(varId))
		{
			mode_->DisNumConfig.OffX = UI::UIData::Number<int>(mode_->DisNumConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisNumConfig.Perm, mode_->DisNumConfig.X + mode_->DisNumConfig.OffX, mode_->DisNumConfig.Y + mode_->DisNumConfig.OffY);
		}
		if (mode_->DisNumConfig.VOffY.Cmp(varId))
		{
			mode_->DisNumConfig.OffY = UI::UIData::Number<int>(mode_->DisNumConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->DisNumConfig.Perm, mode_->DisNumConfig.X + mode_->DisNumConfig.OffX, mode_->DisNumConfig.Y + mode_->DisNumConfig.OffY);
		}
		if (mode_->DisNumConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->DisNumConfig.Perm, pView_);
		if (mode_->DisNumConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->DisNumConfig.Perm, pView_);
		/*后面应该有更改*/
		if (mode_->DisNumConfig.WarnUpper.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.WarnUpper.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.WarnUpper.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.WarnLower.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.WarnLower.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.WarnLower.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.NovUpperSrc.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.NovUpperSrc.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.NovUpperSrc.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.NovLowerSrc.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.NovLowerSrc.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.NovLowerSrc.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.NovUpperRes.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.NovUpperRes.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.NovUpperRes.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.NovLowerRes.DataVarLmt.Cmp(varId))
		{
			mode_->DisNumConfig.NovLowerRes.Num = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.NovLowerRes.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->DisNumConfig.ReadVar.Cmp(varId) || ischangevalue)
		{
			DOUBLE value = UI::UIData::Number<DOUBLE>(mode_->DisNumConfig.ReadVar);
			/*按比例转换数值*/
			if (mode_->DisNumConfig.IsConvertUsed)
			{
				if (mode_->DisNumConfig.NovUpperSrc.Num == mode_->DisNumConfig.NovLowerSrc.Num)
					value = 0;
				else
					value = mode_->DisNumConfig.NovLowerRes.Num + (value - mode_->DisNumConfig.NovLowerSrc.Num) *
						(mode_->DisNumConfig.NovUpperRes.Num - mode_->DisNumConfig.NovLowerRes.Num)/
						(mode_->DisNumConfig.NovUpperSrc.Num - mode_->DisNumConfig.NovLowerSrc.Num);
			}
			//判断是否超出限值
			if (mode_->DisNumConfig.IsWarnUpperUsed && value > mode_->DisNumConfig.WarnUpper.Num)
			{
				pView->IsExceedMinLmt = false;
				pView->IsExceedMaxLmt = true;

				if (mode_->DisNumConfig.WarnStyleUpper.Flash)	//闪烁
				{
					if (HasTimer)
					{
						Page()->RemoveTimeout(DigitalDisplayTimerCb, (void *)this);
						Page()->AddTimeout(500, DigitalDisplayTimerCb, (void *)this, true);
					}
					else
					{
						Page()->AddTimeout(500, DigitalDisplayTimerCb, (void *)this, true);
						HasTimer = true;
					}
				}
			}
			else if (mode_->DisNumConfig.IsWarnLowerUsed && value < mode_->DisNumConfig.WarnLower.Num)
			{
				pView->IsExceedMinLmt = true;
				pView->IsExceedMaxLmt = false;
				Page()->RemoveTimeout(DigitalDisplayTimerCb, (void *)this);

				if (mode_->DisNumConfig.WarnStyleLower.Flash)	//闪烁
				{
					if (HasTimer)
					{
						Page()->RemoveTimeout(DigitalDisplayTimerCb, (void *)this);
						Page()->AddTimeout(500, DigitalDisplayTimerCb, (void *)this, true);
					}
					else
					{
						Page()->AddTimeout(500, DigitalDisplayTimerCb, (void *)this, true);
						HasTimer = true;
					}
				}
			}
			else
			{
				if (HasTimer)
				{
					HasTimer = false;
					Page()->RemoveTimeout(DigitalDisplayTimerCb, (void *)this);
				}
				pView->IsExceedMaxLmt = false;
				pView->IsExceedMinLmt = false;
				pView->IsHideText = false;
			}
			//将数值按格式转换
			DDWORD da;
			//memset(&da, 0, sizeof(da));
			Utility::NumberFmtInfo fmt;
			fmt.Num1 = mode_->DisNumConfig.Accuracy.Act;
			fmt.Num2 = mode_->DisNumConfig.Accuracy.Bct;
			fmt.IsFillZero = mode_->DisNumConfig.IsFillZero;
			memcpy(&da, &value, sizeof(DOUBLE));
			XJDataType tp = UIDataService::Ins().GetDataType(mode_->DisNumConfig.ReadVar);
			pView->DisplayValue = Utility::DataFormate::NumFmt(da,tp,fmt);
			if (mode_->DisNumConfig.Perm.HasShowPerm && mode_->DisNumConfig.Perm.HasLimitShowPerm)
				pView->redraw();
		}
	}
	int DigitalDisplayControl::PeekHMIMessage(Message::Msg* msg)
	{
		DigitalDisplayView* pView = static_cast<DigitalDisplayView*>(pView_);
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


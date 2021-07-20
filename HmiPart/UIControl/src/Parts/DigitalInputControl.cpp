/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputControl.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 创建数值输入框，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DigitalInputControl.h"
#include "DigitalInputModel.h"
#include "DigitalInputView.h"
#include "HMIWindow.h"
#include "Message.h"
#include "UIComm.h"
#include "KeyMsgControl.h"
#include "KeyBtnControl.h"
#include <FL/Fl_Button.H>
#include "DigitalInputView.h"
#include <FL/Fl_Widget.H>
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	DigitalInputControl::DigitalInputControl(HMIPage* w) :BaseControl(w)
	{
		//创建DigitalInputModel
		mode_ = std::shared_ptr < DigitalInputModel>(new DigitalInputModel());
		InitMVCModel(mode_);
	}

	void DigitalInputControl::CreateView() 
	{
		//初始化VIEW
		DigitalInputView* view = new DigitalInputView(
			mode_->InputNumConfig.X + mode_->InputNumConfig.OffX,
			mode_->InputNumConfig.Y + mode_->InputNumConfig.OffY,
			mode_->InputNumConfig.Width,
			mode_->InputNumConfig.Height,nullptr);
		if (IResourceService::Ins()->IsRenderMode())
			view->DisplayValue = mode_->InputNumConfig.FmtStr;
		else
			view->DisplayValue = "";
		InitMVCView(view);
	}

	void DigitalInputControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->InputNumConfig.VOffX != Project::DataVarId::NullId)
			mode_->InputNumConfig.OffX = UI::UIData::Number<int>(mode_->InputNumConfig.VOffX) + pageoffx;
		else
			mode_->InputNumConfig.OffX = pageoffx;


		if (mode_->InputNumConfig.VOffY != Project::DataVarId::NullId)
			mode_->InputNumConfig.OffY = UI::UIData::Number<int>(mode_->InputNumConfig.VOffY) + pageoffy;
		else
			mode_->InputNumConfig.OffY = pageoffy;
	}
	bool DigitalInputControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->InputNumConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->InputNumConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->InputNumConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->InputNumConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->InputNumConfig.Perm,
					mode_->InputNumConfig.X + mode_->InputNumConfig.OffX, 
					mode_->InputNumConfig.Y + mode_->InputNumConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	
	void DigitalInputControl::OnReady()
	{
		DigitalInputView* pView = static_cast<DigitalInputView*>(pView_);
		TimerData.Model = mode_.get();
		TimerData.View = pView;
		HandleSysChange(Permission);
	}
	void DigitalInputTimerCb(void *data)
	{
		DigitalInputControl *ctrl = (DigitalInputControl*)data;
		ctrl->TimerData.View->IsHideText = !ctrl->TimerData.View->IsHideText;
		UI::ViewShowUtility::ShowView(ctrl->TimerData.View, ctrl->TimerData.Model->InputNumConfig.Perm,
			ctrl->TimerData.Model->InputNumConfig.X + ctrl->TimerData.Model->InputNumConfig.OffX,
			ctrl->TimerData.Model->InputNumConfig.Y + ctrl->TimerData.Model->InputNumConfig.OffY);
	}
	void DigitalInputControl::HandleDataVar(Project::DataVarId &varId)
	{
		DigitalInputView* pView = static_cast<DigitalInputView*>(pView_);
		if (mode_->InputNumConfig.VOffX.Cmp(varId))
		{
			mode_->InputNumConfig.OffX = UI::UIData::Number<int>(mode_->InputNumConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputNumConfig.Perm, 
				mode_->InputNumConfig.X + mode_->InputNumConfig.OffX,
				mode_->InputNumConfig.Y + mode_->InputNumConfig.OffY);
		}
		if (mode_->InputNumConfig.VOffY.Cmp(varId))
		{
			mode_->InputNumConfig.OffY = UI::UIData::Number<int>(mode_->InputNumConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->InputNumConfig.Perm,
				mode_->InputNumConfig.X + mode_->InputNumConfig.OffX,
				mode_->InputNumConfig.Y + mode_->InputNumConfig.OffY);
		}
		if (mode_->InputNumConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->InputNumConfig.Perm, pView_);
		if (mode_->InputNumConfig.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(mode_->InputNumConfig.Perm, pView_);
		bool ischangevalue = false;
		/*显示数据有变化 ***** 后面应该有更改*/
		if (mode_->InputNumConfig.WarnUpper.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.WarnUpper.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.WarnUpper.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.WarnLower.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.WarnLower.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.WarnLower.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.NovUpperSrc.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovUpperSrc.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovUpperSrc.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.NovLowerSrc.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovLowerSrc.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovLowerSrc.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.NovUpperRes.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovUpperRes.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovUpperRes.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.NovLowerRes.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovLowerRes.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovLowerRes.DataVarLmt);
			ischangevalue = true;
		}
		if (mode_->InputNumConfig.ReadVar.Cmp(varId) || ischangevalue)
		{
			DOUBLE value = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.ReadVar);
			/*按比例转换数值*/
			if (mode_->InputNumConfig.IsConvertUsed)
			{
				if (mode_->InputNumConfig.NovUpperSrc.Num == mode_->InputNumConfig.NovLowerSrc.Num)
					value = 0;
				else
					value = mode_->InputNumConfig.NovLowerRes.Num + (value - mode_->InputNumConfig.NovLowerSrc.Num) *
						(mode_->InputNumConfig.NovUpperRes.Num - mode_->InputNumConfig.NovLowerRes.Num) /
						(mode_->InputNumConfig.NovUpperSrc.Num - mode_->InputNumConfig.NovLowerSrc.Num);
			}
			//判断是否超出限值
			if (mode_->InputNumConfig.IsWarnUpperUsed && value > mode_->InputNumConfig.WarnUpper.Num)
			{
				pView->IsExceedMinLmt = false;
				pView->IsExceedMaxLmt = true;

				if (mode_->InputNumConfig.WarnStyleUpper.Flash)	//闪烁
				{
					if (HasTimer)
					{
						Page()->RemoveTimeout(DigitalInputTimerCb, (void *)this);
						Page()->AddTimeout(500, DigitalInputTimerCb, (void *)this, true);
					}
					else
					{
						Page()->AddTimeout(500, DigitalInputTimerCb, (void *)this, true);
						HasTimer = true;
					}
				}
			}
			else if (mode_->InputNumConfig.IsWarnLowerUsed && value < mode_->InputNumConfig.WarnLower.Num)
			{
				pView->IsExceedMinLmt = true;
				pView->IsExceedMaxLmt = false;
				Page()->RemoveTimeout(DigitalInputTimerCb, (void *)this);

				if (mode_->InputNumConfig.WarnStyleLower.Flash)	//闪烁
				{
					if (HasTimer)
					{
						Page()->RemoveTimeout(DigitalInputTimerCb, (void *)this);
						Page()->AddTimeout(500, DigitalInputTimerCb, (void *)this, true);
					}
					else
					{
						Page()->AddTimeout(500, DigitalInputTimerCb, (void *)this, true);
						HasTimer = true;
					}
					//Page()->RemoveTimeout(DigitalInputTimerCb, (void *)this);
				}
			}
			else
			{
				if (HasTimer)
				{
					HasTimer = false;
					Page()->RemoveTimeout(DigitalInputTimerCb, (void *)this);
				}
				pView->IsExceedMaxLmt = false;
				pView->IsExceedMinLmt = false;
				pView->IsHideText = false;
			}
			//将数值按格式转换
			DDWORD da;
			//memset(&da, 0, sizeof(da));
			Utility::NumberFmtInfo fmt;
			fmt.Num1 = mode_->InputNumConfig.Accuracy.Act;
			fmt.Num2 = mode_->InputNumConfig.Accuracy.Bct;
			fmt.IsFillZero = mode_->InputNumConfig.IsFillZero;
			memcpy(&da, &value, sizeof(DDWORD));
			XJDataType tp = UIDataService::Ins().GetDataType(mode_->InputNumConfig.ReadVar);
			
			pView->DisplayValue = Utility::DataFormate::NumFmt(da, tp, fmt);
			if (mode_->InputNumConfig.Perm.HasShowPerm && mode_->InputNumConfig.Perm.HasLimitShowPerm)
				pView->redraw();
		}
		if (mode_->InputNumConfig.NovUpperSrcInput.DataVarLmt.Cmp(varId))
			mode_->InputNumConfig.NovUpperSrcInput.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovUpperSrcInput.DataVarLmt);
		if (mode_->InputNumConfig.NovLowerSrcInput.DataVarLmt.Cmp(varId))
			mode_->InputNumConfig.NovLowerSrcInput.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovLowerSrcInput.DataVarLmt);
		if (mode_->InputNumConfig.NovUpperResInput.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovUpperResInput.Num = UI::UIData::Number<DOUBLE>(mode_->InputNumConfig.NovUpperResInput.DataVarLmt);
			if (pView->OpenKeypage)
			{
				/*设置上下限寄存器*/
				pView->MaxValue = mode_->InputNumConfig.NovUpperResInput.Num;
				LocalData::SetNumberData<DOUBLE>(SYS_PSW_INPUTKEY_UPPERERLMT, mode_->InputNumConfig.NovUpperResInput.Num);
			}
		}
		if (mode_->InputNumConfig.NovLowerResInput.DataVarLmt.Cmp(varId))
		{
			mode_->InputNumConfig.NovLowerResInput.Num = UI::UIData::Number<double>(mode_->InputNumConfig.NovLowerResInput.DataVarLmt);
			if (pView->OpenKeypage)
			{
				/*设置上下限寄存器*/
				pView->MinValue = mode_->InputNumConfig.NovLowerResInput.Num;
				LocalData::SetNumberData(SYS_PSW_INPUTKEY_LOWERLMT, mode_->InputNumConfig.NovLowerResInput.Num);
			}
		}
		if (mode_->InputNumConfig.KeyMethod.KeypadVar.Cmp(varId))
		{
			if (UI::UIData::Bit(mode_->InputNumConfig.KeyMethod.KeypadVar))
			{
				map<int, vector<WinCtrlID>>::iterator  iter;
				iter = mode_->InputNumConfig.SequenceGroup.find(mode_->InputNumConfig.Sequence.GroupId);
				if (iter != mode_->InputNumConfig.SequenceGroup.end())
				{
					if (!iter->second.empty())
					{
						bool needfocus = true;
						for (size_t i = 0; i < iter->second.size(); i++)
						{
							std::shared_ptr<BaseControl> nextctrl = Page()->FindCtrl(GetCtrlNo(iter->second[i]));
							InputView *nextview = (InputView *)nextctrl->GetView();
							if (nextview->NeedHandleFocus || nextview->OpenKeypage)
							{
								needfocus = false;
								break;
							}
						}
						if (needfocus)
						{
							pView->NeedHandleFocus = true;
							pView->take_focus();
						}
						//std::shared_ptr<BaseControl> nextctrl = Page()->FindCtrl(GetCtrlNo(iter->second[0]));
						//InputView *nextview = (InputView *)nextctrl->GetView();
						//pView->NeedHandleFocus = false;
						//nextview->BitVid = mode_->InputNumConfig.KeyMethod.KeypadVar;
						//nextview->NeedHandleFocus = true;
						//pView->NeedHandleFocus = true;
						//pView->take_focus();
						//Fl::focus(nextview);
					}
				}
			}
		}
	}

	int DigitalInputControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId;
		DigitalInputView* pView = static_cast<DigitalInputView*>(pView_);
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			varId = Project::DataVarId(msg->LParam);
			HandleDataVar(varId);
			break;
		case WM_EVENT_CLICKED:
			pView->EnterValue();
		default:
			break;
		}
		return 1;//handled;
	}
}

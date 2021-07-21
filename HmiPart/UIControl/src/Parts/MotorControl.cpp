#include "stdafx.h"
#include "HMIWindow.h"
#include "MotorControl.h"
#include "MotorView.h"
#include "MotorConfig.h"
#include "Message.h"
#include "HMIPage.h"
#include "UIData.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
namespace UI
{
	void Blink_func(void *data)
	{
		MotorControl * ctl = (MotorControl *)data;
		ctl->BlinkMoment();
		//Fl::repeat_timeout(ctl->BlinkSpeed, Blink_func, data);
	}

	//static shared_ptr<MotorModel> Mode;

	MotorControl::MotorControl(HMIPage* w):BaseControl(w),IsBlinking(false),BlinkSpeed(-1)
	{
		//创建MotorControl
		mode_ = std::shared_ptr < MotorModel>(new MotorModel());
		InitMVCModel(mode_);
	}

	void MotorControl::CreateView()
	{
		//初始化VIEW
		MotorView* view = new MotorView(mode_->MotorConfig.X, mode_->MotorConfig.Y,
			mode_->MotorConfig.Width, mode_->MotorConfig.Height);

		InitMVCView(view);
		//if (!mode_->MotorConfig.CtlLogic)		//负逻辑
		//	mode_->MotorConfig.CurrentStatus_ = 1;	//ON状态
		//else	//正逻辑
		//	mode_->MotorConfig.CurrentStatus_ = 0;	//OFF状态
	}

	void MotorControl::BlinkMoment()
	{
		MotorView* pView = static_cast<MotorView*>(pView_);
		MotorModel* pMode = static_cast<MotorModel*>(pModel_.get());
		if (pView->IsBlinking)
		{
			pView->CurBlinkState ^= 1;
			//pView->CurBlinkState = (pView->CurBlinkState + 1) % 2;
			pMode->MotorConfig.CurrentStatus_ = pView->CurBlinkState;
		}
		pView_->damage();
		pView_->redraw();
	}

	void MotorControl::OnReady()
	{
		HandleSysChange(Permission);
	}
	bool MotorControl::HandleSysChange(SysChangeEM catogray)
	{
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->MotorConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->MotorConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->MotorConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				mode_->MotorConfig.CurrentStatus_ = UIData::Number<int>(mode_->MotorConfig.DataCtlSign);
				mode_->MotorConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->MotorConfig.Perm, mode_->MotorConfig.X + mode_->MotorConfig.OffX, mode_->MotorConfig.Y + mode_->MotorConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}
	int MotorControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId = Project::DataVarId(msg->LParam);
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			HandleDataVar(varId);
			break;
		default:
			break;
		}
		return 1;//handled;
	}

	void MotorControl::BlinkTick(MotorModel* pMod)
	{
		//当前状态
		//status\logic		0   1
		//		0			1	0
		//		1			0	1

		pMod->MotorConfig.CurrentStatus_ = UIData::Number<int>(pMod->MotorConfig.DataCtlSign);
		if (!pMod->MotorConfig.UseBlink)
		{
			return;
		}
		//闪烁匹配
		if (pMod->MotorConfig.BlinkState != pMod->MotorConfig.CurrentStatus_)
		{
			if (this->BlinkSpeed == -1)
			{
				this->BlinkSpeed = (double)(pMod->MotorConfig.BlinkFrequency);
			}
			if (!this->IsBlinking)
			{
				//Page()->RemoveTimeout(Blink_func, (void*)this);
				Page()->AddTimeout(this->BlinkSpeed, Blink_func, (void*)this,true);
				IsBlinking = true;
			}/*
			else
				pMod->m_motorConfig.CurrentStatus_ = CurBlinkState;*/
		}
		else
		{
			if (this->IsBlinking)
			{
				Page()->RemoveTimeout(Blink_func, (void*)this);
				IsBlinking = false;
			}
		}
	}

	void MotorControl::HandleDataVar(Project::DataVarId & varId)
	{
		MotorModel* pMode = static_cast<MotorModel*>(pModel_.get());
		MotorView* pView = static_cast<MotorView*>(pView_);
		if (mode_->MotorConfig.VOffX.Cmp(varId))
		{
			mode_->MotorConfig.OffX = UI::UIData::Number<int>(mode_->MotorConfig.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, mode_->MotorConfig.Perm,
				mode_->MotorConfig.X + mode_->MotorConfig.OffX,
				mode_->MotorConfig.Y + mode_->MotorConfig.OffY);
		}
		if (mode_->MotorConfig.VOffY.Cmp(varId))
		{
			mode_->MotorConfig.OffY = UI::UIData::Number<int>(mode_->MotorConfig.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, mode_->MotorConfig.Perm,
				mode_->MotorConfig.X + mode_->MotorConfig.OffX,
				mode_->MotorConfig.Y + mode_->MotorConfig.OffY);
		}
		if (mode_->MotorConfig.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(mode_->MotorConfig.Perm, pView_);


		//StartTick(true);
		this->BlinkTick(pMode);
		pView->IsBlinking = this->IsBlinking;
		pView_->damage();
		pView_->redraw();
	}



}
#include "ClockControl.h"
#include "stdafx.h"
#include "ClockView.h"
#include "ClockConfig.h"
#include "HMIWindow.h"
#include "Message.h"
#include "HMIPage.h"
#include "System.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	void UpdateClock(void *data)
	{
		ClockControl* pdc = static_cast<ClockControl*>(data);
		pdc->StartTick();
	}


	ClockControl::ClockControl(HMIPage* w) :BaseControl(w)
	{
		//创建ClockModel
		mode_ = std::shared_ptr <ClockModel>(new ClockModel());
		InitMVCModel(mode_);
	}
	void ClockControl::CreateView()
	{
		//初始化VIEW
		ClockView* view = new ClockView(mode_->ClockConfig.X, mode_->ClockConfig.Y,
			mode_->ClockConfig.Width, mode_->ClockConfig.Height);
		InitMVCView(view);
		view->InitBackPic(mode_.get());
	}
	void ClockControl::SetOffSetPos(int pageoffx, int pageoffy)
	{
		if (mode_->ClockConfig.VOffX != Project::DataVarId::NullId)
			mode_->ClockConfig.OffX = UI::UIData::Number<int>(mode_->ClockConfig.VOffX) + pageoffx;
		else
			mode_->ClockConfig.OffX = pageoffx;


		if (mode_->ClockConfig.VOffY != Project::DataVarId::NullId)
			mode_->ClockConfig.OffY = UI::UIData::Number<int>(mode_->ClockConfig.VOffY) + pageoffy;
		else
			mode_->ClockConfig.OffY = pageoffy;
	}
	void ClockControl::StartTick(bool firstStart)
	{
		unsigned long long lastTime = 500;
		//计算此刻到一秒/分的剩余时间(单位：豪秒)
		if (!firstStart)
		{
			switch (mode_->ClockConfig.TimeFormat)
			{
			case 0:
			case 2:
				lastTime = 500;
				break;
			case 1:
				lastTime = 30000;
				break;
			case 3:
				lastTime = 50;
				break;
			}
		}
		else
		{
			if (mode_->ClockConfig.TimeFormat == 1)
			{
				lastTime = System::GetNextMinuteNodeMs();
			}
		}
		////到第二天刷新
		//ClockView* pView = static_cast<ClockView*>(pView_);
		//ClockModel* pModel = static_cast<ClockModel*>(mode_.get());
		////pView_->damage();
		//pView_->hide();
		//pView_->redraw();
		//if (pModel)
		//{
		//	if (pModel->ClockConfig.Perm.HasShowPerm)
		//		pView_->show();
		//}

		HandleSysChange(Permission);
		Page()->AddTimeout(lastTime, UpdateClock, (void*)this);
	}

	void ClockControl::StopTick()
	{
		ClockView* pView = static_cast<ClockView*>(pView_);
		Page()->RemoveTimeout(UpdateClock, (void*)pView);
	}

	void ClockControl::OnReady()
	{
		HandleSysChange(Permission);
		StartTick(true);
	}

	bool ClockControl::HandleSysChange(SysChangeEM catogray)
	{
		ClockModel* mode_ = static_cast<ClockModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->ClockConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->ClockConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->ClockConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				/*pView_->damage();
				pView_->redraw();*/
				mode_->ClockConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->ClockConfig.Perm, mode_->ClockConfig.X + mode_->ClockConfig.OffX, mode_->ClockConfig.Y + mode_->ClockConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}



	int ClockControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId = Project::DataVarId(msg->LParam);
		ClockView* pView = static_cast<ClockView*>(pView_);
		switch (msg->GetCode())
		{
		case WM_EVENT_DATEUPDATE:
			if (mode_->ClockConfig.VOffX.Cmp(varId))
			{
				mode_->ClockConfig.OffX = UI::UIData::Number<int>(mode_->ClockConfig.VOffX) + Page()->GetPageOffX();
				UI::ViewShowUtility::ShowView(pView_, mode_->ClockConfig.Perm,
					mode_->ClockConfig.X + mode_->ClockConfig.OffX,
					mode_->ClockConfig.Y + mode_->ClockConfig.OffY);
			}
			if (mode_->ClockConfig.VOffY.Cmp(varId))
			{
				mode_->ClockConfig.OffY = UI::UIData::Number<int>(mode_->ClockConfig.VOffY) + Page()->GetPageOffY();
				UI::ViewShowUtility::ShowView(pView_, mode_->ClockConfig.Perm,
					mode_->ClockConfig.X + mode_->ClockConfig.OffX,
					mode_->ClockConfig.Y + mode_->ClockConfig.OffY);
			}
			if (mode_->ClockConfig.Perm.ShowVID.Cmp(varId))
				UI::PermUtility::HandleShowPerm(mode_->ClockConfig.Perm, pView_);
			break;
		case WM_EVENT_CLICKED:
			break;
		default:
			break;
		}
		return 1;
	}
}

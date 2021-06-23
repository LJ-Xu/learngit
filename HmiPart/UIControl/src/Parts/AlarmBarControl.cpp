#include "stdafx.h"
#include "HMIWindow.h"
#include "AlarmBarControl.h"
#include "AlarmBarView.h"
#include "Message.h"
#include "SysCtrlApi.h"
#include "HMIPage.h"
#include "BaseStorage.h"
#include "StorageServiceImpl.h"
#include "UIData.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "AlarmRecord.h"
namespace UI
{
	void UpdateAlarm(void *data)
	{
		AlarmBarControl* pControl = (AlarmBarControl*)data;
		pControl->HandleSysChange(Permission);
	}

	static shared_ptr<AlarmBarModel> Mode;
	AlarmBarControl::AlarmBarControl(HMIPage* w) :BaseControl(w), IsNewAlarm(false)
	{
		//创建AlarmBarModel
		Mode = std::shared_ptr < AlarmBarModel>(new AlarmBarModel());
		InitMVCModel(Mode);
	}

	void AlarmBarControl::CreateView()
	{
		//初始化VIEW
		AlarmBarView* view = new AlarmBarView(Mode->AlarmBarConfig.X, Mode->AlarmBarConfig.Y,
			Mode->AlarmBarConfig.Width, Mode->AlarmBarConfig.Height);
		InitMVCView(view);
	}

	int AlarmBarControl::PeekHMIMessage(Message::Msg* msg)
	{
		AlarmBarModel* pMode = static_cast<AlarmBarModel*>(pModel_.get());
		AlarmBarView* pView = static_cast<AlarmBarView*>(pView_);
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			break;
		default:
			break;
		}
		return 1;//handled;
	}

	void AlarmBarControl::StartScoll()
	{
		if (IsRuning)
		{
			return;
		}
		IsRuning = true;
		IsNewAlarm = true;
		unsigned long long lastTime = 500;
		/*pView_->damage();
		pView_->redraw();*/
		Page()->AddTimeout(lastTime, UpdateAlarm, (void*)this,true);
	}
	void AlarmBarControl::Stop()
	{
		if (!IsRuning)
		{
			return;
		}
		IsRuning = false;
		Page()->RemoveTimeout(UpdateAlarm, this);
	}
	void AlarmBarControl::OnReady()
	{
		HandleSysChange(Permission);
		StartScoll();
	}
	void AlarmBarControl::HandleSysChange(SysChangeEM catogray)
	{
		AlarmBarModel* mode_ = static_cast<AlarmBarModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->AlarmBarConfig.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->AlarmBarConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->AlarmBarConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				AlarmBarView* pView = static_cast<AlarmBarView*>(pView_);
				pView->CurDx++;
				mode_->AlarmBarConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->AlarmBarConfig.Perm, mode_->AlarmBarConfig.X + mode_->AlarmBarConfig.OffX, mode_->AlarmBarConfig.Y + mode_->AlarmBarConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
}
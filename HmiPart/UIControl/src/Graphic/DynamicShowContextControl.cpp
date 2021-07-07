#include "stdafx.h"
#include "HMIWindow.h"
#include "DynamicTextContextControl.h"
#include "DynamicTextContextView.h"
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
	void UpdateDynamicContext(void *data)
	{
		DynamicTextContextControl* pControl = (DynamicTextContextControl*)data;
		pControl->HandleSysChange(Permission);
	}

	static shared_ptr<DynamicTextBarModel> Mode;
	DynamicTextContextControl::DynamicTextContextControl(HMIPage* w) :BaseControl(w), IsNewAlarm(false)
	{
		//创建DynamicTextBarModel
		Mode = std::shared_ptr < DynamicTextBarModel>(new DynamicTextBarModel());
		InitMVCModel(Mode);
	}

	void DynamicTextContextControl::CreateView()
	{
		//初始化VIEW
		DynamicTextBarView* view = new DynamicTextBarView(Mode->DynamicTextBarUnit.X, Mode->DynamicTextBarUnit.Y,
			Mode->DynamicTextBarUnit.Width, Mode->DynamicTextBarUnit.Height);
		InitMVCView(view);
	}

	int DynamicTextContextControl::PeekHMIMessage(Message::Msg* msg)
	{
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

	void DynamicTextContextControl::StartScoll()
	{
		if (IsRuning)
		{
			return;
		}
		IsRuning = true;
		//unsigned long long lastTime = 500;
		/*pView_->damage();
		pView_->redraw();*/
		Page()->AddTimeout(100, UpdateDynamicContext, (void*)this, true);
	}
	void DynamicTextContextControl::Stop()
	{
		if (!IsRuning)
		{
			return;
		}
		IsRuning = false;
		Page()->RemoveTimeout(UpdateDynamicContext, this);
	}
	void DynamicTextContextControl::OnReady()
	{
		HandleSysChange(Permission);
		StartScoll();
	}
	void DynamicTextContextControl::HandleDataVar(Project::DataVarId &varId)
	{
		DynamicTextBarView* pView = static_cast<DynamicTextBarView*>(pView_);
		if (Mode->DynamicTextBarUnit.VOffX.Cmp(varId))
		{
			Mode->DynamicTextBarUnit.OffX = UI::UIData::Number<int>(Mode->DynamicTextBarUnit.VOffX) + Page()->GetPageOffX();
			UI::ViewShowUtility::ShowView(pView_, Mode->DynamicTextBarUnit.Perm, Mode->DynamicTextBarUnit.X + Mode->DynamicTextBarUnit.OffX, Mode->DynamicTextBarUnit.Y + Mode->DynamicTextBarUnit.OffY);
		}
		if (Mode->DynamicTextBarUnit.VOffY.Cmp(varId))
		{
			Mode->DynamicTextBarUnit.OffY = UI::UIData::Number<int>(Mode->DynamicTextBarUnit.VOffY) + Page()->GetPageOffY();
			UI::ViewShowUtility::ShowView(pView_, Mode->DynamicTextBarUnit.Perm, Mode->DynamicTextBarUnit.X + Mode->DynamicTextBarUnit.OffX, Mode->DynamicTextBarUnit.Y + Mode->DynamicTextBarUnit.OffY);
		}
		if (Mode->DynamicTextBarUnit.Perm.ShowVID.Cmp(varId))
			UI::PermUtility::HandleShowPerm(Mode->DynamicTextBarUnit.Perm, pView_);
		if (Mode->DynamicTextBarUnit.Perm.EnableVID.Cmp(varId))
			UI::PermUtility::HandleEnablePerm(Mode->DynamicTextBarUnit.Perm, pView_);
	}
	void DynamicTextContextControl::HandleSysChange(SysChangeEM catogray)
	{
		DynamicTextBarModel* mode_ = static_cast<DynamicTextBarModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*判断是否有权限*/
			if (!UI::PermUtility::HasUserPerm(mode_->DynamicTextBarUnit.Perm.RequireRegion))	//没有权限并且隐藏
			{
				if (mode_->DynamicTextBarUnit.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DynamicTextBarUnit.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				DynamicTextBarView* pView = static_cast<DynamicTextBarView*>(pView_);
				pView->CurDx++;
				mode_->DynamicTextBarUnit.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DynamicTextBarUnit.Perm, mode_->DynamicTextBarUnit.X + mode_->DynamicTextBarUnit.OffX, mode_->DynamicTextBarUnit.Y + mode_->DynamicTextBarUnit.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
	}
}
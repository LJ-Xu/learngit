#include "stdafx.h"
#include "DateControl.h"
#include "DateView.h"
#include "DateConfig.h"
#include "HMIWindow.h"
#include "Message.h"
#include "HMIPage.h"
#include "System.h"
#include "ViewShowUtility.h"
#include "PermUtility.h"
#include "UIData.h"
namespace UI
{
	void UpdateDate(void *data)
	{
		DateControl* pdc = static_cast<DateControl*>(data);
		pdc->StartTick();
	}
	
	DateControl::DateControl(HMIPage* w) :BaseControl(w)
	{
		//����DateModel
		mode_ = std::shared_ptr <DateModel>(new DateModel());
		InitMVCModel(mode_);
	}
	void DateControl::CreateView()
	{
		//��ʼ��VIEW
		DateView* view = new DateView(mode_->DateConfig.X, mode_->DateConfig.Y,
			mode_->DateConfig.Width, mode_->DateConfig.Height);
		view->InitBackPic(mode_.get());
		InitMVCView(view);
		//StartTick(true);
	}

	void DateControl::StartTick(bool firstStart)
	{
		//����˿̵���һ���ʣ��ʱ��
		unsigned long long lastTime = 1000;
		if (!firstStart)
		{
			lastTime = 86400000;
		}
		else
		{
			lastTime = System::GetNextDayNodeMs();
		}
		HandleSysChange(Permission);
		//���ڶ���ˢ��
		/*DateView* pView = static_cast<DateView*>(pView_);
		DateModel* pModel = static_cast<DateModel*>(mode_.get());
		pView_->hide();
		pView_->redraw();
		if (pModel)
		{
			if(pModel->DateConfig.Perm.HasShowPerm)
				pView_->show();
		}*/
		//�ӳ�һ��ˢ��ȷ���������һ���ˢ��CD��û�ܽ�����һ��
		Page()->AddTimeout(++lastTime, UpdateDate, (void*)this);
	}

	void DateControl::StopTick()
	{
		DateView* pView = static_cast<DateView*>(pView_);
		Page()->RemoveTimeout(UpdateDate, (void*)pView);
	}

	bool DateControl::HandleSysChange(SysChangeEM catogray)
	{
		DateModel* mode_ = static_cast<DateModel*>(pModel_.get());
		switch (catogray)
		{
		case UI::Permission:
		{
			/*�ж��Ƿ���Ȩ��*/
			if (!UI::PermUtility::HasUserPerm(mode_->DateConfig.Perm.RequireRegion))	//û��Ȩ�޲�������
			{
				if (mode_->DateConfig.Perm.IsHideElement)
				{
					pView_->hide();
					pView_->redraw();
					mode_->DateConfig.Perm.HasLimitShowPerm = false;
				}
			}
			else
			{
				/*pView_->damage();
				pView_->redraw();*/
				mode_->DateConfig.Perm.HasLimitShowPerm = true;
				UI::ViewShowUtility::ShowView(pView_, mode_->DateConfig.Perm, mode_->DateConfig.X + mode_->DateConfig.OffX, mode_->DateConfig.Y + mode_->DateConfig.OffY);
			}
		}
		case UI::Language:
			break;
		default:
			break;
		}
		return true;
	}

	void DateControl::OnReady()
	{
		HandleSysChange(Permission);
		StartTick(true);
	}


	int DateControl::PeekHMIMessage(Message::Msg* msg)
	{
		Project::DataVarId varId = Project::DataVarId(msg->LParam);
		DateModel* pMode = static_cast<DateModel*>(pModel_.get());
		DateView* pView =  static_cast<DateView*>(pView_);

		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			if (mode_->DateConfig.VOffX.Cmp(varId))
			{
				mode_->DateConfig.OffX = UI::UIData::Number<int>(mode_->DateConfig.VOffX) + Page()->GetPageOffX();
				UI::ViewShowUtility::ShowView(pView_, mode_->DateConfig.Perm,
					mode_->DateConfig.X + mode_->DateConfig.OffX,
					mode_->DateConfig.Y + mode_->DateConfig.OffY);
			}
			if (mode_->DateConfig.VOffY.Cmp(varId))
			{
				mode_->DateConfig.OffY = UI::UIData::Number<int>(mode_->DateConfig.VOffY) + Page()->GetPageOffY();
				UI::ViewShowUtility::ShowView(pView_, mode_->DateConfig.Perm,
					mode_->DateConfig.X + mode_->DateConfig.OffX,
					mode_->DateConfig.Y + mode_->DateConfig.OffY);
			}
			if (mode_->DateConfig.Perm.ShowVID.Cmp(varId))
				UI::PermUtility::HandleShowPerm(mode_->DateConfig.Perm, pView_);
			/*pMode->m_DateConfig.X += 10;
			pMode->m_DateConfig.Y += 10;
			pView_->position(pMode->m_DateConfig.X, pMode->m_DateConfig.Y);
			pView_->damage();
			pView_->redraw();*/
			break;
		default:
			break;
		}
		return 1;//handled;
	}
}

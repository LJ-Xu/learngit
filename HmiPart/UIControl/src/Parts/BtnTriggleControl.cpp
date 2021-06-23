#include "stdafx.h"
#include "BtnTriggleControl.h"
#include "BtnTriggleView.h"
#include "BtnTriggleModel.h"
#include "HMIWindow.h"
#include "Message.h"
namespace UI
{

	static shared_ptr< BtnTriggleModel> Mode;
	static shared_ptr<BtnTriggleView > View;
	BtnTriggleControl::BtnTriggleControl(HMIPage* w) :BaseControl(w)
	{
		//创建BtnTriggleModel
		Mode = std::shared_ptr < BtnTriggleModel>(new BtnTriggleModel());
		InitMVCModel(Mode);
	}
	void BtnTriggleControl::CreateView()
	{
		//初始化VIEW
		BtnTriggleView* view = new BtnTriggleView(Mode->m_config.X, Mode->m_config.Y,
			Mode->m_config.Width, Mode->m_config.Height, Mode->m_config.text.c_str());
		InitMVCView(view);
	}

	void BtnTriggleControl::SetCallback(void(*cb)(HMIBaseView *, void *), void *data)
	{
		BtnTriggleModel* pMode = static_cast<BtnTriggleModel*>(pModel_.get());
		BtnTriggleView* pView = static_cast<BtnTriggleView*>(pView_);
		pView->callback(cb, data);
	}
	void BtnTriggleControl::AddVarData(vector<Project::DataVarId*> vars)
	{
	}

	int BtnTriggleControl::PeekHMIMessage(Message::Msg* msg)
	{
		BtnTriggleModel* pMode = static_cast<BtnTriggleModel*>(pModel_.get());
		BtnTriggleView* pView = static_cast<BtnTriggleView*>(pView_);

		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			pMode->m_config.X += 10;
			pMode->m_config.Y += 10;
			pView_->position(pMode->m_config.X, pMode->m_config.Y);
			pView_->damage();
			pView_->redraw();
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
		default:
			break;
		}
		return 1;//handled;
	}
}
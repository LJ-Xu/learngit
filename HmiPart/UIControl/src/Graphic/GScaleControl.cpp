#include "stdafx.h"
#include "HMIWindow.h"
#include "GScaleControl.h"
#include "GScaleView.h"
#include "Message.h"

namespace UI
{
	static shared_ptr<GScaleModel> Mode;
	GScaleControl::GScaleControl(HMIPage* w) :BaseControl(w)
	{
		//创建GScaleModel
		Mode = std::shared_ptr < GScaleModel>();
		InitMVCModel(Mode);
	}

	void GScaleControl::CreateView()
	{
		//初始化VIEW
		GScaleView*  view = new GScaleView(Mode->m_ScaleConfig.X, Mode->m_ScaleConfig.Y,
			Mode->m_ScaleConfig.Width, Mode->m_ScaleConfig.Height);
		InitMVCView(view);
	}


	int GScaleControl::PeekHMIMessage(Message::Msg* msg)
	{
		GScaleModel* pMode = static_cast<GScaleModel*>(pModel_.get());
		GScaleView* pView = static_cast<GScaleView*>(pView_);

		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			pMode->m_ScaleConfig.X += 10;
			pMode->m_ScaleConfig.Y += 10;
			pView_->position(pMode->m_ScaleConfig.X, pMode->m_ScaleConfig.Y);
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


	//获取数据 
	void GScaleControl::AddVarData(vector<Project::DataVarId*> vars)
	{

	}
}

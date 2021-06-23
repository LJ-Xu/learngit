#include "stdafx.h"
#include "GLabelControl.h"
#include "HMIWindow.h"
#include "Message.h"
namespace UI
{

	static    shared_ptr< GLabelModel> Mode;
	GLabelControl::GLabelControl(HMIPage* w) :BaseControl(w)
	{
		//创建GLabelModel
		Mode = std::shared_ptr < GLabelModel>();
		InitMVCModel(Mode);
	}

	void GLabelControl::CreateView()
	{
		//初始化VIEW
		GLabelView* view  = new GLabelView(Mode->m_config.X, Mode->m_config.Y,
			Mode->m_config.Width, Mode->m_config.Height, Mode->m_config.text.c_str());
		InitMVCView(view);
	}

	void GLabelControl::AddVarData(vector<Project::DataVarId*> vars)
	{

	}

	int GLabelControl::PeekHMIMessage(Message::Msg* msg)
	{
		GLabelModel* pMode = static_cast<GLabelModel*>(pModel_.get());
		GLabelView* pView = static_cast<GLabelView*>(pView_);

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
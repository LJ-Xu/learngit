#include "stdafx.h"
#include "KeyBoardControl.h"
#include "KeyBoardView.h"
#include "KeyBoardModel.h"
#include "UIComm.h"
#include "Message.h"
#include "HMIWindow.h"
namespace UI
{
	static shared_ptr<KeyBoardModel> pMode;
	KeyBoardControl::KeyBoardControl(HMIPage* w) : BaseControl(w)
	{
		//创建KeyBoardModel
		pMode = std::shared_ptr< KeyBoardModel>(new KeyBoardModel());
		InitMVCModel(pMode);
	}

	void KeyBoardControl::CreateView()
	{
		//初始化VIEW
		KeyBoardView* pView = new KeyBoardView(pMode->m_KeyBoardConfig.X, pMode->m_KeyBoardConfig.Y, "");
		//	pView->show();
		InitMVCView(move(pView));
	}

	void KeyBoardControl::AddVarData(vector<Project::DataVarId*> vars)
	{
		vars.push_back(&pMode->m_KeyBoardConfig.VOffX);
		vars.push_back(&pMode->m_KeyBoardConfig.VOffY);
		vars.push_back(&pMode->m_KeyBoardConfig.Perm.Permission.PermData);

	}
	void KeyBoardControl::SendMsg(DigitalInputControl *ctrl, int message, int lparam)
	{
		Win()->SendUserMessage((void*)ctrl->CtrlId(), message, lparam);
	}
	int KeyBoardControl::PeekHMIMessage(Message::Msg* msg)
	{
		//KeyBoardView* pView = new KeyBoardView(pMode->m_KeyBoardConfig.x, pMode->m_KeyBoardConfig.y, "");
		//InitMVC(pMode, pView);
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			pMode->m_KeyBoardConfig.X += 10;
			pMode->m_KeyBoardConfig.Y += 10;
			pView_->position(pMode->m_KeyBoardConfig.X, pMode->m_KeyBoardConfig.Y);
			pView_->damage();
			pView_->redraw();
			break;
		case WM_EVENT_TIMEUP:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
			break;
		case WM_EVENT_READY:
			Win()->SendUserMessage((void*)this->CtrlId(), WM_EVENT_TIMETICK, 1000);//1s
		case WM_EVENT_OPEN:
			//KeyBoardView* pView = new KeyBoardView(pMode->m_KeyBoardConfig.x, pMode->m_KeyBoardConfig.y, "");
			pMode->m_KeyBoardConfig.X += 20;
			pMode->m_KeyBoardConfig.Y += 20;
			pView_->position(pMode->m_KeyBoardConfig.X, pMode->m_KeyBoardConfig.Y);
			/*获取参数*/
			pMode->m_KeyBoardConfig.InputNum = (int)msg->LParam;
			printf("InputNum = %d\n", pMode->m_KeyBoardConfig.InputNum);
			pView_->show();
		case WM_EVENT_CLOSE:
			//pView_->exit(0);
			//exit(0);
			//pView_->hide();
		default:
			break;
		}
		return 1;//handled;
	}
}

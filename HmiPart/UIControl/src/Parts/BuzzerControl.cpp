#include "stdafx.h"
#include "HMIWindow.h"
#include "BuzzerControl.h"
#include "BuzzerView.h"
#include "Message.h"
#include "SysCtrlApi.h"
#include "HMIPage.h"
#include "UIData.h"
//响铃间隔，单位毫秒
#define BEEPINTERVAL 1000
namespace UI
{
	void Beep_func(void *data)
	{
		SysCtrlApi::Beep(1);
	}

	static shared_ptr<BuzzerModel> Mode;
	BuzzerControl::BuzzerControl(HMIPage* w) :BaseControl(w), IsRepeatBeeping(false)
	{
		//创建BuzzerModel
		Mode = std::shared_ptr < BuzzerModel>(new BuzzerModel());
		InitMVCModel(Mode);
	}

	void BuzzerControl::CreateView()
	{
		//初始化VIEW
		BuzzerView* view = new BuzzerView(Mode->BuzzerConfig.X, Mode->BuzzerConfig.Y,
			Mode->BuzzerConfig.Width, Mode->BuzzerConfig.Height);

		InitMVCView(view);

	}

	int BuzzerControl::PeekHMIMessage(Message::Msg* msg)
	{
		BuzzerModel* pMode = static_cast<BuzzerModel*>(pModel_.get());
		BuzzerView* pView = static_cast<BuzzerView*>(pView_);
		switch (msg->Code)
		{
		case WM_EVENT_DATEUPDATE:
			//pMode->m_BuzzerConfig.X += 10;
			//pMode->m_BuzzerConfig.Y += 10;
			//pView_->position(pMode->m_BuzzerConfig.X, pMode->m_BuzzerConfig.Y);
			this->Ring(pMode);
			/*pView_->damage();
			pView_->redraw();*/
			break;
		default:
			break;
		}
		return 1;//handled;
	}

	void BuzzerControl::Ring(BuzzerModel* model)
	{
		bool beep = false;
		int currentValue = UIData::Number<int>(model->BuzzerConfig.TrigVar);
		switch (model->BuzzerConfig.Relation2Value)
		{
		case 0://<
			if (currentValue < model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		case 1://>
			if (currentValue > model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		case 2://
			if (currentValue <= model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		case 3://>=
			if (currentValue >= model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		case 4://==
			if (currentValue < model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		case 5://!=
			if (currentValue < model->BuzzerConfig.RelaValue)
				beep = true;
			break;
		}
		if (beep)
		{
			if (model->BuzzerConfig.Mode == 0)
			{
				SysCtrlApi::Beep(1);
			}
			else
			{

				if (this->IsRepeatBeeping != true)
				{
					Page()->AddTimeout(BEEPINTERVAL, Beep_func, NULL, true);
					this->IsRepeatBeeping = true;
				}
			}
		}
		else
		{
			if (this->IsRepeatBeeping)
			{
				Page()->RemoveTimeout(Beep_func, NULL);
				IsRepeatBeeping = false;
			}
		}
	}
}


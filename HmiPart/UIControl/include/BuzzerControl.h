#pragma once
#include "BaseControl.h"
#include "BuzzerModel.h"
namespace UI
{
	class BuzzerControl :public BaseControl
	{
	public:
		BuzzerControl() :BuzzerControl(nullptr){}
		BuzzerControl(HMIPage*);
		void CreateView()override;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		void Ring(BuzzerModel* model);
	private:
		bool IsRepeatBeeping;
	};
}



#pragma once
#include "BaseControl.h"
#include "KeyBoardView.h"
namespace UI
{
	class KeyBoardControl : public BaseControl
	{
	public:
		KeyBoardControl() :KeyBoardControl(nullptr) {}
		KeyBoardControl(HMIPage*  );
		void CreateView()  override;
		void SendMsg(DigitalInputControl *ctrl, int message, int lparam);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	};
}




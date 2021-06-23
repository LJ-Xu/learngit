#pragma once
#include "BaseControl.h"
#include "GTestModel.h"


namespace UI
{
	class GTestControl :public BaseControl
	{
	public:
		GTestControl() :GTestControl(nullptr) {}
		GTestControl(HMIPage*  );
		void CreateView()override;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	};
}
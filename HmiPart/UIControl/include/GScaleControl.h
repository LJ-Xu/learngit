#pragma once
#include "BaseControl.h"
#include "GScaleModel.h"
namespace UI
{
	class GScaleView;
	class GScaleControl :public BaseControl
	{
	public:
		GScaleControl() :GScaleControl(nullptr) {}
		GScaleControl(HMIPage*  );
		void CreateView()override;
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	};

}
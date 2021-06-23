#pragma once
#include <stdafx.h>
#include "BtnTriggleModel.h"
#include "BaseControl.h"
namespace UI
{
	class BtnTriggleView;
	class BtnTriggleControl : public BaseControl
	{
	public:
		BtnTriggleControl() :BtnTriggleControl(nullptr) {}
		BtnTriggleControl(HMIPage*  );
		void CreateView()override;
		void SetCallback(void(*cb)(HMIBaseView *, void *), void *data);
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	};

}

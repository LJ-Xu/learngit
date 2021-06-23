#pragma once
#include "BaseControl.h"
#include "GLabelModel.h"
#include "GLabelView.h"

namespace UI
{
	class GLabelView;
	class GLabelControl :public BaseControl
	{
	public:
		GLabelControl() :GLabelControl(nullptr) {}
		GLabelControl(HMIPage*  );
		void CreateView()override;
	protected:

		int PeekHMIMessage(Message::Msg* msg) override;
	};

}
#pragma once
#include "IHmiThread.h"
namespace UI
{
	class UIManager;
	class UIThread :
		public IHMIThread
	{
	public:
		UIThread(UIManager* mg);
		virtual void Run() override;
		virtual void Exit() override;
	private :
		UIManager* mg_;
	};

}


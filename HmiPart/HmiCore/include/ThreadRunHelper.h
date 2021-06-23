#pragma once
#include "stdafx.h"
#include "UIManager.h"
#include "HMIProject.h"
#include "IHmiThread.h"
namespace Core
{
	class ThreadRunHelper
	{
	public:
		ThreadRunHelper();
		~ThreadRunHelper();
		void AddThread(std::unique_ptr<IHMIThread> td);
		void Run();
		void Exit();
	private:
		std::vector<std::unique_ptr<IHMIThread>> items_;
		std::vector<thread> tdpool_;
	};
}

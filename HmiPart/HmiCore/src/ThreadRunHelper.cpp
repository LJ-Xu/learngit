#include "ThreadRunHelper.h"
namespace Core
{
	ThreadRunHelper::ThreadRunHelper()
	{
	
	}
	ThreadRunHelper::~ThreadRunHelper()
	{
	
	}
	void ThreadRunHelper::AddThread(std::unique_ptr<IHMIThread> td)
	{
		items_.push_back(move(td));
	}
	static void RunFunc(void *ptr)
	{
		IHMIThread* td = (IHMIThread*)ptr;
		td->Run();
	}
	void ThreadRunHelper::Run()
	{
		for (size_t i = 0; i < items_.size(); i++)
		{
			tdpool_.push_back(std::thread(RunFunc, items_[i].get()));
		}
	}
	void ThreadRunHelper::Exit()
	{
		for (size_t i = 0; i < items_.size(); i++)
		{
			items_[i]->Exit();
		}
		for (size_t i = 0; i < tdpool_.size(); i++)
		{
			if(tdpool_[i].joinable())
				tdpool_[i].join();
		}
	}
}

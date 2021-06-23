#include "UIThread.h"
#include "UIManager.h"
namespace UI
{
	UIThread::UIThread(UIManager* mg)
	{
		mg_ = mg;
	}
	void UIThread::Run()
	{
		mg_->RunUI();
	}
	void UIThread::Exit()
	{

	}
}


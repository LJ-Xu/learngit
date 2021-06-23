#include "ScreenSaver.h"
#include "System.h"
namespace UI
{
	SysSetGControl *ScreenSaver::sysctrl_ = nullptr;
	void ScreenSaver::SetIns(SysSetGControl * ctrl)
	{
		sysctrl_ = ctrl;
	}
	bool ScreenSaver::CheckScreenSaver()
	{
		if (!sysctrl_)
			return false;
		unsigned long long currenttime = System::GetCurrentTimeStampMs();
		if (sysctrl_->AchieveScreenSaverTime())	//达到屏保时间
		{
			sysctrl_->CloseScreenSaver();			//退出屏保
			return true;
		}
		return false;
	}
}
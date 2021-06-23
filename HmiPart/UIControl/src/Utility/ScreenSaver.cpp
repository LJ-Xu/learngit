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
		if (sysctrl_->AchieveScreenSaverTime())	//�ﵽ����ʱ��
		{
			sysctrl_->CloseScreenSaver();			//�˳�����
			return true;
		}
		return false;
	}
}
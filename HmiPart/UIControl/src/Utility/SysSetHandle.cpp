#include "SysSetHandle.h"
#include "System.h"
namespace UI
{
	SysSetGControl *SysSetHandle::sysctrl_ = nullptr;
	void SysSetHandle::SetIns(SysSetGControl * ctrl)
	{
		sysctrl_ = ctrl;
	}
	bool SysSetHandle::CheckScreenSaver()
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
	void SysSetHandle::ReportCurrentWinno(int winno)
	{
		sysctrl_->ReportWinno(winno);
	}
}
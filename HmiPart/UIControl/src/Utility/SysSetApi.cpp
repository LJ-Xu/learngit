#include "SysSetApi.h"
namespace UI
{
	void SysSetApi::TriggerBeep()
	{
		if (!LocalData::GetBit(SYS_PSB_BEEP_DISABLE))
#ifdef WIN32
			SysCtrlApi::Beep();
#else
			SysCtrlApi::Beep();
#endif // WIN32
	}
}
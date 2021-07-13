#pragma once
#include "SysSetGControl.h"
namespace UI
{
	class SysSetHandle
	{
	public:
		static void SetIns(SysSetGControl * ctrl);
		static bool CheckScreenSaver();								//É¾³ýÍ¼Æ¬
		static void ReportCurrentWinno(int winno);
	private:
		static SysSetGControl *sysctrl_;
	};
}




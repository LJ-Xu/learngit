#pragma once
#include "SysSetGControl.h"
namespace UI
{
	class ScreenSaver
	{
	public:
		static void SetIns(SysSetGControl * ctrl);
		static bool CheckScreenSaver();								//É¾³ýÍ¼Æ¬
	private:
		static SysSetGControl *sysctrl_;
	};
}




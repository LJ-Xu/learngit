#pragma once
#include "SysSetGControl.h"
namespace UI
{
	class ScreenSaver
	{
	public:
		static void SetIns(SysSetGControl * ctrl);
		static bool CheckScreenSaver();								//ɾ��ͼƬ
	private:
		static SysSetGControl *sysctrl_;
	};
}




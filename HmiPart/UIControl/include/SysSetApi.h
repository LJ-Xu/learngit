#pragma once
#ifdef WIN32
#include <string>
#include "SysCtrlApi.h"
#include "UIData.h"
#else
#include <string>
#include "UIData.h"
#include "SysCtrlApi.h"
#endif // WIN32
namespace UI
{
	class SysSetApi
	{
	public:
		static void TriggerBeep();			//´¥·¢·äÃùÆ÷
	};
}

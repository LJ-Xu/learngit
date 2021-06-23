#include "UIDataService.h"
#include "LocalData.h"

namespace UI
{
	UIDataService& UIDataService::Ins()
	{
		static UIDataService ins;
		return ins;
	}

}

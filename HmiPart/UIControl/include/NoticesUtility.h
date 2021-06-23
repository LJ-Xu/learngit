#pragma once
#include <string>
#include "NoticesAction.h"
namespace UI
{
	class IResourceService;
	class NoticesUtility
	{
	public:
		static void NoticeWrite(Project::NoticesAction& notice);
	};
}

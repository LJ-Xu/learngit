#pragma once
#include <string>
#include "StatusRes.h"
namespace UI
{
	class IResourceService;
	class StringUtility
	{
	public:
		static std::string GetDrawString(IResourceService* srv, Project::StatusRes& res, int status);
	};
}


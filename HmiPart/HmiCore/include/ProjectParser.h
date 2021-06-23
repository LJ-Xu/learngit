#pragma once
#include "stdafx.h"
#include "DeleteHelper.h"
#include "UIManager.h"
#include "HMIProject.h"
#include "RunEnv.h"
namespace Core
{
	class ProjectParser
	{
	public:
		ProjectParser(const char* path);
		bool CheckCrc();
		int Parse(Project::HMIProject* prj);
	private:
		const char* path_;
	};
}

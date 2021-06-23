#include "ProjectParser.h"
namespace Core
{
	ProjectParser::ProjectParser(const char* path)
	{
		path_ = path;
	}
	bool ProjectParser::CheckCrc()
	{
		return true;
	}
	int ProjectParser::Parse(Project::HMIProject* prj)
	{
		
		prj->LoadBinFile(path_);//加载工程数据
		return 0;
	}

}

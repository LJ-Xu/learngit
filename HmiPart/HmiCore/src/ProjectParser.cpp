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
		
		prj->LoadBinFile(path_);//���ع�������
		return 0;
	}

}

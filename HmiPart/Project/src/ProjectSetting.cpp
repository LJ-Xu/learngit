#include "ProjectSetting.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace Project
{
	void ProjectSetting::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("DirScreen"))
				Dir = json["DirScreen"].GetInt();
			if (json.HasMember("ScreenWidth"))
				Width = json["ScreenWidth"].GetInt();
			if (json.HasMember("ScreenHeight"))
				Height = json["ScreenHeight"].GetInt();
			if (json.HasMember("HmiStartPageNo"))
				StartWinNo = json["HmiStartPageNo"].GetInt();
			if (json.HasMember("HmiCategory") && !json["HmiCategory"].IsNull())
				Category = json["HmiCategory"].GetString();
			if (json.HasMember("HmiName") && !json["HmiName"].IsNull())
				HmiName = json["HmiName"].GetString();
		}
	}
}
 
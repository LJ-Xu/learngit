#include "ProjectInfo.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>
#include <iostream>
using namespace rapidjson;
namespace Project
{
	void ProjectInfo::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/ProjectInfo.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("FmtVer"))
				FmtVer = json["FmtVer"].GetInt();
			if (json.HasMember("PCVer"))
				PCVer = json["PCVer"].GetInt();
			if (json.HasMember("Author") && !json["Author"].IsNull())
				Author = json["Author"].GetString();
			if (json.HasMember("Name") && !json["Name"].IsNull())
				Name = json["Name"].GetString();
			if (json.HasMember("Commit") && !json["Commit"].IsNull())
				Commit = json["Commit"].GetString();
			if (json.HasMember("CTime") && !json["CTime"].IsNull())
				CTime = json["CTime"].GetString();
			if (json.HasMember("LastTime") && !json["LastTime"].IsNull())
				LastTime = json["LastTime"].GetString();
			if (json.HasMember("CompileTime") && !json["CompileTime"].IsNull())
				CompileTime = json["CompileTime"].GetString();
			if (json.HasMember("Screenmodel") && !json["Screenmodel"].IsNull())
				Screenmodel = json["Screenmodel"].GetString();
			if (json.HasMember("DrawBoardSize") && json["DrawBoardSize"].IsObject() && !json["DrawBoardSize"].IsNull())
			{
				if (json["DrawBoardSize"].HasMember("W"))
					DrawBoardSize.W = json["DrawBoardSize"]["W"].GetInt();
				if (json["DrawBoardSize"].HasMember("H"))
					DrawBoardSize.H = json["DrawBoardSize"]["H"].GetInt();
			}
		}
	}
}
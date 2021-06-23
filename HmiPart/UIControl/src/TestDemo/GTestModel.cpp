#include "stdafx.h"
#include "GTestModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>

using namespace rapidjson;
using namespace std;
namespace UI
{
	
	void GTestModel::InitTestData()
	{
		Config.CtrlName = "GTestControl";
		Config.Height = 100;
		Config.Width = 100;
		Config.X = 100;
		Config.Y = 100;
		Config.ID = 526;
 
	}
	void GTestModel::InitData(std::string jstr)
	{
		Document json;
		/*FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/CircleJson.json", "r");
		char buf[0XFFFF];
		rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		json.ParseStream(input);*/
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("x"))
				Config.X = json["x"].GetInt();
			if (json.HasMember("y"))
				Config.Y = json["y"].GetInt();
			if (json.HasMember("width"))
				Config.Width = json["width"].GetInt();
			if (json.HasMember("height"))
				Config.Height = json["height"].GetInt();
			if (json.HasMember("isEnable"))
				Config.IsEnable = json["isEnable"].GetBool();


		}

	}
}
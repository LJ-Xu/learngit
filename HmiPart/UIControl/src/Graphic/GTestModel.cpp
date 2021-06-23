#include "stdafx.h"
#include "GTestModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>

using namespace rapidjson;
using namespace std;
namespace UI
{
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
				m_testConfig.X = json["x"].GetInt();
			if (json.HasMember("y"))
				m_testConfig.Y = json["y"].GetInt();
			if (json.HasMember("width"))
				m_testConfig.Width = json["width"].GetInt();
			if (json.HasMember("height"))
				m_testConfig.Height = json["height"].GetInt();
			if (json.HasMember("isEnable"))
				m_testConfig.IsEnable = json["isEnable"].GetBool();

			if (json.HasMember("hash"))
				m_testConfig.hash = string(json["hash"].GetString());
			if (json.HasMember("label"))
				m_testConfig.label = string(json["label"].GetString());
			if (json.HasMember("tblId"))
				m_testConfig.tblId = json["tblId"].GetInt();
			if (json.HasMember("strId"))
				m_testConfig.strId = json["strId"].GetInt();
		}

	}
}
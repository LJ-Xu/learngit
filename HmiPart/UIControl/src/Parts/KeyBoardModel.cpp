#include "stdafx.h"
#include "KeyBoardModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void KeyBoardModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/2.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		json.Parse(jstr.c_str());
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			if (json.HasMember("x")) {
				Value& value = json["x"];
				m_KeyBoardConfig.X = value.GetInt();
			}
			if (json.HasMember("y")) {
				Value& value = json["y"];
				m_KeyBoardConfig.Y = value.GetInt();
			}
			if (json.HasMember("width")) {
				Value& value = json["width"];
				m_KeyBoardConfig.Width = value.GetInt();
			}
			if (json.HasMember("height")) {
				Value& value = json["height"];
				m_KeyBoardConfig.Height = value.GetInt();
			}

		}
	}
}
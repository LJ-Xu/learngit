#include "stdafx.h"
#include "DateModel.h"
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void DateModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());


		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		DateConfig.InitBaseData(json);

		if (json.HasMember("DateFormat"))
			DateConfig.DateFormat = json["DateFormat"].GetInt();
		if (json.HasMember("ShowFullYear"))
			DateConfig.ShowFullYear = json["ShowFullYear"].GetBool();
		if (json.HasMember("ShowWeek"))
			DateConfig.ShowWeek = json["ShowWeek"].GetBool();

		if (json.HasMember("BackUpImg"))
			DateConfig.BackUpImg.Parse(json["BackUpImg"]);

		if (json.HasMember("DateFontStyle") && json["DateFontStyle"].IsObject())
			DateConfig.DateFontStyle.Parse(json["DateFontStyle"]);


	}

	void DateModel::InitTestData()
	{
		DateConfig.Height = 20;
		DateConfig.Width = 100;
		DateConfig.X = 10;
		DateConfig.Y = 10;

		DateConfig.DateFormat = 0;
		DateConfig.ShowFullYear = true;
		DateConfig.ShowWeek = true;
	}
}
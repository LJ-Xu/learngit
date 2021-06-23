#include "stdafx.h"
#include "ClockModel.h"
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void ClockModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		ClockConfig.InitBaseData(json);

		if (json.HasMember("TimeFormat"))
			ClockConfig.TimeFormat = json["TimeFormat"].GetInt();
		if (json.HasMember("HourType"))
			ClockConfig.HourType = json["HourType"].GetInt();
		if (json.HasMember("ClockFontStyle") && json["ClockFontStyle"].IsObject())
			ClockConfig.ClockFontStyle.Parse(json["ClockFontStyle"]);

		if (json.HasMember("BackUpImg"))
			ClockConfig.BackUpImg.Parse(json["BackUpImg"]);
	}

	void ClockModel::InitTestData()
	{
		ClockConfig.Height = 20;
		ClockConfig.Width = 100;
		ClockConfig.X = 10;
		ClockConfig.Y = 10;


		ClockConfig.TimeFormat = 0;
		ClockConfig.HourType = 0;
	}
}

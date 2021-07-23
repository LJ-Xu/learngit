#include "stdafx.h"
#include "AlarmBarModel.h"
#include <FL/Enumerations.H>

#include "JsonComm.h"
using namespace rapidjson; 
namespace UI
{
	void AlarmBarModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		
		AlarmBarConfig.InitBaseData(json);

		if (json.HasMember("MoveSpeed"))
			AlarmBarConfig.MoveSpeed = json["MoveSpeed"].GetInt();
		if (json.HasMember("GroupScopeFrom"))
			AlarmBarConfig.GroupScopeFrom = json["GroupScopeFrom"].GetInt();
		if (json.HasMember("GroupScopeTo"))
			AlarmBarConfig.GroupScopeTo = json["GroupScopeTo"].GetInt();
		if (json.HasMember("IsUseMultiLanguage"))
			AlarmBarConfig.IsUseMultiLanguage = json["IsUseMultiLanguage"].GetBool();
		if (json.HasMember("MultiLanConfig") && json["MultiLanConfig"].IsArray())
		{
			for (size_t i = 0; i < json["MultiLanConfig"].Size(); i++)
			{
				Project::AlarmBarMultiLan pl;
				pl.Parse(json["MultiLanConfig"][i]);
				AlarmBarConfig.MultiLanConfig.push_back(pl);
			}
		}

		if (json.HasMember("DateFormat"))
			AlarmBarConfig.DateFormat = json["DateFormat"].GetInt();
		if (json.HasMember("TimeFormat"))
			AlarmBarConfig.TimeFormat = json["TimeFormat"].GetInt();
		if (json.HasMember("AlarmBarFont") && json["AlarmBarFont"].IsObject())
		{
			AlarmBarConfig.AlarmBarFont.Parse(json["AlarmBarFont"]);
		}
		if (json.HasMember("IsBorder"))
			AlarmBarConfig.IsBorder = json["IsBorder"].GetBool();
		if (json.HasMember("BorderColor"))
			AlarmBarConfig.BorderColor = json["BorderColor"].GetInt();
		if (json.HasMember("IsFill"))
			AlarmBarConfig.IsFill = json["IsFill"].GetBool();
		if (json.HasMember("FillColor"))
			AlarmBarConfig.FillColor = json["FillColor"].GetInt();
		if (json.HasMember("Transparency"))
			AlarmBarConfig.Transparency = json["Transparency"].GetFloat();
		if (json.HasMember("SearchCtrlVarId") && json["SearchCtrlVarId"].IsObject())
			AlarmBarConfig.AlarmNoticeVarId.Parse(json["SearchCtrlVarId"]);
	}

	void AlarmBarModel::InitTestData()
	{

		AlarmBarConfig.X = 30;
		AlarmBarConfig.Y = 30;
		AlarmBarConfig.Width = 50;
		AlarmBarConfig.Height = 30;
		AlarmBarConfig.IsEnable = false;
		AlarmBarConfig.CtrlName = "AlarmBar0";


		AlarmBarConfig.DateFormat = 0;
		AlarmBarConfig.TimeFormat = 0;

		//字体
		AlarmBarConfig.AlarmBarFont.Colors = 0;
		AlarmBarConfig.AlarmBarFont.Font.Name = "Arial";
		AlarmBarConfig.AlarmBarFont.Font.Size = 12;
		AlarmBarConfig.AlarmBarFont.Alpha = 255;
		AlarmBarConfig.AlarmBarFont.Align = 0;


		AlarmBarConfig.IsBorder = true;
		AlarmBarConfig.BorderColor = 0;
		AlarmBarConfig.IsFill = true;
		AlarmBarConfig.FillColor = 32563;

	}
}
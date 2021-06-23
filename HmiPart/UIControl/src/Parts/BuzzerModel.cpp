#include "stdafx.h"
#include "BuzzerModel.h"
#include <FL/Enumerations.H>

#include "JsonComm.h"
using namespace rapidjson;

namespace UI
{
	void BuzzerModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			if (json.HasMember("X"))
				BuzzerConfig.X = json["X"].GetInt();
			if (json.HasMember("Y"))
				BuzzerConfig.Y = json["Y"].GetInt();
			if (json.HasMember("Width"))
				BuzzerConfig.Width = json["Width"].GetInt();
			if (json.HasMember("Height"))
				BuzzerConfig.Height = json["Height"].GetInt();
		}

		if (json.HasMember("IsEnable"))
			BuzzerConfig.IsEnable = json["IsEnable"].GetBool();

		if (json.HasMember("Reg"))
			BuzzerConfig.Reg = json["Reg"].GetInt();

		if (json.HasMember("TrigVar") && json["TrigVar"].IsObject())
		{
			if(json["TrigVar"].HasMember("VarIdRef")&&json["TrigVar"]["VarIdRef"].IsObject())
				BuzzerConfig.TrigVar.Parse(json["TrigVar"]["VarIdRef"]);
			if (json["TrigVar"].HasMember("relation2Value"))
				BuzzerConfig.Relation2Value = json["TrigVar"]["relation2Value"].GetInt();
			if (json["TrigVar"].HasMember("value"))
				BuzzerConfig.RelaValue = (int)json["TrigVar"]["value"].GetFloat();
		}

		if (json.HasMember("Mode"))
			BuzzerConfig.Mode = json["Mode"].GetInt();
	}


	void BuzzerModel::InitTestData()
	{
		BuzzerConfig.X = 30;
		BuzzerConfig.Y = 30;
		BuzzerConfig.Width = 150;
		BuzzerConfig.Height = 150;
		BuzzerConfig.IsEnable = false;
		BuzzerConfig.CtrlName = "BUZZER0";

		BuzzerConfig.Reg = 0;
		BuzzerConfig.Mode = 0;
	}

}



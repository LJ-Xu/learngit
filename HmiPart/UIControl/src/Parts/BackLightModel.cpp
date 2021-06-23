/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightModel.cpp
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : 关于背景灯参数内容的源码，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "BackLightModel.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void BackLightModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/TableJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);

		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("BackLightModel Parse Json\n");
			BackLightConfig.InitBaseData(json);

			if (json.HasMember("Reg"))
				BackLightConfig.Reg = json["Reg"].GetInt();
			if (json.HasMember("TrigVar") && json["TrigVar"].IsObject())
			{
				if (json["TrigVar"].HasMember("value"))
					BackLightConfig.ComPareValue = json["TrigVar"]["value"].GetDouble();
				if (json["TrigVar"].HasMember("relation2Value"))
					BackLightConfig.ValueTrigCond = (Project::Conditions)json["TrigVar"]["relation2Value"].GetInt();
				if (json["TrigVar"].HasMember("ActStat"))
					BackLightConfig.BitTriggerCond = json["TrigVar"]["ActStat"].GetInt();
				if (json["TrigVar"].HasMember("VarIdRef") && json["TrigVar"]["VarIdRef"].IsObject())
					BackLightConfig.TrigVar.Parse(json["TrigVar"]["VarIdRef"]);
			}
			if (json.HasMember("Act"))
				BackLightConfig.Act = json["Act"].GetInt();
		}
	}
}
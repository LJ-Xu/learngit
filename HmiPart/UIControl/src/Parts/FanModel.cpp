/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FanModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 关于动态图片绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "FanModel.h"
#include <FL/Fl.H>
using namespace rapidjson;

namespace UI
{
	void FanModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/DynamicPic.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("FanModel Parse Json\n");
			FanUnitConfig.InitBaseData(json);
			if (json.HasMember("Act"))
				FanUnitConfig.ActionMode = json["Act"].GetInt();
			if (json.HasMember("Reg"))
				FanUnitConfig.RegMode = json["Reg"].GetInt();
			if (json.HasMember("ActTrigVar") && json["ActTrigVar"].IsObject())
			{
				if (json["ActTrigVar"].HasMember("relation2Value"))
					FanUnitConfig.WordTrigCondition = (Project::Conditions)json["ActTrigVar"]["relation2Value"].GetInt();
				if(json["ActTrigVar"].HasMember("ActStat"))
					FanUnitConfig.BitTrigCondition = json["ActTrigVar"]["ActStat"].GetInt();
				if (json["ActTrigVar"].HasMember("value"))
					FanUnitConfig.WordTrigValue = json["ActTrigVar"]["value"].GetDouble();
				if (json["ActTrigVar"].HasMember("VarIdRef") && json["ActTrigVar"]["VarIdRef"].IsObject())
					FanUnitConfig.ActTrigVar.Parse(json["ActTrigVar"]["VarIdRef"]);
			}
			if (json.HasMember("Dir"))
				FanUnitConfig.RotateDir = json["Dir"].GetInt();
			if (json.HasMember("Speed"))
				FanUnitConfig.CycleTime = json["Speed"].GetDouble();
			if (json.HasMember("SpDataVarId") && json["SpDataVarId"].IsObject())
				FanUnitConfig.CycleTimeVarId.Parse(json["SpDataVarId"]);
			if (json.HasMember("Key") && json["Key"].IsArray())
				Project::ImageResId::Parse(FanUnitConfig.PicKeys, json["Key"]);
		}
	}
}

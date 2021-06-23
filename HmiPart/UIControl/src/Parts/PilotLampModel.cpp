/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PilotLampModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 解析json，关于指示灯绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "PilotLampModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
#include "UIComm.h"

using namespace rapidjson;

namespace UI
{
	void PilotLampModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/PilotLamp.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("PilotLampModel Parse Json\n");
			PLampconfig.InitBaseData(json);
			if (json.HasMember("StatusVar") && json["StatusVar"].IsObject())
				PLampconfig.Status.Parse(json["StatusVar"]);
			if (json.HasMember("Flash"))
				PLampconfig.Flash = json["Flash"].GetInt();
			if (json.HasMember("Logic"))
				PLampconfig.Logic = json["Logic"].GetInt();
			if (json.HasMember("Speed"))
				PLampconfig.Speed = json["Speed"].GetFloat();
			if (json.HasMember("StatusKeys") && json["StatusKeys"].IsArray())
				Project::ImageResId::Parse(PLampconfig.StatusKeys, json["StatusKeys"]);
			if (json.HasMember("Txt"))
				PLampconfig.Text.Parse(json["Txt"]);
			if (json.HasMember("Csses") && json["Csses"].IsArray())
				Project::StringStyle::Parse(PLampconfig.Csses, json["Csses"]);
		}
	}
}

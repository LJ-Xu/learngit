/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于键盘按键绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeyBtnModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
#include "UIComm.h"
using namespace rapidjson;
namespace UI
{
	void KeyBtnModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/KeyBtn.json", "r");
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
			LOG_INFO_("KeyBtnModel Parse Json\n");
			KeyBtnConfig.InitBaseData(json);

			if (json.HasMember("AsciiNum"))
				KeyBtnConfig.AsciiNum = json["AsciiNum"].GetInt();
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				KeyBtnConfig.Txt.Parse(json["Txt"]);
			if (json.HasMember("Csses") && json["Csses"].IsArray())
				Project::StringStyle::Parse(KeyBtnConfig.StrStyles, json["Csses"]);
			if (json.HasMember("Keys") && json["Keys"].IsArray())
				Project::ImageResId::Parse(KeyBtnConfig.PicKey, json["Keys"]);
		}
	}
}

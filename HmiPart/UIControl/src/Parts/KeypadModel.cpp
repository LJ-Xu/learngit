/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ����json�����ڰ������Ʋ������ݣ���������Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeypadModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "UIComm.h"
using namespace rapidjson;
namespace UI
{
	void KeypadModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/Keypad.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//�жϽ����Ƿ���ȷ
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("KeypadModel Parse Json\n");
			KeypadConfig.InitBaseData(json);
			if (json.HasMember("Action"))
				KeypadConfig.Action = (Project::BtnAction)json["Action"].GetInt();
			if (json.HasMember("WriteVar"))
				KeypadConfig.WriteVar.Parse(json["WriteVar"]);
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				KeypadConfig.Txt.Parse(json["Txt"]);
			if (json.HasMember("Css") && json["Css"].IsArray())
				Project::StringStyle::Parse(KeypadConfig.StrStyles, json["Css"]);
			if (json.HasMember("Keys") && json["Keys"].IsArray())
				Project::ImageResId::Parse(KeypadConfig.PicKey, json["Keys"]);
		}
	}
}

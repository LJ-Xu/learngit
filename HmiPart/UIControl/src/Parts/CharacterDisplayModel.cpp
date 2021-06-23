/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json关于字符显示框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterDisplayModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void CharacterDisplayModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/CharacterDisplayJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());

		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("CharacterDisplayModel Parse Json\n");
			DisStrConfig.InitBaseData(json);

			if (json.HasMember("ByteConversion"))
				DisStrConfig.ByteConversion = json["ByteConversion"].GetInt();
			if (json.HasMember("Rule"))
				DisStrConfig.Rule = (UI::CodeFormatUtility::CodeRule)json["Rule"].GetInt();
			if (json.HasMember("FmtStr") && !json["FmtStr"].IsNull())
				DisStrConfig.FmtStr = json["FmtStr"].GetString();
	
			if (json.HasMember("ReadVar"))
				DisStrConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("PswMode"))
				DisStrConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("Key") && !json["Key"].IsNull())
				DisStrConfig.Key.Parse(json["Key"]);

			if (json.HasMember("TextStyle") && json["TextStyle"].IsObject())
				DisStrConfig.TextStyle.Parse(json["TextStyle"]);
			
		}

	}
}
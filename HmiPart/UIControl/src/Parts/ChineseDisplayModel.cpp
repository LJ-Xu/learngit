/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseDisplayModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 解析json关于中文显示框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseDisplayModel.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "ResourceService.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void ChineseDisplayModel::InitData(std::string jstr)
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
			LOG_INFO_("ChineseDisplayModel Parse Json\n");
			DisChineseConfig.InitBaseData(json);

			if (json.HasMember("Rule"))
				DisChineseConfig.Rule = (UI::CodeFormatUtility::CodeRule)json["Rule"].GetInt();
			if (json.HasMember("PswMode"))
				DisChineseConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("Text") && !json["Text"].IsNull())
				DisChineseConfig.Text = json["Text"].GetString();
			if (json.HasMember("ReadVar"))
				DisChineseConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("Key") && !json["Key"].IsNull())
				DisChineseConfig.PicKey.Parse(json["Key"]);
			if (json.HasMember("TextStyle"))
				DisChineseConfig.TextStyle.Parse(json["TextStyle"]);
		}

	}
}

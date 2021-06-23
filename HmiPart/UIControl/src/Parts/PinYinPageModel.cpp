/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json关于拼音界面内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "PinYinPageModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void PinYinPageModel::InitPinYinData(int x, int y)
	{
			PinYinConfig.X = 0;
			PinYinConfig.Y = 0;
			PinYinConfig.Width = 180;
			PinYinConfig.Height = 80;
			PinYinConfig.TextStyle.Font.Name = "Arial";
			PinYinConfig.TextStyle.Font.Size = 14;
			PinYinConfig.TextStyle.Align = 0;
			PinYinConfig.TextStyle.Colors = 0xff0000ff;
	}

	void PinYinPageModel::InitData(std::string jstr)
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
			PinYinConfig.InitBaseData(json);
			if (json.HasMember("ReadVar"))
				PinYinConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("WriteVar"))
				PinYinConfig.WriteVar.Parse(json["ReadVar"]);
			if (json.HasMember("TextStyle") && json["TextStyle"].IsObject())
				PinYinConfig.TextStyle.Parse(json["TextStyle"]);
		}

	}
}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringModel.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于静态文字绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StaticStringModel.h"
#include "JsonComm.h"
#include <FL/fl_draw.H>

#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void StaticStringModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/StatticString.Json", "r");
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
			LOG_INFO_("StaticStringModel Parse Json\n");
			StringConfig.InitBaseData(json);

			if (json.HasMember("Txt"))
				StringConfig.Text.Parse(json["Txt"]);
			if (json.HasMember("Border"))
			{
				Value& border = json["Border"];
				if (border.IsObject())
				{
					if (border.HasMember("Size"))
						StringConfig.Border.BorderSize = border["Size"].GetInt();
					if (border.HasMember("Style"))
						StringConfig.Border.BorderStyle = border["Style"].GetInt();
					if (border.HasMember("Colors"))
						StringConfig.Border.BorderColor = border["Colors"].GetInt();
				}
			}
			if (json.HasMember("Css"))
				StringConfig.Css.Parse(json["Css"]);
		}
	}
}

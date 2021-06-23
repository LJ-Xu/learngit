/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于动态文字绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "VarStringModel.h"
#include "StringStyle.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void VarStringModel::InitData(std::string jstr)
	{
		Document json;
		/*FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/VarString.json", "r");
		char buf[0XFFFF];
		rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		json.ParseStream(input);*/
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("VarStringModel Parse Json\n");
			VarStringConfig.InitBaseData(json);
			if (json.HasMember("Count"))
				VarStringConfig.Count = json["Count"].GetInt();
			if (json.HasMember("SrcIdRef") && json["SrcIdRef"].IsObject())
				VarStringConfig.SrcIdRef.Parse(json["SrcIdRef"]);
			if (json.HasMember("Items") && json["Items"].IsArray())
			{
				Project::VarStringStyle tmpstyle;
				vector<Project::VarStringStyle>().swap(VarStringConfig.StatusStyle);
				for (size_t i = 0; i < json["Items"].Size(); i++)
				{
					if (json["Items"][i].HasMember("SrcId"))
						tmpstyle.SrcId = json["Items"][i]["SrcId"].GetDouble();
					if (json["Items"][i].HasMember("Txt") && json["Items"][i]["Txt"].IsObject())
						tmpstyle.Text.Parse(json["Items"][i]["Txt"]);
					if (json["Items"][i].HasMember("Css") && json["Items"][i]["Css"].IsObject())
						tmpstyle.Css.Parse(json["Items"][i]["Css"]);
					if (json["Items"][i].HasMember("Border") && json["Items"][i]["Border"].IsObject())
						tmpstyle.BorderStyle.Parse(json["Items"][i]["Border"]);
					VarStringConfig.StatusStyle.push_back(tmpstyle);
				}
			}
		}
	}
}
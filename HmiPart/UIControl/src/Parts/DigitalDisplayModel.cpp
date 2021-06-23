/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于数值显示框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DigitalDisplayModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void DigitalDisplayModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/DigitalDisplayJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("DigitalDisplayModel Parse Json\n");
			DisNumConfig.InitBaseData(json);

			if (json.HasMember("PswMode"))
				DisNumConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("IsFillZero"))
				DisNumConfig.IsFillZero = json["IsFillZero"].GetBool();
			if (json.HasMember("Accuracy"))
			{
				if (json["Accuracy"].HasMember("Act"))
					DisNumConfig.Accuracy.Act = json["Accuracy"]["Act"].GetInt();
				if (json["Accuracy"].HasMember("Bct"))
					DisNumConfig.Accuracy.Bct = json["Accuracy"]["Bct"].GetInt();
			}
			if (json.HasMember("IsWarnUpperUsed"))
				DisNumConfig.IsWarnUpperUsed = json["IsWarnUpperUsed"].GetBool();
			if (json.HasMember("WarnUpper") && json["WarnUpper"].IsObject())
				DisNumConfig.WarnUpper.Parse(json["WarnUpper"]);
			if (json.HasMember("IsWarnLowerUsed"))
				DisNumConfig.IsWarnLowerUsed = json["IsWarnLowerUsed"].GetBool();
			if (json.HasMember("WarnLower") && json["WarnLower"].IsObject())
				DisNumConfig.WarnLower.Parse(json["WarnLower"]);
			if (json.HasMember("IsWarnStyleUsed"))
				DisNumConfig.IsWarnStyleUsed = json["IsWarnStyleUsed"].GetBool();
			if (json.HasMember("WarnStyleUpper") && json["WarnStyleUpper"].IsObject())
				DisNumConfig.WarnStyleUpper.Parse(json["WarnStyleUpper"]);
			if (json.HasMember("WarnStyleLower") && json["WarnStyleLower"].IsObject())
				DisNumConfig.WarnStyleLower.Parse(json["WarnStyleLower"]);

			if (json.HasMember("IsConvertUsed"))
				DisNumConfig.IsConvertUsed = json["IsConvertUsed"].GetBool();
			if (json.HasMember("NovUpperSrc") && json["NovUpperSrc"].IsObject())
				DisNumConfig.NovUpperSrc.Parse(json["NovUpperSrc"]);
			if (json.HasMember("NovLowerSrc") && json["NovLowerSrc"].IsObject())
				DisNumConfig.NovLowerSrc.Parse(json["NovLowerSrc"]);
			if (json.HasMember("NovUpperRes") && json["NovUpperRes"].IsObject())
				DisNumConfig.NovUpperRes.Parse(json["NovUpperRes"]);
			if (json.HasMember("NovLowerRes") && json["NovLowerRes"].IsObject())
				DisNumConfig.NovLowerRes.Parse(json["NovLowerRes"]);


			if (json.HasMember("ReadVar"))
				DisNumConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("Key") && !json["Key"].IsNull())
				DisNumConfig.Key.Parse(json["Key"]);
			if (json.HasMember("TextStyle"))
				DisNumConfig.TextStyle.Parse(json["TextStyle"]);
			if (json.HasMember("FmtStr") && !json["FmtStr"].IsNull())
				DisNumConfig.FmtStr = json["FmtStr"].GetString();
		}

	}
}
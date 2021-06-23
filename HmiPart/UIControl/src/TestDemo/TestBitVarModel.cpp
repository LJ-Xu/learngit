/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TestBitVarModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 关于测试按键绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "TestBitVarModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>

using namespace rapidjson;
using namespace std;
namespace UI
{

	void TestBitVarModel::InitTestData()
	{
		BitConfig.CtrlName = "GTestControl";
		BitConfig.Height = 100;
		BitConfig.Width = 100;
		BitConfig.X = 100;
		BitConfig.Y = 100;
	}
	void TestBitVarModel::InitData(std::string jstr)
	{
		Document json;
		/*FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/CircleJson.json", "r");
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
			if (json.HasMember("x"))
				BitConfig.X = json["x"].GetInt();
			if (json.HasMember("y"))
				BitConfig.Y = json["y"].GetInt();
			if (json.HasMember("width"))
				BitConfig.Width = json["width"].GetInt();
			if (json.HasMember("height"))
				BitConfig.Height = json["height"].GetInt();
			if (json.HasMember("isEnable"))
				BitConfig.IsEnable = json["isEnable"].GetBool();
		}

	}
}
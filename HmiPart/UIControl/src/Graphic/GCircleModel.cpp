/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleModel.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GCircleModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "Logger.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void GCircleModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/Zero.json", "r");
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
			LOG_INFO_("GCircleModel Parse Json\n");
			CircleConfig.InitBaseData(json);

			if (json.HasMember("RotateAngle"))
				CircleConfig.RotateAngle = json["RotateAngle"].GetDouble();
			if (json.HasMember("RotateCenter") && json["RotateCenter"].IsObject())
			{
				if (json["RotateCenter"].HasMember("X"))
					CircleConfig.RotateCenter.X = json["RotateCenter"]["X"].GetInt();
				if (json["RotateCenter"].HasMember("Y"))
					CircleConfig.RotateCenter.Y = json["RotateCenter"]["Y"].GetInt();
			}
			if (json.HasMember("IsPerfectCricle"))
				CircleConfig.IsStandard = json["IsPerfectCricle"].GetBool();
			if (json.HasMember("Fill") && json["Fill"].IsObject())
			{
				Value& fill = json["Fill"];  
				if (fill.HasMember("UseFillin"))
					CircleConfig.IsFill = fill["UseFillin"].GetBool();
				if (fill.HasMember("BackColor"))
					CircleConfig.FillColor = fill["BackColor"].GetInt();
				if (fill.HasMember("FillinStyle"))
					CircleConfig.FillStyle = fill["FillinStyle"].GetInt();
				if (fill.HasMember("FadeoutPecent"))
					CircleConfig.Line.Alpha = fill["FadeoutPecent"].GetInt();
				if (fill.HasMember("UseFillinStyle"))
					CircleConfig.UseFillinStyle = fill["UseFillinStyle"].GetBool();
				if (fill.HasMember("ColorFillinStyle"))
					CircleConfig.ColorFillinStyle = fill["ColorFillinStyle"].GetInt();
			}

			if (json.HasMember("Line") && json["Line"].IsObject())
				CircleConfig.Line.Parse(json["Line"]);
			if (json.HasMember("BgPicKey") && json["BgPicKey"].IsObject())
				CircleConfig.BgPicKey.Parse(json["BgPicKey"]);
			if (json.HasMember("CenterPoint"))
			{
				if (json["CenterPoint"].HasMember("X"))
					CircleConfig.ArcRes.Center.X = json["CenterPoint"]["X"].GetInt();
				if (json["CenterPoint"].HasMember("Y"))
					CircleConfig.ArcRes.Center.Y = json["CenterPoint"]["Y"].GetInt();
			}
			if (json.HasMember("LengthA"))
				CircleConfig.ArcRes.A = json["LengthA"].GetInt();
			if (json.HasMember("LengthB"))
				CircleConfig.ArcRes.B = json["LengthB"].GetInt();
		}

	}
}
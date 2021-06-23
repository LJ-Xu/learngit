/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleModel.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，矩形绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GRectangleModel.h"
#include "JsonComm.h"
#include "Logger.h"
using namespace rapidjson;

namespace UI
{
	void GRectangleModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/Rect.Json", "r");
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
			LOG_INFO_("GRectangleModel Parse Json\n");
			RectangleConfig.InitBaseData(json);

			if (json.HasMember("RotateAngle"))
				RectangleConfig.RotateAngle = json["RotateAngle"].GetDouble();
			if (json.HasMember("RotateCenter") && json["RotateCenter"].IsObject())
			{
				if (json["RotateCenter"].HasMember("X"))
					RectangleConfig.RotateCenter.X = json["RotateCenter"]["X"].GetInt();
				if (json["RotateCenter"].HasMember("Y"))
					RectangleConfig.RotateCenter.Y = json["RotateCenter"]["Y"].GetInt();
			}
			if (json.HasMember("RectWidth"))
				RectangleConfig.RectWidth = json["RectWidth"].GetInt();
			if (json.HasMember("RectHeight"))
				RectangleConfig.RectHeight = json["RectHeight"].GetInt();
			if (json.HasMember("CirDiameter"))
				RectangleConfig.Radius = json["CirDiameter"].GetInt() / 2;
			if (json.HasMember("Fill"))
			{
				Value& fill = json["Fill"];
				if (fill.IsObject())
				{
					if (fill.HasMember("UseFillin"))
						RectangleConfig.IsFill = fill["UseFillin"].GetBool();
					if (fill.HasMember("BackColor"))
						RectangleConfig.FillColor = fill["BackColor"].GetInt();
					if (fill.HasMember("FillinStyle"))
						RectangleConfig.FillStyle = fill["FillinStyle"].GetInt();
					if (fill.HasMember("FadeoutPecent"))
						RectangleConfig.Line.Alpha = fill["FadeoutPecent"].GetInt();
				}
			}

			if (json.HasMember("Line") && json["Line"].IsObject())
				RectangleConfig.Line.Parse(json["Line"]);
			if (json.HasMember("BgPicKey") && json["BgPicKey"].IsObject())
				RectangleConfig.BgPicKey.Parse(json["BgPicKey"]);
		}
	}
}
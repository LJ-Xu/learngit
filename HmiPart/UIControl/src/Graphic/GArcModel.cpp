/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcModel.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 关于圆弧绘制参数内容的源码，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GArcModel.h"
#include "Json.h"
#include "FluidUnitModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "Logger.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void GArcModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/GArc.json", "r");
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
			LOG_INFO_("GArcModel Parse Json\n");

			if (json.HasMember("Line") && json["Line"].IsObject())
				ArcConfig.Line.Parse(json["Line"]);
			if (json.HasMember("StartAngle"))
				ArcConfig.StartAngle = json["StartAngle"].GetDouble();
			if (json.HasMember("SweepAngle"))
				ArcConfig.EndAngle = json["SweepAngle"].GetDouble();
			if (json.HasMember("IsSector"))
				ArcConfig.IsSector = json["IsSector"].GetBool();
			if (json.HasMember("RotateAngle"))
				ArcConfig.RotateAngle = json["RotateAngle"].GetDouble();
			if (json.HasMember("RotateCenter") && json["RotateCenter"].IsObject())
			{
				if (json["RotateCenter"].HasMember("X"))
					ArcConfig.RotateCenter.X = json["RotateCenter"]["X"].GetInt();
				if (json["RotateCenter"].HasMember("Y"))
					ArcConfig.RotateCenter.Y = json["RotateCenter"]["Y"].GetInt();
			}
			if (json.HasMember("Fill"))
			{
				Value& fill = json["Fill"];
				if (fill.IsObject())
				{
					if (fill.HasMember("UseFillin"))
						ArcConfig.IsFill = fill["UseFillin"].GetBool();
					if (fill.HasMember("BackColor"))
						ArcConfig.FillColor = fill["BackColor"].GetInt();
					if (fill.HasMember("FillinStyle"))
						ArcConfig.FillStyle = fill["FillinStyle"].GetInt();
					if (fill.HasMember("FadeoutPecent"))
						ArcConfig.Line.Alpha = fill["FadeoutPecent"].GetInt();
				}
			}
			if (json.HasMember("CenterPoint"))
			{
				if (json["CenterPoint"].HasMember("X"))
					ArcConfig.ArcRes.Center.X = json["CenterPoint"]["X"].GetInt();
				if (json["CenterPoint"].HasMember("Y"))
					ArcConfig.ArcRes.Center.Y = json["CenterPoint"]["Y"].GetInt();
			}
			/*获取坐标点*/
			if (json.HasMember("PtSE")) {
				Value& array = json["PtSE"];
				if (array.IsArray()) {
					vector<Project::Point>().swap(ArcConfig.Coordinate);
					for (unsigned i = 0; i < array.Size(); i++) {
						Value& coordinate = array[i];
						if (coordinate.IsObject()) {
							Project::Point tmp;
							if (coordinate.HasMember("X"))
								tmp.X = coordinate["X"].GetInt();
							if (coordinate.HasMember("Y"))
								tmp.Y = coordinate["Y"].GetInt();
							ArcConfig.Coordinate.push_back(tmp);
						}
					}
				}
			}
			if (json.HasMember("LengthA"))
				ArcConfig.ArcRes.A = json["LengthA"].GetInt();
			if (json.HasMember("LengthB"))
				ArcConfig.ArcRes.B = json["LengthB"].GetInt();
			if (json.HasMember("BgPicKey") && json["BgPicKey"].IsObject())
				ArcConfig.BgPicKey.Parse(json["BgPicKey"]);
			ArcConfig.InitBaseData(json);
		}

	}

}

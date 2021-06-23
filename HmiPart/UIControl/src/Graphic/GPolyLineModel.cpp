/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 关于折线多边形绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GPolyLineModel.h"
#include "Point.h"
#include "JsonComm.h"
#include "Logger.h"
using namespace rapidjson;
namespace UI
{
	void GPolyLineModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/PolyLineJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("GPolyLineModel Parse Json\n");
			PolyLineConfig.InitBaseData(json);

			if (json.HasMember("RotateAngle"))
				PolyLineConfig.RotateAngle = json["RotateAngle"].GetDouble();
			if (json.HasMember("RotateCenter") && json["RotateCenter"].IsObject())
			{
				if (json["RotateCenter"].HasMember("X"))
					PolyLineConfig.RotateCenter.X = json["RotateCenter"]["X"].GetInt();
				if (json["RotateCenter"].HasMember("Y"))
					PolyLineConfig.RotateCenter.Y = json["RotateCenter"]["Y"].GetInt();
			}
			/*获取坐标点*/
			if (json.HasMember("Points")) {
				Value& array = json["Points"];
				if (array.IsArray()) {
					vector<Project::Point>().swap(PolyLineConfig.Points);
					for (unsigned i = 0; i < array.Size(); i++) {
						Value& coordinate = array[i];
						if (coordinate.IsObject()) {
							Project::Point tmp;
							if (coordinate.HasMember("X"))
								tmp.X = coordinate["X"].GetInt();
							if (coordinate.HasMember("Y"))
								tmp.Y = coordinate["Y"].GetInt();
							PolyLineConfig.Points.push_back(tmp);
						}
					}
				}
			}
			if (json.HasMember("Line") && json["Line"].IsObject())
				PolyLineConfig.LineStyle.Parse(json["Line"]);
			if (json.HasMember("Mode"))
				PolyLineConfig.Mode = json["Mode"].GetInt();
			if (json.HasMember("SIsArrow"))
				PolyLineConfig.Arrow.HaveStartArrow = json["SIsArrow"].GetBool();
			if (json.HasMember("EIsArrow"))
				PolyLineConfig.Arrow.HaveEndArrow = json["EIsArrow"].GetBool();

			if (json.HasMember("TypeHead"))
				PolyLineConfig.Arrow.StartArrowType = (Project::ArrowType)json["TypeHead"].GetInt();
			if (json.HasMember("TypeTail"))
				PolyLineConfig.Arrow.EndArrowType = (Project::ArrowType)json["TypeTail"].GetInt();
			if (json.HasMember("SizeHead"))
				PolyLineConfig.Arrow.StartArrowSize = json["SizeHead"].GetInt();
			if (json.HasMember("SizeTail"))
				PolyLineConfig.Arrow.EndArrowSize = json["SizeTail"].GetInt();
			if (json.HasMember("UseLine"))
				PolyLineConfig.UseLine = json["UseLine"].GetBool();
			if (json.HasMember("Fill") && json["Fill"].IsObject())
			{
				if(json["Fill"].HasMember("UseFillin"))
					PolyLineConfig.IsFill = json["Fill"]["UseFillin"].GetBool();
				if (json["Fill"].HasMember("BackColor"))
					PolyLineConfig.FillColor = json["Fill"]["BackColor"].GetInt();
				if (json["Fill"].HasMember("FillinStyle"))
					PolyLineConfig.FillStyle = json["Fill"]["FillinStyle"].GetInt();
			}
			if (json.HasMember("BgPicKey") && json["BgPicKey"].IsObject())
				PolyLineConfig.BgPicKey.Parse(json["BgPicKey"]);
		}
	}
}
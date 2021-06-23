/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineModel.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，直线绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GLineModel.h"
#include "JsonComm.h"
#include "Logger.h"
using namespace rapidjson;
namespace UI
{
	void GLineModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/GLine.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);

		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("GLineModel Parse Json\n");
			LineConfig.InitBaseData(json);

			if (json.HasMember("RotateAngle"))
				LineConfig.RotateAngle = json["RotateAngle"].GetDouble();
			if (json.HasMember("RotateCenter") && json["RotateCenter"].IsObject())
			{
				if (json["RotateCenter"].HasMember("X"))
					LineConfig.RotateCenter.X = json["RotateCenter"]["X"].GetInt();
				if (json["RotateCenter"].HasMember("Y"))
					LineConfig.RotateCenter.Y = json["RotateCenter"]["Y"].GetInt();
			}
			/*获取坐标点*/
			if (json.HasMember("PtSE")) {
				Value& array = json["PtSE"];
				if (array.IsArray()) {
					vector<Project::Point>().swap(LineConfig.Coordinate);
					for (size_t i = 0; i < array.Size(); i++) {
						Value& coordinate = array[i];
						if (coordinate.IsObject()) {
							Project::Point tmp;
							if (coordinate.HasMember("X"))
								tmp.X = coordinate["X"].GetInt();
							if (coordinate.HasMember("Y"))
								tmp.Y = coordinate["Y"].GetInt();
							LineConfig.Coordinate.push_back(tmp);
						}
					}
				}
			}
			if (json.HasMember("SIsArrow"))
				LineConfig.Arrow.HaveStartArrow = json["SIsArrow"].GetBool();
			if (json.HasMember("EIsArrow"))
				LineConfig.Arrow.HaveEndArrow = json["EIsArrow"].GetBool();

			if (json.HasMember("TypeHead"))
				LineConfig.Arrow.StartArrowType = (Project::ArrowType)json["TypeHead"].GetInt();
			if (json.HasMember("TypeTail"))
				LineConfig.Arrow.EndArrowType = (Project::ArrowType)json["TypeTail"].GetInt();
			if (json.HasMember("SizeHead"))
				LineConfig.Arrow.StartArrowSize = json["SizeHead"].GetInt();
			if (json.HasMember("SizeTail"))
				LineConfig.Arrow.EndArrowSize = json["SizeTail"].GetInt();
			if (json.HasMember("Line") && json["Line"].IsObject())
				LineConfig.Line.Parse(json["Line"]);
		}
	}
}
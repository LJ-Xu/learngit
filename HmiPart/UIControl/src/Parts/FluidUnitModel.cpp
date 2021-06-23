/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : FluidUnitModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : ����������Ʋ������ݣ�����json����������Ϣ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "FluidUnitModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>

using namespace rapidjson;
using namespace std;
namespace UI
{
	void FluidUnitModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/Fluid.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);

		json.Parse(jstr.c_str());
		if (json.HasParseError())	//�жϽ����Ƿ���ȷ
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("FluidUnitModel Parse Json\n");
			FluidConfig.InitBaseData(json);

			if (json.HasMember("FlowMode"))
				 FluidConfig.ActionMode = json["FlowMode"].GetInt();
			if (json.HasMember("FlowCondition"))
				FluidConfig.FlowCondition = json["FlowCondition"].GetInt();
			if (json.HasMember("ReadAddr") && !json["ReadAddr"].IsNull())
				FluidConfig.FlowVarId.Parse(json["ReadAddr"]);
			if (json.HasMember("FlowDirection"))
				FluidConfig.FlowDirection = json["FlowDirection"].GetInt();
			if (json.HasMember("FlowSpeed"))
				FluidConfig.FlowSpeed = json["FlowSpeed"].GetDouble();
			if (json.HasMember("FlowSpeedData"))
				FluidConfig.SpeedVarId.Parse(json["FlowSpeedData"]);
			

			/*��ȡ�ṹ������*/
			if (json.HasMember("Coordinate") && json["Coordinate"].IsArray()) {
				vector<Project::Point>().swap(FluidConfig.Coordinates);
				for (size_t i = 0; i < json["Coordinate"].Size(); i++) {
					Value& point = json["Coordinate"][i];
					if (point.IsObject()) {
						if (point.HasMember("X") && point["X"].IsInt() && point.HasMember("Y") && point["Y"].IsInt()) {
							Project::Point tmp;
							tmp.X = point["X"].GetInt();
							tmp.Y = point["Y"].GetInt();
							FluidConfig.Coordinates.push_back(tmp);
						}
					}
				}
			}
			
			/*��ȡ��������*/
			if (json.HasMember("CatheterWidget"))
				FluidConfig.PipeParam.PipeWidth = json["CatheterWidget"].GetInt();
			if (json.HasMember("CatheterFrameWidgetPecent"))
				FluidConfig.PipeParam.FrameWidth = json["CatheterFrameWidgetPecent"].GetInt();
			if (json.HasMember("CatherBackColor"))
				FluidConfig.PipeParam.PipeColor = json["CatherBackColor"].GetInt();
			if (json.HasMember("CatheterFrameColor"))
				FluidConfig.PipeParam.FrameColor = json["CatheterFrameColor"].GetInt();
			
			/*��ȡ��������*/
			if (json.HasMember("SliderType"))
				FluidConfig.SliderParam.SliderType = json["SliderType"].GetInt();
			if (json.HasMember("SliderWidget"))
				FluidConfig.SliderParam.SliderWidth = json["SliderWidget"].GetInt();
			if (json.HasMember("SliderLength"))
				FluidConfig.SliderParam.SliderLength = json["SliderLength"].GetInt();
			if (json.HasMember("SliderIntervalWidget"))
				FluidConfig.SliderParam.SliderIntervalWidget = json["SliderIntervalWidget"].GetInt();
			if (json.HasMember("ArrowAngle"))
				FluidConfig.SliderParam.ArrowAngle = json["ArrowAngle"].GetInt();
			if (json.HasMember("SliderColor"))
				FluidConfig.SliderParam.SliderColorON = json["SliderColor"].GetInt();
			if (json.HasMember("SliderColorOff"))
				FluidConfig.SliderParam.SliderColorOFF = json["SliderColorOff"].GetInt();
		}
	}
}
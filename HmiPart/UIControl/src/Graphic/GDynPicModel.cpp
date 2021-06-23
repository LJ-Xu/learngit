/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GDynPicModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 关于动态图片绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "GDynPicModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
#include "Logger.h"
using namespace rapidjson;

namespace UI
{
	void GDynPicModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/DynamicPic.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("GDynPicModel Parse Json\n");
			PicDynConfig.InitBaseData(json);
			if (json.HasMember("PeriodSecond"))
				PicDynConfig.CycleTime = json["PeriodSecond"].GetInt();
			if (json.HasMember("CycleVarId"))
				PicDynConfig.CycleTimeVarId.Parse(json["CycleVarId"]);
			if (json.HasMember("SwitchMode"))
				PicDynConfig.SwitchMode = (Project::PicSwitchMode)json["SwitchMode"].GetInt();
			if (json.HasMember("SwitchSeq"))
				PicDynConfig.SwitchSeq = (Project::PicSwitchSeq)json["SwitchSeq"].GetInt();
			if (json.HasMember("RandomSort") && !json["RandomSort"].IsNull())
				PicDynConfig.SwitchOrder = json["RandomSort"].GetString();
			if (json.HasMember("ImageFmtList") && json["ImageFmtList"].IsArray())
			{
				PicDynConfig.PicNum = json["ImageFmtList"].Size();
				std::vector<Project::ImageFormat>().swap(PicDynConfig.ImageFmtList);
				for (unsigned i = 0; i < json["ImageFmtList"].Size(); i++) {
					Value& Picparam = json["ImageFmtList"][i];
					if (Picparam.IsObject()) {
						Project::ImageFormat PictureTmp;
						if (Picparam.HasMember("KeyVal") && !Picparam["KeyVal"].IsNull())
							PictureTmp.KeyVal = Picparam["KeyVal"].GetString();
						if (Picparam.HasMember("UseFaheout"))
							PictureTmp.UseFaheout = Picparam["UseFaheout"].GetBool();
						if (Picparam.HasMember("FadeoutPecent"))
							PictureTmp.FadeoutPecent = Picparam["FadeoutPecent"].GetInt();
						if (Picparam.HasMember("UseRotate"))
							PictureTmp.UseRotate = Picparam["UseRotate"].GetBool();
						if (Picparam.HasMember("RotateAngle"))
							PictureTmp.RotateAngle = Picparam["RotateAngle"].GetInt();
						PicDynConfig.ImageFmtList.push_back(PictureTmp);
					}
				}
			}
			if (json.HasMember("OriginalImages") && json["OriginalImages"].IsArray())
				Project::ImageResId::Parse(PicDynConfig.OriginalImages, json["OriginalImages"]);
			if (json.HasMember("UseStartSign"))
				PicDynConfig.UseStartSign = json["UseStartSign"].GetBool();
			if (json.HasMember("DataStartSign") && json["DataStartSign"].IsObject())
				PicDynConfig.StartSignVarId.Parse(json["DataStartSign"]);
			if (json.HasMember("UseStopSign"))
				PicDynConfig.UseStopSign = json["UseStopSign"].GetBool();
			if (json.HasMember("DataStopSign") && json["DataStopSign"].IsObject())
				PicDynConfig.StopSignVarId.Parse(json["DataStopSign"]);
			if (json.HasMember("StartTrigCondition"))
				PicDynConfig.StartTrigCondition = json["StartTrigCondition"].GetInt();
			if (json.HasMember("EndTrigCondition"))
				PicDynConfig.EndTrigCondition = json["EndTrigCondition"].GetInt();
		}

	}
}

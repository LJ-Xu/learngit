/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInputModel.cpp
 * Author   : qiaodan
 * Date     : 2020-11-03
 * Descript : 解析json，关于报警录入参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "AlarmInputModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void AlarmInputModel::InitData(std::string jstr)
	{
		Document json;
		FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/AlarmInput.json", "r");
		char buf[0XFFFF];
		rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		json.ParseStream(input);
		//json.Parse(jstr.c_str());
		if (json.HasMember("OffX"))
			InputAlarmConfig.InitOffXData(json["OffX"]);
		if (json.HasMember("OffY"))
			InputAlarmConfig.InitOffYData(json["OffY"]);
		if (json.HasMember("Width"))
			InputAlarmConfig.Width = json["Width"].GetInt();
		if (json.HasMember("Height"))
			InputAlarmConfig.Height = json["Height"].GetInt();
		if (json.HasMember("X"))
			InputAlarmConfig.X = json["X"].GetInt();
		if (json.HasMember("Y"))
			InputAlarmConfig.Y = json["Y"].GetInt();
		if (json.HasMember("IsEnable"))
			InputAlarmConfig.IsEnable = json["IsEnable"].GetBool();
		if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
			InputAlarmConfig.CtrlName = json["CtrlName"].GetString();
		if (json.HasMember("ConfigInfoLst") && json["ConfigInfoLst"].IsArray())
			Project::AlarmInfoRes::Parse(InputAlarmConfig.InfoLst, json["ConfigInfoLst"]);
		if (json.HasMember("FileNameMode"))
			InputAlarmConfig.SaveLst.FileNameMode = (Project::BaseBtnFunction::FlieNameType)json["FileNameMode"].GetInt();
		if (json.HasMember("FileName") && !json["FileName"].IsNull())
			InputAlarmConfig.SaveLst.FileName = json["FileName"].GetString();
		if (json.HasMember("StoreSite"))
			InputAlarmConfig.SaveLst.FileStoreSite = (Project::BaseBtnFunction::FlieLocation)json["StoreSite"].GetInt();
		if (json.HasMember("FileNameVarId") && json["FileNameVarId"].IsObject())
			InputAlarmConfig.SaveLst.FileNameDataVar.Parse(json["FileNameVarId"]);
		if (json.HasMember("StorePosVarId") && json["StorePosVarId"].IsObject())
			InputAlarmConfig.SaveLst.StorePosVarId.Parse(json["StorePosVarId"]);
		if (json.HasMember("HisInfoLst") && json["HisInfoLst"].IsArray())
		{
			bool select;
			std::vector<string>().swap(InputAlarmConfig.SaveLst.Contents);
			for (unsigned i = 0; i < json["HisInfoLst"].Size(); i++)
			{
				if (json["HisInfoLst"][i].HasMember("IsSelect"))
					select = json["HisInfoLst"][i]["IsSelect"].GetBool();
				if (select && json["HisInfoLst"][i].HasMember("Describe") &&
					!json["HisInfoLst"][i]["Describe"].IsNull())
					InputAlarmConfig.SaveLst.Contents.push_back(json["HisInfoLst"][i]["Describe"].GetString());
			}

		}
		if (json.HasMember("Perm"))
			InputAlarmConfig.Perm.Parse(json["Perm"]);
	}
}
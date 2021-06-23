/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DataSampleModel.cpp
 * Author   : qiaodan
 * Date     : 2020-11-12
 * Descript : 解析json，关于数据采样参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DataSampleModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void DataSampleModel::InitData(std::string jstr)
	{
		Document json;
		FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/Sample.json", "r");
		char buf[0XFFFF];
		rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		json.ParseStream(input);
		//json.Parse(jstr.c_str());
		if (json.HasMember("OffX"))
			SampleDataConfig.InitOffXData(json["OffX"]);
		if (json.HasMember("OffY"))
			SampleDataConfig.InitOffYData(json["OffY"]);
		if (json.HasMember("Width"))
			SampleDataConfig.Width = json["Width"].GetInt();
		if (json.HasMember("Height"))
			SampleDataConfig.Height = json["Height"].GetInt();
		if (json.HasMember("X"))
			SampleDataConfig.X = json["X"].GetInt();
		if (json.HasMember("Y"))
			SampleDataConfig.Y = json["Y"].GetInt();
		if (json.HasMember("IsEnable"))
			SampleDataConfig.IsEnable = json["IsEnable"].GetBool();
		if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
			SampleDataConfig.CtrlName = json["CtrlName"].GetString();
		if (json.HasMember("DataSimpleUintsLst") && json["DataSimpleUintsLst"].IsArray())
			Project::SampleInfoRes::Parse(SampleDataConfig.InfoLst, json["DataSimpleUintsLst"]);
		
		if (json.HasMember("Perm"))
			SampleDataConfig.Perm.Parse(json["Perm"]);
	}
}
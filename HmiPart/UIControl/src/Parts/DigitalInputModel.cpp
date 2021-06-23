/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于数值输入框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DigitalInputModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void DigitalInputModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/DigitalInputJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("DigitalInputModel Parse Json\n");
			InputNumConfig.InitBaseData(json);

			if (json.HasMember("IsRwDiff"))
				InputNumConfig.RwDiffAddrUsed = json["IsRwDiff"].GetBool();
			if (json.HasMember("WriteVar") && json["WriteVar"].IsObject())
				InputNumConfig.WriteVar.Parse(json["WriteVar"]);
			if (json.HasMember("KeyMethod"))
				InputNumConfig.KeyMethod.Parse(json["KeyMethod"]);
			if (json.HasMember("Sequence") && json["Sequence"].IsObject())
			{
				if (json["Sequence"].HasMember("CancelSequenceAfterCompleted"))
					InputNumConfig.Sequence.CancelSequenceAfterCompleted = json["Sequence"]["CancelSequenceAfterCompleted"].GetBool();
				if (json["Sequence"].HasMember("SequenceId"))
					InputNumConfig.Sequence.SequenceId = json["Sequence"]["SequenceId"].GetInt();
				if (json["Sequence"].HasMember("GroupId"))
					InputNumConfig.Sequence.GroupId = json["Sequence"]["GroupId"].GetInt();
			}
			if (json.HasMember("SequenceGroup") && json["SequenceGroup"].IsObject())
			{
				Value &group = json["SequenceGroup"];
				for (rapidjson::Value::MemberIterator iter = group.MemberBegin();iter != group.MemberEnd();iter++)
				{
					string name = (iter->name).GetString();
					Value& groupvalue = iter->value;
					vector<Project::WinCtrlID> sequencectrls;
					if (groupvalue.IsArray())
					{
						for (size_t i = 0; i < groupvalue.Size(); i++)
							sequencectrls.push_back(groupvalue[i].GetInt());
					}
					InputNumConfig.SequenceGroup.insert(pair<int, vector<Project::WinCtrlID>>(stoi(name), sequencectrls));
				}
			}
			if (json.HasMember("IsInputConvertUsed"))
				InputNumConfig.IsInputConvertUsed = json["IsInputConvertUsed"].GetBool();
			if (json.HasMember("NovUpperSrcInput") && json["NovUpperSrcInput"].IsObject())
				InputNumConfig.NovUpperSrcInput.Parse(json["NovUpperSrcInput"]);
			if (json.HasMember("NovLowerSrcInput") && json["NovLowerSrcInput"].IsObject())
				InputNumConfig.NovLowerSrcInput.Parse(json["NovLowerSrcInput"]);
			if (json.HasMember("NovUpperResInput") && json["NovUpperResInput"].IsObject())
				InputNumConfig.NovUpperResInput.Parse(json["NovUpperResInput"]);
			if (json.HasMember("NovLowerResInput") && json["NovLowerResInput"].IsObject())
				InputNumConfig.NovLowerResInput.Parse(json["NovLowerResInput"]);
			
			if (json.HasMember("Notifies") && json["Notifies"].IsArray())
			{
				if (json["Notifies"].Size() >= 1)
					InputNumConfig.ResBef.Parse(json["Notifies"][0]);
				if (json["Notifies"].Size() >= 2)
					InputNumConfig.ResAft.Parse(json["Notifies"][1]);
			}
			if (json.HasMember("PswMode"))
				InputNumConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("IsFillZero"))
				InputNumConfig.IsFillZero = json["IsFillZero"].GetBool();
			if (json.HasMember("Accuracy"))
			{
				if (json["Accuracy"].HasMember("Act"))
					InputNumConfig.Accuracy.Act = json["Accuracy"]["Act"].GetInt();
				if (json["Accuracy"].HasMember("Bct"))
					InputNumConfig.Accuracy.Bct = json["Accuracy"]["Bct"].GetInt();
			}
			if (json.HasMember("IsWarnUpperUsed"))
				InputNumConfig.IsWarnUpperUsed = json["IsWarnUpperUsed"].GetBool();
			if (json.HasMember("WarnUpper") && json["WarnUpper"].IsObject())
				InputNumConfig.WarnUpper.Parse(json["WarnUpper"]);
			if (json.HasMember("IsWarnLowerUsed"))
				InputNumConfig.IsWarnLowerUsed = json["IsWarnLowerUsed"].GetBool();
			if (json.HasMember("WarnLower") && json["WarnLower"].IsObject())
				InputNumConfig.WarnLower.Parse(json["WarnLower"]);
			if (json.HasMember("IsWarnStyleUsed"))
				InputNumConfig.IsWarnStyleUsed = json["IsWarnStyleUsed"].GetBool();
			if (json.HasMember("WarnStyleUpper") && json["WarnStyleUpper"].IsObject())
				InputNumConfig.WarnStyleUpper.Parse(json["WarnStyleUpper"]);
			if (json.HasMember("WarnStyleLower") && json["WarnStyleLower"].IsObject())
				InputNumConfig.WarnStyleLower.Parse(json["WarnStyleLower"]);

			if (json.HasMember("IsConvertUsed"))
				InputNumConfig.IsConvertUsed = json["IsConvertUsed"].GetBool();
			if (json.HasMember("NovUpperSrc") && json["NovUpperSrc"].IsObject())
				InputNumConfig.NovUpperSrc.Parse(json["NovUpperSrc"]);
			if (json.HasMember("NovLowerSrc") && json["NovLowerSrc"].IsObject())
				InputNumConfig.NovLowerSrc.Parse(json["NovLowerSrc"]);
			if (json.HasMember("NovUpperRes") && json["NovUpperRes"].IsObject())
				InputNumConfig.NovUpperRes.Parse(json["NovUpperRes"]);
			if (json.HasMember("NovLowerRes") && json["NovLowerRes"].IsObject())
				InputNumConfig.NovLowerRes.Parse(json["NovLowerRes"]);


			if (json.HasMember("ReadVar"))
				InputNumConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("Key") && !json["Key"].IsNull())
				InputNumConfig.Key.Parse(json["Key"]);
			if (json.HasMember("TextStyle"))
				InputNumConfig.TextStyle.Parse(json["TextStyle"]);
			if (json.HasMember("FmtStr") && !json["FmtStr"].IsNull())
				InputNumConfig.FmtStr = json["FmtStr"].GetString();

		}
	}
}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于字符输入框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "CharacterInputModel.h"
#include "JsonComm.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void CharacterInputModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/CharacterInputJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("CharacterInputModel Parse Json\n");
			InputStrConfig.InitBaseData(json);

			if (json.HasMember("WriteVar"))
				InputStrConfig.WriteVar.Parse(json["WriteVar"]);
			if (json.HasMember("KeyMethod"))
				InputStrConfig.KeyMethod.Parse(json["KeyMethod"]);
			if (json.HasMember("Sequence") && json["Sequence"].IsObject())
			{
				if (json["Sequence"].HasMember("CancelSequenceAfterCompleted"))
					InputStrConfig.Sequence.CancelSequenceAfterCompleted = json["Sequence"]["CancelSequenceAfterCompleted"].GetBool();
				if (json["Sequence"].HasMember("SequenceId"))
					InputStrConfig.Sequence.SequenceId = json["Sequence"]["SequenceId"].GetInt();
				if (json["Sequence"].HasMember("GroupId"))
					InputStrConfig.Sequence.GroupId = json["Sequence"]["GroupId"].GetInt();
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
					InputStrConfig.SequenceGroup.insert(pair<int, vector<Project::WinCtrlID>>(stoi(name), sequencectrls));
				}
			}
			if (json.HasMember("Notifies") && json["Notifies"].IsArray())
			{
				if (json["Notifies"].Size() >= 1)
					InputStrConfig.ResBef.Parse(json["Notifies"][0]);
				if (json["Notifies"].Size() >= 2)
					InputStrConfig.ResAft.Parse(json["Notifies"][1]);
			}
			if (json.HasMember("PswMode"))
				InputStrConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("ByteConversion"))
				InputStrConfig.ByteConversion = json["ByteConversion"].GetInt();
			if (json.HasMember("Rule"))
				InputStrConfig.Rule = (UI::CodeFormatUtility::CodeRule)json["Rule"].GetInt();
			if (json.HasMember("Key") && !json["Key"].IsNull())
				InputStrConfig.Key.Parse(json["Key"]);
			if (json.HasMember("TextStyle"))
				InputStrConfig.TextStyle.Parse(json["TextStyle"]);
			if (json.HasMember("FmtStr") && !json["FmtStr"].IsNull())
				InputStrConfig.FmtStr = json["FmtStr"].GetString();
			if (json.HasMember("Perm"))
				InputStrConfig.Perm.Parse(json["Perm"]);
		}
	}
}
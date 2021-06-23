/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 解析json，关于中文输入框内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseInputModel.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "ResourceService.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void ChineseInputModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/ChineseInput.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());

		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("ChineseInputModel Parse Json\n");
			InputChineseConfig.InitBaseData(json);

			if (json.HasMember("ReadVar"))
				InputChineseConfig.WriteVar.Parse(json["ReadVar"]);
			if (json.HasMember("KeyMethod"))
				InputChineseConfig.KeyMethod.Parse(json["KeyMethod"]);
			if (json.HasMember("Sequence") && json["Sequence"].IsObject())
			{
				if (json["Sequence"].HasMember("CancelSequenceAfterCompleted"))
					InputChineseConfig.Sequence.CancelSequenceAfterCompleted = json["Sequence"]["CancelSequenceAfterCompleted"].GetBool();
				if (json["Sequence"].HasMember("SequenceId"))
					InputChineseConfig.Sequence.SequenceId = json["Sequence"]["SequenceId"].GetInt();
				if (json["Sequence"].HasMember("GroupId"))
					InputChineseConfig.Sequence.GroupId = json["Sequence"]["GroupId"].GetInt();
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
					InputChineseConfig.SequenceGroup.insert(pair<int, vector<Project::WinCtrlID>>(stoi(name), sequencectrls));
				}
			}
			if (json.HasMember("Notifies") && json["Notifies"].IsArray())
			{
				if (json["Notifies"].Size() >= 1)
					InputChineseConfig.ResBef.Parse(json["Notifies"][0]);
				if (json["Notifies"].Size() >= 2)
					InputChineseConfig.ResAft.Parse(json["Notifies"][1]);
			}
			if (json.HasMember("PswMode"))
				InputChineseConfig.PswMode = json["PswMode"].GetInt();
			if (json.HasMember("Rule"))
				InputChineseConfig.Rule = (UI::CodeFormatUtility::CodeRule)json["Rule"].GetInt();
			if (json.HasMember("Key") && !json["Key"].IsNull())
				InputChineseConfig.Key.Parse(json["Key"]);
			if (json.HasMember("TextStyle"))
				InputChineseConfig.TextStyle.Parse(json["TextStyle"]);
			if (json.HasMember("Text") && !json["Text"].IsNull())
				InputChineseConfig.Text = json["Text"].GetString();
			if (json.HasMember("Perm"))
				InputChineseConfig.Perm.Parse(json["Perm"]);
		}

	}
}
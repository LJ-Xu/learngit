/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiStatusKeypadModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-23
 * Descript : 解析json，关于多状态按键绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "MultiStatusKeypadModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void MultiStatusKeypadModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/Keypad.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		else
		{
			LOG_INFO_("MultiStatusKeypadModel Parse Json\n");

			MultiStatusKeypadConfig.InitBaseData(json);
			if (json.HasMember("StatusCnt"))
				MultiStatusKeypadConfig.StatusCnt = json["StatusCnt"].GetInt();
			if (json.HasMember("DataVarType"))
				MultiStatusKeypadConfig.DataVarType = json["DataVarType"].GetInt();
			if (json.HasMember("BitNum"))
				MultiStatusKeypadConfig.BitNum = json["BitNum"].GetInt();
			if (json.HasMember("Actions") && json["Actions"].IsArray())
			{
				vector<Project::StatusAction>().swap(MultiStatusKeypadConfig.Action);
				Value &action = json["Actions"];
				for (unsigned i = 0; i < action.Size(); i++)
				{
					Project::StatusAction tmpStatus;
					if (action[i].HasMember("Id"))
						tmpStatus.Id = action[i]["Id"].GetInt();
					if (action[i].HasMember("Val") && !action[i]["Val"].IsNull())
						tmpStatus.Val = action[i]["Val"].GetString();
					if (action[i].HasMember("Key") && action[i]["Key"].IsObject())
						tmpStatus.PicKey.Parse(action[i]["Key"]);
					if (action[i].HasMember("Css") && action[i]["Css"].IsObject())
						tmpStatus.StrStyles.Parse(action[i]["Css"]);
					MultiStatusKeypadConfig.Action.push_back(tmpStatus);
				}
			}
			if (json.HasMember("Vars") && json["Vars"].IsArray())
				Project::DataVarId::Parse(MultiStatusKeypadConfig.RegVars, json["Vars"]);
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				MultiStatusKeypadConfig.Txt.Parse(json["Txt"]);
		}
	}
}
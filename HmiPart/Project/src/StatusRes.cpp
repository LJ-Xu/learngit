/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StatusRes.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ∂‡”Ô—‘Œƒ◊÷jsonΩ‚Œˆ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include <vector>
#include "StatusRes.h"
namespace Project
{
	void StatusRes::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("DefaultText") && jsonObj["DefaultText"].IsArray())
		{
			std::vector<std::string>().swap(Texts);
			for (unsigned i = 0; i < jsonObj["DefaultText"].Size(); i++)
			{
				if (!jsonObj["DefaultText"][i].IsNull())
					Texts.push_back(jsonObj["DefaultText"][i].GetString());
			}
		}
		if (jsonObj.HasMember("LabelName") && !jsonObj["LabelName"].IsNull())
			Label = jsonObj["LabelName"].GetString();
		if (jsonObj.HasMember("TableNo"))
			TableNo = jsonObj["TableNo"].GetInt();
		if (jsonObj.HasMember("RowNo"))
			RowNo = jsonObj["RowNo"].GetInt();
	}
	void StatusRes::Parse(std::vector<StatusRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<StatusRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::StatusRes* id = new Project::StatusRes();
				id->Parse(jsonObj[i]);
				vector.push_back(*id);
			}
		}
	}
}
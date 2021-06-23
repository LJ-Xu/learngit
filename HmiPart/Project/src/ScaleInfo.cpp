/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ScaleInfo.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ×ø±êÀàjson½âÎö
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ScaleInfo.h"
#include <vector>
namespace Project
{
	void ScaleInfo::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Coordinate") && jsonObj["Coordinate"].IsObject())
		{
			if (jsonObj["Coordinate"].HasMember("x"))
				Coordinate.X = jsonObj["Coordinate"]["x"].GetInt();
			if (jsonObj["Coordinate"].HasMember("y"))
				Coordinate.Y = jsonObj["Coordinate"]["y"].GetInt();
		}
		if (jsonObj.HasMember("Width"))
			Width = jsonObj["Width"].GetInt();
		if (jsonObj.HasMember("Height"))
			Height = jsonObj["Height"].GetInt();
		if (jsonObj.HasMember("Scalecontent") && !jsonObj["Scalecontent"].IsNull())
			ScaleContent = jsonObj["Scalecontent"].GetString();
	}
	void ScaleInfo::Parse(std::vector<ScaleInfo>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<ScaleInfo>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::ScaleInfo res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
}
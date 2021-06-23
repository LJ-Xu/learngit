/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChannel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 趋势图通道配置json解析
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "TrendChannel.h"
namespace Project
{
	void TrendChannel::Parse(rapidjson::Value& jsonObj, int gname, int gno)
	{
		if (jsonObj.HasMember("Channel"))
			ChannelNo = jsonObj["Channel"].GetInt() | (gname << 16) | (gno << 24);
		if (jsonObj.HasMember("TrendMax"))
			TrendMax = jsonObj["TrendMax"].GetDouble();
		if (jsonObj.HasMember("TrendMin"))
			TrendMin = jsonObj["TrendMin"].GetDouble();
		if (jsonObj.HasMember("TrendMaxVarId") && jsonObj["TrendMaxVarId"].IsObject())
			TrendMaxVarId.Parse(jsonObj["TrendMaxVarId"]);
		if (jsonObj.HasMember("TrendMinVarId") && jsonObj["TrendMinVarId"].IsObject())
			TrendMinVarId.Parse(jsonObj["TrendMinVarId"]);
		if (jsonObj.HasMember("TrendStyle") && jsonObj["TrendStyle"].IsObject())
			TrendStyle.Parse(jsonObj["TrendStyle"]);
	}
	void TrendChannel::Parse(std::vector<TrendChannel>& vector, rapidjson::Value& jsonObj, int gname, int gno)
	{
		std::vector<TrendChannel>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::TrendChannel res;
				if (jsonObj[i].HasMember("IsSelect") && jsonObj[i]["IsSelect"].GetBool())
				{
					res.Parse(jsonObj[i], gname,gno);
					vector.push_back(res);
				}
			}
		}
	}
}

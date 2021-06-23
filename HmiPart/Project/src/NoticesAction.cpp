/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : NoticesAction.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 消息通知json解析
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "NoticesAction.h"
namespace Project
{
	void NoticesAction::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("BitNotices") && jsonObj["BitNotices"].IsObject())
		{
			if (jsonObj["BitNotices"].HasMember("State"))
				Bit.State = (NotifyBitMode)jsonObj["BitNotices"]["State"].GetInt();
			if (jsonObj["BitNotices"].HasMember("Data") && jsonObj["BitNotices"]["Data"].IsObject())
				Bit.DataBitVar.Parse(jsonObj["BitNotices"]["Data"]);
		}
		if (jsonObj.HasMember("WordNotices") && jsonObj["WordNotices"].IsObject())
		{
			if (jsonObj["WordNotices"].HasMember("Val"))
				Word.Val = jsonObj["WordNotices"]["Val"].GetDouble();
			if (jsonObj["WordNotices"].HasMember("Data") && jsonObj["WordNotices"]["Data"].IsObject())
				Word.DataWordVar.Parse(jsonObj["WordNotices"]["Data"]);
		}
	}
	void NoticesAction::Parse(std::vector<NoticesAction>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<NoticesAction>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::NoticesAction* id = new Project::NoticesAction();
				id->Parse(jsonObj[i]);
				vector.push_back(*id);
			}
		}
	}
}
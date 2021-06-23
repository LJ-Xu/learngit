/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerModel.cpp
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 关于定时器参数内容的源码，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "TimerModel.h"

using namespace rapidjson;
using namespace std;
namespace UI
{
	void TimerModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/TableJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);

		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("TimerModel Parse Json\n");
			TimerConfig.InitBaseData(json);
			if (json.HasMember("TimeUnit"))
			{
				int timeunit = json["TimeUnit"].GetInt();
				if (timeunit == 0) //0.1s
					TimerConfig.CycTime = 100;
				else if(timeunit == 1) //1s
					TimerConfig.CycTime = 1000;
				else if (timeunit == 2) //1min
					TimerConfig.CycTime = 60000;
			}
			if (json.HasMember("UseDelay"))
				TimerConfig.UseDelay = json["UseDelay"].GetBool();
			if (json.HasMember("ExecutionCycle"))
				TimerConfig.DelayExecutionCycle = json["ExecutionCycle"].GetInt();
			if (json.HasMember("MarkVar") && json["MarkVar"].IsObject())
				TimerConfig.MarkVar.Parse(json["MarkVar"]);
			if (json.HasMember("StartConditions"))
				TimerConfig.StartConditions = (Project::TimerStartCond)json["StartConditions"].GetInt();
			if (json.HasMember("StartVar") && json["StartVar"].IsObject())
				TimerConfig.StartVar.Parse(json["StartVar"]);
			if (json.HasMember("IsEqualStartConditionsNum"))
				TimerConfig.UseValueEqual = json["IsEqualStartConditionsNum"].GetBool();
			if (json.HasMember("StartConditionsNum"))
				TimerConfig.ComPareValue = json["StartConditionsNum"].GetDouble();
			if (json.HasMember("DropDownStartConditions"))
				TimerConfig.BitTriggerCond = json["DropDownStartConditions"].GetInt();
			if (json.HasMember("EndConditions"))
				TimerConfig.EndConditions = (Project::TimerStopCond)json["EndConditions"].GetInt();
			if (json.HasMember("EndVar") && json["EndVar"].IsObject())
				TimerConfig.EndVar.Parse(json["EndVar"]);
			if (json.HasMember("DropDownEndConditions"))
				TimerConfig.BitEndCond = json["DropDownEndConditions"].GetInt();
			if (json.HasMember("PreconditionConstant"))
				TimerConfig.PreconditionConstant = json["PreconditionConstant"].GetInt();
			if (json.HasMember("PreconditionVar") && json["PreconditionVar"].IsObject())
				TimerConfig.PreconditionVar.Parse(json["PreconditionVar"]);
			if (json.HasMember("PresetTimeNotifyCondition"))
				TimerConfig.NotifyMode = json["PresetTimeNotifyCondition"].GetInt();
			if (json.HasMember("PresetTimeNotifyVar") && json["PresetTimeNotifyVar"].IsObject())
				TimerConfig.PresetTimeNotifyVar.Parse(json["PresetTimeNotifyVar"]);
			if (json.HasMember("AlreadTimeVar") && json["AlreadTimeVar"].IsObject())
				TimerConfig.AlreadTimeVar.Parse(json["AlreadTimeVar"]);
			if (json.HasMember("UseReset"))
				TimerConfig.UseReset = json["UseReset"].GetBool();
			if (json.HasMember("ResetVar") && json["ResetVar"].IsObject())
				TimerConfig.ResetVar.Parse(json["ResetVar"]);
			if (json.HasMember("ResetCondition"))
				TimerConfig.ResetCond = json["ResetCondition"].GetInt();
		}
	}
}
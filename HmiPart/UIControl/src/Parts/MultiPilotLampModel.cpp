/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : MultiPilotLampModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-27
 * Descript : 解析json，关于多状态指示灯绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "MultiPilotLampModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void MultiPilotLampModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/MultiStatusLamp.json", "r");
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
			LOG_INFO_("MultiPilotLampModel Parse Json\n");
			MPLampconfig.InitBaseData(json);

			if (json.HasMember("DataVarType"))
				MPLampconfig.DataVarType = json["DataVarType"].GetInt();
			if (json.HasMember("BitNum"))
				MPLampconfig.BitNum = json["BitNum"].GetInt();
			if (json.HasMember("StatusCnt"))
				MPLampconfig.StatusCnt = json["StatusCnt"].GetInt();
			if (json.HasMember("DataFormat"))
				MPLampconfig.DataFormat = json["DataFormat"].GetInt();

			if (json.HasMember("StateConds") && json["StateConds"].IsArray())
			{
				vector<Project::StatusCondition>().swap(MPLampconfig.Action);
				Value &action = json["StateConds"];
				for (unsigned i = 0; i < action.Size(); i++)
				{
					Project::StatusCondition tmpStatus;
					if (action[i].HasMember("Type"))
						tmpStatus.Type = action[i]["Type"].GetInt();
					if (action[i].HasMember("ConditionA"))
						tmpStatus.ConditionA = (Project::Conditions)action[i]["ConditionA"].GetInt();
					if (action[i].HasMember("ConditionB"))
						tmpStatus.ConditionB = (Project::Conditions)action[i]["ConditionB"].GetInt();
					if (action[i].HasMember("LogicCondition"))
						tmpStatus.LogicCondition = (Project::Logic)action[i]["LogicCondition"].GetInt();
					if (action[i].HasMember("MovA") && action[i]["MovA"].IsObject())
					{
						if (action[i]["MovA"].HasMember("ManualUsed"))
							tmpStatus.DataA.Mode = action[i]["MovA"]["ManualUsed"].GetInt();
						if (action[i]["MovA"].HasMember("Num"))
							tmpStatus.DataA.Value = action[i]["MovA"]["Num"].GetDouble();
						if (action[i]["MovA"].HasMember("VarIdRef") && action[i]["MovA"]["VarIdRef"].IsObject())
							tmpStatus.DataA.RegVar.Parse(action[i]["MovA"]["VarIdRef"]);
					}
					if (action[i].HasMember("MovB") && action[i]["MovB"].IsObject())
					{
						if (action[i]["MovB"].HasMember("ManualUsed"))
							tmpStatus.DataB.Mode = action[i]["MovB"]["ManualUsed"].GetInt();
						if (action[i]["MovB"].HasMember("Num"))
							tmpStatus.DataB.Value = action[i]["MovB"]["Num"].GetDouble();
						if (action[i]["MovB"].HasMember("VarIdRef") && action[i]["MovB"]["VarIdRef"].IsObject())
							tmpStatus.DataB.RegVar.Parse(action[i]["MovB"]["VarIdRef"]);
					}
					if (action[i].HasMember("Offset"))
						tmpStatus.Offset = action[i]["Offset"].GetInt();
					if (action[i].HasMember("Trigger"))
						tmpStatus.Trigger = action[i]["Trigger"].GetInt();
					if (action[i].HasMember("Flash"))
						tmpStatus.Flash = action[i]["Flash"].GetBool();
					if (action[i].HasMember("Speed"))
						tmpStatus.Speed = action[i]["Speed"].GetFloat();
					if (action[i].HasMember("Key") && action[i]["Key"].IsObject())
						tmpStatus.StatusKey.Parse(action[i]["Key"]);
					if (action[i].HasMember("Css") && action[i]["Css"].IsObject())
						tmpStatus.Css.Parse(action[i]["Css"]);
					MPLampconfig.Action.push_back(tmpStatus);
				}
			}
			if (json.HasMember("ErrState") && json["ErrState"].IsObject())
			{
				if (json["ErrState"].HasMember("Type"))
					MPLampconfig.ErrState.Type =json["ErrState"]["Type"].GetInt();
				if (json["ErrState"].HasMember("ConditionA"))
					MPLampconfig.ErrState.ConditionA = (Project::Conditions)json["ErrState"]["ConditionA"].GetInt();
				if (json["ErrState"].HasMember("ConditionB"))
					MPLampconfig.ErrState.ConditionB = (Project::Conditions)json["ErrState"]["ConditionB"].GetInt();
				if (json["ErrState"].HasMember("LogicCondition"))
					MPLampconfig.ErrState.LogicCondition = (Project::Logic)json["ErrState"]["LogicCondition"].GetInt();
				if (json["ErrState"].HasMember("MovA") && json["ErrState"]["MovA"].IsObject())
				{
					if (json["ErrState"]["MovA"].HasMember("ManualUsed"))
						MPLampconfig.ErrState.DataA.Mode = json["ErrState"]["MovA"]["ManualUsed"].GetInt();
					if (json["ErrState"]["MovA"].HasMember("Num"))
						MPLampconfig.ErrState.DataA.Value = json["ErrState"]["MovA"]["Num"].GetDouble();
					if (json["ErrState"]["MovA"].HasMember("VarIdRef") && json["ErrState"]["MovA"]["VarIdRef"].IsObject())
						MPLampconfig.ErrState.DataA.RegVar.Parse(json["ErrState"]["MovA"]["VarIdRef"]);
				}
				if (json["ErrState"].HasMember("MovB") && json["ErrState"]["MovB"].IsObject())
				{
					if (json["ErrState"]["MovB"].HasMember("ManualUsed"))
						MPLampconfig.ErrState.DataB.Mode = json["ErrState"]["MovB"]["ManualUsed"].GetInt();
					if (json["ErrState"]["MovB"].HasMember("Num"))
						MPLampconfig.ErrState.DataB.Value = json["ErrState"]["MovB"]["Num"].GetDouble();
					if (json["ErrState"]["MovB"].HasMember("VarIdRef") && json["ErrState"]["MovB"]["VarIdRef"].IsObject())
						MPLampconfig.ErrState.DataB.RegVar.Parse(json["ErrState"]["MovB"]["VarIdRef"]);
				}
				if (json["ErrState"].HasMember("Offset"))
					MPLampconfig.ErrState.Offset = json["ErrState"]["Offset"].GetInt();
				if (json["ErrState"].HasMember("Trigger"))
					MPLampconfig.ErrState.Trigger = json["ErrState"]["Trigger"].GetInt();
				if (json["ErrState"].HasMember("Flash"))
					MPLampconfig.ErrState.Flash = json["ErrState"]["Flash"].GetBool();
				if (json["ErrState"].HasMember("Speed"))
					MPLampconfig.ErrState.Speed = json["ErrState"]["Speed"].GetFloat();
				if (json["ErrState"].HasMember("Key") && json["ErrState"]["Key"].IsObject())
					MPLampconfig.ErrState.StatusKey.Parse(json["ErrState"]["Key"]);
				if (json["ErrState"].HasMember("Css") && json["ErrState"]["Css"].IsObject())
					MPLampconfig.ErrState.Css.Parse(json["ErrState"]["Css"]);
			}
			if (json.HasMember("InputErr"))
				MPLampconfig.InputErr = json["InputErr"].GetInt();
			if (json.HasMember("ErrNoticeVarRef") && json["ErrNoticeVarRef"].IsObject())
				MPLampconfig.InputErrVarRef.Parse(json["ErrNoticeVarRef"]);
			if (MPLampconfig.InputErrVarRef != Project::DataVarId::NullId)
				MPLampconfig.UseErrNotice = true;
			else
				MPLampconfig.UseErrNotice = false;

			if (json.HasMember("ReadVarRefs") && json["ReadVarRefs"].IsArray())
				Project::DataVarId::Parse(MPLampconfig.RegVars, json["ReadVarRefs"]);
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				MPLampconfig.Text.Parse(json["Txt"]);
		}
	}

}
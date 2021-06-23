/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : AlarmInfoRes.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 报警信息json解析
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "AlarmInfoRes.h"
namespace Project
{
	void AlarmInfoRes::Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("AlarmGroupNo"))
			GroupNo = jsonObj["AlarmGroupNo"].GetInt();
		if (jsonObj.HasMember("GroupName"))
			GroupName = jsonObj["GroupName"].GetInt();
		if (jsonObj.HasMember("AlarmGroupSecName") && !jsonObj["AlarmGroupSecName"].IsNull())
			GroupSecName = jsonObj["AlarmGroupSecName"].GetString();
		if (jsonObj.HasMember("Grade"))
			Category = (AlarmLevel)jsonObj["Grade"].GetInt();
		if (jsonObj.HasMember("RegisterType"))
			RegType = jsonObj["RegisterType"].GetInt();
		if (jsonObj.HasMember("ReadAddress") && jsonObj["ReadAddress"].IsObject())
			ReadRegVar.Parse(jsonObj["ReadAddress"]);
		if (jsonObj.HasMember("TriCondition"))
		{
			if (RegType)
				BtyeCondition = (Conditions)jsonObj["TriCondition"].GetInt();
			else
				BitCondition = (BitConditions)jsonObj["TriCondition"].GetInt();
		}

		if (jsonObj.HasMember("ConditionValue"))
			ComPareValue = jsonObj["ConditionValue"].GetFloat();
		if (jsonObj.HasMember("ConditionVarId") && jsonObj["ConditionVarId"].IsObject())
			ComPareVar.Parse(jsonObj["ConditionVarId"]);
		if (jsonObj.HasMember("Res") && jsonObj["Res"].IsObject())
			Text.Parse(jsonObj["Res"]);
		if (jsonObj.HasMember("TrrigerBeeper"))
			UseBeep = jsonObj["TrrigerBeeper"].GetInt();
		if (jsonObj.HasMember("BeepTimeOut"))
			BeepTimeOut = jsonObj["BeepTimeOut"].GetInt();
		if (jsonObj.HasMember("IsPopwin"))
			UseAlarmPopWin = jsonObj["IsPopwin"].GetBool();
		else
			UseAlarmPopWin = false;
		if (jsonObj.HasMember("PopWinNo"))
			PopWinNo = jsonObj["PopWinNo"].GetInt();
		if (jsonObj.HasMember("PopMode"))
			PopMode = jsonObj["PopMode"].GetInt();
		if (jsonObj.HasMember("PopTime"))
			PopTime = jsonObj["PopTime"].GetInt();
		if (jsonObj.HasMember("OffwinAlarmFinished"))
			IsCloseWin = jsonObj["OffwinAlarmFinished"].GetBool();
		else
			IsCloseWin = false;

		if (jsonObj.HasMember("AlarmContent"))
			Context.Parse(jsonObj["AlarmContent"].GetString());
		if (jsonObj.HasMember("Contexts") && jsonObj["Contexts"].IsArray())
		{
			Context.Parse(jsonObj["Contexts"]);
		}


	}

	void AlarmInfoRes::Parse(std::vector<AlarmInfoRes>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<AlarmInfoRes>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::AlarmInfoRes res;
				res.Parse(jsonObj[i]);
				vector.push_back(res);
			}
		}
	}
	void AlarmInfoContext::Parse(rapidjson::Value & jsonObj)
	{
		std::vector<AlarmMonitorUnit>().swap(MonitorUnits);
		if (!jsonObj.IsArray())
			return;
		//存入监控列表
		for (int i = 0; i < jsonObj.Size(); i++)
		{
			AlarmMonitorUnit unt;
			unt.Parse(jsonObj[i]);
			MonitorUnits.push_back(unt);
		}

		//校验监控列表
		for (auto itorStr = AlarmContentList.begin(); itorStr != AlarmContentList.end(); ++itorStr)
		{
			if (itorStr->second == -2)
				continue;
			bool ckPs = false;
			for (int index = 0; index < MonitorUnits.size(); ++index)
			{
				if (0 == MonitorUnits[index].VariableName.compare(itorStr->first))
				{
					itorStr->second = index;
					ckPs = true;
					break;
				}
			}
			if (ckPs == false)
				itorStr->second = -2;
		}
	}
	void AlarmInfoContext::Parse(const char* StrContent)
	{
		std::vector<std::pair<std::string, int>>().swap(AlarmContentList);
		std::string tempStr(StrContent);
		int ls = 0, rs = 0, preidx = 0;
		while (1)
		{
			ls = tempStr.find_first_of('{');
			if (ls == -1)
			{
				AlarmContentList.push_back(std::make_pair(tempStr, -2));
				break;
			}
			else
			{
				rs = tempStr.find_first_of('}');
				if (rs == -1)
				{
					AlarmContentList.push_back(std::make_pair(tempStr, -2));
					break;
				}
				AlarmContentList.push_back(std::make_pair(tempStr.substr(preidx, ls - preidx), -2));
				AlarmContentList.push_back(std::make_pair(tempStr.substr(ls + 1, rs - ls - 1), -1));
				tempStr = tempStr.substr(rs + 1);
			}
		}
	}
	void AlarmMonitorUnit::Parse(rapidjson::Value & jsonObj)
	{
		if (!jsonObj.HasMember("monitorAddrUnit"))
		{
			return;
		}
		if (jsonObj["monitorAddrUnit"].IsNull())
		{
			MonitorType = 0;
			if (jsonObj.HasMember("Name") && jsonObj["Name"].IsString())
			{
				VariableName = jsonObj["Name"].GetString();
			}
			if (jsonObj.HasMember("Varstring") && jsonObj["Varstring"].IsObject())
			{
				rapidjson::Value& jsVS = jsonObj["Varstring"];
				if (jsVS.HasMember("SrcIdRef") && jsVS["SrcIdRef"].IsObject())
					MonitorAddrRef.Parse(jsVS["SrcIdRef"]);

				if (jsVS.HasMember("Items") && jsVS["Items"].IsArray())
				{
					Project::VarStringStyle tmpstyle;
					vector<Project::VarStringStyle>().swap(StatusStyle);
					for (size_t i = 0; i < jsVS["Items"].Size(); i++)
					{
						if (jsVS["Items"][i].HasMember("SrcId"))
							tmpstyle.SrcId = jsVS["Items"][i]["SrcId"].GetDouble();
						if (jsVS["Items"][i].HasMember("Txt") && jsVS["Items"][i]["Txt"].IsObject())
							tmpstyle.Text.Parse(jsVS["Items"][i]["Txt"]);
						if (jsVS["Items"][i].HasMember("Css") && jsVS["Items"][i]["Css"].IsObject())
							tmpstyle.Css.Parse(jsVS["Items"][i]["Css"]);
						if (jsVS["Items"][i].HasMember("Border") && jsVS["Items"][i]["Border"].IsObject())
							tmpstyle.BorderStyle.Parse(jsVS["Items"][i]["Border"]);
						StatusStyle.push_back(tmpstyle);
					}
				}

			}
			

		}
		else
		{
			MonitorType = 1;
			if (jsonObj["monitorAddrUnit"].IsObject())
			{
				rapidjson::Value& jsAU = jsonObj["monitorAddrUnit"];
				if (jsAU.HasMember("VariableName") && jsAU["VariableName"].IsString())
				{
					VariableName = jsAU["VariableName"].GetString();
				}
				if (jsAU.HasMember("MonitorAddrRef") && jsAU["MonitorAddrRef"].IsObject())
					MonitorAddrRef.Parse(jsAU["MonitorAddrRef"]);
				if (jsAU.HasMember("AddrType") && jsAU["AddrType"].IsInt())
				{
					AddrType = jsAU["AddrType"].GetInt();
				}
				if (jsAU.HasMember("IsPreLead") && jsAU["IsPreLead"].IsBool())
				{
					IsPreLead = jsAU["IsPreLead"].GetBool();
				}
				if (jsAU.HasMember("Intgerdigits") && jsAU["Intgerdigits"].IsInt())
				{
					Intgerdigits = jsAU["Intgerdigits"].GetInt();
				}
				if (jsAU.HasMember("Decimaldigits") && jsAU["Decimaldigits"].IsInt())
				{
					Decimaldigits = jsAU["Decimaldigits"].GetInt();
				}

				if (jsAU.HasMember("EncodeMode") && jsAU["EncodeMode"].IsInt())
				{
					EncodeMode = jsAU["EncodeMode"].GetInt();
				}
			}
		}
	}
}
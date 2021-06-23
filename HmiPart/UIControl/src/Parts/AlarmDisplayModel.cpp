#include "stdafx.h"
#include "AlarmDisplayModel.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "Logger.h"

using namespace rapidjson;
using namespace std;
namespace UI
{
	void AlarmDisplayModel::InitTestData() {
		AlarmDisConfig.CtrlName = "AlarmDisplayControl";
	}

	void AlarmDisplayModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/AlarmDisplay.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("AlarmDisplayModel Parse Json\n");
			AlarmDisConfig.InitBaseData(json);

			if (json.HasMember("GroupScopeFrom"))
				AlarmDisConfig.StartGroupName = json["GroupScopeFrom"].GetInt();
			if (json.HasMember("GroupScopeTo"))
				AlarmDisConfig.EndGroupName = json["GroupScopeTo"].GetInt();
			if (json.HasMember("IsUseTitle"))
				AlarmDisConfig.UseTitle = json["IsUseTitle"].GetBool();
			if (json.HasMember("IsSameStringStyle"))
				AlarmDisConfig.UseSameStyle = json["IsSameStringStyle"].GetBool();
			if (json.HasMember("Res"))
 				AlarmDisConfig.Title.Parse(json["Res"]);
			if (json.HasMember("TitleStringStyle"))
				AlarmDisConfig.TitleStringStyle.Parse(json["TitleStringStyle"]);
			if (json.HasMember("ListStringStyle"))
				AlarmDisConfig.ListStringStyle.Parse(json["ListStringStyle"]);
			if (json.HasMember("ListTitleStyle"))
				AlarmDisConfig.ListTitleStyle.Parse(json["ListTitleStyle"]);
			if (json.HasMember("SameStringStyle"))
				AlarmDisConfig.SameStringStyle.Parse(json["SameStringStyle"]);
			if (json.HasMember("AlarmMode"))
				AlarmDisConfig.AlarmMode = (Project::AlarmDisMode)json["AlarmMode"].GetInt();
			if (json.HasMember("AlarmCount"))
				AlarmDisConfig.AlarmMaxNum = json["AlarmCount"].GetInt();
			if (json.HasMember("AlarmPageCount"))
				AlarmDisConfig.PageNum = json["AlarmPageCount"].GetInt();
			if (json.HasMember("IsUseMultiLanguage"))
				AlarmDisConfig.IsUseMultiLanguage = json["IsUseMultiLanguage"].GetBool();
			if (json.HasMember("MultiLanConfigLst") && json["MultiLanConfigLst"].IsArray())
			{
				std::vector<Project::ColInfo>().swap(AlarmDisConfig.AlarmOptions);
				for (unsigned i = 0; i < json["MultiLanConfigLst"].Size(); i++)
				{
					Project::ColInfo tmpInfo;
					if (json["MultiLanConfigLst"][i].HasMember("IsSelect"))
					{
						if (json["MultiLanConfigLst"][i]["IsSelect"].GetBool())
						{
							if (json["MultiLanConfigLst"][i].HasMember("Project") && !json["MultiLanConfigLst"][i]["Project"].IsNull())
								tmpInfo.ProjectName = json["MultiLanConfigLst"][i]["Project"].GetString();
							if (json["MultiLanConfigLst"][i].HasMember("Describe") && !json["MultiLanConfigLst"][i]["Describe"].IsNull())
								tmpInfo.DescribeName = json["MultiLanConfigLst"][i]["Describe"].GetString();
							if (json["MultiLanConfigLst"][i].HasMember("Res") && json["MultiLanConfigLst"][i]["Res"].IsObject())
								tmpInfo.TitleDescribe.Parse(json["MultiLanConfigLst"][i]["Res"]);
							if (json["MultiLanConfigLst"][i].HasMember("ColumnWidth"))
								tmpInfo.ColWidth = json["MultiLanConfigLst"][i]["ColumnWidth"].GetInt();
							AlarmDisConfig.AlarmOptions.push_back(tmpInfo);
						}
					}
				}
			}
			if (json.HasMember("TitleHeight"))
				AlarmDisConfig.TitleHeight = json["TitleHeight"].GetInt();
			if (json.HasMember("SubTitleHeight"))
				AlarmDisConfig.EachColTitleHeight = json["SubTitleHeight"].GetInt();
			if (json.HasMember("SortMode"))
				AlarmDisConfig.SortMode = json["SortMode"].GetInt();
			if (json.HasMember("TimeFormat"))
				AlarmDisConfig.Timemode = (Project::TimeMode)json["TimeFormat"].GetInt();
			if (json.HasMember("DateFormat"))
				AlarmDisConfig.Datemode = (Project::DateMode)json["DateFormat"].GetInt();
			if (json.HasMember("IsAlarmDisplayTop"))
				AlarmDisConfig.TopNotRecover = json["IsAlarmDisplayTop"].GetBool();
			if (json.HasMember("IsEnableConfirm"))
				AlarmDisConfig.UseConfirmMode = json["IsEnableConfirm"].GetBool();
			if (json.HasMember("ConfirmMode"))
				AlarmDisConfig.CMode = (Project::ConfirmMode)json["ConfirmMode"].GetInt();
			if (json.HasMember("IsInfoHideCtrl"))
				AlarmDisConfig.IsInfoHideCtrl = json["IsInfoHideCtrl"].GetBool();
			if (json.HasMember("InfoHideVarId") && json["InfoHideVarId"].IsObject())
				AlarmDisConfig.HideVar.Parse(json["InfoHideVarId"]);
			if (json.HasMember("AlarmVarId") && json["AlarmVarId"].IsObject())
				AlarmDisConfig.AlarmVarId.Parse(json["AlarmVarId"]);
			if (json.HasMember("TableBgcolor"))
				AlarmDisConfig.BackColor = json["TableBgcolor"].GetInt();
			if (json.HasMember("TitleBgcolor"))
				AlarmDisConfig.TitleBgColor = json["TitleBgcolor"].GetInt();
			if (json.HasMember("IsBorder"))
				AlarmDisConfig.UseFrame = json["IsBorder"].GetBool();
			if (json.HasMember("BorderStyle") && json["BorderStyle"].IsObject())
				AlarmDisConfig.FrameStyle.Parse(json["BorderStyle"]);
			if (json.HasMember("IsGrid"))
				AlarmDisConfig.UseGrid = json["IsGrid"].GetBool();
			if (json.HasMember("GridStyle") && json["GridStyle"].IsObject())
				AlarmDisConfig.GridStyle.Parse(json["GridStyle"]);
			if (json.HasMember("TriggerStrBgColor"))
				AlarmDisConfig.AlarmTextColor = json["TriggerStrBgColor"].GetInt();
			if (json.HasMember("TriggerBgColor"))
				AlarmDisConfig.AlarmBackColor = json["TriggerBgColor"].GetInt();
			if (json.HasMember("IsAlarmConfirm"))
				AlarmDisConfig.UseConfirm = json["IsAlarmConfirm"].GetBool();
			if (json.HasMember("AlarmConfirmStrBgColor"))
				AlarmDisConfig.ConfirmTextColor = json["AlarmConfirmStrBgColor"].GetInt();
			if (json.HasMember("AlarmConfirmBgColor"))
				AlarmDisConfig.ConfirmBackColor = json["AlarmConfirmBgColor"].GetInt();
			if (json.HasMember("IsAlarmRestore"))
				AlarmDisConfig.UseRecover = json["IsAlarmRestore"].GetBool();
			if (json.HasMember("AlarmRestoreStrBgColor"))
				AlarmDisConfig.RecoverTextColor = json["AlarmRestoreStrBgColor"].GetInt();
			if (json.HasMember("AlarmRestoreBgColor"))
				AlarmDisConfig.RecoverBackColor = json["AlarmRestoreBgColor"].GetInt();
			if (json.HasMember("IsEnableSearch"))
				AlarmDisConfig.EnableSearch = json["IsEnableSearch"].GetBool();
			if (json.HasMember("SearchMode"))
				AlarmDisConfig.SearchMode = (Project::AlarmSeekMode)json["SearchMode"].GetInt();
			
			if (json.HasMember("SearchModeVarId") && json["SearchModeVarId"].IsObject())
				AlarmDisConfig.SeekModeVar.Parse(json["SearchModeVarId"]);
			if (json.HasMember("SearchCtrlVarId") && json["SearchCtrlVarId"].IsObject())
					AlarmDisConfig.SeekCtrlVar.Parse(json["SearchCtrlVarId"]);
			
			if (json.HasMember("SearchDate"))
				AlarmDisConfig.SearchDate = json["SearchDate"].GetInt64() * (DDWORD)1000;
			if (json.HasMember("SearchDateVarId") && json["SearchDateVarId"].IsArray())
				Project::DataVarId::Parse(AlarmDisConfig.SearchDateVarId, json["SearchDateVarId"]);

			if (json.HasMember("SearchFromTimePeriod"))
				AlarmDisConfig.SearchTimeStart = json["SearchFromTimePeriod"].GetInt64() * (DDWORD)1000;
			if (json.HasMember("SearchToTimePeriod"))
				AlarmDisConfig.SearchTimeEnd = json["SearchToTimePeriod"].GetInt64() * (DDWORD)1000;
			if (json.HasMember("SearchTimePeriodVarId") && json["SearchTimePeriodVarId"].IsArray())
				Project::DataVarId::Parse(AlarmDisConfig.SearchTimeVar, json["SearchTimePeriodVarId"]);
			
			if (json.HasMember("SearchGroup"))
				AlarmDisConfig.SearchGroup = json["SearchGroup"].GetInt();
			if (json.HasMember("SearchGroupVarId") && json["SearchGroupVarId"].IsObject())
				AlarmDisConfig.SearchGroupVar.Parse(json["SearchGroupVarId"]);

			if (json.HasMember("SearchNo"))
				AlarmDisConfig.SearchNo = json["SearchNo"].GetInt();
			if (json.HasMember("SearchNoVarId") && json["SearchNoVarId"].IsObject())
				AlarmDisConfig.SearchNoVar.Parse(json["SearchNoVarId"]);

			if (json.HasMember("SearchLevel"))
				AlarmDisConfig.SearchLevel = (Project::AlarmLevel)json["SearchLevel"].GetInt();
			if (json.HasMember("SearchLevelVarId") && json["SearchLevelVarId"].IsObject())
				AlarmDisConfig.SearchLevelVar.Parse(json["SearchLevelVarId"]);

			if (AlarmDisConfig.SearchMode == Project::AlarmSeekMode::AlarmSeekREG)
				AlarmDisConfig.RegMode = true;
			if (json.HasMember("SearchRegVarId") && json["SearchRegVarId"].IsArray())
				Project::DataVarId::Parse(AlarmDisConfig.SearchRegVarId, json["SearchRegVarId"]);
		}
	}
}
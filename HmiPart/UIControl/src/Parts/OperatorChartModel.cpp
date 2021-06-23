#include "stdafx.h"
#include "OperatorChartModel.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void OperatorChartModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/ReportForm.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("OperatorChartModel Parse Json\n");
			OperatorConfig.InitBaseData(json);
			if (json.HasMember("DisplayItems") && json["DisplayItems"].IsArray())
			{
				vector<Project::DisplayReportLst>().swap(OperatorConfig.DisplayItems);
				for (size_t i = 0; i < json["DisplayItems"].Size(); i++)
				{
					if (json["DisplayItems"][i].HasMember("Ischecked") && json["DisplayItems"][i]["Ischecked"].GetBool())
					{
						Project::DisplayReportLst tmpinfo;
						if (json["DisplayItems"][i].HasMember("ItemName") && !json["DisplayItems"][i]["ItemName"].IsNull())
							tmpinfo.ProjectName = json["DisplayItems"][i]["ItemName"].GetString();
						if (json["DisplayItems"][i].HasMember("DescriptionRes") && json["DisplayItems"][i]["DescriptionRes"].IsObject())
							tmpinfo.TitleDescribe.Parse(json["DisplayItems"][i]["DescriptionRes"]);
						if (json["DisplayItems"][i].HasMember("ColumnSpacing"))
							tmpinfo.ColWidth = json["DisplayItems"][i]["ColumnSpacing"].GetInt();
						OperatorConfig.DisplayItems.push_back(tmpinfo);
					}
				}
			}
			if (json.HasMember("RecordVarId") && json["RecordVarId"].IsObject())
				OperatorConfig.RecordVarId.Parse(json["RecordVarId"]);
			if (json.HasMember("UseMultiLanguage"))
				OperatorConfig.UseMultiLanguage = json["UseMultiLanguage"].GetBool();
			if (json.HasMember("SortMode"))
				OperatorConfig.SortMode = json["SortMode"].GetInt();
			
			if (json.HasMember("TimeFormate"))
				OperatorConfig.TimeFormate = (Project::TimeMode)json["TimeFormate"].GetInt();
			if (json.HasMember("DateFormate"))
				OperatorConfig.DateFormate = (Project::DateMode)json["DateFormate"].GetInt();
			if (json.HasMember("RowHeight"))
				OperatorConfig.RowHeight = json["RowHeight"].GetInt();
			if (json.HasMember("UsedTitle"))
				OperatorConfig.UsedTitle = json["UsedTitle"].GetBool();
			if (json.HasMember("TitleStringRes"))
				OperatorConfig.TitleStringRes.Parse(json["TitleStringRes"]);
			if (json.HasMember("TitleHeight"))
				OperatorConfig.TitleHeight = json["TitleHeight"].GetInt();
			if (json.HasMember("IsTitleRecordItemUseSameStyle"))
				OperatorConfig.IsUseSameStyle = json["IsTitleRecordItemUseSameStyle"].GetBool();
			if (json.HasMember("TitleStringStyle") && json["TitleStringStyle"].IsObject())
				OperatorConfig.TitleStringStyle.Parse(json["TitleStringStyle"]);
			if (json.HasMember("RecordItemStringStyle") && json["RecordItemStringStyle"].IsObject())
				OperatorConfig.RecordItemStringStyle.Parse(json["RecordItemStringStyle"]);
	
			if (json.HasMember("GridBackgroundColor"))
				OperatorConfig.TableBgColor = json["GridBackgroundColor"].GetInt();
			if (json.HasMember("TitleBackgroundColor"))
				OperatorConfig.TitleBgColor = json["TitleBackgroundColor"].GetInt();
			if (json.HasMember("IsDisplayBorder"))
				OperatorConfig.IsUseBorder = json["IsDisplayBorder"].GetBool();
			if (json.HasMember("BorderStyle"))
				OperatorConfig.TableBorderStyle.Parse(json["BorderStyle"]);
			if (json.HasMember("GridStyle"))
				OperatorConfig.GridBorderStyle.Parse(json["GridStyle"]);
			if (json.HasMember("IsDisplayGrid"))
				OperatorConfig.IsRowDividLine = OperatorConfig.IsColDividLine = json["IsDisplayGrid"].GetBool();
			//if (json.HasMember("IsRowDividLine"))
			//	OperatorConfig.IsRowDividLine = json["IsRowDividLine"].GetBool();
			//if (json.HasMember("IsColDividLine"))
			//	OperatorConfig.IsColDividLine = json["IsColDividLine"].GetBool();

			if (json.HasMember("IsEnableSearch"))
				OperatorConfig.EnableSearch = json["IsEnableSearch"].GetBool();
			if (json.HasMember("SearchMode"))
				OperatorConfig.SearchMode = (Project::OperatorSeekMode)json["SearchMode"].GetInt();
			if (json.HasMember("SearchModeVarId") && json["SearchModeVarId"].IsObject())
				OperatorConfig.SeekModeVar.Parse(json["SearchModeVarId"]);
			if (json.HasMember("SearchCtrlVarId") && json["SearchCtrlVarId"].IsObject())
				OperatorConfig.SeekCtrlVar.Parse(json["SearchCtrlVarId"]);
			if (OperatorConfig.SearchMode == Project::OperatorSeekMode::OperatorByReg)
				OperatorConfig.RegMode = true;
			if (json.HasMember("Searchreg") && json["Searchreg"].IsArray())
				Project::DataVarId::Parse(OperatorConfig.SearchRegVarId, json["Searchreg"]);

			if (json.HasMember("SearchDate"))
				OperatorConfig.SearchDate = json["SearchDate"].GetInt64()* (DDWORD)1000;
			if (json.HasMember("SearchDateVarId") && json["SearchDateVarId"].IsArray())
				Project::DataVarId::Parse(OperatorConfig.SearchDateVarId, json["SearchDateVarId"]);
			if (json.HasMember("SearchFromTimePeriod"))
				OperatorConfig.SearchTimeStart = json["SearchFromTimePeriod"].GetInt64() * (DDWORD)1000;
			if (json.HasMember("SearchToTimePeriod"))
				OperatorConfig.SearchTimeEnd = json["SearchToTimePeriod"].GetInt64()* (DDWORD)1000;
			if (json.HasMember("SearchTimePeriodVarId") && json["SearchTimePeriodVarId"].IsArray())
				Project::DataVarId::Parse(OperatorConfig.SearchTimeVar, json["SearchTimePeriodVarId"]);
			if (json.HasMember("SearchUserName") && !json["SearchUserName"].IsNull())
				OperatorConfig.SearchKey = json["SearchUserName"].GetString();
			if (json.HasMember("SearchUserNameDataVar") && json["SearchUserNameDataVar"].IsObject())
				OperatorConfig.SearchKeyVar.Parse(json["SearchUserNameDataVar"]);
		}
	}
}
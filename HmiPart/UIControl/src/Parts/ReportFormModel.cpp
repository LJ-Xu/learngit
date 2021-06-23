#include "stdafx.h"
#include "ReportFormModel.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void ReportFormModel::InitData(std::string jstr)
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
			LOG_INFO_("ReportFormModel Parse Json\n");
			ReportConfig.InitBaseData(json);
			if (json.HasMember("SimpleGroupName"))
				ReportConfig.SimpleGroupName = json["SimpleGroupName"].GetInt();
			if (json.HasMember("SimpleGroupNo"))
				ReportConfig.SimpleGroupNo = json["SimpleGroupNo"].GetInt();
			if (json.HasMember("SampleVarId") && json["SampleVarId"].IsObject())
				ReportConfig.SampleVarId.Parse(json["SampleVarId"]);
			if(json.HasMember("ReportChannelLst") && json["ReportChannelLst"].IsArray())
				Project::ReportChannel::Parse(ReportConfig.ReportChannelLst, json["ReportChannelLst"],
					ReportConfig.SimpleGroupName, ReportConfig.SimpleGroupNo);
			if (json.HasMember("TableAttributes") && json["TableAttributes"].IsArray())
				Project::DisplayReportLst::Parse(ReportConfig.InfoLst,json["TableAttributes"],
					ReportConfig.SimpleGroupName, ReportConfig.SimpleGroupNo);
			if (json.HasMember("IsViewSerialNo"))
				ReportConfig.IsViewSerialNo = json["IsViewSerialNo"].GetBool();
			if (json.HasMember("SerialNum"))
				ReportConfig.SerialNum = json["SerialNum"].GetInt();
			if (json.HasMember("SerialColor"))
				ReportConfig.SerialColor = json["SerialColor"].GetInt();
			if (json.HasMember("IsViewDate"))
				ReportConfig.IsViewDate = json["IsViewDate"].GetBool();
			if (json.HasMember("DateColor"))
				ReportConfig.DateColor = json["DateColor"].GetInt();
			if (json.HasMember("DateFormat"))
				ReportConfig.DateStyle = (Project::DateMode)json["DateFormat"].GetInt();
			if (json.HasMember("IsViewTime"))
				ReportConfig.IsViewTime = json["IsViewTime"].GetBool();
			if (json.HasMember("TimeColor"))
				ReportConfig.TimeColor = json["TimeColor"].GetInt();
			if (json.HasMember("TimeFormat"))
				ReportConfig.TimeStyle = (Project::TimeMode)json["TimeFormat"].GetInt();
			if (json.HasMember("MaxRows"))
				ReportConfig.MaxRows = json["MaxRows"].GetInt();
			if (json.HasMember("PerPageRows"))
				ReportConfig.PerPageRows = json["PerPageRows"].GetInt();
			if (json.HasMember("ViewSort"))
				ReportConfig.ViewSort = json["ViewSort"].GetInt();
			if (json.HasMember("IsModifyCellValue"))
				ReportConfig.IsModifyCellValue = json["IsModifyCellValue"].GetBool();
			if (json.HasMember("IsViewTitle"))
				ReportConfig.IsViewTitle = json["IsViewTitle"].GetBool();
			if (json.HasMember("TitleHeight"))
				ReportConfig.TitleHeight = json["TitleHeight"].GetInt();
			if (json.HasMember("Res"))
				ReportConfig.TitleRes.Parse(json["Res"]);
			if (json.HasMember("ElementTitleStyle"))
				ReportConfig.ElementTitleStyle.Parse(json["ElementTitleStyle"]);
			if (json.HasMember("IsViewEachColTitle"))
				ReportConfig.IsViewEachColTitle = json["IsViewEachColTitle"].GetBool();
			if (json.HasMember("SubTitleHeight"))
				ReportConfig.EachColTitleHeight = json["SubTitleHeight"].GetInt();
			if (json.HasMember("IsUseMultiLan"))
				ReportConfig.IsUseMultiLan = json["IsUseMultiLan"].GetBool();
			if (json.HasMember("IsAutoRowColWidth"))
				ReportConfig.IsAutoRowColWidth = json["IsAutoRowColWidth"].GetBool();
			if (json.HasMember("TitleBarStyle"))
				ReportConfig.TitleBarStyle.Parse(json["TitleBarStyle"]);
			if (json.HasMember("ListFontStyle"))
				ReportConfig.ListFontStyle.Parse(json["ListFontStyle"]);
			if (json.HasMember("Appearance"))
				ReportConfig.Appearance = json["Appearance"].GetInt();
			if (json.HasMember("Key"))
				ReportConfig.Key.Parse(json["Key"]);
			if (json.HasMember("TableBgColor"))
				ReportConfig.TableBgColor = json["TableBgColor"].GetInt();
			if (json.HasMember("TitleBgColor"))
				ReportConfig.TitleBgColor = json["TitleBgColor"].GetInt();
			if (json.HasMember("IsUseBorder"))
				ReportConfig.IsUseBorder = json["IsUseBorder"].GetBool();
			if (json.HasMember("TableBorderStyle"))
				ReportConfig.TableBorderStyle.Parse(json["TableBorderStyle"]);
			if (json.HasMember("GridBorderStyle"))
				ReportConfig.GridBorderStyle.Parse(json["GridBorderStyle"]);
			if (json.HasMember("IsRowDividLine"))
				ReportConfig.IsRowDividLine = json["IsRowDividLine"].GetBool();
			if (json.HasMember("IsColDividLine"))
				ReportConfig.IsColDividLine = json["IsColDividLine"].GetBool();

			if (json.HasMember("IsEnableSearch"))
				ReportConfig.EnableSearch = json["IsEnableSearch"].GetBool();
			if (json.HasMember("SearchMode"))
				ReportConfig.SearchMode = (Project::ReportSeekMode)json["SearchMode"].GetInt();
			if (json.HasMember("SearchModeVarId") && json["SearchModeVarId"].IsObject())
				ReportConfig.SeekModeVar.Parse(json["SearchModeVarId"]);
			if (json.HasMember("SearchCtrlVarId") && json["SearchCtrlVarId"].IsObject())
				ReportConfig.SeekCtrlVar.Parse(json["SearchCtrlVarId"]);

			if (json.HasMember("SearchDate"))
				ReportConfig.SearchDate = json["SearchDate"].GetInt64()* (DDWORD)1000;
			if (json.HasMember("SearchDateVarId") && json["SearchDateVarId"].IsArray())
				Project::DataVarId::Parse(ReportConfig.SearchDateVarId, json["SearchDateVarId"]);
			if (json.HasMember("SearchFromTimePeriod"))
				ReportConfig.SearchTimeStart = json["SearchFromTimePeriod"].GetInt64() * (DDWORD)1000;
			if (json.HasMember("SearchToTimePeriod"))
				ReportConfig.SearchTimeEnd = json["SearchToTimePeriod"].GetInt64()* (DDWORD)1000;
			if (json.HasMember("SearchTimePeriodVarId") && json["SearchTimePeriodVarId"].IsArray())
				Project::DataVarId::Parse(ReportConfig.SearchTimeVar, json["SearchTimePeriodVarId"]);
			if (json.HasMember("SearchChannel"))
				ReportConfig.SearchChannelNo = json["SearchChannel"].GetInt();
			if (json.HasMember("SearchChannelVarId") && json["SearchChannelVarId"].IsObject())
				ReportConfig.SearchChannelNoVar.Parse(json["SearchChannelVarId"]);
			if (ReportConfig.SearchMode == Project::ReportSeekMode::ReportREG)
				ReportConfig.RegMode = true;
			if (json.HasMember("SearchRegVarId") && json["SearchRegVarId"].IsArray())
				Project::DataVarId::Parse(ReportConfig.SearchRegVarId, json["SearchRegVarId"]);
		}
	}
}
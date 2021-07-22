#include "RecipeChartModel.h"
#include "stdafx.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void RecipeChartModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/RecipeChart.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("RecipeChartModel Parse Json\n");
			RecipeConfig.InitBaseData(json);
			if (json.HasMember("RecipeId") && !json["RecipeId"].IsNull())
				RecipeConfig.RecipeGroupId = json["RecipeId"].GetString();
			if (json.HasMember("ArrangMode"))
				RecipeConfig.ArrangMode = json["ArrangMode"].GetInt();
			if (json.HasMember("TotalRowCount"))
				RecipeConfig.TotalRowCount = json["TotalRowCount"].GetInt();
			if (json.HasMember("PerPageRowCount"))
				RecipeConfig.PerPageRowCount = json["PerPageRowCount"].GetInt();
			if (json.HasMember("RecipeColAttrs") && json["RecipeColAttrs"].IsArray())
			{
				vector<Project::DisplayReportLst>().swap(RecipeConfig.InfoLst);
				for (size_t i = 0; i < json["RecipeColAttrs"].Size(); i++)
				{
					Project::DisplayReportLst listinfo;
					if (json["RecipeColAttrs"][i].HasMember("Title") &&
						!json["RecipeColAttrs"][i]["Title"].IsNull())
						listinfo.ProjectName = json["RecipeColAttrs"][i]["Title"].GetString();
					if (json["RecipeColAttrs"][i].HasMember("Desc") &&
						json["RecipeColAttrs"][i]["Desc"].IsObject())
						listinfo.TitleDescribe.Parse(json["RecipeColAttrs"][i]["Desc"]);
					if (json["RecipeColAttrs"][i].HasMember("Height"))
						listinfo.RowHeight = json["RecipeColAttrs"][i]["Height"].GetInt();
					if (json["RecipeColAttrs"][i].HasMember("Width"))
						listinfo.ColWidth = json["RecipeColAttrs"][i]["Width"].GetInt();
					RecipeConfig.InfoLst.push_back(listinfo);
				}
			}
			if (json.HasMember("TitleUsed"))
				RecipeConfig.TitleUsed = json["TitleUsed"].GetBool();
			if (json.HasMember("TitleHeight"))
				RecipeConfig.TitleHeight = json["TitleHeight"].GetInt();
			if (json.HasMember("TxtTitle"))
				RecipeConfig.TxtTitle.Parse(json["TxtTitle"]);
			if (json.HasMember("TitleStyle"))
				RecipeConfig.TitleStyle.Parse(json["TitleStyle"]);
			if (json.HasMember("ColTitleShown"))
				RecipeConfig.ColTitleShown = json["ColTitleShown"].GetBool();
			if (json.HasMember("ColTitleHeight"))
				RecipeConfig.EachColTitleHeight = json["ColTitleHeight"].GetInt();
			if (json.HasMember("ColTitleStyle"))
				RecipeConfig.ColTitleStyle.Parse(json["ColTitleStyle"]);
			if (json.HasMember("SerialNoShown"))
				RecipeConfig.SerialNoShown = json["SerialNoShown"].GetBool();
			if (json.HasMember("SerialStyle"))
				RecipeConfig.SerialNumStyle = json["SerialStyle"].GetInt();
			if (json.HasMember("IsUseMultiLan"))
				RecipeConfig.IsUseMultiLan = json["IsUseMultiLan"].GetBool();
			if (json.HasMember("AutoSize"))
				RecipeConfig.AutoSize = json["AutoSize"].GetBool();
			if (json.HasMember("TextStyle"))
				RecipeConfig.ListFontStyle.Parse(json["TextStyle"]);
			if (json.HasMember("AppearMode"))
				RecipeConfig.AppearMode = json["AppearMode"].GetInt();
			if (json.HasMember("Key"))
				RecipeConfig.Key.Parse(json["Key"]);
			if (json.HasMember("ColTitleBackColor"))
				RecipeConfig.ColTitleBackColor = json["ColTitleBackColor"].GetInt();
			if (json.HasMember("TitleBackColor"))
				RecipeConfig.TitleBackColor = json["TitleBackColor"].GetInt();
			if (json.HasMember("EvenBackColor"))
				RecipeConfig.EvenBackColor = json["EvenBackColor"].GetInt();
			if (RecipeConfig.EvenBackColor == -1)
			{
				RecipeConfig.ParityDiffColor = false;
				if (json.HasMember("OddBackColor"))
					RecipeConfig.TableBgColor = json["OddBackColor"].GetInt();
			}
			else
			{
				RecipeConfig.ParityDiffColor = true;
				if (json.HasMember("OddBackColor"))
					RecipeConfig.OddBackColor = json["OddBackColor"].GetInt();
			}
			if (json.HasMember("PresetBorder"))
			{
				Project::GridStyle presetBorder = (Project::GridStyle)json["PresetBorder"].GetInt();
				switch (presetBorder)
				{
				case Project::None:
				{
					RecipeConfig.IsUseBorder = false;
					RecipeConfig.IsRowDividLine = false;
					RecipeConfig.IsColDividLine = false;
					break;
				}
				case Project::Horizontal:
				{
					RecipeConfig.IsUseBorder = false;
					RecipeConfig.IsRowDividLine = true;
					RecipeConfig.IsColDividLine = false;
					break;
				}
				case Project::Vertical:
				{
					RecipeConfig.IsUseBorder = false;
					RecipeConfig.IsRowDividLine = false;
					RecipeConfig.IsColDividLine = true;
					break;
				}
				case Project::Inner:
				{
					RecipeConfig.IsUseBorder = false;
					RecipeConfig.IsRowDividLine = true;
					RecipeConfig.IsColDividLine = true;
					break;
				}
				case Project::Outer:
				{
					RecipeConfig.IsUseBorder = true;
					RecipeConfig.IsRowDividLine = false;
					RecipeConfig.IsColDividLine = false;
					break;
				}
				case Project::OuterHorizontal:
				{
					RecipeConfig.IsUseBorder = true;
					RecipeConfig.IsRowDividLine = true;
					RecipeConfig.IsColDividLine = false;
					break;
				}
				case Project::OuterVertical:
				{
					RecipeConfig.IsUseBorder = true;
					RecipeConfig.IsRowDividLine = false;
					RecipeConfig.IsColDividLine = true;
					break;
				}
				case Project::GridAll:
				{
					RecipeConfig.IsUseBorder = true;
					RecipeConfig.IsRowDividLine = true;
					RecipeConfig.IsColDividLine = true;
					break;
				}
				default:
					break;
				}
			}
			if (json.HasMember("Border"))
				RecipeConfig.TableBorderStyle.Parse(json["Border"]);
			if (json.HasMember("Border"))
				RecipeConfig.GridBorderStyle.Parse(json["Border"]);

			if (json.HasMember("FocusFontColor"))
				RecipeConfig.FocusFontColor = json["FocusFontColor"].GetInt();
			if (json.HasMember("FocusRowColor"))
				RecipeConfig.FocusRowColor = json["FocusRowColor"].GetInt();
			if (json.HasMember("FocusCellColor"))
				RecipeConfig.FocusCellColor = json["FocusCellColor"].GetInt();

			if (json.HasMember("IsEnableSearch"))
				RecipeConfig.EnableSearch = json["IsEnableSearch"].GetBool();
			if (json.HasMember("SearchMethod"))
				RecipeConfig.SearchMode = (Project::RecipeSeekMode)json["SearchMethod"].GetInt();
			if (json.HasMember("SearchMethodVarIdref") && json["SearchMethodVarIdref"].IsObject())
				RecipeConfig.SeekModeVar.Parse(json["SearchMethodVarIdref"]);
			if (json.HasMember("SearchCtrlVarIdRef") && json["SearchCtrlVarIdRef"].IsObject())
				RecipeConfig.SeekCtrlVar.Parse(json["SearchCtrlVarIdRef"]);
			if (json.HasMember("KeyWord") && !json["KeyWord"].IsNull())
				RecipeConfig.SearchKey = json["KeyWord"].GetString();
			if (json.HasMember("MovKeyword") && json["MovKeyword"].IsObject())
				RecipeConfig.SearchKeyVar.Parse(json["MovKeyword"]);
			if (json.HasMember("MovGroupNo") && json["MovGroupNo"].IsObject())
			{
				if(json["MovGroupNo"].HasMember("Num"))
					RecipeConfig.SearchData = json["MovGroupNo"]["Num"].GetDouble();
				if (json["MovGroupNo"].HasMember("VarIdRef") && json["MovGroupNo"]["VarIdRef"].IsObject())
					RecipeConfig.SearchDataVar.Parse(json["MovGroupNo"]["VarIdRef"]);

			}
			if (RecipeConfig.SearchMode == Project::RecipeSeekMode::RecipeREG)
				RecipeConfig.RegMode = true;
			if (json.HasMember("SearchVarIdRef") && json["SearchVarIdRef"].IsArray())
				Project::DataVarId::Parse(RecipeConfig.SearchRegVarId, json["SearchVarIdRef"]);

			if (json.HasMember("UpdateNotice") && json["UpdateNotice"].IsObject())
				RecipeConfig.UpdateNotice.Parse(json["UpdateNotice"]);
		}
	}
}
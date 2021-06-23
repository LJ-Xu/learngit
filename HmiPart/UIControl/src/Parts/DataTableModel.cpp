#include "stdafx.h"
#include "DataTableModel.h"
#include "StringStyle.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void DataTableModel::InitData(std::string jstr)
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
			LOG_INFO_("DataTableModel Parse Json\n");
			DataConfig.InitBaseData(json);

			if (json.HasMember("AddrContinuous"))
				DataConfig.UseContineAddr = json["AddrContinuous"].GetBool();
			if (json.HasMember("RowCnt"))
				DataConfig.RowsNum = json["RowCnt"].GetInt();
			if (json.HasMember("LinesPerPage"))
				DataConfig.PerPageRows = json["LinesPerPage"].GetInt();
			if (json.HasMember("ColumnCnt"))
				DataConfig.ColsNum = json["ColumnCnt"].GetInt();

			if (json.HasMember("TitleUsed"))
				DataConfig.UseTitleBar = json["TitleUsed"].GetBool();
			if (json.HasMember("TitleHeight"))
				DataConfig.TitleHeight = json["TitleHeight"].GetInt();
			if (json.HasMember("TitleRes") && json["TitleRes"].IsObject())
				DataConfig.TitleRes.Parse(json["TitleRes"]);

			if (json.HasMember("ColTitleUsed"))
				DataConfig.ColTitleUsed = json["ColTitleUsed"].GetBool();
			if (json.HasMember("ColTitleHeight"))
				DataConfig.ColTitleHeight = json["ColTitleHeight"].GetInt();
			if (json.HasMember("ColAttrs") && json["ColAttrs"].IsArray())
			{
				vector<Project::DisplayReportLst>().swap(DataConfig.ColInfoLst);
				for (size_t i = 0; i < json["ColAttrs"].Size(); i++)
				{
					Project::DisplayReportLst listinfo;
					if (json["ColAttrs"][i].HasMember("Content") &&
						!json["ColAttrs"][i]["Content"].IsNull())
						listinfo.ProjectName = json["ColAttrs"][i]["Content"].GetString();
					if (json["ColAttrs"][i].HasMember("Title") &&
						json["ColAttrs"][i]["Title"].IsObject())
						listinfo.TitleDescribe.Parse(json["ColAttrs"][i]["Title"]);
					if (json["ColAttrs"][i].HasMember("Height"))
						listinfo.RowHeight = json["ColAttrs"][i]["Height"].GetInt();
					if (json["ColAttrs"][i].HasMember("Width"))
						listinfo.ColWidth = json["ColAttrs"][i]["Width"].GetInt();
					DataConfig.ColInfoLst.push_back(listinfo);
				}
			}
			if (json.HasMember("SerialNoLen"))
				DataConfig.SerialNum = json["SerialNoLen"].GetInt();
			if (json.HasMember("RowNames") && json["RowNames"].IsArray())
			{
				vector<Project::StatusRes>().swap(DataConfig.StaticColName);
				for (size_t i = 0; i < json["RowNames"].Size(); i++)
				{
					if (json["RowNames"][i].IsObject())
					{
						Project::StatusRes res;
						if (json["RowNames"][i].HasMember("RowName") && json["RowNames"][i]["RowName"].IsObject())
						{
							res.Parse(json["RowNames"][i]["RowName"]);
							DataConfig.StaticColName.push_back(res);
						}
					}
				}
			}
				//Project::StatusRes::Parse(DataConfig.StaticColName, json["RowNames"]);
			if (json.HasMember("SameStyleUsed"))
				DataConfig.UseSameStyle = json["SameStyleUsed"].GetBool();
			if (json.HasMember("TitleStyle") && json["TitleStyle"].IsObject())
				DataConfig.TitleBarStyle.Parse(json["TitleStyle"]);
			if (json.HasMember("TextStyle") && json["TextStyle"].IsObject())
				DataConfig.ListFontStyle.Parse(json["TextStyle"]);
			if (json.HasMember("TitleStyle") && json["TitleStyle"].IsObject())
				DataConfig.SameFontStyle.Parse(json["TitleStyle"]);
			if(json.HasMember("DataCells") && json["DataCells"].IsArray())
				Project::DataCell::Parse(DataConfig.DataCellInfoLst, json["DataCells"]);
			if (json.HasMember("AppearMode"))
				DataConfig.Appearance = json["AppearMode"].GetInt();
			if (json.HasMember("Key"))
				DataConfig.Key.Parse(json["Key"]);
			if (json.HasMember("GridBackColor"))
				DataConfig.TableBgColor = json["GridBackColor"].GetInt();
			if (json.HasMember("TitleBackColor"))
				DataConfig.TitleBgColor = json["TitleBackColor"].GetInt();
			if (json.HasMember("IsFrameUsed"))
				DataConfig.IsUseBorder = json["IsFrameUsed"].GetBool();
			if (json.HasMember("FrameStyle"))
				DataConfig.TableBorderStyle.Parse(json["FrameStyle"]);
			if (json.HasMember("GridStyle"))
				DataConfig.GridBorderStyle.Parse(json["GridStyle"]);
			if (json.HasMember("IsHorizontalGridUsed"))
				DataConfig.IsRowDividLine = json["IsHorizontalGridUsed"].GetBool();
			if (json.HasMember("IsVerticalGridUsed"))
				DataConfig.IsColDividLine = json["IsVerticalGridUsed"].GetBool();

		}
	}
}
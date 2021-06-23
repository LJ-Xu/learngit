/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableModel.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 关于表格绘制参数内容的源码，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "TableModel.h"

using namespace rapidjson;
using namespace std;
namespace UI
{
	void TableModel::InitData(std::string jstr)
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
			LOG_INFO_("TableModel Parse Json\n");
			TableConfig.InitBaseData(json);
			if (json.HasMember("OuterBorder") && json["OuterBorder"].IsObject())
				TableConfig.OuterBorder.Parse(json["OuterBorder"]);
			if (json.HasMember("HaveRowBorder"))
				TableConfig.HaveRowBorder = json["HaveRowBorder"].GetBool();
			if (json.HasMember("InnerBorderRow") && json["InnerBorderRow"].IsObject())
				TableConfig.InnerBorderRow.Parse(json["InnerBorderRow"]);
			if (json.HasMember("HaveColBorder"))
				TableConfig.HaveColBorder = json["HaveColBorder"].GetBool();
			if (json.HasMember("InnerBorderCol") && json["InnerBorderCol"].IsObject())
				TableConfig.InnerBorderCol.Parse(json["InnerBorderCol"]);

			if (json.HasMember("Fill") && json["Fill"].IsObject())
			{
				if (json["Fill"].HasMember("UseFillin"))
					TableConfig.UseFillin = json["Fill"]["UseFillin"].GetBool();
				if (json["Fill"].HasMember("BackColor"))
					TableConfig.FillColor = json["Fill"]["BackColor"].GetInt();
			}
		
			/*获取行高*/
			if (json.HasMember("TableRows") && json["TableRows"].IsArray())
			{
				std::vector<int>().swap(TableConfig.RowHeight);
				for (unsigned i = 0; i < json["TableRows"].Size(); i++)
				{
					if (json["TableRows"][i].HasMember("Height"))
						TableConfig.RowHeight.push_back(json["TableRows"][i]["Height"].GetInt());
				}
			}

			/*获取列宽*/
			if (json.HasMember("TableCols") && json["TableCols"].IsArray())
			{
				std::vector<int>().swap(TableConfig.ColWidth);
				for (unsigned i = 0; i < json["TableCols"].Size(); i++)
				{
					if (json["TableCols"][i].HasMember("Width"))
						TableConfig.ColWidth.push_back(json["TableCols"][i]["Width"].GetInt());
				}
			}

			if (json.HasMember("Row"))
				TableConfig.Rows = json["Row"].GetInt();
			if (json.HasMember("Col"))
				TableConfig.Cols = json["Col"].GetInt();
		}
	}
}
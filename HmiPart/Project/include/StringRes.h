/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StringRes.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "ResourceService.h"

namespace Project
{
	class StringRes
	{
	public:
		string Text;		//文字内容  strId<0:文字本身内容  >=0：lable名称
		string Label;		//文字标签库名称
		int TableNo = -1;	//文字库表格
		int RowNo = -1;			//文字库表格行

		
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Label, TableNo, RowNo);
		}
		void Parse(rapidjson::Value& text)
		{
			if (text.IsObject())
			{
				//获取默认字符
				if (text.HasMember("DefaultText") && !text["DefaultText"].IsNull())
					Text = text["DefaultText"].GetString();
				//获取标签库名称
				if (text.HasMember("Label") && !text["Label"].IsNull())
					Label = text["Label"].GetString();
				//获取文字库表格
				if (text.HasMember("TableNo"))
					TableNo = text["TableNo"].GetInt();
				if (text.HasMember("RowNo"))
					RowNo = text["RowNo"].GetInt();
			}
		}
		string GetDrawString()
		{
			string label;
			if (!Label.empty())
				label = UI::ResourceService::Ins().GetString(Label);
			//label = ResourceService::Ins().GetString("Label_1",0,0);
			else if (TableNo >= 0 && RowNo >= 0)
				label = UI::ResourceService::Ins().GetString(TableNo, RowNo);
			else
				label = Text;
			return label;
		}

	};
}

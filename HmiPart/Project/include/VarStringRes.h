#pragma once
#include "DataVar.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
namespace Project
{
	class VarStringRes
	{
	public:
		string Text;
		DataVarId StrId;		//获取表格行号
		string Label;			//文字标签库名称
		int TableNo = -1;		//文字库表格
		int RowNo;				//文字库表格行
		int Offset;
		int Scale;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, StrId, Label, TableNo, RowNo, Offset, Scale);
		}
		void InitVarStringData(rapidjson::Value& text)
		{
			if (text.IsObject())
			{
				if (text.HasMember("DefaultText") && !text["DefaultText"].IsNull())
					Text = text["DefaultText"].GetString();
				if (text.HasMember("Offset"))
					Offset = text["Offset"].GetInt();
				if (text.HasMember("Scale"))
					Scale = text["Scale"].GetInt();
				if (text.HasMember("Label") && !text["Label"].IsNull())
					Label = text["Label"].GetString();
				//获取文字库表格
				if (text.HasMember("TableNo"))
					TableNo = text["TableNo"].GetInt();
				if (text.HasMember("RowNo"))
					RowNo = text["RowNo"].GetInt();
				if (text.HasMember("SrcId"))
				{
					rapidjson::Value &SrcId = text["SrcId"];
					if (SrcId.HasMember("VID"))
						StrId.Vid = SrcId["VID"].GetInt();
				}
			}
		}

	};
}


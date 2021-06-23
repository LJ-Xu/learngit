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
		DataVarId StrId;		//��ȡ����к�
		string Label;			//���ֱ�ǩ������
		int TableNo = -1;		//���ֿ���
		int RowNo;				//���ֿ�����
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
				//��ȡ���ֿ���
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


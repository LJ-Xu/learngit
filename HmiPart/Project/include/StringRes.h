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
		string Text;		//��������  strId<0:���ֱ�������  >=0��lable����
		string Label;		//���ֱ�ǩ������
		int TableNo = -1;	//���ֿ���
		int RowNo = -1;			//���ֿ�����

		
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Label, TableNo, RowNo);
		}
		void Parse(rapidjson::Value& text)
		{
			if (text.IsObject())
			{
				//��ȡĬ���ַ�
				if (text.HasMember("DefaultText") && !text["DefaultText"].IsNull())
					Text = text["DefaultText"].GetString();
				//��ȡ��ǩ������
				if (text.HasMember("Label") && !text["Label"].IsNull())
					Label = text["Label"].GetString();
				//��ȡ���ֿ���
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

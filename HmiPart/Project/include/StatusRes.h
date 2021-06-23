/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StatusRes.h
 * Author   : qiaodan
 * Date     : 2020-10-8
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "Point.h"

namespace Project
{
	class StatusRes
	{
	public:
		std::vector <std::string> Texts;		//文字内容  strId<0:文字本身内容  >=0：lable名称
		std::string Label;		//文字标签库名称
		int TableNo = -1;	//文字库表格
		int RowNo = -1;			//文字库表格行



		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Texts, Label, TableNo, RowNo);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<StatusRes>& vector, rapidjson::Value& jsonObj);

#if 0
		/*******************************************************************************
		 * Name     : GetDrawString
		 * Descript : 从标签库/文字库里获取当前状态的文字内容
		 * Input    : model  - 用于存放按键绘制相关数据的KeyBtnModel结构体
		 * Output   : currentStatus   - 当前状态
		 * Note	    :
		 *******************************************************************************/
		string GetDrawString(int currentStatus)
		{
			string label;
			int i, size;
			size = Texts.size();
			if (!Label.empty())
				label = UI::ResourceService::Ins().GetString(Label, currentStatus);
			//label = ResourceService::Ins().GetString("Label_1",0,0);
			else if (TableNo >= 0 && RowNo >= 0)
				label = UI::ResourceService::Ins().GetString(TableNo, RowNo);
			else
			{
				std::unique_ptr<std::string[]> texts{ new std::string[size] };
				vector<std::string>::iterator currentText;
				for (i = 0, currentText = Texts.begin(); currentText != Texts.end(); currentText++, i++)
					texts.get()[i] = (*currentText);
				label = texts.get()[currentStatus];
			}
			return label;
		}
#endif
		
	};
}

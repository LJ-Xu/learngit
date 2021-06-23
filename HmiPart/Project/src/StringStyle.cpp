/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StringStyle
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 文字样式json解析
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "StringStyle.h"
#include <vector>
namespace Project
{
	void StringStyle::Parse(rapidjson::Value& css)
	{
		if (css.IsObject())
		{
			if (css.HasMember("Style"))
				Style = css["Style"].GetInt();
			if (css.HasMember("Alpha"))
				Alpha = css["Alpha"].GetInt();
			if (css.HasMember("Align"))
				Align = css["Align"].GetInt();
			if (css.HasMember("Colors"))
			{
				Colors = css["Colors"].GetInt();
				RGBColor.R = R(Colors);
				RGBColor.G = G(Colors);
				RGBColor.B = B(Colors);
			}
			if (css.HasMember("Fonts"))
			{
				rapidjson::Value &font = css["Fonts"];
				if (font.IsObject())
				{
					if (font.HasMember("Size"))
						Font.Size = (int)font["Size"].GetFloat();
					//m_StringConfig.css.font.size = 9.0;

					if (font.HasMember("Name") && !font["Name"].IsNull())
						Font.Name = font["Name"].GetString();
				}
			}
			if (css.HasMember("UnderLineUsed"))
				UnderlineUsed = css["UnderLineUsed"].GetBool();
			if (css.HasMember("StrikeoutUsed"))
				StrikeoutUsed = css["StrikeoutUsed"].GetBool();
			/*获取下划线坐标点*/
			if (css.HasMember("UnderPoint") && !css["UnderPoint"].IsNull() && css["UnderPoint"].IsArray())
			{
				rapidjson::Value& array = css["UnderPoint"];
				if (array.IsArray()) {
					vector<Project::Point>().swap(UnderPoint);
					for (size_t i = 0; i < array.Size(); i++) {
						if (array[i].IsObject()) {
							Project::Point tmp;
							if (array[i].HasMember("X"))
								tmp.X = array[i]["X"].GetInt();
							if (array[i].HasMember("Y"))
								tmp.Y = array[i]["Y"].GetInt();
							UnderPoint.push_back(tmp);
						}
					}
				}
			}
			/*获取删除线坐标点*/
			if (css.HasMember("StrikeoutPoint") && !css["StrikeoutPoint"].IsNull() && css["StrikeoutPoint"].IsArray())
			{
				rapidjson::Value& array = css["StrikeoutPoint"];
				if (array.IsArray()) {
					vector<Project::Point>().swap(StrikeoutPoint);
					for (size_t i = 0; i < array.Size(); i++) {
						if (array[i].IsObject()) {
							Project::Point tmp;
							if (array[i].HasMember("X"))
								tmp.X = array[i]["X"].GetInt();
							if (array[i].HasMember("Y"))
								tmp.Y = array[i]["Y"].GetInt();
							StrikeoutPoint.push_back(tmp);
						}
					}
				}
			}
		}
	}

	void StringStyle::Parse(std::vector<StringStyle>& vector, rapidjson::Value& jsonObj)
	{
		std::vector<StringStyle>().swap(vector);
		for (unsigned i = 0; i < jsonObj.Size(); i++)
		{
			if (!jsonObj[i].IsNull())
			{
				Project::StringStyle* style = new Project::StringStyle();
				style->Parse(jsonObj[i]);
				vector.push_back(*style);
			}
		}
	}
}

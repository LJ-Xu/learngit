/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StringStyle.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseStyle.h"
#include "Point.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <vector>
namespace Project
{
	class StringStyle : public BaseStyle
	{
	public:
		int Style;
		int Alpha;
		int Align;

		bool UnderlineUsed;				//是否有下划线
		bool StrikeoutUsed;				//是否有删除线

		std::vector<Point> UnderPoint;			//下划线线段起始终止坐标
		std::vector<Point> StrikeoutPoint;		//删除线线段起始终止坐标

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Style, Alpha, Align, Font, RGBColor, Colors,
				UnderlineUsed, StrikeoutUsed, UnderPoint, StrikeoutPoint);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<StringStyle>& vector, rapidjson::Value& jsonObj);

		//void InitStyleData(rapidjson::Value& css)
		//{
		//	if (css.IsObject())
		//	{
		//		if (css.HasMember("Style"))
		//			Style = css["Style"].GetInt();
		//		if (css.HasMember("Alpha"))
		//			Alpha = css["Alpha"].GetInt();
		//		if (css.HasMember("Align"))
		//			Align = css["Align"].GetInt();
		//		if (css.HasMember("Colors"))
		//		{
		//			int colorfont = css["Colors"].GetInt();
		//			Color.R = R(colorfont);
		//			Color.G = G(colorfont);
		//			Color.B = B(colorfont);
		//		}
		//		if (css.HasMember("Fonts"))
		//		{
		//			rapidjson::Value &font = css["Fonts"];
		//			if (font.IsObject())
		//			{
		//				if (font.HasMember("Size"))
		//					Font.Size = (int)font["Size"].GetFloat();
		//				//m_StringConfig.css.font.size = 9.0;

		//				if (font.HasMember("Name") && !font["Name"].IsNull())
		//					Font.Name = font["Name"].GetString();
		//			}
		//		}
		//		if (css.HasMember("UnderLineUsed"))
		//			UnderlineUsed = css["UnderLineUsed"].GetBool();
		//		if (css.HasMember("StrikeoutUsed"))
		//			StrikeoutUsed = css["StrikeoutUsed"].GetBool();
		//		/*获取下划线坐标点*/
		//		if (css.HasMember("UnderPoint") && !css["UnderPoint"].IsNull() && css["UnderPoint"].IsArray())
		//		{
		//			rapidjson::Value& array = css["UnderPoint"];
		//			if (array.IsArray()) {
		//				vector<Project::Point>().swap(UnderPoint);
		//				for (size_t i = 0; i < array.Size(); i++) {
		//					if (array[i].IsObject()) {
		//						Project::Point tmp;
		//						if (array[i].HasMember("X"))
		//							tmp.X = array[i]["X"].GetInt();
		//						if (array[i].HasMember("Y"))
		//							tmp.Y = array[i]["Y"].GetInt();
		//						UnderPoint.push_back(tmp);
		//					}
		//				}
		//			}
		//		}
		//		/*获取删除线坐标点*/
		//		if (css.HasMember("StrikeoutPoint") && !css["StrikeoutPoint"].IsNull() && css["StrikeoutPoint"].IsArray())
		//		{
		//			rapidjson::Value& array = css["StrikeoutPoint"];
		//			if (array.IsArray()) {
		//				vector<Project::Point>().swap(StrikeoutPoint);
		//				for (size_t i = 0; i < array.Size(); i++) {
		//					if (array[i].IsObject()) {
		//						Project::Point tmp;
		//						if (array[i].HasMember("X"))
		//							tmp.X = array[i]["X"].GetInt();
		//						if (array[i].HasMember("Y"))
		//							tmp.Y = array[i]["Y"].GetInt();
		//						StrikeoutPoint.push_back(tmp);
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
	};
}

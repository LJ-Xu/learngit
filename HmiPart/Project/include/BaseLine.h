/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseLine.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于描绘线属性的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/

#pragma once
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

class BaseLine
{
public:
	int Color = 0;  //颜色
	int Weight = 0; //线的粗度
	/*
	FL_SOLID = 0,		///< line style: <tt>___________</tt>
	FL_DASH = 1,		///< line style: <tt>_ _ _ _ _ _</tt>
	FL_DOT = 2,		///< line style: <tt>. . . . . .</tt>
	FL_DASHDOT = 3,		///< line style: <tt>_ . _ . _ .</tt>
	FL_DASHDOTDOT = 4,		///< line style: <tt>_ . . _ . .</tt>
	*/
	int Type;  //线的类型 点线  短虚线 长虚线 实线
	int Alpha; //透明度

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(Color, Weight, Type, Alpha);
	}

	void Parse(rapidjson::Value& jsonObj)
	{
		if (jsonObj.HasMember("Color"))
			Color = jsonObj["Color"].GetInt();
		if (jsonObj.HasMember("Weight"))
			Weight = jsonObj["Weight"].GetInt();
		if (jsonObj.HasMember("Type"))
			Type = jsonObj["Type"].GetInt();
		if (jsonObj.HasMember("Alpha"))
			Alpha = jsonObj["Alpha"].GetInt();

		if (jsonObj.HasMember("Style"))
			Type = jsonObj["Style"].GetInt();
		if (jsonObj.HasMember("Size"))
			Weight = jsonObj["Size"].GetInt();
		if (jsonObj.HasMember("Colors"))
			Color = jsonObj["Colors"].GetInt();

	}
};


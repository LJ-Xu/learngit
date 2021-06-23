/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseBtnModel.cpp
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 解析json关于中文按键内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "ChineseBtnModel.h"
#include <FL/Fl.H>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
using namespace rapidjson;
namespace UI
{
	void ChineseBtnModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		if (json.HasMember("X"))
			BtnConfig.X = json["X"].GetInt();
		if (json.HasMember("Y"))
			BtnConfig.Y = json["Y"].GetInt();
		if (json.HasMember("Width"))
			BtnConfig.Width = json["Width"].GetInt();
		if (json.HasMember("Height"))
			BtnConfig.Height = json["Height"].GetInt();
		if (json.HasMember("Text"))
			BtnConfig.text = json["Text"].GetString();
		//BtnConfig.text = "中文";
		//BtnConfig.X = 100;  //X坐标
		//BtnConfig.Y = 100;  //Y坐标
		//BtnConfig.Width = 80; //宽度
		//BtnConfig.Height = 40;//高度
		//BtnConfig.backColor = FL_WHITE;
		////m_config.font.color = FL_GREEN;
		//BtnConfig.font.Name = "宋体";

	}
}
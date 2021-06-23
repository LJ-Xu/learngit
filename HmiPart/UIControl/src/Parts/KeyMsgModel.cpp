/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于按键消息绘制参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "KeyMsgModel.h"
#include <FL/Fl.H>
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void KeyMsgModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		KeyMsgConfig.X = 100;  //X坐标
		KeyMsgConfig.Y = 100;  //Y坐标
		KeyMsgConfig.Width = 80; //宽度
		KeyMsgConfig.Height = 40;//高度
	}
}
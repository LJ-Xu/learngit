/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ����json�����ڰ�����Ϣ���Ʋ������ݣ���������Ϣ
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
		KeyMsgConfig.X = 100;  //X����
		KeyMsgConfig.Y = 100;  //Y����
		KeyMsgConfig.Width = 80; //���
		KeyMsgConfig.Height = 40;//�߶�
	}
}
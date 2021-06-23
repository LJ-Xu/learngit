/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnPilotLampModel.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 关于指示灯按键绘制参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "BtnPilotLampModel.h"
#include <FL/Fl.H>
#include "UIComm.h"
#include "JsonComm.h"
using namespace rapidjson;
namespace UI
{
	void BtnPilotLampModel::InitData(std::string jstr)
	{
		Document json;
		json.Parse(jstr.c_str());
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/BtnPilotLamp.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else
		{
			LOG_INFO_("BtnPilotLampModel Parse Json\n");
			BtnPilotLampConfig.InitBaseData(json);
			//if (json.HasMember("X"))
			//	BtnPilotLampConfig.X = json["X"].GetInt();
			//if (json.HasMember("Y"))
			//	BtnPilotLampConfig.Y = json["Y"].GetInt();
			//if (json.HasMember("Width"))
			//	BtnPilotLampConfig.Width = json["Width"].GetInt();
			//if (json.HasMember("Height"))
			//	BtnPilotLampConfig.Height = json["Height"].GetInt();
			//if (json.HasMember("IsEnable"))
			//	BtnPilotLampConfig.IsEnable = json["IsEnable"].GetBool();
			//if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
			//	BtnPilotLampConfig.CtrlName = json["CtrlName"].GetString();
			//if (json.HasMember("OffX"))
			//	BtnPilotLampConfig.InitOffXData(json["OffX"]);
			//if (json.HasMember("OffY"))
			//	BtnPilotLampConfig.InitOffYData(json["OffY"]);
			//if (json.HasMember("Perm") && json["Perm"].IsObject())
			//	BtnPilotLampConfig.Perm.Parse(json["Perm"]);

			if (json.HasMember("Action"))
				BtnPilotLampConfig.Action = (Project::BtnAction)json["Action"].GetInt();
			if (json.HasMember("Flash"))
				BtnPilotLampConfig.Flash = json["Flash"].GetInt();
			if (json.HasMember("Logic"))
				BtnPilotLampConfig.Logic = json["Logic"].GetInt();
			if (json.HasMember("Speed"))
				BtnPilotLampConfig.Speed = json["Speed"].GetFloat();
			if (json.HasMember("UseSameAddr"))
				BtnPilotLampConfig.UseSameAddr = json["UseSameAddr"].GetBool();
			if (json.HasMember("StatusVar") && json["StatusVar"].IsObject())
				BtnPilotLampConfig.ReadVar.Parse(json["StatusVar"]);
			if (json.HasMember("IsRWDiff"))
				BtnPilotLampConfig.RwDiffAddrUsed = json["IsRWDiff"].GetBool();
			if (json.HasMember("StatusKeys") && json["StatusKeys"].IsArray())
				Project::ImageResId::Parse(BtnPilotLampConfig.PicKey, json["StatusKeys"]);
			if (json.HasMember("WriteVar") && json["WriteVar"].IsObject())
				BtnPilotLampConfig.WriteVar.Parse(json["WriteVar"]);
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				BtnPilotLampConfig.Txt.Parse(json["Txt"]);
			if (json.HasMember("Csses") && json["Csses"].IsArray())
				Project::StringStyle::Parse(BtnPilotLampConfig.StrStyles, json["Csses"]);
		}
	}
}
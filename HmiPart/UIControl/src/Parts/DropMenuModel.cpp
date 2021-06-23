/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuModel.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 解析json，关于下拉菜单内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "DropMenuModel.h"
#include <cstdio>
#include <iostream>
#include "UIComm.h"
using namespace rapidjson;
using namespace std;
namespace UI
{
	void DropMenuModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/DropDownMenuJson.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
		}
		else {
			LOG_INFO_("DropMenuModel Parse Json\n");
			MenuConfig.InitBaseData(json);

			if (json.HasMember("MenuMode"))
				MenuConfig.MenuMode = json["MenuMode"].GetInt();
			if (json.HasMember("ReadVar") && json["ReadVar"].IsObject())
				MenuConfig.ReadVar.Parse(json["ReadVar"]);
			if (json.HasMember("DataStyle"))
				MenuConfig.DataStyle = json["DataStyle"].GetInt();
			if (json.HasMember("MenuNum"))
				MenuConfig.MenuNum = json["MenuNum"].GetInt();
			if (json.HasMember("PopMode"))
				MenuConfig.PopMode = json["PopMode"].GetInt();
			if (json.HasMember("ContentMode"))
				MenuConfig.ContentMode = json["ContentMode"].GetInt();
			if (json.HasMember("ContentHeight"))
				MenuConfig.ContentHeight = json["ContentHeight"].GetInt();
			if (json.HasMember("MenuInfo") && json["MenuInfo"].IsArray())
			{
				Value& array = json["MenuInfo"];
				vector<Project::MenuContontLst>().swap(MenuConfig.MenuInfo);
				for (unsigned i = 0; i < array.Size() - 1; i++) {
					Project::MenuContontLst tmpInfo;
					if (array[i].HasMember("Index") && array[i]["Index"].IsInt())
						tmpInfo.Index = array[i]["Index"].GetInt();
					if (array[i].HasMember("Value"))
						tmpInfo.Value = array[i]["Value"].GetDouble();
					if (array[i].HasMember("Content") && array[i]["Content"].IsObject())
						tmpInfo.Content.Parse(array[i]["Content"]);
					MenuConfig.MenuInfo.push_back(tmpInfo);
				}
			}
			if (json.HasMember("PicKey") && json["PicKey"].IsArray())
				Project::ImageResId::Parse(MenuConfig.PicKey, json["PicKey"]);
			if (json.HasMember("RowHeight"))
				MenuConfig.RowHeight = json["RowHeight"].GetInt();
			if (json.HasMember("SelectColor"))
				MenuConfig.SelectColor = json["SelectColor"].GetInt();
			if (json.HasMember("BgColor"))
				MenuConfig.BgColor = json["BgColor"].GetInt();
			if (json.HasMember("FrameColor"))
				MenuConfig.FrameColor = json["FrameColor"].GetInt();
			if (json.HasMember("MenuStyle") && json["MenuStyle"].IsArray())
				Project::StringStyle::Parse(MenuConfig.MenuStyle, json["MenuStyle"]);
			if (json.HasMember("HMICommandItem") && json["HMICommandItem"].IsObject())
			{
				if (json["HMICommandItem"].HasMember("Name") && !json["HMICommandItem"]["Name"].IsNull())
					MenuConfig.HMICommandItem.Name = json["HMICommandItem"]["Name"].GetString();
				if (json["HMICommandItem"].HasMember("Cmd") && !json["HMICommandItem"]["Cmd"].IsNull())
					MenuConfig.HMICommandItem.Cmd = json["HMICommandItem"]["Cmd"].GetString();
			}
			if (json.HasMember("DrawPosition") && json["DrawPosition"].IsObject())
			{
				if (json["DrawPosition"].HasMember("OriginPos") && json["DrawPosition"]["OriginPos"].IsObject())
				{
					if (json["DrawPosition"]["OriginPos"].HasMember("x"))
						MenuConfig.BtnPosition.X = json["DrawPosition"]["OriginPos"]["x"].GetInt();
					if (json["DrawPosition"]["OriginPos"].HasMember("y"))
						MenuConfig.BtnPosition.Y = json["DrawPosition"]["OriginPos"]["y"].GetInt();
				}
			}
			if (json.HasMember("ResErrorNotify") && json["ResErrorNotify"].IsObject())
				MenuConfig.ErrorNotice.Parse(json["ResErrorNotify"]);
		}

	}
}
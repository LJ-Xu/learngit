/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferModel.cpp
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 解析json，关于配方传输参数内容，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "stdafx.h"
#include "RecipeTransferModel.h"
#include <FL/Fl.H>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "UIComm.h"
using namespace rapidjson;
namespace UI
{
	void RecipeTransferModel::InitData(std::string jstr)
	{
		Document json;
		//FILE *fp = fopen("C:/Users/PC/Desktop/HmiJson/json/Keypad.json", "r");
		//char buf[0XFFFF];
		//rapidjson::FileReadStream input(fp, buf, sizeof(buf));
		//json.ParseStream(input);
		json.Parse(jstr.c_str());
		if (json.HasParseError())	//判断解析是否正确
		{
			printf("parse error : (%d:%d)\n", json.GetParseError(), json.GetErrorOffset());
			return;
		}
		else
		{
			LOG_INFO_("RecipeTransferModel Parse Json\n");
			TransferRecipeConfig.InitBaseData(json);
			if (json.HasMember("Action"))
				TransferRecipeConfig.Action = json["Action"].GetInt();
			if (json.HasMember("TransmitMode"))
				TransferRecipeConfig.TransmitMode = (Project::RecipeTransferAction)json["TransmitMode"].GetInt();
			if (json.HasMember("TransmitVarIdRefs") && json["TransmitVarIdRefs"].IsArray())
				Project::DataVarId::Parse(TransferRecipeConfig.TransmitVarIdRefs, json["TransmitVarIdRefs"]);
			if (json.HasMember("TriggerFunc") && json["TriggerFunc"].IsArray())
			{
				vector<int>().swap(TransferRecipeConfig.TriggerFunc);
				for (size_t i = 0; i < json["TriggerFunc"].Size(); i++)
					TransferRecipeConfig.TriggerFunc.push_back(json["TriggerFunc"][i].GetInt());
			}
			if (json.HasMember("RecipeId") && !json["RecipeId"].IsNull())
				TransferRecipeConfig.RecipeId = json["RecipeId"].GetString();
			if (json.HasMember("RecipeIndexRow"))
				TransferRecipeConfig.RecipeIndexRow = json["RecipeIndexRow"].GetInt();
			if (json.HasMember("RecipeCount"))
				TransferRecipeConfig.RecipeCount = json["RecipeCount"].GetInt();
			if (json.HasMember("PlcVarIdRef"))
				TransferRecipeConfig.PlcVarIdRef.Parse(json["PlcVarIdRef"]);
			if (json.HasMember("CompleteVarIdRef"))
				TransferRecipeConfig.CompleteVarIdRef.Parse(json["CompleteVarIdRef"]);
			if (json.HasMember("Txt") && json["Txt"].IsObject())
				TransferRecipeConfig.Txt.Parse(json["Txt"]);
			if (json.HasMember("Css") && json["Css"].IsArray())
				Project::StringStyle::Parse(TransferRecipeConfig.StrStyles, json["Css"]);
			if (json.HasMember("Keys") && json["Keys"].IsArray())
				Project::ImageResId::Parse(TransferRecipeConfig.PicKey, json["Keys"]);
		}
	}
}

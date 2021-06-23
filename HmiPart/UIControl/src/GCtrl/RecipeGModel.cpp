/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeGModel.cpp
 * Author   : qiaodan
 * Date     : 2021-02-03
 * Descript : 关于配方的参数内容，解析json，存放相关信息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "RecipeGModel.h"

namespace UI
{
	RecipeGModel::RecipeGModel() {

	}

	RecipeGModel::~RecipeGModel() {

	}

	void RecipeGModel::GetRect(Rectangle & rect) {

	}

	void RecipeGModel::InitTestData() {

	}

	// 获取控件配置
	void * RecipeGModel::GetTestConfig() {
		return &RecipeGConfig;
	}

	void RecipeGModel::InitData(string jsonstr) {
		rapidjson::Document json;
		// 解析json字符串
		json.Parse(jsonstr.c_str());
		// 获取解析结果
		if (json.HasParseError()) {
			cout << "Parse Error : ("
				<< json.GetParseError() << " : "
				<< json.GetErrorOffset() << ")" << endl;
		}
		else {
			if (json.HasMember("OffX"))
				RecipeGConfig.InitOffXData(json["OffX"]);
			if (json.HasMember("OffY"))
				RecipeGConfig.InitOffYData(json["OffY"]);
			if (json.HasMember("Width"))
				RecipeGConfig.Width = json["Width"].GetInt();
			if (json.HasMember("Height"))
				RecipeGConfig.Height = json["Height"].GetInt();
			if (json.HasMember("X"))
				RecipeGConfig.X = json["X"].GetInt();
			if (json.HasMember("Y"))
				RecipeGConfig.Y = json["Y"].GetInt();
			if (json.HasMember("IsEnable"))
				RecipeGConfig.IsEnable = json["IsEnable"].GetBool();
			if (json.HasMember("CtrlName") && !json["CtrlName"].IsNull())
				RecipeGConfig.CtrlName = json["CtrlName"].GetString();
			if (json.HasMember("Perm"))
				RecipeGConfig.Perm.Parse(json["Perm"]);

			if (json.HasMember("RecipeTBs") && json["RecipeTBs"].IsArray())
			{
				for (size_t i = 0; i < json["RecipeTBs"].Size(); i++)
				{
					vector<Project::RecipeInfoRes> recipeinfo;
					string recipeName;
					if (json["RecipeTBs"][i].IsObject())
					{
						if (json["RecipeTBs"][i].HasMember("RecipeTag") &&
							!json["RecipeTBs"][i]["RecipeTag"].IsNull())
							recipeName = json["RecipeTBs"][i]["RecipeTag"].GetString();
						if (json["RecipeTBs"][i].HasMember("RecipeTb") && json["RecipeTBs"][i]["RecipeTb"].IsArray())
							Project::RecipeInfoRes::Parse(recipeinfo, json["RecipeTBs"][i]["RecipeTb"]);
					}
					RecipeGConfig.TbList.insert(pair<string, vector<Project::RecipeInfoRes>>(recipeName, recipeinfo));
				}
			}
		}
	}

	void RecipeGModel::InitData(vector<char> data) {
		return BaseModel::InitData<decltype(RecipeGConfig)>(data, RecipeGConfig);
	}

	unique_ptr<vector<char>> RecipeGModel::GetData() {
		return BaseModel::GetData<decltype(RecipeGConfig)>(RecipeGConfig);
	}
}
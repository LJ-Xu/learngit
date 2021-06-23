/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeStorage.cpp
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : 数据采集接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#ifdef WIN32
#include <windows.h> 
#endif
#include "System.h"
#include "RecipeStorage.h"
#include <algorithm>
#include <iostream>

namespace Storage
{
	RecipeStorage * RecipeStorage::instance_ = nullptr;

	RecipeStorage * RecipeStorage::Ins() {
		static std::once_flag flag;
		call_once(flag, [&]() {
			instance_ = new RecipeStorage();
			});
		return instance_;
	}
	vector<vector<string>> RecipeStorage::QueryByRepiceName(string name, int col)
	{
		return std::move(RecipeStorageService::Ins(name)->SelectAllRecipeRecord(name,col));
	}

	vector<vector<string>> RecipeStorage::QueryByKey(string recipename, string key, vector<string> colname)
	{
		return std::move(RecipeStorageService::Ins(recipename)->SelectRecipeRecordByKey(recipename, key, colname));
	}
	vector<vector<string>> RecipeStorage::QueryByValue(string recipename, vector<string> key, vector<string> colname)
	{
		return std::move(RecipeStorageService::Ins(recipename)->SelectRecipeRecordByValue(recipename, key, colname));
	}
	int  RecipeStorage::GetCountByRepiceName(string name)
	{
		return RecipeStorageService::Ins(name)->GetRecipeDataNum(name);

	}
	
}

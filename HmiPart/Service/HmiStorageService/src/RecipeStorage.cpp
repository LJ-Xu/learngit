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
		return std::move(RecipeStorageService::Ins()->SelectAllRecipeRecord(name,col));
	}

	vector<vector<string>> RecipeStorage::QueryByKey(string recipename, string key, vector<string> colname)
	{
		return std::move(RecipeStorageService::Ins()->SelectRecipeRecordByKey(recipename, key, colname));
	}
	vector<vector<string>> RecipeStorage::QueryByValue(string recipename, vector<string> key, vector<string> colname)
	{
		return std::move(RecipeStorageService::Ins()->SelectRecipeRecordByValue(recipename, key, colname));
	}
	bool RecipeStorage::DeleteRecord(string name, int row)
	{
		return  RecipeStorageService::Ins()->DeleteRecipeRecord(name, row);
	}
	bool RecipeStorage::AddRecord(string name)
	{
		int row = RecipeStorageService::Ins()->GetRecipeDataNum(name);
		return RecipeStorageService::Ins()->AddRecipeRecord(name, row);
	}
	bool RecipeStorage::InsertRecord(string name, int row)
	{
		return RecipeStorageService::Ins()->InsertRecipeRecord(name, row);
	}
	bool RecipeStorage::CopyRecord(string name, int row, vector<Project::ColDataTypeInfo>& colnames)
	{
		vector<string> data = RecipeStorageService::Ins()->SelectRecipeRecordByRow(name,row,colnames.size() + 1);
		return RecipeStorageService::Ins()->CopyRecipeRecord(name, row, data, colnames);
	}
	bool RecipeStorage::MoveUpRecord(string name, int row)
	{
		if (row < 1)
			return false;
		return RecipeStorageService::Ins()->MoveUpRecipeRecord(name, row);
	}
	bool RecipeStorage::MoveDownRecord(string name, int row)
	{
		int allrow = RecipeStorageService::Ins()->GetRecipeDataNum(name);
		if (row >= allrow - 1)
			return false;
		return RecipeStorageService::Ins()->MoveDownRecipeRecord(name, row);
	}
	bool RecipeStorage::UpdateRecord(string groupname, string colname, unsigned int rowno, string data)
	{
		return RecipeStorageService::Ins()->UpdateRecipeRecord(groupname, colname, rowno, data);
	}
	int  RecipeStorage::GetCountByRepiceName(string name)
	{
		return RecipeStorageService::Ins()->GetRecipeDataNum(name);

	}
	
}

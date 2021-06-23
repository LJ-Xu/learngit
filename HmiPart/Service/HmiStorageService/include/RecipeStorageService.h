#pragma once
#ifndef __SAMPLESTORAGESERVICE__
#define	__SAMPLESTORAGESERVICE__

#include <iostream>
#include <vector>
#include <mutex>
#include "BaseStorageService.h"

namespace Storage
{
	class RecipeStorageService 
	{
	private:
		RecipeStorageService() { }
		RecipeStorageService(string name);
		~RecipeStorageService();
		RecipeStorageService(const RecipeStorageService &) = delete;
		RecipeStorageService & operator=(const RecipeStorageService &) = delete;
	public:
		// 获取数据库对象
		static RecipeStorageService * Ins(string name);
		// 销毁数据库对象
		void Destroy();
		// 创建报警录入内存数据库
	public:
		// 查询采集记录
		vector<vector<string>> SelectRecipe(const char * sql, int col);
		vector<vector<string>> SelectAllRecipeRecord(string recipename,int col);
		vector<vector<string>> SelectRecipeRecordByValue(string recipename, vector<string> value, vector<string> colname);
		vector<vector<string>> SelectRecipeRecordByKey(string recipename, string key, vector<string> colname);
		int GetRecipeDataNum(string recipename);
	private:
		static RecipeStorageService * ins;
		sqlite3 * db;	// 数据库句柄
		bool isOpened;	// 是否打开数据库
	};
}
#endif

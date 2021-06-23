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
		// ��ȡ���ݿ����
		static RecipeStorageService * Ins(string name);
		// �������ݿ����
		void Destroy();
		// ��������¼���ڴ����ݿ�
	public:
		// ��ѯ�ɼ���¼
		vector<vector<string>> SelectRecipe(const char * sql, int col);
		vector<vector<string>> SelectAllRecipeRecord(string recipename,int col);
		vector<vector<string>> SelectRecipeRecordByValue(string recipename, vector<string> value, vector<string> colname);
		vector<vector<string>> SelectRecipeRecordByKey(string recipename, string key, vector<string> colname);
		int GetRecipeDataNum(string recipename);
	private:
		static RecipeStorageService * ins;
		sqlite3 * db;	// ���ݿ���
		bool isOpened;	// �Ƿ�����ݿ�
	};
}
#endif

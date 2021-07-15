#pragma once
#ifndef __SAMPLESTORAGESERVICE__
#define	__SAMPLESTORAGESERVICE__

#include <iostream>
#include <vector>
#include <mutex>
#include "BaseStorageService.h"
#include "BaseVar.h"
namespace Storage
{
	class RecipeStorageService 
	{
	private:
		RecipeStorageService();
		~RecipeStorageService();
		RecipeStorageService(const RecipeStorageService &) = delete;
		RecipeStorageService & operator=(const RecipeStorageService &) = delete;
	public:
		// ��ȡ���ݿ����
		static RecipeStorageService * Ins();
		// �������ݿ����
		void Destroy();
		// ��������¼���ڴ����ݿ�
	public:
		// ��ѯ�ɼ���¼
		vector<vector<string>> SelectRecipe(const char * sql, int col);
		vector<string> SelectRowRecipe(const char * sql, int col);
		vector<vector<string>> SelectAllRecipeRecord(string recipename, int col);
		vector<string> SelectRecipeRecordByRow(string recipename,int row, int cols);
		vector<vector<string>> SelectRecipeRecordByValue(string recipename, vector<string> value, vector<string> colname);
		vector<vector<string>> SelectRecipeRecordByKey(string recipename, string key, vector<string> colname);
		int GetRecipeDataNum(string recipename);
		bool OrderRecipeRecord(string recipename, int mode = 0);
		bool AddRecipeRecord(string recipename, int row);
		bool InsertRecipeRecord(string recipename, int row);
		bool DeleteRecipeRecord(string recipename, int row);
		bool CopyRecipeRecord(string recipename, int row, vector<string>& data, vector<Project::ColDataTypeInfo>& colnames);
		bool MoveUpRecipeRecord(string recipename, int row);
		bool MoveDownRecipeRecord(string recipename, int row);
		bool UpdateRecipeRecord(string recipename, string colname, int rowcol, string data);
	private:
		static RecipeStorageService * ins;
		sqlite3 * db;	// ���ݿ���
		bool isOpened;	// �Ƿ�����ݿ�
	};
}
#endif

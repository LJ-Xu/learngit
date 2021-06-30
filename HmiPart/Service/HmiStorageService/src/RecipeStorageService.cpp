#include <string.h>
#include "RecipeStorageService.h"
#include "RunEnv.h"
namespace Storage
{
	RecipeStorageService * RecipeStorageService::ins = nullptr;

	RecipeStorageService::RecipeStorageService() {
		//int ret = sqlite3_open(name.c_str(), &db);
		int ret = sqlite3_open(RunEnv::Cnf.RecipePath.c_str(), &db);
		if (ret != SQLITE_OK) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", file, __LINE__, ret, sqlite3_errmsg(db));
			sqlite3_close(db);
		}
		// ���ݿ��Ƿ��
		isOpened = (ret == SQLITE_OK);
	}
	RecipeStorageService::~RecipeStorageService()
	{
		if (isOpened) {
			sqlite3_close_v2(db);
			isOpened = false;
		}
	}

	RecipeStorageService * RecipeStorageService::Ins() {
		if(!ins)
			ins = new RecipeStorageService();
		return ins;
	}

	/**
	 * @brief  : �ͷŲ�����¼���ݿ�
	 * @return : void
	 */
	void RecipeStorageService::Destroy() {
		if (ins) {
			delete ins;
			ins = nullptr;
		}
	}


	/**
	 * @brief  : ��ȡͨ���ɼ���¼
	 * @params : sql		���ݿ�ִ�����
	 */
	vector<vector<string>> RecipeStorageService::SelectRecipe(const char * sql, int col) {
		vector<vector<string>> records;
		int ret = 0;
		struct sqlite3_stmt *stmt;
		ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return std::move(records);
		}
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			vector<string> record;
			for (size_t i = 1; i <= (size_t)col; i++)
			{
				const unsigned char* str = sqlite3_column_text(stmt, i);
				string data((const char*)str);
				record.push_back(data);
			}
			records.push_back(record);
		}
		//fprintf(stderr, "Sql Select Complete.\n");
		sqlite3_finalize(stmt);
		if (isOpened) {
			sqlite3_close_v2(db);
			isOpened = false;
			Destroy();
		}
		return std::move(records);
	}

	/**
	 * @brief  : ��ȡ�䷽������
	 * @params : recipename		�䷽����
	 */
	vector<vector<string>> RecipeStorageService::SelectAllRecipeRecord(string recipename, int col) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM %s;", recipename.c_str());
		return std::move(SelectRecipe(sql, col));
	}
	vector<vector<string>> RecipeStorageService::SelectRecipeRecordByValue(string recipename, vector<string> value, vector<string> colname)
	{
		char sql[SQLCMDLEN] = { 0 };
		string sqlstr = "SELECT * FROM " + recipename + " WHERE";
		for (size_t i = 0; i < colname.size(); i++)
		{
			sqlstr = sqlstr + " " + colname[i] + " LIKE " + value[i];
			if (i != colname.size() - 1)
				sqlstr += " OR";
		}
		
		memcpy(sql, sqlstr.c_str(), sizeof(sql));
		return std::move(SelectRecipe(sql, colname.size()));
	}
	/**
	 * @brief  : ��ȡ�䷽��ؼ��ּ�¼
	 * @params : recipename	 �䷽����
	 * @params : key		 �ؼ���
	 */
	vector<vector<string>> RecipeStorageService::SelectRecipeRecordByKey(string recipename, string key, vector<string> colname) {
		char sql[SQLCMDLEN] = { 0 };
		string sqlstr = "SELECT * FROM " + recipename + " WHERE";
		for (size_t i = 0; i < colname.size(); i++)
		{
			sqlstr = sqlstr + " " + colname[i] + " LIKE " + "'%" + key + "%'";
			if (i != colname.size() - 1)
				sqlstr += " OR";
		}
		memcpy(sql, sqlstr.c_str(), sizeof(sql));
		return std::move(SelectRecipe(sql, colname.size()));
	}

	/**
	 * @brief  : ��ȡ�䷽�����䷽��Ŀ����
	 * @params : recipename	�䷽������
	 * @return : int
	 */
	int RecipeStorageService::GetRecipeDataNum(string recipename) {
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s", recipename.c_str());
		int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return -1;
		}
		int count = 0;
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW) {
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		if (isOpened) {
			sqlite3_close_v2(db);
			isOpened = false;
			Destroy();
		}
		return count;
	}
}

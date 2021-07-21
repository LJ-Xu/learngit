#include "IResourceService.h"
#include "CodeFormatUtility.h"
#include "RecipeStorageService.h"
#include "RunEnv.h"
#include <string.h>

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
				string data;
				if (!str)
					data = "";
				else
					data = (const char*)str;
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

	vector<string> RecipeStorageService::SelectRowRecipe(const char * sql, int col)
	{
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
			return vector<string>();
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
		if (!records.empty())
			return records[0];
		return vector<string>();
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
	vector<string> RecipeStorageService::SelectRecipeRecordByRow(string recipename,int row, int cols) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM %s WHERE ��� == '%s';",
			recipename.c_str(),to_string(row).c_str());
		return std::move(SelectRowRecipe(sql, cols));
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

	bool RecipeStorageService::OrderRecipeRecord(string recipename, int mode)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		
		if (mode)	//����
			snprintf(sql, sizeof(sql), "ISELECT * FROM %s ORDER BY ��� DESC", recipename.c_str());
		else		//˳��
			snprintf(sql, sizeof(sql), "ISELECT * FROM %s ORDER BY ��� ASC", recipename.c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return true;
	}

	bool RecipeStorageService::AddRecipeRecord(string recipename, int row)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "INSERT INTO %s (���) VALUES (%s)", recipename.c_str(), to_string(row).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return  OrderRecipeRecord(recipename);
	}
	bool RecipeStorageService::InsertRecipeRecord(string recipename, int row)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		/*��Id�����*/
		snprintf(sql, sizeof(sql), "UPDATE %s SET ��� = ��� + '1' WHERE ��� > '%s'", recipename.c_str(), to_string(row).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		/*����һ��*/
		return AddRecipeRecord(recipename,row + 1);
	}
	bool RecipeStorageService::DeleteRecipeRecord(string recipename, int row)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE ��� = '%s';UPDATE %s SET ��� = ��� - '1' WHERE ��� > '%s'",
			recipename.c_str(), to_string(row).c_str(), recipename.c_str(), to_string(row).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return OrderRecipeRecord(recipename);
	}

	bool RecipeStorageService::CopyRecipeRecord(string recipename, int row, vector<string>& data, vector<Project::ColDataTypeInfo>& colnames)
	{
		if (InsertRecipeRecord(recipename, row))		//����һ��
		{
			char sql[SQLCMDLEN] = { 0 };
			struct sqlite3_stmt *stmt;
			string sqlstr = "UPDATE " + recipename + " SET";
			int size = colnames.size() > data.size() ? data.size() : colnames.size();
			for (size_t i = 0; i < size; i++)
			{
				sqlstr = sqlstr + " " + colnames[i].ColTitle + " = " + data[i];
				if (i != size - 1)
					sqlstr += " ,";
			}
			sqlstr += " WHERE ��� = '" + to_string(row + 1) + "'";
			//memcpy(sql, sqlstr.c_str(), sizeof(sql));
			//string sqlstr = sql;
			if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
				UI::IResourceService::GB2312toUtf8(sqlstr);
			char * errMsg = NULL;
			int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
			//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
			if (ret != SQLITE_OK) {
				fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
				if (isOpened) {
					sqlite3_close_v2(db);
					isOpened = false;
					Destroy();
				}
				return false;
			}
			return OrderRecipeRecord(recipename);
			row + 1;
		}
		return false;
	}

	bool RecipeStorageService::MoveUpRecipeRecord(string recipename, int row)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "UPDATE %s SET ��� = 't' WHERE ��� = '%s';\
			UPDATE %s SET ��� = '%s' WHERE ��� = '%s';\
			UPDATE %s SET ��� = '%s' WHERE ��� = 't'",
			recipename.c_str(), to_string(row).c_str(), 
			recipename.c_str(), to_string(row).c_str(), to_string(row-1).c_str(),
			recipename.c_str(), to_string(row-1).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return OrderRecipeRecord(recipename);
	}
	bool RecipeStorageService::MoveDownRecipeRecord(string recipename, int row)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "UPDATE %s SET ��� = 't' WHERE ��� = '%s';\
			UPDATE %s SET ��� = '%s' WHERE ��� = '%s';\
			UPDATE %s SET ��� = '%s' WHERE ��� = 't'",
			recipename.c_str(), to_string(row).c_str(),
			recipename.c_str(), to_string(row).c_str(), to_string(row + 1).c_str(),
			recipename.c_str(), to_string(row + 1).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return OrderRecipeRecord(recipename);
	}
	bool RecipeStorageService::UpdateRecipeRecord(string recipename, string colname, int rowcol, string data)
	{
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "UPDATE %s SET %s = '%s' WHERE ��� = '%s';",
			recipename.c_str(), colname.c_str(), data.c_str(), to_string(rowcol).c_str());
		string sqlstr = sql;
		if (!UI::CodeFormatUtility::IsStrUtf8(sqlstr.c_str()))
			UI::IResourceService::GB2312toUtf8(sqlstr);
		char * errMsg = NULL;
		int ret = sqlite3_exec(db, sqlstr.c_str(), NULL, NULL, &errMsg);
		//int ret = sqlite3_prepare_v2(db, sqlstr.c_str(), sqlstr.size(), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			if (isOpened) {
				sqlite3_close_v2(db);
				isOpened = false;
				Destroy();
			}
			return false;
		}
		return true;
	}
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseStorageService.cpp
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : 数据库操作接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BaseStorageService.h"
#include <string.h>

namespace Storage
{
	BaseStorageService::BaseStorageService(const string & dbPath, const string & tbname) :dbPath(dbPath), tbName(tbname) ,curId(1){
		// 打开内存数据库
		Open();
	}

	BaseStorageService::~BaseStorageService() {
		// 关闭数据库
		Close();
	}

	/**
	 * @brief  : 打开数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Open() {
		int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI;
		std::string sql; 
		sql.append("file:").append(dbPath).append("?mode=memory&cache=shared");
		//sql.append("file:").append(tbName).append("?mode=memory&cache=shared"); 
		int ret = sqlite3_open_v2(sql.c_str(), &db, flags, NULL);
		if (ret)
		{

		}
		isOpened = (ret == SQLITE_OK);


		return ret;
	}

	/**
	 * @brief  : 关闭数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void BaseStorageService::Close() {
		ExecCommit();
		if (isOpened) {
			sqlite3_close_v2(db);
			isOpened = false;
		}
		for (auto itor = StoreObj.begin(); itor != StoreObj.end(); ++itor)
		{
			sqlite3_finalize(itor->second);
		}
	}

	/**
	 * @brief  : 备份文件数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Flush() {
#if 0
		char sql[512] = { 0 };
		// 备份文件数据库
		return Flush(Count());
#else
		sqlite3_stmt* stmt = GetSTMT(BASE_FLUSH);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		ret = sqlite3_step(stmt);
		stmt = GetSTMT(BASE_DELETE);
		if (stmt == nullptr)
			return -1;
		ret = sqlite3_reset(stmt);
		ret = sqlite3_step(stmt);
#endif
	}

	/**
	 * @brief  : 备份文件数据库
	 * @params : count 备份数据数量
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Flush(int count) {
		char sql[512] = { 0 };
		//// 清除文件数据库历史数据
		//// sqlite3_busy_timeout(db, 2000);
		//snprintf(sql, sizeof(sql), "DELETE FROM fileDb.%s WHERE ID IN (SELECT ID FROM fileDb.%s LIMIT %d)", tbName.c_str(), tbName.c_str(), count);
		//int ret = ExecuteSql(sql);
		//if (ret != SQLITE_OK) {
		//	return ret;
		//}
		// 备份指定数量数据
		ExecCommit();
		snprintf(sql, sizeof(sql), "INSERT OR REPLACE INTO fileDb.%s SELECT * FROM %s LIMIT 0, %d", tbName.c_str(), tbName.c_str(), count);
		int ret = ExecuteSql(sql);
		ExecBegin();
		if (ret != SQLITE_OK) {
			return ret;
		}
		// 删除内存数据库数据
		snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE ID IN (SELECT ID FROM %s LIMIT %d)", tbName.c_str(), tbName.c_str(), count);
		ret = ExecuteSql(sql);
		return ret;
	}

	/**
	 * @brief  : 内存数据库数据量
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Count() {
		char sql[512] = { 0 };
		struct sqlite3_stmt * stmt;
		snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s;", tbName.c_str());
		int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			return -1;
		}
		int count = 0;
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW) {
			// 获取数据库行数
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		return count;
	}
	bool BaseStorageService::NewFMT(char key, const char * sql, int len)
	{
		sqlite3_stmt *stmt;
		int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, 0);
		auto itorret = StoreObj.insert(std::make_pair(key, stmt));
		return itorret.second;
	}
	bool BaseStorageService::FinFMT(char key)
	{
		sqlite3_stmt *stmt = GetSTMT(key);
		if (stmt != nullptr)
		{
			sqlite3_finalize(stmt);
			StoreObj.erase(key);
		}
		return false;
	}
	int BaseStorageService::Attach()
	{
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql), "ATTACH '%s' AS fileDb;", dbPath.c_str());
		// 附加文件数据库
		int ret = ExecuteSql(sql);
		if (ret != SQLITE_OK) {
			Close();
		}

		std::string sqlpre;

		sqlpre.clear();
		sqlpre.append("INSERT OR REPLACE INTO fileDb.").append(tbName).append(" SELECT * FROM ").append(tbName);
		if (!NewFMT(BASE_FLUSH, sqlpre.c_str(), sizeof(sqlpre)))
			return BASE_FLUSH;


		sqlpre.clear();
		sqlpre.append("DELETE FROM  ").append(tbName);
		if (!NewFMT(BASE_DELETE, sqlpre.c_str(), sizeof(sqlpre)))
			return BASE_DELETE;

		return ret;

	}
	sqlite3_stmt * BaseStorageService::GetSTMT(char key)
	{
		sqlite3_stmt* stmt = nullptr;
		try {
			stmt = StoreObj.at(key);
		}
		catch (const std::out_of_range &e) {
			stmt = nullptr;
			std::cout << "Out of range while Getstmt,please check your initlize of stmt in Create()." << e.what();
			return nullptr;
		}
		return stmt;
	}
	void BaseStorageService::ExecBegin()
	{
		sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
	}
	void BaseStorageService::ExecCommit()
	{
		sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
	}
	int BaseStorageService::ExecuteSql(const char * sql)
	{
		// 校验参数合法性
		if (sql == nullptr || strlen(sql) == 0) {
			//const char * file = strrchr(__FILE__, '\\') + 1;
			//fprintf(stderr, "%s[%d] Sql statements is empty\n", file, __LINE__);
			return -1;
		}
		char * errMsg = NULL;
		// 执行Sql语句
		//printf(sql);
		//printf("\n");
		int ret = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
		if (ret != SQLITE_OK) {
			//const char * file = strrchr(__FILE__, '\\') + 1;
			//fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", __LINE__, ret, errMsg);
		}
		//printf("sql exec success!\n");
		return ret;
	}
}

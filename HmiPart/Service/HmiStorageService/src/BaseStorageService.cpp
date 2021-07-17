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
	BaseStorageService::BaseStorageService(const char* tbname) : tbName(tbname) {
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
		sql.append("file:").append("DB").append("?mode=memory&cache=shared");
		int ret = sqlite3_open_v2(sql.c_str(), &db, flags, NULL);
		isOpened = (ret == SQLITE_OK);
		return ret;
	}

	/**
	 * @brief  : 关闭数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void BaseStorageService::Close() {
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
		char sql[512] = { 0 };
		// 备份文件数据库
		return Flush(Count());
	}

	/**
	 * @brief  : 备份文件数据库
	 * @params : count 备份数据数量
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Flush(int count) {
		return 0;
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
}

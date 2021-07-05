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
	BaseStorageService::BaseStorageService(const string & dbPath, const string & tbName, int maxCnt) : dbPath(dbPath), tbName(tbName) {
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
		// 打开内存数据库
		int ret = sqlite3_open(":memory:", &db);
		if (ret != SQLITE_OK) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", file, __LINE__, ret, sqlite3_errmsg(db));
			sqlite3_close(db);
		}
		// 数据库是否打开
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
	}

	/**
	 * @brief  : 执行sql语句
	 * @params : sql	 SQL语句
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::ExecuteSql(const char * sql) {
		// 校验参数合法性
		if (sql == nullptr || strlen(sql) == 0) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] Sql statements is empty\n", file, __LINE__);
			return -1;
		}
		char * errMsg = NULL;
		// 执行Sql语句
		printf(sql);
		printf("\n");
		int ret = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
		if (ret != SQLITE_OK) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", file, __LINE__, ret, errMsg);
		}
		printf("sql exec success!\n");
		return ret;
	}

	/**
	 * @brief  : 附加文件数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Attach() {
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql), "ATTACH '%s' AS fileDb;", dbPath.c_str());
		// 附加文件数据库
		int ret = ExecuteSql(sql);
		if (ret != SQLITE_OK) {
			Close();
		}
		return ret;
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
		char sql[512] = { 0 };
		// 清除文件数据库历史数据
		// sqlite3_busy_timeout(db, 2000);
		snprintf(sql, sizeof(sql), "DELETE FROM fileDb.%s WHERE ID IN (SELECT ID FROM fileDb.%s LIMIT %d)", tbName.c_str(), tbName.c_str(), count);
		int ret = ExecuteSql(sql);
		if (ret != SQLITE_OK) {
			return ret;
		}
		// 备份指定数量数据
		snprintf(sql, sizeof(sql), "INSERT OR REPLACE INTO fileDb.%s SELECT * FROM %s LIMIT 0, %d", tbName.c_str(), tbName.c_str(), count);
		ret = ExecuteSql(sql);
		if (ret != SQLITE_OK) {
			return ret;
		}
		// 删除内存数据库数据
		snprintf(sql, sizeof(sql), "DELETE FROM %s WHERE ID IN (SELECT ID FROM %s LIMIT %d)", tbName.c_str(), tbName.c_str(), count);
		return ExecuteSql(sql);
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
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseStorageService.cpp
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : ���ݿ�����ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BaseStorageService.h"
#include <string.h>

namespace Storage
{
	BaseStorageService::BaseStorageService(const char* tbname) : tbName(tbname) {
		// ���ڴ����ݿ�
		Open();
	}

	BaseStorageService::~BaseStorageService() {
		// �ر����ݿ�
		Close();
	}

	/**
	 * @brief  : �����ݿ�
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
	 * @brief  : �ر����ݿ�
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
	 * @brief  : �����ļ����ݿ�
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Flush() {
		char sql[512] = { 0 };
		// �����ļ����ݿ�
		return Flush(Count());
	}

	/**
	 * @brief  : �����ļ����ݿ�
	 * @params : count ������������
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int BaseStorageService::Flush(int count) {
		return 0;
	}

	/**
	 * @brief  : �ڴ����ݿ�������
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
			// ��ȡ���ݿ�����
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

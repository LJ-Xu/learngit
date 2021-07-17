/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorageService.cpp
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : 操作记录数据库接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "OperatorStorageService.h"

namespace Storage
{
	OperatorStorageService * OperatorStorageService::ins = nullptr;

	OperatorStorageService::OperatorStorageService() : 
		//BaseStorageService("././HMI/Operation.db", "Operation") {
		BaseStorageService(RunEnv::Cnf.OperationPath, "Operation") {
		//BaseStorageService("..\\..\\HMI\\Operation.db", "Operation") {
		// 创建内存数据库
		if (Create())
			Close();
	}

	OperatorStorageService * OperatorStorageService::Ins() {
		static once_flag flag;
		call_once(flag, [&]() {
			ins = new OperatorStorageService();
		});
		return ins;
	}

	/**
	 * @brief  : 释放操作记录数据库
	 * @return : void
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void OperatorStorageService::Destroy() {
		if (ins) {
			delete ins;
			ins = nullptr;
		}
	}

	/**
	 * @brief  : 创建操作记录数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::Create() {
		std::string sql;
		sql.append("CREATE TABLE ").append(tbName);
		//sql.append("(ID INTEGER primary key AUTOINCREMENT,");
		sql.append("(ID INT64 NOT NULL,");
		sql.append("Date INT64 NOT NULL,");
		sql.append("Time INT64 NOT NULL,");
		sql.append("UserName Text NOT NULl,");
		sql.append("Class Text NOT NULL,");
		sql.append("Window INT NOT NULL,");
		sql.append("ObjectName Text NOT NULL,");
		sql.append("Comment Text NOT NULL,");
		sql.append("Action Text NOT NULL,");
		sql.append("Address Text NOT NULL,");
		sql.append("Information Text NOT NULL);");
		char* errmsg;
		int ret = sqlite3_exec(db, sql.c_str(), 0, 0, &errmsg);
		if (ret != SQLITE_OK)
		{
			std::cout << "Operatedb create fail: " << sqlite3_errmsg(db);
			return ret;
		}
		Attach();
		//Prepare
		Init();
		return ret;
	}

	int OperatorStorageService::Init()
	{
		std::string sql;
		//Insert
		{
			sql.clear();
			sql.append("INSERT INTO ").append(tbName).append(" VALUES(?,?,?,?,?,?,?,?,?,?,?)");
			if (!NewFMT(INS_InsertRecord, sql.c_str(), sizeof(sql)))
				return INS_InsertRecord;
		}
		//Delete
		{
			sql.clear();
			sql.append("DELETE FROM ").append(tbName);
			if (!NewFMT(DEL_DeleteRecords, sql.c_str(), sizeof(sql)))
				return DEL_DeleteRecords;
		}
		//Update
		{
			sql.clear();
			sql.append("UPDATE ").append(tbName).append(" SET Date = ?,Time = ?,UserName = '?',Class = '?',Window = ?,ObjectName = '?',Comment = '?',Action = '?',Address = '?',Information = '?' WHERE ID = ?;");
			if (!NewFMT(UPD_UpdateRecord, sql.c_str(), sizeof(sql)))
				return UPD_UpdateRecord;
		}
		//Select
		{
			sql.clear();
			sql.append("SELECT * FROM ").append(tbName);
			if (!NewFMT(SEL_SelectRecords, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecords;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ID >= ?;");
			if (!NewFMT(SEL_SelectRecordsFromId, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecordsFromId;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append("  LIMIT ?,?;");
			if (!NewFMT(SEL_SelectRecordsFromIdLimitByCount, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecordsFromIdLimitByCount;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append("   WHERE UserName = '?';");
			if (!NewFMT(SEL_SelectRecordsByUserName, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecordsByUserName;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append("   WHERE Date BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectRecordsByDate, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecordsByDate;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append("   WHERE Date BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectRecordsByTime, sql.c_str(), sizeof(sql)))
				return SEL_SelectRecordsByTime;
		}
		sqlite3_exec(db, "PRAGMA synchronous = OFF;", NULL, NULL, NULL);
		ExecBegin();
		return 0;
	}

	/**
	 * @brief  : 添加操作记录
	 * @params : record 操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::InsertOperatorRecord(OperatorRecord & record) {
		sqlite3_stmt* stmt = GetSTMT(INS_InsertRecord);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			int i = 1;
			sqlite3_bind_int(stmt, i++, record.ID);
			sqlite3_bind_int64(stmt, i++, record.Date);
			sqlite3_bind_int64(stmt, i++, record.Time);
			sqlite3_bind_text(stmt, i++, record.UserName.c_str(), record.UserName.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Class.c_str(), record.Class.size(), nullptr);
			sqlite3_bind_int(stmt, i++, record.Window);
			sqlite3_bind_text(stmt, i++, record.CtrlName.c_str(), record.CtrlName.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Comment.c_str(), record.Comment.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Action.c_str(), record.Action.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Address.c_str(), record.Address.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Information.c_str(), record.Information.size(), nullptr);
			ret = sqlite3_step(stmt);
			if (!ret)
				curId++;
		}
		return ret;
	}

	/**
	 * @brief  : 删除操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::DeleteOperatorRecords() {
		sqlite3_stmt* stmt = GetSTMT(DEL_DeleteRecords);
		if (stmt == nullptr)
			return -1;
		return sqlite3_step(stmt);
	}

	/**
	 * @brief  : 更新操作记录
	 * @params : id 操作记录编号
	 * @params : record 操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::UpdateOperatorRecord(int id, OperatorRecord & record) {
		sqlite3_stmt* stmt = GetSTMT(UPD_UpdateRecord);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			int i = 1;
			sqlite3_bind_int64(stmt, i++, record.Date);
			sqlite3_bind_int64(stmt, i++, record.Time);
			sqlite3_bind_text(stmt, i++, record.UserName.c_str(), record.UserName.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Class.c_str(), record.Class.size(), nullptr);
			sqlite3_bind_int(stmt, i++, record.Window);
			sqlite3_bind_text(stmt, i++, record.CtrlName.c_str(), record.CtrlName.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Comment.c_str(), record.Comment.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Action.c_str(), record.Action.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Address.c_str(), record.Address.size(), nullptr);
			sqlite3_bind_text(stmt, i++, record.Information.c_str(), record.Information.size(), nullptr);
			sqlite3_bind_int(stmt, i++, record.ID);
			ret = sqlite3_step(stmt);
		}
		return ret;
	}

	/**
	 * @brief  : 获取操作记录
	 * @params : sql	 SQL语句
	 * @params : records 操作记录集合
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::SelectRecords(sqlite3_stmt *stmt, vector<OperatorRecord> & records) {
		int ret = 0;
		// 解析结果集
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			OperatorRecord record;
			record.ID = sqlite3_column_int(stmt, 0);					// 操作记录编号
			record.Date = sqlite3_column_int64(stmt, 1);				// 操作日期
			record.Time = sqlite3_column_int64(stmt, 2);				// 操作时间
			record.UserName = (char *)sqlite3_column_text(stmt, 3);		// 用户名称
			record.Class = (char *)sqlite3_column_text(stmt, 4);		// 操作权限
			record.Window = sqlite3_column_int(stmt, 5);				// 操作窗口
			record.CtrlName = (char *)sqlite3_column_text(stmt, 6);	// 操作对象
			record.Comment = (char *)sqlite3_column_text(stmt, 7);		// 对象描述
			record.Action = (char *)sqlite3_column_text(stmt, 8);		// 操作动作
			record.Address = (char *)sqlite3_column_text(stmt, 9);		// 操作地址
			record.Information = (char *)sqlite3_column_text(stmt, 10);	// 操作信息
			records.push_back(record);
		}
		// 释放对象
		//sqlite3_finalize(stmt);
		return 0;
	}

	int OperatorStorageService::SelectOperatorRecords(vector<OperatorRecord> & records) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecords);
		if (stmt == nullptr)
			return -1;
		return SelectRecords(stmt, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByLimit(int startIndex, int count, vector<OperatorRecord>& records)
	{
		if (count == 0)
		{
			sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecordsFromId);
			if (stmt == nullptr)
				return -1;
			sqlite3_bind_int(stmt, 1, startIndex);
			return SelectRecords(stmt, records);
		}
		else
		{
			sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecordsFromIdLimitByCount);
			if (stmt == nullptr)
				return -1;
			sqlite3_bind_int(stmt, 1, startIndex);
			sqlite3_bind_int(stmt, 2, count);
			return SelectRecords(stmt, records);
		}
	}

	int OperatorStorageService::SelectOperatorRecordsByUser(const char * userName, vector<OperatorRecord> & records) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecordsByUserName);
		if (stmt == nullptr)
			return -1;
		sqlite3_bind_text(stmt,1, userName,strlen(userName),nullptr);
		return SelectRecords(stmt, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByDate(DDWORD sDate, DDWORD eDate, vector<OperatorRecord> & records) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecordsByDate);
		if (stmt == nullptr)
			return -1;
		sqlite3_bind_int64(stmt, 1, sDate);
		sqlite3_bind_int64(stmt, 2, eDate);
		return SelectRecords(stmt, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByTime(DDWORD sTime, DDWORD eTime, vector<OperatorRecord> & records) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectRecordsByTime);
		if (stmt == nullptr)
			return -1;
		sqlite3_bind_int64(stmt, 1, sTime);
		sqlite3_bind_int64(stmt, 2, eTime);
		return SelectRecords(stmt, records);
	}
}


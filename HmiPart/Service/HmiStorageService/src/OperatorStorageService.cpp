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
		// 附加文件数据库
		Attach();
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
		int ret = 0;
		char sql[512] = { 0 };
		// 操作记录内存表SQL语句
		snprintf(sql, sizeof(sql), 
			"CREATE TABLE Operation ("
			"\n\tID INT NOT NULL,"
			"\n\tDate INT64 NOT NULL,"
			"\n\tTime INT64 NOT NULL,"
			"\n\tUserName Text NOT NULl,"
			"\n\tClass Text NOT NULL,"
			"\n\tWindow INT NOT NULL,"
			"\n\tObjectName Text NOT NULL,"
			"\n\tComment Text NOT NULL,"
			"\n\tAction Text NOT NULL,"
			"\n\tAddress Text NOT NULL,"
			"\n\tInformation Text NOT NULL);");
		// 创建操作记录内存表
		ret = ExecuteSql(sql);
		return ret;
	}

	/**
	 * @brief  : 添加操作记录
	 * @params : record 操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::InsertOperatorRecord(OperatorRecord & record) {
		char sql[512] = { 0 };
		// 添加操作记录
		snprintf(sql, sizeof(sql), "INSERT INTO Operation VALUES("
			"%d, %lld, %lld, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s');",
			record.ID, record.Date, record.Time,
			record.UserName.c_str(), record.Class.c_str(),
			record.Window, record.CtrlName.c_str(),
			record.Comment.c_str(), record.Action.c_str(),
			record.Address.c_str(), record.Information.c_str());
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 删除操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::DeleteOperatorRecords() {
		char sql[512] = { 0 };
		// 删除全部操作记录
		snprintf(sql, sizeof(sql), "DELETE FROM Operation;DELETE FROM fileDb.Operation;");
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 更新操作记录
	 * @params : id 操作记录编号
	 * @params : record 操作记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::UpdateOperatorRecord(int id, OperatorRecord & record) {
		char sql[512] = { 0 };
		// 更新指定操作记录
		snprintf(sql, sizeof(sql), 
			"UPDATE Operation SET Date = %lld, "
			"Time = %lld, "
			"UserName = '%s', "
			"Class = '%s', "
			"Window = %d, "
			"ObjectName = '%s', "
			"Comment = '%s', "
			"Action = '%s', "
			"Address = '%s', "
			"Information = '%s' WHERE ID = %d;"
			"UPDATE fileDb.Operation SET Date = %lld, "
			"Time = %lld, "
			"UserName = '%s', "
			"Class = '%s', "
			"Window = %d, "
			"ObjectName = '%s', "
			"Comment = '%s', "
			"Action = '%s', "
			"Address = '%s', "
			"Information = '%s' WHERE ID = %d;",
			record.Date, record.Time, record.UserName.c_str(), record.Class.c_str(),
			record.Window, record.CtrlName.c_str(), record.Comment.c_str(),
			record.Action.c_str(), record.Address.c_str(), record.Information.c_str(), record.ID,
			record.Date, record.Time, record.UserName.c_str(), record.Class.c_str(),
			record.Window, record.CtrlName.c_str(), record.Comment.c_str(),
			record.Action.c_str(), record.Address.c_str(), record.Information.c_str(), record.ID);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 获取操作记录
	 * @params : sql	 SQL语句
	 * @params : records 操作记录集合
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int OperatorStorageService::SelectRecords(const char * sql, vector<OperatorRecord> & records) {
		int ret = 0;
		struct sqlite3_stmt * stmt;
		// 获取操作记录
		ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", file, __LINE__, ret, sqlite3_errmsg(db));
			return ret;
		}
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
		sqlite3_finalize(stmt);
		return 0;
	}

	int OperatorStorageService::SelectOperatorRecords(vector<OperatorRecord> & records) {
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Operation union all SELECT * FROM fileDb.Operation;");
		return SelectRecords(sql, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByLimit(int startIndex, int count, vector<OperatorRecord>& records)
	{
		char sql[512] = { 0 };
		if(count ==0)
			snprintf(sql, sizeof(sql), "SELECT * FROM Operation union all SELECT * FROM fileDb.Operation;",startIndex);
		else
			snprintf(sql, sizeof(sql), "SELECT * FROM Operation union all SELECT * FROM fileDb.Operation;", startIndex, count);
		return SelectRecords(sql, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByUser(const char * userName, vector<OperatorRecord> & records) {
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql), 
			"SELECT * FROM Operation WHERE UserName = '%s' union all "
			"SELECT * FROM fileDb.Operation WHERE UserName = '%s';",
			userName, userName);
		return SelectRecords(sql, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByDate(DDWORD sDate, DDWORD eDate, vector<OperatorRecord> & records) {
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql), 
			"SELECT * FROM Operation WHERE Date BETWEEN %lld AND %lld union all "
			"SELECT * FROM fileDb.Operation WHERE Date BETWEEN %lld AND %lld;",
			sDate, eDate, sDate, eDate);
		return SelectRecords(sql, records);
	}

	int OperatorStorageService::SelectOperatorRecordsByTime(DDWORD sTime, DDWORD eTime, vector<OperatorRecord> & records) {
		char sql[512] = { 0 };
		snprintf(sql, sizeof(sql),
			"SELECT * FROM Operation WHERE Time BETWEEN %lld AND %lld union all "
			"SELECT * FROM fileDb.Operation WHERE Time BETWEEN %lld AND %lld;",
			sTime, eTime, sTime, eTime);
		return SelectRecords(sql, records);
	}
}


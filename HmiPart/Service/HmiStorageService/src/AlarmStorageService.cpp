#include "AlarmStorageService.h"
//#include "Logger.h"
namespace Storage
{
	AlarmStorageService * AlarmStorageService::ins = nullptr;

	AlarmStorageService::AlarmStorageService() : 
		BaseStorageService(RunEnv::Cnf.AlarmPath,"Alarm") {
		// 创建内存数据库
		if (Create()) {
			Close();
		}
	}

	AlarmStorageService * AlarmStorageService::Ins() {
		static once_flag flag;
		call_once(flag, [&]() {
			ins = new AlarmStorageService();
		});
		return ins;
	}

	/**
	 * @brief  : 释放报警数据库
	 * @return : void
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void AlarmStorageService::Destroy() {
		if (ins) {
			delete ins;
			ins = nullptr;
		}
	}

	/**
	 * @brief  : 创建报警录入内存数据库
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::Create() {

		std::string sql;
		sql.append("CREATE TABLE ").append(tbName);
		//sql.append("(Id INT64 primary key AUTOINCREMENT,");
		sql.append("(Id INT64 NOT NULL,");
		sql.append("AlarmGroup INT NOT NULL,");
		sql.append("AlarmNo INT NOT NULL,");
		sql.append("StartTime INT64 NOT NULL,");
		sql.append("Title Text NOT NULl,");
		sql.append("Times INT NOT NULL,");
		sql.append("ResolveTime INT64 NOT NULL,");
		sql.append("CheckTime INT64 NOT NULL,");
		sql.append("Level INT NOT NULL);");
		char* errmsg;
		int ret = sqlite3_exec(db, sql.c_str(), 0, 0, &errmsg);
		if (ret != SQLITE_OK)
		{
			std::cout << "Alarmdb create fail: " << sqlite3_errmsg(db);
			return ret;
		}
		Attach();
		//Prepare
		Init();
		return ret;
	}

	int AlarmStorageService::Init()
	{
		std::string sql;
		//INSERT
		{
			sql.clear();
			sql.append("INSERT INTO ").append(tbName).append(" VALUES(?,?,?,?,?,?,?,?,?)");
			if (!NewFMT(INS_InsertAlarmRecord, sql.c_str(), sizeof(sql)))
				return INS_InsertAlarmRecord;
		}
		//UPDATE
		{
			sql.clear();
			sql.append("UPDATE ").append(tbName).append(" SET StartTime = ?, Title = '?', Times = ?, ResolveTime = ?,CheckTime = ?, Level = ? WHERE AlarmGroup = ? AND AlarmNo = ?;");
			if (!NewFMT(UPD_UpdateAlarmRecord, sql.c_str(), sizeof(sql)))
				return UPD_UpdateAlarmRecord;

			sql.clear();
			sql.append("UPDATE ").append(tbName).append(" SET StartTime = ?, Title = '?', Times = ?, CheckTime = ?, Level = ? WHERE AlarmGroup = ? AND AlarmNo = ?;");
			if (!NewFMT(UPD_UpdateAlarmRecordByConfirm, sql.c_str(), sizeof(sql)))
				return UPD_UpdateAlarmRecordByConfirm;

			sql.clear();
			sql.append("UPDATE ").append(tbName).append(" SET StartTime = ?, Title = '?', Times = ?, ResolveTime = ?, Level = ? WHERE AlarmGroup = ? AND AlarmNo = ?;");
			if (!NewFMT(UPD_UpdateAlarmRecordByResolve, sql.c_str(), sizeof(sql)))
				return UPD_UpdateAlarmRecordByResolve;
		}
		//SELECT
		{
			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" union all SELECT * FROM fileDb.").append(tbName);
			if (!NewFMT(SEL_SelectAllAlarmRecords, sql.c_str(), sizeof(sql)))
				return SEL_SelectAllAlarmRecords;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectAllRecordsRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectAllRecordsRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? ;");
			if (!NewFMT(SEL_SelectByNameAndNo, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNo;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime <> 0;").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ?;");
			if (!NewFMT(SEL_SelectByNameAndNoAndUnResolved, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNoAndUnResolved;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime = 0").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime = 0;");
			if (!NewFMT(SEL_SelectByNameAndNoAndResolved, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNoAndResolved;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByNameAndNoRangeByGroupName, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNoRangeByGroupName;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByNameAndNoAndUnResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNoAndUnResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmNo = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByNameAndNoAndResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNameAndNoAndResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByGroupRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByGroupRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByGroupAndUnResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByGroupAndUnResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmGroup = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmGroup = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByGroupAndResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByGroupAndResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmNo = %d AND AlarmGroup BETWEEN %d AND %d").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmNo = %d AND AlarmGroup BETWEEN %d AND %d;");
			if (!NewFMT(SEL_SelectByNoRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNoRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmNo = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmNo = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByNoAndUnResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNoAndUnResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE Level = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE Level = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByNoAndResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByNoAndResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE AlarmNo = %d AND AlarmGroup BETWEEN %d AND %d").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE AlarmNo = %d AND AlarmGroup BETWEEN %d AND %d;");
			if (!NewFMT(SEL_SelectByLevRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByLevRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE Level = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE Level = ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByLevAndUnResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByLevAndUnResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE Level = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE Level = ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectByLevAndResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectByLevAndResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectCheckedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectCheckedRangeByGroup;
			
			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ResolveTime = 0 OR CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ResolveTime = 0 OR CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectCheckedOrResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectCheckedOrResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectUnResolvedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectUnResolvedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ? OR CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ? OR CheckTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectUnResolvedOrCheckedRangeByGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectUnResolvedOrCheckedRangeByGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectRangeTimeAndGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectRangeTimeAndGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND ResolveTime <> 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectResolvedRangeTimeAndGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectResolvedRangeTimeAndGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE StartTime BETWEEN ? AND ? AND ResolveTime = 0 AND AlarmGroup BETWEEN ? AND ?;");
			if (!NewFMT(SEL_SelectUnResolvedRangeTimeAndGroup, sql.c_str(), sizeof(sql)))
				return SEL_SelectUnResolvedRangeTimeAndGroup;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ResolveTime = 0").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ResolveTime = 0;");
			if (!NewFMT(SEL_SelectAlarmRecordByResolveTick, sql.c_str(), sizeof(sql)))
				return SEL_SelectAlarmRecordByResolveTick;
		}
		sqlite3_exec(db, "PRAGMA synchronous = OFF;", NULL, NULL, NULL);
		ExecBegin();
		return 0;
	}

	/**
	 * @brief  : 添加报警记录
	 * @params : record 报警记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::InsertAlarmRecord(AlarmRecord & record) {
		
		sqlite3_stmt* stmt = GetSTMT(INS_InsertAlarmRecord);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			int idx = 1;
			sqlite3_bind_int64(stmt, idx++, curId);
			sqlite3_bind_int(stmt, idx++, record.AlarmGroup);
			sqlite3_bind_int(stmt, idx++, record.AlarmNo);
			sqlite3_bind_int64(stmt, idx++, record.StartTick);
			sqlite3_bind_text(stmt, idx++, record.Title.c_str(), record.Title.size(),nullptr);
			sqlite3_bind_int(stmt, idx++, record.Times);
			sqlite3_bind_int64(stmt, idx++, record.ResolveTick);
			sqlite3_bind_int64(stmt, idx++, record.CheckTick);
			sqlite3_bind_int(stmt, idx++, record.Level);
			ret = sqlite3_step(stmt);
			if (!ret)
				curId++;
		}
		return ret;
	}

	/**
	 * @brief  : 更新报警记录
	 * @params : alarmGroup 报警组别
	 * @params : record 报警记录
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo,const AlarmRecord & record) {
		sqlite3_stmt* stmt = GetSTMT(UPD_UpdateAlarmRecord);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int64(stmt, 1, record.StartTick);
			sqlite3_bind_text(stmt, 2, record.Title.c_str(), record.Title.size(), nullptr);
			sqlite3_bind_int(stmt, 3, record.Times);
			sqlite3_bind_int64(stmt, 4, record.ResolveTick);
			sqlite3_bind_int64(stmt, 5, record.CheckTick);
			sqlite3_bind_int(stmt, 6, record.Level);
			sqlite3_bind_int(stmt, 7, record.AlarmGroup);
			sqlite3_bind_int(stmt, 8, record.AlarmNo);
			ret = sqlite3_step(stmt);
		}
		return ret;
	}

	int AlarmStorageService::UpdateAlarmRecordByConfirm(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record)
	{
		sqlite3_stmt* stmt = GetSTMT(UPD_UpdateAlarmRecordByConfirm);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int64(stmt, 1, record.StartTick);
			sqlite3_bind_text(stmt, 2, record.Title.c_str(), record.Title.size(), nullptr);
			sqlite3_bind_int(stmt, 3, record.Times);
			sqlite3_bind_int64(stmt, 4, record.CheckTick);
			sqlite3_bind_int(stmt, 5, record.Level);
			sqlite3_bind_int(stmt, 6, record.AlarmGroup);
			sqlite3_bind_int(stmt, 7, record.AlarmNo);
			ret = sqlite3_step(stmt);
		}
		return ret;
	}

	int AlarmStorageService::UpdateAlarmRecordByResolve(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record)
	{
		sqlite3_stmt* stmt = GetSTMT(UPD_UpdateAlarmRecordByResolve);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int64(stmt, 1, record.StartTick);
			sqlite3_bind_text(stmt, 2, record.Title.c_str(), record.Title.size(), nullptr);
			sqlite3_bind_int(stmt, 3, record.Times);
			sqlite3_bind_int64(stmt, 4, record.ResolveTick);
			sqlite3_bind_int(stmt, 5, record.Level);
			sqlite3_bind_int(stmt, 6, record.AlarmGroup);
			sqlite3_bind_int(stmt, 7, record.AlarmNo);
			ret = sqlite3_step(stmt);
		}
	}

	/**
	 * @brief  : 获取报警记录
	 * @params : sql	 SQL语句
	 * @params : records 报警记录集合
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	vector<AlarmRecord>  AlarmStorageService::SelectAlarm(sqlite3_stmt *stmt) {
		vector<AlarmRecord>  records;
		int ret = 0;
		// 解析结果集
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			AlarmRecord record;
			int idx = 0;
			record.Index = sqlite3_column_int(stmt, idx++);
			record.AlarmGroup = sqlite3_column_int(stmt, idx++);	// 报警组别
			record.AlarmNo = sqlite3_column_int(stmt, idx++);		// 报警编号
			record.StartTick = sqlite3_column_int64(stmt, idx++);	// 起始时间
			record.Title = (char *)sqlite3_column_text(stmt, idx++);// 事件内容
			record.Times = sqlite3_column_int(stmt, idx++);			// 报警次数
			record.ResolveTick = sqlite3_column_int64(stmt, idx++);	// 恢复事件
			record.CheckTick = sqlite3_column_int64(stmt, idx++);	// 确认时间
			record.Level = sqlite3_column_int(stmt, idx++);			// 报警等级
			records.push_back(record);
		}
		// 释放对象
		//sqlite3_finalize(stmt);
		return records;
	}

	/**
	 * @brief  : 获取报警记录
	 * @params : records 报警记录集合
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	vector<AlarmRecord> AlarmStorageService::SelectAllAlarmRecords(int startgroupname, int endgroupname) {

		if (startgroupname == -1 || endgroupname == -1)
		{
			sqlite3_stmt* stmt = GetSTMT(SEL_SelectAllAlarmRecords);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			return SelectAlarm(stmt);
		}
		else
		{
			sqlite3_stmt* stmt = GetSTMT(SEL_SelectAllRecordsRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			int ret = sqlite3_reset(stmt);
			int idx = 1;
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			return SelectAlarm(stmt);
		}
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroup(int groupname, int groupno, int startgroupname, int endgroupname, int Recover) {
		sqlite3_stmt* stmt = nullptr;
		int ret = 0;
		int idx = 1;
		if (startgroupname == -1 || endgroupname == -1)
		{
			switch (Recover)
			{
			case 0:
				stmt = GetSTMT(SEL_SelectByNameAndNo);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				break;
			case 1:
				stmt = GetSTMT(SEL_SelectByNameAndNoAndUnResolved);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				break;
			case 2:
				stmt = GetSTMT(SEL_SelectByNameAndNoAndResolved);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				break;
			}
		}
		else
		{
			switch (Recover)
			{
			case 0:
				stmt = GetSTMT(SEL_SelectByNameAndNoRangeByGroupName);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				break;
			case 1:
				stmt = GetSTMT(SEL_SelectByNameAndNoAndUnResolvedRangeByGroup);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				break;
			case 2:
				stmt = GetSTMT(SEL_SelectByNameAndNoAndResolvedRangeByGroup);
				if (stmt == nullptr)
					return vector<AlarmRecord>();
				ret = sqlite3_reset(stmt);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				sqlite3_bind_int(stmt, idx++, groupname);
				sqlite3_bind_int(stmt, idx++, groupno);
				sqlite3_bind_int(stmt, idx++, startgroupname);
				sqlite3_bind_int(stmt, idx++, endgroupname);
				break;
			}
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroupName(int groupname, int startgroupname, int endgroupname, int Recover) {

		sqlite3_stmt* stmt=nullptr;
		int idx = 1;
		int ret = 0;
		switch (Recover)
		{
		case 0:
			stmt = GetSTMT(SEL_SelectByGroupRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 1:
			stmt = GetSTMT(SEL_SelectByGroupAndUnResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:
			stmt = GetSTMT(SEL_SelectByGroupAndResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroupNo(int groupno, int startgroupname, int endgroupname, int Recover) {
		sqlite3_stmt* stmt=nullptr;
		int idx = 1;
		int ret = 0;
		switch (Recover)
		{
		case 0:
			stmt = GetSTMT(SEL_SelectByNoRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 1:
			stmt = GetSTMT(SEL_SelectByNoAndUnResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:
			stmt = GetSTMT(SEL_SelectByNoAndResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, groupno);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByAlarmLevel(int level, int startgroupname, int endgroupname, int Recover) {

		sqlite3_stmt* stmt = nullptr;
		int idx = 1;
		int ret = 0;
		switch (Recover)
		{
		case 0:
			stmt = GetSTMT(SEL_SelectByLevRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 1:
			stmt = GetSTMT(SEL_SelectByLevAndUnResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:
			stmt = GetSTMT(SEL_SelectByLevAndResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, level);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByHide(int hideflag, int startgroupname, int endgroupname) {
		sqlite3_stmt* stmt;
		int idx = 1;
		int ret = 0;
		switch (hideflag & 0x7)
		{
		case 1:
			stmt = GetSTMT(SEL_SelectByLevAndResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:			//bit1 = 1 隐藏已恢复信息(获取未恢复信息)
			return SelectAlarmRecordByUnRecover(startgroupname, endgroupname);
			break;
		case 3:			//bit0 = 1，bit1 = 1 隐藏已确认和已恢复信息(获取未恢复信息或未确认信息)
			stmt = GetSTMT(SEL_SelectCheckedOrResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 4:			//bit2 = 1 隐藏未恢复信息(获取已恢复的)
			stmt = GetSTMT(SEL_SelectUnResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 5:			//bit0 = 1 bit2 = 1  隐藏已确认和未恢复信息（获取未确认或已恢复的信息）
			stmt = GetSTMT(SEL_SelectUnResolvedOrCheckedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 6:			//bit1 = 1 bit2 = 1 隐藏已恢复和未恢复信息
		case 7:			//bit0 = 1 bit1 = 1 bit2 = 1 隐藏已确认、已恢复和未恢复信息
			return vector<AlarmRecord>();
		default:
			return SelectAllAlarmRecords(startgroupname, endgroupname);
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByUnRecover(int startgroupname, int endgroupname) {
		sqlite3_stmt* stmt;
		stmt = GetSTMT(SEL_SelectUnResolvedRangeByGroup);
		if (stmt == nullptr)
			return vector<AlarmRecord>();
		int idx = 1;
		int ret = sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, idx++, startgroupname);
		sqlite3_bind_int(stmt, idx++, endgroupname);
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByRecover(int startgroupname, int endgroupname) {
		sqlite3_stmt* stmt;
		stmt = GetSTMT(SEL_SelectResolvedRangeByGroup);
		if (stmt == nullptr)
			return vector<AlarmRecord>();
		int idx = 1;
		int ret = sqlite3_reset(stmt);
		sqlite3_bind_int(stmt, idx++, startgroupname);
		sqlite3_bind_int(stmt, idx++, endgroupname);
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate, int startgroupname, int endgroupname,int Recover) {
		sqlite3_stmt* stmt = nullptr;
		int idx = 1;
		int ret = 0;
		switch (Recover)
		{
		case 0:
			stmt = GetSTMT(SEL_SelectResolvedRangeByGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startDate);
			sqlite3_bind_int64(stmt, idx++, endDate);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 1:
			stmt = GetSTMT(SEL_SelectResolvedRangeTimeAndGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startDate);
			sqlite3_bind_int64(stmt, idx++, endDate);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:
			stmt = GetSTMT(SEL_SelectUnResolvedRangeTimeAndGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startDate);
			sqlite3_bind_int64(stmt, idx++, endDate);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname); 
			break;
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime, int startgroupname, int endgroupname,int Recover) {
		sqlite3_stmt* stmt = nullptr;
		int idx = 1;
		int ret = 0;
		switch (Recover)
		{
		case 0:
			stmt = GetSTMT(SEL_SelectRangeTimeAndGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startTime);
			sqlite3_bind_int64(stmt, idx++, endTime);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 1:
			stmt = GetSTMT(SEL_SelectResolvedRangeTimeAndGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startTime);
			sqlite3_bind_int64(stmt, idx++, endTime);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		case 2:
			stmt = GetSTMT(SEL_SelectUnResolvedRangeTimeAndGroup);
			if (stmt == nullptr)
				return vector<AlarmRecord>();
			ret = sqlite3_reset(stmt);
			sqlite3_bind_int64(stmt, idx++, startTime);
			sqlite3_bind_int64(stmt, idx++, endTime);
			sqlite3_bind_int(stmt, idx++, startgroupname);
			sqlite3_bind_int(stmt, idx++, endgroupname);
			break;
		}
		return SelectAlarm(stmt);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByResolveTick()
	{
		sqlite3_stmt* stmt = nullptr;
		stmt = GetSTMT(SEL_SelectAlarmRecordByResolveTick);
		if (stmt == nullptr)
			return vector<AlarmRecord>();
		return SelectAlarm(stmt);
	}
}

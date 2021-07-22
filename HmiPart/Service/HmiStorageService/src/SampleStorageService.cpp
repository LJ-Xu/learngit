#include "SampleStorageService.h"
#include "FileSave.h"
#include <string.h>
namespace Storage
{
	SampleStorageService * SampleStorageService::ins = nullptr;

	SampleStorageService::SampleStorageService() : 
		BaseStorageService(RunEnv::Cnf.SamplePath, "Sample") {
		if (Create()) {
			Close();
		}
	}

	SampleStorageService * SampleStorageService::Ins() {
		static once_flag flag;
		call_once(flag, [&]() {
			ins = new SampleStorageService();
		});
		return ins;
	}

	/**
	 * @brief  : 释放操作记录数据库
	 * @return : void
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void SampleStorageService::Destroy() {
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
	int SampleStorageService::Create() {
		std::string sql;
		sql.append("CREATE TABLE ").append(tbName);
		sql.append("(Id INT64 NOT NULL,");
		sql.append("ChannelNo INT NOT NULL,");
		sql.append("ChannelData INT64 NOT NULL,");
		sql.append("DataType INTEGER NOT NULL,");
		sql.append("Format INTEGER NOT NULL,");
		sql.append("Date INT64 NOT NULL);");
		char* errmsg;
		int ret = sqlite3_exec(db, sql.c_str(), 0, 0, &errmsg);
		if (ret != SQLITE_OK)
		{
			std::cout << "Sampledb create fail: " << sqlite3_errmsg(db);
			return ret;
		}
		Attach();
		//Prepare
		Init();
		return ret;
	}
	int SampleStorageService::Init()
	{
		std::string sql;
		//INSERT
		{
			sql.clear();
			sql.append("INSERT INTO ").append(tbName).append(" VALUES(?,?,?,?,?,?)");
			if (!NewFMT(INS_InsertSampleRecord, sql.c_str(), sizeof(sql)))
				return INS_InsertSampleRecord;

			sql.clear();
			sql.append("INSERT INTO ").append(tbName).append(" VALUES(?,?,?,?,?,?)");
			if (!NewFMT(INS_InsertSampleRecordByLimit, sql.c_str(), sizeof(sql)))
				return INS_InsertSampleRecord;
		}


		//UPDATE
		{
			sql.clear();
			sql.append("UPDATE ").append(tbName).append(" SET ChannelNo = ?,ChannelData = ?, DataType = ?, Format = ?, Date = ? WHERE ( ChannelNo  = ? and Date= (SELECT  Date FROM ").append(tbName).append(" WHERE  ChannelNo = ? ORDER BY Date LIMIT 1))");
			if (!NewFMT(UPD_UpdateSampleOnEarliestDate, sql.c_str(), sizeof(sql)))
				return UPD_UpdateSampleOnEarliestDate;
		}

		//DELETE
		{
			sql.clear();
			sql.append("DELETE FROM ").append(tbName).append(" WHERE ChannelNo = ?");
			if (!NewFMT(DEL_DeleteByChannel, sql.c_str(), sizeof(sql)))
				return DEL_DeleteByChannel;

			sql.clear();
			sql.append("DELETE FROM ").append(tbName).append(" WHERE ChannelNo >>16 = ?");
			if (!NewFMT(DEL_DeleteByGroup, sql.c_str(), sizeof(sql)))
				return DEL_DeleteByGroup;
		}
		//SELECT
		{
			sql.clear();
			sql.append("SELECT COUNT(*) FROM ").append(tbName).append(" union all SELECT COUNT(*) FROM fileDb.").append(tbName).append(";");
			if (!NewFMT(SEL_GetAllCount, sql.c_str(), sizeof(sql)))
				return SEL_GetAllCount;

			sql.clear();
			sql.append("SELECT COUNT(*) FROM ").append(tbName).append(" WHERE ChannelNo = ?").append(" union all SELECT COUNT(*) FROM ").append("fileDb.").append(tbName).append(" WHERE ChannelNo = ?;");
			if (!NewFMT(SEL_GetCountByChannel, sql.c_str(), sizeof(sql)))
				return SEL_GetCountByChannel;

			sql.clear();
			sql.append("SELECT COUNT(*) FROM ").append(tbName).append(" WHERE ChannelNo>>16 =?  and Date > ? ").append(" union all SELECT COUNT(*) FROM ").append("fileDb.").append(tbName).append(" WHERE ChannelNo>>16 =?  and Date > ? ");
			if (!NewFMT(SEL_GetChannelCountByDate, sql.c_str(), sizeof(sql)))
				return SEL_GetChannelCountByDate;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ChannelNo = ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ChannelNo = ? ORDER BY Date;");
			if (!NewFMT(SEL_SelectSampleRecordByChannel, sql.c_str(), sizeof(sql)))
				return SEL_SelectSampleRecordByChannel;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ChannelNo = ? AND Date BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ChannelNo = ? AND Date BETWEEN ? AND ? ORDER BY Date;");
			if (!NewFMT(SEL_SelectSampleRecordByTime, sql.c_str(), sizeof(sql)))
				return SEL_SelectSampleRecordByTime;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ChannelNo = ? AND Date BETWEEN ? AND ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ChannelNo = ? AND Date BETWEEN ? AND ? ORDER BY Date;");
			if (!NewFMT(SEL_SelectSampleRecordByDate, sql.c_str(), sizeof(sql)))
				return SEL_SelectSampleRecordByDate;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE ChannelNo>>16 = ?").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE ChannelNo>>16 = ? ORDER BY Date;");
			if (!NewFMT(SEL_SelectSampleRecord, sql.c_str(), sizeof(sql)))
				return SEL_SelectSampleRecord;

			sql.clear();
			sql.append("SELECT * FROM ").append(tbName).append(" WHERE (ChannelNo>>16 = ? AND Date > ?)").append(" union all SELECT * FROM fileDb.").append(tbName).append(" WHERE (ChannelNo>>16 = ? AND Date > ?) ORDER BY Date");
			if (!NewFMT(SEL_SelectSampleRecordByStTm, sql.c_str(), sizeof(sql)))
				return SEL_SelectSampleRecordByStTm;
		}

		sqlite3_exec(db, "PRAGMA synchronous = OFF;", NULL, NULL, NULL);

		ExecBegin();
		return 0;
	}
	/**
	 * @brief  : 插入通道采集记录
	 * @params : record		报警记录
	 * @return : int		是否添加成功
	 * @birth  : created by TangYao on 2021/01/12
	 */
	int SampleStorageService::InsertSampleRecord(const SampleRecord & record) {
		sqlite3_stmt* stmt = GetSTMT(INS_InsertSampleRecord);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int64(stmt, 1, curId);
			sqlite3_bind_int(stmt, 2, record.Channel);
			sqlite3_bind_int64(stmt, 3, record.Data);
			sqlite3_bind_int(stmt, 4, record.Type.Cls);
			sqlite3_bind_int(stmt, 5, record.Type.Type);
			sqlite3_bind_int64(stmt, 6, record.Date);
			ret = sqlite3_step(stmt);
			if (ret==0||ret==100||ret==101)
				curId++;
		}
		return ret;
	}

	int SampleStorageService::InsertSampleRecordByLimit(const SampleRecord & record, int maxcount)
	{
		int gName = (record.Channel >> 16) & 0xff;
		int gNo = record.Channel >> 24;
		if (GetChannelCountByDate(gName, gNo, 0) > maxcount)
		{
			return UpdateSampleRecordByChannel(record);
		}
		else
		{
			return InsertSampleRecord(record);
		}
	}

	int SampleStorageService::UpdateSampleRecordByChannel(const SampleRecord & record) {
		sqlite3_stmt* stmt = GetSTMT(UPD_UpdateSampleOnEarliestDate);
		if (stmt == nullptr)
			return -1;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int(stmt, 1, record.Channel);
			sqlite3_bind_int64(stmt, 2, record.Data);
			sqlite3_bind_int(stmt, 3, record.Type.Cls);
			sqlite3_bind_int(stmt, 4, record.Type.Type);
			sqlite3_bind_int64(stmt, 5, record.Date);
			sqlite3_bind_int(stmt, 6, record.Channel);
			sqlite3_bind_int(stmt, 7, record.Channel);
			ret = sqlite3_step(stmt);
		}
		return ret;
	}

	int SampleStorageService::DeleteSampleRecordByChannel(int channel)
	{
		sqlite3_stmt* stmt = GetSTMT(DEL_DeleteByChannel);
		if (stmt == nullptr)
			return -1;
		sqlite3_bind_int(stmt, 1, channel);
		return sqlite3_step(stmt);
	}

	int SampleStorageService::DeleteSampleRecordByGroup(int gName, int gNo)
	{
		sqlite3_stmt* stmt = GetSTMT(DEL_DeleteByChannel);
		if (stmt == nullptr)
			return -1;
		int matchnm = gName | (gNo << 8);
		sqlite3_bind_int(stmt, 1, matchnm);
		return sqlite3_step(stmt);
	}

	int SampleStorageService::GetChannelCountByDate(int groupId, int no, DDWORD date)
	{
		sqlite3_stmt* stmt = GetSTMT(SEL_GetChannelCountByDate);
		if (stmt == nullptr)
			return -1;
		int matchnm = groupId | (no << 8);
		int count = 0;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int(stmt, 1, matchnm);
			sqlite3_bind_int64(stmt, 2, date);
			ret = sqlite3_step(stmt);
			if (ret == SQLITE_ROW) {
				count = sqlite3_column_int(stmt, 0);
			}
		}
		return ret;
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : channel	数据通道号
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectSampleRecordByChannel(int channel) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectSampleRecordByChannel);
		if (stmt == nullptr)
			return std::vector<SampleRecord>();
		int ret = sqlite3_reset(stmt);
		std::vector<SampleRecord> records;
		sqlite3_bind_int(stmt, 1, channel);
		sqlite3_bind_int(stmt, 2, channel);
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			SampleRecord record;
			record.Channel = sqlite3_column_int(stmt, 1);
			record.Data = sqlite3_column_int64(stmt, 2);
			record.Type.Cls = sqlite3_column_int(stmt, 3);
			record.Type.Type = sqlite3_column_int(stmt, 4);
			record.Date = sqlite3_column_int64(stmt, 5);
			records.push_back(record);
		}
		return records;
	}

	vector<SampleRecord> SampleStorageService::SelectSampleRecord(int gName, int gNo)
	{
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectSampleRecordByChannel);
		if (stmt == nullptr)
			return std::vector<SampleRecord>();
		int ret = sqlite3_reset(stmt);
		int matchnm = gName | (gNo << 8);
		std::vector<SampleRecord> records;
		sqlite3_bind_int(stmt, 1, matchnm);
		sqlite3_bind_int(stmt, 2, matchnm);
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			SampleRecord record;
			record.Channel = sqlite3_column_int(stmt, 1);
			record.Data = sqlite3_column_int64(stmt, 2);
			record.Type.Cls = sqlite3_column_int(stmt, 3);
			record.Type.Type = sqlite3_column_int(stmt, 4);
			record.Date = sqlite3_column_int64(stmt, 5);
			records.push_back(record);
		}
		return records;
	}

	vector<SampleRecord> SampleStorageService::SelectSampleRecordByStTm(int gName, int gNo, unsigned long long startTime)
	{
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectSampleRecordByChannel);
		if (stmt == nullptr)
			return std::vector<SampleRecord>();
		int ret = sqlite3_reset(stmt);
		int matchnm = gName | (gNo << 8);
		std::vector<SampleRecord> records;
		sqlite3_bind_int(stmt, 1, matchnm);
		sqlite3_bind_int64(stmt, 2, startTime);
		sqlite3_bind_int(stmt, 3, matchnm);
		sqlite3_bind_int64(stmt, 4, startTime);
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			SampleRecord record;
			record.Channel = sqlite3_column_int(stmt, 1);
			record.Data = sqlite3_column_int64(stmt, 2);
			record.Type.Cls = sqlite3_column_int(stmt, 3);
			record.Type.Type = sqlite3_column_int(stmt, 4);
			record.Date = sqlite3_column_int64(stmt, 5);
			records.push_back(record);
		}
		return records;
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : channel	数据通道号
	 * @params : startDate	起始采集日期
	 * @params : endDate	终止采集日期
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectSampleRecordByDate);
		if (stmt == nullptr)
			return std::vector<SampleRecord>();
		std::vector<SampleRecord> records;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int(stmt, 1, channel);
			sqlite3_bind_int64(stmt, 2, startDate);
			sqlite3_bind_int64(stmt, 3, endDate);
			sqlite3_bind_int(stmt, 4, channel);
			sqlite3_bind_int64(stmt, 5, startDate);
			sqlite3_bind_int64(stmt, 6, endDate);
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				SampleRecord record;
				record.Channel = sqlite3_column_int(stmt, 1);
				record.Data = sqlite3_column_int64(stmt, 2);
				record.Type.Cls = sqlite3_column_int(stmt, 3);
				record.Type.Type = sqlite3_column_int(stmt, 4);
				record.Date = sqlite3_column_int64(stmt, 5);
				records.push_back(record);
			}
		}
		return records;
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : channel	数据通道号
	 * @params : startTime	起始采集时间
	 * @params : endTime	终止采集时间
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime) {
		sqlite3_stmt* stmt = GetSTMT(SEL_SelectSampleRecordByTime);
		if (stmt == nullptr)
			return std::vector<SampleRecord>();
		std::vector<SampleRecord> records;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int(stmt, 1, channel);
			sqlite3_bind_int64(stmt, 2, startTime);
			sqlite3_bind_int64(stmt, 3, endTime);
			sqlite3_bind_int(stmt, 4, channel);
			sqlite3_bind_int64(stmt, 5, startTime);
			sqlite3_bind_int64(stmt, 6, endTime);
			while (sqlite3_step(stmt) == SQLITE_ROW) {
				SampleRecord record;
				record.Channel = sqlite3_column_int(stmt, 1);
				record.Data = sqlite3_column_int64(stmt, 2);
				record.Type.Cls = sqlite3_column_int(stmt, 3);
				record.Type.Type = sqlite3_column_int(stmt, 4);
				record.Date = sqlite3_column_int64(stmt, 5);
				records.push_back(record);
			}
		}
		return records;
	}

	int SampleStorageService::GetCountByChannel(int channel) {
		sqlite3_stmt* stmt = GetSTMT(SEL_GetCountByChannel);
		if (stmt == nullptr)
			return -1;
		int count = 0;
		int ret = sqlite3_reset(stmt);
		if (!ret)
		{
			sqlite3_bind_int(stmt, 1, channel);
			sqlite3_bind_int(stmt, 2, channel);
			ret = sqlite3_step(stmt);
			if (ret == SQLITE_ROW) {
				count += sqlite3_column_int(stmt, 0);
			}
		}
		return ret;
	}
}

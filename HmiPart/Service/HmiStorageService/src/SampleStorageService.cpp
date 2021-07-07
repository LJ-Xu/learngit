#include "SampleStorageService.h"

#include <string.h>
namespace Storage
{
	SampleStorageService * SampleStorageService::ins = nullptr;

	SampleStorageService::SampleStorageService() : 
		//BaseStorageService("Sample.db", "Sample") {
		BaseStorageService(RunEnv::Cnf.SamplePath, "Sample") {
		//BaseStorageService("././HMI/Sample.db", "Sample") {
		// 创建内存数据库
		if (Create()) {
			Close();
		}
		// 附加文件数据库
		Attach();
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
		int ret = 0;
		char sql[SQLCMDLEN] = { 0 };
		// 操作记录内存表SQL语句
		snprintf(sql, sizeof(sql), "CREATE TABLE Sample ("
			"\n\tChannelNo INT NOT NULL,"
			"\n\tChannelData INT64 NOT NULL,"
			"\n\tDataType INTEGER NOT NULL,"
			"\n\tFormat INTEGER NOT NULl,"
			"\n\tDate INT64 NOT NULL);");
		// 创建操作记录内存表
		ret = ExecuteSql(sql);
		return ret;
	}
	/**
	 * @brief  : 插入通道采集记录
	 * @params : record		报警记录
	 * @return : int		是否添加成功
	 * @birth  : created by TangYao on 2021/01/12
	 */
	int SampleStorageService::InsertSampleRecord(const SampleRecord & record) {
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "INSERT INTO Sample VALUES(%d, %lld, %d, %d, %lld)",
			record.Channel, record.Data, record.Type.Cls, record.Type.Type, record.Date);
		printf("InsertSampleRecord:value = %d.\n",record.Data);
		return ExecuteSql(sql);
	}

	int SampleStorageService::InsertSampleRecord(const SampleRecord & record, int maxcount)
	{
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		int gName = (record.Channel >> 16) & 0xff;
		int gNo = record.Channel >> 24;
		if (GetAllCountByNo(gName,gNo,0) > maxcount)
		{
			//SelectSample("SELECT  * FROM Sample WHERE ( ChannelNo & 0 = 0 and ChannelNo & 0 = 0 )ORDER BY Date LIMIT 1");
			snprintf(sql, sizeof(sql), "UPDATE Sample SET ChannelNo = %d,ChannelData = %lld, DataType = %d, Format = %d, Date = %lld WHERE ( ChannelNo  = %d and Date= (SELECT  Date FROM Sample WHERE  ChannelNo = %d ORDER BY Date LIMIT 1))", record.Channel,
				record.Data, record.Type.Cls, record.Type.Type, record.Date, record.Channel, record.Channel);
			printf("0InsertSampleRecord:value = %lld.\n",record.Data);

			//snprintf(sql, sizeof(sql), "UPDATE Sample SET ChannelNo = %d,ChannelData = %lld, DataType = %d, Format = %d, Date = %lld WHERE ( ChannelNo & %d = %d and ChannelNo & %d = %d and (Date IN (SELECT MIN(DATE) FROM Sample)) ;UPDATE fileDb.Sample SET ChannelNo = %d, ChannelData = %lld, DataType = %d, Format = %d, Date = %lld WHERE ( ChannelNo & %d = %d and ChannelNo & %d = %d and (Date IN (SELECT MIN(DATE) FROM Sample));", record.Channel,record.Data, record.Type.Cls, record.Type.Type, record.Date,gName, gName, gNo, gNo, record.Channel,record.Data, record.Type.Cls, record.Type.Type, record.Date, gName, gName, gNo, gNo);
			//(Date IN (SELECT MIN(DATE) FROM Sample))
			int ret = ExecuteSql(sql);
			printf("0InsertSampleRecord:retvalue = %d.\n",ret);
			return ret;
		}
		else
		{
			printf("1InsertSampleRecord:value = %lld.\n",record.Data);
			snprintf(sql, sizeof(sql), "INSERT INTO Sample VALUES(%d, %lld, %d, %d, %lld)",
				record.Channel, record.Data, record.Type.Cls, record.Type.Type, record.Date);
		}
		
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 删除通道采集记录
	 * @params : channel	数据通道号
	 * @return : int		是否删除成功
	 * @birth  : created by TangYao on 2021/01/12
	 */
	int SampleStorageService::DeleteSampleRecordByChannel(int channel) {
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "DELETE FROM Sample WHERE ChannelNo = %d; \
			DELETE FROM fileDb.Sample WHERE ChannelNo = %d;", channel, channel);
		return ExecuteSql(sql);
	}

	int SampleStorageService::DeleteSampleRecordByGroup(int gName, int gNo)
	{
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		int gnm = gName << 16;
		int matchnm = gName == 0 ? (0xff << 16) : gnm;
		int gno = gNo << 24;
		int matchno = gNo == 0 ? (0xff << 24) : gno;
		snprintf(sql, sizeof(sql), "DELETE FROM Sample WHERE ChannelNo & %d = %d and ChannelNo & %d = %d ; \
			DELETE FROM fileDb.Sample WHERE ChannelNo & %d = %d and ChannelNo & %d = %d ", matchnm, gnm, matchno, gno, matchnm, gnm, matchno, gno);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 更新通道采集记录
	 * @params : channel	数据通道号
	 * @params : record		更新数据
	 * @return : int		是否更新成功
	 * @birth  : created by TangYao on 2021/01/12
	 */
	int SampleStorageService::UpdateSampleRecordByChannel(const SampleRecord & record) {
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "UPDATE Sample SET ChannelData = %lld, DataType = %c, \
									Format = %c, Date = %lld WHERE ChannelNo = %d; \
									UPDATE fileDb.Sample SET ChannelData = %lld, DataType = %c, \
									Format = %c, Date = %lld WHERE ChannelNo = %d;",
			record.Data, record.Type.Cls, record.Type.Type, record.Date, record.Channel,
			record.Data, record.Type.Cls, record.Type.Type, record.Date, record.Channel);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : sql		数据库执行语句
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectSample(const char * sql) {
		vector<SampleRecord> records;
		int ret = 0;
		struct sqlite3_stmt *stmt;
		ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr,"Sql Error: %s\n", sqlite3_errmsg(db));
			return records;
		}
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			SampleRecord record;
			record.Channel = sqlite3_column_int(stmt, 0);
			record.Data = sqlite3_column_int64(stmt, 1);
			record.Type.Cls = sqlite3_column_int(stmt, 2);
			record.Type.Type = sqlite3_column_int(stmt, 3);
			record.Date = sqlite3_column_int64(stmt, 4);
			records.push_back(record);
		}
		//fprintf(stderr, "Sql Select Complete.\n");
		sqlite3_finalize(stmt);
		return records;
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectAllSampleRecord() {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample union all SELECT * FROM fileDb.Sample ORDER BY Date;");
		return SelectSample(sql);
	}

	/**
	 * @brief  : 获取通道采集记录
	 * @params : channel	数据通道号
	 * @params : record		更新数据
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	vector<SampleRecord> SampleStorageService::SelectSampleRecordByChannel(int channel) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d union all \
									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d ORDER BY Date;", channel, channel);
		return SelectSample(sql);
	}

	vector<SampleRecord> SampleStorageService::SelectSampleRecordByNO(int gName, int gNo)
	{
		char sql[SQLCMDLEN] = { 0 };
		int gnm = gName << 16;
		int matchnm = gName == 0 ? (0xff << 16) : gnm;
		int gno = gNo << 24;
		int matchno = gNo == 0 ? (0xff << 24) : gno;
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo & %d = %d and ChannelNo & %d = %d union all SELECT * FROM fileDb.Sample WHERE ChannelNo & %d = %d and ChannelNo & %d = %d ORDER BY Date ", matchnm, gnm, matchno, gno, matchnm, gnm, matchno, gno);
		return SelectSample(sql);
	}

	vector<SampleRecord> SampleStorageService::SelectSampleRecordByNO(int gName, int gNo, unsigned long long startTime)
	{
		char sql[SQLCMDLEN] = { 0 };
		int gnm = gName << 16;
		int matchnm = gName == 0 ? (0xff << 16) : gnm;
		int gno = gNo << 24;
		int matchno = gNo == 0 ? (0xff << 24) : gno;
		//snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE (ChannelNo & %d = %d and ChannelNo & %d = %d and Date > %lld) union all SELECT * FROM fileDb.Sample WHERE (ChannelNo & %d = %d and ChannelNo & %d = %d and Date > %lld) ORDER BY Date", gnm, gnm, gno, gno, startTime, gnm, gnm, gno, gno, startTime);
		//snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE (ChannelNo =%d and Date > %lld) union all SELECT * FROM fileDb.Sample WHERE (ChannelNo = %d and Date > %lld) ORDER BY Date", gnm|gno, startTime, gnm|gno, startTime);
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE (ChannelNo & %d = %d and ChannelNo & %d = %d and Date > %lld) union all SELECT * FROM fileDb.Sample WHERE (ChannelNo & %d = %d and ChannelNo & %d = %d and Date > %lld) ORDER BY Date", matchnm, gnm, matchno, gno, startTime, matchnm, gnm, matchno, gno, startTime);
		return SelectSample(sql);
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
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld union all \
									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld ORDER BY Date;",
			channel, startDate, endDate, channel, startDate, endDate);
		return SelectSample(sql);
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
		char sql[SQLCMDLEN] = { 0 };
		//sprintf(sql, "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld", channel, startTime, endTime);
		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld union all \
									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld ORDER BY Date;",
			channel, startTime, endTime, channel, startTime, endTime);
		return SelectSample(sql);
	}

	/**
	 * @brief  : 获取通道采集记录数量
	 * @params : channel	数据通道号
	 * @return : int
	 * @birth  : created by TangYao on 2021/01/12
	 */
	int SampleStorageService::GetCountByChannel(int channel) {
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM Sample WHERE ChannelNo = %d", channel);
		int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			return -1;
		}
		int count = 0;
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW) {
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		return count;
	}

	int SampleStorageService::GetAllCountByNo(int gName,int gNo, unsigned long long date) {
		char sql[SQLCMDLEN] = { 0 };
		struct sqlite3_stmt *stmt;
		int gnm = gName << 16;
		int matchnm = gName == 0 ? (0xff << 16) : gnm;
		int gno = gNo << 24;
		int matchno = gNo == 0 ? (0xff << 24) : gno;
		snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM Sample WHERE ChannelNo & %d = %d and ChannelNo & %d = %d  and Date > %lld ", matchnm, gnm, matchno, gno,date);
		int ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			fprintf(stderr, "Sql Error: %s\n", sqlite3_errmsg(db));
			return -1;
		}
		int count = 0;
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_ROW) {
			count = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
		return count;
	}
}

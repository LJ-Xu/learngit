///*******************************************************************************
// * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
// * All rights reserved.
// *
// * Name     : StorageService.cpp
// * Author   : TangYao
// * Date     : 2021/01/11
// * Descript : 数据库操作接口
// * Version  : 0.1
// * modify   :
// *          :
// *******************************************************************************/
//#include "Logger.h"
//#include "StorageServiceImpl.h"
//
//namespace Storage
//{
//	StorageService * StorageService::instance_ = nullptr;
//
//	StorageService * StorageService::Ins() {
//		static std::once_flag flag;
//		call_once(flag, [&]() {
//			instance_ = new StorageService();
//		});
//		return instance_;
//	}
//
//	void StorageService::Destroy() {
//		if (instance_) {
//			// 关闭数据库
//			sqlite3_close(memdb_);
//			delete instance_;
//			instance_ = nullptr;
//		}
//	}
//
//	StorageService::StorageService() {
//		/* Open DataBase */
//		CreateDbOnMemory();
//
//		AttachDbOnMemoryDb();
//	}
//
//	/**
//	 * @brief  : 创建内存数据库
//	 * @return : int	是否创建成功
//	 * @birth  : created by TangYao on 2021/01/16
//	 */
//	int StorageService::CreateDbOnMemory() {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		// 打开内存数据库
//		ret = sqlite3_open(":memory:", &memdb_);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Can't Open DataBase: %s\n", sqlite3_errmsg(memdb_));
//			sqlite3_close(memdb_);
//			return -1;
//		}
//		// 创建数据采集内存数据表
//		snprintf(sql, sizeof(sql), "CREATE TABLE Sample (ChannelNo INT, ChannelData INT64, DataType INTEGER, Format INTEGER, Date INT64);");
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Can't Create Memory DataBase: %s\n", errMsg);
//			sqlite3_close(memdb_);
//		}
//		// 创建报警记录内存数据表
//		snprintf(sql, sizeof(sql), "CREATE TABLE Alarm (GroupName INT, GroupNo INT, StartTime INT64, Title Text, Times INT, ResolveTime INT64, CheckTime INT64, Level INT);");
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Can't Create Memory DataBase: %s\n", errMsg);
//			sqlite3_close(memdb_);
//		}
//		return 0;
//	}
//
//	/**
//	 * @brief  : 文件数据库作为内存数据库的附加数据库
//	 * @return : int	是否附加数据库成功
//	 * @birth  : created by TangYao on 2021/01/16
//	 */
//	int StorageService::AttachDbOnMemoryDb() {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		// 将文件数据库作为内存数据库的附加数据库
//		snprintf(sql, sizeof(sql), "ATTACH '.\\Record.db' AS fileDb");
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Can't Attach DataBase [Record.db]: %s\n", errMsg);
//			sqlite3_close(memdb_);
//			return -1;
//		}
//		return 0;
//	}
//
//	/**
//	 * @brief  : 同步内存数据库及文件数据库
//	 * @return : int	是否同步数据库成功
//	 * @birth  : created by TangYao on 2021/01/16
//	 */
//	int StorageService::FlushSample() {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "INSERT OR REPLACE INTO fileDb.Sample SELECT * FROM Sample;");
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Can't Flush memory DataBase: %s\n", errMsg);
//			sqlite3_close(memdb_);
//			return -1;
//		}
//		snprintf(sql, sizeof(sql), "DELETE FROM Sample");
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		return 0;
//	}
//
//	/**
//	 * @brief  : 插入报警记录
//	 * @params : record		报警记录
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/07
//	 */
//	int StorageService::InsertAlarmRecord(AlarmRecord & record) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "INSERT INTO Alarm VALUES(%d, %d, %lld, '%s', %d, %lld, %lld, %d);",
//			record.GroupName, record.GroupNo, record.StartTick, record.Title.c_str(),
//			record.Times, record.ResolveTick, record.CheckTick, record.Level);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Alarm Record Insert Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Alarm Record Insert Success.\n");
//		return 0;
//	}
//
//	/**
//	 * @brief  : 删除报警记录
//	 * @params : eventId		报警事件ID
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	int StorageService::DeleteAlarmRecordByCategory(int groupname, int groupno) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "DELETE FROM Alarm WHERE GroupName = %d AND GroupNo = %d; \
//									DELETE FROM fileDb.Alarm WHERE GroupName = %d AND GroupNo = %d;", 
//			groupname, groupno, groupname, groupno);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Alarm Record Delete Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Alarm Record Delete Success.\n");
//		return 0;
//	}
//	
//	/**
//	 * @brief  : 更新报警记录
//	 * @params : eventId		报警事件ID
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	int StorageService::UpdateAlarmRecord(int groupname, int groupno, AlarmRecord & record) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "UPDATE Alarm SET StartTime = %lld, Title = '%s', Times = %d, \
//									ResolveTime = %lld, CheckTime = %lld WHERE GroupName = %d AND GroupNo = %d; \
//									UPDATE Alarm SET StartTime = %lld, Title = '%s', Times = %d, \
//									ResolveTime = %lld, CheckTime = %lld WHERE GroupName = %d AND GroupNo = %d;",
//			record.StartTick, record.Title.c_str(), record.Times, record.ResolveTick, record.CheckTick, groupname, groupno,
//			record.StartTick, record.Title.c_str(), record.Times, record.ResolveTick, record.CheckTick, groupname, groupno);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Alarm Record Update Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Alarm Record UPDATE Complete.\n");
//		return 0;
//	}
//
//	/**
//	 * @brief  : 查询报警记录
//	 * @params : sql		数据库执行语句
//	 * @params : records	报警记录
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarm(const char * sql) {
//		vector<AlarmRecord> records;
//		int ret = 0;
//		ret = sqlite3_prepare_v2(memdb_, sql, strlen(sql), &stmt_, NULL);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sql Error: %s\n", sqlite3_errmsg(memdb_));
//			return std::move(records);
//		}
//		// 获取结果集
//		while (sqlite3_step(stmt_) == SQLITE_ROW) {
//			AlarmRecord record;
//			record.GroupName = sqlite3_column_int(stmt_, 0);	// 事件ID
//			record.GroupNo = sqlite3_column_int(stmt_, 1);	// 事件ID
//			record.StartTick = sqlite3_column_int64(stmt_, 2);	// 起始时间
//			record.Title = (char *)sqlite3_column_text(stmt_, 3);	// 事件内容
//			record.Times = sqlite3_column_int(stmt_, 4);	// 报警次数
//			record.ResolveTick = sqlite3_column_int64(stmt_, 5);	// 恢复事件
//			record.CheckTick = sqlite3_column_int64(stmt_, 6);	// 确认时间
//			record.Level = (Project::AlarmLevel)sqlite3_column_int(stmt_, 7);	// 确认时间
//			records.push_back(record);
//		}
//		sqlite3_finalize(stmt_);
//		return std::move(records);
//	}
//
//	/**
//	 * @brief  : 查询报警记录
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	vector<AlarmRecord> StorageService::SelectAllAlarmRecords() {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm union all SELECT * FROM fileDb.Alarm;");
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询报警记录
//	 * @params : eventId		报警事件ID
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByGroup(int groupname, int groupno) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE GroupName = %d AND GroupNo = %d union all \
//									SELECT * FROM fileDb.Alarm WHERE GroupName = %d AND GroupNo = %d;", 
//			groupname, groupno, groupname, groupno);
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询组名报警记录
//	 * @params : groupname		组名
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByGroupName(int groupname) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE GroupName = %d union all \
//									SELECT * FROM fileDb.Alarm WHERE GroupName = %d;", groupname, groupname);
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询组编号报警记录
//	 * @params : groupno		组编号
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByGroupNo(int groupno) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE GroupNo = %d union all \
//									SELECT * FROM fileDb.Alarm WHERE GroupNo = %d;", groupno, groupno);
//		return std::move(SelectAlarm(sql));
//	}
//	/**
//	 * @brief  : 查询报警等级报警记录
//	 * @params : level			报警等级
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByAlarmLevel(int level) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE Level = %d union all \
//									SELECT * FROM fileDb.Alarm WHERE Level = %d;", level, level);
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询隐藏后的报警记录
//	 * @params : level			报警等级
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByHide(int hideflag) {
//		char sql[256] = { 0 };
//		switch (hideflag & 0x7)
//		{
//		case 1:			//bit0 = 1 隐藏已确认信息(获取未确认信息)
//			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE CheckTime = 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE CheckTime = 0;");
//			break;
//		case 2:			//bit1 = 1 隐藏已恢复信息(获取未恢复信息)
//			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE ResolveTime = 0;");
//			break;
//		case 3:			//bit0 = 1，bit1 = 1 隐藏已确认和已恢复信息(获取未恢复信息或未确认信息)
//			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 OR CheckTime = 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE RResolveTime = 0 OR CheckTime = 0;");
//			break;
//		case 4:			//bit2 = 1 隐藏未恢复信息(获取已恢复的)
//			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE ResolveTime <> 0;");
//			break;
//		case 5:			//bit0 = 1 bit2 = 1  隐藏已确认和未恢复信息（获取未确认或已恢复的信息）
//			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 OR CheckTime = 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE RResolveTime <> 0 OR CheckTime = 0;");
//			break;		
//		case 6:			//bit1 = 1 bit2 = 1 隐藏已恢复和未恢复信息
//		case 7:			//bit0 = 1 bit1 = 1 bit2 = 1 隐藏已确认、已恢复和未恢复信息
//			return vector<AlarmRecord>();
//		default:
//			return std::move(SelectAllAlarmRecords());
//		}
//		std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询未恢复报警记录
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByUnRecover() {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE ResolveTime = 0;");
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询以恢复报警记录
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by qiaodan on 2021/01/30
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByRecover() {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 union all \
//									SELECT * FROM fileDb.Alarm WHERE ResolveTime <> 0;");
//		return std::move(SelectAlarm(sql));
//	}
//	/**
//	 * @brief  : 查询报警记录
//	 * @params : startDate		起始日期
//	 * @params : endDate		终止日期
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld union all \
//									SELECT * FROM fileDb.Alarm WHERE StartTime BETWEEN %lld AND %lld;", 
//			startDate, endDate, startDate, endDate);
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询报警记录
//	 * @params : startTime		起始时间
//	 * @params : endTime		终止时间
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by TangYao on 2021/01/08
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld union all \
//									SELECT * FROM fileDb.Alarm WHERE StartTime BETWEEN %lld AND %lld", 
//			startTime, endTime, startTime, endTime);
//		return std::move(SelectAlarm(sql));
//	}
//
//	/**
//	 * @brief  : 查询报警记录
//	 * @params : records		报警记录
//	 * @return : void
//	 * @birth  : created by ZhaoJiaQi on 2021/01/28
//	 */
//	vector<AlarmRecord> StorageService::SelectAlarmRecordByResolveTick()
//	{
//		/*char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime > 0  \ SELECT * FROM fileDb.Alarm WHERE ResolveTime > 0");*/
//		return std::move(SelectAlarm("SELECT * FROM Alarm WHERE ResolveTime = 0;"));
//	}
//
//	/**
//	 * @brief  : 插入通道采集记录
//	 * @params : record		报警记录
//	 * @return : int		是否添加成功
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::InsertSampleRecord(SampleRecord & record) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "INSERT INTO Sample VALUES(%d, %lld, %d, %d, %lld)", 
//			record.Channel, record.Data, record.Type.Cls, record.Type.Type, record.Date);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sample Record Insert Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Sample Record Insert Success\n");
//		return 0;
//	}
//
//	/**
//	 * @brief  : 删除通道采集记录
//	 * @params : channel	数据通道号
//	 * @return : int		是否删除成功
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::DeleteSampleRecordByChannel(int channel) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "DELETE FROM Sample WHERE ChannelNo = %d; \
//			DELETE FROM fileDb.Sample WHERE ChannelNo = %d;", channel, channel);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sample Record Delete Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Sample Record Delete Success.\n");
//		return 0;
//	}
//
//	/**
//	 * @brief  : 更新通道采集记录
//	 * @params : channel	数据通道号
//	 * @params : record		更新数据
//	 * @return : int		是否更新成功
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::UpdateSampleRecordByChannel(int channel, SampleRecord & record) {
//		int ret = 0;
//		char * errMsg = NULL;
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "UPDATE Sample SET ChannelData = %lld, DataType = %c, \
//									Format = %c, Date = %lld WHERE ChannelNo = %d; \
//									UPDATE fileDb.Sample SET ChannelData = %lld, DataType = %c, \
//									Format = %c, Date = %lld WHERE ChannelNo = %d;", 
//			record.Data, record.Type.Cls, record.Type.Type, record.Date, channel, 
//			record.Data, record.Type.Cls, record.Type.Type, record.Date, channel);
//		ret = sqlite3_exec(memdb_, sql, NULL, NULL, &errMsg);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sample Record Update Error: %s\n", errMsg);
//			return -1;
//		}
//		LOG_INFO("Sample Record Update Success.\n");
//		return 0;
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录
//	 * @params : sql		数据库执行语句
//	 * @params : record		更新数据
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::SelectSample(const char * sql, vector<SampleRecord> & records) {
//		int ret = 0;
//		ret = sqlite3_prepare_v2(memdb_, sql, strlen(sql), &stmt_, NULL);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sql Error: %s\n", sqlite3_errmsg(memdb_));
//			return -1;
//		}
//		while (sqlite3_step(stmt_) == SQLITE_ROW) {
//			SampleRecord record;
//			record.Channel = sqlite3_column_int(stmt_, 0);
//			record.Data = sqlite3_column_int64(stmt_, 1);
//			record.Type.Cls = sqlite3_column_int(stmt_, 2);
//			record.Type.Type = sqlite3_column_int(stmt_, 3);
//			record.Date = sqlite3_column_int64(stmt_, 4);
//			records.push_back(record);
//		}
//		LOG_INFO("Sql Select Complete.\n");
//		sqlite3_finalize(stmt_);
//		return 0;
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录
//	 * @params : record		更新数据
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::SelectAllSampleRecord(vector<SampleRecord> & records) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Sample union all SELECT * FROM fileDb.Sample;");
//		return SelectSample(sql, records);
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录
//	 * @params : channel	数据通道号
//	 * @params : record		更新数据
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::SelectSampleRecordByChannel(int channel, vector<SampleRecord> & records) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d union all \
//									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d;", channel, channel);
//		return SelectSample(sql, records);
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录
//	 * @params : channel	数据通道号
//	 * @params : startDate	起始采集日期
//	 * @params : endDate	终止采集日期
//	 * @params : record		更新数据
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate, vector<SampleRecord> & records) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld union all \
//									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld;", 
//			channel, startDate, endDate, channel, startDate, endDate);
//		return SelectSample(sql, records);
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录
//	 * @params : channel	数据通道号
//	 * @params : startTime	起始采集时间
//	 * @params : endTime	终止采集时间
//	 * @params : record		更新数据
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime, vector<SampleRecord> & records) {
//		char sql[256] = { 0 };
//		//sprintf(sql, "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld", channel, startTime, endTime);
//		snprintf(sql, sizeof(sql), "SELECT * FROM Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld union all \
//									SELECT * FROM fileDb.Sample WHERE ChannelNo = %d AND Date BETWEEN %lld AND %lld;", 
//			channel, startTime, endTime, channel, startTime, endTime);
//		return SelectSample(sql, records);
//	}
//
//	/**
//	 * @brief  : 获取通道采集记录数量
//	 * @params : channel	数据通道号
//	 * @return : int
//	 * @birth  : created by TangYao on 2021/01/12
//	 */
//	int StorageService::GetCountByChannel(int channel) {
//		char sql[256] = { 0 };
//		snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM Sample WHERE ChannelNo = %d", channel);
//		int ret = sqlite3_prepare_v2(memdb_, sql, strlen(sql), &stmt_, NULL);
//		if (ret != SQLITE_OK) {
//			LOG_ERROR("Sql Error: %s\n", sqlite3_errmsg(memdb_));
//			return -1;
//		}
//		int count = 0;
//		ret = sqlite3_step(stmt_);
//		if (ret == SQLITE_ROW) {
//			count = sqlite3_column_int(stmt_, 0);
//		}
//		sqlite3_finalize(stmt_);
//		return count;
//	}
//}
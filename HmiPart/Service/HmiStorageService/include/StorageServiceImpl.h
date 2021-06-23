///*******************************************************************************
// * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
// * All rights reserved.
// *
// * Name     : StorageService.h
// * Author   : TangYao
// * Date     : 2021/01/11
// * Descript : 数据库操作接口
// * Version  : 0.1
// * modify   :
// *          :
// *******************************************************************************/
//#pragma once
//#include "IStorageService.h"
//#include "XJRingBuffer.h"
//#include "SampleRecord.h"
//#include "AlarmRecord.h"
//#include "sqlite3.h"
//#include <iostream>
//#include <vector>
//#include <mutex>
//
//using namespace std;
//
//namespace Storage
//{
//	class StorageService
//	{
//	private:
//		StorageService();
//		~StorageService() = default;
//		// 不允许拷贝构造
//		StorageService(const StorageService& ins) = delete;
//		// 不允许等号赋值
//		StorageService & operator=(const StorageService& ins) = delete;
//
//	public:
//		// 手动创建对象
//		static StorageService * Ins();
//		// 手动释放对象
//		void Destroy();
//		// 创建内存数据库
//		int CreateDbOnMemory();
//		// 文件数据库作为内存数据库的附加数据库
//		int AttachDbOnMemoryDb();
//		// 同步内存数据库及文件数据库
//		int FlushSample();
//		// 插入报警记录
//		int InsertAlarmRecord(AlarmRecord & record);
//		// 删除报警记录
//		int DeleteAlarmRecordByCategory(int groupname, int groupno);
//		// 更新报警记录
//		int UpdateAlarmRecord(int groupname, int groupno, AlarmRecord & record);
//		// 查询所有报警记录
//		vector<AlarmRecord> SelectAlarm(const char * sql);
//		vector<AlarmRecord> SelectAllAlarmRecords();
//		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno);
//		vector<AlarmRecord> SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate);
//		vector<AlarmRecord> SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime);
//		vector<AlarmRecord> SelectAlarmRecordByResolveTick();
//		vector<AlarmRecord> SelectAlarmRecordByGroupName(int groupname);
//		vector<AlarmRecord> SelectAlarmRecordByGroupNo(int groupno);
//		vector<AlarmRecord> SelectAlarmRecordByAlarmLevel(int level);
//		vector<AlarmRecord> SelectAlarmRecordByHide(int hideflag);
//		vector<AlarmRecord> SelectAlarmRecordByUnRecover();
//		vector<AlarmRecord> SelectAlarmRecordByRecover();
//
//		// 插入采集记录
//		int InsertSampleRecord(SampleRecord & record);
//		// 删除采集记录
//		int DeleteSampleRecordByChannel(int channel);
//		// 更新采集记录
//		int UpdateSampleRecordByChannel(int channel, SampleRecord & record);
//		// 查询采集记录
//		int SelectSample(const char * sql, vector<SampleRecord> & records);
//		int SelectAllSampleRecord(vector<SampleRecord> & records);
//		int SelectSampleRecordByChannel(int channel, vector<SampleRecord> & records);
//		int SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate, vector<SampleRecord> & records);
//		int SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime, vector<SampleRecord> & records);
//		int GetCountByChannel(int channel);
//
//	private:
//		// 唯一对象
//		static StorageService * instance_;
//		sqlite3 * memdb_;
//		sqlite3_stmt *stmt_;
//	};
//}

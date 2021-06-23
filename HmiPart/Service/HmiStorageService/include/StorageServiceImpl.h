///*******************************************************************************
// * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
// * All rights reserved.
// *
// * Name     : StorageService.h
// * Author   : TangYao
// * Date     : 2021/01/11
// * Descript : ���ݿ�����ӿ�
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
//		// ������������
//		StorageService(const StorageService& ins) = delete;
//		// ������ȺŸ�ֵ
//		StorageService & operator=(const StorageService& ins) = delete;
//
//	public:
//		// �ֶ���������
//		static StorageService * Ins();
//		// �ֶ��ͷŶ���
//		void Destroy();
//		// �����ڴ����ݿ�
//		int CreateDbOnMemory();
//		// �ļ����ݿ���Ϊ�ڴ����ݿ�ĸ������ݿ�
//		int AttachDbOnMemoryDb();
//		// ͬ���ڴ����ݿ⼰�ļ����ݿ�
//		int FlushSample();
//		// ���뱨����¼
//		int InsertAlarmRecord(AlarmRecord & record);
//		// ɾ��������¼
//		int DeleteAlarmRecordByCategory(int groupname, int groupno);
//		// ���±�����¼
//		int UpdateAlarmRecord(int groupname, int groupno, AlarmRecord & record);
//		// ��ѯ���б�����¼
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
//		// ����ɼ���¼
//		int InsertSampleRecord(SampleRecord & record);
//		// ɾ���ɼ���¼
//		int DeleteSampleRecordByChannel(int channel);
//		// ���²ɼ���¼
//		int UpdateSampleRecordByChannel(int channel, SampleRecord & record);
//		// ��ѯ�ɼ���¼
//		int SelectSample(const char * sql, vector<SampleRecord> & records);
//		int SelectAllSampleRecord(vector<SampleRecord> & records);
//		int SelectSampleRecordByChannel(int channel, vector<SampleRecord> & records);
//		int SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate, vector<SampleRecord> & records);
//		int SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime, vector<SampleRecord> & records);
//		int GetCountByChannel(int channel);
//
//	private:
//		// Ψһ����
//		static StorageService * instance_;
//		sqlite3 * memdb_;
//		sqlite3_stmt *stmt_;
//	};
//}

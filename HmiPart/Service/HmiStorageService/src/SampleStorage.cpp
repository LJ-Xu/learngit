/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleStorage.cpp
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : ���ݲɼ��ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#ifdef WIN32
#include <windows.h> 
#endif
#include "System.h"
#include "SampleStorage.h"
#include <algorithm>
#include <iostream>

namespace Storage
{
	SampleStorage * SampleStorage::instance_ = nullptr;

	SampleStorage * SampleStorage::Ins() {
		static std::once_flag flag;
		call_once(flag, [&]() {
			instance_ = new SampleStorage();
		});
		return instance_;
	}

	void SampleStorage::Load()
	{
		//���س�ʼ������
	}
	void SampleStorage::Trigger(int channel, DDWORD data, XJDataType type, DDWORD date)
	{
		SampleRecord record;
		record.Channel = channel;
		record.Data = data;
		record.Type = type;
		record.Date = date;

		SampleStorageService::Ins()->InsertSampleRecord(record);
	}

	void SampleStorage::Trigger(int channel, DDWORD data, XJDataType type, DDWORD date, int maxcount)
	{
		SampleRecord record;
		record.Channel = channel;
		record.Data = data;
		record.Type = type;
		record.Date = date;
		SampleStorageService::Ins()->InsertSampleRecordByLimit(record,maxcount);
	}

	void SampleStorage::Flush() {
		SampleStorageService::Ins()->Flush();
	}

	void SampleStorage::CleanByGroup(int gName, int gNo)
	{
		SampleStorageService::Ins()->DeleteSampleRecordByGroup(gName, gNo);
	}

	vector<SampleRecord> SampleStorage::QueryByGroupByTime(int gName, int gNo, unsigned long long startTime)
	{
		return std::move(SampleStorageService::Ins()->SelectSampleRecordByStTm(gName, gNo, startTime));
	}

	vector<SampleRecord> SampleStorage::QueryByChannel(int channel)
	{
		return std::move(SampleStorageService::Ins()->SelectSampleRecordByChannel(channel));
	}

	vector<SampleRecord> SampleStorage::QueryByChannel(int channel,unsigned int start, unsigned int length) {
		vector<SampleRecord> tmpRecords = QueryByChannel(channel);
		if (start >= tmpRecords.size())
			return vector<SampleRecord>();
		if (start + length >= tmpRecords.size())
			length = tmpRecords.size() - start;
		vector<SampleRecord>record;
		record.assign(tmpRecords.begin() + start, tmpRecords.begin() + start + length);
		return std::move(record);
	}

	vector<SampleRecord> SampleStorage::QueryByTime(int channel, DDWORD stime, DDWORD etime) {
		return std::move(SampleStorageService::Ins()->SelectSampleRecordByTime(channel, stime, etime));
	}

	vector<SampleRecord> SampleStorage::QueryByDate(int channel, DDWORD sdate, DDWORD edate) {
		return std::move(SampleStorageService::Ins()->SelectSampleRecordByDate(channel, sdate, edate));
	}

	void SampleStorage::QueryByTimePeriod(int channel, int period, vector<SampleRecord> & records) {
		// ��ȡ��ǰʱ��
		/*auto timeNow = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		auto timeStart = timeNow - period;*/
		// ��ѯָ��ͨ������
		vector<SampleRecord> tmpRecords = QueryByChannel(channel);
		if (tmpRecords.size() == 0)
			return;
		auto timeLast = tmpRecords[tmpRecords.size()-1].Date;
		auto timeStart = timeLast - period;
		// ͨ����������
		if (tmpRecords.size() == 0)
			return;
		// ������ʼλ��
		vector<SampleRecord>::iterator iter = find_if(tmpRecords.begin(), tmpRecords.end(), [timeStart](SampleRecord & recd) {
			return recd.Date >= (DDWORD)timeStart;
		});
		if (iter == tmpRecords.end()) {
			return;
		}
		int startIndex = iter != tmpRecords.begin() ? distance(tmpRecords.begin(), iter) - 1 : 0;
		// ������ֹλ��
		iter = find_if(tmpRecords.begin(), tmpRecords.end(), [timeLast](SampleRecord & recd) {
			return recd.Date >= (DDWORD)timeLast;
		});
		int endIndex = iter == tmpRecords.end() ? tmpRecords.size() : distance(tmpRecords.begin(), iter) + 1;
		records.assign(tmpRecords.begin() + startIndex, tmpRecords.begin() + endIndex);
	}

	void SampleStorage::QueryLastRecords(int channel, unsigned int num, vector<SampleRecord> & records) {
		vector<SampleRecord> tmpRecords = QueryByChannel(channel);
		int startIndex = 0;
		if (num < tmpRecords.size())
			startIndex = tmpRecords.size() - num;
		records.assign(tmpRecords.begin() + startIndex, tmpRecords.end());
	}

	int SampleStorage::GetCountByChannel(int channel) {
		return SampleStorageService::Ins()->GetCountByChannel(channel);
	}


	int SampleStorage::GetAllCountByNo(int gName,int gNo, unsigned long long date) {
		return SampleStorageService::Ins()->GetChannelCountByDate(gName, gNo,date);
	}
}

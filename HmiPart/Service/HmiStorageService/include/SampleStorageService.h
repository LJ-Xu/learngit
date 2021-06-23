#ifndef __SAMPLESTORAGESERVICE__
#define	__SAMPLESTORAGESERVICE__

#include <iostream>
#include <vector>
#include <mutex>
#include "BaseStorageService.h"
#include "SampleRecord.h"

namespace Storage
{
	class SampleStorageService : public BaseStorageService
	{
	private:
		SampleStorageService();
		~SampleStorageService() = default;
		SampleStorageService(const SampleStorageService &) = delete;
		SampleStorageService & operator=(const SampleStorageService &) = delete;
	public:
		// 获取数据库对象
		static SampleStorageService * Ins();
		// 销毁数据库对象
		void Destroy();
		// 创建操作录入内存数据库
		int Create();
	public:

		// 插入采集记录
		int InsertSampleRecord(const SampleRecord & record);
		// 插入采集记录
		int InsertSampleRecord(const SampleRecord & record,int maxcount);
		// 删除采集记录
		int DeleteSampleRecordByChannel(int channel);
		// 删除采集记录
		int DeleteSampleRecordByGroup(int gName,int gNo);
		// 更新采集记录
		int UpdateSampleRecordByChannel(const SampleRecord & record);
		// 查询采集记录
		vector<SampleRecord> SelectSample(const char * sql);
		vector<SampleRecord> SelectAllSampleRecord();
		vector<SampleRecord> SelectSampleRecordByChannel(int channel);
		vector<SampleRecord> SelectSampleRecordByNO(int gName, int gNo);
		vector<SampleRecord> SelectSampleRecordByNO(int gName, int gNo, unsigned long long startTime);
		vector<SampleRecord> SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate);
		vector<SampleRecord> SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime);
		int GetCountByChannel(int channel);
		int GetAllCountByNo(int groupId, int no, unsigned long long date);
	private:
		static SampleStorageService * ins;
	};
}
#endif
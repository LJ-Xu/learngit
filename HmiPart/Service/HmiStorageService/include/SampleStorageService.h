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
		// ��ȡ���ݿ����
		static SampleStorageService * Ins();
		// �������ݿ����
		void Destroy();
		// ��������¼���ڴ����ݿ�
		int Create();
	public:

		// ����ɼ���¼
		int InsertSampleRecord(const SampleRecord & record);
		// ����ɼ���¼
		int InsertSampleRecord(const SampleRecord & record,int maxcount);
		// ɾ���ɼ���¼
		int DeleteSampleRecordByChannel(int channel);
		// ɾ���ɼ���¼
		int DeleteSampleRecordByGroup(int gName,int gNo);
		// ���²ɼ���¼
		int UpdateSampleRecordByChannel(const SampleRecord & record);
		// ��ѯ�ɼ���¼
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
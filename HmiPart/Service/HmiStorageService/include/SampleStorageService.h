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
		enum STMTType :char
		{
			INS_InsertSampleRecord,
			INS_InsertSampleRecordByLimit,

			UPD_UpdateSampleOnEarliestDate,
			//DELETE FROM Sample WHERE ChannelNo = %d
			DEL_DeleteByChannel,
			DEL_DeleteByGroup,

			SEL_GetCountByChannel,
			SEL_GetChannelCountByDate,
			SEL_GetAllCount,
			SEL_SelectSampleRecordByChannel,
			SEL_SelectSampleRecordByTime,
			SEL_SelectSampleRecordByDate,
			SEL_SelectSampleRecord,
			SEL_SelectSampleRecordByStTm
		};
		// 获取数据库对象
		static SampleStorageService * Ins();
		void Destroy();
		int Create() override;
	protected:
		int Init() override;
	public://Insert
		int InsertSampleRecord(const SampleRecord & record);
		int InsertSampleRecordByLimit(const SampleRecord & record, int maxcount); 
		int UpdateSampleRecordByChannel(const SampleRecord & record);
	public://Delete
		// 删除采集记录
		int DeleteSampleRecordByChannel(int channel);
		// 删除采集记录
		int DeleteSampleRecordByGroup(int gName, int gNo);
	public://Select
		int GetChannelCountByDate(int groupId, int no, DDWORD date);
		std::vector<SampleRecord> SelectSampleRecordByChannel(int channel);
		std::vector<SampleRecord> SelectSampleRecordByTime(int channel, DDWORD startTime, DDWORD endTime);
		std::vector<SampleRecord> SelectSampleRecordByDate(int channel, DDWORD startDate, DDWORD endDate);
		int GetCountByChannel(int channel);
		vector<SampleRecord> SelectSampleRecord(int gName, int gNo);
		vector<SampleRecord> SelectSampleRecordByStTm(int gName, int gNo, unsigned long long startTime);
	private:
		static SampleStorageService * ins;
	};
}
#endif
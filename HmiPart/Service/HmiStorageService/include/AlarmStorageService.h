#ifndef __ALARMSTORAGESERVICE__
#define __ALARMSTORAGESERVICE__

#include <iostream>
#include <vector>
#include <mutex>
#include "BaseStorageService.h"
#include "AlarmRecord.h"

namespace Storage
{
	class AlarmStorageService : public BaseStorageService
	{
	private:
		AlarmStorageService();
		~AlarmStorageService() = default;
		AlarmStorageService(const AlarmStorageService &) = delete;
		AlarmStorageService & operator=(const AlarmStorageService &) = delete;

	public:
		// ��ȡ�����ӿڶ���
		static AlarmStorageService * Ins();
		// �������ݿ����
		void Destroy();
		// ��������¼���ڴ����ݿ�
		int Create();
		// ��ӱ�����¼
		int InsertAlarmRecord(AlarmRecord & record);
		// ɾ��������¼
		int DeleteAlarmRecordByCategory(int groupname, int groupno);
		// ɾ�����б�����¼
		int DeleteAlarmRecords();
		// ���ݱ������ɾ��������¼
		int DeleteAlarmRecordByGroup(uint16_t alarmGroup);
		// ���ݱ������ɾ��������¼
		int DeleteAlarmRecordByNo(uint16_t alarmNo);
		// ���±�����¼
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ��ȡ������¼
		vector<AlarmRecord> SelectAlarm(const char * sql);
		// ��ѯ������¼
		vector<AlarmRecord> SelectAllAlarmRecords();
		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno);
		vector<AlarmRecord> SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate);
		vector<AlarmRecord> SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime);
		vector<AlarmRecord> SelectAlarmRecordByResolveTick();
		vector<AlarmRecord> SelectAlarmRecordByGroupName(int groupname);
		vector<AlarmRecord> SelectAlarmRecordByGroupNo(int groupno);
		vector<AlarmRecord> SelectAlarmRecordByAlarmLevel(int level);
		vector<AlarmRecord> SelectAlarmRecordByHide(int hideflag);
		vector<AlarmRecord> SelectAlarmRecordByUnRecover();
		vector<AlarmRecord> SelectAlarmRecordByRecover();

	private:
		static AlarmStorageService * ins;
	};
}


#endif // !__ALARMSTORAGESERVICE__


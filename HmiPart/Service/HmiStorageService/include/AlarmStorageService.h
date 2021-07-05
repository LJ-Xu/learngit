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
		// ���±�����¼(��ȫ����)
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ���±�����¼(ȷ�ϼ�¼)
		int UpdateAlarmRecordByConfirm(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ���±�����¼(�ָ���¼)
		int UpdateAlarmRecordByResolve(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ��ȡ������¼
		vector<AlarmRecord> SelectAlarm(const char * sql);
		// ��ѯ������¼
		vector<AlarmRecord> SelectAllAlarmRecords(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno, int startgroupname, int endgroupname, int Recover = 0);
		//Recover:0:�����ж�;1:ȡRecoverd;2:ȡUnRecover;
		vector<AlarmRecord> SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate, int startgroupname, int endgroupname, int Recover=0);
		vector<AlarmRecord> SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByResolveTick();
		vector<AlarmRecord> SelectAlarmRecordByGroupName(int groupname, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByGroupNo(int groupno, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByAlarmLevel(int level, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByHide(int hideflag, int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByUnRecover(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByRecover(int startgroupname, int endgroupname);

	private:
		static AlarmStorageService * ins;
	};
}


#endif // !__ALARMSTORAGESERVICE__


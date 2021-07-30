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
		enum STMTType :char
		{
			INS_InsertAlarmRecord = BASE_DEFAULT,

			UPD_UpdateAlarmRecord_MEM,
			UPD_UpdateAlarmRecord_DB,
			UPD_UpdateAlarmRecordByConfirm_MEM,
			UPD_UpdateAlarmRecordByConfirm_DB,
			UPD_UpdateAlarmRecordByResolve_MEM,
			UPD_UpdateAlarmRecordByResolveDB,

			SEL_SelectAllAlarmRecords,
			SEL_SelectAllRecordsRangeByGroup,
			SEL_SelectByNameAndNo,
			SEL_SelectByNameAndNoAndUnResolved,
			SEL_SelectByNameAndNoAndResolved,
			SEL_SelectByNameAndNoRangeByGroupName,
			SEL_SelectByNameAndNoAndUnResolvedRangeByGroup,
			SEL_SelectByNameAndNoAndResolvedRangeByGroup,
			SEL_SelectByGroupRangeByGroup,
			SEL_SelectByGroupAndUnResolvedRangeByGroup,
			SEL_SelectByGroupAndResolvedRangeByGroup,
			SEL_SelectByNoRangeByGroup,
			SEL_SelectByNoAndUnResolvedRangeByGroup,
			SEL_SelectByNoAndResolvedRangeByGroup,
			SEL_SelectByLevRangeByGroup,
			SEL_SelectByLevAndUnResolvedRangeByGroup,
			SEL_SelectByLevAndResolvedRangeByGroup,
			//SELECT * FROM Alarm WHERE CheckTime = 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectCheckedRangeByGroup,
			//SELECT * FROM Alarm WHERE ResolveTime = 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectResolvedRangeByGroup,
			//SELECT * FROM Alarm WHERE ResolveTime = 0 OR CheckTime = 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectCheckedOrResolvedRangeByGroup,
			//SELECT * FROM Alarm WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectUnResolvedRangeByGroup,
			//SELECT * FROM Alarm WHERE ResolveTime <> 0 AND AlarmGroup BETWEEN %d AND %d OR CheckTime = 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectUnResolvedOrCheckedRangeByGroup,
			//SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectRangeTimeAndGroup,
			//SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld AND ResolveTime <> 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectResolvedRangeTimeAndGroup,
			//SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld AND ResolveTime = 0 AND AlarmGroup BETWEEN %d AND %d
			SEL_SelectUnResolvedRangeTimeAndGroup,
			//SELECT * FROM Alarm WHERE ResolveTime = 0
			SEL_SelectAlarmRecordByResolveTick
		};
		// ��ȡ�����ӿڶ���
		static AlarmStorageService * Ins();
		// �������ݿ����
		void Destroy();
		// ��������¼���ڴ����ݿ�
		int Create()override;
	protected:
		int Init() override;
	public://Insert
		// ��ӱ�����¼
		int InsertAlarmRecord(AlarmRecord & record);
	public://Update
		// ���±�����¼(��ȫ����)
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ���±�����¼(ȷ�ϼ�¼)
		int UpdateAlarmRecordByConfirm(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// ���±�����¼(�ָ���¼)
		int UpdateAlarmRecordByResolve(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
	private://Select
		// ��ȡ������¼
		vector<AlarmRecord> SelectAlarm(sqlite3_stmt *stmt);
	public:
		// ��ѯ������¼
		vector<AlarmRecord> SelectAllAlarmRecords(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByGroupName(int groupname, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByGroupNo(int groupno, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByAlarmLevel(int level, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno, int startgroupname, int endgroupname, int Recover = 0);
		//Recover:0:�����ж�;1:ȡRecoverd;2:ȡUnRecover;
		vector<AlarmRecord> SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByHide(int hideflag, int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByUnRecover(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByRecover(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByResolveTick();

	private:
		static AlarmStorageService * ins;
	};
}


#endif // !__ALARMSTORAGESERVICE__


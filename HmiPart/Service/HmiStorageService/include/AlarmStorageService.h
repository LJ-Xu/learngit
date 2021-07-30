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
		// 获取报警接口对象
		static AlarmStorageService * Ins();
		// 销毁数据库对象
		void Destroy();
		// 创建报警录入内存数据库
		int Create()override;
	protected:
		int Init() override;
	public://Insert
		// 添加报警记录
		int InsertAlarmRecord(AlarmRecord & record);
	public://Update
		// 更新报警记录(完全覆盖)
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 更新报警记录(确认记录)
		int UpdateAlarmRecordByConfirm(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 更新报警记录(恢复记录)
		int UpdateAlarmRecordByResolve(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
	private://Select
		// 获取报警记录
		vector<AlarmRecord> SelectAlarm(sqlite3_stmt *stmt);
	public:
		// 查询报警记录
		vector<AlarmRecord> SelectAllAlarmRecords(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByGroupName(int groupname, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByGroupNo(int groupno, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByAlarmLevel(int level, int startgroupname, int endgroupname, int Recover = 0);
		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno, int startgroupname, int endgroupname, int Recover = 0);
		//Recover:0:不做判断;1:取Recoverd;2:取UnRecover;
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


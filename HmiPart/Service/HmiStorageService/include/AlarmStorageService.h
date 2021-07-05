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
		// 获取报警接口对象
		static AlarmStorageService * Ins();
		// 销毁数据库对象
		void Destroy();
		// 创建报警录入内存数据库
		int Create();
		// 添加报警记录
		int InsertAlarmRecord(AlarmRecord & record);
		// 删除报警记录
		int DeleteAlarmRecordByCategory(int groupname, int groupno);
		// 删除所有报警记录
		int DeleteAlarmRecords();
		// 根据报警组别删除报警记录
		int DeleteAlarmRecordByGroup(uint16_t alarmGroup);
		// 根据报警编号删除报警记录
		int DeleteAlarmRecordByNo(uint16_t alarmNo);
		// 更新报警记录(完全覆盖)
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 更新报警记录(确认记录)
		int UpdateAlarmRecordByConfirm(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 更新报警记录(恢复记录)
		int UpdateAlarmRecordByResolve(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 获取报警记录
		vector<AlarmRecord> SelectAlarm(const char * sql);
		// 查询报警记录
		vector<AlarmRecord> SelectAllAlarmRecords(int startgroupname, int endgroupname);
		vector<AlarmRecord> SelectAlarmRecordByGroup(int groupname, int groupno, int startgroupname, int endgroupname, int Recover = 0);
		//Recover:0:不做判断;1:取Recoverd;2:取UnRecover;
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


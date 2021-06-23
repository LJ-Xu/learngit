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
		// 更新报警记录
		int UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo, const AlarmRecord & record);
		// 获取报警记录
		vector<AlarmRecord> SelectAlarm(const char * sql);
		// 查询报警记录
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


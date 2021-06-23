#include "AlarmStorage.h"

namespace Storage
{
	AlarmStorage * AlarmStorage::instance_ = nullptr;

	AlarmStorage * AlarmStorage::Ins() {
		static std::once_flag flag;
		call_once(flag, [&]() {
			instance_ = new AlarmStorage();
		});
		return instance_;
	}

	void AlarmStorage::Load()
	{
		//加载初始化内容
		//读取文件初始化循环buf
	}

	// 产生报警记录
	void AlarmStorage::Trigger(int groupname, int groupno, string msg, int times, Project::AlarmLevel level)
	{
		AlarmRecord record;
		// 报警事件编号
		record.AlarmGroup = groupname;
		record.AlarmNo = groupno;
		// 报警开始时间
		record.StartTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
		// 报警信息
		record.Title = msg;
		// 报警次数
		record.Times = times;
		// 报警恢复时间
		record.ResolveTick = 0;
		// 报警确认时间
		record.CheckTick = 0;
		//报警等级
		record.Level = level;
		// 数据库添加报警记录
		AlarmStorageService::Ins()->InsertAlarmRecord(record);
		// 添加报警记录
		Add(record);
		//vector<AlarmRecord> records=StorageService::Ins()->SelectAllAlarmRecords();
	}

	// 报警消失,更新记录
	void AlarmStorage::UnTrigger(int idx)
	{
		// 获取报警记录
		AlarmRecord record = recd_[idx];
		// 获取报警恢复时间
		record.ResolveTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
		// 更新数据库记录
		AlarmStorageService::Ins()->UpdateAlarmRecord(record.AlarmGroup, record.AlarmNo, record);
	}
	
	// 报警消失,更新记录
	void AlarmStorage::UnTriggerByEventId(int groupname, int groupno) {
		// 
		vector<AlarmRecord> records = QueryByGroup(groupname, groupno);
		for (size_t i = 0; i < records.size(); ++i) {
			// 获取报警恢复时间
			records[i].ResolveTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			// 更新数据库记录
			AlarmStorageService::Ins()->UpdateAlarmRecord(groupname, groupno, records[i]);
		}
	}
	void AlarmStorage::UnConfirmByEventId(int groupname, int groupno)
	{
		vector<AlarmRecord> records = QueryByGroup(groupname, groupno);
		for (size_t i = 0; i < records.size(); ++i) {
			// 获取报警恢复时间
			records[i].CheckTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			// 更新数据库记录
			AlarmStorageService::Ins()->UpdateAlarmRecord(groupname, groupno, records[i]);
		}
	}

	vector<AlarmRecord> AlarmStorage::QueryAll() {
		// 获取报警记录
		return std::move(AlarmStorageService::Ins()->SelectAllAlarmRecords());
	}

	vector<AlarmRecord> AlarmStorage::QueryByTime(DDWORD stime, DDWORD etime)
	{
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByTime(stime, etime));
	}

	vector<AlarmRecord> AlarmStorage::QueryByDate(DDWORD sdata, DDWORD edata)
	{
		// 获取时间段内的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByDate(sdata, edata));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroup(uint32_t groupname, uint32_t groupno)
	{
		// 获取指定类型的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroup(groupname, groupno));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroupName(uint32_t groupname)
	{
		// 获取指定组名的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroupName(groupname));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroupNo(uint32_t groupno)
	{
		// 获取指定组编号的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroupNo(groupno));
	}

	vector<AlarmRecord> AlarmStorage::QueryByAlarmLevel(uint32_t level)
	{
		// 获取指定组保险等级的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByAlarmLevel(level));
	}
	vector<AlarmRecord> AlarmStorage::QueryByHide(uint32_t hideflag)
	{
		// 获取指定隐藏的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByHide(hideflag));
	}
	vector<AlarmRecord> AlarmStorage::QueryByUnRecover()
	{
		// 获取未恢复的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByUnRecover());
	}
	vector<AlarmRecord> AlarmStorage::QueryByRecover()
	{
		// 获取已恢复的报警记录
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByRecover());
	}
}
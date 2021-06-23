#pragma once
#include "AlarmRecord.h"
#include "BaseStorage.h"
#include "XJRingBuffer.h"
#include "AlarmStorageService.h"
#include <cstring>
#include <chrono>

using namespace std;
using namespace std::chrono;
typedef std::chrono::milliseconds MilliSecs;

namespace Storage
{
	class AlarmStorage : public BaseStorage
	{
	public:
		void Load();
		void Add(AlarmRecord& recd)
		{
			// recd_.Add(recd);
			recd_.push_back(std::move(recd));
		}
		// 产生并插入报警记录
		void Trigger(int groupname, int groupno, string msg, int times, Project::AlarmLevel level);
		// 报警消失,更新记录
		void UnTrigger(int idx);
		// 报警恢复, 更新记录
		void UnTriggerByEventId(int groupname, int groupno);
		// 报警确认，更新记录
		void UnConfirmByEventId(int groupname, int groupno);

		vector<AlarmRecord> QueryAll();
		/*通过时间查询报警信息*/
		vector<AlarmRecord> QueryByTime(DDWORD, DDWORD);
		/*通过日期查询报警信息*/
		vector<AlarmRecord> QueryByDate(DDWORD, DDWORD);
		/*通过组名和组编号查询*/
		vector<AlarmRecord> QueryByGroup(uint32_t, uint32_t);
		/*查询组*/
		vector<AlarmRecord> QueryByGroupName(uint32_t);
		/*查询编号*/
		vector<AlarmRecord> QueryByGroupNo(uint32_t);
		/*查询报险等级*/
		vector<AlarmRecord> QueryByAlarmLevel(uint32_t);
		/*查询隐藏后的信息*/
		vector<AlarmRecord> QueryByHide(uint32_t);
		/*查询未恢复的信息*/
		vector<AlarmRecord> QueryByUnRecover();
		/*查询以恢复的信息*/
		vector<AlarmRecord> QueryByRecover();

		// 创建报警存取对象
		static AlarmStorage * Ins();

	private:
		AlarmStorage() = default;
		~AlarmStorage() = default;
		AlarmStorage(const AlarmStorage &) = delete;
		AlarmStorage & operator=(const AlarmStorage &) = delete;

	private:
		int UpdateCurse(int count)
		{
			// recd_.UpdateReadPos(count);
			return 0;
		}
		int GetCount()
		{
			// return recd_.GetCount();
			return recd_.size();
		}
		char* operator[](int idx)
		{
			// return (char*)recd_.Get(idx);
			return nullptr;
		}
		// XJRingBuffer<AlarmRecord> recd_;
		vector<AlarmRecord> recd_;
		// 唯一对象
		static AlarmStorage * instance_;
	};
}


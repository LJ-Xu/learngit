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
		// ���������뱨����¼
		void Trigger(int groupname, int groupno, string msg, int times, Project::AlarmLevel level);
		// ������ʧ,���¼�¼
		void UnTrigger(int idx);
		void Flush();
		// �����ָ�, ���¼�¼
		void UnTriggerByEventId(int groupname, int groupno);
		// ����ȷ�ϣ����¼�¼
		void UnConfirmByEventId(int groupname, int groupno);

		vector<AlarmRecord> QueryAll(int startgroupname = -1, int endgroupname = -1);
		/*ͨ��ʱ���ѯ������Ϣ*/
		//Recover:0:�����ж�;1:ȡRecoverd;2:ȡUnRecover;
		vector<AlarmRecord> QueryByTime(DDWORD, DDWORD, int startgroupname, int endgroupname, int Record = 0);
		/*ͨ�����ڲ�ѯ������Ϣ*/
		vector<AlarmRecord> QueryByDate(DDWORD, DDWORD, int startgroupname, int endgroupname, int Record = 0);
		/*ͨ�����������Ų�ѯ*/
		vector<AlarmRecord> QueryByGroup(uint32_t, uint32_t, int startgroupname = -1, int endgroupname = -1, int Record = 0);
		/*��ѯ��*/
		vector<AlarmRecord> QueryByGroupName(uint32_t, int startgroupname, int endgroupname, int Record = 0);
		/*��ѯ���*/
		vector<AlarmRecord> QueryByGroupNo(uint32_t, int startgroupname, int endgroupname, int Record = 0);
		/*��ѯ���յȼ�*/
		vector<AlarmRecord> QueryByAlarmLevel(uint32_t, int startgroupname, int endgroupname, int Record = 0);
		/*��ѯ���غ����Ϣ*/
		vector<AlarmRecord> QueryByHide(uint32_t, int startgroupname, int endgroupname);
		/*��ѯδ�ָ�����Ϣ*/
		vector<AlarmRecord> QueryByUnRecover(int startgroupname, int endgroupname);
		/*��ѯ�Իָ�����Ϣ*/
		vector<AlarmRecord> QueryByRecover(int startgroupname, int endgroupname);

		// ����������ȡ����
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
		// Ψһ����
		static AlarmStorage * instance_;
	};
}


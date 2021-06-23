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
		// �����ָ�, ���¼�¼
		void UnTriggerByEventId(int groupname, int groupno);
		// ����ȷ�ϣ����¼�¼
		void UnConfirmByEventId(int groupname, int groupno);

		vector<AlarmRecord> QueryAll();
		/*ͨ��ʱ���ѯ������Ϣ*/
		vector<AlarmRecord> QueryByTime(DDWORD, DDWORD);
		/*ͨ�����ڲ�ѯ������Ϣ*/
		vector<AlarmRecord> QueryByDate(DDWORD, DDWORD);
		/*ͨ�����������Ų�ѯ*/
		vector<AlarmRecord> QueryByGroup(uint32_t, uint32_t);
		/*��ѯ��*/
		vector<AlarmRecord> QueryByGroupName(uint32_t);
		/*��ѯ���*/
		vector<AlarmRecord> QueryByGroupNo(uint32_t);
		/*��ѯ���յȼ�*/
		vector<AlarmRecord> QueryByAlarmLevel(uint32_t);
		/*��ѯ���غ����Ϣ*/
		vector<AlarmRecord> QueryByHide(uint32_t);
		/*��ѯδ�ָ�����Ϣ*/
		vector<AlarmRecord> QueryByUnRecover();
		/*��ѯ�Իָ�����Ϣ*/
		vector<AlarmRecord> QueryByRecover();

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


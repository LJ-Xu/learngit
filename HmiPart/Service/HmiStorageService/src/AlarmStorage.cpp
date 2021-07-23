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
		//���س�ʼ������
		//��ȡ�ļ���ʼ��ѭ��buf
	}

	void AlarmStorage::Flush() {
		AlarmStorageService::Ins()->Flush();
	}

	// ����������¼
	void AlarmStorage::Trigger(int groupname, int groupno, string msg, int times, Project::AlarmLevel level)
	{
		AlarmRecord record;
		// �����¼����
		record.AlarmGroup = groupname;
		record.AlarmNo = groupno;
		// ������ʼʱ��
		record.StartTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
		// ������Ϣ
		record.Title = msg;
		// ��������
		record.Times = times;
		// �����ָ�ʱ��
		record.ResolveTick = 0;
		// ����ȷ��ʱ��
		record.CheckTick = 0;
		//�����ȼ�
		record.Level = level;
		// ���ݿ���ӱ�����¼
		AlarmStorageService::Ins()->InsertAlarmRecord(record);
		// ��ӱ�����¼
		Add(record);
		//vector<AlarmRecord> records=StorageService::Ins()->SelectAllAlarmRecords();
	}

	// ������ʧ,���¼�¼
	void AlarmStorage::UnTrigger(int idx)
	{
		// ��ȡ������¼
		AlarmRecord record = recd_[idx];
		// ��ȡ�����ָ�ʱ��
		record.ResolveTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
		// �������ݿ��¼
		AlarmStorageService::Ins()->UpdateAlarmRecord(record.AlarmGroup, record.AlarmNo, record);
	}
	
	// ������ʧ,���¼�¼
	void AlarmStorage::UnTriggerByEventId(int groupname, int groupno) {
		// 
		vector<AlarmRecord> records = QueryByGroup(groupname, groupno);
		for (size_t i = 0; i < records.size(); ++i) {
			// ��ȡ�����ָ�ʱ��
			records[i].ResolveTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			// �������ݿ��¼
			AlarmStorageService::Ins()->UpdateAlarmRecordByResolve(groupname, groupno, records[i]);
		}
	}
	void AlarmStorage::UnConfirmByEventId(int groupname, int groupno)
	{
		vector<AlarmRecord> records = QueryByGroup(groupname, groupno);
		for (size_t i = 0; i < records.size(); ++i) {
			// ��ȡ�����ָ�ʱ��
			records[i].CheckTick = duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count();
			// �������ݿ��¼
			AlarmStorageService::Ins()->UpdateAlarmRecordByConfirm(groupname, groupno, records[i]);
		}
	}

	vector<AlarmRecord> AlarmStorage::QueryAll(int startgroupname, int endgroupname) {
		// ��ȡ������¼
		return std::move(AlarmStorageService::Ins()->SelectAllAlarmRecords(startgroupname, endgroupname));
	}

	vector<AlarmRecord> AlarmStorage::QueryByTime(DDWORD stime, DDWORD etime, int startgroupname, int endgroupname, int Record)
	{
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByTime(stime, etime, startgroupname, endgroupname, Record));
	}

	vector<AlarmRecord> AlarmStorage::QueryByDate(DDWORD sdata, DDWORD edata, int startgroupname, int endgroupname, int Record)
	{
		// ��ȡʱ����ڵı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByDate(sdata, edata, startgroupname, endgroupname, Record));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroup(uint32_t groupname, uint32_t groupno, int startgroupname, int endgroupname, int Record)
	{
		// ��ȡָ�����͵ı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroup(groupname, groupno, startgroupname, endgroupname, Record));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroupName(uint32_t groupname, int startgroupname, int endgroupname, int Record)
	{
		// ��ȡָ�������ı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroupName(groupname, startgroupname, endgroupname, Record));
	}

	vector<AlarmRecord> AlarmStorage::QueryByGroupNo(uint32_t groupno, int startgroupname, int endgroupname, int Record)
	{
		// ��ȡָ�����ŵı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByGroupNo(groupno, startgroupname, endgroupname, Record));
	}

	vector<AlarmRecord> AlarmStorage::QueryByAlarmLevel(uint32_t level, int startgroupname, int endgroupname, int Record)
	{
		// ��ȡָ���鱣�յȼ��ı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByAlarmLevel(level,Record, startgroupname, endgroupname));
	}
	vector<AlarmRecord> AlarmStorage::QueryByHide(uint32_t hideflag, int startgroupname, int endgroupname)
	{
		// ��ȡָ�����صı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByHide(hideflag, startgroupname, endgroupname));
	}
	vector<AlarmRecord> AlarmStorage::QueryByUnRecover(int startgroupname, int endgroupname)
	{
		// ��ȡδ�ָ��ı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByUnRecover(startgroupname, endgroupname));
	}
	vector<AlarmRecord> AlarmStorage::QueryByRecover(int startgroupname, int endgroupname)
	{
		// ��ȡ�ѻָ��ı�����¼
		return std::move(AlarmStorageService::Ins()->SelectAlarmRecordByRecover(startgroupname, endgroupname));
	}
}
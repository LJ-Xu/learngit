#include "AlarmStorageService.h"
//#include "Logger.h"
namespace Storage
{
	AlarmStorageService * AlarmStorageService::ins = nullptr;

	AlarmStorageService::AlarmStorageService() : 
		//BaseStorageService("..\\..\\HMI\\Alarm.db", "Alarm") {
		BaseStorageService("././HMI/Alarm.db", "Alarm") {
		// �����ڴ����ݿ�
		if (Create()) {
			Close();
		}
		// �����ļ����ݿ�
		Attach();
	}

	AlarmStorageService * AlarmStorageService::Ins() {
		static once_flag flag;
		call_once(flag, [&]() {
			ins = new AlarmStorageService();
		});
		return ins;
	}

	/**
	 * @brief  : �ͷű������ݿ�
	 * @return : void
	 * @birth  : created by TangYao on 2021/02/04
	 */
	void AlarmStorageService::Destroy() {
		if (ins) {
			delete ins;
			ins = nullptr;
		}
	}

	/**
	 * @brief  : ��������¼���ڴ����ݿ�
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::Create() {
		int ret = 0;
		char sql[SQLCMDLEN] = { 0 };
		// ������¼�ڴ��SQL���
		snprintf(sql, sizeof(sql), "CREATE TABLE Alarm (AlarmGroup INT NOT NULL,AlarmNo INT NOT NULL,StartTime INT64 NOT NULL,Title Text NOT NULl,Times INT NOT NULL,ResolveTime INT64 NOT NULL,CheckTime INT64 NOT NULL,Level INT NOT NULL);");
		// ����������¼�ڴ��
		ret = ExecuteSql(sql);
		return ret;
	}

	/**
	 * @brief  : ��ӱ�����¼
	 * @params : record ������¼
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::InsertAlarmRecord(AlarmRecord & record) {
		//char sql[512] = { 0 };
		//// ���뱨����¼���ڴ��
		//snprintf(sql, sizeof(sql), "INSERT INTO Alarm VALUES(\
		//%d, %d, %lld, '%s', %d, %lld, %lld, %d);",
		//	record.AlarmGroup, record.AlarmNo,
		//	record.StartTick, record.Title.c_str(),
		//	record.Times, record.ResolveTick,
		//	record.CheckTick, record.Level);
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "INSERT INTO Alarm VALUES(%d, %d, %lld, '%s', %d, %lld, %lld, %d);",
			record.AlarmGroup, record.AlarmNo , record.StartTick, record.Title.c_str(),
			record.Times, record.ResolveTick, record.CheckTick, record.Level);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : ɾ��������¼
	 * @params : eventId		�����¼�ID
	 * @return : void
	 * @birth  : created by TangYao on 2021/01/08
	 */
	int AlarmStorageService::DeleteAlarmRecordByCategory(int groupname, int groupno) {
		int ret = 0;
		char * errMsg = NULL;
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "DELETE FROM Alarm WHERE AlarmGroup = %d AND AlarmNo = %d; \
									DELETE FROM fileDb.Alarm WHERE AlarmGroup = %d AND AlarmNo = %d;",
			groupname, groupno, groupname, groupno);
		return ExecuteSql(sql);
	}
	/**
	 * @brief  : ɾ��������¼
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::DeleteAlarmRecords() {
		char sql[SQLCMDLEN] = { 0 };
		// ɾ�����б�����¼
		snprintf(sql, sizeof(sql), "DELETE From Alarm;Delete From fileDb.Alarm;");
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : ɾ��������¼
	 * @params : alarmGroup �������
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::DeleteAlarmRecordByGroup(uint16_t alarmGroup) {
		char sql[SQLCMDLEN] = { 0 };
		// ɾ��ָ��������¼
		snprintf(sql, sizeof(sql), "DELETE FROM Alarm WHERE AlarmGroup = %d; \
								DELETE FROM fileDb.Alarm WHERE AlarmGroup = %d;",
			alarmGroup, alarmGroup);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : ɾ��������¼
	 * @params : alarmNo �������
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::DeleteAlarmRecordByNo(uint16_t alarmNo) {
		char sql[SQLCMDLEN] = { 0 };
		// ɾ��ָ��������¼
		snprintf(sql, sizeof(sql), "DELETE FROM Alarm WHERE AlarmNo = %d; \
								DELETE FROM fileDb.Alarm WHERE AlarmNo = %d;",
			alarmNo, alarmNo);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : ���±�����¼
	 * @params : alarmGroup �������
	 * @params : record ������¼
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	int AlarmStorageService::UpdateAlarmRecord(uint16_t alarmGroup, uint16_t alarmNo,const AlarmRecord & record) {
		char sql[SQLCMDLEN] = { 0 };
		// ����ָ��������¼
		snprintf(sql, sizeof(sql), "UPDATE Alarm SET StartTime = %lld, Title = '%s', Times = %d, ResolveTime = %lld, \
								CheckTime = %lld, Level = %d WHERE AlarmGroup = %d AND AlarmNo = %d; \
								UPDATE fileDb.Alarm SET StartTime = %lld, Title = '%s', Times = %d, ResolveTime = %lld, \
								CheckTime = %lld, Level = %d WHERE AlarmGroup = %d AND AlarmNo = %d;",
			record.StartTick, record.Title.c_str(), record.Times, record.ResolveTick,
			record.CheckTick, record.Level, record.AlarmGroup, record.AlarmNo,
			record.StartTick, record.Title.c_str(), record.Times, record.ResolveTick,
			record.CheckTick, record.Level, record.AlarmGroup, record.AlarmNo);
		return ExecuteSql(sql);
	}

	/**
	 * @brief  : ��ȡ������¼
	 * @params : sql	 SQL���
	 * @params : records ������¼����
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	vector<AlarmRecord>  AlarmStorageService::SelectAlarm(const char * sql) {
		vector<AlarmRecord>  records;
		int ret = 0;
		struct sqlite3_stmt *stmt;
		// ��ȡ������¼
		ret = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if (ret != SQLITE_OK) {
			const char * file = strrchr(__FILE__, '\\') + 1;
			fprintf(stderr, "%s[%d] ErrCode[%d] Msg: %s\n", file, __LINE__, ret, sqlite3_errmsg(db));
			return records;
		}
		// ���������
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			AlarmRecord record;
			record.AlarmGroup = sqlite3_column_int(stmt, 0);	// �������
			record.AlarmNo = sqlite3_column_int(stmt, 1);		// �������
			record.StartTick = sqlite3_column_int64(stmt, 2);	// ��ʼʱ��
			record.Title = (char *)sqlite3_column_text(stmt, 3);// �¼�����
			record.Times = sqlite3_column_int(stmt, 4);			// ��������
			record.ResolveTick = sqlite3_column_int64(stmt, 5);	// �ָ��¼�
			record.CheckTick = sqlite3_column_int64(stmt, 6);	// ȷ��ʱ��
			record.Level = sqlite3_column_int(stmt, 7);			// �����ȼ�
			records.push_back(record);
		}
		// �ͷŶ���
		sqlite3_finalize(stmt);
		return records;
	}

	/**
	 * @brief  : ��ȡ������¼
	 * @params : records ������¼����
	 * @return : int
	 * @birth  : created by TangYao on 2021/02/04
	 */
	vector<AlarmRecord> AlarmStorageService::SelectAllAlarmRecords() {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm union all SELECT * FROM fileDb.Alarm");
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroup(int groupname, int groupno) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE AlarmGroup = %d AND AlarmNo = %d union all \
									SELECT * FROM fileDb.Alarm WHERE AlarmGroup = %d AND AlarmNo = %d;",
			groupname, groupno, groupname, groupno);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroupName(int groupname) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE AlarmGroup = %d union all \
									SELECT * FROM fileDb.Alarm WHERE AlarmGroup = %d;", groupname, groupname);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByGroupNo(int groupno) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE AlarmNo = %d union all \
									SELECT * FROM fileDb.Alarm WHERE AlarmNo = %d;", groupno, groupno);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByAlarmLevel(int level) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE Level = %d union all \
									SELECT * FROM fileDb.Alarm WHERE Level = %d;", level, level);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByHide(int hideflag) {
		char sql[SQLCMDLEN] = { 0 };
		switch (hideflag & 0x7)
		{
		case 1:			//bit0 = 1 ������ȷ����Ϣ(��ȡδȷ����Ϣ)
			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE CheckTime = 0 union all \
									SELECT * FROM fileDb.Alarm WHERE CheckTime = 0;");
			break;
		case 2:			//bit1 = 1 �����ѻָ���Ϣ(��ȡδ�ָ���Ϣ)
			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 union all \
									SELECT * FROM fileDb.Alarm WHERE ResolveTime = 0;");
			break;
		case 3:			//bit0 = 1��bit1 = 1 ������ȷ�Ϻ��ѻָ���Ϣ(��ȡδ�ָ���Ϣ��δȷ����Ϣ)
			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 OR CheckTime = 0 union all \
									SELECT * FROM fileDb.Alarm WHERE RResolveTime = 0 OR CheckTime = 0;");
			break;
		case 4:			//bit2 = 1 ����δ�ָ���Ϣ(��ȡ�ѻָ���)
			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 union all \
									SELECT * FROM fileDb.Alarm WHERE ResolveTime <> 0;");
			break;
		case 5:			//bit0 = 1 bit2 = 1  ������ȷ�Ϻ�δ�ָ���Ϣ����ȡδȷ�ϻ��ѻָ�����Ϣ��
			snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 OR CheckTime = 0 union all \
									SELECT * FROM fileDb.Alarm WHERE RResolveTime <> 0 OR CheckTime = 0;");
			break;
		case 6:			//bit1 = 1 bit2 = 1 �����ѻָ���δ�ָ���Ϣ
		case 7:			//bit0 = 1 bit1 = 1 bit2 = 1 ������ȷ�ϡ��ѻָ���δ�ָ���Ϣ
			return vector<AlarmRecord>();
		default:
			return SelectAllAlarmRecords();
		}
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByUnRecover() {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime = 0 union all \
									SELECT * FROM fileDb.Alarm WHERE ResolveTime = 0;");
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByRecover() {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime <> 0 union all \
									SELECT * FROM fileDb.Alarm WHERE ResolveTime <> 0;");
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByDate(DDWORD startDate, DDWORD endDate) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld union all \
									SELECT * FROM fileDb.Alarm WHERE StartTime BETWEEN %lld AND %lld;",
			startDate, endDate, startDate, endDate);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByTime(DDWORD startTime, DDWORD endTime) {
		char sql[SQLCMDLEN] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE StartTime BETWEEN %lld AND %lld union all \
									SELECT * FROM fileDb.Alarm WHERE StartTime BETWEEN %lld AND %lld",
			startTime, endTime, startTime, endTime);
		return SelectAlarm(sql);
	}

	vector<AlarmRecord> AlarmStorageService::SelectAlarmRecordByResolveTick()
	{
		/*char sql[256] = { 0 };
		snprintf(sql, sizeof(sql), "SELECT * FROM Alarm WHERE ResolveTime > 0  \ SELECT * FROM fileDb.Alarm WHERE ResolveTime > 0");*/
		return SelectAlarm("SELECT * FROM Alarm WHERE ResolveTime = 0;");
	}
}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorageService.h
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : ������¼���ݿ�ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#ifndef __OPERATORSTORAGESERVICE__
#define	__OPERATORSTORAGESERVICE__

#include <iostream>
#include <vector>
#include <mutex>
#include "BaseStorageService.h"
#include "OperatorRecord.h"

namespace Storage
{
	class OperatorStorageService : public BaseStorageService
	{
	private:
		OperatorStorageService();
		~OperatorStorageService() = default;
		OperatorStorageService(const OperatorStorageService &) = delete;
		OperatorStorageService & operator=(const OperatorStorageService &) = delete;

	protected:
		int Init() override;
	public:
		enum STMTType :char
		{
			//INSERT INTO Operation VALUES(""%d, %lld, %lld, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s')
			INS_InsertRecord = BASE_DEFAULT,
			//DELETE FROM Operation
			DEL_DeleteRecords,
			//UPDATE Operation SET Date = %lld,Time = %lld,UserName = '%s',Class = '%s',Window = %d,ObjectName = '%s',Comment = '%s',Action = '%s',Address = '%s',Information = '%s' WHERE ID = %d; 
			UPD_UpdateRecord,
			//SELECT * FROM Operation
			SEL_SelectRecords,
			//SELECT * FROM Operation WEERE ID > %d
			SEL_SelectRecordsFromId,
			//SELECT * FROM Operation LIMIT %d,%d
			SEL_SelectRecordsFromIdLimitByCount,
			//SELECT * FROM Operation WHERE UserName = '%s'
			SEL_SelectRecordsByUserName,
			//SELECT * FROM Operation WHERE Date BETWEEN %lld AND %lld
			SEL_SelectRecordsByDate,
			//SELECT * FROM Operation WHERE Time BETWEEN %lld AND %lld
			SEL_SelectRecordsByTime
		};
	public:
		// ��ȡ���ݿ����
		static OperatorStorageService * Ins();
		// ��������¼���ڴ����ݿ�
		int Create() override;
		// �������ݿ����
		void Destroy();
	public:
		// ��Ӳ�����¼
		int InsertOperatorRecord(OperatorRecord & record);
		// ɾ��������¼
		int DeleteOperatorRecords();
		// ���²�����¼
		int UpdateOperatorRecord(int id, OperatorRecord & record);
		// ��ѯ������¼
		int SelectRecords(sqlite3_stmt *stmt, vector<OperatorRecord> & records);
		int SelectOperatorRecords(vector<OperatorRecord> & records);
		int SelectOperatorRecordsByLimit(int startIndex,int count, vector<OperatorRecord> &  records);
		int SelectOperatorRecordsByUser(const char * userName, vector<OperatorRecord> & records);
		int SelectOperatorRecordsByDate(DDWORD sDate, DDWORD eDate, vector<OperatorRecord> & records);
		int SelectOperatorRecordsByTime(DDWORD sTime, DDWORD eTime, vector<OperatorRecord> & records);

	private:
		static OperatorStorageService * ins;
	};
}

#endif // !__OPERATORSTORAGESERVICE__


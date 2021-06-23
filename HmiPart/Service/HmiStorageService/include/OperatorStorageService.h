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
		// ��������¼���ڴ����ݿ�
		int Create();

	public:
		// ��ȡ���ݿ����
		static OperatorStorageService * Ins();
		// �������ݿ����
		void Destroy();
		// ��Ӳ�����¼
		int InsertOperatorRecord(OperatorRecord & record);
		// ɾ��������¼
		int DeleteOperatorRecords();
		// ���²�����¼
		int UpdateOperatorRecord(int id, OperatorRecord & record);
		// ��ѯ������¼
		int SelectRecords(const char * sql, vector<OperatorRecord> & records);
		int SelectOperatorRecords(vector<OperatorRecord> & records);
		int SelectOperatorRecordsByUser(const char * userName, vector<OperatorRecord> & records);
		int SelectOperatorRecordsByDate(DDWORD sDate, DDWORD eDate, vector<OperatorRecord> & records);
		int SelectOperatorRecordsByTime(DDWORD sTime, DDWORD eTime, vector<OperatorRecord> & records);

	private:
		static OperatorStorageService * ins;
	};
}

#endif // !__OPERATORSTORAGESERVICE__


/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorageService.h
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : 操作记录数据库接口
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
		// 创建操作录入内存数据库
		int Create();

	public:
		// 获取数据库对象
		static OperatorStorageService * Ins();
		// 销毁数据库对象
		void Destroy();
		// 添加操作记录
		int InsertOperatorRecord(OperatorRecord & record);
		// 删除操作记录
		int DeleteOperatorRecords();
		// 更新操作记录
		int UpdateOperatorRecord(int id, OperatorRecord & record);
		// 查询操作记录
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


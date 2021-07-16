#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeStorage.h
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : ���ݲɼ��ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "SampleRecord.h"
#include "BaseStorage.h"
#include "XJRingBuffer.h"
#include "RecipeStorageService.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Storage
{
	class RecipeStorage 
	{
	public:
		vector<vector<string>>  QueryByRepiceName(string name, int col);
		vector<vector<string>>  QueryByKey(string recipename, string key, vector<string> colname);
		vector<vector<string>>  QueryByValue(string recipename, vector<string> key, vector<string> colname);

		bool DeleteRecord(string name, int row);
		bool AddRecord(string name);
		bool InsertRecord(string name, int row);
		bool CopyRecord(string name, int row, vector<Project::ColDataTypeInfo>& data);
		bool MoveUpRecord(string name, int row);
		bool MoveDownRecord(string name, int row);
		bool UpdateRecord(string groupname, string colname, unsigned int rowno, string data);
		int  GetCountByRepiceName(string name);
		// ������ȡ����
		static RecipeStorage * Ins();

	private:
		// Ψһ����
		static RecipeStorage * instance_;
	};
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseStorageService.h
 * Author   : TangYao
 * Date     : 2021/02/04
 * Descript : ���ݿ�����ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#ifndef __BASESTORAGESERVICE__
#define __BASESTORAGESERVICE__

#include <iostream>
#include "sqlite3.h"
#define SQLCMDLEN 512
using namespace std;

namespace Storage
{
	class BaseStorageService
	{
	protected:
		BaseStorageService(const string & dbPath, const string & tbName, int maxCnt = 10000);
		virtual ~BaseStorageService();

	protected:
		// �����ݿ�
		int Open();
		// �ر����ݿ�
		void Close();
		// ִ��sql���
		int ExecuteSql(const char * sql);
		// �����ڴ����ݿ�
		virtual int Create() = 0;
		// �����ļ����ݿ�
		int Attach();

	public:
		// �������ļ����ݿ�
		int Flush();
		// ����ָ����������
		int Flush(int count);
		// ��ȡ�ڴ�����������
		int Count();

	protected:
		sqlite3 * db;	// ���ݿ���
		string dbPath;	// ���ݿ�·��
		string tbName;	// ���ݱ�����
		bool isOpened;	// �Ƿ�����ݿ�
	};
}

#endif // !__BASESTORAGESERVICE__


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
#include <map>
#include "sqlite3.h"
#include "RunEnv.h"
#define SQLCMDLEN 512//sql�����󳤶�
#define BINCOUNT 10//ÿ��sql��ӵ�е�BIN�ļ���
#define BINSIZE  1000//ÿ��bin�ļ��������Ŀ��
#define MAXCOUNT 10000//�ڴ����ݿ������Ŀ��
#define FLUSHTIMEOUT 600000//����FLUSH��Bin�ļ���Ϊ�������ʱ��
using namespace std;

namespace Storage
{
	class BaseStorageService
	{
	protected:
		//BaseStorageService(const string & dbPath, const string & tbName, int maxCnt = 10000);
		BaseStorageService(const string & dbPath, const string & tbName);
		//BaseStorageService(const char* tbname);
		virtual ~BaseStorageService();

	protected:
		enum BaseSTMTType :char
		{
			BASE_FLUSH,
			BASE_DELETE,
			BASE_DEFAULT
		};
		// �����ݿ�
		int Open();
		// �ر����ݿ�
		void Close();
		// ִ��sql���
		//int ExecuteSql(const char * sql);
		// �����ڴ����ݿ�
		virtual int Create() = 0;

	public:
		// �������ļ����ݿ�
		int Flush();
		// ����ָ����������
		int Flush(int count);
		// ��ȡ�ڴ�����������
		int Count();
		bool NewFMT(char key, const char* sql, int len);
		bool FinFMT(char key);
		// �����ļ����ݿ�
		int Attach();
	protected:
		sqlite3_stmt* GetSTMT(char key);
		void ExecBegin();
		void ExecCommit();
		// ֱ��ִ��sql���
		int ExecuteSql(const char * sql);
	protected:
		virtual int Init() = 0;

	protected:
		sqlite3 * db;	// ���ݿ���
		unsigned long long curId;
		string dbPath;	// ���ݿ�·��
		string tbName;	// ���ݱ�����
		bool isOpened;	// �Ƿ�����ݿ�
	private:
		std::map<char, sqlite3_stmt*> StoreObj;
	};
}

#endif // !__BASESTORAGESERVICE__


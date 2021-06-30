/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : OperatorStorage.h
 * Author   : TangYao
 * Date     : 2021/02/24
 * Descript : ���ݲɼ��ӿ�
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "OperatorRecord.h"
#include "BaseStorage.h"
#include "XJRingBuffer.h"
#include "OperatorStorageService.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Storage
{
	class OperatorStorage : public BaseStorage
	{
	public:
		// ��ӿؼ�������¼
		int Trigger(string userName, string devName, size_t win, string ctrlName, int action, Project::BaseVar var, double orgVal, double newVal, XJDataType tp);
		int Trigger(string userName, string devName, size_t win, string ctrlName, Project::BaseVar var, char * orgVal, char * newVal);
		int Trigger(string userName, size_t win, string ctrlName, int action, int startWin, int endWin);
		int Trigger(string userName, string devName, size_t win, string ctrlName, int action, Project::BaseVar var, string describe, int count);
		int Trigger(string userName, size_t win, string ctrlName, int action, vector<string>& describe);
		int CleanRcd();		//���������¼
		void Rcd(RecordEnum recdEm, char* uname,char* addrname,char* ctrlname,char* comt,char* data);
		void Rcd(RecordEnum recdEm, unsigned int uid,unsigned int vid, 
							unsigned int ctrlid, DDWORD oldval, DDWORD newval);
		void QueryByTime(DDWORD stime, DDWORD etime, vector<OperatorRecord> & records);
		void QueryByDate(DDWORD sdate, DDWORD edate, vector<OperatorRecord> & records);
		void QueryByUser(char* name, vector<OperatorRecord> & records);
		// ����������¼���ݿ����
		static OperatorStorage * Ins();

		void Load() { }
		void Add(OperatorRecord & recd) { }

	private:
		int UpdateCurse(int count) { return 0; }
		int GetCount() { return 0; }
		char* operator[](int idx) { return nullptr; }

	private:
		// ������¼���ݿ����
		static OperatorStorage * instance_;
	};
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SampleStorage.h
 * Author   : TangYao
 * Date     : 2021/01/13
 * Descript : 数据采集接口
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "SampleRecord.h"
#include "BaseStorage.h"
#include "XJRingBuffer.h"
#include "SampleStorageService.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

namespace Storage
{
	class SampleStorage :public BaseStorage
	{
	public:
		void Load();
		void Add(SampleRecord& recd)
		{
			// recd_.Add(recd);
		}
		void Trigger(int channel, DDWORD data, XJDataType type, DDWORD date);
		void Trigger(int channel, DDWORD data, XJDataType type, DDWORD date,int maxcount);
		void Flush();
		void CleanByGroup(int gName,int gNo);
		//vector<SampleRecord> QueryByGroup(int gName, int gNo);
		vector<SampleRecord> QueryByGroupByTime(int gName, int gNo,unsigned long long startTime);
		vector<SampleRecord> QueryByChannel(int channel);
		vector<SampleRecord> QueryByChannel(int channel, unsigned int start, unsigned int length);
		vector<SampleRecord> QueryByTime(int channel, DDWORD stime, DDWORD etime);
		vector<SampleRecord> QueryByDate(int channel, DDWORD sdate, DDWORD edate);
		void QueryByTimePeriod(int channel, int period, vector<SampleRecord> & records);
		void QueryLastRecords(int channel, unsigned int num, vector<SampleRecord> & records);
		int  GetCountByChannel(int channel);
		int GetAllCountByNo(int gName, int gNo, unsigned long long date);
		// 创建报警存取对象
		static SampleStorage * Ins();

	private:
		int UpdateCurse(int count)
		{
			// recd_.UpdateReadPos(count);
			return 0;
		}
		int GetCount()
		{
			// return recd_.GetCount();
			return 0;
		}
		char* operator[](int idx)
		{
			// return (char*)recd_.Get(idx);
			return nullptr;
		}
		// XJRingBuffer<SampleRecord> recd_;
		// 唯一对象
		static SampleStorage * instance_;
	};
}

#pragma once
#include "stdafx.h"
#include "BaseRecord.h"
#include <iostream>
#include <string>
#include "AlarmInfoRes.h"
using namespace std;

namespace Storage
{
	struct AlarmRecord : BaseRecord
	{
		//int GroupName;		//组名
		//int GroupNo;			//组编号
		//uint64_t StartTick;//报警开始时间
		//string Title; //报警的内容
		//uint32_t Times;//报警产生的次数
		//uint64_t ResolveTick;//报警恢复时间
		//uint64_t CheckTick;//报警确认时间
		//Project::AlarmLevel Level;	//报警级别
		AlarmRecord() {}
		AlarmRecord(AlarmRecord&& ar)noexcept
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = std::move(ar.Title);
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
		}
		AlarmRecord(const AlarmRecord& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = ar.Title;
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
		}
		AlarmRecord& operator=(AlarmRecord&& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = std::move(ar.Title);
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
			return *this;
		}
		AlarmRecord& operator=(const AlarmRecord& ar)
		{
			AlarmGroup = ar.AlarmGroup;
			AlarmNo = ar.AlarmNo;
			StartTick = ar.StartTick;
			Title = ar.Title;
			Times = ar.Times;
			ResolveTick = ar.ResolveTick;
			CheckTick = ar.CheckTick;
			Level = ar.Level;
			return *this;
		}

		uint16_t AlarmGroup;	// 报警组别
		uint16_t AlarmNo;		// 报警编号
		size_t Times;			// 报警产生的次数
		size_t Level;			// 报警等级
		uint64_t StartTick;		// 报警开始时间	
		uint64_t ResolveTick;	// 报警恢复时间
		uint64_t CheckTick;		// 报警确认时间
		string Title;			// 报警的内容
	};

}
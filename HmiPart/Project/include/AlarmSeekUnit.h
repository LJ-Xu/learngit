#pragma once
#include "DataVarInfo.h"
#include <time.h>
#include "MoveableDisplayUnit.h"
#include "AlarmInfoRes.h"
namespace Project
{
	//enum SeekMode { DATE, TIME, GROUP, NUM, LEVEL, REG };
	class AlarmSeekUnit : public MoveableDisplayUnit
	{
	public:
		bool EnableSearch;				//是否使用查询功能
		//SeekMode SearchMode;			//查询方式

		DataVarId SeekCtrlVar;			//控制查询寄存器 ON状态查询模式
		/*按寄存器*/
		DataVarId SeekModeVar;			//控制查询方式寄存器

		/*按日期*/
		DDWORD SearchDate;
		vector<DataVarId> SearchDateVarId;
		/*按时间*/
		DDWORD SearchTimeStart;
		DDWORD SearchTimeEnd;
		vector<DataVarId> SearchTimeVar;
		/*按分组*/
		int SearchGroup;
		DataVarId SearchGroupVar;
		/*按编号*/
		int SearchNo;
		DataVarId SearchNoVar;
		/*按报警级别*/
		AlarmLevel SearchLevel;		
		DataVarId SearchLevelVar;

		/*按通道*/
		int SearchChannelNo;
		DataVarId SearchChannelNoVar;

		/*按关键字*/
		string SearchKey;
		DataVarId SearchKeyVar;

		/*按数据*/
		double SearchData;
		DataVarId SearchDataVar;

		bool RegMode = false;
		vector<DataVarId> SearchRegVarId;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchGroup, SearchGroupVar,
				SearchNo, SearchNoVar, SearchLevel, SearchLevelVar, SearchChannelNo, 
				SearchChannelNoVar, SearchKey, SearchKeyVar, SearchData, SearchDataVar,
				RegMode, SearchRegVarId);
		}

	};
}



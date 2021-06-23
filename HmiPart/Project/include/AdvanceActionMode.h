#pragma once
#include "JudgeCondition.h"
#include "DataVarInfo.h"
namespace Project
{
	class AdvanceActionMode
	{
	public:
		int Trigmode;						//触发动作模式 0:线圈触发 1:数值逻辑
		int BitTrigCondition;				//线圈触发方式	0：上升沿 1：下降沿  2：边沿触发
		Conditions WordTrigCondition;		//数值逻辑条件
		double WordTrigValue;				//数值逻辑对比值
		DataVarId TrigDatavrIdRef;			//触发地址(位及数值)
		bool Precoilstatus = false;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Trigmode, BitTrigCondition, WordTrigCondition, WordTrigValue, TrigDatavrIdRef);
		}
	};
}


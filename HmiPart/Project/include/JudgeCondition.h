#pragma once
#include <string>
#include "BaseVar.h"
#include "Sysdef.h"
namespace Project
{
	enum Conditions:char
	{
		LT,			//小于
		GT,			//大于
		LE,			//小于等于
		GE,			//大于等于
		EQ,			//相等
		NE			//不等于
	};

	class JudgeCondition
	{
	public:
		static bool MeetCondition(Conditions cond, double src, double dst, XJDataType tp, int decimals = -1);
	};
}


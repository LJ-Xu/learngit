#pragma once
#include <string>
#include "BaseVar.h"
#include "Sysdef.h"
namespace Project
{
	enum Conditions:char
	{
		LT,			//С��
		GT,			//����
		LE,			//С�ڵ���
		GE,			//���ڵ���
		EQ,			//���
		NE			//������
	};

	class JudgeCondition
	{
	public:
		static bool MeetCondition(Conditions cond, double src, double dst, XJDataType tp, int decimals = -1);
	};
}


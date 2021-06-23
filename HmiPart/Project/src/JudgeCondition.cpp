#include "JudgeCondition.h"
#include "math.h"
double eps = 0.0000001;
# define Equ(a,b) (fabs((a)-(b))< eps)		//相等
# define NEq(a,b) (fabs((a)-(b))>= eps)		//不相等

#define More(a,b) (((a)-(b))>(eps))			//大于
#define Less(a,b) (((a)-(b))<(-eps))		//小于
#define MoreEque(a,b) (((a)-(b))>(-eps))	//大于等于
#define LessEque(a,b) (((a)-(b))<(eps))		//小于等于

namespace Project
{
	bool JudgeCondition::MeetCondition(Conditions cond, double src, double dst, XJDataType tp)
	{
		if (tp.Cls == VarDataType::DT_DDWORD && tp.Type == VarNumberType::NT_Float)
			eps = 0.000000000000001;
		else
			eps = 0.0000001;
		switch (cond)
		{
		case LT:			//小于
			if (Less(src, dst))
				return true;
			else
				return false;
		case GT:			//大于
			if (More(src, dst))
				return true;
			else
				return false;
		case LE:			//小于等于
			if (LessEque(src, dst))
				return true;
			else
				return false;
		case GE:			//大于等于
			if (MoreEque(src, dst))
				return true;
			else
				return false;
		case EQ:			//相等
			if (Equ(src, dst))
				return true;
			else
				return false;
		case NE:
			if (NEq(src, dst))
				return true;
			else
				return false;
		default:
			break;
		}
		return false;
	}

}
#include "JudgeCondition.h"
#include "math.h"
#include <sstream>
#include <iomanip>
double eps = 0.0000001;
# define Equ(a,b) (fabs((a)-(b))< eps)		//相等
# define NEq(a,b) (fabs((a)-(b))>= eps)		//不相等

#define More(a,b) (((a)-(b))>(eps))			//大于
#define Less(a,b) (((a)-(b))<(-eps))		//小于
#define MoreEque(a,b) (((a)-(b))>(-eps))	//大于等于
#define LessEque(a,b) (((a)-(b))<(eps))		//小于等于


# define IntEqu(a,b) (fabs((a)-(b))< 0)		//相等
# define IntNEq(a,b) (fabs((a)-(b))>= 0)		//不相等

#define IntMore(a,b) (((a)-(b))>(0))			//大于
#define IntLess(a,b) (((a)-(b))<(-0))		//小于
#define IntMoreEque(a,b) (((a)-(b))>(-0))	//大于等于
#define IntLessEque(a,b) (((a)-(b))<(0))		//小于等于
namespace Project
{
	bool JudgeCondition::MeetCondition(Conditions cond, double src, double dst, XJDataType tp, int decimals)
	{
		if (decimals != -1)			//指定精确位
		{
			std::stringstream srcstr, dststr;
			srcstr << std::setiosflags(std::ios::fixed) << std::setprecision(decimals) << src;
			dststr << std::setiosflags(std::ios::fixed) << std::setprecision(decimals) << dst;
			int result = strcmp(srcstr.str().c_str(), dststr.str().c_str());
			switch (cond)
			{
			case LT:			//小于
				if (result < 0)
					return true;
				else
					return false;
			case GT:			//大于
				if (result > 0)
					return true;
				else
					return false;
			case LE:			//小于等于
				if (result <= 0)
					return true;
				else
					return false;
			case GE:			//大于等于
				if (result >= 0)
					return true;
				else
					return false;
			case EQ:			//相等
				if (result == 0)
					return true;
				else
					return false;
			case NE:
				if (result != 0)
					return true;
				else
					return false;
			default:
				break;
			}
		}
		else		//默认精确位
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
		}
		return false;
	}

}
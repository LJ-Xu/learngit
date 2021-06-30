#include "JudgeCondition.h"
#include "math.h"
#include <sstream>
#include <iomanip>
double eps = 0.0000001;
# define Equ(a,b) (fabs((a)-(b))< eps)		//���
# define NEq(a,b) (fabs((a)-(b))>= eps)		//�����

#define More(a,b) (((a)-(b))>(eps))			//����
#define Less(a,b) (((a)-(b))<(-eps))		//С��
#define MoreEque(a,b) (((a)-(b))>(-eps))	//���ڵ���
#define LessEque(a,b) (((a)-(b))<(eps))		//С�ڵ���


# define IntEqu(a,b) (fabs((a)-(b))< 0)		//���
# define IntNEq(a,b) (fabs((a)-(b))>= 0)		//�����

#define IntMore(a,b) (((a)-(b))>(0))			//����
#define IntLess(a,b) (((a)-(b))<(-0))		//С��
#define IntMoreEque(a,b) (((a)-(b))>(-0))	//���ڵ���
#define IntLessEque(a,b) (((a)-(b))<(0))		//С�ڵ���
namespace Project
{
	bool JudgeCondition::MeetCondition(Conditions cond, double src, double dst, XJDataType tp, int decimals)
	{
		if (decimals != -1)			//ָ����ȷλ
		{
			std::stringstream srcstr, dststr;
			srcstr << std::setiosflags(std::ios::fixed) << std::setprecision(decimals) << src;
			dststr << std::setiosflags(std::ios::fixed) << std::setprecision(decimals) << dst;
			int result = strcmp(srcstr.str().c_str(), dststr.str().c_str());
			switch (cond)
			{
			case LT:			//С��
				if (result < 0)
					return true;
				else
					return false;
			case GT:			//����
				if (result > 0)
					return true;
				else
					return false;
			case LE:			//С�ڵ���
				if (result <= 0)
					return true;
				else
					return false;
			case GE:			//���ڵ���
				if (result >= 0)
					return true;
				else
					return false;
			case EQ:			//���
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
		else		//Ĭ�Ͼ�ȷλ
		{
			if (tp.Cls == VarDataType::DT_DDWORD && tp.Type == VarNumberType::NT_Float)
				eps = 0.000000000000001;
			else
				eps = 0.0000001;
			switch (cond)
			{
			case LT:			//С��
				if (Less(src, dst))
					return true;
				else
					return false;
			case GT:			//����
				if (More(src, dst))
					return true;
				else
					return false;
			case LE:			//С�ڵ���
				if (LessEque(src, dst))
					return true;
				else
					return false;
			case GE:			//���ڵ���
				if (MoreEque(src, dst))
					return true;
				else
					return false;
			case EQ:			//���
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
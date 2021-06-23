#include "JudgeCondition.h"
#include "math.h"
double eps = 0.0000001;
# define Equ(a,b) (fabs((a)-(b))< eps)		//���
# define NEq(a,b) (fabs((a)-(b))>= eps)		//�����

#define More(a,b) (((a)-(b))>(eps))			//����
#define Less(a,b) (((a)-(b))<(-eps))		//С��
#define MoreEque(a,b) (((a)-(b))>(-eps))	//���ڵ���
#define LessEque(a,b) (((a)-(b))<(eps))		//С�ڵ���

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
		return false;
	}

}
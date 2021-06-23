#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"


namespace Project
{
	class BuzzerConfig : public MoveableDisplayUnit
	{
	public:
		// �Ĵ���ģʽ����: 0   λ: 1
		int Reg;
		// ��ȡ�Ĵ���     
		DataVarId TrigVar;
		// ��һ��: 0   ��������: 1
		int Mode;
		int Relation2Value;//0< 1> 2<= 3>= 4== 5!=
		int RelaValue;
		template<class Archive>
		void serialize(Archive & archive) {
			archive(X, Y, Width, Height, CtrlName, Reg, TrigVar, Mode, Relation2Value);
		}
	};
}
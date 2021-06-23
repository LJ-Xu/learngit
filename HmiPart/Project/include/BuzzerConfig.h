#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"


namespace Project
{
	class BuzzerConfig : public MoveableDisplayUnit
	{
	public:
		// 寄存器模式：字: 0   位: 1
		int Reg;
		// 读取寄存器     
		DataVarId TrigVar;
		// 响一声: 0   持续响铃: 1
		int Mode;
		int Relation2Value;//0< 1> 2<= 3>= 4== 5!=
		int RelaValue;
		template<class Archive>
		void serialize(Archive & archive) {
			archive(X, Y, Width, Height, CtrlName, Reg, TrigVar, Mode, Relation2Value);
		}
	};
}
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum VarType
	{
		BitVar,WordVar,SetRandVar, SetFloat, SetString,StateWordVar,StateBitVar,SetChina
	};
	class TestBitVarConfig : public MoveableDisplayUnit
	{
	public:
		VarType Type;		//按键类型
		int Value = 0;					//数值
		bool BitValue = true;
		int IncreaseValue = 0;			//递增数值
		DataVarId VarId;				//绑定的寄存器
		string BGKey;
		string Name;
		int Max;
		int Min;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Type, Value, BitValue, BGKey, IncreaseValue,VarId, Name, Max, Min,
				X, Y, Width, Height, VOffX, CtrlName);
		}
	};

}

#pragma once
#include "CerealCommon.hpp"

class BaseUnit
{
public:
	int X;  //����λ��X
	int Y;  //����λ��Y
	bool IsEnable;
	bool IsRecord = false;
	string CtrlName;
	//virtual BaseUnit*    GetUnit();
	
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(X, Y, IsEnable, IsRecord, CtrlName);
	}
};


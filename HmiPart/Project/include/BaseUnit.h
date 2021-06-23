#pragma once
#include "CerealCommon.hpp"

class BaseUnit
{
public:
	int X;  //坐标位置X
	int Y;  //坐标位置Y
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


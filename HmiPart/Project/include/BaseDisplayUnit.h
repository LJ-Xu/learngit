#pragma once
#include "BaseUnit.h"
#include "CerealCommon.hpp"

class BaseDisplayUnit :
	public BaseUnit
{
public:
	int Width;
	int Height;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(Width, Height);
	}
};


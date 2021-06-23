#pragma once
#include "AdvanceFuncRes.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	class AdvanceFuncConfig :public MoveableDisplayUnit
	{
	public:

		vector<AdvanceFuncRes> Functions;				//¹¦ÄÜ¼¯

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Functions, VOffX, VOffY, Perm, Width, Height, 
				X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

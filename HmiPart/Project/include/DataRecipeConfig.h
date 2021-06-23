#pragma once
#include "MoveableDisplayUnit.h"
#include "RecipeInfoRes.h"

namespace Project
{

	class DataRecipeConfig : public MoveableDisplayUnit
	{
	public:
		map<string, vector<RecipeInfoRes>> TbList;		//≈‰∑Ω±Ì

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(TbList, VOffX, VOffY, Perm, Width,
				Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


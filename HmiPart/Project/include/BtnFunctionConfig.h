#pragma once
#include "BtnUnit.h"
#include "BtnFunctionRes.h"
namespace Project
{
	class BtnFunctionConfig : public BtnUnit
	{
	public:
		vector<BtnFunctionRes> Press;				//°´ÏÂ
		vector<BtnFunctionRes> Release;				//ÊÍ·Å

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Press,  Release, 
				StatusCnt, PicKey, Txt, StrStyles, WriteVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

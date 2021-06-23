#pragma once
#include "MoveableDisplayUnit.h"
#include <vector>
#include "ImageResId.h"
#include "ImageFormat.h"
namespace Project
{
	class GifPicConfig :public MoveableDisplayUnit
	{
	public:
		vector<ImageResId> PicKeys;		//√ø÷°Õº∆¨key÷µ
		vector<int> DelayTime;			//÷°≥§
		ImageFormat PicFormat;
		bool UseFlashing;				// «∑Ò…¡À∏
		double Frequency;				//…¡À∏∆µ¬ 
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PicKeys, DelayTime, PicFormat, UseFlashing, Frequency,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

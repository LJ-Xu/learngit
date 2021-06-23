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
		vector<ImageResId> PicKeys;		//ÿ֡ͼƬkeyֵ
		vector<int> DelayTime;			//֡��
		ImageFormat PicFormat;
		bool UseFlashing;				//�Ƿ���˸
		double Frequency;				//��˸Ƶ��
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PicKeys, DelayTime, PicFormat, UseFlashing, Frequency,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

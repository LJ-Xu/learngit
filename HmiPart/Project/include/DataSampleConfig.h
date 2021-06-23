#pragma once
#include "MoveableDisplayUnit.h"
#include "SampleInfoRes.h"

namespace Project
{

	class DataSampleConfig : public MoveableDisplayUnit
	{
	public:
		vector<SampleInfoRes> InfoLst;			//采样信息列表
		DataVarId SampleNoticeVarId;			//采样通知寄存器

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(InfoLst, SampleNoticeVarId, VOffX, VOffY, Perm,
				Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



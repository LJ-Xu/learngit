#pragma once
#include "MoveableDisplayUnit.h"
#include "SampleInfoRes.h"

namespace Project
{

	class DataSampleConfig : public MoveableDisplayUnit
	{
	public:
		vector<SampleInfoRes> InfoLst;			//������Ϣ�б�
		DataVarId SampleNoticeVarId;			//����֪ͨ�Ĵ���

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(InfoLst, SampleNoticeVarId, VOffX, VOffY, Perm,
				Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



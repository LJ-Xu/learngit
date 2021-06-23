#pragma once
#include "BtnUnit.h"
#include "MoveableDisplayUnit.h"
#include "BtnFunctionRes.h"
#include "AlarmInfoRes.h"
#include <time.h>
using namespace std;
namespace Project
{
	class AlarmInputConfig : public MoveableDisplayUnit
	{
	public:
		vector<AlarmInfoRes> InfoLst;			//����¼����Ϣ�б�
		bool IsSave;//�Ƿ񱣴浽�ļ�
		SaveFileRes SaveLst;					//����¼����Ϣ����
		DataVarId AlarmNoticeVarId;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(InfoLst, SaveLst, AlarmNoticeVarId, VOffX, VOffY,
				Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName, IsSave);
		}
	};
}

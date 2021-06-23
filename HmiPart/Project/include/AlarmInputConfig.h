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
		vector<AlarmInfoRes> InfoLst;			//报警录入信息列表
		bool IsSave;//是否保存到文件
		SaveFileRes SaveLst;					//报警录入信息保存
		DataVarId AlarmNoticeVarId;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(InfoLst, SaveLst, AlarmNoticeVarId, VOffX, VOffY,
				Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName, IsSave);
		}
	};
}

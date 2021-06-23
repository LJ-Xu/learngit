#pragma once
#include "MoveableDisplayUnit.h"
#include "BtnUnit.h"
#include "KeypadConfig.h"
namespace Project
{
	class GBtnPilotLampConfig : public BtnUnit
	{
	public:
		bool RwDiffAddrUsed;
		BtnAction Action;		// 置ON  置OFF  取反  瞬时ON
		int Flash;				//是否闪烁	0:不闪烁
								//			1:ON状态闪烁
								//			2:OFF状态闪烁
		int Logic;				//线圈状态正负逻辑 0:正逻辑 1:负逻辑
		float Speed;				//闪烁速度(ms)
		bool UseSameAddr;		//读写使用同地址
		DataVarId ReadVar;		//读取地址状态

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, Action, Flash, Logic, Speed, UseSameAddr,
				ReadVar, StatusCnt, PicKey, Txt, StrStyles, WriteVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		/******************************************************/
		int CurrentFlash = 1;			//当前闪烁状态	0:隐藏
										//				1:图片
		//int CurrentStatus = 0;			//存放当前状态(1:ON/0:OFF状态)
		bool HasTimer;					//是否已经在闪烁状态下
	};
}



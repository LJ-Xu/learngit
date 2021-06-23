#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "StatusRes.h"
#include "ImageResId.h"
#ifdef __linux
#ifdef Status
#undef Status
#endif
#endif
namespace Project
{
	class PilotLampConfig : public MoveableDisplayUnit
	{
	public:
		StatusRes Text;					//文字内容
		vector<StringStyle> Csses;		//文字样式
		DataVarId Status;				//当前状态
		int Flash;						//是否闪烁	0:不闪烁
										//			1:ON状态闪烁
										//			2:OFF状态闪烁
		int Logic;						//线圈状态正负逻辑 1:正逻辑 0:负逻辑
		float Speed;						//闪烁速度(快闪慢闪)
		vector<ImageResId> StatusKeys;		//图片资源
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Csses, Status, Flash, Logic, Speed, StatusKeys,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
		/******************************************************/
		int CurrentFlash = 1;			//当前闪烁状态	0:隐藏
										//				1:图片
		int CurrentStatus = 0;			//存放当前状态(1:ON/0:OFF状态)
		bool HasTimer;					//是否已经在闪烁状态下
	};

}


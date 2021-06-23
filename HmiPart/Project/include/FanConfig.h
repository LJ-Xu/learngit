#pragma once
#include "MoveableDisplayUnit.h"
#include <string>
#include "BaseLine.h"
#include "ImageResId.h"
#include "MultiPilotLampConfig.h"

namespace Project
{
	class FanConfig : public MoveableDisplayUnit
	{
	public:
		int ActionMode;						//动作模式 0:一直旋转 1:受寄存器控制
		int RegMode;						//寄存器模式 0:字 1:位
		DataVarId ActTrigVar;				//动作模式寄存器地址
		int RotateDir;						//旋转方向 0:正向 1:反向
		double CycleTime;					//切换图片的速度 最快100ms，最慢1000ms
		DataVarId CycleTimeVarId;			//切换图片的速度（寄存器指定）
		vector<ImageResId>PicKeys;			//图片集合
		int BitTrigCondition;				//位触发方式	0：ON 1：OFF 
		Conditions WordTrigCondition;		//数值逻辑条件
		double WordTrigValue;				//数值逻辑对比值
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ActionMode, RegMode, ActTrigVar, RotateDir, CycleTime,
				CycleTimeVarId, PicKeys, BitTrigCondition, WordTrigCondition, WordTrigValue,
				VOffX, VOffY, Width, Height, Perm, X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}
	};
}



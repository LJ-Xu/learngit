/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 搅拌器控件参数类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include <vector>

namespace Project 
{
	class WaterPumpConfig : public MoveableDisplayUnit
	{
	public:
		WaterPumpConfig() { }
		~WaterPumpConfig() { }

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVar, Logic, Dir, Speed, SpVar, Keys,
				VOffX, VOffY, Perm, Width, Height, X, Y, 
				IsEnable, CtrlName, IsRecord, CtrlName, InnerRectX, InnerRectY, InnerRectW, InnerRectH);
		}

	public:
		DataVarId ReadVar;	// 读取寄存器
		int Logic = 0;		// 搅拌器逻辑
		int Dir = 0;		// 搅拌器方向
		int Speed = -1;		// 流动速度
		DataVarId SpVar;	// 流速寄存器
		int InnerRectX;
		int InnerRectY;
		int InnerRectW;
		int InnerRectH;
		vector<ImageResId> Keys;
		int CurrentStatus = 0;
	};
}


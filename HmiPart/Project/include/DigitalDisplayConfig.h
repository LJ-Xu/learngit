/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放数值显示框参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "MacroRes.h"
namespace Project
{
	class DigitalDisplayConfig : public InputNumShowUnit
	{
	public:
		
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PswMode, IsFillZero, Accuracy, IsWarnUpperUsed, WarnUpper, 
				IsWarnLowerUsed, WarnLower, IsWarnStyleUsed, WarnStyleUpper, WarnStyleLower,
				IsConvertUsed, NovUpperSrc, NovLowerSrc, NovUpperRes, NovLowerRes,
				ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}


/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterDisplayConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 字符显示框用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputDataShowUnit.h"
#include "MacroRes.h"
namespace Project
{
	class CharacterDisplayConfig : public InputDataShowUnit
	{
	public:
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PswMode, ByteConversion, Rule, ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}



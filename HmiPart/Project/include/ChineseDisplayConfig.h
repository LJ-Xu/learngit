/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseDisplayConfig.h
 * Author   : qiaodan
 * Date     : 2020-10-15
 * Descript : 中文显示框用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "InputDataShowUnit.h"
namespace Project
{
	class ChineseDisplayConfig : public MoveableDisplayUnit
	{
	public:
		StringStyle TextStyle;
		ImageResId PicKey;
		DataVarId ReadVar;
		string Text;
		UI::CodeFormatUtility::CodeRule Rule;	// 0: ASCII 1: UTF_8 2: UTF_16
		int PswMode;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PicKey, TextStyle, ReadVar, Text, Rule, PswMode,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

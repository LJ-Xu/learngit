/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageConfig.h
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : 拼音界面用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "MacroRes.h"
#include "StringStyle.h"
namespace Project
{
	class PinYinPageConfig : public MoveableDisplayUnit
	{
	public:
		DataVarId ReadVar;			//用于读取拼音的寄存器(SYS_PSW_PINYIN_CURVAL)
		DataVarId WriteVar;			//写入中文字符串(SYS_PSW_INPUTKEY_CURVAL)
		StringStyle TextStyle;		//字体样式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, WriteVar, TextStyle,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}


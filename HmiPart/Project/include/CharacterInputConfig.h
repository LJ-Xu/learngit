/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放字符输入框控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputDataShowUnit.h"
#include "MacroRes.h"
#include "NoticesAction.h"
#include "InputNumShowUnit.h"

namespace Project
{
	class CharacterInputConfig : public InputDataShowUnit
	{
	public:
		DataVarId WriteVar;					//写入寄存器
		InputSequence Sequence;				//输入次序
		map<int, vector<WinCtrlID>> SequenceGroup; 
		InputMethod KeyMethod;				//键盘输入
		NoticesAction ResBef;				//写入前通知
		NoticesAction ResAft;				//写入后通知

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, Sequence, KeyMethod, ResBef, ResAft, SequenceGroup,
				PswMode, ByteConversion, Rule, ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};

}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放数值输入框相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "MacroRes.h"
#include "NoticesAction.h"
namespace Project
{
	class DigitalInputConfig : public InputNumShowUnit
	{
	public:
		bool RwDiffAddrUsed;				//读写使用相同寄存器
		DataVarId WriteVar;					//写入寄存器
		InputSequence Sequence;				//输入次序
		InputMethod KeyMethod;				//输入键盘
		map<int, vector<WinCtrlID>> SequenceGroup/* =
		{
			{1,{65536,65537,65538}}
		}*/;//输入次序组
		bool IsInputConvertUsed;			//是否使用输入比例转换
		DataVarOrManual NovUpperSrcInput;	//比例转换数据输入源上限
		DataVarOrManual NovLowerSrcInput;	//比例转换数据输入源下限
		DataVarOrManual NovUpperResInput;	//比例转换输入转换值上限
		DataVarOrManual NovLowerResInput;	//比例转换输入转换值下限

		NoticesAction ResBef;				//写入前通知
		NoticesAction ResAft;				//写入后通知
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, WriteVar, Sequence, KeyMethod, IsInputConvertUsed,
				NovUpperSrcInput, NovLowerSrcInput, NovUpperResInput, NovLowerResInput, ResBef, ResAft,
				PswMode, IsFillZero, Accuracy, IsWarnUpperUsed, WarnUpper, SequenceGroup,
				IsWarnLowerUsed, WarnLower, IsWarnStyleUsed, WarnStyleUpper, WarnStyleLower,
				IsConvertUsed, NovUpperSrc, NovLowerSrc, NovUpperRes, NovLowerRes,
				ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}
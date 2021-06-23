/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : InputDataUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 输入框用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ReadableDisplayUnit.h"
#include "InputDataShowUnit.h"
#include "InputNumShowUnit.h"
#include "NoticesAction.h"
#include "MacroRes.h"

namespace Project
{
	class InputDataUnit	: public InputDataShowUnit
	{
	public:
		bool RwDiffAddrUsed;				//读写使用相同寄存器
		DataVarId WriteVar;					//写入寄存器
		InputSequence Sequence;				//输入次序
		InputMethod KeyMethod;				//键盘输入
		NoticesAction ResBef;				//写入前通知
		NoticesAction ResAft;				//写入后通知

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, WriteVar, Sequence, KeyMethod, ResBef, ResAft);
		}
	};
}

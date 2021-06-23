/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : 用于存放中文输入框控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "InputDataShowUnit.h"
#include "NoticesAction.h"
namespace Project
{
	class ChineseInputConfig : public MoveableDisplayUnit
	{
	public:
		DataVarId WriteVar;										//写寄存器
		int PswMode;											//密码样式
		InputMethod KeyMethod;									//键盘输入
		NoticesAction ResBef;									//写入前通知
		NoticesAction ResAft;									//写入后通知
		UI::CodeFormatUtility::CodeRule Rule;					//编码格式
		ImageResId Key;											//获取图片的key值
		StringStyle TextStyle;									//字体样式
		string Text;											//初始文字串
		InputSequence Sequence;									//输入次序
		map<int, vector<WinCtrlID>> SequenceGroup;				//输入次序组

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, PswMode, KeyMethod, Sequence, SequenceGroup,
				ResBef, ResAft, Rule, Key, TextStyle, Text, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};
}
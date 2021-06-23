/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : PinYinPageConfig.h
 * Author   : qiaodan
 * Date     : 2021-01-21
 * Descript : ƴ���������ڴ�Ż��Ʋ�������
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
		DataVarId ReadVar;			//���ڶ�ȡƴ���ļĴ���(SYS_PSW_PINYIN_CURVAL)
		DataVarId WriteVar;			//д�������ַ���(SYS_PSW_INPUTKEY_CURVAL)
		StringStyle TextStyle;		//������ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, WriteVar, TextStyle,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}


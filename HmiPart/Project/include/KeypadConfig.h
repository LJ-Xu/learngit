/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeypadConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 按键用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BtnUnit.h"
namespace Project
{
	enum BtnAction :char { BtnSETON, BtnSETOFF, BtnOPPOSITE, BtnSHORTON };
	class KeypadConfig : public BtnUnit
	{
	public:
		BtnAction Action;					//按键动作
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Action, StatusCnt, PicKey, Txt, StrStyles, WriteVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};
}
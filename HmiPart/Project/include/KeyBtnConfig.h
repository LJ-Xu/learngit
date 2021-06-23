/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyBtnConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 键盘按键用于存放绘制参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "BtnUnit.h"
namespace Project
{
	class KeyBtnConfig : public BtnUnit
	{
	public:
		int AsciiNum;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(AsciiNum, StatusCnt, PicKey, CurrentStatus,Txt, StrStyles, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};
}





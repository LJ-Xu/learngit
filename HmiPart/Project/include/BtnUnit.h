/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "StatusRes.h"
#include "StringStyle.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
namespace Project
{
	class BtnUnit : public MoveableDisplayUnit
	{
	public:
		int StatusCnt;						//状态个数
		vector<ImageResId> PicKey;			//状态对应外观
		StatusRes Txt;						//对应文字
		vector<StringStyle> StrStyles;		//文字样式
		DataVarId WriteVar;					//写入寄存器

		int CurrentStatus = 0;				//存放当前状态(1:ON/0:OFF状态)
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StatusCnt, PicKey, Txt, StrStyles, WriteVar);
		}
	};
}

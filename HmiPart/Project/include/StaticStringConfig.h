/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放静态文字控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "BorderRes.h"
#include "BaseLine.h"
#include "Point.h"
namespace Project
{
	class StaticStringConfig : public MoveableDisplayUnit
	{
	public:
		StatusRes Text;
		StringStyle Css;
		//int alpha;      //透明度
		BorderRes Border;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Css, Border, VOffX, VOffY, Perm, Width,
				Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

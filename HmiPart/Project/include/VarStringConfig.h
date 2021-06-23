/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放动态文字控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
//#include "VarStringRes.h"
#include "StringStyle.h"
//#include "BorderRes.h"
#include "BaseLine.h"
#include "StatusRes.h"
namespace Project
{
	struct VarStringStyle
	{
		double SrcId;
		StatusRes Text;			//文字
		StringStyle Css;		//文字样式
		BaseLine BorderStyle;	//边框样式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SrcId, Text, Css, BorderStyle);
		}
	};
	class VarStringConfig : public MoveableDisplayUnit
	{
	public:
		vector<VarStringStyle> StatusStyle;		//每个状态的样式
		int Count;								//状态数
		DataVarId SrcIdRef;						//指定状态的寄存器

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StatusStyle, Count, SrcIdRef,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



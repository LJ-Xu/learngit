/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 阀门控件参数类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once

#include "ImageResId.h"
#include "BtnUnit.h"
#include "KeypadConfig.h"
#define TEMPRWDIF 1
namespace Project 
{
	class ValveConfig : public BtnUnit
	{
	public:
		ValveConfig() { }
		~ValveConfig() { }

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVar, WriteVar, Act, Logic, Dir, Speed, SpVar, PicKey,
					VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable,InnerRectX,InnerRectY,InnerRectW,InnerRectH,IsRecord,CtrlName
//#if TEMPRWDIF
//#else
//				, IsRWDiff
//#endif
			);
		}

	public:
//#if TEMPRWDIF
//#else
//		bool IsRWDiff;
//#endif
		DataVarId ReadVar;		// 读取寄存器
		DataVarId WriteVar;		// 写入寄存器
		BtnAction Act = BtnSETON;	// 阀门动作 0:导通	1:关闭	2:导通/关闭(取反)	3:按压导通
		int Logic = 0;			// 阀门逻辑 0:正逻辑	1:负逻辑
		int Dir = 0;			// 流动方向 0:正向	1:反向
		int Speed = -1;			// 流动速度
		DataVarId SpVar;		// 流速寄存器
		int InnerRectX;
		int InnerRectY;
		int InnerRectW;
		int InnerRectH;
	};
}

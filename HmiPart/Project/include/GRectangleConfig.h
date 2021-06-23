/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放矩形控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
namespace Project
{
	class GRectangleConfig : public MoveableDisplayUnit
	{
	public:
		Point Pos; //圆角心位置
		int Angle; //角度
		int Radius; //圆角直径
		bool IsFill; //是否填充
		int  FillColor; //填充颜色
		int  FillStyle; //填充样式 可以填充 图案
		BaseLine Line; //线
		ImageResId BgPicKey;

		int RectWidth;
		int RectHeight;
		double RotateAngle;			//旋转角度
		Point RotateCenter;			//旋转中心
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Pos, Angle, Radius, IsFill, FillColor, FillStyle, Line, BgPicKey, RectWidth, RectHeight,
				RotateAngle, RotateCenter, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放圆绘制相关内容的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include "GArcConfig.h"
namespace Project
{
	class GCircleConfig : public MoveableDisplayUnit
	{
	public:
		bool IsFill;			//是否填充
		bool IsStandard;		//是否标准圆

		int  FillColor;			//填充颜色
		int  FillStyle;			//填充样式 可以填充 图案
		int  ColorFillinStyle;
		bool UseFillinStyle;
		BaseLine Line; //线

		ImageResId BgPicKey;
        Project::ArcRes ArcRes;				//椭圆信息

		double RotateAngle;			//旋转角度
		Point RotateCenter;			//旋转中心
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ArcRes, IsFill, IsStandard, FillColor, FillStyle, 
				ColorFillinStyle, BgPicKey, RotateAngle,
				RotateCenter, UseFillinStyle, Line, VOffX, VOffY, Perm,
				Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放圆弧绘制参数的结构体
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "GLineConfig.h"
#include "CerealCommon.hpp"
#include "ImageResId.h"
namespace Project
{
	class ArcRes
	{
	public:
		int A;	//长边长度
		int B;	//短边长度
		Point Center;	//圆心坐标


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(A, B, Center);
		}

	};
	class GArcConfig : public MoveableDisplayUnit
	{
	public:
		bool IsFill;				//是否填充

		double StartAngle;			//起始角度
		double EndAngle;			//结束角度

		bool IsSector;				//是否是扇形
		int  FillColor;				//填充颜色

		int  FillStyle;				//填充样式 可以填充 图案
        Project::ArcRes ArcRes;				//椭圆信息
		vector<Point> Coordinate;	//圆弧坐标
		BaseLine Line;				//线

		ImageResId BgPicKey;		//背景填充

		double RotateAngle;			//旋转角度
		Point RotateCenter;			//旋转中心
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsFill, StartAngle, EndAngle, IsSector, FillColor,
				FillStyle, ArcRes, Coordinate, Line, VOffX, VOffY, BgPicKey,
				RotateAngle, RotateCenter, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


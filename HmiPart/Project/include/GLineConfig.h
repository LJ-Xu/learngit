/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 用于存放直线绘制相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#ifdef WIN32
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "CerealCommon.hpp"
#else
#include "BaseLine.h"
#include "Point.h"
#include "MoveableDisplayUnit.h"
#include "CerealCommon.hpp"
#endif // WIN32

namespace Project
{
	enum ArrowType { TriangleArrow, Arrow, SolidArrow, RectArrow, DotArrow };
	class BaseArrow
	{
	public:
		bool HaveStartArrow;	//起始是否有箭头
		ArrowType StartArrowType;		//起始箭头类型
		int StartArrowSize;		//起始箭头尺寸

		bool HaveEndArrow;		//结束是否有箭头
		ArrowType EndArrowType;		//结束箭头类型
		int EndArrowSize;		//结束箭头尺寸

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(HaveStartArrow, StartArrowType, StartArrowSize, 
				HaveEndArrow, EndArrowType, EndArrowSize);
		}
	};

	class GLineConfig : public MoveableDisplayUnit
	{
	public:
		BaseLine Line;				//线特性
		vector<Point> Coordinate;	//线段起始终止坐标
		BaseArrow Arrow;			//箭头特性
		double RotateAngle;			//旋转角度
		Point RotateCenter;			//旋转中心
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Line, Coordinate, RotateAngle, RotateCenter,
				Arrow, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

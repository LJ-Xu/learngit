/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncConfig.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件参数类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "Color.h"

namespace Project
{
	enum DrawFunc {
		DCMapClear = 0,
		DCMapSetBackColor,
		DCMapDrawLine,
		DCMapDrawRect,
		DCMapDrawCircle,
		DCMapDrawEllipse,
		DCMapDrawCircleArc,
		DCMapDrawEllipseArc
	};

	class GFuncConfig : public MoveableDisplayUnit
	{
	public:
		GFuncConfig(){ }
		~GFuncConfig(){ }

		template<class Archive>
		void serialize(Archive & archive) {
			archive(FuncId, VOffX, VOffY, Perm, Width, Height,
				X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}

		void Init() {
			MapId = 0;
			GX = 0;
			GY = 0;
			GW = 0;
			GH = 0;
			LineWidth = 0;
			LineColor = Project::Color(255, 255, 255);
			FillRect = false;
			FillColor = Project::Color(255, 255, 255);
			BackColor = Project::Color(255, 255, 255);
			StartAngle = 0;
			EndAngle = 0;
		}

	public:
		int FuncId;		// 函数对用编号
		int MapId;		// ID
		int GX;			// 起始x坐标(圆心横坐标)
		int GY;			// 起始y坐标(圆心纵坐标)
		int GW;			// 宽(半径)(x轴半径)
		int GH;			// 高(半径)(y轴半径)
		int LineWidth;	// 线条粗细
		int StartAngle;	// 起始角度
		int EndAngle;	// 终点角度
		bool FillRect;	// 是否填充
		Color LineColor;// 线条颜色
		Color FillColor;// 填充颜色
		Color BackColor;// 背景颜色
	};
}


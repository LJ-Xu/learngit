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
			archive(MapId,VOffX, VOffY, Perm, Width, Height,
				X, Y, IsEnable, CtrlName, IsRecord, CtrlName, BackColor);
		}

		void Init() {
			MapId = 0;
		}

	public:


		int MapId;		// ID
		Color BackColor;// 背景颜色
	};
}


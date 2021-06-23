/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : 饼图控件参数类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "Color.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

namespace Project 
{
	class PieChartChannelStyle {
	public:
		float Val = 0.0F;	// 通道文字内容
		float Percent = 0.0F;
		string text;
		Color FontColor;	// 通道文字颜色
		Color BackColor;	// 通道背景颜色
		int StartAngle;		// 起始角度
		int EndAngle;		// 终止角度
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Val, Percent, text, FontColor, BackColor, StartAngle, EndAngle);
		}
	};

	class PieChartConfig : public MoveableDisplayUnit
	{
	public:
		PieChartConfig() {
			CenterColor = Color(255, 255, 255);	// 圆心内部颜色
			CenterEdgeColor = Color(255, 255, 255);	// 圆心边框颜色
			BorderColor = Color(255, 255, 255);	// 饼图边框颜色
		}
		~PieChartConfig() {}

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVars, ChannelNum, Dir, StartAngle, EndAngle, Alpha,
					CenterRadius, CenterColor, CenterEdgeColor, BorderColor, 
					Channels, Mode, Bct, TextStyle,
					VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}

	public:
		vector<DataVarId> ReadVars;			// 读取寄存器
		int ChannelNum = 4;			// 通道数
		int Dir = 0;				// 方向	0:顺时针 1:逆时针
		int StartAngle = 0;			// 起始角度
		int EndAngle = 360;			// 终止角度
		int Alpha = 255;			// 透明度
		float CenterRadius = 0;		// 内部圆心半径
		Color CenterColor;			// 圆心内部颜色
		Color CenterEdgeColor;		// 圆心边框颜色		
		Color BorderColor;			// 饼图边框颜色	
		vector<PieChartChannelStyle> Channels;
		int Mode = 1;				// 显示样式	0:不显示 1:显示数字 2:显示百分比
		int Bct = 0;				// 小数位数
		StringStyle TextStyle;		// 字体样式
		float TotalVal = 0;
	};
}


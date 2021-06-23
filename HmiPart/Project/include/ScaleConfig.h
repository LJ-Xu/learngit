#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class ScaleConfig : public MoveableDisplayUnit
	{
	public:
		enum ScaleType
		{
			Horizontal,//水平的Horizontal
			Vertical,//垂直的Vertical
			UpperSemicircle,//上半圆
			LowerSemicircle,//下半圆
			Circular,//圆
			UDCir//自定义圆
		};

		ScaleType ScaleTp;// 刻度的类型
		int LineColor;// 刻度线条的颜色，默认黑色
		int LineStyle;// 刻度线条样式
		int LineWidth;// 刻度线条宽度
		int MainGraduateCount;// 主刻度分割数
		int MajorScaleLineLength;// 主刻度线长度
		bool UseSecondaryScale;// 是否使用副刻度标志
		int SecondaryScaleCutCount;// 副刻度分割数
		int SecondaryScaleLength;// 副刻度长度
		bool IsDisplayMark;// 是否显示标记
		int MarkIntegerNumber;// 标记的整数位数
		int MarkDecimalNumber;// 标记的小数位数
		StringStyle MarkFontSytle;// 标签的字体样式
		int MarkMaxValue;// 刻度标签上限值
		bool IsMarkMaxValueUseRegister;// 标记上限值是否使用寄存器，默认否
		DataVarId MarkMaxValueVarIdRef;// 标记最大值读取寄存器
		int MarkMinValue;// 标签的下限值
		bool IsMarkMinValueUseRegister;// 标签下限是否使用寄存器
		DataVarId MarkMinValueVarIdRef;// 标签最小值使用寄存器
		bool IsReserveMark;// 是否翻转刻度标记,暂时认为↑、→、逆时针为默认数字增长方向,即IsReserveMark=false时
		// 标记的位置
		// 刻度类型为垂直时：0：左侧 1：右侧 2：居中
		// 水平时：0：上 1：下 2：居中
		// 圆： 0：内 1：外 2：居中
		int MarkPlace;
		int CircularStartAngle;
		int CircularEndAngle;
		bool IsDisplayAxis;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, Width, Height,VOffX,VOffY,Perm, CtrlName, ScaleTp, LineColor, LineStyle, LineWidth, MainGraduateCount, MajorScaleLineLength, UseSecondaryScale, SecondaryScaleCutCount, SecondaryScaleLength,IsDisplayMark, MarkIntegerNumber, MarkDecimalNumber, MarkFontSytle, MarkMaxValue, IsMarkMaxValueUseRegister, MarkMaxValueVarIdRef, MarkMinValue, IsMarkMinValueUseRegister, MarkMinValueVarIdRef, IsReserveMark,MarkPlace, IsDisplayAxis);
		}
	};
}
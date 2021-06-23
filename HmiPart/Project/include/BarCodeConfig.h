#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"

namespace Project
{
	class BarCodeConfig :public MoveableDisplayUnit
	{
	public:
		enum TwoDimensionCodeType
		{
			QRCode,
			DataMatrix,
			PDF417,
		};
		enum OneDimensionCodeType
		{
			EAN13=1,
			Code39,
			Code128=0,
		};
		int Dimension;// 条码的维度 0是一维，1是二维
		TwoDimensionCodeType TwoDimensionType;// 当条码是二维时条码的种类
		OneDimensionCodeType OneDimensionType;// 当条码是一维时条码的种类。
		int EncodingRule;// 编码规则 0.ascii 1.unicode
		int CorrectErrorLevel;// 校正标准，仅当种类是QRCode时有效。0：L级，1：M级 ，2：Q级， 3:H级
		bool UseConstValue;// 是否使用固定值 true是 false 否
		std::string ConstString;// 使用固定值时的字符串
		DataVarId ReadVar;// 使用寄存器指定时的寄存器
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, Dimension, TwoDimensionType, OneDimensionType, EncodingRule, CorrectErrorLevel, UseConstValue, ConstString, ReadVar);
		}
	};
}
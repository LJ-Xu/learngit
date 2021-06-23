#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "GLineConfig.h"
#include "ImageResId.h"
namespace Project
{
	class GPolyLineConfig : public MoveableDisplayUnit
	{
	public:
		vector<Point> Points;		//坐标数组
		int Mode;					//0:折线 1:多边形
		BaseLine LineStyle;			//线
		BaseArrow Arrow;			//箭头特性
		bool IsFill;				//是否填充
		int  FillColor;				//填充颜色
		int  FillStyle;				//填充样式 可以填充 图案
		ImageResId BgPicKey;
		double RotateAngle;			//旋转角度
		Point RotateCenter;			//旋转中心
		bool UseLine;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Points, Mode, LineStyle, Arrow, IsFill, FillColor, FillStyle, BgPicKey, UseLine,
				RotateAngle, RotateCenter, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};


}

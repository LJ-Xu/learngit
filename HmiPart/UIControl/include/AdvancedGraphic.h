#pragma once
#include "stdafx.h"
#include "BasicView.h"
#include "StringStyle.h"

namespace UI
{
	class AdvancedGarphic : public Fl_Widget
	{
	public:
		//获取点所在的象限
		static int GetQuaofpoint(int px, int py);
		//获取圆弧所在区间
		static void GetArcField(double startAngle, double endAngle, int &containquardences, int& existquardences);
	public:
		//画扇形
		static void DrawSector(double startAngle,double endAngle, unsigned long fillRadius, unsigned long hollowRadius,int red,int green,int blue,int alpha=255);
		//绘制直条刻度
		//isHorizontal:true为水平排列，否则垂直排列
		//alignMode:对齐方式，0为左/上对齐，1为右/下对齐
		//dx dy:绘制起始点
		//len:区间长度
		//mcount scount:主刻度数 副刻度数
		//mlen slen:主刻度长度 副刻度长度
		static void DrawScale(bool isHorizontal, int alignMode,int dx,int dy,int len,int mcount,int scount,int mlen,int slen,int style, int color, int widget);
		//绘制扇形刻度
		//static void DrawScale();

		//绘制扇形刻度对应的数字标签
		//x、y:坐标系中心点坐标
		//fontStyle:字符标签的字体
		//radius:字符标签绘制到中心点的距离半径
		//count:分割的字符标签数
		//minvalue、maxvalue:字符标签上下限
		//startAngle、sweepAngle:起始角度、经过角度（sweepAngle为负时逆时针转）
		//stringFormat:字符格式
		static void DrawScaleMark(int x, int y, const Project::StringStyle &fontStyle, int radius, int count, float minvalue, float maxvalue, double startAngle, double sweepAngle, char* stringFormat,int maxDgtLen, bool ShowPercent = false, int active = 1);
		//绘制直条刻度对应的数字标签
		//direction:方向：0上 1下 2左 3右
		//x:绘制起始坐标
		//y:
		//len:延伸方向的长度
		//fontStyle:字体
		//count:标签分割数
		//minvalue:最小值
		//maxvalue:最大值
		//stringFormat:
		//intDgt:
		//decDgt:
		static void DrawScaleMark(int direction, int x, int y,int len, const Project::StringStyle &fontStyle, int count,float minvalue, float maxvalue, char* stringFormat,int MaxDgtLen,int active=1);
	};
}

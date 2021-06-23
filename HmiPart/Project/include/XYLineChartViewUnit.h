#pragma once
#include "TrendViewUnit.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	struct ScaleMark
	{
		double UpperLimit;				//上限
		DataVarId UpperLimitVar;		//上限寄存器
		double LowerLimit;				//下限
		DataVarId LowerLimitVar;		//下限寄存器
		int ScaleColor;					//刻度颜色
		int PrimaryScaleNum;			//主刻度数
		int PrimaryScaleLen;			//主刻度长度
		bool UseSecondaryScaleNum;		//是否使用副刻度
		int SecondaryScaleNum = 1;		//副刻度数
		int SecondaryScaleLen;			//副刻度长度
		ScaleMarkType MarkType;			//数字刻度样式 0:不使用 1:数值 2:百分比
		string ScaleMarkFont;			//标注字体
		int ScaleMarkSize;				//标注字体大小
		int ScaleMarkColor;				//标注颜色
		int IntegerNum;					//整数位数
		int DecimalnNum;				//小数位数

		/*特殊变量*/
		int ScaleType;					//刻度样式	0：时间 1：数字
		bool IsUseTimeScale;			//是否使用时间刻度
		bool IsdiaplayDate;				//显示日期
		DateMode DateStyle;				//日期格式
		bool IsdisplayTime;				//显示时间
		TimeMode TimeStyle;				//时间格式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(UpperLimit, UpperLimitVar, LowerLimit, LowerLimitVar, ScaleColor,
				PrimaryScaleNum, PrimaryScaleLen, SecondaryScaleNum, SecondaryScaleLen,
				MarkType, ScaleMarkFont, ScaleMarkSize, ScaleMarkColor, IntegerNum, DecimalnNum);
		}
	};

	struct ReferenceLine
	{
		int LineColor;					//线颜色
		double LineValue;					//线数值
		DataVarId LineValueVar;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(LineColor, LineValue, LineValueVar);
		}

	};
	class XYLineChartViewUnit : public MoveableDisplayUnit
	{
	public:
		int LineChartBackColor;			//背景颜色
		int ScaleAreaBackColor;			//刻度区背景颜色
		bool UseGrid;					//是否使用栅格显示
		int XGridEqualNum;				//X轴栅格等分数
		int YGridEqualNum;				//Y轴栅格等分数
		BaseLine GridStyle;				//栅格线条样式

		Point OriginPos;				//原点坐标
		int XaxisLen;					//X轴长
		int YaxisLen;					//Y轴长
		int ScrollHeight = 0;			//滚动条高度
		bool UseYScale;
		bool UseXScale;
		ScaleMark XScaleSet;			//X轴标注设置
		ScaleMark YScaleSet;			//Y轴标注设置
		vector<ScaleInfo> XScaleInfo;	//标注位置
		vector<ScaleInfo> YScaleInfo;	//标注位置

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(LineChartBackColor, ScaleAreaBackColor, UseGrid,
				XGridEqualNum, YGridEqualNum, GridStyle, OriginPos, XaxisLen, YaxisLen, ScrollHeight,
				UseYScale, UseXScale, XScaleSet, YScaleSet, XScaleInfo, YScaleInfo);
		}
	};


}
#pragma once
#include "XYLineChartViewUnit.h"
#include "XYLineChannel.h"
namespace Project
{
	enum FullSampleAct {StopSample, CleanSample, CoverSample};
	struct ReferencePolyLines
	{
		int LineColor;
		int LineMode;		//0:折线 1:点
		vector<Point> Pts;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(LineColor, LineMode, Pts);
		}
	};
	class XYTrendConfig : public XYLineChartViewUnit
	{
	public:
		int RefreshMode;					//刷新模式	0：周期式 1：触发式
		int SamplingCycle;					//刷新周期(ms)
		DataVarId TriggerTypeVar;			//触发地址
		int TriggerType;					//触发模式 0:位	1:字
		Conditions ConditionWord;			//字触发条件
		double TriggerData;					//触发判断数据
		DataVarId TriggerDataVar;			//触发对比数据地址
		int  ConditionBit;					//位触发条件	0：ON->OFF 1：OFF->ON
		bool UseZoom;						//使用缩放
		int ZoomPercent;					//缩放百分比
		int PageControlStyle;				//翻页模式 0：滚动条 1：寄存器
		DataVarId TurnControlVar;			//翻页寄存器
		int SamplePoints;					//采样点数
		DataVarId SamplePointsVar;			//采样点数寄存器
		int PointsPerScreen;				//每屏采样点数
		int ChannelNum;						//通道数量
		bool UseXScaleTime;					//X轴使用时间
		vector<XYLineChannel> DataChanel;	//通道设置
		FullSampleAct HandleSampleMode;		//采满处理方式

		int ReferenceLineNum;
		vector<ReferencePolyLines>	ReferenceLines;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RefreshMode, SamplingCycle, TriggerTypeVar, TriggerType, ConditionWord, TriggerData,
				ConditionBit, UseZoom, ZoomPercent, PageControlStyle, TurnControlVar, HandleSampleMode,
				SamplePoints, SamplePointsVar, PointsPerScreen, ChannelNum, UseXScaleTime, DataChanel,
				ReferenceLineNum, ReferenceLines, LineChartBackColor, ScaleAreaBackColor, UseGrid,
				XGridEqualNum, YGridEqualNum, GridStyle, OriginPos, XaxisLen, YaxisLen, ScrollHeight,
				UseYScale, UseXScale, XScaleSet, YScaleSet, XScaleInfo, YScaleInfo,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


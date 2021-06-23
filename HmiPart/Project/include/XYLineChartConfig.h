#pragma once
#include "XYLineChartViewUnit.h"
#include "XYLineChannel.h"
namespace Project
{
	class XYLineChartConfig : public XYLineChartViewUnit
	{
	public:
		int RefreshMode;			//刷新模式	0：周期式 1：触发式
		int SamplingCycle;			//刷新周期(ms)
		DataVarId TriggerTypeVar;	//触发地址
		int TriggerCond;			//触发条件	0：上升沿 1：下降沿
		DataVarId PauseControlVar;		//暂停控制
		DataVarId ClearControlVar;		//清除控制
		int PauseControlCondition;		//0：ON，1：OFF
		int ClearControlCondition;		//0：上升沿 1：下降沿
		bool UseZoom;					//使用缩放
		int ZoomPercent;				//缩放百分比
		int PageControlStyle;				//翻页模式 0：滚动条 1：寄存器
		DataVarId TurnControlVar;			//翻页寄存器
		int SamplePoints;					//采样点数
		DataVarId SamplePointsVar;			//采样点数寄存器
		int PointsPerScreen;				//每屏采样点数
		int ChannelNum;						//通道数量
		vector<XYLineChannel> DataChanel;	//通道设置
		int ReferenceLineNum;			//参考线数量
		vector<ReferenceLine> ReferenceLines;	//参考线设置
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RefreshMode, SamplingCycle, TriggerTypeVar, TriggerCond, PauseControlVar, ClearControlVar,
				UseZoom, ZoomPercent, PageControlStyle, TurnControlVar, PauseControlCondition, ClearControlCondition,
				SamplePoints, SamplePointsVar, PointsPerScreen, ChannelNum, DataChanel,
				ReferenceLineNum, ReferenceLines, LineChartBackColor, ScaleAreaBackColor, UseGrid,
				XGridEqualNum, YGridEqualNum, GridStyle, OriginPos, XaxisLen, YaxisLen, ScrollHeight,
				UseYScale, UseXScale, XScaleSet, YScaleSet, XScaleInfo, YScaleInfo,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

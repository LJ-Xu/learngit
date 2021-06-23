/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 用于存放趋势图通参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "TrendViewUnit.h"
#include "SampleInfoRes.h"
#include "TrendChannel.h"
namespace Project
{
	
	class TrendChartConfig : public TrendViewUnit
	{
	public:
		int DataCapacity;					//数据容量
		int PerSceenViewMode;				//屏幕显示内容	0:点数 1:时间段
		int PerScreendot;					//每屏数据容量
		int PerScreenPeriod;				//每屏时间段
		int SimpleGroup;					//采样组
		int SimpleNo;						//采样编号
		DataVarId SampleVarId;				//通知采样数据变化寄存器
		//Project::SampleMode CollectMode;	//采样模式
		/*周期采样*/
		//DataVarId SimpleCycleVarId;			//采样频率指定寄存器
		//int SimpleCycle = 500;
		vector<TrendChannel> SetChannel;	//采样通道设置

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(DataCapacity, PerSceenViewMode, PerScreendot, PerScreenPeriod, SimpleGroup,
				SimpleNo, SetChannel, SampleVarId,
				BgColor, ScaleAreabgColor, IsGriddisplay, XUniformNum,
				YUniformNum, GridStyle, AxisX, AxisY, OriginPos, XaxisLen, YaxisLen,
				ScrollHeight, YScaleInfo, XScaleInfo, ZoomVarId, PageCtrlVarId,
				IsDataDisplay, DisplayContent, DisplayDateStyle, DisplayTimeStyle,
				InfoBgColor, InfoFontColor, InfoLineColor, SearchMode, RegMode, SearchRegVarId,
				EnableSearch, SeekModeVar, SeekCtrlVar, SearchDate, SearchDateVarId,
				SearchTimeStart, SearchTimeEnd, SearchTimeVar, SearchChannelNo, SearchChannelNoVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};

}


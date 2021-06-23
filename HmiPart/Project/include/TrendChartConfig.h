/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : ���ڴ������ͼͨ��������
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
		int DataCapacity;					//��������
		int PerSceenViewMode;				//��Ļ��ʾ����	0:���� 1:ʱ���
		int PerScreendot;					//ÿ����������
		int PerScreenPeriod;				//ÿ��ʱ���
		int SimpleGroup;					//������
		int SimpleNo;						//�������
		DataVarId SampleVarId;				//֪ͨ�������ݱ仯�Ĵ���
		//Project::SampleMode CollectMode;	//����ģʽ
		/*���ڲ���*/
		//DataVarId SimpleCycleVarId;			//����Ƶ��ָ���Ĵ���
		//int SimpleCycle = 500;
		vector<TrendChannel> SetChannel;	//����ͨ������

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


#pragma once
#include "XYLineChartViewUnit.h"
#include "XYLineChannel.h"
namespace Project
{
	enum FullSampleAct {StopSample, CleanSample, CoverSample};
	struct ReferencePolyLines
	{
		int LineColor;
		int LineMode;		//0:���� 1:��
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
		int RefreshMode;					//ˢ��ģʽ	0������ʽ 1������ʽ
		int SamplingCycle;					//ˢ������(ms)
		DataVarId TriggerTypeVar;			//������ַ
		int TriggerType;					//����ģʽ 0:λ	1:��
		Conditions ConditionWord;			//�ִ�������
		double TriggerData;					//�����ж�����
		DataVarId TriggerDataVar;			//�����Ա����ݵ�ַ
		int  ConditionBit;					//λ��������	0��ON->OFF 1��OFF->ON
		bool UseZoom;						//ʹ������
		int ZoomPercent;					//���Űٷֱ�
		int PageControlStyle;				//��ҳģʽ 0�������� 1���Ĵ���
		DataVarId TurnControlVar;			//��ҳ�Ĵ���
		int SamplePoints;					//��������
		DataVarId SamplePointsVar;			//���������Ĵ���
		int PointsPerScreen;				//ÿ����������
		int ChannelNum;						//ͨ������
		bool UseXScaleTime;					//X��ʹ��ʱ��
		vector<XYLineChannel> DataChanel;	//ͨ������
		FullSampleAct HandleSampleMode;		//��������ʽ

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


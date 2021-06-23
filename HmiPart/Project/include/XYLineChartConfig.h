#pragma once
#include "XYLineChartViewUnit.h"
#include "XYLineChannel.h"
namespace Project
{
	class XYLineChartConfig : public XYLineChartViewUnit
	{
	public:
		int RefreshMode;			//ˢ��ģʽ	0������ʽ 1������ʽ
		int SamplingCycle;			//ˢ������(ms)
		DataVarId TriggerTypeVar;	//������ַ
		int TriggerCond;			//��������	0�������� 1���½���
		DataVarId PauseControlVar;		//��ͣ����
		DataVarId ClearControlVar;		//�������
		int PauseControlCondition;		//0��ON��1��OFF
		int ClearControlCondition;		//0�������� 1���½���
		bool UseZoom;					//ʹ������
		int ZoomPercent;				//���Űٷֱ�
		int PageControlStyle;				//��ҳģʽ 0�������� 1���Ĵ���
		DataVarId TurnControlVar;			//��ҳ�Ĵ���
		int SamplePoints;					//��������
		DataVarId SamplePointsVar;			//���������Ĵ���
		int PointsPerScreen;				//ÿ����������
		int ChannelNum;						//ͨ������
		vector<XYLineChannel> DataChanel;	//ͨ������
		int ReferenceLineNum;			//�ο�������
		vector<ReferenceLine> ReferenceLines;	//�ο�������
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

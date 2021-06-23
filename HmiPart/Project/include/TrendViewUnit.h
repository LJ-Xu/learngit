#pragma once
#include "AlarmSeekUnit.h"
#include "BaseLine.h"
#include "StringStyle.h"
#include "AlarmDisplayConfig.h"
#include "ScaleInfo.h"

namespace Project
{
	class AxisSetting
	{
	public:
		bool IsdisplayScale;			//�Ƿ�ʹ��X��̶�
		int ScaleColor;					//�̶���ɫ
		int MainScaleNum;				//���̶���
		int MainScaleLen;				//���̶��߳���
		bool IsViceScaleNum;			//�Ƿ�ʹ�ø��̶�
		int ViceScaleNum;				//���̶���
		int ViceScaleLen;				//���̶��߳���
		StringStyle ScaleStyle;			//�����ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsdisplayScale, ScaleColor, MainScaleNum, MainScaleLen, IsViceScaleNum,
				ViceScaleNum, ViceScaleLen, ScaleStyle);
		}

	};
	class XAxis : public AxisSetting
	{
	public:
		bool IsUseTimeScale;			//�Ƿ�ʹ��ʱ��̶�
		bool IsdiaplayDate;				//��ʾ����
		DateMode DateStyle;				//���ڸ�ʽ
		bool IsdisplayTime;				//��ʾʱ��
		TimeMode TimeStyle;				//ʱ���ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsUseTimeScale, IsdiaplayDate, DateStyle, IsdisplayTime, TimeStyle,
				IsdisplayScale, ScaleColor, MainScaleNum, MainScaleLen, IsViceScaleNum,
				ViceScaleNum, ViceScaleLen, ScaleStyle);
		}
	};
	enum YScaleMode :char { Custom, Channel, All };
	enum YScaleTag :char { NullSacle, NumSacle, PercentSacle };

	class YAxis : public AxisSetting
	{
	public:
		YScaleTag ScaleTag;					//�̶ȱ��	0������ʾ��1�����֣�2���ٷֱ�
		int YIntegerNum;				//����λ��
		int YDecimalnNum;				//С��λ��
		YScaleMode ScaleRange;			//�̶ȷ�Χ	0���Զ��� 1��ʹ��ͨ�������Сֵ 2����ʾ����ͨ����Χ
		int UseRangeChanbelNo;			//ʹ��ͨ���ı��
		double YScaleMax;				//Y�����ֵ
		DataVarId YScalemaxVarId;		//����Y�����ֵ�Ĵ���
		double YScaleMin;				//Y����Сֵ
		DataVarId YScaleminVarId;		//����Y����Сֵ�Ĵ���

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ScaleTag, YIntegerNum, YDecimalnNum, ScaleRange, YScaleMax, UseRangeChanbelNo,
				YScalemaxVarId, YScaleMin, YScaleminVarId, IsdisplayScale, ScaleColor,
				MainScaleNum, MainScaleLen, IsViceScaleNum, ViceScaleNum, ViceScaleLen, ScaleStyle);
		}
	};

	
	struct DataDisplay
	{
		string ProjectName;
		string DescribeName;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ProjectName, DescribeName);
		}
	};
	enum TrendSeekMode :char { DATE, TIME, CHANNELNO, REG };
	class TrendViewUnit : public AlarmSeekUnit
	{
	public:
		int BgColor;					//����ͼ������ɫ
		int ScaleAreabgColor;			//�̶���������ɫ
		bool IsGriddisplay;				//�Ƿ�ʹ��դ����ʾ
		int XUniformNum;				//X��դ��ȷ���
		int YUniformNum;				//Y��դ��ȷ���
		BaseLine GridStyle;				//դ��������ʽ

		XAxis AxisX;					//X������
		YAxis AxisY;					//Y������

		Point OriginPos;				//ԭ������
		int XaxisLen;					//X�᳤
		int YaxisLen;					//Y�᳤
		int ScrollHeight;				//�������߶�
		vector<vector<ScaleInfo>> YScaleInfo;		//Y���עλ��
		vector<ScaleInfo> XScaleInfo;				//X���עλ��

		bool IsDataDisplay;							//�Ƿ���ʾѡ�е�����
		vector<DataDisplay> DisplayContent;			//��ʾ����
		DateMode DisplayDateStyle;					//���ڸ�ʽ
		TimeMode DisplayTimeStyle;					//ʱ���ʽ
		int InfoBgColor;							//��Ϣ���ڱ�����ɫ
		int InfoFontColor;							//��Ϣ����������ɫ
		int InfoLineColor;							//��Ϣ��������ɫ

		DataVarId ZoomVarId;			//����
		DataVarId PageCtrlVarId;		//��ҳ
		
		TrendSeekMode SearchMode;			//��ѯ��ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(BgColor, ScaleAreabgColor, IsGriddisplay, XUniformNum,
				YUniformNum, GridStyle, AxisX, AxisY, OriginPos, XaxisLen, YaxisLen,
				ScrollHeight, YScaleInfo, XScaleInfo, ZoomVarId, PageCtrlVarId,
				IsDataDisplay, DisplayContent, DisplayDateStyle, DisplayTimeStyle,
				InfoBgColor, InfoFontColor, InfoLineColor, SearchMode);
		}
	};


}

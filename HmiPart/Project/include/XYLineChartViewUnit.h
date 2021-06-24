#pragma once
#include "TrendViewUnit.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	struct ScaleMark
	{
		double UpperLimit;				//����
		DataVarId UpperLimitVar;		//���޼Ĵ���
		double LowerLimit;				//����
		DataVarId LowerLimitVar;		//���޼Ĵ���
		int ScaleColor;					//�̶���ɫ
		int PrimaryScaleNum;			//���̶���
		int PrimaryScaleLen;			//���̶ȳ���
		bool UseSecondaryScaleNum;		//�Ƿ�ʹ�ø��̶�
		int SecondaryScaleNum = 1;		//���̶���
		int SecondaryScaleLen;			//���̶ȳ���
		ScaleMarkType MarkType;			//���̶ֿ���ʽ 0:��ʹ�� 1:��ֵ 2:�ٷֱ�
		string ScaleMarkFont;			//��ע����
		int ScaleMarkSize;				//��ע�����С
		int ScaleMarkColor;				//��ע��ɫ
		int IntegerNum;					//����λ��
		int DecimalnNum;				//С��λ��

		/*�������*/
		int ScaleType;					//�̶���ʽ	0��ʱ�� 1������
		bool IsUseTimeScale;			//�Ƿ�ʹ��ʱ��̶�
		bool IsdiaplayDate;				//��ʾ����
		DateMode DateStyle;				//���ڸ�ʽ
		bool IsdisplayTime;				//��ʾʱ��
		TimeMode TimeStyle;				//ʱ���ʽ

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
		int LineColor;					//����ɫ
		double LineValue;					//����ֵ
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
		int LineChartBackColor;			//������ɫ
		int ScaleAreaBackColor;			//�̶���������ɫ
		bool UseGrid;					//�Ƿ�ʹ��դ����ʾ
		int XGridEqualNum;				//X��դ��ȷ���
		int YGridEqualNum;				//Y��դ��ȷ���
		BaseLine GridStyle;				//դ��������ʽ

		Point OriginPos;				//ԭ������
		int XaxisLen;					//X�᳤
		int YaxisLen;					//Y�᳤
		int ScrollHeight = 0;			//�������߶�
		bool UseYScale;
		bool UseXScale;
		ScaleMark XScaleSet;			//X���ע����
		ScaleMark YScaleSet;			//Y���ע����
		vector<ScaleInfo> XScaleInfo;	//��עλ��
		vector<ScaleInfo> YScaleInfo;	//��עλ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(LineChartBackColor, ScaleAreaBackColor, UseGrid,
				XGridEqualNum, YGridEqualNum, GridStyle, OriginPos, XaxisLen, YaxisLen, ScrollHeight,
				UseYScale, UseXScale, XScaleSet, YScaleSet, XScaleInfo, YScaleInfo);
		}
	};


}
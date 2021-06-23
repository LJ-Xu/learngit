#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class ScaleConfig : public MoveableDisplayUnit
	{
	public:
		enum ScaleType
		{
			Horizontal,//ˮƽ��Horizontal
			Vertical,//��ֱ��Vertical
			UpperSemicircle,//�ϰ�Բ
			LowerSemicircle,//�°�Բ
			Circular,//Բ
			UDCir//�Զ���Բ
		};

		ScaleType ScaleTp;// �̶ȵ�����
		int LineColor;// �̶���������ɫ��Ĭ�Ϻ�ɫ
		int LineStyle;// �̶�������ʽ
		int LineWidth;// �̶��������
		int MainGraduateCount;// ���̶ȷָ���
		int MajorScaleLineLength;// ���̶��߳���
		bool UseSecondaryScale;// �Ƿ�ʹ�ø��̶ȱ�־
		int SecondaryScaleCutCount;// ���̶ȷָ���
		int SecondaryScaleLength;// ���̶ȳ���
		bool IsDisplayMark;// �Ƿ���ʾ���
		int MarkIntegerNumber;// ��ǵ�����λ��
		int MarkDecimalNumber;// ��ǵ�С��λ��
		StringStyle MarkFontSytle;// ��ǩ��������ʽ
		int MarkMaxValue;// �̶ȱ�ǩ����ֵ
		bool IsMarkMaxValueUseRegister;// �������ֵ�Ƿ�ʹ�üĴ�����Ĭ�Ϸ�
		DataVarId MarkMaxValueVarIdRef;// ������ֵ��ȡ�Ĵ���
		int MarkMinValue;// ��ǩ������ֵ
		bool IsMarkMinValueUseRegister;// ��ǩ�����Ƿ�ʹ�üĴ���
		DataVarId MarkMinValueVarIdRef;// ��ǩ��Сֵʹ�üĴ���
		bool IsReserveMark;// �Ƿ�ת�̶ȱ��,��ʱ��Ϊ����������ʱ��ΪĬ��������������,��IsReserveMark=falseʱ
		// ��ǵ�λ��
		// �̶�����Ϊ��ֱʱ��0����� 1���Ҳ� 2������
		// ˮƽʱ��0���� 1���� 2������
		// Բ�� 0���� 1���� 2������
		int MarkPlace;
		int CircularStartAngle;
		int CircularEndAngle;
		bool IsDisplayAxis;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, Width, Height,VOffX,VOffY,Perm, CtrlName, ScaleTp, LineColor, LineStyle, LineWidth, MainGraduateCount, MajorScaleLineLength, UseSecondaryScale, SecondaryScaleCutCount, SecondaryScaleLength,IsDisplayMark, MarkIntegerNumber, MarkDecimalNumber, MarkFontSytle, MarkMaxValue, IsMarkMaxValueUseRegister, MarkMaxValueVarIdRef, MarkMinValue, IsMarkMinValueUseRegister, MarkMinValueVarIdRef, IsReserveMark,MarkPlace, IsDisplayAxis);
		}
	};
}
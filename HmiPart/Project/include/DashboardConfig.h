#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"

namespace Project
{
	class DashboardConfig : public MoveableDisplayUnit
	{
	public:
		
		DataVarId DataCtlSign;//����ź�(��ǰֵ)
		bool MaxUseAddr;//�����Сֵ�Ƿ�ȡ�Ե�ֵַ
		bool MinUseAddr;//��Сֵ�Ƿ�ȡ�Ե�ֵַ 
		DataVarId MaxData;//���ֵ��ַ
		DataVarId MinData;//��Сֵ��ַ
		double MaxValue;//���ֵ
		double MinValue;//��Сֵ
		int DataColor;//������Χ����ɫ
		int NormalRingWidth;//�����Ȼ����
		int RingToCircleCenterLength;//�Ȼ���Բ�ľ���
		bool IsUsedAlarmLimit;//�Ƿ����ñ���������
		bool IsUpperWarnValueUsedRegister;//���ޱ����Ƿ�ʹ�üĴ���
		bool IsLowerWarnValueUsedRegister;//���ޱ����Ƿ�ʹ�üĴ���
		double UpperWarnValue;//��ʾ����
		DataVarId UpperWarnRegister;//�������޼Ĵ���
		double LowerWarnValue;//��ʾ����
		DataVarId LowerWarnRegister;//�������޼Ĵ���
		int UpperWarnColor;//���޾�����ɫ
		int LowerWarnColor;//���޾�����ɫ
		bool IsUsedDangerZone;//�Ƿ�����Σ������
		bool IsUpperDangerUsedRegister;//Σ��ֵ�����Ƿ�ʹ�üĴ���
		bool IsLowerDangerUsedRegister;//Σ��ֵ�����Ƿ�ʹ�üĴ���
		double UpperDangerValue;//Σ��ֵ����
		DataVarId UpperDangerRegister;//Σ��ֵ���޼Ĵ���
		double LowerDangerValue;//Σ��ֵ����
		DataVarId LowerDangerRegister;//Σ��ֵ���޼Ĵ���
		int DangerColor;//Σ��ֵ��ɫ
		ImageResId DashBoardStyle;//�Ǳ�����ʽ
		int ArcDirection;//���� 0˳ʱ�� 1��ʱ��
		int StartAngle;//��ʼ�Ƕ�
		int EndAngle;
		int Transparency;//͸���ȣ���λ%
		ImageResId WatchHandleStyle;//������ʽ
		int WatchHandleWidth;//������
		int WatchHandleLength;//���볤��
		int WatchHandleInnerColor;//�����ڲ���ɫ
		int WatchHandleBorderColor;//����߿���ɫ
		ImageResId AxisStyle;//������ʽ
		int AxisRadius;//���ĵİ뾶��������ʱ�Ǳ߳�
		int AxisInnerColor;//�����ڲ���ɫ
		int AxisBorderColor;//���ı߿���ɫ
		bool ShowGraduate;//��ʾ�̶�
		int MainGraduateCount;//���̶�����
		int SecondaryGraduateCount;//�ο̶�����
		int MainGraduateLen;//���̶ȵȳ���
		int SecondaryGraduateLen;//�ο̶ȵȳ���
		int GraduateColor;//�̶�����ɫ
		int GraduatePosition;//�̶�λ�� 0�ڲ⣬1��࣬2����
		int ShowGraduateSign;//��ʾ�̶ȱ�ǣ����֣�0����ʾ 1��ʾ���� 2��ʾ�ٷֱ�
		StringStyle TypeFaceGraduate;//�̶�����
		int DecimalDigit;//С��λ
		int IntegerDigit;//����λ

		bool IsCuttedBoarder;
		ImageResId CuttedBoardStyle;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, DataCtlSign, MaxUseAddr, MinUseAddr, MaxData, MinData, MaxValue, MinValue, DataColor, NormalRingWidth, RingToCircleCenterLength,IsUsedAlarmLimit, IsUpperWarnValueUsedRegister, IsLowerWarnValueUsedRegister, UpperWarnValue, UpperWarnRegister, LowerWarnValue, LowerWarnRegister, UpperWarnColor, LowerWarnColor,  IsUsedDangerZone, IsUpperDangerUsedRegister, IsLowerDangerUsedRegister, UpperDangerValue, UpperDangerRegister, LowerDangerValue, LowerDangerRegister, DangerColor, DashBoardStyle, ArcDirection, StartAngle, EndAngle, Transparency, WatchHandleStyle, WatchHandleWidth, WatchHandleLength, WatchHandleInnerColor, WatchHandleBorderColor, AxisStyle, AxisRadius, AxisInnerColor, AxisBorderColor, ShowGraduate, MainGraduateCount, SecondaryGraduateCount, MainGraduateLen, SecondaryGraduateLen, GraduateColor, GraduatePosition, ShowGraduateSign, TypeFaceGraduate, DecimalDigit, IntegerDigit);
		}
	};
}
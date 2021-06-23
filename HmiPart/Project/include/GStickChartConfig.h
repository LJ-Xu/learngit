#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"
namespace Project
{

	class GStickChartConfig : public MoveableDisplayUnit
	{
	public:
		////���󣨵�ַ���ݣ�
		DataVarId BarVar; //����

		////��Χ����Χ��
		///���ֵ
		int MaxValue;//���ֵ(����)
		DataVarId MaxData;//���ֵ(��ַ)
		bool UseMaxAddr;//���ֵ�Ƿ��ɼĴ���ָ�� 

		///��Сֵ
		int MinValue;//��Сֵ��������
		DataVarId MinData;//��Сֵ(��ַ)
		bool UseMinAddr;//��Сֵ�Ƿ��ɼĴ���ָ��

		///Ŀ������
		bool UseDstField; //ʹ�ö����Ŀ������

		int DstValue;//Ŀ��ֵ(����)
		DataVarId DstValueAddr;//Ŀ��ֵ(��ַ)
		bool UseDstValueByAddr;//ʹ�üĴ���ָ��Ŀ��ֵ

		int DstRange;//��Χ(����)
		DataVarId DstRangeAddr;//��Χ(��ַ)
		bool UseDstRangeByAddr;//ʹ�üĴ���ָ����Χ

		int DstColor;//���������ɫ

		///��Χ����
		bool UseRangeWarn;//���÷�Χ����

		int UpperWarnValue;//�Ͼ���ֵ(����)
		DataVarId UpperWarnAddr;//�Ͼ���ֵ(��ַ)
		bool UseUpperWarnAddr; //�Ĵ���ָ���Ͼ���ֵ

		int LowerWarnValue;//�¾���ֵ(����)
		DataVarId LowerWarnAddr;//�¾���ֵ(��ַ)
		bool UseLowerWarnAddr; //�Ĵ���ָ���¾���ֵ

		int UpperWarnFillinColor;//�Ͼ������ɫ
		int UpperWarnFrontColor;//�Ͼ���ǰ��ɫ
		int LowerWarnFillinColor;//�¾������ɫ
		int LowerWarnFrontColor;//�¾���ǰ��ɫ

		////���
		int Shape;//��ǰѡ�����״��0ֱ����1����

		///�Զ�����ۣ���ֱ����
		bool IsUserDefineBar;
		////�Զ�����ۿɱ���������Ͻ����꣨ȡ���أ�
		//Point BarAreaRectanglePoint;
		//�Զ�����ۿɱ�����ĳ���
		//BaseDisplayUnit BarAreaRectangle;
		int BarAreaRectanglePointX;
		int BarAreaRectanglePointY;
		int BarAreaRectangleSizeWidth;
		int BarAreaRectangleSizeHeight;
		///ֱ��
		int BarDirection; //����0�ϣ�1�£�2��3��

		///����
		int ArcInnerCirclePercent;//���⻷����
		int ArcStartAngle;//��ʼ�Ƕ�
		int ArcSweepAngle;//���ǽǶ�
		int ArcDirection; //����0˳ʱ�룻1��ʱ��

		///�߿�
		bool ShowFrame;//��ʾ�߿�
		int ColorShowFrame;//ǰ��ɫ

		///����
		bool ShowBackground;//��ʾ����ɫ
		int ColorBack;//����ɫ

		///���
		int FIllinColor;//���ɫ

		///ͼ�����
		bool UseFillinStyle;////ͼ�����
		int FillinStyle;//��ʽ
		int ColorFillinStyle;//ǰ��ɫ

		///����
		bool UseFillinShade;//����
		int ShadeStyle;//��ʽ:0�� 1�� 2�K 3�J
		int ColorShadeStyle;//ǰ��ɫ
		int FadeoutPercent;//͸����

		////�̶�����
		///�̶�
		bool ShowGraduate;//��ʾ�̶�
		//�̶����ͼ�����λ�ã����ݰ�ͼ����ķ�������ʽ�䶯
		//ֵ��      0        1
		//����      ��       ��
		//�᣺      ��       ��
		//�ȣ�      ��       ��
		int RelatedGraduateSeat;
		int MainGraduateCount; //���̶���
		int MainGraduateLen;//���̶ȳ���
		int SecondaryGraduateCount; //���̶���
		int SecondaryGraduateLen;//���̶ȳ���

		///�̶���
		int LineColor;//�̶�����ɫ
		int LineStyle;//�̶�����ʽ
		int LineWidget;//�̶��߿��

		///���ֱ��
		bool ShowGraduateSign; //��ʾ���ֱ��
		int IntegralDigit;//����λ
		int DecimalDigit; //С��λ
		StringStyle TypeFaceGraduate;//�̶�����

		///�ٷֱ�
		bool ShowPercent;
		StringStyle TypeFacePecent; //�̶�����
		bool ShowAxis;//��ʾ����

		///��ȫ����
		//ControlPerm CtlPerm;
		//��ʱ��Ŀ���������ǰ��������ʽ��ӣ�
		ImageResId BarRes;//Һ��/�Զ����ͼͼƬ
		ImageResId LinearGradientRes;//Һ��/�Զ����ͼͼƬ

	public:
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, Width, Height, VOffX, VOffY,CtrlName, Perm,IsUserDefineBar, IsRecord,
				BarVar,
				MaxData, UseMaxAddr,MaxValue, 
				MinValue, MinData, UseMinAddr,
				UseDstField, DstValue, DstValueAddr, UseDstValueByAddr, DstRange, DstRangeAddr, UseDstRangeByAddr,
				DstColor, UseRangeWarn, UpperWarnValue, UpperWarnAddr, UseUpperWarnAddr, LowerWarnValue, LowerWarnAddr, UseLowerWarnAddr,UpperWarnFillinColor, UpperWarnFrontColor, LowerWarnFillinColor, LowerWarnFrontColor,
				Shape, BarDirection, LinearGradientRes,
				ArcInnerCirclePercent, ArcStartAngle, ArcSweepAngle, ArcDirection,
				ShowFrame, ColorShowFrame,
				ShowBackground, ColorBack, FIllinColor,
				UseFillinStyle, FillinStyle, ColorFillinStyle,
				UseFillinShade, ShadeStyle, ColorShadeStyle, FadeoutPercent,
				ShowGraduate, RelatedGraduateSeat, MainGraduateCount, 
				BarAreaRectanglePointX,
				BarAreaRectanglePointY,
				BarAreaRectangleSizeWidth,
				BarAreaRectangleSizeHeight,
				MainGraduateLen, SecondaryGraduateCount, SecondaryGraduateLen,
				LineColor, LineStyle, LineWidget,
				ShowGraduateSign, IntegralDigit, DecimalDigit, TypeFaceGraduate,
				ShowPercent, TypeFacePecent, ShowAxis,
				BarRes
			);
		}
	};

}
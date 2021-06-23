#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"

namespace Project
{
	class BarCodeConfig :public MoveableDisplayUnit
	{
	public:
		enum TwoDimensionCodeType
		{
			QRCode,
			DataMatrix,
			PDF417,
		};
		enum OneDimensionCodeType
		{
			EAN13=1,
			Code39,
			Code128=0,
		};
		int Dimension;// �����ά�� 0��һά��1�Ƕ�ά
		TwoDimensionCodeType TwoDimensionType;// �������Ƕ�άʱ���������
		OneDimensionCodeType OneDimensionType;// ��������һάʱ��������ࡣ
		int EncodingRule;// ������� 0.ascii 1.unicode
		int CorrectErrorLevel;// У����׼������������QRCodeʱ��Ч��0��L����1��M�� ��2��Q���� 3:H��
		bool UseConstValue;// �Ƿ�ʹ�ù̶�ֵ true�� false ��
		std::string ConstString;// ʹ�ù̶�ֵʱ���ַ���
		DataVarId ReadVar;// ʹ�üĴ���ָ��ʱ�ļĴ���
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(X, Y, VOffX, VOffY, Perm, Width, Height, CtrlName, Dimension, TwoDimensionType, OneDimensionType, EncodingRule, CorrectErrorLevel, UseConstValue, ConstString, ReadVar);
		}
	};
}
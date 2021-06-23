#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "GLineConfig.h"
#include "ImageResId.h"
namespace Project
{
	class GPolyLineConfig : public MoveableDisplayUnit
	{
	public:
		vector<Point> Points;		//��������
		int Mode;					//0:���� 1:�����
		BaseLine LineStyle;			//��
		BaseArrow Arrow;			//��ͷ����
		bool IsFill;				//�Ƿ����
		int  FillColor;				//�����ɫ
		int  FillStyle;				//�����ʽ ������� ͼ��
		ImageResId BgPicKey;
		double RotateAngle;			//��ת�Ƕ�
		Point RotateCenter;			//��ת����
		bool UseLine;
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Points, Mode, LineStyle, Arrow, IsFill, FillColor, FillStyle, BgPicKey, UseLine,
				RotateAngle, RotateCenter, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};


}

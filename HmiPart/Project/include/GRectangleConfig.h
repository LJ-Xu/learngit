/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ�ž��οؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
namespace Project
{
	class GRectangleConfig : public MoveableDisplayUnit
	{
	public:
		Point Pos; //Բ����λ��
		int Angle; //�Ƕ�
		int Radius; //Բ��ֱ��
		bool IsFill; //�Ƿ����
		int  FillColor; //�����ɫ
		int  FillStyle; //�����ʽ ������� ͼ��
		BaseLine Line; //��
		ImageResId BgPicKey;

		int RectWidth;
		int RectHeight;
		double RotateAngle;			//��ת�Ƕ�
		Point RotateCenter;			//��ת����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Pos, Angle, Radius, IsFill, FillColor, FillStyle, Line, BgPicKey, RectWidth, RectHeight,
				RotateAngle, RotateCenter, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}
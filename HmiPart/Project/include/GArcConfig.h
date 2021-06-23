/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ��Բ�����Ʋ����Ľṹ��
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "GLineConfig.h"
#include "CerealCommon.hpp"
#include "ImageResId.h"
namespace Project
{
	class ArcRes
	{
	public:
		int A;	//���߳���
		int B;	//�̱߳���
		Point Center;	//Բ������


		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(A, B, Center);
		}

	};
	class GArcConfig : public MoveableDisplayUnit
	{
	public:
		bool IsFill;				//�Ƿ����

		double StartAngle;			//��ʼ�Ƕ�
		double EndAngle;			//�����Ƕ�

		bool IsSector;				//�Ƿ�������
		int  FillColor;				//�����ɫ

		int  FillStyle;				//�����ʽ ������� ͼ��
        Project::ArcRes ArcRes;				//��Բ��Ϣ
		vector<Point> Coordinate;	//Բ������
		BaseLine Line;				//��

		ImageResId BgPicKey;		//�������

		double RotateAngle;			//��ת�Ƕ�
		Point RotateCenter;			//��ת����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsFill, StartAngle, EndAngle, IsSector, FillColor,
				FillStyle, ArcRes, Coordinate, Line, VOffX, VOffY, BgPicKey,
				RotateAngle, RotateCenter, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}


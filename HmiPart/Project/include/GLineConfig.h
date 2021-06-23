/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ��ֱ�߻�����ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#ifdef WIN32
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "CerealCommon.hpp"
#else
#include "BaseLine.h"
#include "Point.h"
#include "MoveableDisplayUnit.h"
#include "CerealCommon.hpp"
#endif // WIN32

namespace Project
{
	enum ArrowType { TriangleArrow, Arrow, SolidArrow, RectArrow, DotArrow };
	class BaseArrow
	{
	public:
		bool HaveStartArrow;	//��ʼ�Ƿ��м�ͷ
		ArrowType StartArrowType;		//��ʼ��ͷ����
		int StartArrowSize;		//��ʼ��ͷ�ߴ�

		bool HaveEndArrow;		//�����Ƿ��м�ͷ
		ArrowType EndArrowType;		//������ͷ����
		int EndArrowSize;		//������ͷ�ߴ�

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(HaveStartArrow, StartArrowType, StartArrowSize, 
				HaveEndArrow, EndArrowType, EndArrowSize);
		}
	};

	class GLineConfig : public MoveableDisplayUnit
	{
	public:
		BaseLine Line;				//������
		vector<Point> Coordinate;	//�߶���ʼ��ֹ����
		BaseArrow Arrow;			//��ͷ����
		double RotateAngle;			//��ת�Ƕ�
		Point RotateCenter;			//��ת����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Line, Coordinate, RotateAngle, RotateCenter,
				Arrow, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

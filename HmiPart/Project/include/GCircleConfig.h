/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleConfig.h
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ��Բ����������ݵ���
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
#include "BaseLine.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include "GArcConfig.h"
namespace Project
{
	class GCircleConfig : public MoveableDisplayUnit
	{
	public:
		bool IsFill;			//�Ƿ����
		bool IsStandard;		//�Ƿ��׼Բ

		int  FillColor;			//�����ɫ
		int  FillStyle;			//�����ʽ ������� ͼ��
		int  ColorFillinStyle;
		bool UseFillinStyle;
		BaseLine Line; //��

		ImageResId BgPicKey;
        Project::ArcRes ArcRes;				//��Բ��Ϣ

		double RotateAngle;			//��ת�Ƕ�
		Point RotateCenter;			//��ת����
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ArcRes, IsFill, IsStandard, FillColor, FillStyle, 
				ColorFillinStyle, BgPicKey, RotateAngle,
				RotateCenter, UseFillinStyle, Line, VOffX, VOffY, Perm,
				Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}

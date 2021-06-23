/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GFuncConfig.h
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : ���������ؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "Color.h"

namespace Project
{
	enum DrawFunc {
		DCMapClear = 0,
		DCMapSetBackColor,
		DCMapDrawLine,
		DCMapDrawRect,
		DCMapDrawCircle,
		DCMapDrawEllipse,
		DCMapDrawCircleArc,
		DCMapDrawEllipseArc
	};

	class GFuncConfig : public MoveableDisplayUnit
	{
	public:
		GFuncConfig(){ }
		~GFuncConfig(){ }

		template<class Archive>
		void serialize(Archive & archive) {
			archive(FuncId, VOffX, VOffY, Perm, Width, Height,
				X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}

		void Init() {
			MapId = 0;
			GX = 0;
			GY = 0;
			GW = 0;
			GH = 0;
			LineWidth = 0;
			LineColor = Project::Color(255, 255, 255);
			FillRect = false;
			FillColor = Project::Color(255, 255, 255);
			BackColor = Project::Color(255, 255, 255);
			StartAngle = 0;
			EndAngle = 0;
		}

	public:
		int FuncId;		// �������ñ��
		int MapId;		// ID
		int GX;			// ��ʼx����(Բ�ĺ�����)
		int GY;			// ��ʼy����(Բ��������)
		int GW;			// ��(�뾶)(x��뾶)
		int GH;			// ��(�뾶)(y��뾶)
		int LineWidth;	// ������ϸ
		int StartAngle;	// ��ʼ�Ƕ�
		int EndAngle;	// �յ�Ƕ�
		bool FillRect;	// �Ƿ����
		Color LineColor;// ������ɫ
		Color FillColor;// �����ɫ
		Color BackColor;// ������ɫ
	};
}


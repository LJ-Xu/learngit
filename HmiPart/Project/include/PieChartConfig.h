/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ��ͼ�ؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "Color.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

namespace Project 
{
	class PieChartChannelStyle {
	public:
		float Val = 0.0F;	// ͨ����������
		float Percent = 0.0F;
		string text;
		Color FontColor;	// ͨ��������ɫ
		Color BackColor;	// ͨ��������ɫ
		int StartAngle;		// ��ʼ�Ƕ�
		int EndAngle;		// ��ֹ�Ƕ�
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Val, Percent, text, FontColor, BackColor, StartAngle, EndAngle);
		}
	};

	class PieChartConfig : public MoveableDisplayUnit
	{
	public:
		PieChartConfig() {
			CenterColor = Color(255, 255, 255);	// Բ���ڲ���ɫ
			CenterEdgeColor = Color(255, 255, 255);	// Բ�ı߿���ɫ
			BorderColor = Color(255, 255, 255);	// ��ͼ�߿���ɫ
		}
		~PieChartConfig() {}

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVars, ChannelNum, Dir, StartAngle, EndAngle, Alpha,
					CenterRadius, CenterColor, CenterEdgeColor, BorderColor, 
					Channels, Mode, Bct, TextStyle,
					VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}

	public:
		vector<DataVarId> ReadVars;			// ��ȡ�Ĵ���
		int ChannelNum = 4;			// ͨ����
		int Dir = 0;				// ����	0:˳ʱ�� 1:��ʱ��
		int StartAngle = 0;			// ��ʼ�Ƕ�
		int EndAngle = 360;			// ��ֹ�Ƕ�
		int Alpha = 255;			// ͸����
		float CenterRadius = 0;		// �ڲ�Բ�İ뾶
		Color CenterColor;			// Բ���ڲ���ɫ
		Color CenterEdgeColor;		// Բ�ı߿���ɫ		
		Color BorderColor;			// ��ͼ�߿���ɫ	
		vector<PieChartChannelStyle> Channels;
		int Mode = 1;				// ��ʾ��ʽ	0:����ʾ 1:��ʾ���� 2:��ʾ�ٷֱ�
		int Bct = 0;				// С��λ��
		StringStyle TextStyle;		// ������ʽ
		float TotalVal = 0;
	};
}


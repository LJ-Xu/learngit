/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : ���ڴ�Ż�������ؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "ImageResId.h"
#include "NoticesAction.h"
#include "TrendViewUnit.h"
#include "BaseLine.h"
namespace Project
{
	enum SlideDir {SildeUp, SildeDown, SildeLeft, SildeRight};
	class SlideInputConfig : public MoveableDisplayUnit
	{
	public:
		DataVarId ReadVar;				//��д��ַ
		int DataStyle;					//���ݸ�ʽ
		double AttrMax;					//���ֵ
		DataVarId AttrMaxVar;	
		double AttrMin;					//��Сֵ
		DataVarId AttrMinVar;

		SlideDir Dir;					//����
		int MinScale;					//��С�̶�
		bool UseIncDecPerClick;
		int IncDecPerClickScale;		//ÿ�ε�����ӱ���
		bool UseChangeInput;			//��ק��ʾ��ֵ

		int SlideRailWidth;				//������
		int SlideRailHeight;			//����߶�
		int SlideRailBackColor;			//���챳����ɫ
		int SlideRailBorderColor;		//����߿���ɫ
		ImageResId SlideRailKey;		//������ʽͼƬ
		Point SlideRailPos;				//����λ��

		int SlideWidth;					//������
		int SlideHeight;				//����߶�
		int SlideBackColor;				//���鱳����ɫ
		int SlideBorderColor;			//����߿���ɫ
		ImageResId SlideKey;			//������ʽͼƬ

		bool ShowScale;					//��ʾ�̶�
		int ScalePos;					//�̶�λ��	0���Ϸ�/�󷽣� 1���·�/�ҷ���
		Point AxisPos;					//��������
		int PrimaryScaleNum;			//���̶���
		int PrimaryScaleLen;			//���̶ȳ���
		int SecondaryScaleNum;			//���̶���
		int SecondaryScaleLen;			//���̶ȳ���
		BaseLine ScaleLineStyle;		//��������
		vector<Project::ScaleInfo> ScaleInfo;	//���עλ��

		bool ShowMark;					//��ʾ���ֱ�ע
		int Act;						//����λ
		int Bct;						//С��λ
		StringStyle MarkStyle;			//���ֱ�ע��ʽ

		bool ShowPer;					//��ʾ�ٷֱ�
		StringStyle PerStyle;			//�ٷֱȱ�ע��ʽ

		bool ShowAxis;					//��ʾ����

		NoticesAction ResBef;			//д��ǰ֪ͨ
		NoticesAction ResAft;			//д���֪ͨ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(ReadVar, DataStyle, AttrMax, AttrMaxVar, AttrMin, AttrMinVar,
				Dir, MinScale, UseIncDecPerClick, IncDecPerClickScale, UseChangeInput, SlideRailPos,
				SlideRailWidth, SlideRailHeight, SlideRailBackColor, SlideRailBorderColor, SlideRailKey,
				SlideWidth, SlideHeight, SlideBackColor, SlideBorderColor, SlideKey, ScaleInfo,
				ShowScale, ScalePos, AxisPos,PrimaryScaleNum, PrimaryScaleLen, SecondaryScaleNum, SecondaryScaleLen,
				ScaleLineStyle, ShowMark, Act, Bct, MarkStyle, ShowPer, PerStyle, ShowAxis, ResBef, ResAft,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

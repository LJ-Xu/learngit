/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ChineseInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-10-19
 * Descript : ���ڴ�����������ؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "InputDataShowUnit.h"
#include "NoticesAction.h"
namespace Project
{
	class ChineseInputConfig : public MoveableDisplayUnit
	{
	public:
		DataVarId WriteVar;										//д�Ĵ���
		int PswMode;											//������ʽ
		InputMethod KeyMethod;									//��������
		NoticesAction ResBef;									//д��ǰ֪ͨ
		NoticesAction ResAft;									//д���֪ͨ
		UI::CodeFormatUtility::CodeRule Rule;					//�����ʽ
		ImageResId Key;											//��ȡͼƬ��keyֵ
		StringStyle TextStyle;									//������ʽ
		string Text;											//��ʼ���ִ�
		InputSequence Sequence;									//�������
		map<int, vector<WinCtrlID>> SequenceGroup;				//���������

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, PswMode, KeyMethod, Sequence, SequenceGroup,
				ResBef, ResAft, Rule, Key, TextStyle, Text, 
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};
}
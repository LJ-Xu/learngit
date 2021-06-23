/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ����ֵ�������ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputNumShowUnit.h"
#include "MacroRes.h"
#include "NoticesAction.h"
namespace Project
{
	class DigitalInputConfig : public InputNumShowUnit
	{
	public:
		bool RwDiffAddrUsed;				//��дʹ����ͬ�Ĵ���
		DataVarId WriteVar;					//д��Ĵ���
		InputSequence Sequence;				//�������
		InputMethod KeyMethod;				//�������
		map<int, vector<WinCtrlID>> SequenceGroup/* =
		{
			{1,{65536,65537,65538}}
		}*/;//���������
		bool IsInputConvertUsed;			//�Ƿ�ʹ���������ת��
		DataVarOrManual NovUpperSrcInput;	//����ת����������Դ����
		DataVarOrManual NovLowerSrcInput;	//����ת����������Դ����
		DataVarOrManual NovUpperResInput;	//����ת������ת��ֵ����
		DataVarOrManual NovLowerResInput;	//����ת������ת��ֵ����

		NoticesAction ResBef;				//д��ǰ֪ͨ
		NoticesAction ResAft;				//д���֪ͨ
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, WriteVar, Sequence, KeyMethod, IsInputConvertUsed,
				NovUpperSrcInput, NovLowerSrcInput, NovUpperResInput, NovLowerResInput, ResBef, ResAft,
				PswMode, IsFillZero, Accuracy, IsWarnUpperUsed, WarnUpper, SequenceGroup,
				IsWarnLowerUsed, WarnLower, IsWarnStyleUsed, WarnStyleUpper, WarnStyleLower,
				IsConvertUsed, NovUpperSrc, NovLowerSrc, NovUpperRes, NovLowerRes,
				ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}
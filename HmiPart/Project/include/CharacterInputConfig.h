/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ���ַ������ؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "InputDataShowUnit.h"
#include "MacroRes.h"
#include "NoticesAction.h"
#include "InputNumShowUnit.h"

namespace Project
{
	class CharacterInputConfig : public InputDataShowUnit
	{
	public:
		DataVarId WriteVar;					//д��Ĵ���
		InputSequence Sequence;				//�������
		map<int, vector<WinCtrlID>> SequenceGroup; 
		InputMethod KeyMethod;				//��������
		NoticesAction ResBef;				//д��ǰ֪ͨ
		NoticesAction ResAft;				//д���֪ͨ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, Sequence, KeyMethod, ResBef, ResAft, SequenceGroup,
				PswMode, ByteConversion, Rule, ReadVar, Key, TextStyle, FmtStr,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};

}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : InputDataUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ��������ڴ�Ż��Ʋ�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ReadableDisplayUnit.h"
#include "InputDataShowUnit.h"
#include "InputNumShowUnit.h"
#include "NoticesAction.h"
#include "MacroRes.h"

namespace Project
{
	class InputDataUnit	: public InputDataShowUnit
	{
	public:
		bool RwDiffAddrUsed;				//��дʹ����ͬ�Ĵ���
		DataVarId WriteVar;					//д��Ĵ���
		InputSequence Sequence;				//�������
		InputMethod KeyMethod;				//��������
		NoticesAction ResBef;				//д��ǰ֪ͨ
		NoticesAction ResAft;				//д���֪ͨ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(RwDiffAddrUsed, WriteVar, Sequence, KeyMethod, ResBef, ResAft);
		}
	};
}

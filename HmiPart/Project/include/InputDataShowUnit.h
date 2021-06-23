/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : InputDataShowUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "ReadableDisplayUnit.h"
#include "CodeFormatUtility.h"
namespace Project
{
	

	class InputDataShowUnit : public ReadableDisplayUnit
	{
	public:
		int PswMode;			//�Ƿ�ʹ�������ʽ
		int ByteConversion;		//�ߵ��ֽ�ת��
		UI::CodeFormatUtility::CodeRule Rule;			//�����ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(PswMode, ByteConversion, Rule);
		}
	};
}


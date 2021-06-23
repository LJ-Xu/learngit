/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BaseOpUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
namespace Project
{
	class BaseOpUnit : public MoveableDisplayUnit
	{
	public:
		bool IsRecord;					//�Ƿ��¼
		DataVarId WriteVar;				//д��Ĵ���
		DataVarId Delay;				//��ʱ����ʱ�� 
		int AcceptTime = 20;			//ȷ�ϵȴ�ʱ��

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, Delay, AcceptTime);
		}
	};

}
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
		bool IsRecord;					//是否记录
		DataVarId WriteVar;				//写入寄存器
		DataVarId Delay;				//延时动作时间 
		int AcceptTime = 20;			//确认等待时间

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(WriteVar, Delay, AcceptTime);
		}
	};

}
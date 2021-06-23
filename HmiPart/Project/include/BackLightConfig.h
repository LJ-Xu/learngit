/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightConfig.h
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : 用于存放背景灯控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "MultiPilotLampConfig.h"
namespace Project
{
	class BackLightConfig :public MoveableDisplayUnit
	{
	public:
		int Reg;								//启用背景灯条件 0:字 1:位
		DataVarId TrigVar;						//触发地址
		int BitTriggerCond;						//位触发条件 0:ON状态 1:OFF状态
		double ComPareValue;					//比较数值
		Conditions ValueTrigCond;				//数值触发条件
		int Act;								//0:熄灭 1:唤醒
	
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Reg, TrigVar, BitTriggerCond, ComPareValue, ValueTrigCond, Act,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}

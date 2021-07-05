/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerConfig.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : 用于存放定时器控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum TimerStartCond :char { StartBitChange, StartValueChange, TimerStartWin, StartCloseWin };
	enum TimerStopCond :char { StopCloseWin, StopReachTime, StopBitChange };

	class TimerConfig :public MoveableDisplayUnit
	{
	public:
		int CycTime;							//周期时间
		bool UseDelay;							//是否使用延迟
		int DelayExecutionCycle;				//延迟周期
		DataVarId MarkVar;						//定时器执行标志位
		TimerStartCond StartConditions;			//定时器触发条件
		DataVarId StartVar;						//触发地址
		int BitTriggerCond;						//位触发条件 0:上升沿 1:下降沿
		bool UseValueEqual;						//是否使用数值相等
		double ComPareValue;					//比较数值

		TimerStopCond EndConditions;			//定时器结束条件
		DataVarId EndVar;						//结束地址
		int BitEndCond;							//位结束条件 0:上升沿 1:下降沿

		int PreconditionConstant;				//预设时间
		DataVarId PreconditionVar;				//预设时间地址

		DataVarId PresetTimeNotifyVar;			//定时器达到预设时间通知
		int NotifyMode;							//通知模式 0:置ON，1:置OFF
		DataVarId AlreadTimeVar;				//已记时间
		bool UseReset;							//是否使用重置位
		DataVarId ResetVar;						//重置位寄存器
		int ResetCond;							//重置位条件 0:ON 1:OFF
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CycTime, UseDelay, DelayExecutionCycle, MarkVar, StartConditions, 
				StartVar, BitTriggerCond, UseValueEqual, ComPareValue, EndConditions, 
				EndVar, BitEndCond, PreconditionConstant, PreconditionVar, PresetTimeNotifyVar,
				NotifyMode, AlreadTimeVar, UseReset, ResetVar, ResetCond,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}


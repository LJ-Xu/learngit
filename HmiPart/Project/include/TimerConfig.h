/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TimerConfig.h
 * Author   : qiaodan
 * Date     : 2021-04-01
 * Descript : ���ڴ�Ŷ�ʱ���ؼ���ز�������
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
		int CycTime;							//����ʱ��
		bool UseDelay;							//�Ƿ�ʹ���ӳ�
		int DelayExecutionCycle;				//�ӳ�����
		DataVarId MarkVar;						//��ʱ��ִ�б�־λ
		TimerStartCond StartConditions;			//��ʱ����������
		DataVarId StartVar;						//������ַ
		int BitTriggerCond;						//λ�������� 0:������ 1:�½���
		bool UseValueEqual;						//�Ƿ�ʹ����ֵ���
		double ComPareValue;					//�Ƚ���ֵ

		TimerStopCond EndConditions;			//��ʱ����������
		DataVarId EndVar;						//������ַ
		int BitEndCond;							//λ�������� 0:������ 1:�½���

		int PreconditionConstant;				//Ԥ��ʱ��
		DataVarId PreconditionVar;				//Ԥ��ʱ���ַ

		DataVarId PresetTimeNotifyVar;			//��ʱ���ﵽԤ��ʱ��֪ͨ
		int NotifyMode;							//֪ͨģʽ 0:��ON��1:��OFF
		DataVarId AlreadTimeVar;				//�Ѽ�ʱ��
		bool UseReset;							//�Ƿ�ʹ������λ
		DataVarId ResetVar;						//����λ�Ĵ���
		int ResetCond;							//����λ���� 0:ON 1:OFF
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


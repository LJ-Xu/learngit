/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BackLightConfig.h
 * Author   : qiaodan
 * Date     : 2021-04-03
 * Descript : ���ڴ�ű����ƿؼ���ز�������
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
		int Reg;								//���ñ��������� 0:�� 1:λ
		DataVarId TrigVar;						//������ַ
		int BitTriggerCond;						//λ�������� 0:ON״̬ 1:OFF״̬
		double ComPareValue;					//�Ƚ���ֵ
		Conditions ValueTrigCond;				//��ֵ��������
		int Act;								//0:Ϩ�� 1:����
	
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Reg, TrigVar, BitTriggerCond, ComPareValue, ValueTrigCond, Act,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}

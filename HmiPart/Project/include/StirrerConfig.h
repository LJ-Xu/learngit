/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : �������ؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
#include <vector>

namespace Project 
{
#define MINRUNSPEED 10
#define MAXRUNSPEED 100
	class StirrerConfig : public MoveableDisplayUnit
	{
	public:
		StirrerConfig() { }
		~StirrerConfig() { }

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVar, Logic, Dir, Speed, SpVar, Keys,
					VOffX, VOffY, Perm, Width, Height,
				X, Y, IsEnable, CtrlName, IsRecord, CtrlName);
		}

	public:
		DataVarId ReadVar;	// ��ȡ�Ĵ���
		int Logic = 0;		// �������߼�
		int Dir = 0;		// ����������
		int Speed = -1;		// �����ٶ�
		DataVarId SpVar;	// ���ټĴ���
		vector<ImageResId> Keys;
		int CurrentStatus = 0;
	};
}


/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyboardRes.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "Point.h"
namespace Project
{
	class KeyboardRes
	{
	public:
		Point KeypadPos;	//���̵�λ��
		int KeypadIdx;		//���̵�����

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(KeypadPos, KeypadIdx);
		}

	};
}

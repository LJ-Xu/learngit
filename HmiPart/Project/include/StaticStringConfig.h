/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ�ž�̬���ֿؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "StatusRes.h"
#include "StringStyle.h"
#include "BorderRes.h"
#include "BaseLine.h"
#include "Point.h"
namespace Project
{
	class StaticStringConfig : public MoveableDisplayUnit
	{
	public:
		StatusRes Text;
		StringStyle Css;
		//int alpha;      //͸����
		BorderRes Border;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, Css, Border, VOffX, VOffY, Perm, Width,
				Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}

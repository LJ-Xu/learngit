/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : VarStringConfig.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ڴ�Ŷ�̬���ֿؼ���ز�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
//#include "VarStringRes.h"
#include "StringStyle.h"
//#include "BorderRes.h"
#include "BaseLine.h"
#include "StatusRes.h"
namespace Project
{
	struct VarStringStyle
	{
		double SrcId;
		StatusRes Text;			//����
		StringStyle Css;		//������ʽ
		BaseLine BorderStyle;	//�߿���ʽ

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(SrcId, Text, Css, BorderStyle);
		}
	};
	class VarStringConfig : public MoveableDisplayUnit
	{
	public:
		vector<VarStringStyle> StatusStyle;		//ÿ��״̬����ʽ
		int Count;								//״̬��
		DataVarId SrcIdRef;						//ָ��״̬�ļĴ���

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StatusStyle, Count, SrcIdRef,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



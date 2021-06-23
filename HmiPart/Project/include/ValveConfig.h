/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : ValveConfig.h
 * Author   : TangYao
 * Date     : 2020/12/19
 * Descript : ���ſؼ�������
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once

#include "ImageResId.h"
#include "BtnUnit.h"
#include "KeypadConfig.h"
#define TEMPRWDIF 1
namespace Project 
{
	class ValveConfig : public BtnUnit
	{
	public:
		ValveConfig() { }
		~ValveConfig() { }

	public:
		template<class Archive>
		void serialize(Archive & archive) {
			archive(ReadVar, WriteVar, Act, Logic, Dir, Speed, SpVar, PicKey,
					VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable,InnerRectX,InnerRectY,InnerRectW,InnerRectH,IsRecord,CtrlName
//#if TEMPRWDIF
//#else
//				, IsRWDiff
//#endif
			);
		}

	public:
//#if TEMPRWDIF
//#else
//		bool IsRWDiff;
//#endif
		DataVarId ReadVar;		// ��ȡ�Ĵ���
		DataVarId WriteVar;		// д��Ĵ���
		BtnAction Act = BtnSETON;	// ���Ŷ��� 0:��ͨ	1:�ر�	2:��ͨ/�ر�(ȡ��)	3:��ѹ��ͨ
		int Logic = 0;			// �����߼� 0:���߼�	1:���߼�
		int Dir = 0;			// �������� 0:����	1:����
		int Speed = -1;			// �����ٶ�
		DataVarId SpVar;		// ���ټĴ���
		int InnerRectX;
		int InnerRectY;
		int InnerRectW;
		int InnerRectH;
	};
}

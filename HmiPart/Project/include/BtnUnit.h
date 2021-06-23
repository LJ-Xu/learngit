/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : BtnUnit.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "StatusRes.h"
#include "StringStyle.h"
#include "MoveableDisplayUnit.h"
#include "ImageResId.h"
namespace Project
{
	class BtnUnit : public MoveableDisplayUnit
	{
	public:
		int StatusCnt;						//״̬����
		vector<ImageResId> PicKey;			//״̬��Ӧ���
		StatusRes Txt;						//��Ӧ����
		vector<StringStyle> StrStyles;		//������ʽ
		DataVarId WriteVar;					//д��Ĵ���

		int CurrentStatus = 0;				//��ŵ�ǰ״̬(1:ON/0:OFF״̬)
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(StatusCnt, PicKey, Txt, StrStyles, WriteVar);
		}
	};
}

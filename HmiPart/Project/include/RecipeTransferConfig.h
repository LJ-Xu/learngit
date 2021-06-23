/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferConfig.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : �䷽�������ڴ�Ż��Ʋ�������
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BtnUnit.h"
namespace Project
{
	enum RecipeTransferAction { DownLoadToPLC, UpLoadFromPLC, RecipeReg };
	class RecipeTransferConfig : public BtnUnit
	{
	public:
		int Action;								//�������� 0:���� 1:�ͷ�
		RecipeTransferAction TransmitMode;		//���䷽��
		vector<DataVarId> TransmitVarIdRefs;	//���ƴ��䷽��Ĵ���
		vector<int> TriggerFunc;				//���ƴ��䷽�� 0:������	1:�½���
		string RecipeId;							//�䷽��ID
		int RecipeIndexRow;						//�䷽��������
		int RecipeCount;						//����
		DataVarId PlcVarIdRef;					//PLCԴ��ַ
		DataVarId CompleteVarIdRef;				//�䷽������ɱ�־

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Action, TransmitMode, TransmitVarIdRefs, RecipeId,
				RecipeCount, PlcVarIdRef, CompleteVarIdRef, RecipeIndexRow,
				StatusCnt, PicKey, Txt, StrStyles, WriteVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};
}
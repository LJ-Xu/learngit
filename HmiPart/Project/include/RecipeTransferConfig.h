/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RecipeTransferConfig.h
 * Author   : qiaodan
 * Date     : 2021-03-02
 * Descript : 配方传输用于存放绘制参数的类
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
		int Action;								//按键动作 0:按下 1:释放
		RecipeTransferAction TransmitMode;		//传输方向
		vector<DataVarId> TransmitVarIdRefs;	//控制传输方向寄存器
		vector<int> TriggerFunc;				//控制传输方向 0:上升沿	1:下降沿
		string RecipeId;							//配方表ID
		int RecipeIndexRow;						//配方表索引行
		int RecipeCount;						//字数
		DataVarId PlcVarIdRef;					//PLC源地址
		DataVarId CompleteVarIdRef;				//配方传输完成标志

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
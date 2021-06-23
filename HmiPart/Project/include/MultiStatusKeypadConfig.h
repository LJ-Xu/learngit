#pragma once
#include "MoveableDisplayUnit.h"
#include "BtnUnit.h"
#include "HImage.h"
namespace Project
{
	struct StatusAction {
		int Id;		//状态
		string Val;	//设置值
		ImageResId PicKey; //状态对应外观
		StringStyle StrStyles;//文字样式
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Id, Val, PicKey, StrStyles);
		}
	};

	class MultiStatusKeypadConfig : public MoveableDisplayUnit
	{
	public:
		vector<StatusAction> Action;		//动作设置
		int DataVarType;					//0 : 位寄存器
											//1 : 字寄存器
		int BitNum;
		vector<DataVarId> RegVars;			//寄存器Vid
		int StatusCnt;						//状态个数
		StatusRes Txt;						//对应文字
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Action, DataVarType, RegVars, StatusCnt, Txt, BitNum,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



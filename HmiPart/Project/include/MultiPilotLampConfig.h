#pragma once
#include "MoveableDisplayUnit.h"
#include "StringStyle.h"
#include "StatusRes.h"
#include "ImageResId.h"
#include "JudgeCondition.h"

namespace Project
{
	struct GetNumWay
	{
		int Mode;				//获取方式	0:常数
								//			1:寄存器获取
		double Value;
		DataVarId RegVar;		//对比寄存器Vid

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Mode, Value, RegVar);
		}
	};
	enum Logic
	{
		NONE,
		AND,
		OR
	};
	struct StatusCondition
	{
		int Type;					//0:范围	1:位
		/*范围条件*/
		Conditions ConditionA;		//条件A
		Conditions ConditionB;		//条件B
		Logic LogicCondition;		//AB之间条件
		GetNumWay DataA;			//数据A
		GetNumWay DataB;			//数据B
		/*位条件*/
		int Offset;					//偏移量
		int Trigger;				//0:ON  1:OFF	(0-15)
		/*动作*/
		bool Flash;					//是否闪烁
		float Speed;					//闪烁速度

		StringStyle Css;			//文字样式
		ImageResId StatusKey;		//图片资源
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Type, ConditionA, ConditionB, LogicCondition, Offset,
				Trigger, Flash, Speed, DataA, DataB, Css, StatusKey);
		}
	};
	class MultiPilotLampConfig : public MoveableDisplayUnit
	{
	public:
		int DataVarType;					//寄存器模式		0：字寄存器
											//					1：位寄存器

		vector<DataVarId> RegVars;			//位、字寄存器Vid(最后一个是字)
		int StatusCnt;						//状态总数
		int BitNum;							//多位组合位数
		int DataFormat;						//0:有符号 1:无符号
		StatusRes Text;						//文字内容

		vector<StatusCondition> Action;		//状态对应
		StatusCondition ErrState;			//错误状态
		int InputErr;						//非法输入处理方式 0:显示错误状态 1:显示当前状态 
		bool UseErrNotice;					//是否使用错误通知
		DataVarId InputErrVarRef;			//错误通知寄存器

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Text, StatusCnt, BitNum, Action, DataVarType, RegVars,
				ErrState, InputErr, InputErrVarRef, DataFormat, UseErrNotice,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}

	};

}


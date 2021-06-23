#pragma once
#include "BtnFunctionRes.h"
#include "MultiPilotLampConfig.h"
#include "MoveableDisplayUnit.h"
namespace Project
{
	enum ActionMode {Startframe, CloseFrame, CoilTrigger, FuncTimer, FuncContinue, FirstDownload, FirstPowerON, NumLogic};
	class RegionFunctionConfig :public MoveableDisplayUnit
	{
	public:
		ActionMode Mode;			//触发动作模式
		DataVarId CoilVar;			//线圈触发的线圈地址
		int TriggerMode;			//线圈触发方式	0：上升沿 1：下降沿 
		double Time;				//定时时间
		DataVarId TimeLmtVar;		//定时/连续模式线圈限置
		int StopMode;				//定时/连续模式线圈限置模式 0:OFF,1:ON
		DataVarId TimeValueVarId;	//定时显示计时时间
		DataVarId ValueVarId;		//数值逻辑条件获取值寄存器
		double Value;				//数值逻辑对比值
		Conditions NumlogicCond;	//数值逻辑条件
		vector<BtnFunctionRes> Functions;				//功能集

		DataVarId FirstDownLoadVar;	//下载后第一次扫描标志
		DataVarId PowerOnVar;		//开机后第一次扫描标志

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Mode, CoilVar, TimeValueVarId, ValueVarId, Value, NumlogicCond, PowerOnVar,
				TriggerMode, TriggerMode, Time, TimeLmtVar, StopMode, Functions, FirstDownLoadVar,
				VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



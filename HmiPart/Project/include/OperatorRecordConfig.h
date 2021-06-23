#pragma once
#include "AlarmDisplayConfig.h"
#include "MoveableDisplayUnit.h"
//#include "StringStyle.h"
#include "AlarmInfoRes.h"
#include <time.h>
using namespace std;
namespace Project
{
	struct OperationRecordPrint
	{
		bool IsEnableRecordSetPrint;			//是否启用打印功能
		int PrintDirection;						//打印方向 0:水平 1:垂直
		StringStyle PrintFontStyle;				//打印字体设置
		int PrintRange;							//打印范围 0:时间 1:笔数
		DDWORD PrintStartTime;					//起始时间
		DDWORD PrintEndTime;					//结束时间
		int PrintCount;							//打印数量（笔数）	
		DataVarId ControlAddress;				//控制打印地址PSB
		int PrintCond;							//打印条件 0:上升沿 1:下降沿
		bool IsDisplayPrintDate;				//是否显示打印时间
		TimeMode PrintTimeFormate;				//打印时间时间格式
		DateMode PrintDateFormate;				//打印时间日期格式
		double Scaling;							//画面缩放比例
		vector<string> PrintItems;				//打印项目
		int PrintSortMode;						//排序 0:时间顺序 1:时间逆序
		DateMode PrintItemDateFormate;			//打印内容时间格式
		TimeMode PrintItemTimeFormate;			//打印内容日期格式

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(IsEnableRecordSetPrint, PrintDirection, PrintFontStyle, PrintRange, PrintStartTime, PrintCond,
				PrintEndTime, PrintCount, ControlAddress, IsDisplayPrintDate, PrintTimeFormate, PrintDateFormate,
				Scaling, PrintItems, PrintSortMode, PrintItemDateFormate, PrintItemTimeFormate);
		}
		void Parse(rapidjson::Value& jsonObj);

	};
	class OperatorRecordConfig : public MoveableDisplayUnit
	{
	public:
		bool Enable;							//是否启用操作记录录入
		DataVarId CmdIdRef;						//命令寄存器
		DataVarId ResultIdRef;					//结果寄存器
		DataVarId ControlIdRef;					//控制寄存器 0:关闭 1:开启
		SaveFileRes SaveLst;					//操作记录录入信息保存
		OperationRecordPrint PrintSet;			//打印设置
		DataVarId RecordNoticeVarId;			//操作记录通知寄存器

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Enable, CmdIdRef, ResultIdRef, ControlIdRef, SaveLst, RecordNoticeVarId,
				PrintSet, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};
}



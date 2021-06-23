#pragma once
#include <string>
#include <vector>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "DataVarInfo.h"
#include "MultiPilotLampConfig.h"
#include "SaveFileRes.h"
#include "StatusRes.h"
#include "VarStringConfig.h"
#include <time.h>
//#ifdef WIN32
//#include <windows.h>
//#include <winsock.h>
//#else
//#include <sys/time.h>
//#endif
using namespace std;
//#ifdef WIN32
//
//static int gettimeofday(timeval *tp, void *tzp)
//{
//	time_t clock;
//	struct tm tm;
//	SYSTEMTIME wtm;
//
//	GetLocalTime(&wtm);
//	tm.tm_year = wtm.wYear - 1900;
//	tm.tm_mon = wtm.wMonth - 1;
//	tm.tm_mday = wtm.wDay;
//	tm.tm_hour = wtm.wHour;
//	tm.tm_min = wtm.wMinute;
//	tm.tm_sec = wtm.wSecond;
//	tm.tm_isdst = -1;
//	clock = mktime(&tm);
//	tp->tv_sec = clock;
//	tp->tv_usec = wtm.wMilliseconds * 1000;
//	return (0);
//}
//#endif

namespace Project
{
	struct SearchTime
	{
		time_t tv_sec;
		long long tv_usec;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(tv_sec, tv_usec);
		}
		
	};

	struct AlarmDisPlayInfo
	{
		Project::WinCtrlID CtrlId;			//报警显示控件ID
		int GroupLowerLmt;				//显示报警组范围下限
		int GroupUpperLmt;				//显示报警组范围上限
		bool NeedNotify;				//是否需要通知
	};

	struct AlarmWinInfo
	{
		Project::WinCtrlID CtrlId;			//报警弹窗控件ID
		int GroupName;					//报警组
		int GroupNo;					//报警编号
		bool NeedNotify;				//是否需要通知

		int InfoMode;					//0:恢复	1:报警
	}; 
	enum AlarmLevel { LOW, NOEMAL, HIGH, URGENT };	//低、正常、高、紧急

	struct AlarmContent
	{
		int GroupName;
		int GroupNo;
		AlarmLevel Category;	//紧急级别
		SearchTime TriggerTime;		//报警触发时间
		string Msg;				//报警信息
		int Count;				//报警次数
		SearchTime ConfirmTime;		//报警确认时间
		SearchTime RecoverTime;		//报警恢复时间
		bool Confirmed = false;			//报警已确认
		bool Recovered = false;			//报警已恢复
	};

	struct AlarmMonitorUnit
	{
		std::string VariableName;
		int MonitorType;//0监控地址，1动态文字串
		DataVarId MonitorAddrRef;//指定状态的寄存器
		vector<VarStringStyle> StatusStyle;//{MonitorType=1}每个状态的样式
		int AddrType;//{MonitorType=0}0数字1字符2中文
		bool IsPreLead;//{MonitorType=0}{AddrType=0}前导
		int Intgerdigits;//{MonitorType=0}{AddrType=0}整数位
		int Decimaldigits;//{MonitorType=0}{AddrType=0}小数位	
		int EncodeMode;//{MonitorType=0}{AddrType!=0}0:utf-8;1:utf-16;2:ascii,3:gb2312
		void Parse(rapidjson::Value& jsonObj);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(VariableName, MonitorType, MonitorAddrRef, StatusStyle, AddrType, IsPreLead, Intgerdigits, Intgerdigits, Decimaldigits, EncodeMode);
		}
	};

	struct AlarmInfoContext
	{
		std::vector<std::pair<std::string,int>> AlarmContentList;//没有地址对应时int为-2，否则为MonitorUnits对应下标
		std::vector<AlarmMonitorUnit> MonitorUnits;
		void Parse(rapidjson::Value& jsonObj);
		void Parse(const char* StrContent);
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(AlarmContentList, MonitorUnits);
		}
	};

	enum BitConditions:int	{ ON, OFF, FALL, RISE	};
	struct AlarmInfoRes
	{
		int GroupNo;						//报警组次编号
		int GroupName;						//报警分组名称
		string GroupSecName;				//报警组名称
		AlarmLevel Category;				//紧急级别

		int RegType;						//0:位寄存器	1：字寄存器
		DataVarId ReadRegVar;				//读取寄存器地址
		Conditions BtyeCondition;			//字寄存器触发条件
		BitConditions BitCondition;			//位寄存器触发条件

		float ComPareValue;					//对比值
		DataVarId ComPareVar;				//对比寄存器地址
		StatusRes Text;						//报警信息
		int UseBeep;						//是否使用蜂鸣器 0:否 1:是
		int BeepTimeOut;					//蜂鸣器超时时间
		int Count = 0;						//用来记录报警次数

		bool UseAlarmPopWin;				//是否使用报警弹窗	
		int PopWinNo;							//报警弹窗编号
		int PopMode;						//弹窗模式 0:弹出一次 1:周期弹出
		int PopTime;						//弹出周期
		bool IsCloseWin;					//报警结束后关闭窗口
		bool ChangeFlag = false;
		char TempByteState=-1;//临时保存位状态
		AlarmInfoContext Context;//监控内容
		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(GroupNo, GroupName, Category, RegType, ReadRegVar,
				BtyeCondition, BitCondition, ComPareValue, ComPareVar, Text, UseBeep, BeepTimeOut,
				UseAlarmPopWin, PopWinNo, PopMode, PopTime, IsCloseWin, Context);
		}

		void Parse(rapidjson::Value& jsonObj);
		static void Parse(std::vector<AlarmInfoRes>& vector, rapidjson::Value& jsonObj);
	};
}



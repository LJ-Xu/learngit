#pragma once
#include <string>
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "BaseRecord.h"
namespace Storage
{
	enum RecordEnum
	{
		HR_DATA_TOGGLE,//开关
		HR_DATA_SET,  //设置
		HR_WINDOW_CLOSE,//窗体关闭
		HR_WINDOW_OPEN, //窗体打开
	};
	struct HistoryRecord : BaseRecord
	{
		RecordEnum RecdTp;//记录类别
		char Name[15];
		Project::DataVarId VarId;//变量信息
		Project::WinCtrlID WCId ;
		DDWORD val1;
		DDWORD val2;
	};

}
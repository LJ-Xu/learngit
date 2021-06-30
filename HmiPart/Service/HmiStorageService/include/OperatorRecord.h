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

	enum OperatorAction :int
	{
		OA_SET,		// 设置值
		OA_TOGGLE,	// 位取反
		OA_CONST,	// 设置常数
		OA_STRING,	// 设置字符串
		OA_PREVWIN,	// 跳转上一窗口
		OA_NEXTWIN,	// 跳转下一窗口
		OA_UPLOAD,	//上传配方
		OA_DOWNLOND,	//下载配方
		OA_PRESS,		//按下
		OA_RELEASE		//释放
	};

	struct OperatorRecord : BaseRecord
	{
		size_t ID;			// 记录编号
		uint64_t Date;		// 操作日期
		uint64_t Time;		// 操作时间
		string UserName;	// 用户名称
		string Class;		// 操作权限
		size_t Window;		// 操作窗口
		string CtrlName;	// 控件名称
		string Comment;		// 对象描述
		string Action;		// 操作动作
		string Address;		// 操作地址
		string Information;	// 操作信息

		/*
		RecordEnum RecdTp;//记录类别
		//char Name[15];//用户名称
		unsigned int Tick;//s

		int UID;//用户ID
		Project::DataVarId VarId;//变量信息
		Project::WinCtrlID WCId ;
		DDWORD val1;//old val
		DDWORD val2;//new val
		*/
	};

}
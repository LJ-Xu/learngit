#pragma once
#include "stdafx.h"
#include "DataVarInfo.h"
#include "UIDataService.h"
#include "DataFormate.h"
using namespace Project;
namespace UI
{
	class UIData
	{
	public:
		template<typename T>
		static T Number(Project::DataVarId& vid)
		{
			XJDataType tp = UIDataService::Ins().GetDataType(vid);
			DDWORD da = UIDataService::Ins().GetNumber(vid);
			return Utility::DataFormate::GetNumber<T>(da, tp);
			// return UIDataService::Ins().GetNumber<T>(vid);
		}
		template<typename T>
		static void Number(Project::DataVarId& vid, T val)
		{
			XJDataType tp = UIDataService::Ins().GetDataType(vid);
			DDWORD da = Utility::DataFormate::SetNumber<T>(val, tp);
			UIDataService::Ins().SetNumber(vid, da);
			// UIDataService::Ins().SetNumber(vid, val);
		}
		static bool Bit(Project::DataVarId& vid);

		static void Bit(Project::DataVarId& vid, bool val);
	};

#define SYS_PSW_TIME_DEC				TP_SPSW,16				//十进制系统时间
#define SYS_PSW_TIME_HEX				TP_SPSW,23				//十六进制系统时间
#define SYS_PSW_CURRENT_WINNO			TP_SPSW,30				//当前窗口号
#define SYS_PSW_INPUTKEY_LOWERLMT		TP_SPSW,100				//输入键盘显示下限（占用4寄存器，8btye）
#define SYS_PSW_INPUTKEY_UPPERERLMT		TP_SPSW,104				//输入键盘显示上限（占用4寄存器，8btye）
#define SYS_PSW_INPUTKEY_CURVAL			TP_SPSW,108,16*2		//输入键盘实时显示内容
#define SYS_PSW_PINYIN_CURVAL			TP_SPSW,124,8*2			//当前拼音
#define SYS_PSW_LOGINName				TP_SPSW,132,8*2			//存放登陆用户名
#define SYS_PSW_LOGINPwd				TP_SPSW,140,8*2			//存放登陆密码
#define SYS_PSW_LOGINFlag				TP_SPSW,148				//登陆返回状态标志位 0：未操作 1：登陆成功 2：登陆失败 3：注销成功 4：注销失败
#define SYS_PSW_LoginedName				TP_SPSW,150,8*2			//当前以登陆用户
#define SYS_PSW_ConfirmOperate			TP_SPSW,158				//操作确认 0:超时 1:触发 2:确认操作 3：取消操作
#define SYS_PSW_OperateCtrlID           TP_SPSW,160				//执行操作确认的控件ID
#define SYS_PSW_ConfirmTime				TP_SPSW,162				//操作确认等待时间
#define SYS_PSW_RecipeIndex				TP_SPSW,256				//配方索引

#define SYS_PINYIN_PAGENUM				25900			//拼音界面编号
#define SYS_FULLKEY_PAGENUM				25007			//全拼键盘编号

#define SYS_PSB_HASLOGINUSER			TP_SPSB,0		//用户权限登录标志位
#define SYS_PSB_COMMPORT_FAIL			TP_SPSB,23		//通讯失败
#define SYS_PSB_DATASAMPLE_NOTICE		TP_SPSB,100		//采集数据更新通知
#define SYS_PSB_ALARMRECORD_NOTICE		TP_SPSB,101		//报警数据更新通知
#define SYS_PSB_LOGINUSER				TP_SPSB,102		//login动作标志位
#define SYS_PSB_LOGOUTUSER				TP_SPSB,103		//logout动作标志位
#define SYS_PSB_OperatorRECORD_NOTICE	TP_SPSB,104		//操作记录数据更新通知
#define SYS_PSB_BEEP_DISABLE			TP_SPSB,105		//蜂鸣器是否使能 ON：不使能 OFF：使能

//#define SYS_PSW_OperatorRECORD_CONTRONL	TP_SPSB,106		//操作记录控制标志位(目前没用)
#define SYS_PSB_ReciprRECORD_NOTICE		TP_SPSB,106		//配方记录更新通知，取反
#define SYS_PSB_CSV_NOTICE				TP_SPSB,107		//CSV正在导入导出通知 0:未在进行 1:进行中
#define SYS_PSB_ClearFlag				TP_SPSB,204		//清除SPSB7、8标志位（第一次下载和上电后标志位）

}

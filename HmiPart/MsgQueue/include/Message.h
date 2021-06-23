/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : Message.h
 * Author   : zhanglinbo
 * Date     : 2020-9-16
 * Descript : //TODO
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/


#pragma once

#include <functional>
#include "stdafx.h"
namespace Message
{
#define GET_MESSAGE_CODE(msg) (((msg)&0x7FF)|0xF000)

	//typedef void(*InvokeCBFunc)(void* param);
	typedef std::function<void(void*parma)> InvokeCBFunc;
	struct  InvokeCBInfo
	{
		InvokeCBFunc Func;
		void*        Param;
	};
	struct  _AutoFreeMsg
	{
		void(*Delete)(void*);
		void*        Ptr;
	};
	struct Msg
	{
	public:

		/*******************************************************************************
		 * Name     : Create
		 * Descript : 创建消息
		 * Input    : code     -  enum MsgCode
		 *		      hWnd     -
		 *			  wparam   -
		 *			  lparam   -
		 * Output   : return
		 * Note	    :
		 *******************************************************************************/
		//static Msg* Create(int code, HWND hWnd, WPARAM wparam, LPARAM lparam);//创建消息
		//static int SendMsg(Msg& msg);
		//static int SendMsg(HWND winHd, HWND ctrHd, int message, LPARAM param, int exparam);
		//static void Dispose(Msg* msg);
		int GetCode()
		{
			return GET_MESSAGE_CODE(Code);
		}
		int Code;   //消息 ID 号。
		HWND   Hwnd;   // 消息的窗口句柄
		WPARAM WParam; //消息的参数1   //BaseControl  
		LPARAM LParam; //消息的参数2   //具体参数   DavaVarInfo*
		int    Data; //消息的返回值。
		//char buf[0];
	};



}
//class DataChangeMessage
//{
//public:
//	//BaseVar* Base;  
//	long long   oldVal;
//	long long   newVal;
//	long long    tick;
//};

#define MESSAGE_IS_AUTOFREE(msg) ((msg) & 0x800)
#define MAKE_AUTOFREE_MESSAGE(msg) ((msg) | 0x800)

#define WM_EVENT_BASE          0xF000//WM_USER			 //0x0400
#define WM_EVENT_READY         WM_EVENT_BASE+1		//数据发送更新
#define WM_EVENT_DATEUPDATE    WM_EVENT_BASE+2		//数据发送更新
#define WM_EVENT_TIMETICK      WM_EVENT_BASE+3		//定时功能
#define WM_EVENT_TIMEUP        WM_EVENT_BASE+4		//定时时间到
#define WM_EVENT_OPEN		   WM_EVENT_BASE+5		//打开键盘
#define WM_EVENT_CLOSE         WM_EVENT_BASE+6		//关闭键盘
#define KEY_EVENT_NUM		   WM_EVENT_BASE+7		//按键数字
#define KEY_EVENT_CHAR		   WM_EVENT_BASE+8		//按键字符
#define WM_CHANGE_PERM		   WM_EVENT_BASE+9		//用户登陆发生变化

#define BTN_GET_CHINESE		   WM_EVENT_BASE+10		//获取中文更改按键
#define BTN_SET_CHINESE		   WM_EVENT_BASE+11		//确认当前输入中文
#define KEY_EVENT_PINYIN	   WM_EVENT_BASE+13		//搜索拼音改变

#define KEY_EVENT_ESC		   WM_EVENT_BASE+12		//退出
#define KEY_EVENT_FUNCTION	   WM_EVENT_BASE+14		//功能按键
#define KEY_EVENT_ENTER		   WM_EVENT_BASE+15		//回车按键

#define BTN_SET_ENABLE		   WM_EVENT_BASE+16		//回车按键

#define KEY_EVENT_CHINESE	   WM_EVENT_BASE+17		//确定数值消息
#define KEY_EVENT_VALUE		   WM_EVENT_BASE+18		//确定数值消息
#define KEY_EVENT_KEYMSG	   WM_EVENT_BASE+19		//keyctrl传递消息

#define AlARM_EVENT_DISPLAY	   WM_EVENT_BASE+20		//报警显示注册
#define AlARM_EVENT_WIN		   WM_EVENT_BASE+21		//报警弹窗注册
#define AlARM_EVENT_CHANGE	   WM_EVENT_BASE+22		//报警更新

#define SAMPLE_EVENT_START	   WM_EVENT_BASE+23		//开始采样
#define SAMPLE_ALAEM_CLOCK	   WM_EVENT_BASE+24		//到达采样开始/停止时间
#define TREND_SAMPLE_CHANGE	   WM_EVENT_BASE+25		//趋势图更新
#define WM_EVENT_INVOKE  	   WM_EVENT_BASE+26		//UI线程执行消息
#define WM_EVENT_UI_FUNC 	   WM_EVENT_BASE+27		//UI执行

#define WM_EVENT_CLICKED 	   WM_EVENT_BASE+28		//Click确认事件


#define WM_EVENT_MAX 		WM_EVENT_BASE+ 100
/*
#define KEY_EVENT_Q			  WM_EVENT_BASE+7
#define KEY_EVENT_W 		  WM_EVENT_BASE+8
#define KEY_EVENT_E			  WM_EVENT_BASE+9
#define KEY_EVENT_R			  WM_EVENT_BASE+10
#define KEY_EVENT_T			  WM_EVENT_BASE+11
#define KEY_EVENT_Y			  WM_EVENT_BASE+12
#define KEY_EVENT_U			  WM_EVENT_BASE+13
#define KEY_EVENT_I			  WM_EVENT_BASE+14
#define KEY_EVENT_O			  WM_EVENT_BASE+15
#define KEY_EVENT_P			  WM_EVENT_BASE+16
#define KEY_EVENT_A			  WM_EVENT_BASE+17
#define KEY_EVENT_S			  WM_EVENT_BASE+18
#define KEY_EVENT_D			  WM_EVENT_BASE+19
#define KEY_EVENT_F			  WM_EVENT_BASE+20
#define KEY_EVENT_G			  WM_EVENT_BASE+21
#define KEY_EVENT_H			  WM_EVENT_BASE+22
#define KEY_EVENT_J			  WM_EVENT_BASE+23
#define KEY_EVENT_K			  WM_EVENT_BASE+24
#define KEY_EVENT_L			  WM_EVENT_BASE+25
#define KEY_EVENT_Z			  WM_EVENT_BASE+26
#define KEY_EVENT_X			  WM_EVENT_BASE+27
#define KEY_EVENT_C			  WM_EVENT_BASE+28
#define KEY_EVENT_V			  WM_EVENT_BASE+29
#define KEY_EVENT_B			  WM_EVENT_BASE+30
#define KEY_EVENT_N			  WM_EVENT_BASE+31
#define KEY_EVENT_M			  WM_EVENT_BASE+32
*/

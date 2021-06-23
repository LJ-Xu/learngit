/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : KeyMsgControl.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于键盘消息控件的控制的头文件，创建按键消息绘制，传递处理消息
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BaseControl.h"
#include "KeyMsgView.h"
#include "DataVarInfo.h"
#include "InputNumShowUnit.h"
namespace UI
{
#define KEY_DEL		   8		//回退按键
#define KEY_ESC		   27		//退出按键
#define KEY_CAPS	   42		//切换大小写按键
#define KEY_ENTER	   13		//回车按键
#define KEY_MINUS	   44		//取相反数
#define KEY_CLR		   127 		//清除
#define KEY_DOT		   46		//小数点按键
	

	class KeyMessage
	{
	public:
		int SrcWinNo;		//源目标windowNo
		int DstWinNo;		//目标windowNo
		Project::WinCtrlID DstCtrlId;
		Project::WinCtrlID SrcCtrlId;
		Project::WinCtrlID PinYinCtrlId;	//拼音窗口keyId
		Project::WinCtrlID InputCtrlId;	//输入框ctrlId
		int InputType;		//输入框类型	0：数值
							//				1：字符
							//				2：中文
		int KeypadPosx;		//键盘窗口x位置
		int KeypadPosy;		//键盘窗口y位置

		float UpperLmt;		//上限值
		float LowerLmt;		//下限值
		string FmtStr;		//输入框输入格式
		Project::AccuracyParam Accuracy;		//数值输入框精度管理
		string Value;		//传输按键值(包括pinyin)
		string Chinese;		//中文字符
	};

	class KeyMsgControl : public BaseControl
	{
	public:
		KeyMsgControl() :KeyMsgControl(nullptr) {}
		KeyMsgControl(HMIPage* );
		void CreateView()override;

		/*******************************************************************************
		 * Name     : HandleKeyMessage
		 * Descript : 处理接收的按键消息
		 * Input    : keyValue - 按键内容
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void HandleKeyMessage(int keyValue);

		void OpenKeyWindow(int lparam);
		void InitKeyMsg(int lparam);
		void HandleKeyChar(int lparam);
		void HandleKeyNum(int lparam);
		void HandleChangePinyin(int lparam);
		KeyMessage CurrentMsg;		//存放当前按键信息
	protected:
		int PeekHMIMessage(Message::Msg* msg) override;
	private:
		std::shared_ptr<KeyMsgModel> mode_;
	};
}


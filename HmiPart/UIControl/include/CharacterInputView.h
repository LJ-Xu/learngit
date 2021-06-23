/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : CharacterInputView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符输入的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <FL/Fl_Input.H>
#include <stdafx.h>
#include "DigitalInputView.h"
namespace UI
{
	class CharacterInputView : public InputView
	{
	public:
		CharacterInputView(int X, int Y, int W, int H, const char* l);
		~CharacterInputView();
		BasicView BaseView;
		string DisplayValue;
		char *WriteValue;
		void FoucsNextSequence();
		void InitStartInput();			//开始输入
		void EndInput();				//结束输入
		void SetWriteData();
		bool HandleOperatePerm();
		bool HandleOperateConfirm();

		//bool OpenKeypage = false;		//是否打开键盘页面
		//size_t MaxSize = 16;
		/*******************************************************************************
		 * Name     : handle
		 * Descript : 重写Fl_Input中关于事件响应处理函数
		 * Input    : event	- 事件标号
		 * Output   : 1 - 处理事件成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
		string Inputstring;
	protected:
		void draw() override;

	};
}

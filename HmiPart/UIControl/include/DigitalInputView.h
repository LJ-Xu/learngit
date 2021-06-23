/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
#include <FL/Fl_Shared_Image.H>
 * Name     : DigitalInputView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符输入框的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include "DigitalDisplayView.h"
namespace UI
{
	
	class DigitalInputView : public InputView
	{
	public:
		DigitalInputView(int X, int Y, int W, int H, const char* l);
		~DigitalInputView();
		void FoucsNextSequence();
		void InitStartInput();			//开始输入
		void EndInput();				//结束输入
		bool HandleOperatePerm(); 
		bool HandleOperateConfirm();
		void EnterValue();

		/*******************************************************************************
		 * Name     : handle
		 * Descript : 重写Fl_Input中关于事件响应处理函数
		 * Input    : event	- 事件标号
		 * Output   : 1 - 处理事件成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);

		BasicView BaseView;
		string DisplayValue;
		bool IsExceedMaxLmt = false;	//超出最大值
		bool IsExceedMinLmt = false;	//超出最小值
		bool IsHideText = false;		//超出限值闪烁

		string OldValue;				//用来存放按键输入前数据
		double MaxValue;
		double MinValue;
		double InputValue = 0;
	protected:
		void draw() override;
	};
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于字符显示框的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Input.H>

//#include "DigitalInputView.h"
namespace UI
{
	class InputView : public HMIBaseInput
	{
	public:
		InputView(int X, int Y, int W, int H, const char* l) :HMIBaseInput(X, Y, W, H, l) {};
		~InputView() {};

		/*******************************************************************************
		 * Name     : drawtext
		 * Descript : 用于绘制输入框及输入框背景、文字
		 * Input    : X		-	 输入框左上角x坐标
		 *			  Y		-	 输入框左上角y坐标
		 *			  W		-	 输入框宽度
		 *			  H		-	 输入框长度
		 *			  align	-	 输入框文字显示对齐方式，数值参照Fl_Align
		 *			  img	-	 输入框背景图片资源
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void drawtext(int X, int Y, int W, int H, int align, Fl_Image *img, bool usepassword = false, bool hidetext = false);
		const char* expand(const char*, char*, char *) const;
		double expandpos(const char*, const char*, const char*, int*) const;
		bool OpenKeypage = false;				//是否打开键盘页面
		bool NeedHandleFocus = false;					//是否需要处理焦点
//		DataVarId BitVid = DataVarId::NullId;
		//bool IsSearchFirst = false;						//是否在进行搜寻
	};

	class DigitalDisplayView: public InputView
	{
	public:
		DigitalDisplayView(int X, int Y, int W, int H, const char* l);
		~DigitalDisplayView();
		BasicView BaseView;
		string DisplayValue;
		bool IsExceedMaxLmt = false;	//超出最大值
		bool IsExceedMinLmt = false;	//超出最小值
		bool IsHideText = false;		//超出限值闪烁
		/*******************************************************************************
		 * Name     : handle
		 * Descript : 重写Fl_Input中关于事件响应处理函数
		 * Input    : event	- 事件标号
		 * Output   : 1 - 处理事件成功
		 * Note	    :
		 *******************************************************************************/
		int handle(int event);
	protected:
		void draw() override;
	};
}

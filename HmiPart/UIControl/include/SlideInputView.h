#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : SlideInputView.h
 * Author   : qiaodan
 * Date     : 2020-12-17
 * Descript : 关于滑动输入的绘制头文件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scrollbar.H>
#include "Point.h"
namespace UI
{
	//void scroll_cb(Fl_Widget*, void*);	// hscrollbar callback
	class SlideInputView : public HMIBaseSlider
	{
	public:
		SlideInputView(int X, int Y, int W, int H, const char* l);
		~SlideInputView();

		/*初始化绘制使用的一些参数*/
		void InitDraw();
		/*绘制刻度及标注*/
		void DrawScale();
		void DrawXScale();
		void DrawYScale();

		/*转换标注格式*/
		string ChangeDisplayFormat(double value,int mode);
		/*将数值写入到寄存器*/
		void SetReadVar();
		bool HandleOperatePush();
		int handle(int event);
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		/*绘制使用常量*/
		Fl_Color slideRailBackColor_;		//滑轨背景颜色
		Fl_Color slideRailFrameColor_;		//滑轨边框颜色
		Fl_Color slideBackColor_;			//滑块背景颜色
		Fl_Color slideFrameColor_;			//滑块边框颜色
		Project::Point axispos_;			//轴线位置
		int axislength_;					//轴线长度
		Fl_Color axisColor_;				//轴线颜色
		int primaryScaleLen_;				//主刻度长度
		int secondScaleLen_;				//副刻度长度		
		Fl_Font scalefont_;					//标注字体
		Fl_Color scalecolor_;				//标注颜色
		int scalesize_;						//标注大小
		int slideW_;						//滑块宽/高
	};
}





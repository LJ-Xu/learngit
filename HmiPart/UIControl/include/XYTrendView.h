#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : XYTrendView.h
 * Author   : qiaodan
 * Date     : 2021-03-12
 * Descript : 关于XY趋势图的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include "SampleInfoRes.h"
#include "XYTrendConfig.h"
#include "Point.h"
namespace UI
{
	//void XYTrendScroll_cb(Fl_Widget*, void*);	// hscrollbar callback
	class XYTrendView : public HMIBaseGroup
	{
	public:
		XYTrendView(int X, int Y, int W, int H, const char* l);
		~XYTrendView();

		/*存放通道数据*/
		vector<vector<DDWORD>> XData;		//X坐标数据
		vector<vector<DDWORD>> YData;		//Y坐标数据
		/*******************************************************************************
		 * Name     : InitDraw
		 * Descript : 初始化绘制使用的一些参数
		 * Input    :
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void InitDraw();
		/*绘制X轴、X轴刻度及X轴标注*/
		void DrawXaxis();
		/*绘制Y轴、Y轴刻度及Y轴标注*/
		void DrawYaxis();
		/*绘制栅格*/
		void DrawGrid();
		/*绘制XY折线图*/
		void DrawChannelXYTrend(Project::XYLineChannel channelinfo, int startnum, int num);
		/*绘制点*/
		void DrawDot(Project::DotStyle style, int x, int y, int size);
		/*绘制参考线*/
		void DrawReferenceLine();
		/*转换标注格式*/
		string ChangeDisplayFormat(float value, Project::ScaleMark style);
		//int handle(int event);
		BasicView BaseView;

		void DrawRenderChart();
	protected:
		void draw() override;
	private:
		/*绘制使用常量*/
		Fl_Color chartBgColor_;		//折线图图背景颜色
		Fl_Color scaleBgColor_;		//刻度区背景颜色
		bool useGrid;				//是否使用栅格
		bool useXAxis;				//是否使用X轴
		int xscalefontSize_;		//X轴标注字体大小
		int xscalefontStyle_;		//X轴标注字体格式
		Fl_Color xscalefontColor_;	//X轴标注字体颜色
		int xscaleInterval_;		//x轴标注间隙
		int xscaleNum = 0;			//X轴当前标注数

		int yscalefontSize_;		//Y轴标注字体大小
		int yscalefontStyle_;		//Y轴标注字体格式
		Fl_Color yscalefontColor_;	//Y轴标注字体颜色

		bool useYAxis;				//是否使用Y轴
		int xaxislength;					//X轴(滚动条)长度
		int yaxislength;			//Y轴长
		Project::Point origin;		//原点坐标
		int scrollheight_;			//滚动条高度

		Fl_Color currentdotColor_;	//刻度区背景颜色

		int perScreendot_;			//每屏点数
	};
}

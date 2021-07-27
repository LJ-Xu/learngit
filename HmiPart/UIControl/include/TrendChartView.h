#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartView.h
 * Author   : qiaodan
 * Date     : 2020-11-16
 * Descript : 关于趋势图的绘制头文件，绘制趋势图
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Scrollbar.H>
#include "SampleInfoRes.h"
#include "TrendChartConfig.h"
#include "Point.h"
#include "SampleStorage.h"
typedef std::chrono::milliseconds MilliSecs;

namespace UI
{
	void TrendChartScrollCb(Fl_Widget*, void*);	// hscrollbar callback
	class TrendChartView : public HMIBaseGroup
	{
	public:
		TrendChartView(int X, int Y, int W, int H, const char* l);
		~TrendChartView();
		int DisplayMode;				//0:正常模式 1:查询模式
		int Xaxislength;				//X轴(滚动条)长度
		int ScrollClick = 0;				//滚动条是否点击标志
		bool UseInfoDisplay;

		Fl_Scrollbar* Hscrollbar;		//滚动条

		/*存放通道数据*/
		vector<Storage::SampleRecord> ChannelData;
		//vector<vector<Project::SampleChannel::SampleContent>> SampleData;
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
		void DrawRenderXaxis();			/*绘制状态下x轴坐标*/
		/*绘制每屏点数模式下x轴时间标注*/
		void DrawXScaleDot(int dot, DDWORD time);
		/*绘制Y轴、Y轴刻度及Y轴标注*/
		void DrawYaxis();
		/*绘制Y轴标注*/
		void DrawYaxisScale();
		void DrawEveryYaxisScale(DOUBLE max, DOUBLE min, vector<Project::ScaleInfo>& info);
		/*绘制栅格*/
		void DrawGrid();
		int GetChannelNo(int no);
		/*绘制趋势图*/
		void DrawChannelLine(Project::TrendLineType type, double x, double y);
		void DrawChannelTrend(Project::TrendChannel channelinfo);
		void DrawSearchTrend(Project::TrendChannel channelinfo);
		/*绘制显示信息*/
		void DrawInfoDisplay();


		/*确定信息显示线磁吸位置*/
		void SetDisPlayPos(int num,int dx);
		
		/*******************************************************************************
		 * Name     : GetLimitValue
		 * Descript : 获取绘制时限值
		 * Input    : channelNo - 通道编号
		 *			  max - 最大值
		 *			  min - 最小值
		 * Output   :
		 * Note	    :
		 *******************************************************************************/
		void GetLimitValue(int channelNo, DOUBLE &max, DOUBLE &min);
		/*转换Y轴标注格式*/
		string ChangeDisplayFormat(DOUBLE value);
		/*******************************************************************************
		 * Name     : GetTimeString
		 * Descript : 获取X轴标注转换后字符串
		 * Input    : time - 需要转换的时间
		 *			  mode - 1:时间	0:日期
		 *			  style - 日期/时间的格式
		 * Output   : string - 时间按格式转换后的字符串
		 * Note	    :
		 *******************************************************************************/
		string GetTimeString(DDWORD time, int mode, int style);
		/*获取数据Y轴偏移值*/
		int GetYCoordinate(DDWORD& data, XJDataType& info, DOUBLE max, DOUBLE min);
		/*设置翻页*/
		void SetCurrentPage(int pagenum);
		/*设置缩放*/
		void SetZoomParam(int zoom);

		void DrawRenderTrend();
		int handle(int event);
		BasicView BaseView;
	protected:
		void draw() override;
	private:
		/*绘制使用常量*/
		Fl_Color trendBgColor_;		//趋势图背景颜色
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
		int perScreendot_;			//每屏点数
		int perScreenPeriod_;	    //每屏时间段
		int yaxislength;			//Y轴长
		Project::Point origin;		//原点坐标
		int scrollheight_;			//滚动条高度

		vector<Storage::SampleRecord> currentContent_;
		vector<string> title_;			//标题内容
		Fl_Color infoBgColor_;			//信息窗口背景颜色
		Fl_Color infofontcolor_;		//信息窗口字体颜色
		Fl_Color infolinecolor_;		//信息数据线颜色
		int displaylinex_;				//信息线x轴坐标
		int displayNo_ = 0;				//信息线所在点
		int linex_;

		int tableX_;
		int tableY_;
		int tableW_;
		int tableH_;
		int rows_;						//行数
		int rowheight_;
		int columns_;					//列数
		vector<int> colwidth_;


		int scalecurrentnum_;			//滚动条点击时数据容量
		DDWORD scalecurrenttime_;
		int currentnum_;				//当前数据容量
		DDWORD firstStartTime_;
		DDWORD firstEndTime_;
		DDWORD startTime_ = 0;		//正常采样下开始时间
		DDWORD clickstarttime_ = 0;	//点击后显示时间
		bool hasfirstStartTime_ = false;

		bool hasxscaledot = false;
	};
}



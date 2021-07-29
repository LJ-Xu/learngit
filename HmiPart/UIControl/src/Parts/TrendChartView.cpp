/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TrendChartView.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 绘制趋势图
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "TrendChartControl.h"
#include "TrendChartView.h"
#include "TrendChartModel.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iomanip>
#include "UIDataService.h"
#include "IResourceService.h"
#include "DataInfo.h"
#include "UIData.h"
#include "CodeFormatUtility.h"
#include "ViewShowUtility.h"
namespace UI
{
	TrendChartView::TrendChartView(int X, int Y, int W, int H, const char* L) : HMIBaseGroup(X, Y, W, H, L) {
		Hscrollbar = new Fl_Scrollbar(0,0, 16, 10);
		Fl_Group::end();		// end the group's begin()
	}
	TrendChartView::~TrendChartView() {}

	/*初始画绘制参数*/
	void TrendChartView::InitDraw()
	{
		std::shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		trendBgColor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.BgColor));
		scaleBgColor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.ScaleAreabgColor));
		useGrid = model->ChartTrendConfig.IsGriddisplay;
		useXAxis = model->ChartTrendConfig.AxisX.IsdisplayScale;
		useYAxis = model->ChartTrendConfig.AxisY.IsdisplayScale;
		xscalefontSize_ = model->ChartTrendConfig.AxisX.ScaleStyle.Font.Size;
		xscalefontStyle_ = UI::IResourceService::GetFontIdx(model->ChartTrendConfig.AxisX.ScaleStyle.Font.Name);
		xscalefontColor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleStyle.Colors));
		
		xscaleInterval_ = model->ChartTrendConfig.XaxisLen / model->ChartTrendConfig.AxisX.MainScaleNum;

		yscalefontSize_ = model->ChartTrendConfig.AxisY.ScaleStyle.Font.Size;
		yscalefontStyle_ = UI::IResourceService::GetFontIdx(model->ChartTrendConfig.AxisY.ScaleStyle.Font.Name);
		yscalefontColor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisY.ScaleStyle.Colors));
		perScreendot_ = model->ChartTrendConfig.PerScreendot;			//每屏点数
		perScreenPeriod_ = model->ChartTrendConfig.PerScreenPeriod;				//每屏时间段
		Xaxislength = model->ChartTrendConfig.XaxisLen;
		yaxislength = model->ChartTrendConfig.YaxisLen;
		origin.X = model->ChartTrendConfig.OriginPos.X;
		origin.Y = model->ChartTrendConfig.OriginPos.Y;
		scrollheight_ = model->ChartTrendConfig.ScrollHeight;

		//if (model->ChartTrendConfig.IsDataDisplay)

		tableW_ = 0;
		//确定信息显示框行列
		rows_ = model->ChartTrendConfig.DisplayContent.size() + 1;
		columns_ = 4;
		//确定行高
		fl_font(0, 14);
		rowheight_ = fl_height() + 2 * fl_descent();
		tableH_ = rowheight_ * (model->ChartTrendConfig.DisplayContent.size() + 1);
		//确定列宽和标题
		vector<int>().swap(colwidth_);
		colwidth_.push_back(rowheight_);
		vector<string>().swap(title_);
		title_.push_back("");
		tableW_ += rowheight_;

		string title = "描述内容";
		UI::IResourceService::GB2312toUtf8(title);
		colwidth_.push_back((int)fl_width(title.c_str(), strlen(title.c_str())));
		title_.push_back(title);
		tableW_ += (int)fl_width(title.c_str(), strlen(title.c_str()));

		title = "坐标范围";
		UI::IResourceService::GB2312toUtf8(title);
		colwidth_.push_back((int)fl_width(title.c_str(), strlen(title.c_str())));
		title_.push_back(title);
		tableW_ += (int)fl_width(title.c_str(), strlen(title.c_str()));

		title = "当前值";
		UI::IResourceService::GB2312toUtf8(title);
		title_.push_back(title);
		int colwidth = (int)fl_width(title.c_str(), strlen(title.c_str()));
		//colwidth_.push_back(colwidth);
		tableW_ += colwidth;

		string datestring = "日期";
		UI::IResourceService::GB2312toUtf8(datestring);
		string timestring = "时间";
		UI::IResourceService::GB2312toUtf8(timestring);

		for (unsigned i = 0; i < model->ChartTrendConfig.DisplayContent.size(); i++)
		{
			UI::IResourceService::GB2312toUtf8(model->ChartTrendConfig.DisplayContent[i].DescribeName);
			if (model->ChartTrendConfig.DisplayContent[i].DescribeName == datestring)
			{
				//title_.push_back(datestring);
				switch (model->ChartTrendConfig.DisplayDateStyle)
				{
				case Project::MDYSlash:
				case Project::DMYSlash:
				case Project::YMDSlash:
					colwidth = (int)fl_width("25\\11\\2020", strlen("25\\11\\2020"));
					break;
				case Project::YMDChinese:
				{
					string date = "xxxx年xx月xx日";
					UI::IResourceService::GB2312toUtf8(date);
					colwidth = (int)fl_width(date.c_str(), strlen(date.c_str()));
					break;
				}
				default:
					break;

				}
			}
			else if (model->ChartTrendConfig.DisplayContent[i].DescribeName == timestring)
			{
				//title_.push_back(timestring);
				switch (model->ChartTrendConfig.DisplayTimeStyle)
				{
				case Project::HMS:
				{
					if (colwidth < (int)fl_width("xx:xx:xx", strlen("xx:xx:xx")))
						colwidth = (int)fl_width("xx:xx:xx", strlen("xx:xx:xx"));
				}
				break;
				case Project::HM:
					if (colwidth < (int)fl_width("xx:xx", strlen("xx:xx")))
						colwidth = (int)fl_width("xx:xx", strlen("xx:xx"));
					break;
				case Project::HMSMS:
					if (colwidth < (int)fl_width("xx:xx:xx:xxx", strlen("xx:xx:xx:xxx")))
						colwidth = (int)fl_width("xx:xx:xx:xxx", strlen("xx:xx:xx:xxx"));
					break;
				case Project::HMSChinese:
				{
					string time = "xx时xx分xx秒";
					UI::IResourceService::GB2312toUtf8(time);
					if (colwidth < (int)fl_width(time.c_str(), strlen(time.c_str())))
						colwidth = (int)fl_width(time.c_str(), strlen(time.c_str()));
					break;
				}
				default:
					break;
				}
				
			}
			
		}
		colwidth_.push_back(colwidth);
		tableW_ += colwidth;
		//确定信息显示框背景颜色
		infoBgColor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.InfoBgColor));			//信息窗口背景颜色
		infofontcolor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.InfoFontColor));		//信息窗口字体颜色
		//确定信息显示线颜色
		infolinecolor_ = fl_rgb_color(RGBColor(model->ChartTrendConfig.InfoLineColor));

	}

	/********************************事件处理***************************************/
	/*事件处理（鼠标拖动）*/
	int TrendChartView::handle(int event)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();

		switch (event) {
		case FL_MOVE:
		{
			linex_ = Fl::event_x();
			int liney = Fl::event_y();
			if (linex_ <= origin.X + model->ChartTrendConfig.OffX + Xaxislength && 
				linex_ > origin.X + model->ChartTrendConfig.OffX
				&& liney <= origin.Y + model->ChartTrendConfig.OffY - scrollheight_ &&
				liney > origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength)
			{
				/*取定显示框坐标*/
				if (model->ChartTrendConfig.IsDataDisplay)
				{
					if (linex_ <= origin.X + model->ChartTrendConfig.OffX + Xaxislength / 2)
						tableX_ = origin.X + model->ChartTrendConfig.OffX;
					else
						tableX_ = origin.X + model->ChartTrendConfig.OffX + Xaxislength - tableW_;
					if (liney <= origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength / 2)
						tableY_ = origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength;
					else
						tableY_ = origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - tableH_;
				}
				UI::ViewShowUtility::ShowView(this, model->ChartTrendConfig.Perm,
					model->ChartTrendConfig.X + model->ChartTrendConfig.OffX,
					model->ChartTrendConfig.Y + model->ChartTrendConfig.OffY);
			}
			break;
		}
		default:
			return Fl_Group::handle(event);
		}
		return 1;
	}
	/*滚动条回调*/
	void TrendChartScrollCb(Fl_Widget* w, void* data) {
		TrendChartView* o = (TrendChartView*)data;
		if (o->Hscrollbar->value() != o->Xaxislength)
			o->ScrollClick = 1;
		else
			o->ScrollClick = 0;
		o->redraw();
		printf("value = %d\n", o->Hscrollbar->value());
	}
	/*******************************************************************************/

	/**********************************参数获取*************************************/
	/*转换数据显示格式*/
	string TrendChartView::ChangeDisplayFormat(DOUBLE value)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		string data = to_string(value);
		string intString, floatString;
		int pos = data.find(".");
		if (pos == 0)	//小数点在头部
		{
			intString = "0";
			floatString = data.substr(pos + 1, data.size());
		}
		else if (pos == -1)
		{
			intString = data;
			floatString = "";
		}
		else
		{
			intString = data.substr(0, pos);
			floatString = data.substr(pos + 1, data.size());
		}
		//去除头部多余0和增加小数点前0
		int f = atoi(intString.c_str());
		intString = to_string(f);
		if (floatString.size() < (unsigned)model->ChartTrendConfig.AxisY.YDecimalnNum)
		{
			unsigned size = (unsigned)model->ChartTrendConfig.AxisY.YDecimalnNum - floatString.size();
			for (unsigned i = 0; i < size;i++)		
				floatString += "0";
		}
		else
			floatString = floatString.substr(0, model->ChartTrendConfig.AxisY.YDecimalnNum);
		if (intString.size() > (unsigned)model->ChartTrendConfig.AxisY.YIntegerNum)
		{
			string maxString;
			for (size_t i = 0; i < (size_t)model->ChartTrendConfig.AxisY.YIntegerNum; i++)
				maxString += "*";
			if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::PercentSacle)		//百分比
				maxString += "%";
			return maxString;
		}
		if (model->ChartTrendConfig.AxisY.YDecimalnNum)
		{
			if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::PercentSacle)		//百分比
				return intString + "." + floatString + "%"; 
			else if(model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::NumSacle)
				return intString + "." + floatString;
		}
		else
		{
			if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::PercentSacle)		//百分比
				return intString + "%";
			else if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::NumSacle)
				return intString;
		}
		return " ";
	}
	/*将时间转换成格式字符串*/
	string TrendChartView::GetTimeString(DDWORD time, int mode, int style)
	{

		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		char displayTime[100];
		string text = "";
		time_t tmSec = time / 1000;
		int tmMs = (int)(time % 1000);
		struct tm *local;
		local = localtime(&tmSec);
		if (!local)
			return text;
		if (mode)	//时间
		{
			switch (style)
			{
			case Project::HMS:
				sprintf(displayTime, "%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			case Project::HM:
				sprintf(displayTime, "%d:%d", local->tm_hour, local->tm_min);
				break;
			case Project::HMSMS:
				sprintf(displayTime, "%d:%d:%d:%d", local->tm_hour, local->tm_min, local->tm_sec, tmMs);
				break;
			case Project::HMSChinese:
				sprintf(displayTime, "%d时%d分%d秒", local->tm_hour, local->tm_min, local->tm_sec);
				break;
			default:
				break;
			}
		}
		else    //日期
		{
			switch (style)
			{
			case Project::MDYSlash:
				sprintf(displayTime, "%d\/%d\/%d", local->tm_mon + 1, local->tm_mday, local->tm_year + 1900);
				break;
			case Project::DMYSlash:
				sprintf(displayTime, "%d\/%d\/%d", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
				break;
			case Project::YMDChinese:
				sprintf(displayTime, "%d年%d月%d日", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			case Project::YMDSlash:
				sprintf(displayTime, "%d\/%d\/%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
				break;
			default:
				break;
			}
		}
		text = displayTime;
		UI::IResourceService::GB2312toUtf8(text);
		return text;
	}
	/*获取点的Y轴坐标偏移*/
	int TrendChartView::GetYCoordinate(int channelNo, DDWORD& data, XJDataType& info, DOUBLE max, DOUBLE min)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		
		Utility::ScaleInfo scale;
		scale.A = (DOUBLE)(yaxislength) / (max - min);
		scale.B = 0 - (scale.A * min);
		int ydata = Utility::DataFormate::GetScale(scale, data, info);
		
		int datamax = 0, datamin = 0;
		XJDataType tp;
		tp.Cls = Project::VarDataType::DT_DDWORD;
		tp.Type = Project::VarNumberType::NT_Float;
		for (size_t i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
		{
			if (channelNo == model->ChartTrendConfig.SetChannel[i].ChannelNo)
			{
				/*计算通道数据Y轴偏移*/
				DDWORD maxda, minda;
				memcpy(&maxda, &model->ChartTrendConfig.SetChannel[i].TrendMax, sizeof(DDWORD));
				memcpy(&minda, &model->ChartTrendConfig.SetChannel[i].TrendMin, sizeof(DDWORD));

				datamax = Utility::DataFormate::GetScale(scale, maxda, tp);
				datamin = Utility::DataFormate::GetScale(scale, minda, tp);
				if (ydata > datamax)
					return datamax;
				else if (ydata < datamin)
					return datamin;
				else
					return ydata;
			}
		}
		return ydata;
	}
	/*确定信息显示线位置*/
	void TrendChartView::SetDisPlayPos(int num, int dx)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		//displayNo_ = 0;
		if (num == 0 && linex_ - (origin.X + model->ChartTrendConfig.OffX) <= dx)
		{
			displayNo_ = num;
			displaylinex_ = dx;                 //画线在第一个点
			currentContent_.push_back(ChannelData[displayNo_]);
		}
		else if (num == ChannelData.size() - 1)
		{
			if (linex_ - (origin.X + model->ChartTrendConfig.OffX) >= dx)
			{
				displayNo_ = num;
				displaylinex_ = dx;             //画线在最后一个点
				currentContent_.push_back(ChannelData[displayNo_]);

			}
		}
		else
		{
			int dnextx;
			if (model->ChartTrendConfig.PerSceenViewMode)
				dnextx = (int)((ChannelData[num + 1].Date - startTime_) * (DDWORD)Xaxislength / (DDWORD)perScreenPeriod_);
			else
				dnextx = (num + 1) * (Xaxislength / (perScreendot_ - 1));

			if (linex_ - (origin.X + model->ChartTrendConfig.OffX) >= dx && 
				linex_ - (origin.X + model->ChartTrendConfig.OffX) <= dnextx)
			{
				if (linex_ - (origin.X + model->ChartTrendConfig.OffX) - dx > 
					dnextx - (linex_ - (origin.X + model->ChartTrendConfig.OffX)))        //偏向dnextx
				{
					displayNo_ = num + 1;
					displaylinex_ = dnextx;
					currentContent_.push_back(ChannelData[displayNo_]);
				}
				else {
					displayNo_ = num;
					displaylinex_ = dx;
					currentContent_.push_back(ChannelData[displayNo_]);
				}
			}
		}
	}
	/*根据翻页页码获取显示内容*/
	void TrendChartView::SetCurrentPage(int pagenum)
	{
		if (pagenum <= 0) pagenum = 1;		//最小从第一页起
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		if (model->ChartTrendConfig.PerSceenViewMode)		//时间段
		{
			DDWORD starttime = (pagenum - 1) * perScreenPeriod_ + firstStartTime_;
			if (starttime <= startTime_)	//超出当前页面
			{
				if ((scalecurrenttime_ - (DDWORD)perScreenPeriod_ - firstStartTime_) == 0)
					return;
				int scrollbarvalue = (int)((starttime - firstStartTime_) * (DDWORD)((Hscrollbar->maximum() - Hscrollbar->minimum()))
					/ (scalecurrenttime_ - (DDWORD)perScreenPeriod_ - firstStartTime_));
				Hscrollbar->value(scrollbarvalue);
				ScrollClick = 1;
				redraw();
			}
			else
			{
				Hscrollbar->value(Xaxislength);
				ScrollClick = 0;
				redraw();
			}
		}
		else
		{
			int startnum = (pagenum - 1) * perScreendot_;
			//int num = 
			if (startnum <= scalecurrentnum_ - perScreendot_ )
			{
				if (scalecurrentnum_ - perScreendot_ == 0)
					return;
				int scrollbarvalue = (int)((double)startnum * (Hscrollbar->maximum() - Hscrollbar->minimum())
					/ (double)(scalecurrentnum_ - perScreendot_));
				Hscrollbar->value(scrollbarvalue);
				ScrollClick = 1;
				redraw();
			}
			else
			{
				Hscrollbar->value(Xaxislength);
				ScrollClick = 0;
				redraw();
			}

		}
	}
	/*设置缩放比例*/
	void TrendChartView::SetZoomParam(int zoom)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		zoom = zoom < 10 ? 10 : zoom;
		zoom = zoom > 1000 ? 1000 : zoom;
		perScreendot_ = (int)((float)model->ChartTrendConfig.PerScreendot * (float)100.0 / (float)zoom );
		perScreenPeriod_ = (int)((float)model->ChartTrendConfig.PerScreenPeriod * (float)100.0 / (float)zoom);
		firstEndTime_ = firstStartTime_ + perScreenPeriod_;
		redraw();
	}
	/*获取通道最大最小值*/
	void TrendChartView::GetLimitValue(int channelNo, DOUBLE &max, DOUBLE &min)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		shared_ptr<TrendChartControl> ctrl = BaseView.GetControl<TrendChartControl>();
		
		switch (model->ChartTrendConfig.AxisY.ScaleRange)
		{
		case Project::Custom:
			max = model->ChartTrendConfig.AxisY.YScaleMax;
			min = model->ChartTrendConfig.AxisY.YScaleMin;
		break;
		case Project::Channel:
			channelNo = model->ChartTrendConfig.AxisY.UseRangeChanbelNo;
		case Project::All:
		{
			size_t num;
			for (size_t i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
			{
				if (GetChannelNo(channelNo) == model->ChartTrendConfig.SetChannel[i].ChannelNo)
					num = i;
			}
			if (num < model->ChartTrendConfig.SetChannel.size())
			{
				//if (model->ChartTrendConfig.SetChannel[num].TrendMaxVarId != Project::DataVarId::NullId)
				//	max = UI::UIData::Number<DOUBLE>(model->ChartTrendConfig.SetChannel[num].TrendMaxVarId);
				//else
					max = model->ChartTrendConfig.SetChannel[num].TrendMax;
				//if (model->ChartTrendConfig.SetChannel[num].TrendMinVarId != Project::DataVarId::NullId)
				//	min = UI::UIData::Number<DOUBLE>(model->ChartTrendConfig.SetChannel[num].TrendMinVarId);
				//else
					min = model->ChartTrendConfig.SetChannel[num].TrendMin;
			}
			break;
		}
		//case Project::Channel:
		//{
		//	size_t num;
		//	for (size_t i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
		//	{
		//		if (model->ChartTrendConfig.AxisY.UseRangeChanbelNo == model->ChartTrendConfig.SetChannel[i].ChannelNo)
		//			num = i;
		//	}
		//	if (num < model->ChartTrendConfig.SetChannel.size())
		//	{
		//		if (model->ChartTrendConfig.SetChannel[num].TrendMaxVarId != Project::DataVarId::NullId)
		//			max = UI::UIData::Number<DOUBLE>(model->ChartTrendConfig.SetChannel[num].TrendMaxVarId);
		//		else
		//			max = model->ChartTrendConfig.SetChannel[num].TrendMax;
		//		if (model->ChartTrendConfig.SetChannel[num].TrendMinVarId != Project::DataVarId::NullId)
		//			min = UI::UIData::Number<DOUBLE>(model->ChartTrendConfig.SetChannel[num].TrendMinVarId);
		//		else
		//			min = model->ChartTrendConfig.SetChannel[num].TrendMin;
		//	}
		//	break;
		//}
		default:
			break;
		}
	}
	/*******************************************************************************/

	/*******************************绘制XY轴****************************************/
	/*绘制每个通道Y轴标注*/
	void TrendChartView::DrawEveryYaxisScale(DOUBLE max, DOUBLE min, vector<Project::ScaleInfo>& info)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		
		/*设置字体*/
		fl_font(yscalefontStyle_, yscalefontSize_);
		/*设置字体颜色*/
		fl_color(active() ? yscalefontColor_ : fl_inactive(yscalefontColor_));
		if (info.size() == 0)
			return;
		DOUBLE numInterval;
		if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::PercentSacle)		//百分比
			numInterval = 100.0 / (DOUBLE)model->ChartTrendConfig.AxisY.MainScaleNum;
		else if(model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::NumSacle)
			numInterval = (DOUBLE)(max - min) / (DOUBLE)model->ChartTrendConfig.AxisY.MainScaleNum;
		for (size_t i = 0; i < info.size(); i++)
		{
			DOUBLE num;
			if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::PercentSacle)
			{
				num = numInterval * (DOUBLE)i;
				if (i == info.size() - 1)
					num = 100.0;
			}
			else if (model->ChartTrendConfig.AxisY.ScaleTag == Project::ScaleMarkType::NumSacle)
			{
				num = numInterval * (DOUBLE)i + min;
				if (i == info.size() - 1)
					num = max;
			}
			info[i].ScaleContent = ChangeDisplayFormat(num);
			fl_draw(info[i].ScaleContent.data(),
				info[i].Coordinate.X + model->ChartTrendConfig.OffX,
				info[i].Coordinate.Y + model->ChartTrendConfig.OffY,
				info[i].Width, info[i].Height, FL_ALIGN_CENTER);
		}
	}
	/*绘制Y轴标注*/
	void TrendChartView::DrawYaxisScale()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		switch (model->ChartTrendConfig.AxisY.ScaleRange)
		{
		case Project::Custom:
		{
			DOUBLE max, min;
			GetLimitValue(1, max, min);
			DrawEveryYaxisScale(max, min, model->ChartTrendConfig.YScaleInfo[0]);
			break;
		}
		case Project::Channel:
		{
			DOUBLE max, min;
			if ((size_t)model->ChartTrendConfig.AxisY.UseRangeChanbelNo < model->ChartTrendConfig.SetChannel.size())
			{
				GetLimitValue(model->ChartTrendConfig.AxisY.UseRangeChanbelNo, max, min);
				DrawEveryYaxisScale(max, min, model->ChartTrendConfig.YScaleInfo[0]);
			}
			break;
		}
		case Project::All:
		{
			for (unsigned i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
			{
				DOUBLE max, min;
				GetLimitValue(model->ChartTrendConfig.SetChannel[i].ChannelNo, max, min);
				DrawEveryYaxisScale(max, min, model->ChartTrendConfig.YScaleInfo[i]);
			}
			break;
		}
		default:
			break;
		}
	}
	/*绘制Y轴刻度*/
	void TrendChartView::DrawYaxis()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();

		double spaceInterval;
		fl_color(active() ? fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisY.ScaleColor)) 
			: fl_inactive(fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisY.ScaleColor))));
		fl_line_style(0, 0);		//设置刻度线样式

		/*绘制Y轴*/
		fl_line(model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.OffX, 
				model->ChartTrendConfig.OriginPos.Y - model->ChartTrendConfig.ScrollHeight + model->ChartTrendConfig.OffY, 
				model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.OffX,
				model->ChartTrendConfig.OriginPos.Y - model->ChartTrendConfig.YaxisLen - model->ChartTrendConfig.ScrollHeight + model->ChartTrendConfig.OffY);
		
		/*绘制Y轴刻度*/
		spaceInterval = (double)model->ChartTrendConfig.YaxisLen / (double)(model->ChartTrendConfig.AxisY.MainScaleNum * model->ChartTrendConfig.AxisY.ViceScaleNum); //刻度间隔
		for (int i = 0; i < model->ChartTrendConfig.AxisY.MainScaleNum * model->ChartTrendConfig.AxisY.ViceScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->ChartTrendConfig.AxisY.ViceScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartTrendConfig.ScrollHeight + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X - (double)model->ChartTrendConfig.AxisY.MainScaleLen + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y - spaceInterval * (double)i- (double)model->ChartTrendConfig.ScrollHeight + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
			else                                                  //副刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartTrendConfig.ScrollHeight + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X - (double)model->ChartTrendConfig.AxisY.ViceScaleLen + (double)model->ChartTrendConfig.OffX,
						(double)model->ChartTrendConfig.OriginPos.Y - spaceInterval * (double)i- (double)model->ChartTrendConfig.ScrollHeight + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
		}
		if (model->ChartTrendConfig.AxisY.ScaleTag != Project::ScaleMarkType::NullSacle)
		{
			if(!model->ChartTrendConfig.YScaleInfo.empty())
				DrawYaxisScale();
		}
	}
	/*绘制X轴标注*/
	void TrendChartView::DrawXaxis()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		string timeScale;
		double spaceInterval;
		fl_color(active() ? fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleColor))
			: fl_inactive(fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleColor))));
		//fl_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleColor));    //设置刻度线的颜色   
		fl_line_style(0, 0);											 //设置刻度线样式

		/*绘制X轴*/
		fl_line(model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.OffX,
				model->ChartTrendConfig.OriginPos.Y + model->ChartTrendConfig.OffY,
				model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.XaxisLen + model->ChartTrendConfig.OffX,
				model->ChartTrendConfig.OriginPos.Y + model->ChartTrendConfig.OffY);

		/*绘制X轴刻度*/
		spaceInterval = (double)model->ChartTrendConfig.XaxisLen / (double)(model->ChartTrendConfig.AxisX.MainScaleNum * model->ChartTrendConfig.AxisX.ViceScaleNum); //刻度间隔
				
		for (int i = 0; i < model->ChartTrendConfig.AxisX.MainScaleNum * model->ChartTrendConfig.AxisX.ViceScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->ChartTrendConfig.AxisX.ViceScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * i + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i+ (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.AxisX.MainScaleLen + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
			else		
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i+ (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i+ (double)model->ChartTrendConfig.OffX,
						(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.AxisX.ViceScaleLen + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
		}

		/*绘制查询模式下坐标*/
		if (DisplayMode && model->ChartTrendConfig.AxisX.IsUseTimeScale)
		{
			if (ChannelData.empty())
				return;
			/*设置字体*/
			fl_font(xscalefontStyle_, xscalefontSize_);
			/*设置字体颜色*/
			fl_color(active() ? xscalefontColor_ : fl_inactive(xscalefontColor_));
			DDWORD timeInterval;
			DDWORD starttime = 0;
			if (model->ChartTrendConfig.AxisX.MainScaleNum == 0)model->ChartTrendConfig.AxisX.MainScaleNum = 1;
			timeInterval = (model->ChartTrendConfig.SearchTimeEnd - model->ChartTrendConfig.SearchTimeStart)
				/ (DDWORD)model->ChartTrendConfig.AxisX.MainScaleNum;
			starttime = startTime_;

			for (size_t i = 0; i < model->ChartTrendConfig.XScaleInfo.size(); i++)
			{
				string date, time;
				if (model->ChartTrendConfig.AxisX.IsdisplayTime)
					time = GetTimeString(starttime + (DDWORD)i * timeInterval, 1, model->ChartTrendConfig.AxisX.TimeStyle);
				if (model->ChartTrendConfig.AxisX.IsdiaplayDate)
					date = GetTimeString(starttime + (DDWORD)i * timeInterval, 0, model->ChartTrendConfig.AxisX.DateStyle);
				model->ChartTrendConfig.XScaleInfo[i].ScaleContent = date + "\n" + time;
				fl_draw(model->ChartTrendConfig.XScaleInfo[i].ScaleContent.data(),
					model->ChartTrendConfig.XScaleInfo[i].Coordinate.X + model->ChartTrendConfig.OffX,
					model->ChartTrendConfig.XScaleInfo[i].Coordinate.Y + model->ChartTrendConfig.OffY,
					model->ChartTrendConfig.XScaleInfo[i].Width, model->ChartTrendConfig.XScaleInfo[i].Height, FL_ALIGN_CENTER);
			}
		}
		else if (model->ChartTrendConfig.AxisX.IsUseTimeScale && model->ChartTrendConfig.PerSceenViewMode)	//使用时间并且每屏时间模式
		{
			/*设置字体*/
			fl_font(xscalefontStyle_, xscalefontSize_);
			/*设置字体颜色*/
			fl_color(active() ? xscalefontColor_ : fl_inactive(xscalefontColor_));
			if (model->ChartTrendConfig.AxisX.IsdiaplayDate && !model->ChartTrendConfig.XScaleInfo.empty())
			{
				int timeInterval;
				DDWORD starttime = 0;
				timeInterval = perScreenPeriod_ / model->ChartTrendConfig.AxisX.MainScaleNum;
				if (startTime_ != 0 || clickstarttime_ != 0)
				{
					if (ScrollClick)
						starttime = clickstarttime_;
					else
						starttime = startTime_;
				}
				for (size_t i = 0; i < model->ChartTrendConfig.XScaleInfo.size(); i++)
				{
					string date, time;
					if (model->ChartTrendConfig.AxisX.IsdisplayTime)
						time = GetTimeString(starttime + (DDWORD)i * timeInterval, 1, model->ChartTrendConfig.AxisX.TimeStyle);
					if (model->ChartTrendConfig.AxisX.IsdiaplayDate)
						date = GetTimeString(starttime + (DDWORD)i * timeInterval, 0, model->ChartTrendConfig.AxisX.DateStyle);
					model->ChartTrendConfig.XScaleInfo[i].ScaleContent = date + "\n" + time;
					fl_draw(model->ChartTrendConfig.XScaleInfo[i].ScaleContent.data(),
						model->ChartTrendConfig.XScaleInfo[i].Coordinate.X + model->ChartTrendConfig.OffX,
						model->ChartTrendConfig.XScaleInfo[i].Coordinate.Y + model->ChartTrendConfig.OffY,
						model->ChartTrendConfig.XScaleInfo[i].Width, model->ChartTrendConfig.XScaleInfo[i].Height, FL_ALIGN_CENTER);
				}
			}
		}
	}
	void TrendChartView::DrawRenderXaxis()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		string timeScale;
		double spaceInterval;
		fl_color(active() ? fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleColor))
			: fl_inactive(fl_rgb_color(RGBColor(model->ChartTrendConfig.AxisX.ScaleColor))));
		fl_line_style(0, 0);											 //设置刻度线样式

		/*绘制X轴*/
		fl_line(model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.OffX,
			model->ChartTrendConfig.OriginPos.Y + model->ChartTrendConfig.OffY,
			model->ChartTrendConfig.OriginPos.X + model->ChartTrendConfig.XaxisLen + model->ChartTrendConfig.OffX,
			model->ChartTrendConfig.OriginPos.Y + model->ChartTrendConfig.OffY);

		/*绘制X轴刻度*/
		spaceInterval = (double)model->ChartTrendConfig.XaxisLen / (double)(model->ChartTrendConfig.AxisX.MainScaleNum * model->ChartTrendConfig.AxisX.ViceScaleNum); //刻度间隔

		for (int i = 0; i < model->ChartTrendConfig.AxisX.MainScaleNum * model->ChartTrendConfig.AxisX.ViceScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->ChartTrendConfig.AxisX.ViceScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * i + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.AxisX.MainScaleLen + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
			else
			{
				fl_begin_line();
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.OffY);
				fl_vertex((double)model->ChartTrendConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartTrendConfig.OffX,
					(double)model->ChartTrendConfig.OriginPos.Y + (double)model->ChartTrendConfig.AxisX.ViceScaleLen + (double)model->ChartTrendConfig.OffY);
				fl_end_line();
			}
		}
		if (!model->ChartTrendConfig.AxisX.IsUseTimeScale)
			return;
		string date, time;
		if (model->ChartTrendConfig.AxisX.IsdisplayTime)
			time = GetTimeString(duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count(), 1, model->ChartTrendConfig.AxisX.TimeStyle);
		if (model->ChartTrendConfig.AxisX.IsdiaplayDate)
			date = GetTimeString(duration_cast<MilliSecs>(system_clock::now().time_since_epoch()).count(), 0, model->ChartTrendConfig.AxisX.DateStyle);
		/*设置字体*/
		fl_font(xscalefontStyle_, xscalefontSize_);
		/*设置字体颜色*/
		fl_color(active() ? xscalefontColor_ : fl_inactive(xscalefontColor_));
		for (size_t i = 0; i < model->ChartTrendConfig.XScaleInfo.size(); i++)
		{
			model->ChartTrendConfig.XScaleInfo[i].ScaleContent = date + "\n" + time;
			fl_draw(model->ChartTrendConfig.XScaleInfo[i].ScaleContent.data(),
				model->ChartTrendConfig.XScaleInfo[i].Coordinate.X + model->ChartTrendConfig.OffX,
				model->ChartTrendConfig.XScaleInfo[i].Coordinate.Y + model->ChartTrendConfig.OffY,
				model->ChartTrendConfig.XScaleInfo[i].Width, model->ChartTrendConfig.XScaleInfo[i].Height, FL_ALIGN_CENTER);
		}
	}

	/*绘制每屏点数模式下的X轴标注*/
	void TrendChartView::DrawXScaleDot(int dot, DDWORD time)
	{
		if (hasxscaledot)
			return;

		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		if (model->ChartTrendConfig.AxisX.IsUseTimeScale && model->ChartTrendConfig.PerSceenViewMode == 0)	//使用时间并且每屏点数模式
		{
			/*设置字体*/
			fl_font(xscalefontStyle_, xscalefontSize_);
			/*设置字体颜色*/
			fl_color(active() ? xscalefontColor_ : fl_inactive(xscalefontColor_));

			string datestring, timestring;
			//if (startTime_ != 0)
			//{
				if (model->ChartTrendConfig.AxisX.IsdisplayTime)
					timestring = GetTimeString(time, 1, model->ChartTrendConfig.AxisX.TimeStyle);
				if (model->ChartTrendConfig.AxisX.IsdiaplayDate)
					datestring = GetTimeString(time, 0, model->ChartTrendConfig.AxisX.DateStyle);
			//}
			model->ChartTrendConfig.XScaleInfo[dot].ScaleContent = datestring + "\n" + timestring;
			fl_draw(model->ChartTrendConfig.XScaleInfo[dot].ScaleContent.data(),
				model->ChartTrendConfig.XScaleInfo[dot].Coordinate.X + model->ChartTrendConfig.OffX,
				model->ChartTrendConfig.XScaleInfo[dot].Coordinate.Y + model->ChartTrendConfig.OffY,
				model->ChartTrendConfig.XScaleInfo[dot].Width, model->ChartTrendConfig.XScaleInfo[dot].Height, FL_ALIGN_CENTER);
		}
	}
	/*******************************************************************************/
	void TrendChartView::DrawGrid()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		fl_color(active() ? fl_rgb_color(RGBColor(model->ChartTrendConfig.GridStyle.Color)) 
			: fl_inactive(fl_rgb_color(RGBColor(model->ChartTrendConfig.GridStyle.Color))));
		fl_line_style(model->ChartTrendConfig.GridStyle.Type, model->ChartTrendConfig.GridStyle.Weight);//设置栅格 样式 宽度
		double xSpaceInterval, ySpaceInterval;
		xSpaceInterval = (double)model->ChartTrendConfig.XaxisLen / (double)model->ChartTrendConfig.XUniformNum; //X轴栅格间隔
		ySpaceInterval = (double)model->ChartTrendConfig.YaxisLen / (double)model->ChartTrendConfig.YUniformNum;	//Y轴栅格间隔
		for (int i = 1; i < model->ChartTrendConfig.XUniformNum; i++)	//绘制X轴栅格	
		{
			fl_begin_line();
			fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->ChartTrendConfig.OffX,
				(double)model->ChartTrendConfig.OriginPos.Y - (double)scrollheight_ + (double)model->ChartTrendConfig.OffY);
			fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->ChartTrendConfig.OffX,
				(double)model->ChartTrendConfig.OriginPos.Y - (double)scrollheight_ - (double)model->ChartTrendConfig.YaxisLen + (double)model->ChartTrendConfig.OffY);
			fl_end_line();
		}
		for (int i = 1; i < model->ChartTrendConfig.YUniformNum; i++)	//绘制X轴栅格	
		{
			fl_begin_line();
			fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)model->ChartTrendConfig.OffX,
				(double)model->ChartTrendConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->ChartTrendConfig.OffY);
			fl_vertex((double)model->ChartTrendConfig.OriginPos.X + (double)model->ChartTrendConfig.XaxisLen + (double)model->ChartTrendConfig.OffX,
				(double)model->ChartTrendConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->ChartTrendConfig.OffY);

			fl_end_line();
		}
	}
	int TrendChartView::GetChannelNo(int no)
	{
		shared_ptr< TrendChartModel> model = BaseView.GetModel< TrendChartModel>();
		for (size_t i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
		{
			if ((no | (model->ChartTrendConfig.SimpleGroup << 16) |
				(model->ChartTrendConfig.SimpleNo << 24)) == model->ChartTrendConfig.SetChannel[i].ChannelNo)
				return  model->ChartTrendConfig.SetChannel[i].ChannelNo;
		}
		return 0;
	}
	/*绘制信息显示框*/
	void TrendChartView::DrawInfoDisplay()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		//画辅助线
		fl_color(active() ? infolinecolor_ : fl_inactive(infolinecolor_));
		fl_line_style(0, 1);
		fl_line(origin.X + model->ChartTrendConfig.OffX + displaylinex_,
			origin.Y + model->ChartTrendConfig.OffY - yaxislength - Hscrollbar->h(),
			origin.X + model->ChartTrendConfig.OffX + displaylinex_, 
			origin.Y + model->ChartTrendConfig.OffY - Hscrollbar->h());

		//绘制显示框
		/*1.绘制列标题*/
		fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
		{
			int tableX = tableX_;
			int tableY = tableY_;
			fl_font(0, 14);
			if (colwidth_.size() == title_.size())
			{
				for (unsigned i = 0; i < colwidth_.size(); i++)
				{
					fl_push_clip(tableX, tableY, colwidth_[i], rowheight_);
					{
						//fl_draw_box(FL_THIN_UP_BOX, tableX, tableY, colwidth_[i], rowheight_, infoBgColor_);
						//绘制填充背景
						fl_color(active() ? infoBgColor_ : fl_inactive(infoBgColor_));
						fl_rectf(tableX, tableY, colwidth_[i], rowheight_);
						//绘制文字
						fl_color(active() ? infofontcolor_ : infofontcolor_);
						fl_draw(title_[i].c_str(), tableX, tableY, colwidth_[i], rowheight_, FL_ALIGN_CENTER);
						//绘制边框
						fl_color(active() ? FL_BLACK : fl_inactive(FL_BLACK));
						fl_rect(tableX, tableY, colwidth_[i], rowheight_);
					}
					fl_pop_clip();
					tableX += colwidth_[i];
				}
				/*2.绘制每行数据*/
				tableY = tableY_ + rowheight_;
				string datestring = "日期";
				UI::IResourceService::GB2312toUtf8(datestring);
				//string  utf16 = CodeFormatUtility::CodeConversion(datestring, UI::CodeFormatUtility::CodeRule::UTF_16);
				//string utf16le = CodeFormatUtility::EndianSwap(utf16);
				//string  ascii = CodeFormatUtility::CodeConversion(datestring, UI::CodeFormatUtility::CodeRule::ASCII);
				//string  gb2312= CodeFormatUtility::CodeConversion(datestring, UI::CodeFormatUtility::CodeRule::GB2312);

				string timestring = "时间";
				UI::IResourceService::GB2312toUtf8(timestring);
				for (unsigned i = 0; i < model->ChartTrendConfig.DisplayContent.size(); i++)	//行数
				{
					//列数
					tableX = tableX_;
					vector<string> displayContent;	//获取显示内容
					Fl_Color lineColor = 0;
					/*限制可能有问题需要验证*/
					if (currentContent_.size() != 0)
					{
						if (model->ChartTrendConfig.DisplayContent[i].DescribeName == datestring)
						{
							lineColor = infolinecolor_;
							displayContent.push_back(model->ChartTrendConfig.DisplayContent[i].DescribeName);
							displayContent.push_back("day");
							DDWORD time = currentContent_[0].Date;
							displayContent.push_back(GetTimeString(time, 0, model->ChartTrendConfig.DisplayDateStyle));
						}
						else if (model->ChartTrendConfig.DisplayContent[i].DescribeName == timestring)
						{
							lineColor = infolinecolor_;
							displayContent.push_back(model->ChartTrendConfig.DisplayContent[i].DescribeName);
							switch (model->ChartTrendConfig.DisplayTimeStyle)
							{
							case Project::HMS:
								displayContent.push_back("s");
								break;
							case Project::HM:
								displayContent.push_back("min");
								break;
							case Project::HMSMS:
								displayContent.push_back("ms");
								break;
							case Project::HMSChinese:
								displayContent.push_back("s");
								break;
							default:
								break;
							}
							DDWORD time = currentContent_[0].Date;
							displayContent.push_back(GetTimeString(time, 1, model->ChartTrendConfig.DisplayTimeStyle));
						}
						else
						{
							int num = atoi(&model->ChartTrendConfig.DisplayContent[i].ProjectName.back());
							displayContent.push_back(model->ChartTrendConfig.DisplayContent[i].DescribeName);

							DOUBLE max, min;
							GetLimitValue(num, max, min);
							displayContent.push_back(std::to_string(min) + "~" + std::to_string(max));
							bool hasdata = false;
							for (unsigned no = 0; no < currentContent_.size(); no++)
							{
								if (currentContent_[no].Channel == GetChannelNo(num))
								{
									hasdata = true;
									Utility::NumberFmtInfo fmt;
									fmt.IsFillZero = false;
									fmt.Num1 = model->ChartTrendConfig.AxisY.YIntegerNum;
									fmt.Num2 = model->ChartTrendConfig.AxisY.YDecimalnNum;
									double a = Utility::DataFormate::GetNumber<double>(currentContent_[no].Data, currentContent_[no].Type);

									//double a = currentContent_[no].Data;
									DDWORD da;
									memcpy(&da, &a, sizeof(DDWORD));
									displayContent.push_back(Utility::DataFormate::NumFmt(
										da, currentContent_[no].Type, fmt));
									//displayContent.push_back(Utility::DataFormate::NumFmt(
									//	currentContent_[no].Data, currentContent_[no].Type, fmt));
								}
							}
							if (!hasdata)
								displayContent.push_back("0");

							for (unsigned no = 0; no < model->ChartTrendConfig.SetChannel.size(); no++)
							{
								if (model->ChartTrendConfig.SetChannel[no].ChannelNo == num)
									lineColor = fl_rgb_color(RGBColor(model->ChartTrendConfig.SetChannel[no].TrendStyle.Color));
							}
						}
						for (unsigned j = 0; j < colwidth_.size(); j++)
						{
							fl_push_clip(tableX, tableY, colwidth_[j], rowheight_);
							{
								// BG COLOR
								fl_color(active() ? infoBgColor_ : fl_inactive(infoBgColor_));
								fl_rectf(tableX, tableY, colwidth_[j], rowheight_);

								// TEXT
								if (j == 0)
								{
									fl_color(active() ? lineColor : fl_inactive(lineColor));
									fl_rectf(tableX + (colwidth_[j] - fl_height()) / 2, tableY + (rowheight_ - fl_height()) / 2, fl_height(), fl_height());
								}
								else
								{
									fl_color(active() ? infofontcolor_ : fl_inactive(infofontcolor_));
									fl_draw(displayContent[j - 1].c_str(), tableX, tableY, colwidth_[j], rowheight_, FL_ALIGN_CENTER);
								}

								// BORDER
								fl_color(active() ? FL_BLACK : fl_inactive(FL_BLACK));
								fl_rect(tableX, tableY, colwidth_[j], rowheight_);

								tableX += colwidth_[j];
							}
							fl_pop_clip();

						}
						tableY += rowheight_;
					}
				}
			}
		}
		fl_pop_clip();
	}

	/********************************绘制趋势****************************************/
	/*绘制查询模式*/
	void TrendChartView::DrawSearchTrend(Project::TrendChannel channelinfo)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		DOUBLE max, min;		//获取最大最小值
		DDWORD PeriodTime;
		if (Storage::SampleStorage::Ins()->GetCountByChannel(channelinfo.ChannelNo) == 0)      //获取当前数据数
			return;
		GetLimitValue(channelinfo.ChannelNo, max, min);
		vector<Storage::SampleRecord>().swap(ChannelData);
		Fl_Color linecolor = fl_rgb_color(RGBColor(channelinfo.TrendStyle.Color));
		switch (model->ChartTrendConfig.SearchMode)
		{
		case Project::TrendSeekMode::CHANNELNO:
		{
			ChannelData = Storage::SampleStorage::Ins()->QueryByChannel(channelinfo.ChannelNo);
			if (ChannelData.empty())
				return;
			PeriodTime = ChannelData[ChannelData.size() - 1].Date - ChannelData[0].Date;
			startTime_ = ChannelData[0].Date;
			break;
		}
		case Project::TrendSeekMode::DATE:
		{
			PeriodTime = 86400000;
			ChannelData = Storage::SampleStorage::Ins()->QueryByDate(channelinfo.ChannelNo, model->ChartTrendConfig.SearchDate,
					model->ChartTrendConfig.SearchDate + (DDWORD)86400000);
			startTime_ = model->ChartTrendConfig.SearchDate;
			break;
		}
			
		case Project::TrendSeekMode::TIME:
		{
			PeriodTime = model->ChartTrendConfig.SearchTimeEnd - model->ChartTrendConfig.SearchTimeStart;
			ChannelData = Storage::SampleStorage::Ins()->QueryByTime(channelinfo.ChannelNo, model->ChartTrendConfig.SearchTimeStart,
				model->ChartTrendConfig.SearchTimeEnd);
			startTime_ = model->ChartTrendConfig.SearchTimeStart;
			break;
		}
		default:
			break;
		}
		if (ChannelData.empty())
			return;
	
		//Hscrollbar->value(Xaxislength);
		Hscrollbar->slider_size(1.0);
		int screendotnum = ChannelData.size();
		fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
		{
			fl_begin_line();
			for (size_t i = 0;i < ChannelData.size(); i++)
			{
				int dx;
				if (PeriodTime == 0)
					dx = 0;
				else
					dx = (int)((ChannelData[i].Date - startTime_) * (DDWORD)Xaxislength / PeriodTime);
				SetDisPlayPos(i, dx);
				fl_color(active() ? linecolor : fl_inactive(linecolor));
				fl_line_style(0, channelinfo.TrendStyle.Weight);
				DrawChannelLine((Project::TrendLineType)channelinfo.TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + dx,
					origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(channelinfo.ChannelNo, ChannelData[i].Data, ChannelData[i].Type, max, min));

				//fl_vertex(origin.X + model->ChartTrendConfig.OffX + dx, 
				//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min));
				//fl_pie(origin.X + model->ChartTrendConfig.OffX + dx - 2, 
				//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min) - 2, 4, 4, 0, 360);
			}
			fl_end_line();
		}
		fl_pop_clip();
	}
	void TrendChartView::DrawChannelLine(Project::TrendLineType type, double x, double y)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		switch (type)
		{
		case Project::TrendLine:
		case Project::TrendCurve:
		{
			fl_vertex(x,y);
			break;
		}
		case Project::TrendDot:
			fl_pie((int)x, (int)y - 2, 4, 4, 0.0, 360.0);
			break;
		case Project::TrendLineDot:
			fl_vertex(x, y);
			fl_pie((int)x, (int)y - 2, 4, 4, 0.0, 360.0);
			break;
		default:
			break;
		}
	}
	/*绘制正常显示模式*/
	void TrendChartView::DrawChannelTrend(Project::TrendChannel channelinfo)
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		int channelno = channelinfo.ChannelNo;
		DOUBLE max, min;		//获取最大最小值
		GetLimitValue(channelno, max, min);
		if (Storage::SampleStorage::Ins()->GetCountByChannel(channelno) == 0)      //获取当前数据数
		{
			hasfirstStartTime_ = false;
			return;
		}
		/*获取线条颜色*/
		Fl_Color linecolor = fl_rgb_color(RGBColor(channelinfo.TrendStyle.Color));
		/*根据屏幕显示内容模式获取数据*/
		if (model->ChartTrendConfig.PerSceenViewMode)		//时间段
		{
			vector<Storage::SampleRecord>().swap(ChannelData);
			Storage::SampleStorage::Ins()->QueryByTimePeriod(channelno, perScreenPeriod_, ChannelData);
			if (perScreenPeriod_ == 0)
				return;
			if (!ChannelData.empty())
			{
				scalecurrenttime_ = ChannelData[ChannelData.size() - 1].Date;
				startTime_ = ChannelData[0].Date;

				/*设置滚动条*/
				float hscrolltab = (ChannelData[ChannelData.size() - 1].Date <= firstEndTime_) ? 1 : (float)perScreenPeriod_ / (float)(ChannelData[ChannelData.size() - 1].Date - firstStartTime_);
				if (hscrolltab == 1)
					Hscrollbar->value(Xaxislength);
				Hscrollbar->slider_size(hscrolltab);
			}
			if (Hscrollbar->value() == Xaxislength || ScrollClick == 0)
			{
				if (ChannelData.empty())
					return;
				//if (ChannelData[ChannelData.size() - 1].Date <= firstStartTime_)
				//	startTime_ = ChannelData[0].Date;
				//else
				startTime_ = ChannelData[ChannelData.size() - 1].Date - perScreenPeriod_;
				vector<Storage::SampleRecord>().swap(ChannelData);
				ChannelData = Storage::SampleStorage::Ins()->QueryByTime(channelno, startTime_, startTime_ + perScreenPeriod_);

				fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
				{
					fl_begin_line();
					{
						for (size_t i = 0;i < ChannelData.size(); i++)
						{
							int dx = (int)((ChannelData[i].Date - startTime_) * (DDWORD)Xaxislength / perScreenPeriod_);
							SetDisPlayPos(i, dx);
							fl_color(active() ? linecolor : fl_inactive(linecolor));
							fl_line_style(0, channelinfo.TrendStyle.Weight);
							DrawChannelLine((Project::TrendLineType)channelinfo.TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + dx,
								origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(channelinfo.ChannelNo, ChannelData[i].Data, ChannelData[i].Type, max, min));
						}
					}
					fl_end_line();
				}
				fl_pop_clip();
			}
			else if (ScrollClick)
			{
				if (Hscrollbar->maximum() - Hscrollbar->minimum() == 0) return;
				clickstarttime_ = firstStartTime_ + (((scalecurrenttime_ - (DDWORD)perScreenPeriod_) - firstStartTime_) * (DDWORD)Hscrollbar->value() / (DDWORD)(Hscrollbar->maximum() - Hscrollbar->minimum()));
				vector<Storage::SampleRecord>().swap(ChannelData);
				ChannelData = Storage::SampleStorage::Ins()->QueryByTime(channelno, clickstarttime_, clickstarttime_ + perScreenPeriod_);
				LOG_INFO("ScrollClick ChannelData size = %d\n", ChannelData.size());

				fl_begin_line();
				fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
				{
					for (size_t i = 0; i < ChannelData.size(); i++)
					{
						int dx = (int)((ChannelData[i].Date - clickstarttime_) * (DDWORD)Xaxislength / perScreenPeriod_);
						SetDisPlayPos(i, dx);
						fl_color(active() ? linecolor : fl_inactive(linecolor));
						fl_line_style(0, channelinfo.TrendStyle.Weight);
						DrawChannelLine((Project::TrendLineType)channelinfo.TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + dx,
							origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(channelinfo.ChannelNo, ChannelData[i].Data, ChannelData[i].Type, max, min));
					}
				}
				fl_pop_clip();
				fl_end_line();

			}
		}
		else
		{
			currentnum_ = Storage::SampleStorage::Ins()->GetCountByChannel(channelno);      //获取当前数据数
			scalecurrentnum_ = currentnum_; //获取当前数据数

			if (currentnum_ == 0 || perScreendot_ == 0)
				return;
			float hscrolltab = (currentnum_ <= perScreendot_) ? 1 : (float)perScreendot_ / (float)currentnum_;
			if (hscrolltab == 1)
				Hscrollbar->value(Xaxislength);
			Hscrollbar->slider_size(hscrolltab);
			if (Hscrollbar->value() == Xaxislength || ScrollClick == 0)
			{
				vector<Storage::SampleRecord>().swap(ChannelData);
				vector<DDWORD> times;
				Storage::SampleStorage::Ins()->QueryLastRecords(channelno, perScreendot_, ChannelData);
				xscaleNum = 0;
				printf("value1 = %d\n", Hscrollbar->value());
				fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
				{
					fl_begin_line();
					for (size_t i = 0; i < ChannelData.size(); i++)
					{
						int dx;
						
						if (perScreendot_ == 1)
							dx = 0;
						else
							dx = i * Xaxislength / (perScreendot_ - 1);
						SetDisPlayPos(i, dx);
					
						fl_color(active() ? linecolor : fl_inactive(linecolor));
						fl_line_style(0, channelinfo.TrendStyle.Weight);
						DrawChannelLine((Project::TrendLineType)channelinfo.TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + dx,
							origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(channelinfo.ChannelNo, ChannelData[i].Data, ChannelData[i].Type, max, min));

						//fl_vertex(origin.X + model->ChartTrendConfig.OffX + dx, 
						//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min));
						//fl_pie(origin.X + model->ChartTrendConfig.OffX + dx - 2, 
						//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min) - 2, 4, 4, 0, 360);
						
						if (dx >= xscaleInterval_ * xscaleNum)
						{
							times.push_back(ChannelData[i].Date);
							/*DrawXScaleDot(xscaleNum, ChannelData[i].Date);*/
							xscaleNum++;
							if (xscaleNum == model->ChartTrendConfig.AxisX.MainScaleNum + 1)
								xscaleNum = 0;
							fl_color(active() ? linecolor : fl_inactive(linecolor));
						}
					}
					fl_end_line();
				}
				fl_pop_clip();
				for (size_t i = 0; i < times.size(); i++)
					DrawXScaleDot(i, times[i]);
			}
			else if (ScrollClick == 1)
			{
				/*获取数据*/
				int startNum = (int)((double)((scalecurrentnum_ - perScreendot_) * Hscrollbar->value()) / (Hscrollbar->maximum() - Hscrollbar->minimum()));
				vector<Storage::SampleRecord>().swap(ChannelData);
				ChannelData = Storage::SampleStorage::Ins()->QueryByChannel(channelno, startNum, perScreendot_);
				vector<DDWORD> times;
				fl_push_clip(origin.X + model->ChartTrendConfig.OffX, origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength, Xaxislength, yaxislength);
				{
					fl_begin_line();
					for (size_t i = 0; i < ChannelData.size(); i++)
					{
						int dx = i * Xaxislength / (perScreendot_ - 1);
						
							SetDisPlayPos(i, dx);
					
							fl_color(active() ? linecolor : fl_inactive(linecolor));
							fl_line_style(0, channelinfo.TrendStyle.Weight);
							DrawChannelLine((Project::TrendLineType)channelinfo.TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + dx,
								origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(channelinfo.ChannelNo, ChannelData[i].Data, ChannelData[i].Type, max, min));

							//fl_vertex(origin.X + model->ChartTrendConfig.OffX + dx, 
							//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min));
							//fl_pie(origin.X + model->ChartTrendConfig.OffX + dx - 2, 
							//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - GetYCoordinate(ChannelData[i].Data, ChannelData[i].Type, max, min) - 2, 4, 4, 0, 360);
						
						if (dx >= xscaleInterval_ * xscaleNum)
						{
							times.push_back(ChannelData[i].Date);
							//DrawXScaleDot(xscaleNum, ChannelData[i].Date);
							xscaleNum++;
							if (xscaleNum == model->ChartTrendConfig.AxisX.MainScaleNum + 1)
								xscaleNum = 0;
						}
					}
					fl_end_line();
				}
				fl_pop_clip();
				for (size_t i = 0; i < times.size(); i++)
					DrawXScaleDot(i, times[i]);
			}
		}
	}
	/*******************************************************************************/
	void TrendChartView::DrawRenderTrend()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		double xspaceInterval = (double)Xaxislength / 4.0;
		vector<double> xpos;
		vector<double> ypos;
		for (size_t i = 0; i < 5; i++)
			xpos.push_back((double)i * xspaceInterval);
		ypos.push_back((double)yaxislength / 2.0);
		ypos.push_back((double)yaxislength / 4.0);
		ypos.push_back((double)yaxislength / 2.0);
		ypos.push_back((double)yaxislength * 3.0 / 4.0);
		ypos.push_back((double)yaxislength / 2.0);

		for (unsigned j = 0; j < model->ChartTrendConfig.SetChannel.size(); j++)
		{
			Fl_Color linecolor = fl_rgb_color(RGBColor(model->ChartTrendConfig.SetChannel[j].TrendStyle.Color));
			fl_color(active() ? linecolor : fl_inactive(linecolor));
			fl_line_style(0, model->ChartTrendConfig.SetChannel[j].TrendStyle.Weight);

			GraphicDrawHandle::PushClip((origin.X + model->ChartTrendConfig.OffX),
				(origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - yaxislength),
				Xaxislength , yaxislength);
			{
				fl_begin_line();
				for (size_t i = 0;i < xpos.size(); i++)
				{
					if (i == 0 || i == xpos.size() - 1)
					{
						DrawChannelLine((Project::TrendLineType)model->ChartTrendConfig.SetChannel[j].TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + xpos[i],
							origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i] - j * 10);

						//fl_vertex(origin.X + model->ChartTrendConfig.OffX + xpos[i],
						//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i] - j * 10);
						//fl_pie((int)(origin.X + model->ChartTrendConfig.OffX + xpos[i] - 2),
						//	(int)(origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i] - 2 - j * 10), 4, 4, 0.0, 360.0);
					}
					else
					{
						DrawChannelLine((Project::TrendLineType)model->ChartTrendConfig.SetChannel[j].TrendStyle.Type, origin.X + model->ChartTrendConfig.OffX + xpos[i] + j * 10,
							origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i]);
						//fl_vertex(origin.X + model->ChartTrendConfig.OffX + xpos[i] + j * 10,
						//	origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i]);
						//fl_pie((int)(origin.X + model->ChartTrendConfig.OffX + xpos[i] - 2 + j * 10),
						//	(int)(origin.Y + model->ChartTrendConfig.OffY - scrollheight_ - ypos[i] - 2), 4, 4, 0.0, 360.0);
					}
				}
				fl_end_line();
			}
			fl_pop_clip();
		}
		//绘制坐标轴
		if (useXAxis)
			DrawRenderXaxis();
		if (useYAxis)
			DrawYaxis();
	}

	/*绘制趋势图*/
	void TrendChartView::draw()
	{
		shared_ptr<TrendChartModel> model = BaseView.GetModel<TrendChartModel>();
		double zoom = UI::IResourceService::Ins()->GetZoom();
		//绘制刻度背景
		draw_box((Fl_Boxtype)1,model->ChartTrendConfig.X + model->ChartTrendConfig.OffX,
			model->ChartTrendConfig.Y + model->ChartTrendConfig.OffY,
			model->ChartTrendConfig.Width, model->ChartTrendConfig.Height, scaleBgColor_);
		//绘制趋势图背景
		draw_box((Fl_Boxtype)1, origin.X + model->ChartTrendConfig.OffX, 
			origin.Y - scrollheight_ - yaxislength + model->ChartTrendConfig.OffY, 
			Xaxislength, yaxislength, trendBgColor_);
		//绘制栅格
		if (useGrid)
			DrawGrid();
		//绘制滚动条
		//Fl::scrollbar_size(scrollheight_);
		Hscrollbar->resize(origin.X + model->ChartTrendConfig.OffX, 
			origin.Y - scrollheight_ + model->ChartTrendConfig.OffY, Xaxislength, scrollheight_);
		Hscrollbar->type(FL_HORIZONTAL);
		Hscrollbar->callback(TrendChartScrollCb, (void*)this);
		Hscrollbar->bounds(0, Xaxislength);
		//Hscrollbar->slider_size(1);
		Hscrollbar->damage(FL_DAMAGE_ALL);
		Fl_Group::draw();
		if (IResourceService::Ins()->IsRenderMode())
		{
			DrawRenderTrend();
			return;
		}
		//绘制趋势线条
		if (model->ChartTrendConfig.SetChannel.empty())
			return;

		if (hasfirstStartTime_ == false)
		{
			vector<Storage::SampleRecord>().swap(ChannelData);
			ChannelData = Storage::SampleStorage::Ins()->QueryByChannel(model->ChartTrendConfig.SetChannel[0].ChannelNo, 0, 1 );
			if(!ChannelData.empty())
			{
				hasfirstStartTime_ = true;
				firstStartTime_ = ChannelData[0].Date;
				firstEndTime_ = firstStartTime_ + perScreenPeriod_;
			}
		}
		vector<Storage::SampleRecord>().swap(currentContent_);
		if (DisplayMode)	//查询模式
		{
			if (model->ChartTrendConfig.SearchMode == Project::TrendSeekMode::CHANNELNO)
			{
				for (unsigned i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
				{
					if (model->ChartTrendConfig.SetChannel[i].ChannelNo == model->ChartTrendConfig.SearchChannelNo)
						DrawSearchTrend(model->ChartTrendConfig.SetChannel[i]);
				}
			}
			else
			{
				for (unsigned i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
					DrawSearchTrend(model->ChartTrendConfig.SetChannel[i]);
			}
		}
		else //显示模式
		{
			for (unsigned i = 0; i < model->ChartTrendConfig.SetChannel.size(); i++)
			{
				if (i == 0)
					hasxscaledot = false;
				else
					hasxscaledot = true;
				DrawChannelTrend(model->ChartTrendConfig.SetChannel[i]);
			}
		}
		//绘制信息显示
		if(UseInfoDisplay)
			DrawInfoDisplay();
		//绘制坐标轴
		if (useXAxis)
			DrawXaxis();
		if (useYAxis)
			DrawYaxis();
	}
}
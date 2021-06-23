#include "GraphicDrawHandle.h"
#include "XYLineChartView.h"
#include "XYLineChartControl.h"
#include "XYLineChartModel.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iomanip>
#include "IResourceService.h"
#include "DataInfo.h"
#include "DataFormate.h"
#include "UIDataService.h"
namespace UI
{
	XYLineChartView::XYLineChartView(int X, int Y, int W, int H, const char* L) : HMIBaseGroup(X, Y, W, H, L) {
		Hscrollbar = new Fl_Scrollbar(0, 0, 16, 10);
		Fl_Group::end();		// end the group's begin()
	}
	XYLineChartView::~XYLineChartView() {}

	/*初始画绘制参数*/
	void XYLineChartView::InitDraw()
	{
		std::shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
		chartBgColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.LineChartBackColor));
		scaleBgColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.ScaleAreaBackColor));
		useGrid = model->ChartXYLineConfig.UseGrid;
		useXAxis = model->ChartXYLineConfig.UseYScale;
		useYAxis = model->ChartXYLineConfig.UseXScale;
		if (model->ChartXYLineConfig.XScaleSet.UseScaleMark)
		{
			xscalefontSize_ = model->ChartXYLineConfig.XScaleSet.ScaleMarkSize;
			xscalefontStyle_ = UI::IResourceService::GetFontIdx(model->ChartXYLineConfig.XScaleSet.ScaleMarkFont);
			xscalefontColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.XScaleSet.ScaleMarkColor));
		}
		xscaleInterval_ = model->ChartXYLineConfig.XaxisLen / model->ChartXYLineConfig.XScaleSet.PrimaryScaleNum;
		
		if (model->ChartXYLineConfig.YScaleSet.UseScaleMark)
		{
			yscalefontSize_ = model->ChartXYLineConfig.YScaleSet.ScaleMarkSize;
			yscalefontStyle_ = UI::IResourceService::GetFontIdx(model->ChartXYLineConfig.YScaleSet.ScaleMarkFont);
			yscalefontColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.YScaleSet.ScaleMarkColor));
		}

		perScreendot_ = model->ChartXYLineConfig.PointsPerScreen;			//每屏点数
		Xaxislength = model->ChartXYLineConfig.XaxisLen;
		yaxislength = model->ChartXYLineConfig.YaxisLen;
		origin.X = model->ChartXYLineConfig.OriginPos.X;
		origin.Y = model->ChartXYLineConfig.OriginPos.Y;
		scrollheight_ = model->ChartXYLineConfig.ScrollHeight;
		
		if (model->ChartXYLineConfig.SamplePointsVar == Project::DataVarId::NullId)
			SampleNum = model->ChartXYLineConfig.SamplePoints;
		perScreendot_ = model->ChartXYLineConfig.PointsPerScreen;
	}
	void XYLineChartView::DrawGrid()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();

		fl_color(RGBColor(model->ChartXYLineConfig.GridStyle.Color));    //设置栅格的颜色  
		fl_line_style(model->ChartXYLineConfig.GridStyle.Type, model->ChartXYLineConfig.GridStyle.Weight);//设置栅格 样式 宽度
		double xSpaceInterval, ySpaceInterval;
		xSpaceInterval = (double)model->ChartXYLineConfig.XaxisLen / (double)model->ChartXYLineConfig.XGridEqualNum; //X轴栅格间隔
		ySpaceInterval = (double)model->ChartXYLineConfig.YaxisLen / (double)model->ChartXYLineConfig.YGridEqualNum;	//Y轴栅格间隔
		for (int i = 1; i < model->ChartXYLineConfig.XGridEqualNum; i++)	//绘制X轴栅格	
		{
			fl_begin_line();
			fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->ChartXYLineConfig.OffX,
				(double)model->ChartXYLineConfig.OriginPos.Y - (double)scrollheight_ + (double)model->ChartXYLineConfig.OffY);
			fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->ChartXYLineConfig.OffX,
				(double)model->ChartXYLineConfig.OriginPos.Y - (double)scrollheight_ - (double)model->ChartXYLineConfig.YaxisLen + (double)model->ChartXYLineConfig.OffY);
			fl_end_line();
		}
		for (int i = 1; i < model->ChartXYLineConfig.YGridEqualNum; i++)	//绘制X轴栅格	
		{
			fl_begin_line();
			fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)model->ChartXYLineConfig.OffX,
				(double)model->ChartXYLineConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->ChartXYLineConfig.OffY);
			fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)model->ChartXYLineConfig.XaxisLen + (double)model->ChartXYLineConfig.OffX,
				(double)model->ChartXYLineConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->ChartXYLineConfig.OffY);

			fl_end_line();
		}
	}

	/*转换数据显示格式*/
	string XYLineChartView::ChangeDisplayFormat(float value, int integer, int decimal)
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
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
		if (floatString.size() < (unsigned)decimal)
		{
			unsigned size = (unsigned)decimal - floatString.size();
			for (unsigned i = 0; i < size;i++)
				floatString += "0";
		}
		else
			floatString = floatString.substr(0, decimal);
		if (intString.size() > (size_t)integer)
		{
			string maxString;
			for (size_t i = 0; i < (size_t)integer; i++)
				maxString += "*";
			return maxString;
		}
		if (decimal)
			return intString + "." + floatString;
		else
			return intString;
	}


	void XYLineChartView::DrawYaxis()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
		double spaceInterval;
		fl_color(RGBColor(model->ChartXYLineConfig.YScaleSet.ScaleColor));    //设置刻度线的颜色   
		fl_line_style(0, 0);													//设置刻度线样式

		/*绘制X轴*/
		fl_line(model->ChartXYLineConfig.OriginPos.X + model->ChartXYLineConfig.OffX,
			model->ChartXYLineConfig.OriginPos.Y - model->ChartXYLineConfig.ScrollHeight + model->ChartXYLineConfig.OffY,
			model->ChartXYLineConfig.OriginPos.X + model->ChartXYLineConfig.OffX,
			model->ChartXYLineConfig.OriginPos.Y - model->ChartXYLineConfig.YaxisLen - model->ChartXYLineConfig.ScrollHeight + model->ChartXYLineConfig.OffY);

		/*绘制X轴刻度*/
		spaceInterval = (double)model->ChartXYLineConfig.YaxisLen / (double)(model->ChartXYLineConfig.YScaleSet.PrimaryScaleNum * model->ChartXYLineConfig.YScaleSet.SecondaryScaleNum); //刻度间隔

		for (int i = 0; i < model->ChartXYLineConfig.YScaleSet.PrimaryScaleNum * model->ChartXYLineConfig.YScaleSet.SecondaryScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->ChartXYLineConfig.YScaleSet.SecondaryScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartXYLineConfig.ScrollHeight + (double)model->ChartXYLineConfig.OffY);
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X - (double)model->ChartXYLineConfig.YScaleSet.PrimaryScaleLen + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartXYLineConfig.ScrollHeight + (double)model->ChartXYLineConfig.OffY);
				fl_end_line();
			}
			else if (model->ChartXYLineConfig.YScaleSet.UseSecondaryScaleNum)//副刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartXYLineConfig.ScrollHeight + (double)model->ChartXYLineConfig.OffY);
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X - (double)model->ChartXYLineConfig.YScaleSet.SecondaryScaleLen + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y - spaceInterval * (double)i - (double)model->ChartXYLineConfig.ScrollHeight + (double)model->ChartXYLineConfig.OffY);
				fl_end_line();
			}
		}
		/*绘制坐标*/
		float numInterval = (float)(model->ChartXYLineConfig.YScaleSet.UpperLimit - model->ChartXYLineConfig.YScaleSet.LowerLimit) / (float)model->ChartXYLineConfig.XScaleSet.PrimaryScaleNum;
		if (model->ChartXYLineConfig.YScaleSet.UseScaleMark)	
		{
			/*设置字体*/
			fl_font(yscalefontStyle_, yscalefontSize_);
			/*设置字体颜色*/
			fl_color(yscalefontColor_);
			for (size_t i = 0; i < model->ChartXYLineConfig.YScaleInfo.size(); i++)
			{
				float num = numInterval * (float)i + (float)model->ChartXYLineConfig.YScaleSet.LowerLimit;
				if (i == model->ChartXYLineConfig.YScaleInfo.size() - 1)
					num = (float)model->ChartXYLineConfig.YScaleSet.UpperLimit;
				model->ChartXYLineConfig.YScaleInfo[i].ScaleContent = ChangeDisplayFormat(num,model->ChartXYLineConfig.YScaleSet.IntegerNum, model->ChartXYLineConfig.YScaleSet.DecimalnNum);
				fl_draw(model->ChartXYLineConfig.YScaleInfo[i].ScaleContent.data(),
					model->ChartXYLineConfig.YScaleInfo[i].Coordinate.X + model->ChartXYLineConfig.OffX,
					model->ChartXYLineConfig.YScaleInfo[i].Coordinate.Y + model->ChartXYLineConfig.OffY,
					model->ChartXYLineConfig.YScaleInfo[i].Width, model->ChartXYLineConfig.YScaleInfo[i].Height, FL_ALIGN_CENTER);
			}
		}
	}
	void XYLineChartView::DrawDot(Project::DotStyle style, int x, int y, int size)
	{
		switch (style)
		{
		case Project::FilledRect:
		{
			x -= size / 2;
			y -= size / 2;
			fl_rectf(x,y,size,size,currentdotColor_);
			break;
		}
		case Project::FilledTriangle:
		{
			double a = cos(30.0 * 3.1415926 / 180.0) * (double)size / 3.0;
			fl_color(currentdotColor_);
			fl_begin_complex_polygon();
			fl_vertex(x - size / 2, y + a);
			fl_vertex(x, y - a * 2);
			fl_vertex(x + size / 2, y + a);
			fl_end_complex_polygon();
			break;
		}
		case Project::FilledCricle:
		{
			x -= size / 2;
			y -= size / 2;
			fl_color(currentdotColor_);
			fl_pie(x,y,size,size,0,360);
			break;
		}
		case Project::Rect:
		{
			x -= size / 2;
			y -= size / 2;
			fl_rect(x, y, size, size, currentdotColor_);
			break;
		}		
		case Project::Triangel:
		{
			double a = cos(30 / 180 * 3.1415926) * size / 3;
			fl_color(currentdotColor_);
			fl_begin_polygon();
			fl_vertex(x - size / 2, y + a);
			fl_vertex(x, y - a * 2);
			fl_vertex(x + size * 2, y + a);
			fl_end_polygon();
			break;
		}
		case Project::Cricle:
		{
			x -= size / 2;
			y -= size / 2;
			fl_color(currentdotColor_);
			fl_arc(x, y, size, size, 0, 360);
			break;
		}
		case Project::Fork:
		{
			fl_color(currentdotColor_);
			fl_line_style(0, 0);											 //设置刻度线样式
			fl_line(x - size / 2, y - size / 2, x + size / 2, y + size / 2);
			fl_line(x - size / 2, y + size / 2, x + size / 2, y - size / 2);
			break;
		}
		default:
			break;
		}
	}

	void XYLineChartView::DrawReferenceLine()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
		GraphicDrawHandle::PushClip(origin.X + model->ChartXYLineConfig.OffX,
			origin.Y - scrollheight_ - yaxislength + model->ChartXYLineConfig.OffY, Xaxislength, yaxislength);
		for (unsigned i = 0; i < model->ChartXYLineConfig.ReferenceLines.size(); i++)
		{
			fl_color(fl_rgb_color(RGBColor(model->ChartXYLineConfig.ReferenceLines[i].LineColor)));
			fl_line_style(0, 0);
			double y0 = (double)(model->ChartXYLineConfig.ReferenceLines[i].LineValue * (double)yaxislength /
				(double)(model->ChartXYLineConfig.YScaleSet.UpperLimit - model->ChartXYLineConfig.YScaleSet.LowerLimit));
			fl_begin_line();
			fl_vertex((double)origin.X + (double)model->ChartXYLineConfig.OffX,
				(double)origin.Y + (double)model->ChartXYLineConfig.OffY - (double)scrollheight_ - y0);
			fl_vertex((double)origin.X + (double)model->ChartXYLineConfig.OffX + (double)Xaxislength,
				(double)origin.Y + (double)model->ChartXYLineConfig.OffY - (double)scrollheight_ - y0);
			fl_end_line();
		}
		fl_pop_clip();
	}

	void XYLineChartView::DrawXaxis()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
		double spaceInterval;
		fl_color(RGBColor(model->ChartXYLineConfig.XScaleSet.ScaleColor));    //设置刻度线的颜色   
		fl_line_style(0, 0);											 //设置刻度线样式

		/*绘制X轴*/
		fl_line(model->ChartXYLineConfig.OriginPos.X + model->ChartXYLineConfig.OffX,
			model->ChartXYLineConfig.OriginPos.Y + model->ChartXYLineConfig.OffY,
			model->ChartXYLineConfig.OriginPos.X + model->ChartXYLineConfig.XaxisLen + model->ChartXYLineConfig.OffX,
			model->ChartXYLineConfig.OriginPos.Y + model->ChartXYLineConfig.OffY);

		/*绘制X轴刻度*/
		spaceInterval = (double)model->ChartXYLineConfig.XaxisLen / (double)(model->ChartXYLineConfig.XScaleSet.PrimaryScaleNum * model->ChartXYLineConfig.XScaleSet.SecondaryScaleNum); //刻度间隔

		for (int i = 0; i < model->ChartXYLineConfig.XScaleSet.PrimaryScaleNum * model->ChartXYLineConfig.XScaleSet.SecondaryScaleNum + 1; i++)	//绘制刻度线	
		{
			if (i % model->ChartXYLineConfig.XScaleSet.SecondaryScaleNum == 0)         //主刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y + (double)model->ChartXYLineConfig.OffY);
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y + (double)model->ChartXYLineConfig.XScaleSet.PrimaryScaleLen + (double)model->ChartXYLineConfig.OffY);
				fl_end_line();
			}
			else if (model->ChartXYLineConfig.XScaleSet.UseSecondaryScaleNum)//副刻度
			{
				fl_begin_line();
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y + (double)model->ChartXYLineConfig.OffY);
				fl_vertex((double)model->ChartXYLineConfig.OriginPos.X + spaceInterval * (double)i + (double)model->ChartXYLineConfig.OffX,
					(double)model->ChartXYLineConfig.OriginPos.Y + (double)model->ChartXYLineConfig.XScaleSet.SecondaryScaleLen + (double)model->ChartXYLineConfig.OffY);
				fl_end_line();
			}
		}
		/*绘制坐标*/
		float numInterval = (float)(model->ChartXYLineConfig.XScaleSet.UpperLimit - model->ChartXYLineConfig.XScaleSet.LowerLimit) / (float)model->ChartXYLineConfig.XScaleSet.PrimaryScaleNum;
		if (model->ChartXYLineConfig.XScaleSet.UseScaleMark)	//使用时间并且每屏时间模式
		{
			/*设置字体*/
			fl_font(xscalefontStyle_, xscalefontSize_);
			/*设置字体颜色*/
			fl_color(xscalefontColor_);
			for (size_t i = 0; i < model->ChartXYLineConfig.XScaleInfo.size(); i++)
			{
				float num = numInterval * (float)i + (float)model->ChartXYLineConfig.XScaleSet.LowerLimit;
				if (i == model->ChartXYLineConfig.XScaleInfo.size() - 1)
					num = (float)model->ChartXYLineConfig.XScaleSet.UpperLimit;
				model->ChartXYLineConfig.XScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, model->ChartXYLineConfig.XScaleSet.IntegerNum, model->ChartXYLineConfig.XScaleSet.DecimalnNum);
				fl_draw(model->ChartXYLineConfig.XScaleInfo[i].ScaleContent.data(),
					model->ChartXYLineConfig.XScaleInfo[i].Coordinate.X + model->ChartXYLineConfig.OffX,
					model->ChartXYLineConfig.XScaleInfo[i].Coordinate.Y + model->ChartXYLineConfig.OffY,
					model->ChartXYLineConfig.XScaleInfo[i].Width, model->ChartXYLineConfig.XScaleInfo[i].Height, FL_ALIGN_CENTER);
			}
		}
	}
	
	void XYLineChartView::DrawChannelXYLine(Project::XYLineChannel channelinfo, int startnum, int num)
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();

		Fl_Color linecolor = fl_rgb_color(RGBColor(channelinfo.LineColor));

		Utility::ScaleInfo xscale;
		if (model->ChartXYLineConfig.XScaleSet.UpperLimit == model->ChartXYLineConfig.XScaleSet.LowerLimit)
			xscale.A = 0;
		else
			xscale.A = (DOUBLE)(Xaxislength) / (model->ChartXYLineConfig.XScaleSet.UpperLimit - model->ChartXYLineConfig.XScaleSet.LowerLimit);
		xscale.B = 0 - (xscale.A *  model->ChartXYLineConfig.XScaleSet.LowerLimit);

		Utility::ScaleInfo yscale;
		if (model->ChartXYLineConfig.YScaleSet.UpperLimit == model->ChartXYLineConfig.YScaleSet.LowerLimit)
			yscale.A = 0;
		else
			yscale.A = (DOUBLE)(yaxislength) / (model->ChartXYLineConfig.YScaleSet.UpperLimit - model->ChartXYLineConfig.YScaleSet.LowerLimit);
		yscale.B = 0 - (yscale.A * model->ChartXYLineConfig.YScaleSet.LowerLimit);
		

		XJDataType tpx = UIDataService::Ins().GetDataType(channelinfo.StartXAddr);
		XJDataType tpy;
		if (channelinfo.IsXYDataFromSameSource)
			tpy = tpx;
		else
			tpy = UIDataService::Ins().GetDataType(channelinfo.StartYAddr);

		GraphicDrawHandle::PushClip(origin.X + model->ChartXYLineConfig.OffX,
			origin.Y - scrollheight_ - yaxislength + model->ChartXYLineConfig.OffY, 
			Xaxislength,yaxislength);
		{
			switch (channelinfo.CurveStyle)
			{
			case Project::Line:
			{
				fl_begin_line();
				{
					;
					for (int i = startnum; i < startnum + num; i++)
					{
						fl_color(linecolor);			//设置线颜色
						fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//设置线样式
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy) + model->ChartXYLineConfig.OffY);
					}
				}
				fl_end_line();
				break;
			}
			case Project::LineDot:
			{
				currentdotColor_ = fl_rgb_color(RGBColor(channelinfo.DrawDotColor));
				fl_begin_line();
				{
					fl_color(linecolor);			//设置线颜色
					for (int i = startnum; i < num + startnum; i++)
					{
						fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//设置线样式
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					}
				}
				fl_end_line();
				fl_color(currentdotColor_);
				for (size_t i = (size_t)startnum; i < (size_t)(num + startnum); i++)
					DrawDot(channelinfo.DrawDotStyle, origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
						origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy), channelinfo.DrawDotSize);

				break;
			}
			case Project::Dot:
			{
				currentdotColor_ = fl_rgb_color(RGBColor(channelinfo.DrawDotColor));
				fl_color(currentdotColor_);

				fl_begin_line();
				{
					for (int i = startnum; i < num + startnum; i++)
						DrawDot(channelinfo.DrawDotStyle, origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy), channelinfo.DrawDotSize);
				}
				fl_end_line();
				break;
			}
			case Project::Xshadow:
			{
				fl_begin_complex_polygon();
				{
					fl_color(linecolor);										//设置线颜色
					fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//设置线样式

					fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][startnum], tpx), 
						origin.Y + model->ChartXYLineConfig.OffY - scrollheight_);
					for (int i = startnum; i < num + startnum; i++)
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][startnum + num - 1], tpx),
						origin.Y + model->ChartXYLineConfig.OffY - scrollheight_);

				}
				fl_end_complex_polygon();
				break;
			}
			case Project::Yshadow:
			{
				fl_begin_complex_polygon();
				{
					fl_color(linecolor);										//设置线颜色
					fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//设置线样式

					fl_vertex(origin.X + model->ChartXYLineConfig.OffX, 
						origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][startnum], tpy));
					for (int i = startnum; i < num + startnum; i++)
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					fl_vertex(origin.X + model->ChartXYLineConfig.OffX, 
						origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][startnum + num - 1], tpy));

				}
				fl_end_complex_polygon();
				break;
			}
			default:
				break;
			}
		}
		fl_pop_clip();
	}
	void XYLineChartView::DrawRenderChart()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();
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
		for (size_t j = 0; j < model->ChartXYLineConfig.DataChanel.size(); j++)
		{
			Fl_Color linecolor = fl_rgb_color(RGBColor(model->ChartXYLineConfig.DataChanel[j].LineColor));
			GraphicDrawHandle::PushClip(origin.X + model->ChartXYLineConfig.OffX,
				origin.Y - scrollheight_ - yaxislength + model->ChartXYLineConfig.OffY,
				Xaxislength, yaxislength);
			{
				switch (model->ChartXYLineConfig.DataChanel[j].CurveStyle)
				{
				case Project::CurveLine:
				case Project::Line:
				{
					fl_color(linecolor);			//设置线颜色
					fl_begin_line();
					fl_line_style(model->ChartXYLineConfig.DataChanel[j].LineStyle, model->ChartXYLineConfig.DataChanel[j].LineSize);	//设置线样式

					for (size_t i = 0;i < xpos.size(); i++)
					{

						if (i == 0 || i == xpos.size() - 1)
						{
							fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i],
								origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10);
						}
						else
						{
							fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10,
								origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]);
						}
					}
					fl_end_line();
					break;
				}
				case Project::LineDot:
				{
					currentdotColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.DataChanel[j].DrawDotColor));
					fl_begin_line();
					{
						fl_color(linecolor);			//设置线颜色
						for (size_t i = 0;i < xpos.size(); i++)
						{
							fl_line_style(model->ChartXYLineConfig.DataChanel[j].LineStyle, model->ChartXYLineConfig.DataChanel[j].LineSize);	//设置线样式

							if (i == 0 || i == xpos.size() - 1)
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i],
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							else
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]);
						}
					}
					fl_end_line();
					fl_color(currentdotColor_);
					for (size_t i = 0;i < xpos.size(); i++)
					{
						if (i == 0 || i == xpos.size() - 1)
							DrawDot(model->ChartXYLineConfig.DataChanel[j].DrawDotStyle,
								(int)(origin.X + model->ChartXYLineConfig.OffX + xpos[i]),
								(int)(origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10),
								model->ChartXYLineConfig.DataChanel[j].DrawDotSize);
						else
							DrawDot(model->ChartXYLineConfig.DataChanel[j].DrawDotStyle, 
								(int)(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10),
								(int)(origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]),
								model->ChartXYLineConfig.DataChanel[j].DrawDotSize);
					}
					break;
				}
				case Project::Dot:
				{
					currentdotColor_ = fl_rgb_color(RGBColor(model->ChartXYLineConfig.DataChanel[j].DrawDotColor));
					fl_color(currentdotColor_);
					fl_begin_line();
					{
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
								DrawDot(model->ChartXYLineConfig.DataChanel[j].DrawDotStyle,
									(int)(origin.X + model->ChartXYLineConfig.OffX + xpos[i]),
									(int)(origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10),
									model->ChartXYLineConfig.DataChanel[j].DrawDotSize);
							else
								DrawDot(model->ChartXYLineConfig.DataChanel[j].DrawDotStyle,
									(int)(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10),
									(int)(origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]),
									model->ChartXYLineConfig.DataChanel[j].DrawDotSize);
						}
					}
					fl_end_line();
					break;
				}
				case Project::Xshadow:
				{
					fl_color(linecolor);										//设置线颜色
					fl_begin_complex_polygon();
					{
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[0],
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_);
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
							{
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i],
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							}
							else
							{
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]);
							}
						}
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[xpos.size() - 1] + j * 10,
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_);

					}
					fl_end_complex_polygon();
					break;
				}
				case Project::Yshadow:
				{
					fl_begin_complex_polygon();
					{
						fl_color(linecolor);										//设置线颜色
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX,
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[0] - j * 10);
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
							{
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i],
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							}
							else
							{
								fl_vertex(origin.X + model->ChartXYLineConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[i]);
							}
						}
						fl_vertex(origin.X + model->ChartXYLineConfig.OffX,
							origin.Y + model->ChartXYLineConfig.OffY - scrollheight_ - ypos[ypos.size() - 1]);

					}
					fl_end_complex_polygon();
					break;
				}
				default:
					break;
				}
			}
			fl_pop_clip();
		}
	}
	/*滚动条回调*/
	void XYLineScrollCb(Fl_Widget* w, void* data) {
		XYLineChartView* o = (XYLineChartView*)data;
		if (o->Hscrollbar->value() != o->Xaxislength)
			o->ScrollClick = 1;
		else
			o->ScrollClick = 0;
		o->redraw();
		o->ScrollValue = o->Hscrollbar->value();
		printf("value = %d\n", o->Hscrollbar->value());
	}
	void XYLineChartView::draw()
	{
		shared_ptr<XYLineChartModel> model = BaseView.GetModel<XYLineChartModel>();

		//绘制刻度背景
		draw_box((Fl_Boxtype)1, model->ChartXYLineConfig.X + model->ChartXYLineConfig.OffX, 
			model->ChartXYLineConfig.Y + model->ChartXYLineConfig.OffY,
			model->ChartXYLineConfig.Width, model->ChartXYLineConfig.Height, scaleBgColor_);
		//绘制趋势图背景
		draw_box((Fl_Boxtype)1, origin.X + model->ChartXYLineConfig.OffX, 
			origin.Y - scrollheight_ - yaxislength + model->ChartXYLineConfig.OffY, Xaxislength ,
			yaxislength, chartBgColor_);
		//绘制栅格
		if (useGrid)
			DrawGrid();
		
		//确定当前页面
		if (model->ChartXYLineConfig.PageControlStyle == 0)	
		{
			//配置滚动条属性
			//Fl::scrollbar_size(scrollheight_);
			Hscrollbar->resize(origin.X + model->ChartXYLineConfig.OffX,
				origin.Y - scrollheight_ + model->ChartXYLineConfig.OffY, Xaxislength, scrollheight_);
			Hscrollbar->type(FL_HORIZONTAL);
			Hscrollbar->callback(XYLineScrollCb, (void*)this);
			Hscrollbar->bounds(0, Xaxislength);
			float hscrolltab = (float)perScreendot_ / (float)SampleNum;
			Hscrollbar->slider_size(hscrolltab);
			Hscrollbar->value(ScrollValue);
			Hscrollbar->damage(FL_DAMAGE_ALL);

			//确定当前页面
			startnum_ = ScrollValue * (SampleNum - perScreendot_) / Xaxislength;
		}
		else
		{
			//从寄存器获取当前页面值
			startnum_ = (PageNo - 1) * perScreendot_;
		}
		Fl_Group::draw();
		//绘制折线图
		if (!XData.empty() && !YData.empty())
		{
			//for (unsigned i = 0; i < model->ChartXYLineConfig.ChannelNum; i++)
			for (unsigned i = 0; i < XData.size(); i++)
			{
				if (!XData[i].empty())
				{
					startnum_ = startnum_ < 0 ? 0 : startnum_;
					int num = XData[i].size() - startnum_;
					if (num > perScreendot_)
						num = perScreendot_;
					if(i < model->ChartXYLineConfig.DataChanel.size())
						DrawChannelXYLine(model->ChartXYLineConfig.DataChanel[i], startnum_, num);
				}
			}
		}
		//绘制坐标轴
		if (IResourceService::Ins()->IsRenderMode())
			DrawRenderChart();
		if (useXAxis)
			DrawXaxis();
		if (useYAxis)
			DrawYaxis();
		//绘制参考线
		DrawReferenceLine();
	}
}

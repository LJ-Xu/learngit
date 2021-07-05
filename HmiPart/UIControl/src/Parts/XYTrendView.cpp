#include "GraphicDrawHandle.h"
#include "XYTrendView.h"
#include "XYTrendView.h"
#include "XYTrendControl.h"
#include "XYTrendModel.h"
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iomanip>
#include "IResourceService.h"
#include "DataInfo.h"
#include "DataFormate.h"
#include "UIDataService.h"
namespace UI
{
	XYTrendView::XYTrendView(int X, int Y, int W, int H, const char* L) : HMIBaseGroup(X, Y, W, H, L) {
		Fl_Group::end();		// end the group's begin()
	}
	XYTrendView::~XYTrendView() {}

	/*��ʼ�����Ʋ���*/
	void XYTrendView::InitDraw()
	{
		std::shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		chartBgColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.LineChartBackColor));
		scaleBgColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.ScaleAreaBackColor));
		useGrid = model->TrendXYConfig.UseGrid;
		useXAxis = model->TrendXYConfig.UseXScale;
		useYAxis = model->TrendXYConfig.UseYScale;
		if (model->TrendXYConfig.XScaleSet.MarkType != Project::ScaleMarkType::NullSacle)
		{
			xscalefontSize_ = model->TrendXYConfig.XScaleSet.ScaleMarkSize;
			xscalefontStyle_ = UI::IResourceService::GetFontIdx(model->TrendXYConfig.XScaleSet.ScaleMarkFont);
			xscalefontColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.XScaleSet.ScaleMarkColor));
		}
		xscaleInterval_ = model->TrendXYConfig.XaxisLen / model->TrendXYConfig.XScaleSet.PrimaryScaleNum;

		if (model->TrendXYConfig.YScaleSet.MarkType != Project::ScaleMarkType::NullSacle)
		{
			yscalefontSize_ = model->TrendXYConfig.YScaleSet.ScaleMarkSize;
			yscalefontStyle_ = UI::IResourceService::GetFontIdx(model->TrendXYConfig.YScaleSet.ScaleMarkFont);
			yscalefontColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.YScaleSet.ScaleMarkColor));
		}

		perScreendot_ = model->TrendXYConfig.PointsPerScreen;			//ÿ������
		xaxislength = model->TrendXYConfig.XaxisLen;
		yaxislength = model->TrendXYConfig.YaxisLen;
		origin.X = model->TrendXYConfig.OriginPos.X;
		origin.Y = model->TrendXYConfig.OriginPos.Y;
		scrollheight_ = 0;
	}

	void XYTrendView::DrawGrid()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		fl_color(active() ? fl_rgb_color(RGBColor(model->TrendXYConfig.GridStyle.Color))
			: fl_inactive(fl_rgb_color(RGBColor(model->TrendXYConfig.GridStyle.Color))));
		fl_line_style(model->TrendXYConfig.GridStyle.Type, model->TrendXYConfig.GridStyle.Weight);//����դ�� ��ʽ ���
		double xSpaceInterval, ySpaceInterval;
		xSpaceInterval = (double)model->TrendXYConfig.XaxisLen / (double)model->TrendXYConfig.XGridEqualNum; //X��դ����
		ySpaceInterval = (double)model->TrendXYConfig.YaxisLen / (double)model->TrendXYConfig.YGridEqualNum;	//Y��դ����
		for (int i = 1; i < model->TrendXYConfig.XGridEqualNum; i++)	//����X��դ��	
		{
			fl_begin_line();
			fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->TrendXYConfig.OffX,
				(double)model->TrendXYConfig.OriginPos.Y - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
			fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)(xSpaceInterval * (double)i) + (double)model->TrendXYConfig.OffX,
				(double)model->TrendXYConfig.OriginPos.Y - (double)scrollheight_ - (double)model->TrendXYConfig.YaxisLen + (double)model->TrendXYConfig.OffY);
			fl_end_line();
		}
		for (int i = 1; i < model->TrendXYConfig.YGridEqualNum; i++)	//����X��դ��	
		{
			fl_begin_line();
			fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)model->TrendXYConfig.OffX,
				(double)model->TrendXYConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
			fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)model->TrendXYConfig.XaxisLen + (double)model->TrendXYConfig.OffX,
				(double)model->TrendXYConfig.OriginPos.Y - (double)(ySpaceInterval * (double)i) - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);

			fl_end_line();
		}
	}
	/*ת��������ʾ��ʽ*/
	string XYTrendView::ChangeDisplayFormat(float value, Project::ScaleMark style)
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		int integer = style.IntegerNum, decimal = style.DecimalnNum;
		string data = to_string(value);
		string intString = "", floatString;
		int pos = data.find(".");
		if (pos == 0)	//С������ͷ��
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
		//ȥ��ͷ������0������С����ǰ0
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
			if (style.MarkType == Project::ScaleMarkType::PercentSacle)
				maxString += "%";
			return maxString;
		}
		if (decimal)
		{
			if (style.MarkType == Project::ScaleMarkType::PercentSacle)
				return intString + "." + floatString + "%";
			else if (style.MarkType == Project::ScaleMarkType::NumSacle)
				return intString + "." + floatString;
		}
		else
		{
			if (style.MarkType == Project::ScaleMarkType::PercentSacle)
				return intString + "%";
			else if (style.MarkType == Project::ScaleMarkType::NumSacle)
				return intString;
		}
		return intString;
	}


	void XYTrendView::DrawYaxis()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		double spaceInterval;
		fl_color(active() ? fl_rgb_color(RGBColor(model->TrendXYConfig.YScaleSet.ScaleColor))
			: fl_inactive(fl_rgb_color(RGBColor(model->TrendXYConfig.YScaleSet.ScaleColor)))); //���ÿ̶��ߵ���ɫ  
		fl_line_style(0, 0);													//���ÿ̶�����ʽ

		/*����X��*/
		fl_line(model->TrendXYConfig.OriginPos.X + model->TrendXYConfig.OffX,
			model->TrendXYConfig.OriginPos.Y - scrollheight_ + model->TrendXYConfig.OffY,
			model->TrendXYConfig.OriginPos.X + model->TrendXYConfig.OffX,
			model->TrendXYConfig.OriginPos.Y - model->TrendXYConfig.YaxisLen - scrollheight_ + model->TrendXYConfig.OffY);

		/*����X��̶�*/
		spaceInterval = (double)model->TrendXYConfig.YaxisLen / (double)(model->TrendXYConfig.YScaleSet.PrimaryScaleNum * model->TrendXYConfig.YScaleSet.SecondaryScaleNum); //�̶ȼ��

		for (int i = 0; i < model->TrendXYConfig.YScaleSet.PrimaryScaleNum * model->TrendXYConfig.YScaleSet.SecondaryScaleNum + 1; i++)	//���ƿ̶���	
		{
			if (i % model->TrendXYConfig.YScaleSet.SecondaryScaleNum == 0)         //���̶�
			{
				fl_begin_line();
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y - spaceInterval * (double)i - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
				fl_vertex((double)model->TrendXYConfig.OriginPos.X - (double)model->TrendXYConfig.YScaleSet.PrimaryScaleLen + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y - spaceInterval * (double)i - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
				fl_end_line();
			}
			else if (model->TrendXYConfig.YScaleSet.UseSecondaryScaleNum)//���̶�
			{
				fl_begin_line();
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + (double)model->TrendXYConfig.OffX,
				(double)model->TrendXYConfig.OriginPos.Y - spaceInterval * (double)i - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
				fl_vertex((double)model->TrendXYConfig.OriginPos.X - (double)model->TrendXYConfig.YScaleSet.SecondaryScaleLen + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y - spaceInterval * (double)i - (double)scrollheight_ + (double)model->TrendXYConfig.OffY);
				fl_end_line();
			}
		}
		/*��������*/
		float numInterval;
		if (model->TrendXYConfig.YScaleSet.MarkType == Project::ScaleMarkType::PercentSacle)		//�ٷֱ�
			numInterval = 100.0 / (float)model->TrendXYConfig.YScaleSet.PrimaryScaleNum;
		else
			numInterval = (float)(model->TrendXYConfig.YScaleSet.UpperLimit - model->TrendXYConfig.YScaleSet.LowerLimit) / (float)model->TrendXYConfig.YScaleSet.PrimaryScaleNum;

		if (model->TrendXYConfig.YScaleSet.MarkType != Project::ScaleMarkType::NullSacle)
		{
			/*��������*/
			fl_font(yscalefontStyle_, yscalefontSize_);
			/*����������ɫ*/
			fl_color(active() ? yscalefontColor_ : fl_inactive(yscalefontColor_)); 
			for (size_t i = 0; i < model->TrendXYConfig.YScaleInfo.size(); i++)
			{
				float num;
				if (model->TrendXYConfig.YScaleSet.MarkType == Project::ScaleMarkType::PercentSacle)
				{
					num = numInterval * (float)i;
					if (i == model->TrendXYConfig.YScaleInfo.size() - 1)
						num = 100.0;
				}
				else if (model->TrendXYConfig.YScaleSet.MarkType == Project::ScaleMarkType::NumSacle)
				{
					num = numInterval * (float)i + (float)model->TrendXYConfig.YScaleSet.LowerLimit;
					if (i == model->TrendXYConfig.YScaleInfo.size() - 1)
						num = (float)model->TrendXYConfig.YScaleSet.UpperLimit;
				}
				if (i == model->TrendXYConfig.YScaleInfo.size() - 1)
					num = (float)model->TrendXYConfig.YScaleSet.UpperLimit;
				model->TrendXYConfig.YScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, model->TrendXYConfig.YScaleSet);
				fl_draw(model->TrendXYConfig.YScaleInfo[i].ScaleContent.data(),
					model->TrendXYConfig.YScaleInfo[i].Coordinate.X + model->TrendXYConfig.OffX,
					model->TrendXYConfig.YScaleInfo[i].Coordinate.Y + model->TrendXYConfig.OffY,
					model->TrendXYConfig.YScaleInfo[i].Width, model->TrendXYConfig.YScaleInfo[i].Height, FL_ALIGN_CENTER);
			}
		}
	}
	void XYTrendView::DrawDot(Project::DotStyle style, int x, int y, int size)
	{
		switch (style)
		{
		case Project::FilledRect:
		{
			x -= size / 2;
			y -= size / 2;
			fl_rectf(x, y, size, size, active() ? currentdotColor_ : fl_inactive(currentdotColor_));
			break;
		}
		case Project::FilledTriangle:
		{
			double a = cos(30.0 * 3.1415926 / 180.0) * (double)size / 3.0;
			fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));

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
			fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
			fl_pie(x, y, size, size, 0, 360);
			break;
		}
		case Project::Rect:
		{
			x -= size / 2;
			y -= size / 2;
			fl_rect(x, y, size, size, active() ? currentdotColor_ : fl_inactive(currentdotColor_));
			break;
		}
		case Project::Triangel:
		{
			double a = cos(30 / 180 * 3.1415926) * size / 3;
			fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
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
			fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
			fl_arc(x, y, size, size, 0, 360);
			break;
		}
		case Project::Fork:
		{
			fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
			fl_line_style(0, 0);											 //���ÿ̶�����ʽ
			fl_line(x - size / 2, y - size / 2, x + size / 2, y + size / 2);
			fl_line(x - size / 2, y + size / 2, x + size / 2, y - size / 2);
			break;
		}
		default:
			break;
		}
	}

	void XYTrendView::DrawReferenceLine()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		GraphicDrawHandle::PushClip(origin.X + model->TrendXYConfig.OffX,
			origin.Y - scrollheight_ - yaxislength + model->TrendXYConfig.OffY,
			xaxislength, yaxislength);
		{
			for (unsigned i = 0; i < model->TrendXYConfig.ReferenceLines.size(); i++)
			{
				fl_color(active() ? fl_rgb_color(RGBColor(model->TrendXYConfig.ReferenceLines[i].LineColor))
					: fl_inactive(fl_rgb_color(RGBColor(model->TrendXYConfig.ReferenceLines[i].LineColor))));
				fl_line_style(0, 0);
				if (model->TrendXYConfig.ReferenceLines[i].LineMode)		//��
				{
					fl_begin_points();
					for (size_t j = 0; j < model->TrendXYConfig.ReferenceLines[i].Pts.size(); j++)
					{
						double y = (model->TrendXYConfig.ReferenceLines[i].Pts[j].Y - model->TrendXYConfig.YScaleSet.LowerLimit) * (float)yaxislength /
							(model->TrendXYConfig.YScaleSet.UpperLimit - model->TrendXYConfig.YScaleSet.LowerLimit);
						double x = model->TrendXYConfig.ReferenceLines[i].Pts[j].X * (float)xaxislength /
							(model->TrendXYConfig.XScaleSet.UpperLimit - model->TrendXYConfig.XScaleSet.LowerLimit);
						fl_vertex(origin.X + model->TrendXYConfig.OffX + x, origin.Y - scrollheight_ - y);
					}
					fl_end_points();
				}
				else
				{
					fl_begin_line();
					for (size_t j = 0; j < model->TrendXYConfig.ReferenceLines[i].Pts.size(); j++)
					{
						double y = model->TrendXYConfig.ReferenceLines[i].Pts[j].Y * (float)yaxislength /
							(model->TrendXYConfig.YScaleSet.UpperLimit - model->TrendXYConfig.YScaleSet.LowerLimit);
						double x = model->TrendXYConfig.ReferenceLines[i].Pts[j].X * (float)xaxislength /
							(model->TrendXYConfig.XScaleSet.UpperLimit - model->TrendXYConfig.XScaleSet.LowerLimit);
						fl_vertex(origin.X + model->TrendXYConfig.OffX + x, origin.Y - scrollheight_ - y);
					}
					fl_end_line();
				}
				//int y0 = (int)(model->TrendXYConfig.ReferenceLines[i].LineValue * (float)yaxislength /
				//	(model->TrendXYConfig.YScaleSet.UpperLimit - model->TrendXYConfig.YScaleSet.LowerLimit));
				//fl_line(origin.X + model->TrendXYConfig.OffX,
				//	origin.Y + model->TrendXYConfig.OffY - scrollheight_ - y0,
				//	origin.X + model->TrendXYConfig.OffX + xaxislength,
				//	origin.Y + model->TrendXYConfig.OffY - scrollheight_ - y0);
			}
		}
		fl_pop_clip();
	}

	void XYTrendView::DrawXaxis()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		double spaceInterval;
		fl_color(active() ? fl_rgb_color(RGBColor(model->TrendXYConfig.XScaleSet.ScaleColor))
			: fl_inactive(fl_rgb_color(RGBColor(model->TrendXYConfig.XScaleSet.ScaleColor))));
		fl_line_style(0, 0);											 //���ÿ̶�����ʽ

		/*����X��*/
		fl_line(model->TrendXYConfig.OriginPos.X + model->TrendXYConfig.OffX,
			model->TrendXYConfig.OriginPos.Y + model->TrendXYConfig.OffY,
			model->TrendXYConfig.OriginPos.X + model->TrendXYConfig.XaxisLen + model->TrendXYConfig.OffX,
			model->TrendXYConfig.OriginPos.Y + model->TrendXYConfig.OffY);

		/*����X��̶�*/
		spaceInterval = (double)model->TrendXYConfig.XaxisLen / (double)(model->TrendXYConfig.XScaleSet.PrimaryScaleNum * model->TrendXYConfig.XScaleSet.SecondaryScaleNum); //�̶ȼ��

		for (int i = 0; i < model->TrendXYConfig.XScaleSet.PrimaryScaleNum * model->TrendXYConfig.XScaleSet.SecondaryScaleNum + 1; i++)	//���ƿ̶���	
		{
			if (i % model->TrendXYConfig.XScaleSet.SecondaryScaleNum == 0)         //���̶�
			{
				fl_begin_line();
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + spaceInterval * (double)i + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y + (double)model->TrendXYConfig.OffY);
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + spaceInterval * (double)i + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y + (double)model->TrendXYConfig.XScaleSet.PrimaryScaleLen + (double)model->TrendXYConfig.OffY);
				fl_end_line();
			}
			else if (model->TrendXYConfig.XScaleSet.UseSecondaryScaleNum)//���̶�
			{
				fl_begin_line();
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + spaceInterval * (double)i + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y + (double)model->TrendXYConfig.OffY);
				fl_vertex((double)model->TrendXYConfig.OriginPos.X + spaceInterval * (double)i + (double)model->TrendXYConfig.OffX,
					(double)model->TrendXYConfig.OriginPos.Y + (double)model->TrendXYConfig.XScaleSet.SecondaryScaleLen + (double)model->TrendXYConfig.OffY);
				fl_end_line();
			}
		}
		/*��������*/
		float numInterval;
		if (model->TrendXYConfig.XScaleSet.MarkType == Project::ScaleMarkType::PercentSacle)		//�ٷֱ�
			numInterval = 100.0 / (float)model->TrendXYConfig.XScaleSet.PrimaryScaleNum;
		else
			numInterval = (float)(model->TrendXYConfig.XScaleSet.UpperLimit - model->TrendXYConfig.XScaleSet.LowerLimit) / (float)model->TrendXYConfig.XScaleSet.PrimaryScaleNum;
		if (model->TrendXYConfig.XScaleSet.MarkType != Project::ScaleMarkType::NullSacle)
		{
			/*��������*/
			fl_font(xscalefontStyle_, xscalefontSize_);
			/*����������ɫ*/
			fl_color(active() ? xscalefontColor_ : fl_inactive(xscalefontColor_));
			for (size_t i = 0; i < model->TrendXYConfig.XScaleInfo.size(); i++)
			{
				float num;
				if (model->TrendXYConfig.XScaleSet.MarkType == Project::ScaleMarkType::PercentSacle)
				{
					num = numInterval * (float)i;
					if (i == model->TrendXYConfig.XScaleInfo.size() - 1)
						num = 100.0;
				}
				else if (model->TrendXYConfig.XScaleSet.MarkType == Project::ScaleMarkType::NumSacle)
				{
					num = numInterval * (float)i + (float)model->TrendXYConfig.XScaleSet.LowerLimit;
					if (i == model->TrendXYConfig.XScaleInfo.size() - 1)
						num = (float)model->TrendXYConfig.XScaleSet.UpperLimit;
				}
				if (i == model->TrendXYConfig.XScaleInfo.size() - 1)
					num = (float)model->TrendXYConfig.XScaleSet.UpperLimit;
				model->TrendXYConfig.XScaleInfo[i].ScaleContent = ChangeDisplayFormat(num, model->TrendXYConfig.XScaleSet);
				fl_draw(model->TrendXYConfig.XScaleInfo[i].ScaleContent.data(),
					model->TrendXYConfig.XScaleInfo[i].Coordinate.X + model->TrendXYConfig.OffX,
					model->TrendXYConfig.XScaleInfo[i].Coordinate.Y + model->TrendXYConfig.OffY,
					model->TrendXYConfig.XScaleInfo[i].Width, model->TrendXYConfig.XScaleInfo[i].Height, FL_ALIGN_CENTER);
			}
		}
	}

	void XYTrendView::DrawChannelXYTrend(Project::XYLineChannel channelinfo, int startnum, int num)
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();

		Fl_Color linecolor = fl_rgb_color(RGBColor(channelinfo.LineColor));

		Utility::ScaleInfo xscale;
		if (model->TrendXYConfig.XScaleSet.UpperLimit == model->TrendXYConfig.XScaleSet.LowerLimit)
			xscale.A = 0;
		else
			xscale.A = (DOUBLE)(xaxislength) / (model->TrendXYConfig.XScaleSet.UpperLimit - model->TrendXYConfig.XScaleSet.LowerLimit);
		xscale.B = 0 - (xscale.A *  model->TrendXYConfig.XScaleSet.LowerLimit);

		Utility::ScaleInfo yscale;
		if (model->TrendXYConfig.YScaleSet.UpperLimit == model->TrendXYConfig.YScaleSet.LowerLimit)
			yscale.A = 0;
		else
			yscale.A = (DOUBLE)(yaxislength) / (model->TrendXYConfig.YScaleSet.UpperLimit - model->TrendXYConfig.YScaleSet.LowerLimit);
		yscale.B = 0 - (yscale.A * model->TrendXYConfig.YScaleSet.LowerLimit);


		XJDataType tpx = UIDataService::Ins().GetDataType(channelinfo.StartXAddr);
		XJDataType tpy;
		if (channelinfo.IsXYDataFromSameSource)
			tpy = tpx;
		else
			tpy = UIDataService::Ins().GetDataType(channelinfo.StartYAddr);

		GraphicDrawHandle::PushClip(origin.X + model->TrendXYConfig.OffX,
			origin.Y - scrollheight_ - yaxislength + model->TrendXYConfig.OffY,
			xaxislength, yaxislength);
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
						fl_color(active() ? linecolor : fl_inactive(linecolor));
						fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//��������ʽ
						fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy) + model->TrendXYConfig.OffY);
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
					fl_color(active() ? linecolor : fl_inactive(linecolor));
					for (int i = startnum; i < num + startnum; i++)
					{
						fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//��������ʽ
						fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					}
				}
				fl_end_line();
				fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
				for (size_t i = (size_t)startnum; i < (size_t)(num + startnum); i++)
					DrawDot(channelinfo.DrawDotStyle, origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
						origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy), channelinfo.DrawDotSize);

				break;
			}
			case Project::Dot:
			{
				currentdotColor_ = fl_rgb_color(RGBColor(channelinfo.DrawDotColor));
				fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));

				fl_begin_line();
				{
					for (int i = startnum; i < num + startnum; i++)
						DrawDot(channelinfo.DrawDotStyle, origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy), channelinfo.DrawDotSize);
				}
				fl_end_line();
				break;
			}
			case Project::Xshadow:
			{
				fl_begin_complex_polygon();
				{
					fl_color(active() ? linecolor : fl_inactive(linecolor));
					fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//��������ʽ

					fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][startnum], tpx),
						origin.Y + model->TrendXYConfig.OffY - scrollheight_);
					for (int i = startnum; i < num + startnum; i++)
						fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][startnum + num - 1], tpx),
						origin.Y + model->TrendXYConfig.OffY - scrollheight_);

				}
				fl_end_complex_polygon();
				break;
			}
			case Project::Yshadow:
			{
				fl_begin_complex_polygon();
				{
					fl_color(active() ? linecolor : fl_inactive(linecolor));
					fl_line_style(channelinfo.LineStyle, channelinfo.LineSize);	//��������ʽ

					fl_vertex(origin.X + model->TrendXYConfig.OffX,
						origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][startnum], tpy));
					for (int i = startnum; i < num + startnum; i++)
						fl_vertex(origin.X + model->TrendXYConfig.OffX + Utility::DataFormate::GetScale(xscale, XData[channelinfo.Index - 1][i], tpx),
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][i], tpy));
					fl_vertex(origin.X + model->TrendXYConfig.OffX,
						origin.Y + model->TrendXYConfig.OffY - scrollheight_ - Utility::DataFormate::GetScale(yscale, YData[channelinfo.Index - 1][startnum + num - 1], tpy));

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
	void XYTrendView::DrawRenderChart()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		double xspaceInterval = (double)xaxislength / 4.0;
		vector<double> xpos;
		vector<double> ypos;
		for (size_t i = 0; i < 5; i++)
			xpos.push_back((double)i * xspaceInterval);
		ypos.push_back((double)yaxislength / 2.0);
		ypos.push_back((double)yaxislength / 4.0);
		ypos.push_back((double)yaxislength / 2.0);
		ypos.push_back((double)yaxislength * 3.0 / 4.0);
		ypos.push_back((double)yaxislength / 2.0);
		for (size_t j = 0; j < model->TrendXYConfig.DataChanel.size(); j++)
		{
			Fl_Color linecolor = fl_rgb_color(RGBColor(model->TrendXYConfig.DataChanel[j].LineColor));
			GraphicDrawHandle::PushClip(origin.X + model->TrendXYConfig.OffX,
				origin.Y - scrollheight_ - yaxislength + model->TrendXYConfig.OffY,
				xaxislength, yaxislength);
			{
				switch (model->TrendXYConfig.DataChanel[j].CurveStyle)
				{
				case Project::CurveLine:
				case Project::Line:
				{
					fl_color(active() ? linecolor : fl_inactive(linecolor));			//��������ɫ
					fl_begin_line();
					fl_line_style(model->TrendXYConfig.DataChanel[j].LineStyle, model->TrendXYConfig.DataChanel[j].LineSize);	//��������ʽ

					for (size_t i = 0;i < xpos.size(); i++)
					{

						if (i == 0 || i == xpos.size() - 1)
						{
							fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i],
								origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10);
						}
						else
						{
							fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10,
								origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]);
						}
					}
					fl_end_line();
					break;
				}
				case Project::LineDot:
				{
					currentdotColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.DataChanel[j].DrawDotColor));
					fl_begin_line();
					{
						fl_color(active() ? linecolor : fl_inactive(linecolor));			//��������ɫ
						for (size_t i = 0;i < xpos.size(); i++)
						{
							fl_line_style(model->TrendXYConfig.DataChanel[j].LineStyle, model->TrendXYConfig.DataChanel[j].LineSize);	//��������ʽ

							if (i == 0 || i == xpos.size() - 1)
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i],
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							else
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]);
						}
					}
					fl_end_line();
					fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
					for (size_t i = 0;i < xpos.size(); i++)
					{
						if (i == 0 || i == xpos.size() - 1)
							DrawDot(model->TrendXYConfig.DataChanel[j].DrawDotStyle,
								(int)(origin.X + model->TrendXYConfig.OffX + xpos[i]),
								(int)(origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10),
								model->TrendXYConfig.DataChanel[j].DrawDotSize);
						else
							DrawDot(model->TrendXYConfig.DataChanel[j].DrawDotStyle,
								(int)(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10),
								(int)(origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]),
								model->TrendXYConfig.DataChanel[j].DrawDotSize);
					}
					break;
				}
				case Project::Dot:
				{
					currentdotColor_ = fl_rgb_color(RGBColor(model->TrendXYConfig.DataChanel[j].DrawDotColor));
					fl_color(active() ? currentdotColor_ : fl_inactive(currentdotColor_));
					fl_begin_line();
					{
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
								DrawDot(model->TrendXYConfig.DataChanel[j].DrawDotStyle,
									(int)(origin.X + model->TrendXYConfig.OffX + xpos[i]),
									(int)(origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10),
									model->TrendXYConfig.DataChanel[j].DrawDotSize);
							else
								DrawDot(model->TrendXYConfig.DataChanel[j].DrawDotStyle,
									(int)(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10),
									(int)(origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]),
									model->TrendXYConfig.DataChanel[j].DrawDotSize);
						}
					}
					fl_end_line();
					break;
				}
				case Project::Xshadow:
				{
					fl_color(active() ? linecolor : fl_inactive(linecolor));										//��������ɫ
					fl_begin_complex_polygon();
					{
						fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[0],
							origin.Y + model->TrendXYConfig.OffY - scrollheight_);
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
							{
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i],
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							}
							else
							{
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]);
							}
						}
						fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[xpos.size() - 1] + j * 10,
							origin.Y + model->TrendXYConfig.OffY - scrollheight_);

					}
					fl_end_complex_polygon();
					break;
				}
				case Project::Yshadow:
				{
					fl_begin_complex_polygon();
					{
						fl_color(active() ? linecolor : fl_inactive(linecolor));										//��������ɫ
						fl_vertex(origin.X + model->TrendXYConfig.OffX,
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[0] - j * 10);
						for (size_t i = 0;i < xpos.size(); i++)
						{
							if (i == 0 || i == xpos.size() - 1)
							{
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i],
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i] - j * 10);
							}
							else
							{
								fl_vertex(origin.X + model->TrendXYConfig.OffX + xpos[i] + j * 10,
									origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[i]);
							}
						}
						fl_vertex(origin.X + model->TrendXYConfig.OffX,
							origin.Y + model->TrendXYConfig.OffY - scrollheight_ - ypos[ypos.size() - 1]);

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


	void XYTrendView::draw()
	{
		shared_ptr<XYTrendModel> model = BaseView.GetModel<XYTrendModel>();
		if (IResourceService::Ins()->IsRenderMode())
			InitDraw();
		//���ƿ̶ȱ���
		draw_box((Fl_Boxtype)1, model->TrendXYConfig.X + model->TrendXYConfig.OffX,
			model->TrendXYConfig.Y + model->TrendXYConfig.OffY,
			model->TrendXYConfig.Width, model->TrendXYConfig.Height, scaleBgColor_);
		//��������ͼ����
		draw_box((Fl_Boxtype)1, origin.X + model->TrendXYConfig.OffX,
			origin.Y - scrollheight_ - yaxislength + model->TrendXYConfig.OffY, xaxislength,
			yaxislength, chartBgColor_);
		//����դ��
		if (useGrid)
			DrawGrid();

		Fl_Group::draw();
		//��������ͼ
		if (!XData.empty() && !YData.empty())
		{
			for (unsigned i = 0; i < XData.size(); i++)
			{
				if (!XData[i].empty())
				{
					int startnum, num;
					if (XData[i].size() > (size_t)model->TrendXYConfig.SamplePoints)
					{
						startnum = XData[i].size() - model->TrendXYConfig.SamplePoints;
						num = model->TrendXYConfig.SamplePoints;
					}
					else
					{
						startnum = 0;
						num = XData[i].size();
					}
					//int num = XData[i].size() - startnum_;
					//if (num > perScreendot_)
						//num = perScreendot_;
					DrawChannelXYTrend(model->TrendXYConfig.DataChanel[i], startnum, num);
				}
			}
		}
		if (IResourceService::Ins()->IsRenderMode())
			DrawRenderChart();
		//����������
		if (useXAxis)
			DrawXaxis();
		if (useYAxis)
			DrawYaxis();
		//���Ʋο���
		DrawReferenceLine();
	}
}

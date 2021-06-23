/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GLineView.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制直线
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GLineView.h"
#include "UIComm.h"
#include "cmath"
#include "FL/fl_draw.H"
namespace UI
{
	GLineView::GLineView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GLineView::~GLineView() {}


	void GLineView::DrawStartArrow(double angle, int& startx, int& starty)
	{
		shared_ptr<GLineModel> model = BaseView.GetModel<GLineModel>();
		if (model->LineConfig.Coordinate.size() == 0)
			return;
		//设置起始终止箭头大小
		int sArrow_Size = model->LineConfig.Arrow.StartArrowSize;
		while (angle > 360.0)	angle -= 360.0;
		//Fl_Color lineColor = fl_rgb_color(RGBColor(model->LineConfig.Line.Color));
		//GraphicDrawHandle::Ins()->SetBrushStyle(active() ? lineColor : fl_inactive(lineColor),
		//	model->LineConfig.Line.Type, 1, model->LineConfig.Line.Alpha);
		GraphicDrawHandle::Ins()->DrawArrow(model->LineConfig.Arrow.StartArrowType,angle,startx,starty, sArrow_Size);

	}

	void GLineView::DrawEndArrow(double angle, int& endx, int& endy)
	{
		shared_ptr<GLineModel> model = BaseView.GetModel<GLineModel>();
		if (model->LineConfig.Coordinate.size() < 2)
			return;
		//设置起始终止箭头大小
		int sArrow_Size = model->LineConfig.Arrow.EndArrowSize;
		while (angle > 360)	angle -= 360;
		//Fl_Color lineColor = fl_rgb_color(RGBColor(model->LineConfig.Line.Color));
		//GraphicDrawHandle::Ins()->SetBrushStyle(active() ? lineColor : fl_inactive(lineColor),
		//	model->LineConfig.Line.Type, 1, model->LineConfig.Line.Alpha);
		GraphicDrawHandle::Ins()->DrawArrow(model->LineConfig.Arrow.EndArrowType, angle, endx, endy, sArrow_Size);
	}
	void GLineView::CalculateAngle(double &startangle, double &endangle)
	{
		shared_ptr<GLineModel> model = BaseView.GetModel<GLineModel>();
		double lineAngle;
		int startx = model->LineConfig.Coordinate[0].X + model->LineConfig.OffX;        //线段起始坐标
		int starty = model->LineConfig.Coordinate[0].Y + model->LineConfig.OffY;
		int endx = model->LineConfig.Coordinate[1].X + model->LineConfig.OffX;        //线段终点坐标
		int endy = model->LineConfig.Coordinate[1].Y + model->LineConfig.OffY;
		//计算线的角度 
		if (startx > endx)
		{
			if (starty == endy)
			{
				startangle = 270;
				endangle = 90;
			}
			if (starty > endy)
			{
				lineAngle = (double)(starty - endy) / (double)(startx - endx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangle = 270 - lineAngle;
				endangle = 90 - lineAngle;
			}
			if (starty < endy)
			{
				lineAngle = (double)(endy - starty) / (double)(startx - endx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangle = 270 + lineAngle;
				endangle = 90 + lineAngle;
			}
		}
		else if (startx == endx)
		{
			if (starty > endy)
			{
				startangle = 180;
				endangle = 0;
			}
			if (starty < endy)
			{
				startangle = 0;
				endangle = 180;
			}
		}
		else if (startx < endx)
		{
			if (starty == endy)
			{
				startangle = 90;
				endangle = 270;
			}
			if (starty > endy)
			{
				lineAngle = (double)(starty - endy) / (double)(endx - startx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangle = 90 + lineAngle;
				endangle = 270 + lineAngle;
			}
			if (starty < endy)
			{
				lineAngle = (double)(endy - starty) / (double)(endx - startx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangle = 90 - lineAngle;
				endangle = 270 - lineAngle;
			}
		}
	}

	void GLineView::DrawLine()
	{
		shared_ptr<GLineModel> model = BaseView.GetModel<GLineModel>();

		int startx, starty, endx, endy;
		if (model->LineConfig.Coordinate.size() < 2)
			return;
	
		startx = model->LineConfig.Coordinate[0].X + model->LineConfig.OffX;        //线段起始坐标
		starty = model->LineConfig.Coordinate[0].Y + model->LineConfig.OffY;
		endx = model->LineConfig.Coordinate[1].X + model->LineConfig.OffX;        //线段终点坐标
		endy = model->LineConfig.Coordinate[1].Y + model->LineConfig.OffY;

		double pointX = model->LineConfig.RotateCenter.X + (double)model->LineConfig.OffX;
		double pointY = model->LineConfig.RotateCenter.Y + (double)model->LineConfig.OffY;

		Fl_Color lineColor = fl_rgb_color(RGBColor(model->LineConfig.Line.Color));
		GraphicDrawHandle::Ins()->BeginPushMatrix(model->LineConfig.RotateAngle, 1.0, 1.0, pointX, pointY);
		GraphicDrawHandle::Ins()->SetBrushStyle(active() ? lineColor : fl_inactive(lineColor),
			model->LineConfig.Line.Type, model->LineConfig.Line.Weight, model->LineConfig.Line.Alpha);

		double startangle, endangle;
		CalculateAngle(startangle, endangle);
		
		if (model->LineConfig.Arrow.HaveEndArrow)
			DrawEndArrow(endangle, endx, endy);
		if (model->LineConfig.Arrow.HaveStartArrow)
			DrawStartArrow(startangle, startx, starty);

		/*绘制线段*/
		/*测试使用*/
		pointX = (startx + endx) / 2.0;
		pointY = (starty + endy) / 2.0;
		GraphicDrawHandle::Ins()->DrawLine(startx, starty, endx, endy);
		GraphicDrawHandle::Ins()->EndPopMatrix();
		//fl_pop_matrix();
	}

	void GLineView::draw()
	{
		GLineView::DrawLine();
	}
}

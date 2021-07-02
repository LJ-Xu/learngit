/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GPolyLineView.cpp
 * Author   : qiaodan
 * Date     : 2021-01-19
 * Descript : 绘制折线多边形
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GPolyLineView.h"
#include "UIComm.h"
#include "GPolyLineModel.h"
#include "FL/fl_draw.H"
#include "IResourceService.h"
#include "RotatePic.h"
#include "Logger.h"
namespace UI
{
	GPolyLineView::GPolyLineView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GPolyLineView::~GPolyLineView() {}

	double GPolyLineView::GetArrowAngle(int startx, int endx, int starty, int endy, int mode)
	{
		double lineAngle, startangele, endangle;

		if (startx > endx)
		{
			if (starty == endy)
			{
				startangele = 270;
				endangle = 90;
			}
			if (starty > endy)
			{
				lineAngle = (double)(starty - endy) / (double)(startx - endx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangele = 270 - lineAngle;
				endangle = 90 - lineAngle;
			}
			if (starty < endy)
			{
				lineAngle = (double)(endy - starty) / (double)(startx - endx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangele = 270 + lineAngle;
				endangle = 90 + lineAngle;
			}
		}
		else if (startx == endx)
		{
			if (starty > endy)
			{
				startangele = 180;
				endangle = 0;
			}
			if (starty < endy)
			{
				startangele = 0;
				endangle = 180;
			}
		}
		else if (startx < endx)
		{
			if (starty == endy)
			{
				startangele = 90;
				endangle = 270;
			}
			if (starty > endy)
			{
				lineAngle = (double)(starty - endy) / (double)(endx - startx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangele = 90 + lineAngle;
				endangle = 270 + lineAngle;
			}
			if (starty < endy)
			{
				lineAngle = (double)(endy - starty) / (double)(endx - startx);
				lineAngle = atan(lineAngle) * 180 / 3.14;
				startangele = 90 - lineAngle;
				endangle = 270 - lineAngle;
			}
		}
		if (mode)
			return endangle;
		else
			return startangele;
	}
	void GPolyLineView::DrawFill()
	{
		std::shared_ptr<GPolyLineModel> model = BaseView.GetModel<GPolyLineModel>();

		bool needDelete = false;
		Fl_Image *picRes = IResourceService::Ins()->GetImage(model->PolyLineConfig.BgPicKey.KeyVal);
		if (!picRes)
		{
			LOG_INFO_("PolyLine FillPic is NULL");
			return;
		}
		//设置图片为透明色
		if (picRes->d() != 4)
		{
			uchar *rgb = new uchar[picRes->w() * picRes->h() * picRes->d()];
			uchar *rgba = new uchar[picRes->w() * picRes->h() * 4];
			memcpy(rgb, picRes->data()[0], picRes->w() * picRes->h() * picRes->d());
			int j, i;
			for (i = 0, j = 0; i < picRes->w() * picRes->h() * picRes->d(); i += 3, j += 4)
			{
				rgba[j] = rgb[i];
				rgba[j + 1] = rgb[i + 1];
				rgba[j + 2] = rgb[i + 2];
				rgba[j + 3] = 255;
			}
			delete[] rgb;
			picRes = new Fl_RGB_Image(rgba, picRes->w(), picRes->h(), 4);
			needDelete = true;
		}
		if (model->PolyLineConfig.RotateAngle != 0)
		{
			Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->PolyLineConfig.RotateAngle);
			resize(model->PolyLineConfig.X + model->PolyLineConfig.OffX,
				model->PolyLineConfig.Y + model->PolyLineConfig.OffY,
				rotatePicRes->w(), rotatePicRes->h());
			rotatePicRes->draw((int)(model->PolyLineConfig.X + model->PolyLineConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5),
				(int)(model->PolyLineConfig.Y + model->PolyLineConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5));
			UI::RotatePic::DeleteRotatePic(rotatePicRes);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
		else
		{
			resize(model->PolyLineConfig.X + model->PolyLineConfig.OffX,
				model->PolyLineConfig.Y + model->PolyLineConfig.OffY,
				picRes->w(), picRes->h());
			picRes->draw(model->PolyLineConfig.X + model->PolyLineConfig.OffX,
				model->PolyLineConfig.Y + model->PolyLineConfig.OffY);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
	}

	void GPolyLineView::draw()
	{
		shared_ptr<GPolyLineModel> model = BaseView.GetModel<GPolyLineModel>();
		if (model->PolyLineConfig.IsFill)            //是否填充  
			DrawFill();
		if (model->PolyLineConfig.Points.size() < 2)
			return;
		if (model->PolyLineConfig.Mode == 1 && !model->PolyLineConfig.UseLine)
			return;
		double pointX = model->PolyLineConfig.RotateCenter.X + model->PolyLineConfig.OffX;
		double pointY = model->PolyLineConfig.RotateCenter.Y + model->PolyLineConfig.OffY;
		/*测试使用*/
		pointX = model->PolyLineConfig.X + model->PolyLineConfig.Width / 2.0;
		pointY = model->PolyLineConfig.Y + model->PolyLineConfig.Height / 2.0;
		Fl_Color linecolor = fl_rgb_color(RGBColor(model->PolyLineConfig.LineStyle.Color));
		linecolor = active() ? linecolor : fl_inactive(linecolor);

		GraphicDrawHandle::Ins()->BeginPushMatrix(model->PolyLineConfig.RotateAngle, 1.0, 1.0, pointX, pointY);
		GraphicDrawHandle::Ins()->SetBrushStyle(active() ? linecolor : fl_inactive(linecolor),
			model->PolyLineConfig.LineStyle.Type, model->PolyLineConfig.LineStyle.Weight,
			model->PolyLineConfig.LineStyle.Alpha);
		/*获取点*/
		vector<Project::Point> points;
		//points.swap(model->PolyLineConfig.Points);
		for (size_t i = 0; i < model->PolyLineConfig.Points.size(); i++)
		{
			Project::Point tmppoint;
			tmppoint.X = model->PolyLineConfig.Points[i].X + model->PolyLineConfig.OffX;
			tmppoint.Y = model->PolyLineConfig.Points[i].Y + model->PolyLineConfig.OffY;
			points.push_back(tmppoint);
		}
		if (model->PolyLineConfig.Mode == 0)		//折线
		{
			double angle = 0;
			if (model->PolyLineConfig.Arrow.HaveStartArrow)
			{
				angle = GetArrowAngle(points[0].X, points[1].X,	points[0].Y, points[1].Y, 0);
				//Fl_Color lineColor = fl_rgb_color(RGBColor(model->PolyLineConfig.LineStyle.Color));
				//GraphicDrawHandle::Ins()->SetBrushStyle(active() ? lineColor : fl_inactive(lineColor),
				//	model->PolyLineConfig.LineStyle.Type, 1, model->PolyLineConfig.LineStyle.Alpha);
				GraphicDrawHandle::Ins()->DrawArrow(model->PolyLineConfig.Arrow.StartArrowType, angle,
					points[0].X, points[0].Y, model->PolyLineConfig.Arrow.StartArrowSize);
			}
			if (model->PolyLineConfig.Arrow.HaveEndArrow)
			{
				angle = GetArrowAngle(points[points.size() - 2].X,
					points[points.size() - 1].X, points[points.size() - 2].Y,
					points[points.size() - 1].Y, 1);
				//Fl_Color lineColor = fl_rgb_color(RGBColor(model->PolyLineConfig.LineStyle.Color));
				//GraphicDrawHandle::Ins()->SetBrushStyle(active() ? lineColor : fl_inactive(lineColor),
				//	model->PolyLineConfig.LineStyle.Type, 1, model->PolyLineConfig.LineStyle.Alpha);

				GraphicDrawHandle::Ins()->DrawArrow(model->PolyLineConfig.Arrow.EndArrowType, angle,
					points[points.size() - 1].X, points[points.size() - 1].Y,
					model->PolyLineConfig.Arrow.EndArrowSize);
				//drawArrow(angle, model->PolyLineConfig.Points[num - 1].X + model->PolyLineConfig.OffX,
				//	model->PolyLineConfig.Points[num - 1].Y + model->PolyLineConfig.OffY, 1);
			}
		}
		GraphicDrawHandle::Ins()->DrawPolyLine(model->PolyLineConfig.Mode, points);

		GraphicDrawHandle::Ins()->EndPopMatrix();
	
	}
}

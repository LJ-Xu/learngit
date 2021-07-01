/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GArcView.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制圆弧
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GArcView.h"
#include "FL/fl_draw.H"
#include "UIComm.h"
#include "GArcControl.h"
#include "GArcModel.h"
#include "IResourceService.h"
#include "RotatePic.h"
#include "Logger.h"
namespace UI
{
	GArcView::GArcView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GArcView::~GArcView() {}

	void GArcView::DrawFill()
	{
		std::shared_ptr<GArcModel> model = BaseView.GetModel<GArcModel>();

		bool needDelete = false;
		Fl_Image *picRes = IResourceService::Ins()->GetImage(model->ArcConfig.BgPicKey.KeyVal);
		if (!picRes)
		{
			LOG_INFO_("Arc FillPic is NULL");
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
		if (model->ArcConfig.RotateAngle != 0)
		{
			Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->ArcConfig.RotateAngle);
			resize(model->ArcConfig.X + model->ArcConfig.OffX,
				model->ArcConfig.Y + model->ArcConfig.OffY,
				rotatePicRes->w(), rotatePicRes->h());
			rotatePicRes->draw((int)(model->ArcConfig.X + model->ArcConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5),
				(int)(model->ArcConfig.Y + model->ArcConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5));
			UI::RotatePic::DeleteRotatePic(rotatePicRes);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
		else
		{
			resize(model->ArcConfig.X + model->ArcConfig.OffX,
				model->ArcConfig.Y + model->ArcConfig.OffY,
				picRes->w(), picRes->h());
			picRes->draw(model->ArcConfig.X + model->ArcConfig.OffX,
				model->ArcConfig.Y + model->ArcConfig.OffY);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
	}
	void GArcView::draw()
	{
		std::shared_ptr<GArcModel> model = BaseView.GetModel<GArcModel>();
		/*绘制填充*/
		if (model->ArcConfig.IsFill)            //是否填充  
			DrawFill();
		/*开始绘制圆弧*/
		//DrawArc();
		int startx, starty, endx, endy;

		if (model->ArcConfig.Coordinate.size() == 0)
			return;
		startx = model->ArcConfig.Coordinate[0].X + model->ArcConfig.OffX;        //线段起始坐标
		starty = model->ArcConfig.Coordinate[0].Y + model->ArcConfig.OffY;
		endx = model->ArcConfig.Coordinate[1].X + model->ArcConfig.OffX;        //线段终点坐标
		endy = model->ArcConfig.Coordinate[1].Y + model->ArcConfig.OffY;

		Project::Point point;
		point.X = model->ArcConfig.ArcRes.Center.X + model->ArcConfig.OffX;
		point.Y = model->ArcConfig.ArcRes.Center.Y + model->ArcConfig.OffY;			//圆心坐标
		LOG_INFO("Draw Arc X = %d, Y = %d, W = %d, H = %d\n", point.X, point.Y, 
			model->ArcConfig.ArcRes.A, model->ArcConfig.ArcRes.B);
		
		Fl_Color linecolor = fl_rgb_color(RGBColor(model->ArcConfig.Line.Color));
		linecolor = active() ? linecolor : fl_inactive(linecolor);
		GraphicDrawHandle::Ins()->BeginPushMatrix(model->ArcConfig.RotateAngle, 1.0, 1.0, point.X, point.Y);
		GraphicDrawHandle::Ins()->SetBrushStyle(active() ? linecolor : fl_inactive(linecolor),
			model->ArcConfig.Line.Type, model->ArcConfig.Line.Weight, model->ArcConfig.Line.Alpha);
		GraphicDrawHandle::Ins()->DrawArc(model->ArcConfig.IsSector, point, model->ArcConfig.ArcRes.A, model->ArcConfig.ArcRes.B,
			model->ArcConfig.StartAngle, model->ArcConfig.EndAngle, startx, starty, endx, endy);
		/*旋转功能时使用*/
		//GraphicDrawHandle::Ins()->DrawArc(model->ArcConfig.IsSector, point,model->ArcConfig.ArcRes.A,model->ArcConfig.ArcRes.B,
		//	model->ArcConfig.StartAngle,model->ArcConfig.EndAngle);
		GraphicDrawHandle::Ins()->EndPopMatrix();
	}
}

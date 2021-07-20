/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GCircleView.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制圆
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GCircleView.h"
#include "UIComm.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include "IResourceService.h"
#include "RotatePic.h"
#include "Logger.h"
namespace UI
{
	GCircleView::GCircleView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GCircleView::~GCircleView() {}
	
	void GCircleView::DrawFill()
	{
		std::shared_ptr<GCircleModel> model = BaseView.GetModel<GCircleModel>();

		bool needDelete = false;
		Fl_Image *picRes = IResourceService::Ins()->GetImage(model->CircleConfig.BgPicKey.KeyVal);
		if (!picRes)
		{
			LOG_INFO_("Circle FillPic is NULL");
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
		if (model->CircleConfig.RotateAngle != 0)
		{
			Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->CircleConfig.RotateAngle);
			resize(model->CircleConfig.X + model->CircleConfig.OffX,
				model->CircleConfig.Y + model->CircleConfig.OffY,
				rotatePicRes->w(), rotatePicRes->h());
			rotatePicRes->draw((int)(model->CircleConfig.X + model->CircleConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5),
				(int)(model->CircleConfig.Y + model->CircleConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5));
			UI::RotatePic::DeleteRotatePic(rotatePicRes);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
		else
		{
			resize(model->CircleConfig.X + model->CircleConfig.OffX,
				model->CircleConfig.Y + model->CircleConfig.OffY,
				picRes->w(), picRes->h());
			picRes->draw(model->CircleConfig.X + model->CircleConfig.OffX,
				model->CircleConfig.Y + model->CircleConfig.OffY);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
	}
	void GCircleView::draw()
	{
		shared_ptr<GCircleModel> model = BaseView.GetModel<GCircleModel>();
		//画圆 圆边样式 粗细 颜色
		if (model->CircleConfig.IsFill)            //圆形是否填充  
			DrawFill();
		//DrawCircle();
		Project::Point point;
		point.X = model->CircleConfig.ArcRes.Center.X + model->CircleConfig.OffX;
		point.Y = model->CircleConfig.ArcRes.Center.Y + model->CircleConfig.OffY;			//圆心坐标
		Fl_Color linecolor = fl_rgb_color(RGBColor(model->CircleConfig.Line.Color));
		linecolor = active() ? linecolor : fl_inactive(linecolor);

		/**********************************测试使用********************************/
		point.X = model->CircleConfig.X + model->CircleConfig.Width / 2 + model->CircleConfig.OffX;
		point.Y = model->CircleConfig.Y + model->CircleConfig.Height / 2 + model->CircleConfig.OffY;			//圆心坐标
		model->CircleConfig.ArcRes.A = model->CircleConfig.Width / 2;
		model->CircleConfig.ArcRes.B = model->CircleConfig.Height / 2;
		/**************************************************************************/
		LOG_INFO("Draw Circle Pointx = %d, Pointy = %d\n", point.X, point.Y);
		GraphicDrawHandle::Ins()->BeginPushMatrix(model->CircleConfig.RotateAngle, 1.0, 1.0, point.X, point.Y);
		GraphicDrawHandle::Ins()->SetBrushStyle(active() ? linecolor : fl_inactive(linecolor), 
			model->CircleConfig.Line.Type, model->CircleConfig.Line.Weight, model->CircleConfig.Line.Alpha);
		GraphicDrawHandle::Ins()->DrawCircle(point, model->CircleConfig.ArcRes.A, model->CircleConfig.ArcRes.B);
		GraphicDrawHandle::Ins()->EndPopMatrix();
	}
	int GCircleView::handle(int event)
	{
		return 0;
	}
}

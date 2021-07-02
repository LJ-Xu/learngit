/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GRectangleView.cpp
 * Author   : wangjinchuan\qiaodan
 * Date     : 2020-09-18
 * Descript : »æÖÆ¾ØÐÎ
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GRectangleView.h"
#include "UIComm.h"
#include "GRectangleModel.h"
#include "FL/fl_draw.H"
#include <math.h>
#include "IResourceService.h"
#include "RotatePic.h"
#include "Logger.h"
namespace UI
{
	GRectangleView::GRectangleView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	GRectangleView::~GRectangleView() {}

	void GRectangleView::DrawFill()
	{
		shared_ptr<GRectangleModel> model = BaseView.GetModel<GRectangleModel>();
		bool needDelete = false;
		Fl_Image *picRes = IResourceService::Ins()->GetImage(model->RectangleConfig.BgPicKey.KeyVal);
		if (!picRes)
		{
			LOG_INFO_("Rectangle FillPic is NULL");
			return;
		}
		//ÉèÖÃÍ¼Æ¬ÎªÍ¸Ã÷É«
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
		if (model->RectangleConfig.RotateAngle != 0)
		{
			Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->RectangleConfig.RotateAngle);
			resize(model->RectangleConfig.X + model->RectangleConfig.OffX,
				model->RectangleConfig.Y + model->RectangleConfig.OffY,
				rotatePicRes->w(), rotatePicRes->h());
			rotatePicRes->draw((int)(model->RectangleConfig.X + model->RectangleConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5),
				(int)(model->RectangleConfig.Y + model->RectangleConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5));
			UI::RotatePic::DeleteRotatePic(rotatePicRes);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
		else
		{
			resize(model->RectangleConfig.X + model->RectangleConfig.OffX,
				model->RectangleConfig.Y + model->RectangleConfig.OffY,
				picRes->w(), picRes->h());
			image(picRes);
			//box(FL_NO_BOX);
			//align(FL_ALIGN_IMAGE_BACKDROP);		//ÉèÖÃ±³¾°Í¼Æ¬
			//draw_box();
			picRes->draw(model->RectangleConfig.X + model->RectangleConfig.OffX,
				model->RectangleConfig.Y + model->RectangleConfig.OffY);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
	}

	void GRectangleView::draw()
	{
		shared_ptr<GRectangleModel> model = BaseView.GetModel<GRectangleModel>();
		if (model->RectangleConfig.Radius > model->RectangleConfig.Height)
			drawRectRadius = model->RectangleConfig.Height;
		else
			drawRectRadius = model->RectangleConfig.Radius;

		double pointX = model->RectangleConfig.RotateCenter.X + (double)model->RectangleConfig.OffX;
		double pointY = model->RectangleConfig.RotateCenter.Y + (double)model->RectangleConfig.OffY;
		/*²âÊÔÊ¹ÓÃ*/
		//pointX = model->RectangleConfig.X + model->RectangleConfig.Width / 2;
		//pointY = model->RectangleConfig.Y + model->RectangleConfig.Height / 2;
		model->RectangleConfig.RectHeight = model->RectangleConfig.Height;
		model->RectangleConfig.RectWidth = model->RectangleConfig.Width;
		if (model->RectangleConfig.IsFill)  //ÊÇ·ñÌî³ä
			DrawFill();
		Fl_Color linecolor = fl_rgb_color(RGBColor(model->RectangleConfig.Line.Color));
		linecolor = active() ? linecolor : fl_inactive(linecolor);

		GraphicDrawHandle::Ins()->BeginPushMatrix(model->RectangleConfig.RotateAngle, 1.0, 1.0, pointX, pointY);
		GraphicDrawHandle::Ins()->SetBrushStyle(active() ? linecolor : fl_inactive(linecolor),
							model->RectangleConfig.Line.Type, model->RectangleConfig.Line.Weight,
							model->RectangleConfig.Line.Alpha);
		GraphicDrawHandle::Ins()->DrawRectangle(drawRectRadius, pointX - model->RectangleConfig.RectWidth / 2,
			pointY - model->RectangleConfig.RectHeight / 2, model->RectangleConfig.RectWidth,model->RectangleConfig.RectHeight,
			model->RectangleConfig.Line.Weight);
		GraphicDrawHandle::Ins()->EndPopMatrix();

	
	}
}

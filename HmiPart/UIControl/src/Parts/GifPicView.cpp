/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : GifPicView.cpp
 * Author   : qiaodan
 * Date     : 2020-12-22
 * Descript : 绘制Gif图片控件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "stdafx.h"
#include "GifPicView.h"
#include "UIComm.h"
#include "FL/fl_draw.H"
#include "GifPicModel.h"
#include <FL/Fl_Shared_Image.H>
#include "RotatePic.h"
extern "C" {
#include "gifdec.h"
}
namespace UI
{
	GifPicView::GifPicView(int X, int Y, int W, int H, const char* l) : HMIBaseView(X, Y, W, H, l){}
	GifPicView::~GifPicView() {}

	void GifPicView::InitDraw()
	{
		shared_ptr<GifPicModel> model = BaseView.GetModel<GifPicModel>();
		/************通过下位机解析方式绘制Gif图片***************/
		//gd_GIF* gif;
		//gif = gd_open_gif("C:/Users/PC/Desktop/1.gif");
		//std::vector<Fl_RGB_Image *>().swap(PicRes);
		//while (gd_get_frame(gif))
		//{
		//	uchar* color, *frame;
		//	frame = (uchar*)malloc(gif->width * gif->height * 3);
		//	gd_render_frame(gif, frame);
		//	Fl_RGB_Image* RGBAimage = new Fl_RGB_Image(frame, gif->width, gif->height, 3);
		//	PicRes.push_back(RGBAimage);
		//	unsigned delay = gif->gce.delay * 10;
		//	DelayTime.push_back(delay);
		//}
		//PicNum = PicRes.size();
		//CurrentPicNo = 0;
		/************通过上位机解析方式绘制Gif图片***************/
		PicNum = model->GifConfig.PicKeys.size();
		CurrentPicNo = 0;
	}

	void GifPicView::draw()
	{
		shared_ptr<GifPicModel> model = BaseView.GetModel<GifPicModel>();
		if (CurrentFlash == 0)
			return;
		/************通过下位机解析方式绘制Gif图片***************/
		//if (CurrentPicNo > PicRes.size() || CurrentPicNo > PicNum)
		//	return;
		//fl_register_images();
		//Fl_RGB_Image *img = PicRes[CurrentPicNo];
		//img->draw(model->GifConfig.X + model->GifConfig.OffX, 
		//	      model->GifConfig.Y + model->GifConfig.OffY);
		////image(img);
		////align(FL_ALIGN_IMAGE_BACKDROP);
		////box(FL_NO_BOX);
		////draw_box();
		/************通过上位机解析方式绘制Gif图片***************/
		if (CurrentPicNo >= PicNum)
			return;
		bool needDelete = false;
		Fl_Image *picRes = IResourceService::Ins()->GetImage(model->GifConfig.PicKeys[CurrentPicNo].KeyVal);
		//设置图片为透明色
#if 0
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
				if (model->GifConfig.PicFormat.UseFaheout)
					rgba[j + 3] = model->GifConfig.PicFormat.FadeoutPecent * 255 / 100;
				else
					rgba[j + 3] = 255;
			}
			delete[] rgb;
			picRes = new Fl_RGB_Image(rgba, picRes->w(), picRes->h(), 4);
			needDelete = true;
		}
		//else
		//{
		//	uchar *rgba = new uchar[picRes->w() * picRes->h() * picRes->d()];
		//	memcpy(rgba, picRes->data()[0], picRes->w() * picRes->h() * picRes->d());
		//	for (int i = 0; i < picRes->w() * picRes->h() * picRes->d(); i += 4)
		//	{
		//		if (model->GifConfig.PicFormat.UseFaheout)
		//			rgba[i + 3] = model->GifConfig.PicFormat.FadeoutPecent * 255 / 100;
		//		else
		//			rgba[i + 3] = 255;
		//	}
		//	picRes = new Fl_RGB_Image(rgba, picRes->w(), picRes->h(), 4);
		//	needDelete = true;
		//}
		if (model->GifConfig.PicFormat.UseRotate)
		{
			Fl_Image *rotatePicRes = UI::RotatePic::NewRotatePic(picRes, (double)model->GifConfig.PicFormat.RotateAngle);
			resize(model->GifConfig.X + model->GifConfig.OffX,
				model->GifConfig.Y + model->GifConfig.OffY,
				rotatePicRes->w(), rotatePicRes->h());
			rotatePicRes->draw((int)(model->GifConfig.X + model->GifConfig.OffX - (rotatePicRes->w() - picRes->w()) * 0.5),
				(int)(model->GifConfig.Y + model->GifConfig.OffY - (rotatePicRes->h() - picRes->h()) * 0.5));

			//draw_box();
			UI::RotatePic::DeleteRotatePic(rotatePicRes);
			if (needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
		else
		{
			resize(model->GifConfig.X + model->GifConfig.OffX,
				model->GifConfig.Y + model->GifConfig.OffY,
				picRes->w(), picRes->h());
			picRes->draw(model->GifConfig.X + model->GifConfig.OffX,
				model->GifConfig.Y + model->GifConfig.OffY);
			if(needDelete)
				UI::RotatePic::DeleteRotatePic(picRes);
		}
#endif
		if (picRes)
			picRes->draw(model->GifConfig.X + model->GifConfig.OffX,
					      model->GifConfig.Y + model->GifConfig.OffY);
		else
			LOG_INFO("Gif Pic%d is NULL\n", CurrentPicNo);
	}
}

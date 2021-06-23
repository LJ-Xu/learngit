/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : RotatePic.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 旋转图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "RotatePic.h"
#include <string.h>
#include <math.h>
namespace UI
{
	void RotatePic::DeleteRotatePic(Fl_Image *image)
	{
		Fl_RGB_Image *rgbimage = (Fl_RGB_Image *)image;
		const uchar *data = rgbimage->array;
		delete rgbimage;
		delete[] data;
	}
	Fl_Image * RotatePic::NewRotatePic(Fl_Image *image, double angle, double xscale, double yscale)
	{
		//获取源图片数据
		uchar* srcrgbadata = new uchar[image->w() * image->h() * 4];
		memcpy(srcrgbadata, image->data()[0], image->w() * image->h() * image->d());
		//设置目标图片为透明图片
		long dst_wh = (long)(::sqrt(1.0 * image->w() * image->w() + image->h() * image->h()) + 4 + 0.5);

		Fl_Image* rotatepic = new Fl_Image(dst_wh, dst_wh, 4);
		uchar* rotatergba = new uchar[rotatepic->w() * rotatepic->h() * rotatepic->d()];

		for (int j = 0; j < rotatepic->w() * rotatepic->h() * rotatepic->d(); j += 4)
			rotatergba[j + 3] = 0;

		double rotaryAngle = (3.1415926 * 2) * (0 * 1.0 / 1.0 + angle / 360.0);
		double rx0 = image->w() * 0.5;  //(rx0,ry0)为旋转中心 
		double ry0 = image->h() * 0.5;

		double move_x = (rotatepic->w() - image->w()) * 0.5;
		double move_y = (rotatepic->h() - image->h()) * 0.5;
		for (long y = 0;y < rotatepic->h();++y)
		{
			for (long x = 0;x < rotatepic->w();++x)
			{
				long srcx = (long)((x - (move_x + rx0)) / 1 * cos(rotaryAngle) - (y - (move_y + ry0)) / 1 * sin(rotaryAngle) + rx0);
				long srcy = (long)((x - (move_x + rx0)) / 1 * sin(rotaryAngle) + (y - (move_y + ry0)) / 1 * cos(rotaryAngle) + ry0);
				srcx = ((double)(srcx ) / xscale);
				srcy = ((double)(srcy ) / yscale);
				if ((unsigned long)srcx < (unsigned long)image->w() && (unsigned long)srcy < (unsigned long)image->h())
				{
					rotatergba[(rotatepic->w() * y + x) * 4 + 0] = srcrgbadata[(image->w() * srcy + srcx) * 4 + 0];
					rotatergba[(rotatepic->w() * y + x) * 4 + 1] = srcrgbadata[(image->w() * srcy + srcx) * 4 + 1];
					rotatergba[(rotatepic->w() * y + x) * 4 + 2] = srcrgbadata[(image->w() * srcy + srcx) * 4 + 2];
					rotatergba[(rotatepic->w() * y + x) * 4 + 3] = srcrgbadata[(image->w() * srcy + srcx) * 4 + 3];
				}

			}
		}

		Fl_Image *newImage = new Fl_RGB_Image(rotatergba, rotatepic->w(), rotatepic->h(), 4);
		delete rotatepic;
		delete[] srcrgbadata;
		return newImage;
	}
}

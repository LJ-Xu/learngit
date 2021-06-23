#pragma once
#include <string>
#include <FL/Fl_Image.H>
namespace UI
{
	class RotatePic
	{
	public:
		static Fl_Image *NewRotatePic(Fl_Image *image, double angle, double xscale = 1, double yscale = 1);				//生成图片旋转及透明度处理
		static void DeleteRotatePic(Fl_Image *image);								//删除图片
	};
}


#pragma once
#include "FL/Fl_Image.H"
namespace UI
{
	/**
 The Fl_BMP_Image class supports loading, caching,
 and drawing of Windows Bitmap (BMP) image files.
 */
	class FL_EXPORT BinBitmap : public Fl_RGB_Image {

	public:

		BinBitmap(const char *data, int size);
	};

	/*class FL_EXPORT Fl_BMP_Image1 : public Fl_RGB_Image {

	public:

		Fl_BMP_Image1(const char *path);
	};*/
}


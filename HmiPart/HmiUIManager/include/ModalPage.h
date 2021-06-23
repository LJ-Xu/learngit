#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <memory>
#include <vector>
namespace UI
{
	class ModalPage : public Fl_Widget {
	public:
		ModalPage(int x, int y, int w, int h,int alpha=0);
		void draw();
		int handle(int);
		void set_alpha(int alpha)
		{
			alpha_ = alpha;
		}
	private:
		int alpha_ = 0;
	};
}

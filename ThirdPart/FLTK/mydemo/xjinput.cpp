#include <stdio.h>
#include "xjinput.h"

XJinput::XJinput(int X, int Y, int W, int H, const char *l)
	: Fl_Input(X, Y, W, H, l) {}
 
void XJinput::draw()
{
	Fl_Display_Device * dev = Fl_Display_Device::display_device();
	Fl_Graphics_Driver* graphdrv = dev->driver();
	fl_color(0x00FF00);
	fl_line_style(FL_DASH);
	fl_rect(0, 0, 100, 100);
	fl_draw("dsadasdasd", 10, 10);
}
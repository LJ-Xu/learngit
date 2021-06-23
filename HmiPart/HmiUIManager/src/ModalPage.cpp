#include "ModalPage.h"
namespace UI
{
	ModalPage::ModalPage(int x, int y, int w, int h, int alpha) : Fl_Widget(x, y, w, h, "ModalPage")
	{
	}
	void ModalPage::draw() {
		fl_rectf(x(), y(), w(), h(), FL_RED);
	}
	int ModalPage::handle(int) {
		return 1;
	}

}
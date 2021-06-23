#include "stdafx.h"
#include "..\..\include\HirePurchaseView.h"
#include "HMIWindow.h"
namespace UI
{
	void HirePurchaseGView::draw()
	{
		shared_ptr<HirePurchaseGModel> model = BaseView.GetModel<HirePurchaseGModel>();
		shared_ptr<HirePurchaseGControl> ctrl = BaseView.GetControl<HirePurchaseGControl>();
		fl_color(FL_BLACK);
		fl_rectf(x(), y(), w(), h());
		fl_font(FL_HELVETICA, 16);
		fl_color(FL_GREEN);
		fl_draw("Press the Password!", w() / 4, h() / 4);
		if (Lock)
		{
			//Ê¹ÓÃµ¯³ö¼üÅÌ
			//ctrl->Win()->OpenDialogPage(25006, nullptr,w() / 4,h() / 2);
			if (Fl::focus() != this) {
				Fl::focus(this);
			}
			mark(0);
			char *data = new char[32];
			memset(data, '\0', 32);
			memcpy(data, value(), strlen(value()));
		}
	}

}

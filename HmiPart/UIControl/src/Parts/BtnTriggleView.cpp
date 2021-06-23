#include "stdafx.h"
#include "BtnTriggleView.h"
#include "UIComm.h"
#include "BtnTriggleControl.h"
#include "BtnTriggleModel.h"
#include "ResourceService.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
namespace UI
{
	BtnTriggleView::BtnTriggleView(int X, int Y, int W, int H, const char *L) : HMIBaseButton(X, Y, W, H, L) {}
	BtnTriggleView::~BtnTriggleView() {}

	void BtnTriggleView::draw()
	{
		shared_ptr<BtnTriggleControl> ctrl = BaseView.GetControl<BtnTriggleControl>();
		shared_ptr<BtnTriggleModel> model = BaseView.GetModel<BtnTriggleModel>();
		fl_register_images();
		std::string s = model->m_config.image.path;
		Fl_Shared_Image *jpg = Fl_Shared_Image::get(s.c_str(), w(), h());
		//char utf8[256];
		//const wchar_t *wstr = L"ËÎÌו";
		//fl_utf8fromwc(utf8, 256, wstr, wcslen(wstr));
		//model->m_config.font.name = utf8;
		int fontstyle = UI::IResourceService::GetFontIdx(model->m_config.font.Name);

		printf("fontstyle = %d\n", fontstyle);
		//fl_font(fontstyle, model->m_config.font.size);
		labelfont(fontstyle);
		labelsize(24);
	//	labelcolor((model->m_config.font.color));
		//fl_color(RGBColor(model->m_config.font.color));
		align(FL_ALIGN_IMAGE_BACKDROP);
		image(jpg);
		HMIBaseButton::draw();

		//color(model->m_config.backColor);

	}
}

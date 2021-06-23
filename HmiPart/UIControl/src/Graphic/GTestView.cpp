#include "stdafx.h"
#include "ResourceService.h"
#include "UIComm.h"
#include "GTestView.h"
#include "GTestControl.h"
#include "GTestModel.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Button.H>

#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
namespace UI
{
	GTestView::GTestView(int X, int Y, int W, int H, const char* L = 0) : HMIBaseButton(X, Y, W, H, L)
	{
	
	}
	GTestView::~GTestView() {}
	void GTestView::draw()
	{
		shared_ptr<GTestControl> ctrl = BaseView.GetControl<GTestControl>();
		shared_ptr<GTestModel> model = BaseView.GetModel<GTestModel>();
		HMIImage* img = IResourceService::Ins()->GetImage(model->m_testConfig.hash);
		if (img)
			image(img);
		HMIBaseButton::draw();
	}
}

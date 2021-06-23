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
		shared_ptr<GTestModel> model = BaseView.GetModel<GTestModel>();
		
		if (!model->Config.BGKey.empty())
		{
			
			Fl_Image* img = IResourceService::Ins()->GetImage(model->Config.BGKey);
			image(img);
			align(FL_ALIGN_IMAGE_BACKDROP);
			fl_color(FL_RED);
		}
		
		HMIBaseButton::draw();
	}
}

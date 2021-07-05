#include "stdafx.h"
#include "UIComm.h"
#include "MotorView.h"
#include "MotorControl.h"
#include "MotorModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>
#include "HMIWindow.h"
#include "PermUtility.h"

namespace UI
{
	MotorView::MotorView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H), CurBlinkState(0)
	{
	}
	MotorView::~MotorView()
	{
	}
	void MotorView::draw()
	{
		shared_ptr<MotorControl> ctrl = BaseView.GetControl<MotorControl>();
		shared_ptr<MotorModel> model = BaseView.GetModel<MotorModel>();
		DrawStickChart(ctrl.get(), model.get());
	}
	void MotorView::DrawStickChart(MotorControl * ctrl, MotorModel * model)
	{
		//ªÒ»°µ±«∞◊¥Ã¨Õº∆¨
		Fl_Image *flImgPtr = nullptr;

		IResourceService::Ins()->SetRenderStatus(model->MotorConfig.CurrentStatus_, 2);
		//…¡À∏∆•≈‰
		switch (model->MotorConfig.CurrentStatus_)
		{
		case 1:
		{
			flImgPtr = IResourceService::Ins()->GetImage(model->MotorConfig.KeyImgOn.KeyVal);
			break;
		}
		case 0:
		{
			flImgPtr = IResourceService::Ins()->GetImage(model->MotorConfig.KeyImgOff.KeyVal);
			break;
		}
		default:
			flImgPtr = IResourceService::Ins()->GetImage(model->MotorConfig.KeyImgOff.KeyVal);
			break;
		}

		//ªÒ»°µ±«∞Õº∆¨
		if (!flImgPtr) {
			printf("Motor Can't get image\n");
		}
		else {
			image(flImgPtr);
			align(FL_ALIGN_IMAGE_BACKDROP);		//…Ë÷√±≥æ∞Õº∆¨
			box(FL_NO_BOX);
		}
		//ªÊ÷∆Õº∆¨øÚÃÂ
		draw_box();
	}
}
#include "stdafx.h"
#include "UIComm.h"
#include "BuzzerView.h"
#include "BuzzerControl.h"
#include "BuzzerModel.h"
#include "ResourceService.h"
#include "StringUtility.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Output.H>


namespace UI
{
	BuzzerView::BuzzerView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H)
	{
	}
	BuzzerView::~BuzzerView()
	{
	}

	void BuzzerView::draw()
	{
		shared_ptr<BuzzerControl> ctrl = BaseView.GetControl<BuzzerControl>();
		shared_ptr<BuzzerModel> model = BaseView.GetModel<BuzzerModel>();
		DrawStickChart(ctrl.get(), model.get());
	}

	void BuzzerView::DrawStickChart(BuzzerControl* ctrl, BuzzerModel* model)
	{
		////暂时只画一个黑框来标识蜂鸣器所在位置
		//fl_color(0);
		//fl_rect(model->BuzzerConfig.X, model->BuzzerConfig.Y, model->BuzzerConfig.Width, model->BuzzerConfig.Height);
	}
}



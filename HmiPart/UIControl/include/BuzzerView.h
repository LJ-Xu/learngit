#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class BuzzerControl;
	class BuzzerModel;
	class BuzzerView : public HMIBaseView
	{
	public:
		BuzzerView(int X, int Y, int W, int H);
		~BuzzerView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(BuzzerControl* ctrl, BuzzerModel* model);
	};
}


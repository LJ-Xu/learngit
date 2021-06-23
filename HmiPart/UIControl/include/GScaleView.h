#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class GScaleControl;
	class GScaleModel;
	class GScaleView : public HMIBaseView
	{
	public:
		GScaleView(int X, int Y, int W, int H);
		~GScaleView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawScale(GScaleControl* ctrl, GScaleModel* model);
		void Line_Scale(GScaleModel* model);
		void Arc_Scale(GScaleModel* model);
	};
}

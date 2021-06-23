#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
namespace UI
{
	class GTestControl;
	class GTestModel;
	class GTestView : public HMIBaseButton
	{
	public:
		GTestView(int X, int Y, int W, int H,const char* L);
		~GTestView();
		BasicView BaseView;
	protected:
		void draw() override;
	};


}

#pragma once
#include <FL/Fl_Button.H>
#include <BasicView.h>
namespace UI
{
	class BtnTriggleView : public HMIBaseButton
	{
	public:
		BtnTriggleView(int X, int Y, int W, int H, const char *L);
		~BtnTriggleView();
		BasicView BaseView;
	protected:
		void draw() override;
	};
}



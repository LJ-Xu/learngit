#pragma once
#include <stdafx.h>
#include <FL/Fl_Button.H>
#include <BasicView.h>
namespace UI
{
	class TestBitVarControl;
	class TestBitVarModel;
	class TestBitVarView : public HMIBaseButton
	{
	public:
		TestBitVarView(int X, int Y, int W, int H, const char* L);
		~TestBitVarView();
		BasicView BaseView;
		int handle(int event);

	protected:
		void draw() override;
	};


}



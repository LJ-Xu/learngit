#pragma once
#include "stdafx.h"
#include <BasicView.h>
#include <FL/Fl_Box.H>
namespace UI
{
	class GLabelView : public HMIBaseLabel
	{
	public:
		GLabelView(int X, int Y, int W, int H, const char *L);
		~GLabelView();
		BasicView BaseView;
	protected:
		void draw() override;
	};
}

#pragma once
#include "stdafx.h"
#include "BasicView.h"
#include "DigitalInputView.h"
namespace UI
{
	class HirePurchaseGControl;
	class HirePurchaseGModel;

	class HirePurchaseGView : public InputView
	{
	public:
		HirePurchaseGView(int X, int Y, int W, int H,const char* l = ""):InputView(X, Y, W, H, l)
		{
			Lock = false;
		}
		~HirePurchaseGView() {}
		BasicView BaseView;
	protected:
		void draw()override;
	public:
		bool Lock;
	};
}
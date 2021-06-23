#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class MotorControl;
	class MotorModel;

	class MotorView : public HMIBaseView
	{
	public:
		MotorView(int X, int Y, int W, int H);
		~MotorView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(MotorControl* ctrl, MotorModel* model);
	public:
		int CurBlinkState;
		bool IsBlinking;
	};
}
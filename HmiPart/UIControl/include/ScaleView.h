#pragma once
#include "stdafx.h"
#include "BasicView.h"
namespace UI
{
	class ScaleControl;
	class ScaleModel;
	class ScaleView : public HMIBaseView
	{
	public:
		ScaleView(int X, int Y, int W, int H);
		~ScaleView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(ScaleControl* ctrl, ScaleModel* model);
	private:
		void setLineStyle(ScaleModel* model);
		void calculatePosOnCircle(double angle, int radius, int& offsetX, int& offsetY);
		void initFormat(ScaleModel* model);
		void drawonstick(ScaleModel* model);
		void drawonarc(ScaleModel* model);
	private:
		int fontWidth;
		int FinX;
		int FinY;
		int FinW;
		int FinH;
		char GradFormat[20];//当前刻度格式 例：06.2f = 123.10
	public:
		int CurMaxValue;
		int CurMinValue;
	};
}



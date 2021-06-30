#pragma once
#include "stdafx.h"
#include "qrencode.h"
#include "BasicView.h"
#include "zint.h"

#define QRBUFFLEN 128
namespace UI
{
	class BarCodeControl;
	class BarCodeModel;
	class BarCodeView : public HMIBaseView
	{
	public:
		BarCodeView(int X, int Y, int W, int H);
		~BarCodeView();
		BasicView BaseView;
	protected:
		void draw() override;
		void DrawStickChart(BarCodeControl* ctrl, BarCodeModel* model);
	private:
		void drawqrcode(BarCodeModel* model,unsigned char* data,int datawidth,int dataheight,int drawwidth,int drawheight);
	private:
		Fl_Image* PtrBarImage;
	public:
		int FinX;
		int FinY;
		int BuffLen;
		char BuffValue[QRBUFFLEN];
	};

}

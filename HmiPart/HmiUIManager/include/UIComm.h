#pragma once
#include "stdafx.h"
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "RowTable.h"

//#include "HMIBaseView.h"
#define RGBColor(color) R(color),G(color),B(color)
#define HMIBaseGroup Fl_Group
#define HMIImage Fl_Image
#define HMIBaseView Fl_Widget
#define HMIBaseWindow Fl_Window
#define HMIDoubleWindow Fl_Double_Window
#define HMIBaseOutput Fl_Output
#define HMIBaseInput Fl_Input
#define HMIBaseLabel Fl_Box
#define HMIBaseTable RowTable
#define HMIBaseButton Fl_Button
#define HMIBaseDWindow Fl_Double_Window
#define HMIBaseSlider Fl_Valuator
namespace UI { 
	struct Rectangle
	{
		int X;
		int Y;
		int W;
		int H;
	};
}
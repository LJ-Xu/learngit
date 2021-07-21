#pragma once
#ifdef WIN32
#include <string>
#include "Point.h"
#include <vector>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#else
#include <string>
#include "GLineConfig.h"
#include "Point.h"
#include <vector>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#endif // WIN32


#ifdef WIN32
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <GdiplusColor.h>
//#include <GdiplusPen.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#else
//#include "Fl_XColor.H"
#include <FL/x.H>
#endif // Win32
#ifdef WIN32
#include "GLineConfig.h"
#endif // WIN32

namespace UI
{
	class GraphicDrawHandle
	{
	public:
		static GraphicDrawHandle* Ins();
#ifdef WIN32
		Graphics *graphics;
		//Matrix matrix;
		Pen *pen;
		SolidBrush  *brush;
		ULONG_PTR           gdiplusToken;
#endif
		const double PI = 3.1415926535897932384626433832795;

		/*******************************************************************************
		 * Name     : PushMatrix
		 * Descript : 设置图形旋转缩放矩阵
		 * Input    : angle：旋转角度
		 *			  xscale,yscale：x方向和y方向的缩放比例因子
		 *			  centerx,centery：旋转缩放中心点
		 * Note	    : 与PopMatrix同时使用
		 *******************************************************************************/
		void BeginPushMatrix(double angle, double xscale, double yscale, 
							   double centerx, double centery);
		void EndPopMatrix(void);
		void ArctoBezier(double cx, double cy, double rx, double ry, 
								double start_angle, double sweep_angle, double* curve);
		void SetBrushStyle(Fl_Color color, int style = 0, int width = 0, int alpha = 255);
		void DrawLine(double startx, double starty, double endx, double endy);
		void DrawPolyLine(bool close, std::vector<Project::Point>& points, int offx = 0, int offy = 0);
		void DrawRectangle(double radius, double x, double y, double w, double h, double linew);
		void DrawArc(bool isSector, Project::Point center, double xlength, double ylength, 
							double startangle, double endangle);
		void DrawArc(bool isSector, Project::Point center, double xlength, double ylength,
			double startangle, double endangle,double startx,double starty, double endx, double endy);
		void DrawCircle(Project::Point center, double xlength, double ylength);
		void DrawArrow(Project::ArrowType style, double angle, int x, int y, int width);
		void DrawRenderScroll(int mode, int X, int Y, int W, int H);
		static void PushClip(double x, double y, double w, double h);
	};
}


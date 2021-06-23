#ifdef WIN32
static unsigned fl_colormap[256] = {
#include "fl_cmap.h" // this is a file produced by "cmap.cxx":
};
#endif
#include "GraphicDrawHandle.h"
#include "UIComm.h"
#include <vector>
#include "IResourceService.h"
namespace UI
{
	GraphicDrawHandle* GraphicDrawHandle::Ins()
	{
		static GraphicDrawHandle ins;
		return &ins;
	}
	void GraphicDrawHandle::BeginPushMatrix(double angle, double xscale, double yscale, double centerx, double centery)
	{
#ifdef WIN32
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		graphics = Graphics::FromHDC(fl_gc);
		//matrix.Translate(centerx, centery);
		//matrix.Scale(xscale, yscale);//代码2
		//matrix.Rotate(-angle);
		//matrix.Translate(-centerx, -centery);
		//graphics->SetTransform(&matrix);

#endif
		fl_push_matrix();
		fl_translate(centerx, centery);
		fl_rotate(angle);
		fl_scale(xscale, yscale);
		fl_translate(-centerx, -centery);
	}
	void GraphicDrawHandle::EndPopMatrix(void)
	{
#ifdef WIN32
	//	graphics->ResetTransform();
		GdiplusShutdown(gdiplusToken);
#endif
		fl_pop_matrix();
	}
	void GraphicDrawHandle::SetBrushStyle(Fl_Color color, int style, int width, int alpha)
	{
#ifdef WIN32
		unsigned rgb = 0;
		if (color & 0xffffff00)
			rgb = (unsigned)color;
		else
			rgb = fl_colormap[color];
		brush = new SolidBrush(Color(alpha, (uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8)));
		pen = new Pen(Color(alpha, (uchar)(rgb >> 24), (uchar)(rgb >> 16), (uchar)(rgb >> 8)), width);
		pen->SetDashStyle((DashStyle)style);
		pen->SetLineJoin(LineJoinRound);
		pen->SetLineCap(LineCapSquare, LineCapSquare, DashCapFlat);
		//fl_color(color);
		//fl_line_style(FL_CAP_SQUARE | FL_JOIN_ROUND | style, width);
		fl_color(color);
		fl_line_style(FL_CAP_SQUARE | FL_JOIN_ROUND | style, width);
#else
		//XSetForeground(fl_display, fl_gc, fl_xpixel(i));  //可在X11上单独设置颜色
		fl_color(color);
		fl_line_style(FL_CAP_SQUARE | FL_JOIN_ROUND | style, width);
#endif // WIN32
	}
#if 0
	void generateBezierPoints(double rx, double ry, double x1, double y1, 
		double x2, double y2, double theta, double delta)
	{

		double rX = abs(rx);
		double rY = abs(ry);

		double dx2 = (x1 - x2) / 2;
		double dy2 = (y1 - y2) / 2;

		double x1p = dx2;
		double y1p = dy2;

		double rxs = rX * rX;
		double rys = rY * rY;
		double x1ps = x1p * x1p;
		double y1ps = y1p * y1p;

		double cr = x1ps / rxs + y1ps / rys;
		if (cr > 1) {
			double s = sqrt(cr);
			rX = s * rX;
			rY = s * rY;
			rxs = rX * rX;
			rys = rY * rY;
		}

		double dq = (rxs * y1ps + rys * x1ps);
		double pq = (rxs*rys - dq) / dq;
		double q = sqrt(max(0, pq));
		double cxp = q * rX * y1p / rY;
		double cyp = -q * rY * x1p / rX;
		double cx = cxp + (x1 + x2) / 2;
		double cy = cyp + (y1 + y2) / 2;

		double en1x = cx + rx * cos(theta);
		double en1y = cy + ry * sin(theta);
		double en2x = cx + rx * cos(delta);
		double en2y = cy + ry * sin(delta);

		double edn1x = -1 * rx * sin(theta);
		double edn1y = ry * cos(theta);
		double edn2x = -1 * rx * sin(delta);
		double edn2y = ry * cos(delta);

		double alpha = sin(delta - theta) * (sqrt(4 + 3 * pow(tan((delta - theta) / 2), 2)) - 1) / 3;

		double cpx1 = en1x + alpha * edn1x;
		double cpy1 = en1y + alpha * edn1y;
		double cpx2 = en2x - alpha * edn2x;
		double cpy2 = en2y - alpha * edn2y;



		//std::vector<double> data;
		//data.push_back(n1);
		//d (n1);

		//double interval = PI / 2;

		//while (data[data.size() - 1] + interval < n2)
		//	data.push_back(data[data.size() - 1] + interval);
		//data.push_back(n2);
	}
#endif
	void GraphicDrawHandle::ArctoBezier(double cx, double cy, double rx, double ry,
		double start_angle, double sweep_angle, double* curve)
	{
		double x0 = cos(sweep_angle / 2.0);
		double y0 = sin(sweep_angle / 2.0);
		double tx = (1.0 - x0) * 4.0 / 3.0;
		double ty = y0 - tx * x0 / y0;
		double px[4];
		double py[4];
		px[0] = x0;
		py[0] = -y0;
		px[1] = x0 + tx;
		py[1] = -ty;
		px[2] = x0 + tx;
		py[2] = ty;
		px[3] = x0;
		py[3] = y0;

		double sn = sin(start_angle + sweep_angle / 2.0);
		double cs = cos(start_angle + sweep_angle / 2.0);
		//unsigned i;
		//for (i = 0; i < 4; i++)
		//{
		//	curve[i * 2] = cx + rx * (px[i] * cs - py[i] * sn);
		//	curve[i * 2 + 1] = cy + ry * (px[i] * sn + py[i] * cs);
		//}
		double r = (rx * ry) / sqrt(pow(sin(start_angle) * rx, 2.0) + pow(cos(start_angle) * ry, 2.0));
		curve[0] = cx + r * cos(start_angle);
		curve[1] = cy + r * sin(start_angle);

		r = (rx * ry) / sqrt(pow(sin(start_angle + sweep_angle) * rx, 2.0) + pow(cos(start_angle + sweep_angle) * ry, 2.0));
		curve[6] = cx + r * cos(start_angle + sweep_angle);
		curve[7] = cy + r * sin(start_angle + sweep_angle);

		double en1x = curve[0];
		double en1y = curve[1];
		double en2x = curve[6];
		double en2y = curve[7];

		double edn1x = -1 * rx * sin(start_angle);
		double edn1y = ry * cos(start_angle);
		double edn2x = -1 * rx * sin(start_angle + sweep_angle);
		double edn2y = ry * cos(start_angle + sweep_angle);

		double alpha = sin(sweep_angle) * (sqrt(4 + 3 * pow(tan((sweep_angle) / 2), 2)) - 1) / 3;

		curve[2] = en1x + alpha * edn1x;
		curve[3] = en1y + alpha * edn1y;
		curve[4] = en2x - alpha * edn2x;
		curve[5] = en2y - alpha * edn2y;
		//a = 4 * tan(sweep_angle / 4) / 3;
		//curve[2] = curve[0] - a * (curve[1] - cy);
		//curve[3] = curve[1] + a * (curve[0] - cx);
		//curve[4] = curve[6] + a * (curve[6] - cy);
		//curve[5] = curve[7] - a * (curve[7] - cx);
	}
	void GraphicDrawHandle::DrawLine(double startx, double starty, double endx, double endy)
	{
#ifdef WIN32
		double x1 = fl_transform_x(startx, starty);
		double y1 = fl_transform_y(startx, starty);
		double x2 = fl_transform_x(endx, endy);
		double y2 = fl_transform_y(endx, endy);

		graphics->DrawLine(pen, (INT)x1, (INT)y1, (INT)x2, (INT)y2);
#else
		fl_begin_line();
		fl_vertex(startx, starty);
		fl_vertex(endx, endy);
		fl_end_line();
#endif // WIN32
	}
	void GraphicDrawHandle::DrawPolyLine(bool close, std::vector<Project::Point>& points, int offx, int offy)
	{
		if (points.empty())
			return;
#ifdef WIN32
		Gdiplus::Point *winpoint = new Gdiplus::Point[points.size()];
		for (size_t i = 0; i < points.size(); i++)
		{
			winpoint[i].X = (int)fl_transform_x(points[i].X + offx, points[i].Y + offy);
			winpoint[i].Y = (int)fl_transform_y(points[i].X + offx, points[i].Y + offy);
		}
		if (close)
			graphics->DrawPolygon(pen, winpoint, points.size());
		else
			graphics->DrawCurve(pen, winpoint, points.size(), 0.0f);
		delete[] winpoint;
#else
		fl_begin_line();
		for (size_t i = 0; i < points.size(); i++)
			fl_vertex(points[i].X + offx, points[i].Y + offy);
		if (close)
			fl_vertex(points[0].X + offx, points[0].Y + offy);
		fl_end_line();
#endif // WIN32
	}
	void GraphicDrawHandle::DrawRectangle(double radius, double x, double y, double w, double h, double linew)
	{
#ifdef WIN32
		/*上边框*/
		Gdiplus::Point winpoint[2] ={
			Gdiplus::Point((int)fl_transform_x(x + linew / 2 + radius - 1, y + linew / 2),
			(int)fl_transform_y(x + linew / 2 + radius - 1, y + linew / 2)),
			Gdiplus::Point((int)fl_transform_x(x - linew / 2 + w - radius + 1,y + linew / 2),
			(int)fl_transform_y(x - linew / 2 + w - radius + 1,y + linew / 2)) };
		graphics->DrawCurve(pen, winpoint, 2, 0.0f);
		/*下边框*/
		winpoint[0].X = (int)fl_transform_x(x + linew / 2 + radius - 1, y - linew / 2 + h);
		winpoint[0].Y = (int)fl_transform_y(x + linew / 2 + radius - 1, y - linew / 2 + h);
		winpoint[1].X = (int)fl_transform_x(x - linew / 2 + w - radius + 1, y - linew / 2 + h);
		winpoint[1].Y = (int)fl_transform_y(x - linew / 2 + w - radius + 1, y - linew / 2 + h);
		graphics->DrawCurve(pen, winpoint, 2, 0.0f);
		/*左边框*/
		winpoint[0].X = (int)fl_transform_x(x + linew / 2, y + linew / 2 + radius - 1);
		winpoint[0].Y = (int)fl_transform_y(x + linew / 2, y + linew / 2 + radius - 1);
		winpoint[1].X = (int)fl_transform_x(x + linew / 2, y - linew / 2 + h - radius + 1);
		winpoint[1].Y = (int)fl_transform_y(x + linew / 2, y - linew / 2 + h - radius + 1);
		graphics->DrawCurve(pen, winpoint, 2, 0.0f);
		/*右边框*/
		winpoint[0].X = (int)fl_transform_x(x - linew / 2 + w, y + linew / 2 + radius - 1);
		winpoint[0].Y = (int)fl_transform_y(x - linew / 2 + w, y + linew / 2 + radius - 1);
		winpoint[1].X = (int)fl_transform_x(x - linew / 2 + w, y - linew / 2 + h - radius + 1);
		winpoint[1].Y = (int)fl_transform_y(x - linew / 2 + w, y - linew / 2 + h - radius + 1);
		graphics->DrawCurve(pen, winpoint, 2, 0.0f);

		double *curve = new double[8];
		double start, end;
		//左上圆角
		start = -90.0 * 3.1415926 / 180.0;
		end = -90.0 * 3.1415926 / 180.0;
		ArctoBezier(x + linew / 2 + radius, y + linew / 2 + radius,
			radius, radius, start, end, curve);
		Gdiplus::Point p1[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
							   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
							   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
							   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
		graphics->DrawBeziers(pen, p1, 4);

		//右上圆角
		start = -0.0 * 3.1415926 / 180.0;
		ArctoBezier(x - linew / 2 + w - radius, y + linew / 2 + radius,
			radius, radius, start, end, curve);
		Gdiplus::Point p2[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
							   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
							   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
							   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
		graphics->DrawBeziers(pen, p2, 4);

		//左下圆角
		start = -180.0 * 3.1415926 / 180.0;
		ArctoBezier(x + linew / 2 + radius, y - linew / 2 + h - radius,
			radius, radius, start, end, curve);
		Gdiplus::Point p3[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
							   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
							   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
							   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
		graphics->DrawBeziers(pen, p3, 4);

		//右下圆角
		start = -270.0 * 3.1415926 / 180.0;
		ArctoBezier(x - linew / 2 + w - radius, y - linew / 2 + h - radius,
			radius, radius, start, end, curve);
		Gdiplus::Point p4[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
							   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
							   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
							   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
		graphics->DrawBeziers(pen, p4, 4);
#if 0
		/*左上角圆*/
		graphics->DrawArc(pen,(INT)(x + linew / 2), y + linew / 2,
			radius * 2, radius * 2, -90, -90);
		/*右上角圆*/
		graphics->DrawArc(pen, (INT)(x - linew / 2 + w - radius * 2), y + linew / 2,
			radius * 2, radius * 2, 0, -90);
		/*左下角圆*/
		graphics->DrawArc(pen, (INT)(x + linew / 2 ), y - linew / 2 + h - radius * 2,
			radius * 2, radius * 2, -180, -90);
		/*右下角圆*/
		graphics->DrawArc(pen, (INT)(x - linew / 2 + w - radius * 2), y - linew / 2 + h - radius * 2,
			radius * 2, radius * 2, -270, -90);
#endif
#else
		fl_begin_line();
		fl_vertex(x + linew / 2 + radius,	y + linew / 2  );
		fl_vertex(x - linew / 2 + w - radius,y + linew / 2  );
		fl_end_line();


		fl_begin_line();
		fl_vertex(x + linew / 2 + radius, y - linew / 2 + h);
		fl_vertex(x - linew / 2 + w - radius, y - linew / 2 + h);
		fl_end_line();

		fl_begin_line();
		fl_vertex(x + linew / 2, y + linew / 2 + radius);
		fl_vertex(x + linew / 2, y - linew / 2 + h - radius);
		fl_end_line();

		fl_begin_line();
		fl_vertex(x - linew / 2 + w, y + linew / 2 + radius);
		fl_vertex(x - linew / 2 + w, y - linew / 2 + h - radius);
		fl_end_line();

		double *curve = new double[8];
		double start, end;
		//左上圆角
		start = -90.0 * 3.1415926 / 180.0;
		end = -90.0 * 3.1415926 / 180.0;
		ArctoBezier(x + linew / 2 + radius, y + linew / 2 + radius,
			radius, radius, start, end, curve);
		fl_begin_line();
		fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
		fl_end_line();

		//右上圆角
		start = -0.0 * 3.1415926 / 180.0;
		ArctoBezier(x - linew / 2 + w - radius,	y + linew / 2 + radius,
			radius, radius, start, end, curve);
		fl_begin_line();
		fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
		fl_end_line();

		//左下圆角
		start = -180.0 * 3.1415926 / 180.0;
		ArctoBezier(x + linew / 2 + radius, y - linew / 2 + h - radius,
			radius, radius, start, end, curve);
		fl_begin_line();
		fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
		fl_end_line();

		//右下圆角
		start = -270.0 * 3.1415926 / 180.0;
		ArctoBezier(x - linew / 2 + w -radius, y - linew / 2 + h - radius,
			radius, radius, start, end, curve);
		fl_begin_line();
		fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
		fl_end_line();
#endif // WIN32
	}
	void GraphicDrawHandle::DrawArc(bool isSector, Project::Point center, double width, double height,
		double sAngle, double eAngle, double startx, double starty, double endx, double endy)
	{
		int pointX = center.X ;
		int pointY = center.Y;			//圆心坐标
		int arcX = pointX - (int)width;			//指定限定矩形左上角的X坐标
		int	arcY = pointY - (int)height;			//指定限定矩形左上角的y坐标
		/*开始绘制圆弧*/
		fl_begin_line();
#ifdef WIN32
		if (fabs(sAngle - eAngle) < 90) {
			if (startx == endx && starty == endy) SetPixel(fl_gc, (int)startx, (int)starty, fl_RGB());
			else Arc(fl_gc, (int)arcX, (int)arcY, arcX + (int)width * 2, arcY + (int)height * 2,
				(int)startx, (int)starty, (int)endx, (int)endy);
		}
		else Arc(fl_gc, (int)arcX, (int)arcY, arcX + (int)width * 2, 
			arcY + (int)height * 2, (int)startx, (int)starty, (int)endx, (int)endy);
#else //X11
        fl_arc(arcX, arcY, width * 2, height * 2, sAngle, eAngle);
#endif
		fl_end_line();
		/*绘制边框线*/
		if (isSector)
		{
			fl_line(pointX, pointY, (int)startx, (int)starty);		//绘制起始线段
			fl_line(pointX, pointY, (int)endx, (int)endy);		//绘制终止线段
		}

	}

	void GraphicDrawHandle::DrawArc(bool isSector, Project::Point center, double xlength, double ylength, double startangle, double endangle)
	{
#ifdef WIN32
		double startlineX, startlineY, endlineX, endlineY;
		double *curve = new double[8];
		double start, end;
		double sweepangle;
		/*计算扫描角度*/
		if (endangle - startangle < 0)
			sweepangle = 360 + (endangle - startangle);
		else
			sweepangle = endangle - startangle;
		double nextsweepangle = 0;
		bool hasStartpoint = false;
		/*为了避免绘制偏差*/
		//double r = (xlength * ylength) / sqrt(pow(sin(startangle) * xlength, 2.0) + pow(cos(startangle) * ylength, 2.0));
		//startlineX = center.X + r * cos(startangle);
		//startlineY = center.Y + r * sin(startangle);

		//r = (xlength * ylength) / sqrt(pow(sin(endangle) * xlength, 2.0) + pow(cos(endangle) * ylength, 2.0));
		//endlineX = center.X + r * cos(endangle);
		//endlineY = center.Y + r * sin(endangle);
		while (sweepangle > 2.5)
		{
			start = -startangle * 3.1415926 / 180.0;
			end = -2.5 * 3.1415926 / 180.0;
			ArctoBezier(center.X, center.Y, xlength, ylength, start, end, curve);
			if (!hasStartpoint)
			{
				startlineX = curve[0];
				startlineY = curve[1];
				hasStartpoint = true;
			}
			Gdiplus::Point p1[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
								   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
								   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
								   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
			graphics->DrawBeziers(pen, p1, 4);
			startangle += 2.5;
			sweepangle -= 2.5;
		}
		if (sweepangle >= 0)
		{
			start = -startangle * 3.1415926 / 180.0;
			end = -sweepangle * 3.1415926 / 180.0;
			ArctoBezier(center.X, center.Y, xlength, ylength, start, end, curve);
			if (!hasStartpoint)
			{
				startlineX = curve[0];
				startlineY = curve[1];
				hasStartpoint = true;
			}
			Gdiplus::Point p2[] = { Point((int)fl_transform_x(curve[0], curve[1]),(int)fl_transform_y(curve[0], curve[1])),
								   Point((int)fl_transform_x(curve[2], curve[3]),(int)fl_transform_y(curve[2], curve[3])),
								   Point((int)fl_transform_x(curve[4], curve[5]),(int)fl_transform_y(curve[4], curve[5])),
								   Point((int)fl_transform_x(curve[6], curve[7]),(int)fl_transform_y(curve[6], curve[7])), };
			graphics->DrawBeziers(pen, p2, 4);
		}
		endlineX = curve[6];
		endlineY = curve[7];
		if (isSector)
		{
			graphics->DrawLine(pen, (INT)(int)fl_transform_x(startlineX, startlineY), (int)fl_transform_y(startlineX, startlineY),
				(int)fl_transform_x(center.X, center.Y), (int)fl_transform_y(center.X, center.Y));
			graphics->DrawLine(pen, (INT)(int)fl_transform_x(endlineX, endlineY), (int)fl_transform_y(endlineX, endlineY),
				(int)fl_transform_x(center.X, center.Y), (int)fl_transform_y(center.X, center.Y));
		}
		delete[] curve;

#else
		fl_begin_line();
		double startlineX, startlineY, endlineX, endlineY;
		double *curve = new double[8];
		double start, end;
		double sweepangle;
		/*计算扫描角度*/
		if (endangle - startangle < 0)
			sweepangle = 360 + (endangle - startangle);
		else
			sweepangle = endangle - startangle;
		double nextsweepangle = 0;
		bool hasStartpoint = false;

		/*为了避免绘制偏差*/
		while (sweepangle > 2.5)
		{
			start = -startangle * 3.1415926 / 180.0;
			end = -2.5 * 3.1415926 / 180.0;
			ArctoBezier(center.X, center.Y, xlength, ylength, start, end, curve);
			if (!hasStartpoint)
			{
				startlineX = curve[0];
				startlineY = curve[1];
				hasStartpoint = true;
			}
			fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
			startangle += 2.5;
			sweepangle -= 2.5;
		}
		if (sweepangle >= 0)
		{
			start = -startangle * 3.1415926 / 180.0;
			end = -sweepangle * 3.1415926 / 180.0;
			ArctoBezier(center.X, center.Y, xlength, ylength, start, end, curve);
			if (!hasStartpoint)
			{
				startlineX = curve[0];
				startlineY = curve[1];
				hasStartpoint = true;
			}
			fl_curve(curve[0], curve[1], curve[2], curve[3], curve[4], curve[5], curve[6], curve[7]);
		}
		endlineX = curve[6];
		endlineY = curve[7];

		fl_end_line();
		if (isSector)
		{
			fl_begin_line();
			fl_vertex(startlineX, startlineY);
			fl_vertex(center.X, center.Y);
			fl_vertex(endlineX, endlineY);
			fl_end_line();
		}
		delete[] curve;
#endif // WIN32
	}
	void GraphicDrawHandle::DrawCircle(Project::Point center, double xlength, double ylength)
	{
#ifdef WIN32
		const double EToBConst = 0.2761423749154;
		double offsetx = xlength * 2 * EToBConst;
		double offsety = ylength * 2 * EToBConst;

		Project::DoublePoint cCtlPt[13];
		double x = center.X - xlength;
		double y = center.Y - ylength;
		cCtlPt[0].X = cCtlPt[1].X = cCtlPt[11].X = cCtlPt[12].X = x;
		cCtlPt[5].X = cCtlPt[6].X = cCtlPt[7].X = x + 2 * xlength;
		cCtlPt[2].X = cCtlPt[10].X = center.X - offsetx;
		cCtlPt[4].X = cCtlPt[8].X = center.X + offsetx;
		cCtlPt[3].X = cCtlPt[9].X = center.X;
		cCtlPt[2].Y = cCtlPt[3].Y = cCtlPt[4].Y = y;
		cCtlPt[8].Y = cCtlPt[9].Y = cCtlPt[10].Y = y + 2 * ylength;
		cCtlPt[7].Y = cCtlPt[11].Y = center.Y + offsety;
		cCtlPt[1].Y = cCtlPt[5].Y = center.Y - offsety;
		cCtlPt[0].Y = cCtlPt[12].Y = cCtlPt[6].Y = center.Y;

		Gdiplus::Point p1[] = { Point((int)fl_transform_x(cCtlPt[0].X, cCtlPt[0].Y),(int)fl_transform_y(cCtlPt[0].X, cCtlPt[0].Y)),
					   Point((int)fl_transform_x(cCtlPt[1].X, cCtlPt[1].Y),(int)fl_transform_y(cCtlPt[1].X, cCtlPt[1].Y)),
					   Point((int)fl_transform_x(cCtlPt[2].X, cCtlPt[2].Y),(int)fl_transform_y(cCtlPt[2].X, cCtlPt[2].Y)),
					   Point((int)fl_transform_x(cCtlPt[3].X, cCtlPt[3].Y),(int)fl_transform_y(cCtlPt[3].X, cCtlPt[3].Y)), };
		graphics->DrawBeziers(pen, p1, 4);

		Gdiplus::Point p2[] = { Point((int)fl_transform_x(cCtlPt[3].X, cCtlPt[3].Y),(int)fl_transform_y(cCtlPt[3].X, cCtlPt[3].Y)),
			           Point((int)fl_transform_x(cCtlPt[4].X, cCtlPt[4].Y),(int)fl_transform_y(cCtlPt[4].X, cCtlPt[4].Y)),
					   Point((int)fl_transform_x(cCtlPt[5].X, cCtlPt[5].Y),(int)fl_transform_y(cCtlPt[5].X, cCtlPt[5].Y)),
					   Point((int)fl_transform_x(cCtlPt[6].X, cCtlPt[6].Y),(int)fl_transform_y(cCtlPt[6].X, cCtlPt[6].Y)),};
		graphics->DrawBeziers(pen, p2, 4);

		Gdiplus::Point p3[] = { Point((int)fl_transform_x(cCtlPt[6].X, cCtlPt[6].Y),(int)fl_transform_y(cCtlPt[6].X, cCtlPt[6].Y)),
					   Point((int)fl_transform_x(cCtlPt[7].X, cCtlPt[7].Y),(int)fl_transform_y(cCtlPt[7].X, cCtlPt[7].Y)),
					   Point((int)fl_transform_x(cCtlPt[8].X, cCtlPt[8].Y),(int)fl_transform_y(cCtlPt[8].X, cCtlPt[8].Y)),
					   Point((int)fl_transform_x(cCtlPt[9].X, cCtlPt[9].Y),(int)fl_transform_y(cCtlPt[9].X, cCtlPt[9].Y)),};
		graphics->DrawBeziers(pen, p3, 4);

		Gdiplus::Point p4[] = {	Point((int)fl_transform_x(cCtlPt[9].X, cCtlPt[9].Y),(int)fl_transform_y(cCtlPt[9].X, cCtlPt[9].Y)),
			           Point((int)fl_transform_x(cCtlPt[10].X, cCtlPt[10].Y),(int)fl_transform_y(cCtlPt[10].X, cCtlPt[10].Y)),
					   Point((int)fl_transform_x(cCtlPt[11].X, cCtlPt[11].Y),(int)fl_transform_y(cCtlPt[11].X, cCtlPt[11].Y)),
					   Point((int)fl_transform_x(cCtlPt[12].X, cCtlPt[12].Y),(int)fl_transform_y(cCtlPt[12].X, cCtlPt[12].Y)),};
		graphics->DrawBeziers(pen, p4, 4);
#else
		fl_begin_line();
		const double EToBConst = 0.2761423749154;
		double offsetx = xlength * 2 * EToBConst;
		double offsety = ylength * 2 * EToBConst;

		Project::Point cCtlPt[13];
		double x = center.X - xlength;
		double y = center.Y - ylength;
		cCtlPt[0].X = cCtlPt[1].X = cCtlPt[11].X = cCtlPt[12].X = x;
		cCtlPt[5].X = cCtlPt[6].X = cCtlPt[7].X = x + 2 * xlength;
		cCtlPt[2].X = cCtlPt[10].X = center.X - offsetx;
		cCtlPt[4].X = cCtlPt[8].X = center.X + offsetx;
		cCtlPt[3].X = cCtlPt[9].X = center.X;
		cCtlPt[2].Y = cCtlPt[3].Y = cCtlPt[4].Y = y;
		cCtlPt[8].Y = cCtlPt[9].Y = cCtlPt[10].Y = y + 2 * ylength;
		cCtlPt[7].Y = cCtlPt[11].Y = center.Y + offsety;
		cCtlPt[1].Y = cCtlPt[5].Y = center.Y - offsety;
		cCtlPt[0].Y = cCtlPt[12].Y = cCtlPt[6].Y = center.Y;

		fl_curve(cCtlPt[0].X, cCtlPt[0].Y, cCtlPt[1].X, cCtlPt[1].Y, cCtlPt[2].X, cCtlPt[2].Y, cCtlPt[3].X, cCtlPt[3].Y);
		fl_curve(cCtlPt[3].X, cCtlPt[3].Y, cCtlPt[4].X, cCtlPt[4].Y, cCtlPt[5].X, cCtlPt[5].Y, cCtlPt[6].X, cCtlPt[6].Y);
		fl_curve(cCtlPt[6].X, cCtlPt[6].Y, cCtlPt[7].X, cCtlPt[7].Y, cCtlPt[8].X, cCtlPt[8].Y, cCtlPt[9].X, cCtlPt[9].Y);
		fl_curve(cCtlPt[9].X, cCtlPt[9].Y, cCtlPt[10].X, cCtlPt[10].Y, cCtlPt[11].X, cCtlPt[11].Y, cCtlPt[12].X, cCtlPt[12].Y);
		fl_end_line();
		//delete[] cCtlPt;
#endif // WIN32
	}
	void GraphicDrawHandle::DrawArrow(Project::ArrowType style, double angle, int& x, int& y, int size)
	{
		int tmpx = x, tmpy = y;
		fl_push_matrix();
		fl_translate(x, y);
		fl_rotate(angle);
		fl_translate(-x, -y);
		switch (style)
		{
		case Project::ArrowType::TriangleArrow:
		{
#ifdef WIN32
			//pen->SetWidth(0);
			Gdiplus::Point p1[] = { Point((int)fl_transform_x(x, y),(int)fl_transform_y(x, y)),
									Point((int)fl_transform_x((double)x - (double)size / 2.0, (double)y + (double)size),
										(int)fl_transform_y((double)x - (double)size / 2.0, (double)y + (double)size)),
									Point((int)fl_transform_x((double)x + (double)size / 2.0, (double)y + (double)size),
										(int)fl_transform_y((double)x + (double)size / 2.0, (double)y + (double)size)), };
			graphics->FillPolygon(brush,p1,3);
#else
			//fl_line_style(0, 1);   //箭头边框颜色 宽度
			fl_begin_complex_polygon();
			fl_vertex(x, y);
			fl_vertex((double)x - (double)size / 2.0, (double)y + (double)size);
			fl_vertex((double)x + (double)size / 2.0, (double)y + (double)size);
			fl_end_complex_polygon();
#endif // WIN32
			break;
		}
		case Project::ArrowType::Arrow:
		{
#ifdef WIN32
			Gdiplus::Point p1[] = { Point((int)fl_transform_x(x, y),(int)fl_transform_y(x, y)),
									Point((int)fl_transform_x((double)x - (double)size / 2.0, (double)y + (double)size),
										(int)fl_transform_y((double)x - (double)size / 2.0, (double)y + (double)size)),
									Point((int)fl_transform_x((double)x + (double)size / 2.0, (double)y + (double)size),
										(int)fl_transform_y((double)x + (double)size / 2.0, (double)y + (double)size)), };
			graphics->DrawPolygon(pen,p1,3);

#else
			//fl_line_style(0, 1);   //箭头边框颜色 宽度
			fl_begin_line();
			fl_vertex(x - (double)size / 2.0, y + (double)size);
			fl_vertex(x, y);
			fl_vertex(x + (double)size / 2.0, y + (double)size);
			fl_end_line();
#endif // WIN32
			break;
		}
		case Project::ArrowType::SolidArrow:
		{
#ifdef WIN32
			Gdiplus::Point p1[] = { Point((int)fl_transform_x(x, y),(int)fl_transform_y(x, y)),
									Point((int)fl_transform_x(x - size / 2, y + size),(int)fl_transform_y(x - size / 2, y + size)),
									Point((int)fl_transform_x(x, y + size * 2 / 3), (int)fl_transform_y(x, y + size * 2 / 3)),
									Point((int)fl_transform_x(x + size / 2, y + size), (int)fl_transform_y(x + size / 2, y + size)), };
			graphics->FillPolygon(brush, p1, 4);
#else
			//fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER, 1);   //箭头边框颜色 宽度
			fl_begin_complex_polygon();
			fl_vertex(x, y);
			fl_vertex(x - size / 2, y + size);
			fl_vertex(x, y + size * 2 / 3);
			fl_vertex(x + size / 2, y + size);
			fl_vertex(x, y);
			fl_end_complex_polygon();
#endif
			break;
		}
		case Project::ArrowType::RectArrow:
		{
#ifdef WIN32
			Gdiplus::Point p1[] = { Point((int)fl_transform_x(x, y),(int)fl_transform_y(x, y)),
									Point((int)fl_transform_x(x - size / 2, y + size),(int)fl_transform_y(x - size / 2, y + size)),
									Point((int)fl_transform_x(x, y + size * 2), (int)fl_transform_y(x, y + size * 2)),
									Point((int)fl_transform_x(x + size / 2, y + size), (int)fl_transform_y(x + size / 2, y + size)), };
			graphics->FillPolygon(brush, p1, 4);
#else
			//fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER, 1);   //箭头边框颜色 宽度
			fl_begin_complex_polygon();
			fl_vertex(x, y);
			fl_vertex(x - size / 2, y + size);
			fl_vertex(x, y + 2 * size);
			fl_vertex(x + size / 2, y + size);
			fl_end_complex_polygon();
#endif
			break;
		}
		case Project::ArrowType::DotArrow:
		{
#ifdef WIN32
			int pointx = (int)fl_transform_x(x, y);
			int pointy = (int)fl_transform_y(x, y);
			graphics->FillPie(brush, pointx - size / 2, pointy - size / 2, size, size, 0, 360);
#else
			fl_pie(x - size / 2, y - size / 2, size, size, 0, 360);
#endif
			break;
		}
		default:
			break;
		}
		if (style != Project::ArrowType::Arrow)
		{
			x = (int)fl_transform_x((double)tmpx, (double)(tmpy + size / 2));
			y = (int)fl_transform_y((double)tmpx, (double)(tmpy + size / 2));
		}
		fl_pop_matrix();
	}
	void GraphicDrawHandle::DrawRenderScroll(int mode, int X, int Y, int W, int H)
	{
		fl_draw_box(FL_FLAT_BOX, X, Y, W, H, FL_DARK2);
		if (mode) {				//0:垂直 1:水平
			int xsl, ysl, wsl, hsl;
			xsl = X + H;
			wsl = W / 3;
			ysl = Y;
			hsl = H;
			fl_draw_box(FL_UP_BOX, xsl, ysl, wsl, hsl, FL_GRAY);
			fl_draw_box(FL_UP_BOX, X, Y, H, H, FL_GRAY);
			fl_draw_box(FL_UP_BOX, X + W - H, Y, H, H, FL_GRAY);
			fl_color(FL_FOREGROUND_COLOR);
			int w1 = (H - 4) / 3; if (w1 < 1) w1 = 1;
			int x1 = X + (H - w1 - 1) / 2;
			int yy1 = Y + (H - 2 * w1 - 1) / 2;
			fl_polygon(x1, yy1 + w1, x1 + w1, yy1 + 2 * w1, x1 + w1, yy1);
			x1 += (W - H);
			fl_polygon(x1, yy1, x1, yy1 + 2 * w1, x1 + w1, yy1 + w1);
		}
		else
		{
			int xsl, ysl, wsl, hsl;
			ysl = Y + W;
			hsl = H / 3;
			xsl = X;
			wsl = W;
			fl_draw_box(FL_UP_BOX, xsl, ysl, wsl, hsl, FL_GRAY);
			fl_draw_box(FL_UP_BOX, X, Y, W, W, FL_GRAY);
			fl_draw_box(FL_UP_BOX, X, Y + H - W, W, W, FL_GRAY);
			fl_color(FL_FOREGROUND_COLOR);
			int w1 = (W - 4) / 3; if (w1 < 1) w1 = 1;
			int x1 = X + (W - 2 * w1 - 1) / 2;
			int yy1 = Y + (W - w1 - 1) / 2;
			fl_polygon(x1, yy1 + w1, x1 + 2 * w1, yy1 + w1, x1 + w1, yy1);
			yy1 += H - W;
			fl_polygon(x1, yy1, x1 + w1, yy1 + w1, x1 + 2 * w1, yy1);
		}
	}
	void GraphicDrawHandle::PushClip(double x, double y, double w, double h)
	{
		double zoom = UI::IResourceService::Ins()->GetZoom();
		fl_push_clip((int)(x * zoom), (int)(y * zoom), (int)(w * zoom), (int)(h * zoom));
	}

}

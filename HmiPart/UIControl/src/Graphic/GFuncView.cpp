/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerView.cpp
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : 函数画布控件绘制类
 * Version  : 0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "GFuncView.h"
#include "GFuncModel.h"

namespace UI
{
	GFuncView::GFuncView(int x, int y, int w, int h)
		: HMIBaseView(x, y, w, h) {

	}

	GFuncView::~GFuncView() {

	}

	void GFuncView::draw() {
		shared_ptr<GFuncModel> model = BaseView.GetModel<GFuncModel>();
		switch (model->GFuncUnit.FuncId)
		{
		case Project::DrawFunc::DCMapClear: 
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X, 
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width, 
						 model->GFuncUnit.Height);
			// 设置原始背景色
			Fl_Color originColor = fl_rgb_color(255, 255, 255);
			fl_color(originColor);
			// 绘制画布背景颜色
			fl_rectf(model->GFuncUnit.X, 
					 model->GFuncUnit.Y, 
					 model->GFuncUnit.Width, 
					 model->GFuncUnit.Height);
			fl_pop_clip();
			break;
		}	
		case Project::DrawFunc::DCMapSetBackColor:
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X, 
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width, 
						 model->GFuncUnit.Height);
			// 设置背景颜色
			Fl_Color backColor = fl_rgb_color(model->GFuncUnit.BackColor.R,
											  model->GFuncUnit.BackColor.G,
											  model->GFuncUnit.BackColor.B);
			fl_color(backColor);
			// 绘制画布背景颜色
			fl_rectf(model->GFuncUnit.X, 
					 model->GFuncUnit.Y, 
					 model->GFuncUnit.Width, 
					 model->GFuncUnit.Height);
			fl_pop_clip();
			break;
		}
		case Project::DrawFunc::DCMapDrawLine:
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X,
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width,
						 model->GFuncUnit.Height);
			// 绘制线条
			if (model->GFuncUnit.LineWidth > 0) {
				// 设置线条样式
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// 设置线条颜色
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// 绘制线条
				fl_line(model->GFuncUnit.X + model->GFuncUnit.GX,
						model->GFuncUnit.Y + model->GFuncUnit.GY,
						model->GFuncUnit.X + model->GFuncUnit.GX + model->GFuncUnit.GW,
						model->GFuncUnit.Y + model->GFuncUnit.GY + model->GFuncUnit.GH);
			}
			fl_pop_clip();
			break;
		}	
		case Project::DrawFunc::DCMapDrawRect:
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X,
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width,
						 model->GFuncUnit.Height);
			// 是否填充
			if (model->GFuncUnit.FillRect) {
				// 设置填充颜色
				Fl_Color fillColor = fl_rgb_color(model->GFuncUnit.FillColor.R,
												  model->GFuncUnit.FillColor.G,
												  model->GFuncUnit.FillColor.B);
				fl_color(fillColor);
				// 填充内部
				fl_rectf(model->GFuncUnit.X + model->GFuncUnit.GX,
						 model->GFuncUnit.Y + model->GFuncUnit.GY,
						 model->GFuncUnit.GW,
						 model->GFuncUnit.GH);
			}
			// 绘制边框
			if (model->GFuncUnit.LineWidth > 0) {
				// 设置线条样式
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// 设置线条颜色
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				// 绘制边框
				fl_rect(model->GFuncUnit.X + model->GFuncUnit.GX,
						model->GFuncUnit.Y + model->GFuncUnit.GY,
						model->GFuncUnit.GW,
						model->GFuncUnit.GH, lineColor);
			}
			fl_pop_clip();
			break;
		}	
		case Project::DrawFunc::DCMapDrawCircle:
		case Project::DrawFunc::DCMapDrawEllipse:
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X,
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width,
						 model->GFuncUnit.Height);
			// 是否填充
			if (model->GFuncUnit.FillRect)
			{
				// 设置填充颜色
				Fl_Color fillColor = fl_rgb_color(model->GFuncUnit.FillColor.R,
												  model->GFuncUnit.FillColor.G,
												  model->GFuncUnit.FillColor.B);
				fl_color(fillColor);
				// 填充内部
				fl_pie(model->GFuncUnit.X + model->GFuncUnit.GX - model->GFuncUnit.GW,
					   model->GFuncUnit.Y + model->GFuncUnit.GY - model->GFuncUnit.GH,
					   model->GFuncUnit.GW * 2,
					   model->GFuncUnit.GH * 2, 0, 360);
			}
			// 绘制边框
			if (model->GFuncUnit.LineWidth > 0) {
				// 设置线条样式
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// 设置线条颜色
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// 绘制边框
				fl_arc(model->GFuncUnit.X + model->GFuncUnit.GX - model->GFuncUnit.GW,
					   model->GFuncUnit.Y + model->GFuncUnit.GY - model->GFuncUnit.GH,
					   model->GFuncUnit.GW * 2, 
					   model->GFuncUnit.GH * 2, 0, 360);
			}
			fl_pop_clip();
			break;
		}
		case Project::DrawFunc::DCMapDrawCircleArc:
		case Project::DrawFunc::DCMapDrawEllipseArc:
		{
			GraphicDrawHandle::PushClip(model->GFuncUnit.X,
						 model->GFuncUnit.Y,
						 model->GFuncUnit.Width,
						 model->GFuncUnit.Height);
			// 绘制边框
			if (model->GFuncUnit.LineWidth > 0) {
				// 设置线条样式
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// 设置线条颜色
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// 绘制边框
				fl_arc(model->GFuncUnit.X + model->GFuncUnit.GX - model->GFuncUnit.GW,
					   model->GFuncUnit.Y + model->GFuncUnit.GY - model->GFuncUnit.GH,
					   model->GFuncUnit.GW * 2,
					   model->GFuncUnit.GH * 2,
					   model->GFuncUnit.StartAngle,
					   model->GFuncUnit.EndAngle);
			}
			fl_pop_clip();
			break;
		}
		default:
			break;
		}
	}
}
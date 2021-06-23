/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StirrerView.cpp
 * Author   : TangYao
 * Date     : 2021/02/07
 * Descript : ���������ؼ�������
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
			// ����ԭʼ����ɫ
			Fl_Color originColor = fl_rgb_color(255, 255, 255);
			fl_color(originColor);
			// ���ƻ���������ɫ
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
			// ���ñ�����ɫ
			Fl_Color backColor = fl_rgb_color(model->GFuncUnit.BackColor.R,
											  model->GFuncUnit.BackColor.G,
											  model->GFuncUnit.BackColor.B);
			fl_color(backColor);
			// ���ƻ���������ɫ
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
			// ��������
			if (model->GFuncUnit.LineWidth > 0) {
				// ����������ʽ
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// ����������ɫ
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// ��������
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
			// �Ƿ����
			if (model->GFuncUnit.FillRect) {
				// ���������ɫ
				Fl_Color fillColor = fl_rgb_color(model->GFuncUnit.FillColor.R,
												  model->GFuncUnit.FillColor.G,
												  model->GFuncUnit.FillColor.B);
				fl_color(fillColor);
				// ����ڲ�
				fl_rectf(model->GFuncUnit.X + model->GFuncUnit.GX,
						 model->GFuncUnit.Y + model->GFuncUnit.GY,
						 model->GFuncUnit.GW,
						 model->GFuncUnit.GH);
			}
			// ���Ʊ߿�
			if (model->GFuncUnit.LineWidth > 0) {
				// ����������ʽ
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// ����������ɫ
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				// ���Ʊ߿�
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
			// �Ƿ����
			if (model->GFuncUnit.FillRect)
			{
				// ���������ɫ
				Fl_Color fillColor = fl_rgb_color(model->GFuncUnit.FillColor.R,
												  model->GFuncUnit.FillColor.G,
												  model->GFuncUnit.FillColor.B);
				fl_color(fillColor);
				// ����ڲ�
				fl_pie(model->GFuncUnit.X + model->GFuncUnit.GX - model->GFuncUnit.GW,
					   model->GFuncUnit.Y + model->GFuncUnit.GY - model->GFuncUnit.GH,
					   model->GFuncUnit.GW * 2,
					   model->GFuncUnit.GH * 2, 0, 360);
			}
			// ���Ʊ߿�
			if (model->GFuncUnit.LineWidth > 0) {
				// ����������ʽ
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// ����������ɫ
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// ���Ʊ߿�
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
			// ���Ʊ߿�
			if (model->GFuncUnit.LineWidth > 0) {
				// ����������ʽ
				fl_line_style(0, model->GFuncUnit.LineWidth);
				// ����������ɫ
				Fl_Color lineColor = fl_rgb_color(model->GFuncUnit.LineColor.R,
												  model->GFuncUnit.LineColor.G,
												  model->GFuncUnit.LineColor.B);
				fl_color(lineColor);
				// ���Ʊ߿�
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
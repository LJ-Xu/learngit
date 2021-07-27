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
#include "UIApi.h"
using namespace Project;
namespace UI
{
	GFuncView::GFuncView(int x, int y, int w, int h)
		: HMIBaseView(x, y, w, h), CurType(DCMapDefault){

	}

	GFuncView::~GFuncView() {

	}

	void GFuncView::draw() {
		shared_ptr<GFuncModel> model = BaseView.GetModel<GFuncModel>();
		//shared_ptr<GFuncControl> ctrl = BaseView.GetControl<GFuncControl>();
		int Finx = model->GFuncUnit.X + model->GFuncUnit.OffX;
		int Finy = model->GFuncUnit.Y + model->GFuncUnit.OffY;
		GraphicDrawHandle::PushClip(Finx, Finy, model->GFuncUnit.Width, model->GFuncUnit.Height);
		switch (CurType)
		{
		case DCMapClear:
		{
			fl_color(RGBColor(model->GFuncUnit.BackColor));
			// ���ƻ���������ɫ
			fl_rectf(Finx, Finy, model->GFuncUnit.Width, model->GFuncUnit.Height);
			break;
		}
		case DCMapSetBackColor:
		{
			fl_color(RGBColor(Params[0]));
			// ���ƻ���������ɫ
			fl_rectf(Finx, Finy, model->GFuncUnit.Width, model->GFuncUnit.Height);
			break;
		}
		case DCMapDrawLine:
		{
			fl_line_style(0, Params[4]);
			fl_color(RGBColor(Params[5]));
			// ��������
			fl_line(Finx + Params[0],
				Finy + Params[1],
				Finx + Params[2],
				Finy + Params[3]);
			break;
		}
		case DCMapDrawRect:
		{
			//fillrect
			if (Params[6])
			{
				fl_color(RGBColor(Params[7]));
				fl_rectf(Finx + Params[0],Finy + Params[1], Params[2], Params[3]);
			}
			if (Params[4] > 0) {
				// ����������ʽ
				fl_line_style(0, Params[4]);
				// ����������ɫ
				fl_color(RGBColor(Params[5]));
				// ��������
				fl_rect(Finx + Params[0],Finy + Params[1], Params[2], Params[3]);
			}
			break;
		}
		case DCMapDrawCircle:
		{
			if (Params[5])
			{
				// ���������ɫ
				fl_color(RGBColor(Params[6]));
				// ����ڲ�
				fl_pie(Finx + Params[0], Finy + Params[1],Params[2] * 2,Params[2] * 2, 0, 360);
			}
			// ���Ʊ߿�
			if (Params[3] > 0) {
				// ����������ʽ
				fl_line_style(0, Params[3]);
				// ����������ɫ
				fl_color(RGBColor(Params[4]));
				// ���Ʊ߿�
				fl_arc(Finx + Params[0], Finy + Params[1], Params[2] * 2, Params[2] * 2, 0, 360);
			}
			break;
		}
		case DCMapDrawEllipse:
		{
			// �Ƿ����
			if (Params[6])
			{
				// ���������ɫ
				fl_color(RGBColor(Params[7]));
				// ����ڲ�
				fl_pie(Finx + Params[0], Finy + Params[1],Params[2] * 2,Params[3] * 2, 0, 360);
			}
			// ���Ʊ߿�
			if (Params[4] > 0) {
				// ����������ʽ
				fl_line_style(0, Params[4]);
				// ����������ɫ
				fl_color(RGBColor(Params[5]));
				// ���Ʊ߿�
				fl_arc(Finx + Params[0], Finy + Params[1],Params[2] * 2,Params[3] * 2, 0, 360);
			}
			break;
		}
		case DCMapDrawCircleArc:
		{
			// ���Ʊ߿�
			if (Params[3] > 0) {
				// ����������ʽ
				fl_line_style(0, Params[3]);
				// ����������ɫ
				fl_color(RGBColor(Params[4]));
				// ���Ʊ߿�
				fl_arc(Finx + Params[0], Finy + Params[1],
					Params[2] * 2,
					Params[2] * 2, Params[5], Params[6]);
			}
			break;
		}
		case DCMapDrawEllipseArc:
		{
			if (Params[4] > 0) {
				// ����������ʽ
				fl_line_style(0, Params[4]);
				// ����������ɫ
				fl_color(RGBColor(Params[5]));
				// ���Ʊ߿�
				fl_arc(Finx + Params[0], Finy + Params[1],
					Params[2] * 2,
					Params[3] * 2, Params[6], Params[7]);
			}
			break;
		}
		}
		fl_pop_clip();
		/*shared_ptr<GFuncModel> model = BaseView.GetModel<GFuncModel>();
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
		}*/
	}
	void GFuncView::AddrDrawTask(Project::DrawFunc cmdtype, Project::ParamType * pararms, int pararmcount)
	{
		CurType = cmdtype;
		memset(Params, 0, sizeof(Project::ParamType)*10);
		if (pararms&&pararmcount)
		{
			memcpy(Params, pararms, pararmcount * sizeof(Project::ParamType));
		}
		
	}
}
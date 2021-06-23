/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 * 
 * Name     : VarStringView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ƶ�̬���ֿؼ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "stdafx.h"
#include "VarStringView.h"
#include "UIComm.h"
#include "StringUtility.h"
#include <FL/fl_draw.H>
#include <FL/Fl_Device.H>
namespace UI
{
	VarStringView::VarStringView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	VarStringView::~VarStringView() {}

	void VarStringView::drawLine(vector<Project::Point> &point)
	{
		shared_ptr<VarStringModel> model = BaseView.GetModel<VarStringModel>();
		int startx, starty, endx, endy;
		if (point.size() != 2)
			return;
		startx = point[0].X + model->VarStringConfig.OffX;			//�߶���ʼ����
		starty = point[0].Y + model->VarStringConfig.OffY;
		endx = point[1].X + model->VarStringConfig.OffX;			//�߶��յ�����
		endy = point[1].Y + model->VarStringConfig.OffY;

		//fl_color(RGBColor(model->StringConfig.Line.Color));	    //�����߶���ɫ
		fl_line_style(0, 0);
		fl_line(startx, starty, endx, endy);
	}

	void VarStringView::draw()
	{
		shared_ptr<VarStringModel> model = BaseView.GetModel<VarStringModel>();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,	model->VarStringConfig.StatusStyle.size());		//��ȡ��ǰ״̬

		if ((size_t)CurrentStatus > model->VarStringConfig.StatusStyle.size() || model->VarStringConfig.StatusStyle.size() == 0)
			return;
		/*��ȡ���ڱ߿��λ������*/
		int borderSize = model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Weight;
		int borderX = model->VarStringConfig.X + borderSize / 2 + model->VarStringConfig.OffX;
		int borderY = model->VarStringConfig.Y + borderSize / 2 + model->VarStringConfig.OffY;
		int borderW = model->VarStringConfig.Width - borderSize;
		int borderH = model->VarStringConfig.Height - borderSize;


		/*��ȡ���ڱ߿���������*/
		/*  FL_SOLID	= 0,		///< line style: <tt>___________</tt>
			FL_DASH	= 1,		///< line style: <tt>_ _ _ _ _ _</tt>
			FL_DOT	= 2,		///< line style: <tt>. . . . . .</tt>
			FL_DASHDOT	= 3,		///< line style: <tt>_ . _ . _ .</tt>
			FL_DASHDOTDOT	= 4,*/
		/*���ñ߿���ɫ*/
		Fl_Color bordercolor = fl_rgb_color(RGBColor(model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Color));
		fl_color(active() ? bordercolor : fl_inactive(bordercolor));

		/*���Ʊ߿�*/
		if (model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Weight) {
			fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Type, 
				model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Weight);
			fl_begin_loop();
			fl_vertex(borderX, borderY);
			fl_vertex(borderX + borderW, borderY);
			fl_vertex(borderX + borderW, borderY + borderH);
			fl_vertex(borderX, borderY + borderH);
			fl_vertex(borderX, borderY + borderH);
			fl_end_loop();
		}

		/*��ȡ�������������*/
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->VarStringConfig.StatusStyle[CurrentStatus].Text, 0);
		UI::IResourceService::GB2312toUtf8(text);

		/*����������ʽ����С*/
		fl_font(UI::IResourceService::GetFontIdx(model->VarStringConfig.StatusStyle[CurrentStatus].Css.Font.Name) , 
			model->VarStringConfig.StatusStyle[CurrentStatus].Css.Font.Size);
				
		/*����������ɫ*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->VarStringConfig.StatusStyle[CurrentStatus].Css.Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));
		/*�����ı�*/
		fl_draw(text.c_str(), borderX + borderSize / 2, borderY + borderSize / 2, borderW - borderSize,
			borderH - borderSize, model->VarStringConfig.StatusStyle[CurrentStatus].Css.Align);	//FL_ALIGN_CENTER

		if (model->VarStringConfig.StatusStyle[CurrentStatus].Css.UnderlineUsed)
			drawLine(model->VarStringConfig.StatusStyle[CurrentStatus].Css.UnderPoint);
		if (model->VarStringConfig.StatusStyle[CurrentStatus].Css.StrikeoutUsed)
			drawLine(model->VarStringConfig.StatusStyle[CurrentStatus].Css.StrikeoutPoint);
	}

}

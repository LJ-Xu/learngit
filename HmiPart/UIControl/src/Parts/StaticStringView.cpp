/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : StaticStringView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : ���ƾ�̬���ֿؼ�
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "stdafx.h"
#include "StaticStringView.h"
#include "UIComm.h"
#include "StringUtility.h"
#include "FL/fl_draw.H"
namespace UI
{
	StaticStringView::StaticStringView(int X, int Y, int W, int H) : HMIBaseView(X, Y, W, H) {}
	StaticStringView::~StaticStringView() {}


	void StaticStringView::drawUnderline()
	{
		shared_ptr<StaticStringModel> model = BaseView.GetModel<StaticStringModel>();
		int startx, starty, endx, endy;
		if (model->StringConfig.Css.UnderPoint.size() != 2)
			return;
	
		startx = model->StringConfig.Css.UnderPoint[0].X + model->StringConfig.OffX;        //�߶���ʼ����
		starty = model->StringConfig.Css.UnderPoint[0].Y + model->StringConfig.OffY;
		endx = model->StringConfig.Css.UnderPoint[1].X + model->StringConfig.OffX;        //�߶��յ�����
		endy = model->StringConfig.Css.UnderPoint[1].Y + model->StringConfig.OffY;

		//fl_color(RGBColor(model->StringConfig.Line.Color));        //�����߶���ɫ
		fl_line_style(0, 0);
		fl_line(startx, starty, endx, endy);
	}
	void StaticStringView::drawStrikeout()
	{
		shared_ptr<StaticStringModel> model = BaseView.GetModel<StaticStringModel>();
		int startx, starty, endx, endy;
		if (model->StringConfig.Css.StrikeoutPoint.size() != 2)
			return;
	
		startx = model->StringConfig.Css.StrikeoutPoint[0].X + model->StringConfig.OffX;        //�߶���ʼ����
		starty = model->StringConfig.Css.StrikeoutPoint[0].Y + model->StringConfig.OffY;
		endx = model->StringConfig.Css.StrikeoutPoint[1].X + model->StringConfig.OffX;        //�߶��յ�����
		endy = model->StringConfig.Css.StrikeoutPoint[1].Y + model->StringConfig.OffY;

		//fl_color(RGBColor(model->StringConfig.Line.Color));        //�����߶���ɫ
		fl_line_style(0, 0);
		fl_line(startx, starty, endx, endy);
	}
	void StaticStringView::draw()
	{
		shared_ptr<StaticStringControl> ctrl = BaseView.GetControl<StaticStringControl>();
		shared_ptr<StaticStringModel> model = BaseView.GetModel<StaticStringModel>();

		/*��ȡ���ڱ߿������*/
		int borderSize = model->StringConfig.Border.BorderSize;
		int borderX = model->StringConfig.X + borderSize / 2 + model->StringConfig.OffX;
		int borderY = model->StringConfig.Y + borderSize / 2 + model->StringConfig.OffY;
		int borderW = model->StringConfig.Width - borderSize;
		int borderH = model->StringConfig.Height - borderSize;
		/*
			FL_SOLID	= 0,		///< line style: <tt>___________</tt>
			FL_DASH	= 1,		///< line style: <tt>_ _ _ _ _ _</tt>
			FL_DOT	= 2,		///< line style: <tt>. . . . . .</tt>
			FL_DASHDOT	= 3,		///< line style: <tt>_ . _ . _ .</tt>
			FL_DASHDOTDOT	= 4,
		*/
		int borderstyle = model->StringConfig.Border.BorderStyle;
		/*��ȡ���ñ߿���ɫ*/
		Fl_Color bordercolor = fl_rgb_color(RGBColor(model->StringConfig.Border.BorderColor));
		fl_color(active() ? bordercolor : fl_inactive(bordercolor));
		/*���Ʊ߿�*/
		if (borderSize) {
			fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | borderstyle, borderSize);
			fl_begin_loop();
			fl_vertex(borderX, borderY);
			fl_vertex(borderX + borderW, borderY);
			fl_vertex(borderX + borderW, borderY + borderH);
			fl_vertex(borderX, borderY + borderH);
			fl_vertex(borderX, borderY + borderH);
			fl_end_loop();
			//fl_rect(borderX, borderY, borderW, borderH);
		}

		/*��ȡ�����ַ���*/
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->StringConfig.Text, 0);
		UI::IResourceService::GB2312toUtf8(text);
				
		/*��������ʹ�С*/
		int fontSize = model->StringConfig.Css.Font.Size;
		int fontStyle = UI::IResourceService::GetFontIdx(model->StringConfig.Css.Font.Name);
		printf("StaticString font style = %d, size = %d\n", fontStyle, fontSize);
		fl_font(fontStyle, fontSize);
		/*����������ɫ*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->StringConfig.Css.Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));

		/*�����ı�*/
		fl_draw(text.data(), borderX + borderSize / 2, borderY + borderSize / 2, borderW - borderSize, borderH - borderSize,
				model->StringConfig.Css.Align);	//FL_ALIGN_CENTER

		/*�����»���*/
		if (model->StringConfig.Css.UnderlineUsed)
			drawUnderline();
		/*����ɾ����*/
		if (model->StringConfig.Css.StrikeoutUsed)
			drawStrikeout();
	}
}

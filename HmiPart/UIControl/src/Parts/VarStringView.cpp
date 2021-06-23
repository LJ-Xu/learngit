/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 * 
 * Name     : VarStringView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制动态文字控件
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
		startx = point[0].X + model->VarStringConfig.OffX;			//线段起始坐标
		starty = point[0].Y + model->VarStringConfig.OffY;
		endx = point[1].X + model->VarStringConfig.OffX;			//线段终点坐标
		endy = point[1].Y + model->VarStringConfig.OffY;

		//fl_color(RGBColor(model->StringConfig.Line.Color));	    //设置线段颜色
		fl_line_style(0, 0);
		fl_line(startx, starty, endx, endy);
	}

	void VarStringView::draw()
	{
		shared_ptr<VarStringModel> model = BaseView.GetModel<VarStringModel>();
		IResourceService::Ins()->SetRenderStatus(CurrentStatus,	model->VarStringConfig.StatusStyle.size());		//获取当前状态

		if ((size_t)CurrentStatus > model->VarStringConfig.StatusStyle.size() || model->VarStringConfig.StatusStyle.size() == 0)
			return;
		/*获取关于边框的位置数据*/
		int borderSize = model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Weight;
		int borderX = model->VarStringConfig.X + borderSize / 2 + model->VarStringConfig.OffX;
		int borderY = model->VarStringConfig.Y + borderSize / 2 + model->VarStringConfig.OffY;
		int borderW = model->VarStringConfig.Width - borderSize;
		int borderH = model->VarStringConfig.Height - borderSize;


		/*获取关于边框属性数据*/
		/*  FL_SOLID	= 0,		///< line style: <tt>___________</tt>
			FL_DASH	= 1,		///< line style: <tt>_ _ _ _ _ _</tt>
			FL_DOT	= 2,		///< line style: <tt>. . . . . .</tt>
			FL_DASHDOT	= 3,		///< line style: <tt>_ . _ . _ .</tt>
			FL_DASHDOTDOT	= 4,*/
		/*设置边框颜色*/
		Fl_Color bordercolor = fl_rgb_color(RGBColor(model->VarStringConfig.StatusStyle[CurrentStatus].BorderStyle.Color));
		fl_color(active() ? bordercolor : fl_inactive(bordercolor));

		/*绘制边框*/
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

		/*获取关于字体的数据*/
		string text = StringUtility::GetDrawString(IResourceService::Ins(),
			model->VarStringConfig.StatusStyle[CurrentStatus].Text, 0);
		UI::IResourceService::GB2312toUtf8(text);

		/*设置字体样式及大小*/
		fl_font(UI::IResourceService::GetFontIdx(model->VarStringConfig.StatusStyle[CurrentStatus].Css.Font.Name) , 
			model->VarStringConfig.StatusStyle[CurrentStatus].Css.Font.Size);
				
		/*设置字体颜色*/
		Fl_Color textcolor = fl_rgb_color(RGBColor(model->VarStringConfig.StatusStyle[CurrentStatus].Css.Colors));
		fl_color(active() ? textcolor : fl_inactive(textcolor));
		/*绘制文本*/
		fl_draw(text.c_str(), borderX + borderSize / 2, borderY + borderSize / 2, borderW - borderSize,
			borderH - borderSize, model->VarStringConfig.StatusStyle[CurrentStatus].Css.Align);	//FL_ALIGN_CENTER

		if (model->VarStringConfig.StatusStyle[CurrentStatus].Css.UnderlineUsed)
			drawLine(model->VarStringConfig.StatusStyle[CurrentStatus].Css.UnderPoint);
		if (model->VarStringConfig.StatusStyle[CurrentStatus].Css.StrikeoutUsed)
			drawLine(model->VarStringConfig.StatusStyle[CurrentStatus].Css.StrikeoutPoint);
	}

}

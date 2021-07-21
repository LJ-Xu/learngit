/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableView.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 绘制表格
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "stdafx.h"
#include "TableView.h"
#include "FL/fl_draw.H"

namespace UI
{
	TableView::TableView(int X, int Y, int W, int H, const char *l = 0) : RowTable(X, Y, W, H, l) {
		end();
	}
	TableView::~TableView() {}

	void TableView::InitDraw()
	{
		shared_ptr<TableModel> model = BaseView.GetModel<TableModel>();

		//设置是否使用行、列标题
		col_header(0);
		row_header(0);

		//设置是否可变宽高
		row_resize(0);
		col_resize(0);

		//设置列数
		cols(model->TableConfig.Cols);
		//设置行数
		rows(model->TableConfig.Rows);
		//设置行高
		for (unsigned i = 0; i < model->TableConfig.RowHeight.size(); i++)
			row_height(i,model->TableConfig.RowHeight[i]);
		//设置列宽
		for (unsigned i = 0; i < model->TableConfig.ColWidth.size(); i++)
			col_width(i,model->TableConfig.ColWidth[i]);

		//设置背景颜色
		if(model->TableConfig.UseFillin)
			bgcolor_ = fl_rgb_color(RGBColor(model->TableConfig.FillColor));
		if (model->TableConfig.HaveColBorder)
			colcolor_ = fl_rgb_color(RGBColor(model->TableConfig.InnerBorderCol.Color));
		if (model->TableConfig.HaveRowBorder)
			rowcolor_ = fl_rgb_color(RGBColor(model->TableConfig.InnerBorderRow.Color));
		bordercolor_ = fl_rgb_color(RGBColor(model->TableConfig.OuterBorder.Color));
		box(FL_NO_BOX);
		//model->TableConfig.OuterBorder.Weight *= 2;
		vscrollbar->hide();
		vscrollbar->size(0,0);
		hscrollbar->hide();
		hscrollbar->size(0, 0);

	}

	void TableView::draw_cell(TableContext context,
		int R, int C, int X, int Y, int W, int H)
	{
		shared_ptr<TableModel> model = BaseView.GetModel<TableModel>();
		switch (context)
		{
		case CONTEXT_STARTPAGE:
			return;
		case CONTEXT_COL_HEADER:				//绘制列标题
			return;
		case CONTEXT_ROW_HEADER:
			return;
		case CONTEXT_CELL:
		{
			GraphicDrawHandle::PushClip(X, Y, W, H);
			{
				// BG COLOR
				if (model->TableConfig.UseFillin)
				{
					fl_color(active() ? bgcolor_ : fl_inactive(bgcolor_));
					fl_rectf(X - 2, Y - 2, W + 6, H + 6);
				}
			}
			fl_pop_clip();
			return;
		}
		case CONTEXT_TABLE:
			fprintf(stderr, "TABLE CONTEXT CALLED\n");
			return;
		case CONTEXT_ENDPAGE:		//画边框
		{
			GraphicDrawHandle::PushClip(model->TableConfig.X + model->TableConfig.OffX,
				model->TableConfig.Y + model->TableConfig.OffY,
				model->TableConfig.Width + 5, model->TableConfig.Height + 5);
			{
				if (model->TableConfig.HaveRowBorder)		//使用行分割线
				{
					fl_color(active() ? rowcolor_ : fl_inactive(rowcolor_));
					fl_line_style(model->TableConfig.InnerBorderRow.Type, model->TableConfig.InnerBorderRow.Weight);

					for (unsigned i = 0, rowY = 0; i < model->TableConfig.RowHeight.size() - 1; i++)
					{
						rowY += model->TableConfig.RowHeight[i];
						fl_line(model->TableConfig.X + model->TableConfig.OffX,
							model->TableConfig.Y + model->TableConfig.OffY + rowY,
							model->TableConfig.X + model->TableConfig.OffX + model->TableConfig.Width,
							model->TableConfig.Y + model->TableConfig.OffY + rowY);

					}
				}
				if (model->TableConfig.HaveColBorder)		//使用列分割线
				{
					fl_color(active() ? colcolor_ : fl_inactive(colcolor_));
					fl_line_style(model->TableConfig.InnerBorderCol.Type, model->TableConfig.InnerBorderCol.Weight);

					for (unsigned i = 0, rowX = 0; i < model->TableConfig.ColWidth.size() - 1; i++) 
					{
						rowX += model->TableConfig.ColWidth[i];
						fl_line(model->TableConfig.X + model->TableConfig.OffX + rowX,
							model->TableConfig.Y + model->TableConfig.OffY,
							model->TableConfig.X + model->TableConfig.OffX + rowX,
							model->TableConfig.Y + model->TableConfig.OffY + model->TableConfig.Height);
					}
				}
			}
			fl_pop_clip();
			//画外边框
			fl_color(active() ? bordercolor_ : fl_inactive(bordercolor_));
			fl_line_style(FL_CAP_SQUARE | FL_JOIN_MITER | model->TableConfig.OuterBorder.Type, model->TableConfig.OuterBorder.Weight);

			fl_begin_line();
			fl_vertex((double)model->TableConfig.X + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX,
				(double)model->TableConfig.Y + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY);
			fl_vertex((double)model->TableConfig.X - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX + (double)model->TableConfig.Width,
				(double)model->TableConfig.Y + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY);
			fl_end_line();
			
			fl_begin_line();
			fl_vertex((double)model->TableConfig.X + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX,
				(double)model->TableConfig.Y - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY + (double)model->TableConfig.Height);
			fl_vertex((double)model->TableConfig.X - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX + (double)model->TableConfig.Width,
				(double)model->TableConfig.Y - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY + (double)model->TableConfig.Height);
			fl_end_line();

			fl_begin_line();
			fl_vertex((double)model->TableConfig.X + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX,
				(double)model->TableConfig.Y + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY);
			fl_vertex((double)model->TableConfig.X + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX,
				(double)model->TableConfig.Y - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY + (double)model->TableConfig.Height);
			fl_end_line();

			fl_begin_line();
			fl_vertex((double)model->TableConfig.X - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX + (double)model->TableConfig.Width,
				(double)model->TableConfig.Y + (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY);
			fl_vertex((double)model->TableConfig.X - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffX + (double)model->TableConfig.Width,
				(double)model->TableConfig.Y - (double)model->TableConfig.OuterBorder.Weight / 2.0 + (double)model->TableConfig.OffY + (double)model->TableConfig.Height);
			fl_end_line();
			//fl_rect(model->TableConfig.X + model->TableConfig.OuterBorder.Weight / 2 + model->TableConfig.OffX,
			//	model->TableConfig.Y + model->TableConfig.OuterBorder.Weight / 2 + model->TableConfig.OffY,
			//	model->TableConfig.Width - model->TableConfig.OuterBorder.Weight, 
			//	model->TableConfig.Height - model->TableConfig.OuterBorder.Weight);
			return;
		}
		case CONTEXT_RC_RESIZE:
		case CONTEXT_NONE:
			return;
		}
	}
	int TableView::handle(int event)
	{
		return 0;
	}
}
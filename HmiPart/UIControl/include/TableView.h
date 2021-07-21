/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableView.h
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 关于表格的绘制头文件，绘制表格控件
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "RowTable.h"
#include "BasicView.h"
#include <stdafx.h>
#include <FL/Fl_Table_Row.H>
#include "TableModel.h"
#include "TableControl.h"
namespace UI
{
	class TableView : public RowTable
	{
	public:
		TableView(int X, int Y, int W, int H, const char *l);
		~TableView();
		void InitDraw(void);
		BasicView BaseView;
	protected:
		void draw_cell(TableContext context,  		// table cell drawing
			int R = 0, int C = 0, int X = 0, int Y = 0, int W = 0, int H = 0);
		int handle(int event);

	private:
		Fl_Color bgcolor_;			//列表背景颜色
		Fl_Color bordercolor_;			//列表序号文字颜色
		Fl_Color rowcolor_;			//列表日期文字颜色
		Fl_Color colcolor_;			//列表时间文字颜色
	};
}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : TableConfig.h
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 用于存放表格控件相关参数的类
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "MoveableDisplayUnit.h"
#include "BaseLine.h"
namespace Project
{
	class TableConfig :public MoveableDisplayUnit
	{
	public:
		int Rows;					//行数
		int Cols;					//列数
		BaseLine OuterBorder;		//外框样式
		bool HaveRowBorder;			//是否显示行网格线
		BaseLine InnerBorderRow;	//行网格线样式
		bool HaveColBorder;			//是否显示列网格线
		BaseLine InnerBorderCol;	//列网格线样式
		bool UseFillin;				//是否使用填充颜色
		int FillColor;				//填充颜色

		vector<int> ColWidth;		//列宽
		vector<int> RowHeight;		//行高

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(Rows, Cols, OuterBorder, HaveRowBorder, InnerBorderRow,
				HaveColBorder, InnerBorderCol, UseFillin, FillColor, ColWidth,
				RowHeight, VOffX, VOffY, Perm, Width, Height, X, Y, IsEnable, IsRecord, CtrlName);
		}
	};

}


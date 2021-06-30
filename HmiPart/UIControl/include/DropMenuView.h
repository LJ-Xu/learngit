/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuView.h
 * Author   : qiaodan
 * Date     : 2020-12-10
 * Descript : 关于下拉菜单的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include "DropMenuUnit.h"
#include <stdafx.h>
#include "DropMenuItem.h"
#include "BasicView.h"
#include "PermUtility.h"
#include <FL/Fl_Scrollbar.H>
namespace UI
{
	class DropMenuView :public DropMenuUnit
	{
	public:
		BasicView BaseView;
		enum popup_buttons { POPUP1 = 1, POPUP2, POPUP12, POPUP3, POPUP13, POPUP23, POPUP123 };
		int handle(int);
		const DropMenuItem* popup();
		DropMenuView(int, int, int, int, const char * = 0);
		void InitDraw();
		void show();
		void hide();
		bool HandleOperatePerm();
		bool HandleOperateConfirm();
		void SetWriteData();

		int CurrentIndex = 0;	//当前显示的下拉条目索引
		int StartNum;
		int SelectNum = 0;
		Fl_Scrollbar* MenuScrollbar;		//滚动条
		int MenuScrollvalue;
		int Rowheight;
		int MenuNum;
		DropMenuItem *SelectMenu;
	protected:
		void draw();
	private:
		int labelmaxsize_ = 0;
		int labelmaxfont_ = 0;
		Fl_Color bgcolor_;
		int labelalign_;
		int labelcolor_;
		int labelfont_;
		int labelsize_;
		string labeltext_;
	};
}


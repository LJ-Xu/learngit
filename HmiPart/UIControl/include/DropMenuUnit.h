#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuUnit.h
 * Author   : qiaodan
 * Date     : 2020-12-10
 * Descript : 关于下拉菜单的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#pragma once
#include "BasicView.h"
#include "DropMenuItem.h"
#include <stdafx.h>
#include <FL/Fl_Widget.H>
#include "ImageResId.h"
namespace UI
{
	size_t MenuStrlcat(char *dst, const char *src, size_t size);
	
	class DropMenuUnit :public HMIBaseView
	{
	protected:

		uchar alloc;			
		uchar down_box_;
		Fl_Font textfont_;
		Fl_Fontsize textsize_;
		Fl_Color textcolor_;
		int item_pathname_(char *name, int namelen, const DropMenuItem *finditem,
			DropMenuItem *menu = 0) ;
	public:
		DropMenuUnit(int, int, int, int, const char * = 0);
		~DropMenuUnit();

		DropMenuItem *menu_;
		const DropMenuItem *value_;
		//Project::ImageResId PicKey;
		Fl_Color FrameColor;

		int item_pathname(char *name, int namelen, const DropMenuItem *finditem = 0);
		const DropMenuItem* picked(const DropMenuItem*);
		const DropMenuItem* find_item(const char *name);
		const DropMenuItem* find_item(Fl_Callback*);
		int find_index(const char *name) const;
		int find_index(const DropMenuItem *item) const;
		//int find_index(Fl_Callback *cb) const;

		//const DropMenuItem* test_shortcut() { return picked(menu()->test_shortcut()); }
		
		DropMenuItem *menu() { return menu_; }
		void menu(const DropMenuItem *m);
		void copy(const DropMenuItem *m, void* user_data = 0);
		int insert(int index, const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
		int  add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0); // see src/Fl_Menu_add.cxx
		int  add(const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) {
			return add(a, fl_old_shortcut(b), c, d, e);
		}
		int insert(int index, const char* a, const char* b, Fl_Callback* c, void* d = 0, int e = 0) {
			return insert(index, a, fl_old_shortcut(b), c, d, e);
		}
		int  add(const char *);
		int  size() const;
		void size(int W, int H) { Fl_Widget::size(W, H); }
		void clear();
		//int clear_submenu(int index);
		//void replace(int, const char *);
		//void remove(int);
		//void shortcut(int i, int s) { menu_[i].shortcut(s); }
		void mode(int i, int fl) { menu_[i].flags = fl; }
		int  mode(int i) const { return menu_[i].flags; }
		const DropMenuItem *mvalue() const { return value_; }
		int value() const { return value_ ? (int)(value_ - menu_) : -1; }
		int value(const DropMenuItem*);
		int value(int i) { return value(menu_ + i); }
		const char *text() const { return value_ ? value_->text.c_str() : 0; }
		const char *text(int i) const { return menu_[i].text.c_str(); }

		Fl_Font textfont() const { return textfont_; }
		void textfont(Fl_Font c) { textfont_ = c; }
		Fl_Fontsize textsize() const { return textsize_; }
		void textsize(Fl_Fontsize c) { textsize_ = c; }
		Fl_Color textcolor() const { return textcolor_; }
		void textcolor(Fl_Color c) { textcolor_ = c; }
		Fl_Boxtype down_box() const { return (Fl_Boxtype)down_box_; }
		void down_box(Fl_Boxtype b) { down_box_ = b; }
		Fl_Color down_color() const { return selection_color(); }
		void down_color(unsigned c) { selection_color(c); }
		//void setonly(DropMenuItem* item);
	};
}


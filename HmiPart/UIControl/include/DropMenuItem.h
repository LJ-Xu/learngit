#pragma once
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuItem.h
 * Author   : qiaodan
 * Date     : 2020-12-10
 * Descript : 关于下拉菜单的绘制头文件，绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "BasicView.h"
#include <FL/Fl_Image.H>
#include <stdafx.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Scrollbar.H>
#include <stdio.h>
#include <string.h>
extern FL_EXPORT Fl_Shortcut fl_old_shortcut(const char*);

namespace UI
{
	enum { // values for flags:
		FL_MENU_INACTIVE = 1,		///< Deactivate menu item (gray out)
		FL_MENU_TOGGLE = 2,		///< Item is a checkbox toggle (shows checkbox for on/off state)
		FL_MENU_VALUE = 4,		///< The on/off state for checkbox/radio buttons (if set, state is 'on')
		FL_MENU_RADIO = 8,		///< Item is a radio button (one checkbox of many can be on)
		FL_MENU_INVISIBLE = 0x10,	///< Item will not show up (shortcut will work)
		FL_SUBMENU_POINTER = 0x20,	///< Indicates user_data() is a pointer to another menu array
		FL_SUBMENU = 0x40,		///< This item is a submenu to other items
		FL_MENU_DIVIDER = 0x80,	///< Creates divider line below this item. Also ends a group of radio buttons.
		FL_MENU_HORIZONTAL = 0x100	///< ??? -- reserved
	};

	class DropMenuUnit;
	struct DropMenuWidow;
	struct DropMenuState;
	struct DropMenuItem
	{
		string text;	    ///< menu item text, returned by label()
		int shortcut_;	    ///< menu item shortcut
		Fl_Callback *callback_;   ///< menu item callback
		void *user_data_;	    ///< menu item user_data for the menu's callback
		int flags;		    ///< menu item flags like FL_MENU_TOGGLE, FL_MENU_RADIO
		uchar labeltype_;	    ///< how the menu item text looks like
		Fl_Font labelfont_;	    ///< which font for this menu item text
		Fl_Fontsize labelsize_;   ///< size of menu item text
		Fl_Color labelcolor_;	    ///< menu item text color
		Fl_Align labelalign_;		//menu text align
		DropMenuState *MenuState = nullptr;
		const DropMenuItem *next(int = 1) const;
		DropMenuItem *next(int i = 1) {
			return (DropMenuItem*)(((const DropMenuItem*)this)->next(i));
		}

		const DropMenuItem *first() const { return next(0); }
		DropMenuItem *first() { return next(0); }
		const char* label() const { return text.c_str(); }

		void label(const char* a) { text = a; }
		//void label(Fl_Labeltype a, const char* b) { labeltype_ = a; text = b; }

		Fl_Labeltype labeltype() const { return (Fl_Labeltype)labeltype_; }
		void labeltype(Fl_Labeltype a) { labeltype_ = a; }

		Fl_Color labelcolor() const { return labelcolor_; }
		void labelcolor(Fl_Color a) { labelcolor_ = a; }
		Fl_Font labelfont() const { return labelfont_; }
		void labelfont(Fl_Font a) { labelfont_ = a; }
		Fl_Fontsize labelsize() const { return labelsize_; }
		void labelsize(Fl_Fontsize a) { labelsize_ = a; }
		Fl_Align labelalign() const { return labelalign_; }
		void labelalign(Fl_Align a) { labelalign_ = a; }

		Fl_Callback_p callback() const { return callback_; }
		void callback(Fl_Callback* c, void* p) { callback_ = c; user_data_ = p; }
		
		void callback(Fl_Callback* c) { callback_ = c; }

		void callback(Fl_Callback0*c) { callback_ = (Fl_Callback*)c; }

		
		void callback(Fl_Callback1*c, long p = 0) { callback_ = (Fl_Callback*)c; user_data_ = (void*)(fl_intptr_t)p; }
		void* user_data() const { return user_data_; }
		void user_data(void* v) { user_data_ = v; }
		long argument() const { return (long)(fl_intptr_t)user_data_; }

		void argument(long v) { user_data_ = (void*)(fl_intptr_t)v; }
		int shortcut() const { return shortcut_; }
		void shortcut(int s) { shortcut_ = s; }
		
		int submenu() const { return flags & (FL_SUBMENU | FL_SUBMENU_POINTER); }
		
		int checkbox() const { return flags & FL_MENU_TOGGLE; }
		
		int radio() const { return flags & FL_MENU_RADIO; }
		
		int value() const { return flags & FL_MENU_VALUE; }

		void set() { flags |= FL_MENU_VALUE; }
		void clear() { flags &= ~FL_MENU_VALUE; }

		int visible() const { return !(flags&FL_MENU_INVISIBLE); }
		void show() { flags &= ~FL_MENU_INVISIBLE; }

		void hide() { flags |= FL_MENU_INVISIBLE; }
		int active() const { return !(flags&FL_MENU_INACTIVE); }
		void activate() { flags &= ~FL_MENU_INACTIVE; }
		void deactivate() { flags |= FL_MENU_INACTIVE; }
		int activevisible() const { return !(flags & (FL_MENU_INACTIVE | FL_MENU_INVISIBLE)); }
		void image(Fl_Image* a) {  }

		void image(Fl_Image& a) {  }
		int measure(int* h, const DropMenuUnit*) const;
		void draw(int x, int y, int w, int h, const DropMenuUnit*, int t = 0) const;
		//const DropMenuItem* popup(
		//	int X, int Y,
		//	const char *title = 0,
		//	const DropMenuItem* picked = 0,
		//	const DropMenuUnit* = 0) const;
		const DropMenuItem* pulldown(
			int X, int Y, int W, int H, int winheigth, int hscollwidth,
			const DropMenuItem* picked = 0,
			const DropMenuUnit* = 0) ;
		//const DropMenuItem* test_shortcut() const;
		void do_callback(Fl_Widget* o) const { callback_(o, user_data_); }
		void do_callback(Fl_Widget* o, void* arg) const { callback_(o, arg); }
		void do_callback(Fl_Widget* o, long arg) const { callback_(o, (void*)(fl_intptr_t)arg); }
		int checked() const { return flags & FL_MENU_VALUE; }
		void check() { flags |= FL_MENU_VALUE; }
		void uncheck() { flags &= ~FL_MENU_VALUE; }
		int insert(int, const char*, int, Fl_Callback*, void* = 0, int = 0);
		int add(const char*, int shortcut, Fl_Callback*, void* = 0, int = 0);
		int add(const char*a, const char* b, Fl_Callback* c,
			void* d = 0, int e = 0) {
			return add(a, fl_old_shortcut(b), c, d, e);
		}
		int size() const;
	};
	
	struct DropMenuWidow : public Fl_Window {
		void draw();
		void drawentry(const DropMenuItem*, int i, int erase);
	public:
		int Startnum = 0;
		int RealH;			//下拉框高度
		int AllMenuH;		//所有下拉条目高度

		int itemheight;		//下拉条目高度
		int numitems;		//下拉总条目个数
		Fl_Scrollbar* hscrollbar;

		int handle(int);
		int selected;
		int drawn_selected;	// last redraw has this selected
		int shortcutWidth;
		const DropMenuItem* menu;
		DropMenuWidow(DropMenuItem* m, int X, int Y, int W, int H, int realHeight,
			const DropMenuItem* picked);
		~DropMenuWidow();
#ifndef WIN32
		int early_hide_handle(int);
#endif
		void set_selected(int);
		int find_selected(int mx, int my);
		int titlex(int);
		void autoscroll(int);
		void position(int x, int y);
		int is_inside(int x, int y);
	};

	struct DropMenuState {
		const DropMenuItem* current_item; // what mouse is pointing at
		int menu_number; // which menu it is in
		int item_number; // which item in that menu, -1 if none
		DropMenuWidow* p[20]; // pointers to menus
		int nummenus;
		int menubar; // if true p[0] is a menubar
		int state;
		DropMenuWidow* fakemenu; // kludge for buttons in menubar
		int is_inside(int mx, int my);
	};

	typedef DropMenuItem Fl_Menu; // back compatibility

	enum {	
		FL_PUP_NONE = 0,
		FL_PUP_GREY = FL_MENU_INACTIVE,
		FL_PUP_GRAY = FL_MENU_INACTIVE,
		FL_MENU_BOX = FL_MENU_TOGGLE,
		FL_PUP_BOX = FL_MENU_TOGGLE,
		FL_MENU_CHECK = FL_MENU_VALUE,
		FL_PUP_CHECK = FL_MENU_VALUE,
		FL_PUP_RADIO = FL_MENU_RADIO,
		FL_PUP_INVISIBLE = FL_MENU_INVISIBLE,
		FL_PUP_SUBMENU = FL_SUBMENU_POINTER
	};
}



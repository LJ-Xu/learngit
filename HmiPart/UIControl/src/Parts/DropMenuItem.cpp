#include "DropMenuItem.h"
#include "DropMenuUnit.h"
#include <FL/Fl.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <FL/Fl_Scrollbar.H>
#include "IResourceService.h"
extern char fl_draw_shortcut;

namespace UI
{
	int DropMenuItem::size() const {
		const DropMenuItem* m = this;
		int nest = 0;
		for (;;) {
			if (m->text.empty()) {
				if (!nest) return (int)(m - this + 1);
				nest--;
			}
			else if (m->flags & FL_SUBMENU) {
				nest++;
			}
			m++;
		}
	}
	static const DropMenuItem* next_visible_or_not(const DropMenuItem* m) {
		int nest = 0;
		do {
			if (m->text.empty()) {
				if (!nest) return m;
				nest--;
			}
			else if (m->flags&FL_SUBMENU) {
				nest++;
			}
			m++;
		} while (nest);
		return m;
	}
	const DropMenuItem* DropMenuItem::next(int n) const {
		if (n < 0) return 0; // this is so selected==-1 returns NULL
		const DropMenuItem* m = this;
		if (!m->visible()) n++;
		while (n) {
			m = next_visible_or_not(m);
			if (m->visible() || m->text.empty()) n--;
		}
		return m;
	}
	static const DropMenuUnit* button = 0;



#define LEADING 4 // extra vertical leading

	int DropMenuItem::measure(int* hp, const DropMenuUnit* m) const {
		Fl_Label l;
		l.value = text.c_str();
		l.image = 0;
		l.deimage = 0;
		l.type = labeltype_;
		l.font = labelsize_ || labelfont_ ? labelfont_ : (m ? m->textfont() : FL_HELVETICA);
		l.size = labelsize_ ? labelsize_ : m ? m->textsize() : FL_NORMAL_SIZE;
		l.color = FL_FOREGROUND_COLOR; // this makes no difference?
		fl_draw_shortcut = 1;
		int w = 0; int h = 0;
		l.measure(w, hp ? *hp : h); 
		fl_draw_shortcut = 0;
		if (flags & (FL_MENU_TOGGLE | FL_MENU_RADIO)) w += FL_NORMAL_SIZE;
		return w;
	}
	void DropMenuItem::draw(int x, int y, int w, int h, const DropMenuUnit* m,
		int selected) const {
		//Fl_Label l;
		//l.value = text.c_str();
		//l.image = 0;
		//l.deimage = 0;
		//l.type = labeltype_;

		//l.font = labelsize_ || labelfont_ ? labelfont_ : (m ? m->textfont() : FL_HELVETICA);
		//l.size = labelsize_ ? labelsize_ : m ? m->textsize() : FL_NORMAL_SIZE;
		//l.color = labelcolor_ ? labelcolor_ : m ? m->textcolor() : int(FL_FOREGROUND_COLOR);
		//if (!active()) l.color = fl_inactive((Fl_Color)l.color);
		Fl_Color color = m ? m->color() : FL_GRAY;
		if (selected) {
			Fl_Color r = m ? m->selection_color() : FL_SELECTION_COLOR;
			Fl_Boxtype b = m && m->down_box() ? m->down_box() : FL_FLAT_BOX;
			fl_draw_box(b, x + 1, y - (LEADING - 2) / 2, w - 2, h + (LEADING - 2), r);
		}

		//if (!fl_draw_shortcut) fl_draw_shortcut = 1;
		//l.draw(x + 3, y, w > 6 ? w - 6 : 0, h, labelalign());
		//fl_draw_shortcut = 0;

		fl_font(labelsize_ || labelfont_ ? labelfont_ : (m ? m->textfont() : FL_HELVETICA),
			labelsize_ ? labelsize_ : m ? m->textsize() : FL_NORMAL_SIZE);
		Fl_Color textcolor = labelcolor_ ? labelcolor_ : m ? m->textcolor() : int(FL_FOREGROUND_COLOR);
		if (!active()) textcolor = fl_inactive((Fl_Color)textcolor);
		fl_color(textcolor);
		if (!fl_draw_shortcut) fl_draw_shortcut = 1;
		fl_draw(text.data(), x + 3, y, w > 6 ? w - 6 : 0, h, labelalign());	//FL_ALIGN_CENTER
		fl_draw_shortcut = 0;
	}
	
	DropMenuWidow::DropMenuWidow(DropMenuItem* m, int X, int Y, int Wp, int Hp,
		int realHeight, const DropMenuItem* picked)	: Fl_Menu_Window(X, Y, Wp, Hp, 0)
	{
		int scr_x, scr_y, scr_w, scr_h;
		int tx = X, ty = Y;
		Fl::screen_work_area(scr_x, scr_y, scr_w, scr_h);
		end();
		set_modal();
		clear_border();
		set_menu_window();
		menu = m;
		if (m) m = m->first(); // find the first item that needs to be rendered
		drawn_selected = -1;
		if (button) {
			box(button->box());
			if (box() == FL_NO_BOX || box() == FL_FLAT_BOX) box(FL_UP_BOX);
		}
		else {
			box(FL_UP_BOX);
		}
		color(button && !Fl::scheme() ? button->color() : FL_GRAY);
		selected = -1;
		{
			int j = 0;
			if (m) for (const DropMenuItem* m1 = m; ; m1 = m1->next(), j++) {
				if (picked) {
					if (m1 == picked) { selected = j; picked = 0; }
					else if (m1 > picked) { selected = j - 1; picked = 0; Wp = Hp = 0; }
				}
				if (m1->text.empty()) break;
			}
			numitems = j;
		}
		itemheight = 1;
		int W = Wp;
		if (m) for (; !m->text.empty(); m = m->next()) {
			int hh;
			int w1 = m->measure(&hh, button);
			if (hh + LEADING > itemheight) itemheight = hh + LEADING;
		}
		int BW = Fl::box_dx(box());
		if (X < scr_x) X = scr_x;
		if (X > scr_x + scr_w - W) X = scr_x + scr_w - W;
		x(X); w(W);
		AllMenuH = ((numitems ? itemheight * numitems - LEADING : 0) + 2 * BW + 3);
		RealH = realHeight;
		h(realHeight);
		Y = Y + Hp;
		if (m) y(Y); else { y(Y - 2); w(1); h(1); }
	}
	DropMenuWidow::~DropMenuWidow() {
		delete hscrollbar;
		hide();
	}
	void DropMenuWidow::position(int X, int Y) {
		Fl_Menu_Window::position(X, Y);
	}
	void DropMenuWidow::autoscroll(int n) {/*
	  int scr_y, scr_h;
	  int Y = y()+Fl::box_dx(box())+2+n*itemheight;

	  int xx, ww;
	  Fl::screen_work_area(xx, scr_y, ww, scr_h);
	  if (Y <= scr_y) Y = scr_y-Y+10;
	  else {
		Y = Y+itemheight-scr_h-scr_y;
		if (Y < 0) return;
		Y = -Y-10;
	  }
	  Fl_Menu_Window::position(x(), y()+Y);*/
	  // y(y()+Y); // don't wait for response from X
	}
	void DropMenuWidow::drawentry(const DropMenuItem* m, int n, int eraseit) {
		if (!m) return; // this happens if -1 is selected item and redrawn

		int BW = Fl::box_dx(box());
		int xx = BW;
		int W = w() - hscrollbar->w();
		int ww = W - BW - 1;
		int yy = BW + 1 + (n - Startnum)*itemheight;
		int hh = itemheight - LEADING;
		m->draw(xx, yy, ww, hh, button, n == selected);
	}

	void DropMenuWidow::draw() {
		Fl_Window::draw();
		Fl_Group::draw();
		//»æÖÆ±³¾°Í¼Æ¬
		//Fl_Image *RGBAimage = nullptr;
		//if (!button->PicKey.KeyVal.empty())
		//	RGBAimage = IResourceService::Ins()->GetImage(button->PicKey.KeyVal);
		//if (RGBAimage)					//Ê¹ÓÃ±³¾°Í¼Æ¬
		//{
		//	fl_push_clip(0, 0, w() - hscrollbar->w(), h());
		//		RGBAimage->draw(0, 0);
		//	fl_pop_clip();
		//}
		//else						    //²»Ê¹ÓÃ±³¾°Í¼Æ¬
		//{
			fl_draw_box(FL_FLAT_BOX, 0, 0, w() - hscrollbar->w(), h(), button ? button->color() : color());
			fl_color(button ? button->FrameColor : color());
			fl_rect(0, 0, w() - hscrollbar->w(), h());
		//}
		if (menu) {
			const DropMenuItem* m; int j;
			for (m = menu->next(Startnum), j = Startnum; !m->text.empty(); j++, m = m->next()) drawentry(m, j, 0);
		}
	}
	void DropMenuWidow::set_selected(int n) {
		if (n != selected) { selected = n; damage(FL_DAMAGE_CHILD); }
	}
	int DropMenuWidow::find_selected(int mx, int my) {
		if (!menu || menu->text.empty()) return -1;
		mx -= x();
		my -= y();
		if (my < 0 || my >= h()) return -1;
		if (!itemheight) { // menubar
			int xx = 3; int n = 0;
			const DropMenuItem* m = menu ? menu->first() : 0;
			for (; ; m = m->next(), n++) {
				if (m->text.empty()) return -1;
				xx += m->measure(0, button) + 16;
				if (xx > mx) break;
			}
			return n;
		}
		if (mx < Fl::box_dx(box()) || mx >= w() - hscrollbar->w()) return -1;
		int n = (my - Fl::box_dx(box()) - 1) / itemheight;
		n += Startnum;
		if (n < 0 || n >= numitems) return -1;
		return n;
	}
	int DropMenuWidow::titlex(int n) {
		const DropMenuItem* m;
		int xx = 3;
		for (m = menu->first(); n--; m = m->next()) xx += m->measure(0, button) + 16;
		return xx;
	}
	int DropMenuWidow::is_inside(int mx, int my) {
		if (mx < x_root() || mx >= x_root() + w() ||
			my < y_root() || my >= y_root() + h()) {
			return 0;
		}
		if (itemheight == 0 && find_selected(mx, my) == -1) {
			// in the menubar but out from any menu header
			return 0;
		}
		return 1;
	}

#define INITIAL_STATE 0	// no mouse up or down since popup() called
#define PUSH_STATE 1	// mouse has been pushed on a normal item
#define DONE_STATE 2	// exit the popup, the current item was picked
#define MENU_PUSH_STATE 3 // mouse has been pushed on a menu title

	static DropMenuState* p = 0;
	int DropMenuState::is_inside(int mx, int my) {
		int i;
		for (i = nummenus - 1; i >= 0; i--) {
			if (p[i]->is_inside(mx, my))
				return 1;
		}
		return 0;
	}
	static inline void setitem(const DropMenuItem* i, int m, int n) {
		p->current_item = i;
		p->menu_number = m;
		p->item_number = n;
	}
	static void setitem(int m, int n) {
		DropMenuState &pp = *p;
		pp.current_item = (n >= 0) ? pp.p[m]->menu->next(n) : 0;
		pp.menu_number = m;
		pp.item_number = n;
	}
	static int forward(int menu) { // go to next item in menu menu if possible
		DropMenuState &pp = *p;
		// Fl_Menu_Button can generate menu=-1. This line fixes it and selectes the first item.
		if (menu == -1)
			menu = 0;
		DropMenuWidow &m = *(pp.p[menu]);
		int item = (menu == pp.menu_number) ? pp.item_number : m.selected;
		while (++item < m.numitems) {
			const DropMenuItem* m1 = m.menu->next(item);
			if (m1->activevisible()) { setitem(m1, menu, item); return 1; }
		}
		return 0;
	}
	static int backward(int menu) { // previous item in menu menu if possible
		DropMenuState &pp = *p;
		DropMenuWidow &m = *(pp.p[menu]);
		int item = (menu == pp.menu_number) ? pp.item_number : m.selected;
		if (item < 0) item = m.numitems;
		while (--item >= 0) {
			const DropMenuItem* m1 = m.menu->next(item);
			if (m1->activevisible()) { setitem(m1, menu, item); return 1; }
		}
		return 0;
	}
	int DropMenuWidow::handle(int e) {
#if defined (__APPLE__) || defined (USE_X11)
		// This off-route takes care of the "detached menu" bug on OS X.
		// Apple event handler requires that we hide all menu windows right
		// now, so that Carbon can continue undisturbed with handling window
		// manager events, like dragging the application window.
		int ret = early_hide_handle(e);
		DropMenuState &pp = *p;
		if (pp.state == DONE_STATE) {
			hide();
			if (pp.fakemenu) {
				pp.fakemenu->hide();
				if (pp.fakemenu->title)
					pp.fakemenu->title->hide();
			}
			int i = pp.nummenus;
			while (i > 0) {
				DropMenuWidow *mw = pp.p[--i];
				if (mw) {
					mw->hide();
					if (mw->title)
						mw->title->hide();
				}
			}
		}
		return ret;
	}

	int DropMenuWidow::early_hide_handle(int e) {
#endif
		DropMenuState &pp = *p;
		switch (e) {
		case FL_KEYBOARD:
			switch (Fl::event_key()) {
			case FL_BackSpace:
			BACKTAB:
				if (!backward(pp.menu_number)) { pp.item_number = -1;backward(pp.menu_number); }
				return 1;
			case FL_Up:
				if (pp.menubar && pp.menu_number == 0) {
					// Do nothing...
				}
				else if (backward(pp.menu_number)) {
					// Do nothing...
				}
				else if (pp.menubar && pp.menu_number == 1) {
					setitem(0, pp.p[0]->selected);
				}
				return 1;
			case FL_Tab:
				if (Fl::event_shift()) goto BACKTAB;
			case FL_Down:
				if (pp.menu_number || !pp.menubar) {
					if (!forward(pp.menu_number) && Fl::event_key() == FL_Tab) {
						pp.item_number = -1;
						forward(pp.menu_number);
					}
				}
				else if (pp.menu_number < pp.nummenus - 1) {
					forward(pp.menu_number + 1);
				}
				return 1;
			case FL_Right:
				if (pp.menubar && (pp.menu_number <= 0 || (pp.menu_number == 1 && pp.nummenus == 2)))
					forward(0);
				else if (pp.menu_number < pp.nummenus - 1) forward(pp.menu_number + 1);
				return 1;
			case FL_Left:
				if (pp.menubar && pp.menu_number <= 1) backward(0);
				else if (pp.menu_number > 0)
					setitem(pp.menu_number - 1, pp.p[pp.menu_number - 1]->selected);
				return 1;
			case FL_Enter:
			case FL_KP_Enter:
			case ' ':
				pp.state = DONE_STATE;
				return 1;
			case FL_Escape:
				setitem(0, -1, 0);
				pp.state = DONE_STATE;
				return 1;
			}
			break;
		case FL_MOVE:
#if ! (defined(WIN32) || defined(__APPLE__))
			if (pp.state == DONE_STATE) {
				return 1; // Fix for STR #2619
			}
			/* FALLTHROUGH */
#endif
		case FL_ENTER:
		case FL_PUSH:
		{
			int mx = Fl::event_x_root();
			int my = Fl::event_y_root();
			mx -= x();
			my -= y();
			if (mx >= w() - hscrollbar->w() && mx <= w()
				&& my >= 0 && my <= RealH)
				return Fl_Window::handle(e);
		}
		case FL_DRAG:
		{
			int mx = Fl::event_x_root();
			int my = Fl::event_y_root();
			if (mx - x() >= w() - hscrollbar->w() && mx - x() <= w()
				&& my - y() >= 0 && my - y() <= RealH)
				return Fl_Window::handle(e);
			int item = 0; int mymenu = pp.nummenus - 1;
			// Clicking or dragging outside menu cancels it...
			if ((!pp.menubar || mymenu) && !pp.is_inside(mx, my)) {
				setitem(0, -1, 0);
				if (e == FL_PUSH)
					pp.state = DONE_STATE;
				return 1;
			}
			for (mymenu = pp.nummenus - 1; ; mymenu--) {
				item = pp.p[mymenu]->find_selected(mx, my);
				if (item >= 0)
					break;
				if (mymenu <= 0) {
					if (pp.menu_number == -1 && e == FL_PUSH) {
						pp.state = DONE_STATE;
						return 1;
					}
					if (pp.current_item && pp.menu_number == 0 && !pp.current_item->submenu()) {
						if (e == FL_PUSH)
							pp.state = DONE_STATE;
						setitem(0, -1, 0);
						return 1;
					}
					return 0;
				}
			}
			if (my == 0 && item > 0) setitem(mymenu, item - 1);
			else setitem(mymenu, item);
			if (e == FL_PUSH) {
				if (pp.current_item && pp.current_item->submenu() // this is a menu title
					&& item != pp.p[mymenu]->selected // and it is not already on
					&& !pp.current_item->callback_) // and it does not have a callback
					pp.state = MENU_PUSH_STATE;
				else
					pp.state = PUSH_STATE;
			}
		}
		return 1;
		case FL_RELEASE:
			int mx = Fl::event_x_root();
			int my = Fl::event_y_root();
			//ÊÇ·ñËÉ¿ª¹ö¶¯Ìõ
			if (mx - x() >= w() - hscrollbar->w() && mx - x() <= w()
				&& my - y() >= 0 && my - y() <= RealH)
				return Fl_Window::handle(e);

			if (!Fl::event_is_click()
				|| pp.state == PUSH_STATE
				|| (pp.menubar && pp.current_item && !pp.current_item->submenu()) // button
				) 
			{
					if (!pp.current_item || pp.current_item->activevisible())
						pp.state = DONE_STATE;
			}
			return Fl_Window::handle(e);
		}
		return Fl_Window::handle(e);
	}
	void menuscroll_cb(Fl_Widget* w, void* data) {
		DropMenuWidow* o = (DropMenuWidow*)data;
		int value = o->hscrollbar->value();
		o->Startnum = o->hscrollbar->value() * (o->numitems - o->RealH / o->itemheight) / o->RealH;
		printf("value = %d, Startnum = %d\n", value, o->Startnum);
		o->redraw();
	}
	const DropMenuItem* DropMenuItem::pulldown(
		int X, int Y, int W, int H, int winheigth, int hscollwidth,
		const DropMenuItem* initial_item,
		const DropMenuUnit* pbutton)  
	{
		Fl_Group::current(0); // fix possible user error...
		DropMenuItem* m1 = this;
		Fl_Widget_Tracker wp((Fl_Widget *)pbutton);
		button = pbutton;
		if (pbutton && pbutton->window()) {
			for (Fl_Window* w = pbutton->window(); w; w = w->window()) {
				X += w->x();
				Y += w->y();
			}
		}
		else {
			X += Fl::event_x_root() - Fl::event_x();
			Y += Fl::event_y_root() - Fl::event_y();
		}
		DropMenuWidow mw(this, X, Y, W, H, winheigth,initial_item);  
#ifndef WIN32
		mw.clear_border();
#endif
		if (mw.RealH < mw.AllMenuH)
		{
			mw.hscrollbar = new Fl_Scrollbar(W - hscollwidth, 0, hscollwidth, mw.RealH);
			mw.hscrollbar->bounds(0, mw.RealH);
			float hscrolltab = (float)mw.RealH / mw.AllMenuH;
			mw.hscrollbar->slider_size(hscrolltab);
			mw.hscrollbar->type(FL_VERTICAL);
			mw.add(mw.hscrollbar);
			mw.hscrollbar->callback(menuscroll_cb, (void*)&mw);
		}
		else
			mw.hscrollbar = new Fl_Scrollbar(0, 0, 0, 0);
		mw.show();
		Fl::grab(mw);
		DropMenuState pp; p = &pp;
		MenuState = &pp;
		pp.p[0] = &mw;
		pp.nummenus = 1;
		pp.menubar = 0;
		pp.state = INITIAL_STATE;
		pp.fakemenu = 0; // kludge for buttons in menubar

		// preselected item, pop up submenus if necessary:
		if (initial_item && mw.selected >= 0) {
			setitem(0, mw.selected);
			goto STARTUP;
		}

		pp.current_item = 0; pp.menu_number = 0; pp.item_number = -1;
		
		initial_item = pp.current_item;
		if (initial_item) goto STARTUP;

		// the main loop: runs until p.state goes to DONE_STATE or the menu
		// widget is deleted (e.g. from a timer callback, see STR #3503):
		for (;;) {

			// make sure all the menus are shown:
			{
				for (int k = 0; k < pp.nummenus; k++) {
					if (!pp.p[k]->shown()) {
						pp.p[k]->show();
					}
				}
			}
			// get events:
			{
				const DropMenuItem* oldi = pp.current_item;
				Fl::wait();
				if (pbutton && wp.deleted()) // menu widget has been deleted (STR #3503)
					break;
				if (pp.state == DONE_STATE) break; // done.
				if (pp.current_item == oldi) continue;
			}

			// only do rest if item changes:
			if (pp.fakemenu) { delete pp.fakemenu; pp.fakemenu = 0; } // turn off "menubar button"

			if (!pp.current_item) { // pointing at nothing
			  // turn off selection in deepest menu, but don't erase other menus:
				pp.p[pp.nummenus - 1]->set_selected(-1);
				continue;
			}

			if (pp.fakemenu) { delete pp.fakemenu; pp.fakemenu = 0; }
			initial_item = 0; // stop the startup code
			pp.p[pp.menu_number]->autoscroll(pp.item_number);

		STARTUP:
			DropMenuWidow& cw = *pp.p[pp.menu_number];
			const DropMenuItem* m = pp.current_item;
			if (!m->activevisible()) { // pointing at inactive item
				cw.set_selected(-1);
				initial_item = 0; // turn off startup code
				continue;
			}
			cw.set_selected(pp.item_number);

			if (m == initial_item) initial_item = 0; // stop the startup code if item found
			while (pp.nummenus > pp.menu_number + 1) delete pp.p[--pp.nummenus];
		}
		const DropMenuItem* m = (pbutton && wp.deleted()) ? NULL : pp.current_item;
		delete pp.fakemenu;
		while (pp.nummenus > 1) delete pp.p[--pp.nummenus];
		mw.hide();
		Fl::grab(0);
		return m;
	}

	


	//const DropMenuItem* DropMenuItem::test_shortcut() const {
	//	const DropMenuItem* m = this;
	//	const DropMenuItem* ret = 0;
	//	if (m) for (; m->text; m = next_visible_or_not(m)) {
	//		if (m->active()) {
	//			// return immediately any match of an item in top level menu:
	//			if (Fl::test_shortcut(m->shortcut_)) return m;
	//			// if (Fl_Widget::test_shortcut(m->text)) return m;
	//			// only return matches from lower menu if nothing found in top menu:
	//			if (!ret && m->submenu()) {
	//				const DropMenuItem* s =
	//					(m->flags&FL_SUBMENU) ? m + 1 : (const DropMenuItem*)m->user_data_;
	//				ret = s->test_shortcut();
	//			}
	//		}
	//	}
	//	return ret;
	//}
}
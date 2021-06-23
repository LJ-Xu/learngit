#include <FL/Fl.H>
#include "DropMenuUnit.h"
//#include <src/flstring.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DropMenuItem.h"
namespace UI
{
#define SAFE_STRCAT(s) { len += (int) strlen(s); if ( len >= namelen ) { *name='\0'; return(-2); } else strcat(name,(s)); }

	size_t MenuStrlcat(char *dst, const char *src, size_t size)
	{
		size_t	srclen;
		size_t	dstlen;
		dstlen = strlen(dst);
		size -= dstlen + 1;
		if (!size) return (dstlen);
		srclen = strlen(src);
		if (srclen > size) srclen = size;
		memcpy(dst + dstlen, src, srclen);
		dst[dstlen + srclen] = '\0';
		return (dstlen + srclen);
	}
	int DropMenuUnit::item_pathname(char *name, int namelen, const DropMenuItem *finditem) {
		name[0] = '\0';
		return item_pathname_(name, namelen, finditem, menu_);
	}

	// INTERNAL: Descend into a specific menu hierarchy
	int DropMenuUnit::item_pathname_(char *name,
		int namelen,
		const DropMenuItem *finditem,
		DropMenuItem *menu)  {
		int len = 0;
		int level = 0;
		finditem = finditem ? finditem : mvalue();
		menu = menu ? menu : this->menu();
		for (int t = 0; t < size(); t++) {
			const DropMenuItem *m = menu + t;
			if (m->submenu()) {				// submenu? descend
				if (m->flags & FL_SUBMENU_POINTER) {
					// SUBMENU POINTER? Recurse to descend
					int slen = strlen(name);
				    DropMenuItem *submenu = (DropMenuItem*)m->user_data();
					if (m->label()) {
						if (*name) SAFE_STRCAT("/");
						SAFE_STRCAT(m->label());
					}
					if (item_pathname_(name, len, finditem, submenu) == 0)
						return 0;
					name[slen] = 0;				// continue from where we were
				}
				else {
					// REGULAR SUBMENU? DESCEND
					++level;
					if (*name) SAFE_STRCAT("/");
					if (m->label()) SAFE_STRCAT(m->label());
					if (m == finditem) return(0);		// found? done.
				}
			}
			else {
				if (m->label()) {				// menu item?
					if (m == finditem) {			// found? tack on itemname, done.
						SAFE_STRCAT("/");
						SAFE_STRCAT(m->label());
						return(0);
					}
				}
				else {					// end of submenu? pop
					if (--level < 0) {
						*name = '\0';
						return -1;
					}
					char *ss = strrchr(name, '/');
					if (ss) { *ss = 0; len = (int)strlen(name); }	// "File/Edit" -> "File"
					else { name[0] = '\0'; len = 0; }	// "File" -> ""
					continue;
				}
			}
		}
		*name = '\0';
		return(-1);					// item not found
	}

	/**
	 Find the menu item for a given menu \p pathname, such as "Edit/Copy".

	 This method finds a menu item in the menu array, also traversing submenus, but
	 not submenu pointers (FL_SUBMENU_POINTER).

	 To get the menu item's index, use find_index(const char*)

	  \b Example:
	  \code
		DropMenuUnitBar *menubar = new DropMenuUnitBar(..);
		menubar->add("File/&Open");
		menubar->add("File/&Save");
		menubar->add("Edit/&Copy");
		// [..]
		DropMenuItem *item;
		if ( ( item = (DropMenuItem*)menubar->find_item("File/&Open") ) != NULL ) {
		item->labelcolor(FL_RED);
		}
		if ( ( item = (DropMenuItem*)menubar->find_item("Edit/&Copy") ) != NULL ) {
		item->labelcolor(FL_GREEN);
		}
	  \endcode

	  \param pathname The path and name of the menu item
	  \returns The item found, or NULL if not found
	  \see find_index(const char*), find_item(Fl_Callback*), item_pathname()
	*/
	const DropMenuItem * DropMenuUnit::find_item(const char *pathname) {
		int i = find_index(pathname);
		return((i == -1) ? 0 : (const DropMenuItem*)(menu_ + i));
	}

	/**
	 Find the index into the menu array for a given \p item.

	 A way to convert a menu item pointer into an index.

	 Does \b not handle items that are in submenu pointers (FL_SUBMENU_POINTER).

	 -1 is returned if the item is not in this menu
	 or is part of an FL_SUBMENU_POINTER submenu.

	 Current implementation is fast and not expensive.

	 \code
	   // Convert an index-to-item
	   int index = 12;
	   const DropMenuItem *item = mymenu->menu() + index;

	   // Convert an item-to-index
	   int index = mymenu->find_index(item);
	   if ( index == -1 ) { ..error.. }
	 \endcode

	 \param[in]  item  The item to be found
	 \returns    The index of the item, or -1 if not found.
	 \see        menu()
	*/
	int DropMenuUnit::find_index(const DropMenuItem *item) const {
		DropMenuItem *max = menu_ + size();
		if (item < menu_ || item >= max) return(-1);
		return (int)(item - menu_);
	}

	/**
	 Find the index into the menu array for a given callback \p cb.

	 This method finds a menu item's index position, also traversing submenus, but
	 \b not submenu pointers (FL_SUBMENU_POINTER). This is useful if an
	 application uses internationalisation and a menu item can not be found
	 using its label. This search is also much faster.

	 \param cb Find the first item with this callback
	 \returns  The index of the item with the specific callback, or -1 if not found
	 \see      find_index(const char*)
	 */
	//int DropMenuUnit::find_index(Fl_Callback *cb) const {
	//	for (int t = 0; t < size(); t++)
	//		if (menu_[t].callback_ == cb)
	//			return(t);
	//	return(-1);
	//}

	/**
	 Find the menu item index for a given menu \p pathname, such as "Edit/Copy".

	 This method finds a menu item's index position for the given menu pathname,
	 also traversing submenus, but \b not submenu pointers (FL_SUBMENU_POINTER).

	 To get the menu item pointer for a pathname, use find_item()

	 \param[in] pathname The path and name of the menu item to find
	 \returns        The index of the matching item, or -1 if not found.
	 \see            item_pathname()

	*/
	int DropMenuUnit::find_index(const char *pathname) const {
		char menupath[1024] = "";	// File/Export
		
		for (int t = 0; t < size(); t++) {
			DropMenuItem *m = menu_ + t;
			if (m->flags&FL_SUBMENU) {
				// IT'S A SUBMENU
				// we do not support searches through FL_SUBMENU_POINTER links
				if (menupath[0]) MenuStrlcat(menupath, "/", sizeof(menupath));
				MenuStrlcat(menupath, m->label(), sizeof(menupath));
				if (!strcmp(menupath, pathname)) return(t);
			}
			else {
				if (!m->label()) {
					// END OF SUBMENU? Pop back one level.
					char *ss = strrchr(menupath, '/');
					if (ss) *ss = 0;
					else menupath[0] = '\0';
					continue;
				}
				// IT'S A MENU ITEM
				char itempath[1024];	// eg. Edit/Copy
				strcpy(itempath, menupath);
				if (itempath[0]) MenuStrlcat(itempath, "/", sizeof(itempath));
				MenuStrlcat(itempath, m->label(), sizeof(itempath));
				if (!strcmp(itempath, pathname)) return(t);
			}
		}
		return(-1);
	}

	/**
	 Find the menu item for the given callback \p cb.

	 This method finds a menu item in a menu array, also traversing submenus, but
	 not submenu pointers. This is useful if an application uses
	 internationalisation and a menu item can not be found using its label. This
	 search is also much faster.

	 \param[in] cb find the first item with this callback
	 \returns The item found, or NULL if not found
	 \see find_item(const char*)
	 */
	const DropMenuItem * DropMenuUnit::find_item(Fl_Callback *cb) {
		for (int t = 0; t < size(); t++) {
			const DropMenuItem *m = menu_ + t;
			if (m->callback_ == cb) {
				return m;
			}
		}
		return (const DropMenuItem *)0;
	}

	/**
	  The value is the index into menu() of the last item chosen by
	  the user.  It is zero initially.  You can set it as an integer, or set
	  it with a pointer to a menu item.  The set routines return non-zero if
	  the new value is different than the old one.
	*/
	int DropMenuUnit::value(const DropMenuItem* m) {
		clear_changed();
		if (value_ != m) { value_ = m; return 1; }
		return 0;
	}

	/**
	 When user picks a menu item, call this.  It will do the callback.
	 Unfortunately this also casts away const for the checkboxes, but this
	 was necessary so non-checkbox menus can really be declared const...
	*/
	const DropMenuItem* DropMenuUnit::picked(const DropMenuItem* v) {
		if (v) {
			if (v->radio()) {
				if (!v->value()) { // they are turning on a radio item
					set_changed();
					//setonly((DropMenuItem*)v);
				}
				redraw();
			}
			else if (v->flags & FL_MENU_TOGGLE) {
				set_changed();
				((DropMenuItem*)v)->flags ^= FL_MENU_VALUE;
				redraw();
			}
			else if (v != value_) { // normal item
				set_changed();
			}
			value_ = v;
			if (when()&(FL_WHEN_CHANGED | FL_WHEN_RELEASE)) {
				if (changed() || when()&FL_WHEN_NOT_CHANGED) {
					if (value_ && value_->callback_) value_->do_callback((Fl_Widget*)this);
					else do_callback();
				}
			}
		}
		return v;
	}

	/* Scans an array of DropMenuItem's that begins at start, searching for item.
	 Returns NULL if item is not found.
	 If item is present, returns start, unless item belongs to an
	 FL_SUBMENU_POINTER-adressed array of items, in which case the first item of this array is returned.
	 */
	static DropMenuItem *first_submenu_item(DropMenuItem *item, DropMenuItem *start)
	{
		DropMenuItem* m = start;
		int nest = 0; // will indicate submenu nesting depth
		while (1) { // loop over all menu items
			if (m->text.empty()) { // m is a null item
				if (!nest) return NULL; // item was not found
				nest--; // m marks the end of a submenu -> decrement submenu nesting depth
			}
			else { // a true item
				if (m == item) return start; // item is found, return menu start item
				if (m->flags & FL_SUBMENU_POINTER) {
					// scan the detached submenu which begins at m->user_data()
					DropMenuItem *first = first_submenu_item(item, (DropMenuItem*)m->user_data());
					if (first) return first; // if item was found in the submenu, return
				}
				else if (m->flags & FL_SUBMENU) { // a direct submenu
					nest++; // increment submenu nesting depth
				}
			}
			m++; // step to next menu item
		}
	}


	/** Turns the radio item "on" for the menu item and turns "off" adjacent radio items of the same group. */
	//void DropMenuUnit::setonly(DropMenuItem* item) {
	//	// find the first item of the (sub)menu containing item
	//	DropMenuItem* first = first_submenu_item(item, menu_);
	//	if (!first) return; // item does not belong to our menu
	//	item->flags |= FL_MENU_RADIO | FL_MENU_VALUE;
	//	DropMenuItem* j;
	//	for (j = item; ; ) {	// go down
	//		if (j->flags & FL_MENU_DIVIDER) break; // stop on divider lines
	//		j++;
	//		if (!j->text || !j->radio()) break; // stop after group
	//		j->clear();
	//	}
	//	for (j = item - 1; j >= first; j--) { // go up
	//	  //DEBUG printf("GO UP: WORKING ON: item='%s', flags=%x\n", j->text, j->flags);
	//		if (!j->text || (j->flags&FL_MENU_DIVIDER) || !j->radio()) break;
	//		j->clear();
	//	}
	//}

	/** Turns the radio item "on" for the menu item and turns "off" adjacent radio items set.
	 \deprecated This method is dangerous if radio items are first in the menu.
	 Use Fl_Menu_::setonly(DropMenuItem*) instead.
	 */
	//void DropMenuItem::setonly() {
	//	flags |= FL_MENU_RADIO | FL_MENU_VALUE;
	//	DropMenuItem* j;
	//	for (j = this; ; ) {	// go down
	//		if (j->flags & FL_MENU_DIVIDER) break; // stop on divider lines
	//		j++;
	//		if (!j->text || !j->radio()) break; // stop after group
	//		j->clear();
	//	}
	//	for (j = this - 1; ; j--) { // go up
	//		if (!j->text || (j->flags&FL_MENU_DIVIDER) || !j->radio()) break;
	//		j->clear();
	//	}
	//}

	/**
	 Creates a new Fl_Menu_ widget using the given position, size,
	 and label string.  menu() is initialized to null.
	 */
	DropMenuUnit::DropMenuUnit(int X, int Y, int W, int H, const char* l)
		: Fl_Widget(X, Y, W, H, l) {
		set_flag(SHORTCUT_LABEL);
		box(FL_UP_BOX);
		when(FL_WHEN_RELEASE_ALWAYS);
		value_ = menu_ = 0;
		alloc = 0;
		selection_color(FL_SELECTION_COLOR);
		textfont(FL_HELVETICA);
		textsize(FL_NORMAL_SIZE);
		textcolor(FL_FOREGROUND_COLOR);
		down_box(FL_NO_BOX);
	}

	/**
	  This returns the number of DropMenuItem structures that make up the
	  menu, correctly counting submenus.  This includes the "terminator"
	  item at the end.  To copy a menu array you need to copy
	  size()*sizeof(DropMenuItem) bytes.  If the menu is
	  NULL this returns zero (an empty menu will return 1).
	*/
	int DropMenuUnit::size() const {
		if (!menu_) return 0;
		return menu_->size();
	}

	/**
		Sets the menu array pointer directly.  If the old menu is private it is
		deleted.  NULL is allowed and acts the same as a zero-length
		menu.  If you try to modify the array (with add(), replace(), or
		remove()) a private copy is automatically done.
	*/
	void DropMenuUnit::menu(const DropMenuItem* m) {
		clear();
		value_ = menu_ = (DropMenuItem*)m;
	}

	// this version is ok with new Fl_Menu_add code with fl_menu_array_owner:

	/**
	  Sets the menu array pointer with a copy of m that will be automatically deleted.
	  If userdata \p ud is not NULL, then all user data pointers are changed in the menus as well.
	  See void Fl_Menu_::menu(const DropMenuItem* m).
	*/
	void DropMenuUnit::copy(const DropMenuItem* m, void* ud) {
		int n = m->size();
		DropMenuItem* newMenu = new DropMenuItem[n];
		memcpy(newMenu, m, n * sizeof(DropMenuItem));
		menu(newMenu);
		alloc = 1; // make destructor free array, but not strings
		// for convenience, provide way to change all the user data pointers:
		if (ud) for (; n--;) {
			if (newMenu->callback_) newMenu->user_data_ = ud;
			newMenu++;
		}
	}

	DropMenuUnit::~DropMenuUnit() {
		clear();
	}
	
	// Fl_Menu::add() uses this to indicate the owner of the dynamically-
	// expanding array.  We must not free this array:
	DropMenuUnit* fl_menu_array_owner = 0;

	/**
	  Same as menu(NULL), set the array pointer to null, indicating
	  a zero-length menu.

	  Menus must not be cleared during a callback to the same menu.
	*/
	void DropMenuUnit::clear() {
		if (alloc) {
			if (alloc > 1) for (int i = size(); i--;)
				if (!menu_[i].text.empty()) free((void*)menu_[i].text.c_str());
			if (this == fl_menu_array_owner)
				fl_menu_array_owner = 0;
			else
				delete[] menu_;
			menu_ = 0;
			value_ = 0;
			alloc = 0;
		}
	}

	/**
	 Clears the specified submenu pointed to by \p index of all menu items.

	 This method is useful for clearing a submenu so that it can be
	 re-populated with new items. Example: a "File/Recent Files/..." submenu
	 that shows the last few files that have been opened.

	 The specified \p index must point to a submenu.

	 The submenu is cleared with remove().
	 If the menu array was directly set with menu(x), then copy()
	 is done to make a private array.

	 \warning Since this method can change the internal menu array, any menu
	 item pointers or indices the application may have cached can become
	 stale, and should be recalculated/refreshed.

	 \b Example:
	 \code
	   int index = menubar->find_index("File/Recent");    // get index of "File/Recent" submenu
	   if ( index != -1 ) menubar->clear_submenu(index);  // clear the submenu
	   menubar->add("File/Recent/Aaa");
	   menubar->add("File/Recent/Bbb");
	   [..]
	 \endcode

	 \param index The index of the submenu to be cleared
	 \returns 0 on success, -1 if the index is out of range or not a submenu
	 \see remove(int)
	 */
	//int DropMenuUnit::clear_submenu(int index) {
	//	if (index < 0 || index >= size()) return(-1);
	//	if (!(menu_[index].flags & FL_SUBMENU)) return(-1);
	//	++index;					// advance to first item in submenu
	//	while (index < size()) {                    // keep remove()ing top item until end is reached
	//		if (menu_[index].text == 0) break;	// end of this submenu? done
	//		remove(index);				// remove items/submenus
	//	}
	//	return(0);
	//}

	//
	// End of "$Id$".
	//

}
/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DigitalDisplayView.cpp
 * Author   : qiaodan
 * Date     : 2020-09-18
 * Descript : 绘制显示框和文字、背景图片
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "ResourceService.h"
#include "stdafx.h"
#include "DigitalDisplayView.h"
#include "DigitalDisplayControl.h"
#include "DigitalDisplayModel.h"
#include <FL/fl_draw.H>
#include "Message.h"

namespace UI
{
#define MAXBUF 320
#if defined(USE_X11) && !USE_XFT
	const int secret_char = '*'; // asterisk to hide secret input
#else
	const int secret_char = 0x2022; // bullet to hide secret input
#endif
	static int l_secret;

	const char* InputView::expand(const char* p, char* buf, char* text) const {
		char* o = buf;
		char* e = buf + (MAXBUF - 4);
		const char* lastspace = p;
		char* lastspace_out = o;
		int width_to_lastspace = 0;
		int word_count = 0;
		int word_wrap;
		//  const char *pe = p + strlen(p);

		if (input_type() == FL_SECRET_INPUT) {
			while (o < e && p < text + size()) {
				if (fl_utf8len((char)p[0]) >= 1) {
					l_secret = fl_utf8encode(secret_char, o);
					o += l_secret;
				}
				p++;
			}

		}
		else while (o < e) {
			if (wrap() && (p >= text + size() || isspace(*p & 255))) {
				word_wrap = w() - Fl::box_dw(box()) - 2;
				width_to_lastspace += (int)fl_width(lastspace_out, (int)(o - lastspace_out));
				if (p > lastspace + 1) {
					if (word_count && width_to_lastspace > word_wrap) {
						p = lastspace; o = lastspace_out; break;
					}
					word_count++;
				}
				lastspace = p;
				lastspace_out = o;
			}

			if (p >= text + size()) break;
			int c = *p++ & 255;
			if (c < ' ' || c == 127) {
				if (c == '\n' && input_type() == FL_MULTILINE_INPUT) { p--; break; }
				if (c == '\t' && input_type() == FL_MULTILINE_INPUT) {
					for (c = fl_utf_nb_char((uchar*)buf, (int)(o - buf)) % 8; c < 8 && o < e; c++) {
						*o++ = ' ';
					}
				}
				else {
					*o++ = '^';
					*o++ = c ^ 0x40;
				}
			}
			else {
				*o++ = c;
			}
		}
		*o = 0;
		return p;
	}

	double InputView::expandpos(
		const char* p,	// real string
		const char* e,	// pointer into real string
		const char* buf,	// conversion of real string by expand()
		int* returnn		// return offset into buf here
	) const {
		int n = 0;
		int chr = 0;
		int l;
		if (input_type() == FL_SECRET_INPUT) {
			while (p < e) {
				l = fl_utf8len((char)p[0]);
				if (l >= 1) n += l_secret;
				p += l;
			}
		}
		else while (p < e) {
			int c = *p & 255;
			if (c < ' ' || c == 127) {
				if (c == '\t' && input_type() == FL_MULTILINE_INPUT) {
					n += 8 - (chr % 8);
					chr += 7 - (chr % 8);
				}
				else n += 2;
			}
			else {
				n++;
			}
			chr += fl_utf8len((char)p[0]) >= 1;
			p++;
		}
		if (returnn) *returnn = n;
		return fl_width(buf, n);
	}

	void InputView::drawtext(int X, int Y, int W, int H, int align, Fl_Image *img,
		bool usepassword, bool hidetext) {
		int do_mu = !(damage()&FL_DAMAGE_ALL);
		if (hidetext)
		{
			draw_box();
			return;
		}
		int tsize = strlen(value()) ;
		char *text = new char[tsize + 1];
		strcpy(text, value());
		if (usepassword)
		{
			memset(text, 0, tsize);
			for (size_t i = 0; i < (size_t)tsize; i++)
				text[i] = '*';
		}
		if (Fl::focus() != this && !size()) {
			if (do_mu) { // we have to erase it if cursor was there
				draw_box();
			}
			delete[] text;
			return;
		}
		int selstart, selend;
		if (Fl::focus() != this && /*Fl::selection_owner()!=this &&*/ Fl::pushed() != this)
			selstart = selend = 0;
		else if (position() <= mark()) {
			selstart = position(); selend = mark();
		}
		else {
			selend = position(); selstart = mark();
		}

		fl_font(textfont(), textsize());
		const char *p, *e;
		char buf[MAXBUF];

		// count how many lines and put the last one into the buffer:
		// And figure out where the cursor is:
		int height = fl_height();
		int threshold = height / 2;
		int lines;
		int curx, cury;
		for (p = text, curx = cury = lines = 0; ;) {
			e = expand(p, buf, text);
			if (position() >= p - text && position() <= e - text) {
				curx = int(expandpos(p, text + position(), buf, 0) + .5);
				//if (Fl::focus() == this && !was_up_down) up_down_pos = curx;
				cury = lines * height;
			}
			lines++;
			if (e >= text + size()) break;
			p = e + 1;
		}

		//fl_push_clip(X, Y, W, H);
		Fl_Color tc = active_r() ? textcolor() : fl_inactive(textcolor());

		p = text;
		// visit each line and draw it:
		//计算X起始位置，和Y起始位置偏差值
		int imageW = 3;	//图片边框宽
		int desc = height - fl_descent();
		float xpos;
		int width = (int)fl_width(text, strlen(text));
		int ypos;
		//int align = 0;
		if (align & FL_ALIGN_BOTTOM) ypos = H - lines * height - imageW;
		else if (align & FL_ALIGN_TOP) ypos = imageW;
		else ypos = (H - lines * height) / 2;

		if (align & FL_ALIGN_LEFT) xpos = (float)X + (float)imageW;
		else if (align & FL_ALIGN_RIGHT) xpos = (float)X + (float)W - (float)(width + imageW + 1);
		else xpos = (float)X + (float)(W - (int)(width + imageW)) / 2;

		for (; ypos < H;) {

			// re-expand line unless it is the last one calculated above:

			if (do_mu) {	// for minimal update:
				const char* pp = text; // pointer to where minimal update starts
				if (e < pp) goto CONTINUE2; // this line is before the changes
				// calculate area to erase:
				float r = (float)(X + W);
				float xx;
				if (p >= pp) {
					xx = (float)X;
				}
				else {
					xx = xpos + (float)expandpos(p, pp, buf, 0);
				}
				draw_box();
				// it now draws entire line over it
				// this should not draw letters to left of erased area, but
				// that is nyi.
			}

			// Draw selection area if required:
			if (selstart < selend && selstart <= e - text && selend > p - text) {
				const char* pp = text + selstart;
				float x1 = xpos;
				int offset1 = 0;
				if (pp > p) {
					fl_color(tc);
					x1 += (float)expandpos(p, pp, buf, &offset1);
					fl_draw(buf, offset1, (int)xpos, (int)(Y + ypos + desc));
				}
				pp = text + selend;
				float x2 = (float)(X + W);
				int offset2;
				if (pp <= e) x2 = xpos + (float)expandpos(p, pp, buf, &offset2);
				else offset2 = (int)strlen(buf);

				{
					fl_color(selection_color());
					fl_rectf((int)(x1 + 0.5), Y + ypos, (int)(x2 - x1 + 0.5), height);
					fl_color(fl_contrast(textcolor(), selection_color()));
				}
				fl_draw(buf + offset1, offset2 - offset1, (int)x1, (int)(Y + ypos + desc));

				if (pp < e) {
					fl_color(tc);
					fl_draw(buf + offset2, (int)strlen(buf + offset2), (int)x2, (int)(Y + ypos + desc));
				}
			}
			else {
				// draw unselected text
				fl_color(tc);
				fl_draw(buf, (int)strlen(buf), (int)xpos, (int)(Y + ypos + desc));
			}

			if (do_mu) fl_pop_clip();

		CONTINUE2:
			// draw the cursor:
			if (Fl::focus() == this && (selstart == selend) &&
				position() >= p - text && position() <= e - text && OpenKeypage) {
				fl_color(cursor_color());
				// cursor position may need to be recomputed (see STR #2486)
				curx = int(expandpos(p, text + position(), buf, 0) + .5);
				if (readonly()) {
					fl_line((int)(xpos + curx - 2.5f), Y + ypos + height - 1,
						(int)(xpos + curx + 0.5f), Y + ypos + height - 4,
						(int)(xpos + curx + 3.5f), Y + ypos + height - 1);
				}
				else {
					fl_rectf((int)(xpos + curx + 0.5), Y + ypos, 2, height);
				}

			}

		CONTINUE:
			ypos += height;
			if (e >= text + size()) break;
			if (*e == '\n' || *e == ' ') e++;
			p = e;
		}

		fl_pop_clip();
		if (Fl::focus() == this) {
			fl_set_spot(textfont(), textsize(),
				(int)xpos + curx, Y + ypos - fl_descent(), W, H, window());
		
		}
		delete[] text;
	}

	DigitalDisplayView::DigitalDisplayView(int X, int Y, int W, int H, const char* l) : InputView(X, Y, W, H, l) {}
	DigitalDisplayView::~DigitalDisplayView() {}
	
	int DigitalDisplayView::handle(int event)
	{
		shared_ptr<DigitalDisplayModel> model = BaseView.GetModel<DigitalDisplayModel>();
		shared_ptr<DigitalDisplayControl> ctrl = BaseView.GetControl<DigitalDisplayControl>();
		//return HMIBaseInput::handle(event);

		return 1;
	}
	void DigitalDisplayView::draw()
	{
		shared_ptr<DigitalDisplayControl> ctrl = BaseView.GetControl<DigitalDisplayControl>();
		shared_ptr<DigitalDisplayModel> model = BaseView.GetModel<DigitalDisplayModel>();
		//绘制框体
		Fl_Image *rgbImage = IResourceService::Ins()->GetImage(model->DisNumConfig.Key.KeyVal);
		if (!rgbImage) {
			LOG_INFO_("Digital Display Pic is NULL\n");
		}
		else {
			image(rgbImage);
			align(FL_ALIGN_IMAGE_BACKDROP);		//设置背景图片
			box(FL_NO_BOX);
		}
		/*获取数据*/
		Fl_Boxtype b = box();
		if (damage() & FL_DAMAGE_ALL) {
			draw_box();
		}
		/*设置字体*/
		textfont(UI::IResourceService::GetFontIdx(model->DisNumConfig.TextStyle.Font.Name));
		textsize(model->DisNumConfig.TextStyle.Font.Size);
		/*设置字体颜色和文字*/
		Fl_Color tcolor;
		value(DisplayValue.c_str());
		if (model->DisNumConfig.IsWarnStyleUsed)
		{
			if (IsExceedMaxLmt)		//超出最大值
				tcolor = fl_rgb_color(RGBColor(model->DisNumConfig.WarnStyleUpper.Color));
			else if (IsExceedMinLmt)	//低于最小值
			
				tcolor = fl_rgb_color(RGBColor(model->DisNumConfig.WarnStyleLower.Color));
			else
				tcolor = fl_rgb_color(RGBColor(model->DisNumConfig.TextStyle.Colors));
		}
		else
			tcolor = fl_rgb_color(RGBColor(model->DisNumConfig.TextStyle.Colors));
		textcolor(tcolor);
	
		drawtext(model->DisNumConfig.X + model->DisNumConfig.OffX,
			model->DisNumConfig.Y + model->DisNumConfig.OffY,
			model->DisNumConfig.Width, model->DisNumConfig.Height,
			model->DisNumConfig.TextStyle.Align, rgbImage,model->DisNumConfig.PswMode,IsHideText);
	}
}

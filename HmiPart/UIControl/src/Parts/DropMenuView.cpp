/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name     : DropMenuView.cpp
 * Author   : qiaodan
 * Date     : 2020-12-03
 * Descript : 绘制下拉菜单和清单
 * Version  : V0.1
 * modify   :
 *          :
 *******************************************************************************/
#include "GraphicDrawHandle.h"
#include "DropMenuView.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include "DropMenuModel.h"
#include "StringUtility.h"
#include "IResourceService.h"
#include "UIData.h"
#include "UICommand.h"
#include "HMIWindow.h"
#include "DropMenuControl.h"
#include "OperatorGControl.h"
#include "SysSetApi.h"
namespace UI
{
	static DropMenuView	*pressed_menu_button_ = 0;

	void Listscroll_cb(Fl_Widget* w, void* data) {
		DropMenuView* o = (DropMenuView*)data;
		o->MenuScrollvalue = o->MenuScrollbar->value();
		//o->MenuScrollbar->value() * o->Rowheight * o->MenuNum / o->h();
		o->StartNum = o->MenuScrollbar->value() * (int)(((double)o->MenuNum - (double)o->h() / (double)o->Rowheight)) / o->h();
		o->redraw();
	}

	void DropMenuView::draw()
	{
		if (IResourceService::Ins()->IsRenderMode())
			InitDraw();
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		if (model->MenuConfig.MenuMode == 0)
			h(model->MenuConfig.ContentHeight);
		else
			Rowheight = model->MenuConfig.ContentHeight;
		//判断模式
		if (model->MenuConfig.MenuMode == 0)	 //下拉菜单
		{
			DropMenuItem* m = menu()->next(SelectNum);
			if(m)
			{
				align(m->labelalign_);
				labelcolor(m->labelcolor_);
				labelfont(m->labelfont_);
				labelsize(m->labelsize_);
				label(m->text.c_str());
			}	

			if (model->MenuConfig.PopMode == 1)		//向上弹出
				resize(model->MenuConfig.X + model->MenuConfig.OffX, 
					model->MenuConfig.Y + (model->MenuConfig.Height - h()) + model->MenuConfig.OffY, w(), h());

			int H = (labelmaxsize_ - 3)&-2;
			int X = x() + w() - H - 1;
			int Y = y() + (h() - H) / 2;
			Fl_Image *RGBAimage = nullptr;
			/*绘制背景*/
			if (model->MenuConfig.PicKey.size() >= 3)
			{
				RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[2].KeyVal);
				if (RGBAimage)			//使用背景图片
				{
					color(bgcolor_);	//设置背景颜色
					image(RGBAimage);
					align(align() | FL_ALIGN_IMAGE_BACKDROP);
					box(FL_NO_BOX);
					draw_box();
					fl_color(active() ? FrameColor : fl_inactive(FrameColor));
					fl_line_style(0, 0);
					fl_rect(x(), y(), w(), h());
					fl_box(FL_FLAT_BOX);
				}
				else
				{
					LOG_INFO_("DropMenu0 BackPic is NULL\n");
					color(bgcolor_);	//设置背景颜色
					draw_box(pressed_menu_button_ == this ? fl_down(box()) : box(), color());
					fl_color(active() ? FrameColor : fl_inactive(FrameColor));
					fl_line_style(0, 0);
					fl_rect(x(), y(), w(), h());
					fl_box(FL_FLAT_BOX);
				}
			}
			/*绘制按钮*/
			if (IResourceService::Ins()->IsRenderMode())
			{
				int status;
				IResourceService::Ins()->SetRenderStatus(status, 2);
				if (status == 0 && !model->MenuConfig.PicKey.empty())
				{
					RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[0].KeyVal);
					if (RGBAimage)			//使用背景图片
						RGBAimage->draw(model->MenuConfig.BtnPosition.X + model->MenuConfig.OffX,
							model->MenuConfig.BtnPosition.Y + model->MenuConfig.OffY);
					else
						LOG_INFO_("DropMenu0 UpPic is NULL\n");
				}
				else if (status == 1 && model->MenuConfig.PicKey.size() >= 2)
				{
					RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[1].KeyVal);
					if (RGBAimage)			//使用背景图片
						RGBAimage->draw(model->MenuConfig.BtnPosition.X + model->MenuConfig.OffX,
							model->MenuConfig.BtnPosition.Y + model->MenuConfig.OffY);
					else
						LOG_INFO_("DropMenu0 PushPic is NULL\n");
				}
				else
					LOG_INFO_("DropMenu0 Pic is NULL\n");
			}
			else
			{
				if (pressed_menu_button_ == this)		//按下
				{
					if (!model->MenuConfig.PicKey.empty())
					{
						RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[0].KeyVal);
						if (RGBAimage)			//使用背景图片
							RGBAimage->draw(model->MenuConfig.BtnPosition.X + model->MenuConfig.OffX,
								model->MenuConfig.BtnPosition.Y + model->MenuConfig.OffY);
						else
							LOG_INFO_("DropMenu0 BackPic is NULL\n");
					}
				}
				else
				{
					if (model->MenuConfig.PicKey.size() >= 2)
					{
						RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[1].KeyVal);
						if (RGBAimage)			//使用背景图片
							RGBAimage->draw(model->MenuConfig.BtnPosition.X + model->MenuConfig.OffX,
								model->MenuConfig.BtnPosition.Y + model->MenuConfig.OffY);
						else
							LOG_INFO_("DropMenu10 UpPic is NULL\n");
					}
					else
						LOG_INFO_("DropMenu10 PushPic is NULL\n");
				}

			}

			/*绘制文字*/
			fl_font(labelfont(), labelsize());
			fl_color(labelcolor());
			draw_label(x(), y(), X - x() + 2, h());
			if (IResourceService::Ins()->IsRenderMode())
			{
				if (model->MenuConfig.PopMode == 0)		//向下拉
				{
					GraphicDrawHandle::PushClip(x(), y() + h(), w(), model->MenuConfig.Height - h());	//这里的h是整个长
					{
						fl_draw_box(FL_FLAT_BOX, x(), y() + h(), w() - model->MenuConfig.HscollWidth, model->MenuConfig.Height - h(), color());
						fl_color(color());
						fl_rect(x(), y() + h(), w() - model->MenuConfig.HscollWidth, model->MenuConfig.Height - h());
						if (menu()) {
							const DropMenuItem* m; int j;
							int Startnum = 0;
							for (m = menu()->next(Startnum), j = Startnum; !m->text.empty(); j++, m = m->next())
							{
								if (m) {
									int BW = Fl::box_dx(box());
									int xx = BW + x();
									int W = w() - model->MenuConfig.HscollWidth;
									int ww = W - BW;
									int yy = BW + (j - Startnum) * h() + y() + h();
									int hh = h();
									m->draw(xx, yy, ww, hh, this, j == 0);
								}
							}
							float hscrolltab = (float)(model->MenuConfig.Height - h()) / (float)(h() * MenuNum);
							MenuScrollbar->slider_size(hscrolltab);
							fl_draw_box(MenuScrollbar->box(), x() + w() - model->MenuConfig.HscollWidth, 
								y() + h(), model->MenuConfig.HscollWidth, model->MenuConfig.Height - h(), MenuScrollbar->color());
							fl_draw_box(MenuScrollbar->slider(), x() + w() - model->MenuConfig.HscollWidth, y() + h(),
								model->MenuConfig.HscollWidth, (int)(MenuScrollbar->slider_size()*(model->MenuConfig.Height - h()) + .5), FL_GRAY);
						}
					}
					fl_pop_clip();
				}
				else //向上弹出
				{
					GraphicDrawHandle::PushClip(x(), y() - (model->MenuConfig.Height - h()), w(), model->MenuConfig.Height - h());	//这里的h是整个长
					{
						fl_draw_box(FL_FLAT_BOX, x(), y() - (model->MenuConfig.Height - h()), w() - model->MenuConfig.HscollWidth, model->MenuConfig.Height - h(), color());
						fl_color(color());
						fl_rect(x(), y() - (model->MenuConfig.Height - h()), w() - model->MenuConfig.HscollWidth, model->MenuConfig.Height - h());
						if (menu()) {
							const DropMenuItem* m; int j;
							int Startnum = 0;
							for (m = menu()->next(Startnum), j = Startnum; !m->text.empty(); j++, m = m->next())
							{
								if (m) {
									int BW = Fl::box_dx(box());
									int xx = BW + x();
									int W = w() - model->MenuConfig.HscollWidth;
									int ww = W - BW;
									int yy = BW + (j - Startnum) * h() + y() - (model->MenuConfig.Height - h());
									int hh = h();
									m->draw(xx, yy, ww, hh, this, j == 0);
								}
							}
							float hscrolltab = (float)(model->MenuConfig.Height - h()) / (float)(h() * MenuNum);
							MenuScrollbar->slider_size(hscrolltab);
							fl_draw_box(MenuScrollbar->box(), x() + w() - model->MenuConfig.HscollWidth,
								y() - (model->MenuConfig.Height - h()), model->MenuConfig.HscollWidth, model->MenuConfig.Height - h(), MenuScrollbar->color());
							fl_draw_box(MenuScrollbar->slider(), x() + w() - model->MenuConfig.HscollWidth, y() - (model->MenuConfig.Height - h()),
								model->MenuConfig.HscollWidth, (int)(MenuScrollbar->slider_size()*(model->MenuConfig.Height - h()) + .5), FL_GRAY);
						}
					}
					fl_pop_clip();
				}
			}
		}
		else
		{
			int dy = 0;
			//if (h() % Rowheight != 0)
			//	dy = (h() / Rowheight + 1) * Rowheight - h();
				//dy = (h() / Rowheight + 1) * Rowheight - h();
			//Fl_Group::draw();
			dy = ((Rowheight * MenuNum) - h()) * MenuScrollvalue / h();
			if (Rowheight * MenuNum <= h())
				model->MenuConfig.HscollWidth = 0;
			Fl_Image *RGBAimage = nullptr;
			if (!model->MenuConfig.PicKey.empty())
				RGBAimage = IResourceService::Ins()->GetImage(model->MenuConfig.PicKey[0].KeyVal);
			if (RGBAimage)			//使用背景图片
			{
				image(RGBAimage);
				align(align() | FL_ALIGN_IMAGE_BACKDROP);
				box(FL_NO_BOX);
				draw_box();
			}
			else
			{
				LOG_INFO_("Menu BackPic is NULL\n");
				color(active() ? bgcolor_ : fl_inactive(bgcolor_));		//设置背景颜色
				box(FL_FLAT_BOX);
				draw_box();
			}
			GraphicDrawHandle::PushClip(x(), y(), w() - model->MenuConfig.HscollWidth, h());	//这里的h是整个长
			{
				for (unsigned i = 0; i < model->MenuConfig.MenuInfo.size(); i++)
				{
					if (i == SelectNum)
					{
						fl_color(active() ? selection_color() : fl_inactive(selection_color()));
						fl_rectf(x(), y() - dy + Rowheight * i, w() - model->MenuConfig.HscollWidth, Rowheight);
					}
					DropMenuItem* m = menu()->next(i);
					fl_color(active() ? m->labelcolor() : fl_inactive(m->labelcolor()));
					fl_font(m->labelfont(), m->labelsize());
					fl_draw(m->label(), x(), y() - dy + Rowheight * i, w() - model->MenuConfig.HscollWidth, Rowheight,m->labelalign());	//FL_ALIGN_CENTER
				}
				if (!RGBAimage)					//绘制边框
				{
					fl_line_style(0, 0);
					fl_color(active_r() ? FrameColor : fl_inactive(FrameColor));
					fl_rect(x(), y(), w() - model->MenuConfig.HscollWidth, h());
				}
			}
			fl_pop_clip();
			
			//绘制滚动条
			Fl::scrollbar_size(model->MenuConfig.HscollWidth);
			MenuScrollbar->resize(x() + w() - model->MenuConfig.HscollWidth, y(), model->MenuConfig.HscollWidth, h());
			MenuScrollbar->type(FL_VERTICAL);
			MenuScrollbar->callback(Listscroll_cb, (void*)this);
			MenuScrollbar->bounds(0, h());
			float hscrolltab = (float)h() / (float)(Rowheight * MenuNum);
			MenuScrollbar->slider_size(hscrolltab);
			if (active())
				MenuScrollbar->activate();
			else
				MenuScrollbar->deactivate();

			fl_draw_box(MenuScrollbar->box(), x() + w() - model->MenuConfig.HscollWidth, y(), model->MenuConfig.HscollWidth, h(), MenuScrollbar->color());
			fl_draw_box(MenuScrollbar->slider(), x() + w() - model->MenuConfig.HscollWidth, y(), model->MenuConfig.HscollWidth, (int)(MenuScrollbar->slider_size()*h() + .5), FL_GRAY);
			//Fl::scheme("gtk+");
		}
	}

	const DropMenuItem* DropMenuView::popup() {
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		const DropMenuItem* m;
		pressed_menu_button_ = this;
		redraw();
		Fl_Widget_Tracker mb(this);
		if (model->MenuConfig.PopMode == 0)		//向下拉
			m = menu()->pulldown(x(), y(), w(), h(),
				model->MenuConfig.Height - h(), model->MenuConfig.HscollWidth,0, this);
		else
			m = menu()->pulldown(x(), y(), w(), h() - model->MenuConfig.Height,
				model->MenuConfig.Height - h(), model->MenuConfig.HscollWidth, 0,this);
		picked(m);
		pressed_menu_button_ = 0;
		if (mb.exists()) redraw();
		return m;
	}

	bool DropMenuView::HandleOperatePerm()
	{
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		shared_ptr<DropMenuControl> ctrl = BaseView.GetControl<DropMenuControl>();
		if ((model->MenuConfig.Perm.RequireRegion != 0)
			&& (!UI::PermUtility::HasUserPerm(model->MenuConfig.Perm.RequireRegion)))		//用户设置了安全权限并且没有权限
		{
			if (model->MenuConfig.Perm.IsHideElement)			//隐藏原件
			{
				model->MenuConfig.Perm.HasShowPerm = false;
				hide();
				redraw();
				return false;
			}
			if (model->MenuConfig.Perm.IsPopwin)				//弹出提示窗口
			{
				ctrl->Win()->OpenDialogPage(SafeTipsWinNum);
				return false;
			}
		}
		return true;
	}
	void DropMenuView::SetWriteData()
	{
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		shared_ptr<DropMenuControl> ctrl = BaseView.GetControl<DropMenuControl>();
		SelectNum = CurrentIndex;
		DropMenuItem* menu = menu_->next(SelectNum);		//通过索引寻找内容
		if(!menu)
			return;
		//labelalign_ = menu->labelalign();
		//labelcolor_ = menu->labelcolor();
		//labelfont_ = menu->labelfont();
		//labelsize_ = menu->labelsize();
		//labeltext_ = menu->text;
		if (model->MenuConfig.ContentMode && model->MenuConfig.HMICommandItem.Cmd == "GetUserList")		//命令模式
			LocalData::SetString(SYS_PSW_LOGINName, menu->text.c_str());		//设置登陆用户名
		else
		{
			if (model->MenuConfig.IsRecord)
				OperatorGControl::Ins()->AddOperatorRecord(ctrl->Page()->Winno(), model->MenuConfig.CtrlName, Storage::OA_SET,
					model->MenuConfig.ReadVar, UIData::Number<double>(model->MenuConfig.ReadVar), model->MenuConfig.MenuInfo[SelectNum].Value);
			UI::UIData::Number(model->MenuConfig.ReadVar, model->MenuConfig.MenuInfo[SelectNum].Value);
		}
		PermUtility::QuitLimitPerm(model->MenuConfig.Perm);

		//if (model->MenuConfig.MenuMode == 0)
		//{

		//	if (model->MenuConfig.ContentMode && model->MenuConfig.HMICommandItem.Cmd == "GetUserList")		//命令模式
		//		LocalData::SetString(SYS_PSW_LOGINName, menu->text.c_str());		//设置登陆用户名
		//	else
		//		UI::UIData::Number(model->MenuConfig.ReadVar, model->MenuConfig.MenuInfo[CurrentIndex].Value);
		//}
	}

	bool DropMenuView::HandleOperateConfirm()
	{
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		shared_ptr<DropMenuControl> ctrl = BaseView.GetControl<DropMenuControl>();
		if (!PermUtility::HandleConfirmPerm(model->MenuConfig.Perm, ctrl->CtrlId()))
		{
			redraw();
			return false;
		}
		else
		{
			SetWriteData();
			return true;
		}
	}
	
	int DropMenuView::handle(int e) {
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();

		if (!menu() || menu()->text.empty())
			return 0;
		switch (e) {
		case FL_ENTER: /* FALLTHROUGH */
		case FL_LEAVE:
			return (box() && !type()) ? 1 : 0;
		case FL_MOVE:
		{
			if (model->MenuConfig.MenuMode)
			{
				int mx = Fl::event_x();
				int my = Fl::event_y();
				if (mx >= x() && mx <= x() + w() - model->MenuConfig.HscollWidth && my >= y() && my <= y() + h())
				{
					SelectNum = (my - y()) / Rowheight + StartNum;
					redraw();
				}
				return Fl_Widget::handle(e);
			}
			return 0;
		}
		case FL_PUSH:
		{
			SysSetApi::TriggerBeep();
			char dataflag = DataApi::GetDataFlag(model->MenuConfig.ReadVar);
			if (dataflag == 0)		return 1;
			if (HandleOperatePerm())
			{
				if (model->MenuConfig.MenuMode == 0)
				{
					if (Fl::visible_focus()) Fl::focus(this);
					const DropMenuItem* m1 = popup();
					if (m1)		//将字体颜色设置为所选内容
					{
						CurrentIndex = find_index(m1->text.c_str());
						//设置寄存器
						if ((size_t)CurrentIndex < model->MenuConfig.MenuInfo.size())
							HandleOperateConfirm();
					}
				}
				else
				{
					int mx = Fl::event_x();
					int my = Fl::event_y();
					if (mx >= x() && mx <= x() + w() - model->MenuConfig.HscollWidth && my >= y() && my <= y() + h())
					{
						int dy = ((Rowheight * MenuNum) - h()) * MenuScrollvalue / h();
						int a = y() - dy + (StartNum) * Rowheight;
						CurrentIndex = (my - (y() - dy + (StartNum ) * Rowheight)) / Rowheight + StartNum;
						if ((size_t)CurrentIndex < model->MenuConfig.MenuInfo.size())		//命令模式
						{
							if (HandleOperateConfirm())
								return Fl_Widget::handle(e);
						}
					}
				}
			}
			return 1;

			//if (model->MenuConfig.MenuMode == 0)
			//{
			//	//if (!box()) {
			//	//	if (Fl::event_button() != 3) return 0;
			//	//}
			//	//else if (type()) {
			//	//	if (!(type() & (1 << (Fl::event_button() - 1)))) return 0;
			//	//}
			//	if (Fl::visible_focus()) Fl::focus(this);
			//	const DropMenuItem* m1 = popup();
			//	if (m1)		//将字体颜色设置为所选内容
			//	{
			//		labelalign_ = m1->labelalign();
			//		labelcolor_ = m1->labelcolor();
			//		labelfont_ = m1->labelfont();
			//		labelsize_ = m1->labelsize();
			//		labeltext_ = m1->text;
			//		
			//		CurrentIndex = find_index(m1->text.c_str());
			//		//设置寄存器
			//		if(CurrentIndex < model->MenuConfig.MenuInfo.size())
			//			UI::UIData::Number(model->MenuConfig.ReadVar, model->MenuConfig.MenuInfo[CurrentIndex].Value);
			//		if (model->MenuConfig.ContentMode && model->MenuConfig.HMICommandItem.Cmd == "GetUserList")		//命令模式
			//			LocalData::SetString(SYS_PSW_LOGINName, m1->text.c_str());		//设置登陆用户名
			//		redraw();

			//	}
			//	return 1;
			//}
			//else
			//{
			//	int mx = Fl::event_x();
			//	int my = Fl::event_y();
			//	if (mx >= x() && mx <= x() + w() - model->MenuConfig.HscollWidth && my >= y() && my <= y() + h())
			//	{
			//		SelectNum = (my - y()) / Rowheight + StartNum;
			//		CurrentIndex = SelectNum;
			//		if(CurrentIndex < model->MenuConfig.MenuInfo.size())
			//			UI::UIData::Number(model->MenuConfig.ReadVar, model->MenuConfig.MenuInfo[CurrentIndex].Value);
			//		redraw();
			//	}
			//	return Fl_Widget::handle(e);
			//}
		}
		case FL_KEYBOARD:
			if (!box()) return 0;
			if (Fl::event_key() == ' ' &&
				!(Fl::event_state() & (FL_SHIFT | FL_CTRL | FL_ALT | FL_META))) {
				popup();
				return 1;
			}
			else return 0;
		case FL_FOCUS: /* FALLTHROUGH */
		case FL_UNFOCUS:
			if (box() && Fl::visible_focus()) {
				redraw();
				return 1;
			}
		default:
			return 0;
		}
	}

	void DropMenuView::InitDraw()
	{
		shared_ptr<DropMenuModel> model = BaseView.GetModel<DropMenuModel>();
		if (model->MenuConfig.MenuNum == 0)
			return;
		
		if (model->MenuConfig.ContentMode == 1)		//命令编辑模式
		{
			vector<string> data;
			GetCommandDatas[model->MenuConfig.HMICommandItem.Cmd](model->MenuConfig.HMICommandItem.Name, data);
			MenuNum = data.size();
			menu_ = new UI::DropMenuItem[MenuNum +1];
			for (size_t i = 0; i < data.size(); i++)
			{
				//string text = data[i];
				UI::IResourceService::GB2312toUtf8(data[i]);
				menu_[i].label(data[i].c_str());
				if (!model->MenuConfig.MenuStyle.empty())
				{
					menu_[i].labelalign(model->MenuConfig.MenuStyle[0].Align);			//设置下拉条目对齐方式
					menu_[i].labelcolor(fl_rgb_color(RGBColor(model->MenuConfig.MenuStyle[0].Colors)));			//设置下拉条目字体颜色
					menu_[i].labelfont(UI::IResourceService::GetFontIdx(model->MenuConfig.MenuStyle[0].Font.Name));			//设置下拉条目字体
					menu_[i].labelsize(model->MenuConfig.MenuStyle[0].Font.Size);			//设置下拉条目字体大小
					menu_[i].labeltype(FL_NORMAL_LABEL);
					menu_[i].flags = 0;
					menu_[i].callback_ = 0;
					labelmaxsize_ = menu_[0].labelsize();
					labelmaxfont_ = menu_[0].labelfont();
				}
			}
		}
		else
		{
			MenuNum = model->MenuConfig.MenuNum;
			menu_ = new UI::DropMenuItem[model->MenuConfig.MenuNum + 1];
			for (size_t i = 0; i < (size_t)model->MenuConfig.MenuNum; i++)
			{
				menu_[i].labelalign(model->MenuConfig.MenuStyle[i].Align);			//设置下拉条目对齐方式
				menu_[i].labelcolor(fl_rgb_color(RGBColor(model->MenuConfig.MenuStyle[i].Colors)));			//设置下拉条目字体颜色
				menu_[i].labelfont(UI::IResourceService::GetFontIdx(model->MenuConfig.MenuStyle[i].Font.Name));			//设置下拉条目字体
				menu_[i].labelsize(model->MenuConfig.MenuStyle[i].Font.Size);			//设置下拉条目字体大小
				menu_[i].labeltype(FL_NORMAL_LABEL);
				menu_[i].flags = 0;
				menu_[i].callback_ = 0;
				if (model->MenuConfig.MenuStyle[i].Font.Size > labelmaxsize_)
				{
					labelmaxsize_ = menu_[i].labelsize();
					labelmaxfont_ = menu_[i].labelfont();
				}
				string text = StringUtility::GetDrawString(IResourceService::Ins(),
					model->MenuConfig.MenuInfo[i].Content, 0);
				UI::IResourceService::GB2312toUtf8(text);
				menu_[i].label(text.c_str());		//设置下拉条目内容
			}
		}
		menu_[MenuNum] = { "", 0 };
		//menu(menuinfo.get());
		selection_color(fl_rgb_color(RGBColor(model->MenuConfig.SelectColor)));	//设置选中颜色
		labelalign_ = menu_[0].labelalign();
		labelcolor_ = menu_[0].labelcolor();
		labelfont_ = menu_[0].labelfont();
		labelsize_ = menu_[0].labelsize();
		labeltext_ = menu_[0].text;
		//align(menu_[0].labelalign());
		//labelcolor(menu_[0].labelcolor());
		//labelfont(menu_[0].labelfont());
		//labelsize(menu_[0].labelsize());
		//label(menu()->text.c_str());

		//计算行高
		fl_font(labelmaxfont_, labelmaxsize_);
		//int height, width;
		//fl_measure(label(), width, height);

		bgcolor_ = fl_rgb_color(RGBColor(model->MenuConfig.BgColor));

		FrameColor = fl_rgb_color(RGBColor(model->MenuConfig.FrameColor));
		//PicKey = model->MenuConfig.PicKey;
	}
	void DropMenuView::show()
	{
		MenuScrollbar->show();
		Fl_Widget::show();
	}
	void DropMenuView::hide()
	{
		MenuScrollbar->hide();
		Fl_Widget::hide();
	}
	DropMenuView::DropMenuView(int X, int Y, int W, int H, const char *l)
		: DropMenuUnit(X, Y, W, H, l) {
		MenuScrollbar = new Fl_Scrollbar(0, 0, 0, 0);
		down_box(FL_NO_BOX);
	}
}

/*******************************************************************************
 * Copyright (C) 2020-, Xinje Electronic Co., Ltd.
 * All rights reserved.
 *
 * Name		: HMIWindow.cs
 * Author   : zhanglinbo
 * Date     : 2020/08/11
 * Descript	: UI窗体相关操作，该窗体通过消息进行事件传递
 * Version	: 1.0
 * modify	:
 *			:
 *******************************************************************************/
#define FL_INTERNALS
#ifdef WIN32
#include <FL/Fl.H>
#include <FL/x.H>
#include "BaseControl.h"
#include "HMIWindow.h"
#include "Message.h"
#include "CtrlID.h"
#include "UIManager.h"
#include "IPageFactory.h"
#include "IControlFactory.h"
#include "RunEnv.h"
#include "PermUtility.h"
#else
#include "BaseControl.h"
#include <FL/Fl.H>
#include <FL/x.H>
#include "HMIWindow.h"
#include "Message.h"
#include "CtrlID.h"
#include "UIManager.h"
#include "IPageFactory.h"
#include "IControlFactory.h"
#include "RunEnv.h"
#include "PermUtility.h"
#endif // WIN32


#ifdef __linux
#include <X11/extensions/XTest.h>
#endif
#include "UIData.h"
#include "SysSetHandle.h"
#include "System.h"
unsigned long long ScreenSaverClickTime = 0;
namespace UI
{
	/*******************************************************************************
	 * Name	    : SetMessage
	 * Descript : 消息的转换，把FLTK的消息转为HMI的自定义消息
	 * Input	: msg - 被设置的HMI消息
	 * Output   : none
	 * Note	    :
	 *******************************************************************************/
	void HMIWindow::GetHMIMessage(Message::Msg& msg)
	{
#if defined(WIN32)
		msg.Code = fl_msg.message;
		msg.Hwnd = (HWND)fl_msg.hwnd;
		msg.WParam = (WPARAM)fl_msg.wParam;
		msg.LParam = (LPARAM)fl_msg.lParam;
		msg.Data = (INT32)fl_msg.time;
#else
		msg.Code = fl_xevent->xclient.message_type;
		msg.Hwnd = fl_xevent->xany.window;
		msg.WParam = fl_xevent->xclient.data.l[0];
		msg.LParam = fl_xevent->xclient.data.l[1];
		msg.Data = fl_xevent->xclient.data.l[2];
#endif
	}
//#define WPARAM_PRED_MARK (-1)
//	static void MarkMsgProcessed()
//	{
//		WPARAM val = WPARAM_PRED_MARK;
//#if defined(WIN32)
//		fl_msg.wParam = val;
//#else
//		//fl_xevent->xclient.data.l[0] = val;
//#endif
//
//	}
	static int MsgCode()
	{
#if defined(WIN32)
		return fl_msg.message;
#else
		return fl_xevent->xclient.message_type;
#endif
	}
//	static bool IsMsgProcessed()
//	{
//		WPARAM wparam;
//#if defined(WIN32)
//		wparam = (INT32)fl_msg.wParam;
//#else
//		wparam = fl_xevent->xclient.data.l[0];
//#endif
//		return wparam == (WPARAM)WPARAM_PRED_MARK;
//	}
	/*******************************************************************************
	 * Name	    : Event_Dispatch
	 * Descript : 实现消息分发的作用，分发前作一些消息的处理工作
	 * Input	: event - 事件的类型，FLTK里使用的
	 *            w     - 发生该事件的窗体，FLTK里使用
	 * Output   : int - 0:消息未被处理
	 *				  - =!0:消息已被处理
	 * Note	    :
	 *******************************************************************************/
	int HMIWindow::Event_Dispatch(int event, HMIBaseWindow *w)
	{
		switch (event)
		{
		case FL_PUSH:
		case FL_MOVE:
		case FL_DRAG:
		case FL_KEYUP:
		case FL_KEYBOARD:
		case FL_MOUSEWHEEL:
		{
			if (SysSetHandle::CheckScreenSaver())
				return 1;
			ScreenSaverClickTime = System::GetCurrentTimeStampMs();			//记录时间
			break;
		}
		default:
			break;
		}
		

		switch (event)
		{
		case FL_CLOSE:
			UIManager::Ins().Exit();
			break;
		default:
#if 0
			Message::Msg msg;
			/*if (w == nullptr)return Fl::handle_(event, w);
			HMIWindow* win = (HMIWindow*)w;*/
			GetHMIMessage(msg);
#if false
			if (msg.Code >= WM_EVENT_BASE)
			{
				HMIWindow* win = (HMIWindow*)fl_find(msg.Hwnd);
				if (!win)return 1;

			}
#else
			if (msg.Code >= WM_EVENT_BASE)
			{
				HMIWindow* win = (HMIWindow*)fl_find(msg.Hwnd);
				if (!win)
					return 1;
				if (msg.Code == WM_EVENT_INVOKE)
				{
					std::unique_ptr<Message::InvokeCBInfo> info(reinterpret_cast<Message::InvokeCBInfo*>(msg.LParam));
					if (info != nullptr)
					{
						info->Func(info->Param);
					}
					return 1;
				}
				else
				{
					int ret = 1;
					std::shared_ptr<BaseControl> ctrl = win->GetCtrlById((Project::WinCtrlID)msg.WParam);
					if (MESSAGE_IS_AUTOFREE(msg.Code))
					{
						std::unique_ptr<Message::_AutoFreeMsg>
							autoMsg(reinterpret_cast<Message::_AutoFreeMsg*>(msg.LParam));
						msg.LParam = (LPARAM)(autoMsg->Ptr);
						if (ctrl != nullptr)
							ret = ctrl->PeekHMIMessage(&msg);
						autoMsg->Delete(autoMsg->Ptr);
						//MarkMsgProcessed();
					}
					else
					{
						if (ctrl != nullptr)
							ret = ctrl->PeekHMIMessage(&msg);

					}
					return ret;

				}
			}
#endif
#endif
			break;
		}


		return Fl::handle_(event, w);

	}
	HMIWindow::HMIWindow(int x, int y, int w, int h, const char* title) : HMIDoubleWindow(x, y, w, h, title)
	{
		Fl::event_dispatch(Event_Dispatch);
	}
	void HMIWindow::draw()
	{
		//Fl_Double_Window::draw();
		if (!curPage_)return;
		HMIPage* tp = curPage_;
		while (tp)
		{
			tp->draw();
			tp = tp->Next;
			if (tp == curPage_)
				break;

		}
	}
	void HMIWindow::Invoke(Message::InvokeCBFunc func, void*param, int winno)
	{
		if (UIManager::Ins().IsInUIThread())
		{
			func(param);
			return;
		}
		std::unique_ptr<Message::InvokeCBInfo> info(new Message::InvokeCBInfo());
		info->Func = func;
		info->Param = param;
		Message::InvokeCBInfo* p = info.release();
		if (!SendUserMessage(0, WM_EVENT_INVOKE, reinterpret_cast<LPARAM>(p)))
			delete p;

	}
	/*******************************************************************************
	 * Name	    : SendUserMessageEx
	 * Descript : 发送消息
	 * Input	: hwnd - 控件的句柄，窗体句柄为this，在函数内容部使用了
	 *            message - 消息类别
	 *			  lparam  - 附带的参数
	 * Output   : int -   0:调用失败
	 *				  - =!0:调用成功
	 * Note	    :
	 *******************************************************************************/
	int HMIWindow::SendUserMessageEx(void* hwnd, int message, int lparam)
	{
		if (this == nullptr)
			return 0;
		Fl_X* flx = Fl_X::i((Fl_Window *)this);
		/*if (flx == nullptr)
			return 0;*/
		//return Message::Msg::SendMsg((HWND)flx->xid, (HWND)hwnd, (int)message, (LPARAM)lparam, 0);
		return UIManager::Ins().SendMsg((HWND)flx->xid, (int)message, (WPARAM)hwnd, (LPARAM)lparam, 0);
	}

	/*******************************************************************************
	 * Name	    : SendUserMessage
	 * Descript : 发送消息
	 * Input	: hwnd - 控件的句柄，窗体句柄为this，在函数内容部使用了
	 *            message - 消息类别
	 *			  lparam  - 附带的参数
	 * Output   : int -   0:调用失败
	 *				  - =!0:调用成功
	 * Note	    :
	 *******************************************************************************/
	int HMIWindow::SendUserMessage(void* hwnd, int message, int lparam)
	{
		return SendUserMessageEx(hwnd, message, lparam); 
	}

	
#ifdef WIN32

	int ConvertToKeySm(int message)
	{
		return message;
	}
	int HMIWindow::SendKey(int message)
	{
		Fl_X* flx = Fl_X::i((Fl_Window *)this);
		int v = ConvertToKeySm(message);
		//PostMessage((HWND)flx->xid, WM_KEYDOWN, v, 0);
		//WM_SYSCHAR
		if(v == VK_CAPITAL)
			return PostMessage((HWND)flx->xid, WM_KEYDOWN, v, 0x3A0001);
		return PostMessage((HWND)flx->xid, WM_KEYDOWN, v, 0x14B0001);
	}
#elif defined(__APPLE__)
#else
	static struct {
		unsigned char asic;
		unsigned short key;
	}    keymaps[] = {
		{0x0D,0xFF0D},  //enter
		{0x08,0xFF08}, //backsapce
		{0x14,0xFFE5}, //Caps Lock
		{0x10,0xFFE1}, //shift Lock
		{0x1B,0xFF1B}, //esc
		{0x20,0xff20}, //Space
		{0x5B,0x005b}, //[
		{0x5D,0x005d}, //]
		{0x5C,0x005c}, //'\'
		{0x6C,0x002c}, //,
		{0x6E,0xFFaE}, //.
		{0x6F,0xffaf}, //'/'
		{0x6B,0xffab}, //+
		{0x6D,0xffad}, //-
		{0x6A,0xffaa}, //*

	};
	int ConvertToKeySm(int message)
	{
		int i;
		for(i=0;i<sizeof(keymaps)/sizeof(keymaps[0]);i++)
		{
			if(message == keymaps[i].asic)
			{
				message = keymaps[i].key;
				break;
			}
  				
		}
		 
		return message;
	}

	int HMIWindow::SendKey(int message)
	{
		Display* disp = XOpenDisplay(NULL);
		int v = ConvertToKeySm(message);
		XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ConvertToKeySm(v)), True, CurrentTime);
		XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, ConvertToKeySm(v)), False, CurrentTime);
		XCloseDisplay(disp);
	}


#endif






	

	void HMIWindow::AddTimeout(double time, TimeoutHandler handler, void* param)
	{
		Fl::add_timeout(time, handler, param);
	}
	void HMIWindow::RemoveTimeout(TimeoutHandler handler, void* param)
	{
		Fl::remove_timeout(handler, param);
	}
	std::shared_ptr<BaseControl> HMIWindow::GetCtrlById(Project::WinCtrlID ctrid)
	{
		HMIPage* page = HMIWindow::FindPage(Project::GetWinNo(ctrid));
		if (page)
		{
			return page->FindCtrl(Project::GetCtrlNo(ctrid));
		}
		return nullptr;

	}

	HMIPage* HMIWindow::FindPage(int winno, bool iscontaintmp)
	{
		if (winno == 0)
			return gPage_;
		HMIPage* tp = curPage_;
	
		do
		{
			if (!tp)break;
			HMIPage* p = iscontaintmp?tp->FindPage(winno):(tp->Winno()==winno?tp:nullptr);
			if (p)
				return p;
			tp = tp->Next;
		} while (tp != curPage_);
		return nullptr;
	}
	bool HMIWindow::PageHasPrem(int winno, bool popwin)
	{
		Project::PageInfo& pi = prj_->Pages.Screens[winno];
		if (!UI::PermUtility::HasUserPerm(pi.PageCfg.UserLimit) && winno != prj_->Setting.StartWinNo)		//切换页面没权限
		{
			if (popwin)
				OpenDialogPage(SafeLoginWinNum);		//弹出登陆页面
			return false;
		}
		return true;
	}

	HMIPage* HMIWindow::ProducePage(int winno, int offx, int offy)
	{
		HMIPage* pg;
		pg = FindPage(winno);
		if(pg)  //如果当前page被使用的话，就直接返回空。同一个页面不能同时出现
			return nullptr;

		if (prj_->Pages.Screens.find(winno) == prj_->Pages.Screens.end())
			return nullptr;
		Project::PageInfo& pi = prj_->Pages.Screens[winno];
//		if (!UI::PermUtility::HasUserPerm(pi.PageCfg.UserLimit) && winno != prj_->Setting.StartWinNo)		//切换页面没权限
//			return nullptr;
		pg = IPageFactory::Ins()->NewPage(pi.Catogray, pi.Id, pi.Pos.X, pi.Pos.Y, pi.Sz.W, pi.Sz.H).release();
		//pg = new HMIPage(pi.Id, pi.Pos.X, pi.Pos.Y, pi.Sz.W, pi.Sz.H/*, pi.Name.c_str()*/);
		pg->Win(this);
		pg->color(fl_rgb_color(RGBColor(pi.WinColor)));
		if (pi.PageCfg.IsMonopoly)		//垄断
			pg->SetModal(true);
		//构建并添加底部page
		if (pi.Template.BottomWinId > 0 && pi.Template.TopWinId != winno)
			pg->TemplateBottomPage(ProducePage(pi.Template.BottomWinId));

		pg->ProduceCtrl(pi.Shapes, winno);
		if(offx != 0 || offy != 0)
			pg->ChangePos(pi.Pos.X + offx, pi.Pos.Y + offy);

		//构建并添加顶部page
		if (pi.Template.TopWinId > 0 && pi.Template.TopWinId != winno)
			pg->TemplateTopPage(ProducePage(pi.Template.TopWinId));

		return pg;
	}



	static void InitListNodeHead(HMIPage* head, HMIPage*& cur)
	{
		head->Next = head;
		head->Prev = head;
		cur = head;
	}
	static void RemoveListNode(HMIPage* page)
	{
		page->Next->Prev = page->Prev;
		page->Prev->Next = page->Next;
	}
	static void AddListNode(HMIPage* head, HMIPage* page)
	{
		page->Next = head;
		page->Prev = head->Prev;
		head->Prev->Next = page;
		head->Prev = page;
	}
	static bool IsParentPage(HMIPage* page, HMIPage* prtpg)
	{
		HMIPage* pg = page->Parent;
		while (pg)
		{
			if (pg == prtpg)
				return true;
			pg = pg->Parent;
		}
		return false;
	}
	void HMIWindow::ShowPage(HMIPage* page, HMIPage* ppage)
	{
		AddListNode(curPage_, page);
		if (ppage)
			page->Parent = ppage;
		else
			page->Parent = curPage_;
		ShowPage(page);
	}
	void HMIWindow::ShowPage(HMIPage* page)
	{
		page->Open();
		SysSetHandle::ReportCurrentWinno(page->Winno());
		/*add(page);
		page->visible();
		page->show();
		page->redraw();*/
		page->Display(this);

	}
	//返回上一页面
	int HMIWindow::GobackPage(int offx, int offy)
	{
		int prewin = curPage_->LastWinno();
		if (!prewin)return 0;
		HMIPage* pg = ProducePage(prewin, offx, offy);
		HMIPage *perpg = curPage_;

		if (pg)
		{
			pg->LastWinno(curPage_->Winno());
			InitListNodeHead(pg, curPage_);
			CloseOnePage(perpg);
			ShowPage(curPage_);
			return 0;
		}
		return -1;
	}

	int HMIWindow::SwitchStartPage(int offx, int offy)
	{
		SwitchPage(prj_->Setting.StartWinNo);
		return prj_->Setting.StartWinNo;
	}

	int HMIWindow::SwitchPage(int page, int offx, int offy)
	{
		if (curPage_->Winno() == page)return 0;
		HMIPage* pg = ProducePage(page, offx, offy);
		// 关闭之前的页面
		CloseAllDialog();
		HMIPage *perpg = curPage_;
		if (pg)
		{
			pg->LastWinno(curPage_->Winno());
			InitListNodeHead(pg, curPage_);
			CloseOnePage(perpg);
			ShowPage(curPage_);
			return 0;
		}
		return -1;
	}

	//打开弹出窗体
	int HMIWindow::OpenDialogPage(int page, HMIPage* p, int offx, int offy)
	{
		HMIPage* pg = FindPage(page);
		if (pg)return 0;
		pg = ProducePage(page, offx, offy);
		if (!pg)return -1;


		ShowPage(pg, p);
		return 0;
	}
	//切换弹出窗体
	int HMIWindow::SwitchDialogPage(int page, HMIPage* p, int offx, int offy)
	{
		HMIPage* pg = FindPage(page);
		if (pg)return 0;
		pg = ProducePage(page, offx, offy);
		if (!pg)return -1;
		//找出最新的窗体，进行切换
		HMIPage* tmp = curPage_->Prev;
		while (tmp && tmp != curPage_)
		{
			if (tmp->Parent == p)
			{
				pg->LastWinno(tmp->Winno());
				CloseAllDialog(tmp);
				CloseOnePage(tmp);
				break;
			}
			tmp = tmp->Prev;
		}

		ShowPage(pg, p);
		return 0;
	}
	void HMIWindow::DeleteOnePage(HMIPage* page)
	{
		if (prj_->Pages.Screens.find(page->Winno()) != prj_->Pages.Screens.end())
		{
			Project::PageInfo& pi = prj_->Pages.Screens[page->Winno()];
			if (pi.PageCfg.IsChangeToLimit)
				pi.PageCfg.UserLimit = pi.PageCfg.ChangeToLimit;
		}
		page->Close();
		/*page->hide();
		remove(page);*/
		page->Remove(this);
		delete page;
	}

	int HMIWindow::ClosePage(int page)
	{
		HMIPage* pg = FindPage(page, false);
		if (!pg)return 0;
		CloseAllDialog(pg);
		CloseOnePage(page);
		return 0;
	}
	//关闭弹出窗体
	int HMIWindow::CloseOnePage(int page)
	{
		HMIPage* pg = FindPage(page,false);
		return CloseOnePage(pg);
	}
	int HMIWindow::CloseOnePage(HMIPage*page)
	{
		if (page == curPage_)return 0;
		RemoveListNode(page);
		DeleteOnePage(page);
		return 0;
	}
	//关闭所有窗体
	int HMIWindow::CloseAllDialog(HMIPage* page)
	{
		HMIPage* pg = curPage_->Next;
		while (pg != curPage_)
		{
			if (IsParentPage(pg, page))
			{
				HMIPage* tmp = pg->Next;
				RemoveListNode(pg);
				DeleteOnePage(pg);
				pg = tmp;
			}
			else
				pg = pg->Next;
		}
		return 0;
	}
	int HMIWindow::CloseAllDialog()
	{
		HMIPage* pg = curPage_->Next;
		while (pg != curPage_)
		{
			HMIPage* tp = pg->Next;
			CloseOnePage(pg);
			pg = tp;
		}
		return 0;
	}
	void HMIWindow::AppStart(int offx, int offy)
	{
		if (RunEnv::HMIApp.Event.OnAppStart)
			RunEnv::HMIApp.Event.OnAppStart();

		//创建0窗体
		gPage_ = (GHMIPage*)ProducePage(0);
		ShowPage(gPage_);
		//创建启动窗体
		HMIPage* tp = ProducePage(prj_->Setting.StartWinNo, offx, offy);
		if (tp)
		{
			InitListNodeHead(tp, curPage_);
			ShowPage(curPage_);
		}
		else
		{
			//error

		}
	}


	/*******************************************************************************
	 * Name	    : Update
	 * Descript : 消息的处理，通过VID进行通知
	 * Input	: msg - 被设置的HMI消息
	 * Output   : none
	 * Note	    :
	 *******************************************************************************/
	void HMIWindow::Update(Project::DataVarId id)
	{
		Project::WinCtrlID ctrlid = prj_->Vars.GetCtrlID(id);
		if (ctrlid == Project::WinCtrlIDEmpty)return;
		int winno = Project::GetWinNo(ctrlid);
		if (winno == 0)
		{
			gPage_->HandleDataNotify(Project::GetCtrlNo(ctrlid), id);
		}
		else
		{
			/*HMIPage* pg = FindPage(winno);
			if (pg)*/
			{
				//if (!pg->QueueRunFlag)
				{
					SendUserMessage((void*)ctrlid, WM_EVENT_DATEUPDATE, id.Vid);
				}

			}

		}
	}

	void HMIWindow::NotifySysChange(SysChangeEM ctg)
	{
		HMIPage* pg = curPage_->Prev;
		while (pg != curPage_)
		{
			HMIPage* tp = pg->Prev;
			if (!tp->NotifySysChange(ctg))
				tp = curPage_->Prev;
				//return;
			pg = tp;
		}
		pg->NotifySysChange(ctg);
	}
}

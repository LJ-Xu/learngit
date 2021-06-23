#include "stdafx.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include "Message.h"
#include "IControlFactory.h"
#include "IResourceService.h"
#include "BaseControl.h"
#include "HMIProject.h"
#include "HMIWindow.h"
#include "GlobalWindow.h"
#include "UIManager.h"
#include "System.h"
#if defined(WIN32)
extern MSG fl_msg;
#else
extern const XEvent* fl_xevent;
#endif
namespace UI
{
	UIManager& UIManager::Ins()
	{
		static UIManager mgr;
		return mgr;
	}
#if 0
	/*******************************************************************************
	 * Name	    : SetMessage
	 * Descript : 消息的转换，把FLTK的消息转为HMI的自定义消息
	 * Input	: msg - 被设置的HMI消息
	 * Output   : none
	 * Note	    :
	 *******************************************************************************/
	void UIManager::GetHMIMessage(Message::Msg& msg)
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
#define WPARAM_PRED_MARK (-1)
	static void MarkMsgProcessed()
	{
		WPARAM val = WPARAM_PRED_MARK;
#if defined(WIN32)
		fl_msg.wParam = val;
#else
		fl_xevent->xclient.data.l[0] = val;
#endif

	}
	static int MsgCode()
	{
#if defined(WIN32)
		return fl_msg.message;
#else
		return fl_xevent->xclient.message_type;
#endif
	}
	static bool IsMsgProcessed()
	{
		WPARAM wparam;
#if defined(WIN32)
		wparam = (INT32)fl_msg.wParam;
#else
		wparam = fl_xevent->xclient.data.l[0];
#endif
		return wparam == (WPARAM)WPARAM_PRED_MARK;
	}
	/*******************************************************************************
	 * Name	    : Event_Dispatch
	 * Descript : 实现消息分发的作用，分发前作一些消息的处理工作
	 * Input	: event - 事件的类型，FLTK里使用的
	 *            w     - 发生该事件的窗体，FLTK里使用
	 * Output   : int - 0:消息未被处理
	 *				  - =!0:消息已被处理
	 * Note	    :
	 *******************************************************************************/

	int UIManager::FlMsgInterpCheck(void *event, void *data)
	{
		Message::Msg msg;
		if (MsgCode() < WM_EVENT_BASE)return 0;
		if (MESSAGE_IS_AUTOFREE(MsgCode()))
		{
			if (IsMsgProcessed())
				return 0;
			UIManager::GetHMIMessage(msg);
			gwin_->PeekUnHandleMessage(&msg);//需要自动释放的，但消息未被处理
		}
		//判断窗体是否存在，不存在则判断消息类型进行资源释放等
		return 0;
	}

	/********窗体管理**********/
	void UIManager::CloseWindow(int winno)
	{

	}
	HMIWindow* UIManager::OpenWindow(int winno)
	{

		return nullptr;
	}
	HMIWindow* UIManager::OpenWindow(HMIWindow* win)
	{
		return nullptr;
	}
	HMIWindow* UIManager::OpenSubWindow(int winno)
	{
		return nullptr;
	}

	void UIManager::HideWindow(int winno)
	{

	}
	bool UIManager::ExistWindow(int winno)
	{
		return true;
	}
	HMIWindow* UIManager::OpenAlarmWin(int winno)
	{
		return nullptr;
	}
	HMIWindow* UIManager::CloseAlarmWin(int winno)
	{
		return nullptr;
	}
	HMIWindow* UIManager::JumpWindow(int fromwinno, int towinno)
	{
		return nullptr;
	}
	HMIWindow* UIManager::GetWinByCtrID(int ctrid)
	{
		int winno = Project::GetWinNo(ctrid);
		return GetWin(winno);
	}
	HMIWindow* UIManager::GetWin(int winno)
	{
		HMIWindow* win = nullptr;

		return win;
	}
	/*****************服务注册*********************/

	void UIManager::DataNotifyServiceCB(int ctrid, int notifyData)
	{
		HMIWindow* win = GetWinByCtrID(ctrid);
		if (win != nullptr)
			win->SendUserMessage((void*)ctrid, WM_EVENT_DATEUPDATE, notifyData);
	}

#endif
	bool UIManager::IsInUIThread()
	{
		return threadId_ == this_thread::get_id();
	}
	void UIManager::PrepareRunEnv()
	{
		threadId_ = this_thread::get_id();
	}


	/*static GlobalWindow* gwin;
	void beepcb(Fl_Widget *, void *) {

		gwin->SendUserMessage((void*)-1, WM_EVENT_OPEN, 111);
	}*/
	static int GetCenterX(int w)
	{
		return (System::ScreenWidth() - w) / 2;
	}
	static int GetCenterY(int h)
	{
		return (System::ScreenHeight() - h) / 2;
	}
	int UIManager::ShowWindow()
	{

		curWin_ = new  HMIWindow(GetCenterX(prj_->Setting.Width),
			GetCenterY(prj_->Setting.Height),
			prj_->Setting.Width, prj_->Setting.Height);
#ifndef WIN32
		curWin_->clear_border();
#endif // linux_

		curWin_->HMIProject(prj_);
		//curWin_->resizable(curWin_);
		curWin_->show();
		curWin_->AppStart();

		//curWin_->fullscreen();
		return 0;
	}
	void ProcessMessage(Message::Msg& msg)
	{
		if (msg.Code >= WM_EVENT_BASE)
		{
			HMIWindow* win = (HMIWindow*)fl_find((HWND)msg.Hwnd);
			if (!win)
				return ;
			if (msg.Code == WM_EVENT_INVOKE)
			{
				std::unique_ptr<Message::InvokeCBInfo> info(reinterpret_cast<Message::InvokeCBInfo*>(msg.LParam));
				if (info != nullptr)
				{
					info->Func(info->Param);
				}
				return ;
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
				return;
			}
		}
	}
	int UIManager::RunUI()
	{
#if 1
		void *  next_message = nullptr;

		PrepareRunEnv();

		/* Create your windows and widgets here */
		Fl::lock(); /* "start" the FLTK lock mechanism */

		/* show your window */
		ShowWindow();

		/* Run the FLTK loop and process thread messages */
		while (Fl::wait() > 0) {
			if ((next_message = Fl::thread_message()) != NULL) {
				/* process your data, update widgets, etc. */
				Message::Msg msg;
				while(queue_.GetMsg(msg))
					ProcessMessage(msg);
			}
		}
		return 0;
#else
		PrepareRunEnv();
		ShowWindow();
		return Fl::run();
#endif
		
	}
	void UIManager::AddExit(function<void(void)> func)
	{
		funcs_.push_back(func);
	}
	void UIManager::Exit()
	{
		for (size_t i = 0; i < funcs_.size(); i++)
		{
			funcs_[i]();
		}
	}
	void UIManager::Update(Project::DataVarId id)
	{
		curWin_->Update(id);
	}
	UIManager::UIManager()
	{
		queue_.Init(1000);
	}
	bool UIManager::SendMsg(HWND hwnd, int msgcode, WPARAM wparam, LPARAM lparam, void* pdata)
	{
		bool ret = queue_.SendMsg(hwnd, msgcode, wparam, lparam, pdata);
		Fl::awake((void*)1);
		return ret;
	}
}



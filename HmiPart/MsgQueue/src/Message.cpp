#define FL_INTERNALS
#include "stdafx.h"
#include <FL/Fl.H>
#include <FL/x.H>
#include "Message.h"

#ifndef WIN32
static void fl_sendClientMessage(Window window, Atom message,
                                 unsigned long d0,
                                 unsigned long d1=0,
                                 unsigned long d2=0,
                                 unsigned long d3=0,
                                 unsigned long d4=0)
{
  XEvent e;
  e.xany.type = ClientMessage;
  e.xany.window = window;
  e.xclient.message_type = message;
  e.xclient.format = 32;
  e.xclient.data.l[0] = (long)d0;
  e.xclient.data.l[1] = (long)d1;
  e.xclient.data.l[2] = (long)d2;
  e.xclient.data.l[3] = (long)d3;
  e.xclient.data.l[4] = (long)d4;
  XSendEvent(fl_display, window, 0, 0, &e);
}

#endif // !WIN32


namespace Message
{
//	Msg* Msg::Create(int code, HWND hWnd, WPARAM wparam, LPARAM lparam)
//	{
//		Msg* msg = new Msg();
//		msg->Code = code;
//		msg->Hwnd = hWnd;
//		msg->LParam = lparam;
//		msg->WParam = wparam;
//		return msg;
//	}
//	void Msg::Dispose(Msg* msg)
//	{
//		 
//	}
//	int Msg::SendMsg(Msg& msg)
//	{
//		return 1;
//	}
//	int Msg::SendMsg(HWND winHd, HWND ctrHd, int message, LPARAM param, int exparam)
//	{
//#if 0
//		Msg* msg = Msg::Create(message, (HWND)winHd, (WPARAM)ctrHd, (LPARAM)param);
//		if (!Fl::awake(msg))
//			Msg::Dispose(msg);
// 
//		return 0;
//#else
//#if defined(WIN32)
//		return PostMessage((HWND)winHd, (UINT)message, (WPARAM)ctrHd, (LPARAM)param);
//#else
//        fl_sendClientMessage((Window)winHd, (Atom)message, ctrHd, param, exparam, 0, 0);
//        //fl_sendClientMessage((Window)0, (Atom)0, 0, 0, 0, 0, 0);                      
//        return 0;
//#endif
//#endif
//	}
}

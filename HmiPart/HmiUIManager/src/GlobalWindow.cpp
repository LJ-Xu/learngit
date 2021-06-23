#include "BaseGControl.h"
#include "GlobalWindow.h"
namespace UI
{
	GlobalWindow::GlobalWindow(int w, int h,const char* title) :HMIWindow(w, h,0, title)
	{
		ctrls_[GCtrlEnum::SAMPLE] = IControlFactory::Ins()->GetNewGControlByID(GCtrlEnum::SAMPLE);
		ctrls_[GCtrlEnum::SAMPLE]->SetHMIWindow(this);
 
	}
 
	void GlobalWindow::PeekUnHandleMessage(Message::Msg* msg)
	{
		//判断消息类型，处理需要释放内存的消息
		/*if (msg->WParam == -1)
		{
			Fl_Window *window = new Fl_Window(320, 65);
			window->show();
			return;
		}*/
		Message::_AutoFreeMsg* autoMsg = (Message::_AutoFreeMsg*)msg->LParam;
		autoMsg->Delete(autoMsg->Ptr);
	}
	///目前在DataService线程里处理
	///数据采集和报警
	void GlobalWindow::HandleDataNotify(int ctrlidx, Project::DataVarId id)
	{	
		 
	}
}
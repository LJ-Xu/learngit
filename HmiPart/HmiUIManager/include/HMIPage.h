#pragma once
#ifdef WIN32
#include <stdlib.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <memory>
#include <vector>
#include <atomic>
#include "ModalPage.h"
#include "CtrlID.h"
#include "ProjectPages.h"
#include "ListQueue.h"
#else
#include <stdlib.h>
#include <stdio.h>
#include "ProjectPages.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <memory>
#include <vector>
#include <atomic>
#include "ModalPage.h"
#include "CtrlID.h"
#include "ListQueue.h"
#endif // WIN32


namespace Project
{
	struct WindowVar;
}

namespace UI
{
	enum SysChangeEM :char
	{
		Permission,
		Language,
	};
	typedef Fl_Timeout_Handler TimeoutHandler;
	struct PageTimerParam
	{ 
		TimeoutHandler Handler;
		void* Param;
		unsigned int Repeat;//tick
		PageTimerParam* Next;
		PageTimerParam* Prev;
		PageTimerParam():Handler(nullptr), Param(nullptr), 
			Repeat(0), Next(nullptr), Prev(nullptr)
		{
 
		}
	};
	class BaseControl;
	class HMIWindow;
	class HMIPage :public Fl_Group
	{
	public:
		HMIPage(int pageid,int x, int y, int w, int h, const char* title = 0);
			
		void Init(Project::WindowVar* wvar)
		{
			wVar_ = wvar;
		}
		void draw();
		/**********************************/

	 
		/*******************属性***********************/
 
		int Winno()
		{
			return winno_;
		}
		int LastWinno()
		{
			return lastWinno_;
		}
		void LastWinno(int winno)
		{
			lastWinno_ = winno;
		}
		void ResizeCtrlBuf(int size)
		{
			ctrls_.resize(size);
		}
		void SetCtrl(int idx,std::shared_ptr<BaseControl> ctrl)
		{
			ctrls_[idx] = ctrl;
		}
		void AddCtrl(std::shared_ptr<BaseControl> ctrl)
		{
			ctrls_.push_back(ctrl);
		}
		std::shared_ptr<BaseControl> FindCtrl(int ctrlid)
		{
			if (ctrls_.size() > (size_t)ctrlid)
				return ctrls_[ctrlid];
			else
				return nullptr;
		}
		UI::HMIWindow* Win()
		{
			return win_;
		}
		void Win(UI::HMIWindow* win)
		{
			win_ = win;
		}
		//打开Page
		void Open();	
		//关闭Page
		void Close();
		//通知当前页有系统消息通知
		void NotifySysChange(SysChangeEM ctg);
		//改变坐标
		void ChangePos(int offx, int offy);

		HMIPage* FindPage(int winno);

		//显示到WIN里
		void Display(HMIWindow* win);
		//从WIN里移除
		void Remove(HMIWindow* win);
		//增加定时器
		void AddTimeout(unsigned int ms, TimeoutHandler handler, void* param, bool isrepeat=false);
		//移除定时器
		void RemoveTimeout(TimeoutHandler handler, void* param);
		static void TimeOutHandler(void* page);
		void TemplateTopPage(HMIPage* page)
		{
			topPage_ = page;
			if(page)
				add(page);
		}
		void TemplateBottomPage(HMIPage* page)
		{
			bottomPage_ = page;
			if (page)
				add(page);
		}
		/*创建ctrl*/
		void ProduceCtrl(vector<Project::UnitInfo>& shapes, int winno);
		static std::shared_ptr<BaseControl> NewCtr(Project::UnitInfo &unintInfo, UI::HMIPage*);		
		//int  PeekHMIMessage(Message::HMIMsg* msg);
		HMIPage* Next = nullptr;
		HMIPage* Prev = nullptr;
		HMIPage* Parent = nullptr;//属于哪个页面
		
		bool IsModal()
		{
			return modal_;
		}
		void SetModal(bool v)
		{
			modal_ = v;
		}
		int GetPageOffX()
		{
			return pageoffx_;
		}
		int GetPageOffY()
		{
			return pageoffy_;
		}
	public:
		std::atomic<bool> QueueRunFlag;//当前队列是否正在运行
	protected:
		std::vector<std::shared_ptr<BaseControl>> ctrls_;
	private:
		PageTimerParam timerParamHead_;
		int lastWinno_ = 0;
		int winno_ = 0;
		int pageoffx_ = 0;
		int pageoffy_ = 0;		
		//Message::ListQueue listQueue_;
		UI::HMIWindow* win_ = nullptr;
		bool modal_ = false;    
		Project::WindowVar* wVar_ = nullptr;//引用的DATAVAR相关变量
		ModalPage* modalpg_ = nullptr;
		void NotifyCtrlClose();
		HMIPage* topPage_ = nullptr;
		HMIPage* bottomPage_ = nullptr;
	private:
		//关闭Page下的所有定时器
		HMIPage* FindPageEx(int winno);
		void CloseTimerEx();
		void NotifyCtrlReadyEx();
		void OpenEx();
		void CloseEx();
		void NotifySysChangeEx(SysChangeEM ctg);
		void ChangePosEx(int offx, int offy);		
	};

 
}

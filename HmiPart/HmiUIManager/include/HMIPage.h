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

	 
		/*******************����***********************/
 
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
		//��Page
		void Open();	
		//�ر�Page
		void Close();
		//֪ͨ��ǰҳ��ϵͳ��Ϣ֪ͨ
		void NotifySysChange(SysChangeEM ctg);
		//�ı�����
		void ChangePos(int offx, int offy);

		HMIPage* FindPage(int winno);

		//��ʾ��WIN��
		void Display(HMIWindow* win);
		//��WIN���Ƴ�
		void Remove(HMIWindow* win);
		//���Ӷ�ʱ��
		void AddTimeout(unsigned int ms, TimeoutHandler handler, void* param, bool isrepeat=false);
		//�Ƴ���ʱ��
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
		/*����ctrl*/
		void ProduceCtrl(vector<Project::UnitInfo>& shapes, int winno);
		static std::shared_ptr<BaseControl> NewCtr(Project::UnitInfo &unintInfo, UI::HMIPage*);		
		//int  PeekHMIMessage(Message::HMIMsg* msg);
		HMIPage* Next = nullptr;
		HMIPage* Prev = nullptr;
		HMIPage* Parent = nullptr;//�����ĸ�ҳ��
		
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
		std::atomic<bool> QueueRunFlag;//��ǰ�����Ƿ���������
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
		Project::WindowVar* wVar_ = nullptr;//���õ�DATAVAR��ر���
		ModalPage* modalpg_ = nullptr;
		void NotifyCtrlClose();
		HMIPage* topPage_ = nullptr;
		HMIPage* bottomPage_ = nullptr;
	private:
		//�ر�Page�µ����ж�ʱ��
		HMIPage* FindPageEx(int winno);
		void CloseTimerEx();
		void NotifyCtrlReadyEx();
		void OpenEx();
		void CloseEx();
		void NotifySysChangeEx(SysChangeEM ctg);
		void ChangePosEx(int offx, int offy);		
	};

 
}

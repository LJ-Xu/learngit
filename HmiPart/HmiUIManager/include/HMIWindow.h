#pragma once
#include "stdafx.h"
#include "UIComm.h"
#include "CtrlID.h"
#include "Message.h"
#include "ProjectPortVarsInfo.h"
#include "ProjectPages.h"
#include "HMIProject.h"
#include "UIManager.h"
#include "StackArray.h"
#include "HMIPage.h"
#include "GHMIPage.h"

namespace Project
{
	struct HMIWindow;
}
namespace UI
{
	class BaseControl;
	class KeyMessage;

	class HMIWindow :public HMIDoubleWindow
	{
	public:
		/**********************类型定义***********************/
		
		/************************************************/
		HMIWindow(int x, int y, int w, int h, const char* title = 0);
		void draw();
		void Update(Project::DataVarId id);
		
		void NotifySysChange(SysChangeEM ctg);
		/**************************Page页面操作*******************************/
		void DeleteOnePage(HMIPage* page);
		void ShowPage(HMIPage* page);
		void ShowPage(HMIPage* page, HMIPage* ppage);
		void AppStart(int offx = 0, int offy = 0);

		//返回上一页面
		int GobackPage(int offx = 0, int offy = 0);
		//int GobackDialogPage(int offx = 0, int offy = 0);	
		int SwitchStartPage(int offx = 0, int offy = 0);
		//切换页面，入栈,最多N个页面入栈
		int SwitchPage(int page, int offx = 0, int offy = 0);

		int OpenKeyBoard(int page, HMIPage * p, int x, int y);

		//打开弹出窗体
		int OpenDialogPage(int page, HMIPage* p = nullptr, int offx = 0, int offy = 0);
		//切换弹出窗体

		int SwitchDialogPage(int page, HMIPage* p = nullptr, int offx = 0, int offy = 0);
		//关闭弹出窗体

		int ClosePage(int page);
		
		//关闭所有窗体
		int CloseAllDialog();
		HMIPage* FindPage(int page, bool iscontaintmp = true);

		/*****************************消息相关*************************************/
		static void GetHMIMessage(Message::Msg& msg);
		static int  Event_Dispatch(int event, HMIBaseWindow *w);
		template <class T>
		int SendAutoFreeMessage(void* hwnd, int message, T* lparam)
		{
			Message::_AutoFreeMsg* _msg = new Message::_AutoFreeMsg();
			_msg->Ptr = lparam;
			_msg->Delete = [](void* p) { delete (T*)p; };
			return SendUserMessage(hwnd, MAKE_AUTOFREE_MESSAGE(message), reinterpret_cast<LPARAM>(_msg));
		}
		///hwnd 发送的control
		///message 类型
		///lparam 参数
		int SendUserMessage(void* hwnd, int message, int lparam);
		int SendKey(int message);
		void Invoke(Message::InvokeCBFunc func, void*param, int winno = 0);

		/*********************相关属性*********************/
		//原始属性.....由FLTK实现
		/************************************************/
		typedef Fl_Timeout_Handler TimeoutHandler;
		void AddTimeout(double time, TimeoutHandler handler, void* param);		
		void RemoveTimeout(TimeoutHandler handler, void* param);
		std::shared_ptr<BaseControl> GetCtrlById(Project::WinCtrlID ctrno);
		/***********************静态方法******************************/
		void HMIProject(Project::HMIProject* prj)
		{
			prj_ = prj;
		}
		Project::HMIProject* HMIProject()
		{
			return prj_;
		}
		bool PageHasPrem(int winno, bool popwin);
		HMIPage * ProduceKeyBoardPage(int winno, int x, int y);
		HMIPage* ProducePage(int winno, int offx = 0, int offy = 0);
	protected:
		int CloseOnePage(int page);
		int CloseOnePage(HMIPage*);
		int CloseAllDialog(HMIPage*);
	private:
		Project::HMIProject* prj_ = nullptr;
		GHMIPage* gPage_ = nullptr;//全局页面,页面0
		//HMIPage* bottomPage_;//bottom
		//HMIPage* topPage_;//bottom
		HMIPage* curPage_ = nullptr;//当前页面

		int SendUserMessageEx(void* hwnd, int message, int lparam);
		//static std::shared_ptr<BaseControl> NewCtr(Project::UnitInfo &unintInfo, UI::HMIPage*);
	};
}
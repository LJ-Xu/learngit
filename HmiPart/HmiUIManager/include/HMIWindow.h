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
		/**********************���Ͷ���***********************/
		
		/************************************************/
		HMIWindow(int x, int y, int w, int h, const char* title = 0);
		void draw();
		void Update(Project::DataVarId id);
		
		void NotifySysChange(SysChangeEM ctg);
		/**************************Pageҳ�����*******************************/
		void DeleteOnePage(HMIPage* page);
		void ShowPage(HMIPage* page);
		void ShowPage(HMIPage* page, HMIPage* ppage);
		void AppStart(int offx = 0, int offy = 0);

		//������һҳ��
		int GobackPage(int offx = 0, int offy = 0);
		//int GobackDialogPage(int offx = 0, int offy = 0);	
		int SwitchStartPage(int offx = 0, int offy = 0);
		//�л�ҳ�棬��ջ,���N��ҳ����ջ
		int SwitchPage(int page, int offx = 0, int offy = 0);

		int OpenKeyBoard(int page, HMIPage * p, int x, int y);

		//�򿪵�������
		int OpenDialogPage(int page, HMIPage* p = nullptr, int offx = 0, int offy = 0);
		//�л���������

		int SwitchDialogPage(int page, HMIPage* p = nullptr, int offx = 0, int offy = 0);
		//�رյ�������

		int ClosePage(int page);
		
		//�ر����д���
		int CloseAllDialog();
		HMIPage* FindPage(int page, bool iscontaintmp = true);

		/*****************************��Ϣ���*************************************/
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
		///hwnd ���͵�control
		///message ����
		///lparam ����
		int SendUserMessage(void* hwnd, int message, int lparam);
		int SendKey(int message);
		void Invoke(Message::InvokeCBFunc func, void*param, int winno = 0);

		/*********************�������*********************/
		//ԭʼ����.....��FLTKʵ��
		/************************************************/
		typedef Fl_Timeout_Handler TimeoutHandler;
		void AddTimeout(double time, TimeoutHandler handler, void* param);		
		void RemoveTimeout(TimeoutHandler handler, void* param);
		std::shared_ptr<BaseControl> GetCtrlById(Project::WinCtrlID ctrno);
		/***********************��̬����******************************/
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
		GHMIPage* gPage_ = nullptr;//ȫ��ҳ��,ҳ��0
		//HMIPage* bottomPage_;//bottom
		//HMIPage* topPage_;//bottom
		HMIPage* curPage_ = nullptr;//��ǰҳ��

		int SendUserMessageEx(void* hwnd, int message, int lparam);
		//static std::shared_ptr<BaseControl> NewCtr(Project::UnitInfo &unintInfo, UI::HMIPage*);
	};
}
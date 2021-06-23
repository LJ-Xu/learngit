#pragma once
#include <vector>
#include <thread>
#include "Message.h"
#include "Observer.h"
#include "ListQueue.h"
namespace DataService
{
	class IDataService;
}
namespace Project
{
	struct HMIProject;
	struct HMIWindow;
}
namespace UI
{
	class BaseGControl;
	class GlobalWindow;
	class HMIWindow;
	class UIManager : public Observer
	{
	public:
		static UIManager& Ins();
		UIManager();
		void Init(Project::HMIProject* prj) { prj_ = prj; }

		///********�������**********/

		//void CloseWindow(int winno);
		//
		//HMIWindow* OpenWindow(int winno);	

		//HMIWindow* OpenWindow(HMIWindow* win);


		///***************�ڵ�ǰ�����´��Ӵ���*****************/
		//HMIWindow* OpenSubWindow(int winno);
		// 
		//HMIWindow* JumpWindow(int fromwinno, int towinno);

		//HMIWindow* GetWin(int winNo);
		///*
		//HMIWindow* OpenKeyBoard(int winno);
		//HMIWindow* CloseKeyBoard(int winno);
		//*/
		//void HideWindow(int winno);
		//bool ExistWindow(int winno);	
		//
		//HMIWindow* GetWinByCtrID(int ctrid);

		//HMIWindow* OpenAlarmWin(int winno);
		//HMIWindow* CloseAlarmWin(int winno);

		int RunUI();
		bool IsInUIThread();
		int ShowWindow();

		//void Update(Project::DataVarId  id);
		///*******************����************************/

		//GlobalWindow* GlobalWin()
		//{
		//	return gwin_;
		//}

		//Project::HMIProject*  Project()
		//{
		//	return prj_;
		//}
		//static UIManager* Ins() { return ins_; }
		//static void GetHMIMessage(Message::Msg& msg);
		void Update(Project::DataVarId id);
		void AddExit(function<void(void)> func);
		void Exit();
		bool SendMsg(HWND hwnd, int msgcode, WPARAM wparam, LPARAM lparam, void* pdata=nullptr);
		HMIWindow* CurrWin()
		{
			return curWin_;
		}
	private:
		thread::id threadId_;
		HMIWindow*  curWin_ = nullptr;
		Project::HMIProject* prj_ = nullptr;
		Message::ListQueue queue_;
		//GlobalWindow* gwin_;
		//static UIManager*  ins_;
		//void DataNotifyServiceCB(int ctrid, int notifyData);//  ȫ�ֵ�����֪ͨ����ӿ�
		//std::vector<BaseGControl*> gCtrls_;//ȫ�ֿؼ����� ,WIN=0, CTRLID
		//std::vector<HMIWindow*> wins_; //��������д���
		//int FlMsgInterpCheck(void *event, void *data);
		vector<function<void(void)>> funcs_;
		void PrepareRunEnv();
	};
}


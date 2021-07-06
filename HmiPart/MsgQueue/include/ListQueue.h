#pragma once
#include <mutex>
#include "Message.h"
namespace Message
{
	//struct HMIMsg
	//{
	//	int    Code;   //消息 ID 号。
	//	int    Hwnd;   // 消息的窗口句柄
	//	int    WParam; //消息的参数1   
	//	int    LParam; //消息的参数2  
	//	void*  Data;
	//};
	struct ListQueueItem
	{
		struct Msg Msg;
		ListQueueItem * Next;
	};

	class ListQueue
	{
	public:
		ListQueue();
		~ListQueue();
	public:
		void Init(int cnt);
		bool SendMsg(HWND hwnd, int msgcode, WPARAM wparam, LPARAM lparam, void* pdata = nullptr);
		bool GetMsg(Msg& msg);
		//bool FreeMsg(Msg* msg);
	private:
		ListQueueItem* items_;
		ListQueueItem* NewItem();
		void TrimItem(ListQueueItem* item);
		ListQueueItem head_;
		ListQueueItem* tail_;
		ListQueueItem free_;
		int cnt_;
		int size_;
		std::mutex mtx_;
	};
}
#pragma once
#include <mutex>
#include "Message.h"
namespace Message
{
	//struct HMIMsg
	//{
	//	int    Code;   //��Ϣ ID �š�
	//	int    Hwnd;   // ��Ϣ�Ĵ��ھ��
	//	int    WParam; //��Ϣ�Ĳ���1   
	//	int    LParam; //��Ϣ�Ĳ���2  
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
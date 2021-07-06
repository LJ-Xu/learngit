#include "ListQueue.h"
namespace Message
{
	ListQueue::ListQueue()
	{

	}
	void ListQueue::Init(int cnt)
	{
		items_ = new ListQueueItem[cnt];
		free_.Next = items_;
		for (size_t i = 0; i < cnt; i++)
		{
			if (i + 1 < cnt)
				free_.Next[i].Next = &free_.Next[i + 1];
			else
				free_.Next[i].Next = nullptr;
		}
		size_ = cnt_ = cnt;
		tail_ = &head_;
		head_.Next = nullptr;
	}
	ListQueueItem* ListQueue::NewItem()
	{
		ListQueueItem* item;
		if (!free_.Next)
		{
			item = new ListQueueItem();
			++cnt_;
			return item;
		}
		item = free_.Next;
		free_.Next = item->Next;
		return item;
	}
	void ListQueue::TrimItem(ListQueueItem* item)
	{
		while (cnt_ > size_)
		{

		}

	}
	bool ListQueue::SendMsg(HWND hwnd, int msgcode, WPARAM wparam, LPARAM lparam, void* pdata)
	{
		ListQueueItem* tmp;//mtx_
		ListQueueItem* item;
		std::lock_guard<std::mutex> lck(mtx_);
		item = NewItem();
		if (!item)
		{
			return false;
		}
		item->Msg.Hwnd = hwnd;
		item->Msg.Code = msgcode;
		item->Msg.LParam = lparam;
		item->Msg.WParam = wparam;
		item->Msg.Data = 0;
		item->Next = nullptr;

		tail_->Next = item;
		tail_ = item;
		return true;

	}
	/*bool ListQueue::FreeMsg(Msg* msg)
	{
		std::lock_guard<std::mutex> lck(mtx_);
		ListQueueItem * lqt = free_.Next;
		((ListQueueItem *)msg)->Next = lqt;
		free_.Next =(ListQueueItem *) msg;
	}*/
	bool ListQueue::GetMsg(Msg& msg)
	{
		ListQueueItem* tmp;
		std::lock_guard<std::mutex> lck(mtx_);
		if (head_.Next)
		{
			tmp = head_.Next;
			head_.Next = tmp->Next;
			if (!head_.Next)
				tail_ = &head_;
			msg = tmp->Msg;
			//放入空闲列表
			tmp->Next = free_.Next;
			free_.Next = tmp;
			return true;
		}
		return false;
	}
	ListQueue::~ListQueue()
	{
		ListQueueItem* tmp = head_.Next;
		ListQueueItem* tp;
		while (tmp)
		{
			tp = tmp->Next;
			if (!tp)break;
			if ((unsigned int)tp >= (unsigned int)items_ &&
				(unsigned int)tp < (unsigned int)&items_[size_])
			{
				//不需要删除
			}
			else
			{
				delete tmp;
			}

			tmp = tp;
		}
		if (items_)
			delete[]items_;
	}
}

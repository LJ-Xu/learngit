#pragma once
#include "HistoryRecord.h"
#include "BaseStorage.h"
#include "XJRingBuffer.h"
namespace Storage
{
 
	class HistoryStorage :public BaseStorage
	{
	public:
		void Add(HistoryRecord& recd)
		{
			recd_.Add(recd);
		}
		XJRingBuffer<HistoryRecord>::Iterator Query(int mode, int param1, int param2)
		{

		}

	private:
		int UpdateCurse(int count)
		{
			recd_.UpdateReadPos(count);
		}
		int GetCount()
		{
			return recd_.GetCount();
		}
		char* operator[](int idx)
		{
			return (char*)recd_.Get(idx);
		}
		XJRingBuffer<HistoryRecord> recd_;
	};
}

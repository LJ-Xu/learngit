#pragma once
#include "XJRingBuffer.h"
namespace Storage
{
	enum ResultCode
	{
		SS_SUCCESS,
		SS_FAIL
	};
	struct StorageConfig
	{
		int  Capacity; //元素的个数
		int  ItemSize;//单个元素的大小
		int  SaveLevel;
		char Path[20];//保存路径
		char Media;//存储介质
		char Days;//保存的时间期限
	};
	
	class BaseStorage :public IObserver
	{
	public :
		void Init(StorageConfig& cnf);
		void Clear();
		void Save();
		void StorageNotify(int count);
		virtual void Load() = 0;//设置读指针
	protected:
		virtual int UpdateCurse(int count)=0;//设置读指针
		virtual int GetCount() = 0;//设置读指针
		virtual char* operator[](int idx)=0;
		StorageConfig cnf_;
	private:
		
	};
 
}

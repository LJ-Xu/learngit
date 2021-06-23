#pragma once
#include <vector>
struct DelegateInfo
{
	DelegateInfo(void(*f)(void*), void *p)
	{
		DelFunc = f;
		Param = p;
	}
	void(*DelFunc)(void*);
	void* Param;
};
class DeleteHelper
{
public:
	DeleteHelper();
	~DeleteHelper();
	template <class T>
	void AddDeleteMgr(T* ptr)
	{
		delDelegates_.push_back(DelegateInfo([](void*p) {delete (T*)p; }, ptr));
	}
private:
	std::vector<DelegateInfo> delDelegates_;
};


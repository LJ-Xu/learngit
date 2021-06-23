#pragma once
#include <string>
namespace Storage
{
	class Iterator
	{
	public:
		Iterator() {};
		virtual ~Iterator() {};
		virtual void* First() = 0;
		virtual void* Next() = 0;
		virtual void* GetCur() = 0;
		virtual bool IsEnd() = 0;
	};
}
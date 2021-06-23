#pragma once
#ifdef _MSC_VER
#include <WinSock2.h>
#else
typedef int SOCKET;
#endif
typedef int XJSocketCode;

class XJSocket
{
public:
	
	void* GetExData()
	{
		return exdata_;
	}
	void SetExData(void* exd)
	{
		exdata_ = exd;
	}
	XJSocket(SOCKET sock)
	{
		Sock = sock;
	}
	XJSocket()=default;
	SOCKET Sock;
private:
	void* exdata_;
};
#pragma once
#ifdef _MSC_VER
#include "XJSocket.h"
#include <string>
class XJSocketClientMulti;
class WinSocketClientMulti
{
public:

	WinSocketClientMulti() = default;

	friend DWORD WINAPI ClientMultiWorkThread(LPVOID IpParam);

	int Init(XJSocketClientMulti* cm);

	XJSocket* Connect(std::string addr,int port);

	int Send(XJSocket* sck, char* buf, int len);

	int DisConnect(XJSocket* sck);

	int DisConnect();

	int Start(int threads);

private:
	
	bool recvFlag_;
	//XJSocket socket_;
	XJSocketClientMulti* clientMulti_;
	char* recvBuf_;
	HANDLE iocp_;
	bool runFlag_;
};

#endif
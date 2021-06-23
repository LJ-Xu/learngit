#pragma once
#ifdef _MSC_VER
#include "XJSocket.h"
#include "XJSocketServer.h"
#include <string>
class XJSocketServer;
class WinSocketServer
{
public:
	WinSocketServer() = default;

	friend DWORD WINAPI ServerWorkThread(LPVOID IpParam);

	int Init(std::string addr, int port, XJSocketServer*);

	int Start(int =0);

	int Send(XJSocket* sck,char* buf , int len);

private:
	XJSocketServer* server_;
	XJSocket socket_;
	std::string addr_;
	int port_;
	HANDLE iocp_;
};
#endif
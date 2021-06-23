#pragma once
#include "XJSocket.h"
#include <string>
#include <thread>
#ifdef _MSC_VER
class WinSocketServer;
#else
class LinuxSocketServer;
#endif
class XJSocketServer
{
public:

	int Init(std::string addr, int port);

	int Start(int threads=0);

	int Send(XJSocket*,char* buf,int len);

	virtual void OnRecved(XJSocket*, char* buf, int len) {};

	virtual void OnConnected(XJSocket*) {};

	virtual void OnDisConnected(XJSocket*) {};

private:
#ifdef _MSC_VER
	WinSocketServer* server_;
#else
	LinuxSocketServer* server_;
#endif
	std::thread* td_;
};

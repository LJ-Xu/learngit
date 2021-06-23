#pragma once
#include "XJSocket.h"
#include <string>
#include <thread>
#ifdef _MSC_VER
class WinSocketClientMulti;
#else
class LinuxSocketClientMulti;
#endif
class XJSocketClientMulti
{
public:

	int Init();

	int Start(int threads=0);

	XJSocket* Connect(const char* addr,int port);

	int Send(XJSocket*,char* buf,int len);

	virtual void OnRecved(XJSocket*, char* buf, int len) {};

	virtual void OnConnected(XJSocket*, XJSocketCode code) {};

	virtual void OnDisConnected(XJSocket*, XJSocketCode code) {};

private:
#ifdef _MSC_VER
	WinSocketClientMulti* clientMulti_;
#else
	LinuxSocketClientMulti* clientMulti_;
#endif
	std::thread* td_;
};

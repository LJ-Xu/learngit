#pragma once
#include "XJSocket.h"
#include <string>
#ifdef _MSC_VER
class WinSocketClient;
#else
class LinuxSocketClient;
#endif
class XJSocketClient
{
public:

	int Init();

	int Connect(std::string addr,int port);

	int Send(const char* buf,int len);

	virtual void OnRecved(char* buf, int len) {};

	virtual void OnConnected(XJSocketCode code) {};
 
	virtual void OnDisConnected(XJSocketCode code) {};

private:
#ifdef _MSC_VER
	WinSocketClient* client_;
#else
	LinuxSocketClient* client_;
#endif
};


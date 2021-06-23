#pragma once
#ifndef _MSC_VER
#include "XJSocket.h"
#include <string>

class XJSocketServer;
class LinuxSocketServer
{
public:
	LinuxSocketServer() = default;

	int Init(std::string addr, int port, XJSocketServer* srv);

	int Start(int threads = 0);

	int Send(XJSocket* sck, char* buf, int len);


private:
	int CreateSocket();
	int UnBlock(int sfd);
	void EventLoop();
	void Close(int fd);

private:
	XJSocketServer* server_;
	XJSocket socket_;
	std::string addr_;
	int port_;
};
#endif

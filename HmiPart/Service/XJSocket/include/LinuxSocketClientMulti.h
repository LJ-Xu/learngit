#pragma once
#ifndef _MSC_VER
#include "XJSocket.h"
#include <string>
#include <map>
class XJSocketClientMulti;
class LinuxSocketClientMulti
{
public:

	LinuxSocketClientMulti() = default;

	int Init(XJSocketClientMulti* cm);

	int Start(int threads);

	XJSocket* Connect(std::string addr, int port);

	int Send(XJSocket* sck, char* buf, int len);

private:
	int UnBlock(int sfd);

	void EventLoop();

	void Close(int fd);

private:
	XJSocketClientMulti* clientMulti_;
	int epfd_;
};

#endif
#pragma once
#ifndef _MSC_VER
#include "XJSocket.h"
#include <string>
#include <thread>

class XJSocketClient;
class LinuxSocketClient
{
public:

	LinuxSocketClient() = default;

	int Init(XJSocketClient* client);

	int Connect(std::string addr, int port);

	int DisConnect();

	int Send(const char* buf, int len);

private:
	int UnBlock(int sfd);
	void Close();
	void ReceiveMessageThread(void* ptr);
private:
	XJSocketClient* client_;
	XJSocket socket_;

	std::shared_ptr<std::thread> thread_recv_;
	bool recvflag_ = false;
	char* recvbuf_;
};
#endif

#pragma once
#include "XJSocket.h"
#include <string>
#include <thread>
class XJSocketClient;
class WinSocketClient
{
public:

	WinSocketClient() = default;

	friend DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter);

	int Init(XJSocketClient* client);

	int Connect(std::string addr,int port);

	int Send(const char* buf,int len);

	int DisConnect();

	~WinSocketClient();

private:
	bool recvFlag_;
	char* recvBuf_ = nullptr;
	XJSocketClient* client_;
	XJSocket socket_;
	std::thread receiveThread_;
	
};


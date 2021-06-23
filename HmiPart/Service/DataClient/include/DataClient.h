#pragma once
#include "XJSocket.h"
#include "XJSocketClient.h"
#include <memory.h>
#include <thread>
#include <string>
#include <chrono>  
#include "ClientSession.h"
#define BUFFER_SIZE 4096u // 4 Kb
namespace HMI
{
	class DataClient :public XJSocketClient {
	public:
		void OnRecved(char* buf, int len);

		void OnConnected(XJSocketCode code);

		void OnDisConnected(XJSocketCode code);
	public:
		void SetCB(IDaAppCB* appcb)
		{
			appcb_ = appcb;
		}
		ClientSession * GetSession()
		{
			return session_;
		}
	private:
		IDaAppCB* appcb_;
		ClientSession * session_;
		/*std::vector<ClientSession*> clients_;
		int RemoveClient(netiolib::TcpConnectorPtr& ptr);
		int AddClient(netiolib::TcpConnectorPtr& ptr);
		ClientSession* GetClient(netiolib::TcpConnectorPtr& ptr);*/
	};

}


//class DataClient : IClient
//{
//public:
//	void Init(IDaAppCB *appcb);
//	int Connect();
//	DAManger* DAMgr() 
//	{
//		return damgr_;
//	}
//	int Send(char* buf, int len);
//	int Recv();
//private:	 
//	DAManger* damgr_;
//	ClientSocket* client_;
//	char buf_[BUFFER_SIZE];
//};


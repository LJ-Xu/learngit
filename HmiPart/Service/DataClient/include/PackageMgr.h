#pragma once
#include "ClientBaseVar.h"
#include "ClientSession.h"
#include "NolockFifo.h"
#include <memory>
#include <functional>
namespace HMI
{

#define PACKAGE_MINI_SIZE 11
#define PACKAGE_TAG_HEAD 0x55
#define PACKAGE_TAG_END  0xAA
#define IDX_TAG_HEAD 0
#define IDX_LEN      6
#define IDX_CMD      4
#define IDX_DATA     8
#define IDX_CRC(totallen)  ((totallen)-3)
#define SOCKET_FIFO_BUFFER_SIZE (1024*64)


#define CMD_READ_ASYNC 0x01
#define CMD_READ_SYNC 0x02
#define CMD_WRITE_SYNC 0x10
#define CMD_WRITE_ASYNC 0x11

	class ClientSession;
	class PackageMgr
	{
	public:
		//PackageMgr(Socket* sock);
		PackageMgr(ClientSession* session);
		~PackageMgr();
		//PackageMgr(PackageMgr&) {};
		/*void PostSend(RespVar* resp);*/
		/*void SetSendFunc(std::function<int(char*, int)> func)
		{
			sendFunc_ = func;
		}*/
		void Recv(char* data, int len);
		int Send(const char* buf, int len)
		{
			/*if (!sendFunc_)return 0;
			return sendFunc_(buf, len);*/
			int ret = session_->Send(buf, len);
			return len;
		}
		/*void Run();*/
		void Stop()
		{
			parseflag_ = false;
		}
		void ReadByVar(BaseVar* var, int id);

		void ReadByFrame(BaseVar* var, int fid1, int fid2);

		//void Read(BaseVar* var, int type, int id, int eid);

		void WriteByVar(BaseVar* var, const  char* data, int len);
		void TriggerMacro(const char* funcname);
		void MoveData(const char* cmd);

		ClientSession* GetClientSession()
		{
			return session_;
		}
		void HandleRecipeRecord(const char* recipeinfo, Project::RecipeAction action);
		void ExportCSVdata(const char* csvinfo);
		void ImportCSVdata(const char* csvinfo);
		void EnableLazySend(bool flag);//需要线程保护
		int TriggerSend();
	private:
		int Parse();
		void ProcessPackage(const char* buf, int len);
		void SendPackage(int cmd, const  char* data, int len);
		void SendPackage(int cmd, int type, int vid, const  char*buf, int len);
		void SendPackage(int cmd, int type, int vid, int eid, const  char*buf, int len);
		void SendEx(char* data, int len);
		char* GetBufEx(int len);
		//int TrrigerSend();
		std::unique_ptr<char> buf_;
		int bufSize_;
		int widx_;
		bool lazy_send_;
		/*
		int recLen_;
		int ridx_;
		int widx_;*/
		ClientSession* session_;
		NolockFifo fifo_;
		bool parseflag_;
		/*Socket* sock_;*/
		std::function<int(const char*, int)> sendFunc_;
	};
}


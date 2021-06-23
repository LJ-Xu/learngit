#pragma once
#include "ClientBaseVar.h"
//#include "LocalDataManger.h"
#include  "IDaAppCB.h"
namespace HMI
{
	class PackageMgr;
	class DataClient;
	class ClientSession
	{
	public:
		ClientSession(DataClient* client);
		~ClientSession();


	public:
		IDaAppCB* AppCB;
		void Recv(char* buf, int len);
		int  Send(const char* buf, int len);
		/*netiolib::TcpConnectorPtr& GetTcpCon()
		{
			return csock_;
		}
		netiolib::TcpSocketPtr& GetTcpSck()
		{
			return sock_;
		}*/
	public:
		void TriggerMacro(const char* funcname);
		void MoveDatas(const char* cmd);
		void HandleRecipeRecord(const char* recipeinfo, Project::RecipeAction action);
		void ExportCSVdata(const char* csvinfo);
		void ImportCSVdata(const char* csvinfo);

		void MoveByVar(BaseVar* svar, BaseVar* dvar,int regcount);
		void ReadByVar(BaseVar* var, int id);
		void ReadByFrame(BaseVar* var, int fid1, int fid2);
		void Read(BaseVar* var, int type, int fid1, int fid2);
		void WriteByVar(BaseVar* var, char* data, int len);

		void FlushSend();
		
 
	private:
		DataClient* client_;
		PackageMgr* packageMgr_;

	};
}




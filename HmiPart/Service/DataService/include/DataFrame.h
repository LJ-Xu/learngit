#pragma once
#include "ProjectPortVarsInfo.h"
#include "ClientBaseVar.h"
namespace UI
{
	struct DataRequest
	{
		char type;
		int id;
		int eid;
		Project::BaseVar* VarPtr;
	};
	struct WinRequestInfo
	{
		WinRequestInfo(int winno, Project::WindowVar* var)
		{
			Reset(winno, var);
		}
		void Reset(int winno, Project::WindowVar* var)
		{
			WinNo = winno;
			FIdx = 0;
			NIdx = 0;
			WinVar = var;
		}
		int WinNo;
		int FIdx;//Frame的索引
		int NIdx; //VID的索引
		Project::WindowVar* WinVar;
	};
	class DataFrame
	{
	public:
		void Init(Project::ProjectPortVarsInfo* vars)
		{
			vars_ = vars;
		}
		int Get(DataRequest& req);
		void AddWin(int winno);
		void RemoveWin(int winno);
	private:
		std::mutex mtx_;
		Project::ProjectPortVarsInfo* vars_ = nullptr;
		std::vector<WinRequestInfo> winReqs_;
		int winReqIdx_ = 0;
		WinRequestInfo* GetNextWinReqInfo();
		int GetNextBaseVar(WinRequestInfo* info, DataRequest& req);
		void SetWindowVarFlag(WindowVar& wvar, char flag);
	};
}



#pragma once
#include "ProjectPortVarsInfo.h"
#include "ClientBaseVar.h"
#include <list>
#include <vector>
#include <memory>
namespace UI
{
	struct DataRequest
	{
		char type;
		int id;
		int eid;
		Project::BaseVar* VarPtr;
	};
	struct ReqVarItem
	{
		ReqVarItem() {}
		ReqVarItem(int fid, int vidx) :FrameIdx(fid), Vidx(vidx) {}
		int FrameIdx;
		unsigned int Vidx;
	};
	struct DevRequestGroup
	{
		std::mutex Lock;
		std::list<ReqVarItem*> ReqVarList;
		~DevRequestGroup()
		{
			for (list<ReqVarItem*>::iterator it = ReqVarList.begin(); it != ReqVarList.end();)
			{
				ReqVarItem* ptr = *it;
				ReqVarList.erase(it++); // 删除节点，并到下一个节点
				delete ptr;
			}
		}
	};
	struct WinRequestInfo
	{
		WinRequestInfo(int winno, Project::WindowVar* var)
		{
			Reset(winno, var);
		}
		void Init(WindowVar* wvar);
		void Reset(int winno, Project::WindowVar* var)
		{
			WinNo = winno;
			DIdx = 0;
			WinVar = var;
		}
		int WinNo;
		int DIdx;//Devid的索引
		Project::WindowVar* WinVar;
		std::vector<DevRequestGroup*> DevReqHlp;
		~WinRequestInfo()
		{
			for (size_t i = 0; i < DevReqHlp.size(); i++)
			{
				delete DevReqHlp[i];
			}
		}
		
	};
	class DataFrame
	{
	public:
		void Init(Project::ProjectPortVarsInfo* vars)
		{
			vars_ = vars;
		}
		/*int EachGet(std::function<void(DataRequest& req)>);*/
		int Get(DataRequest& req);
		int Req(DataRequest& req);
		void AddWin(int winno);
		void RemoveWin(int winno);
	public:
		~DataFrame()
		{
			if (!winReqs_.empty())
			{
				for (list<WinRequestInfo*>::iterator it = winReqs_.begin(); it != winReqs_.end();)
				{
					WinRequestInfo* ptr = *it;
					winReqs_.erase(it++); // 删除节点，并到下一个节点
					delete ptr;
				}
				
			}
			 
		}
	private:
		std::mutex mtx_;
		Project::ProjectPortVarsInfo* vars_ = nullptr;
		std::list<WinRequestInfo*> winReqs_;
		int winReqIdx_ = 0;
		WinRequestInfo* GetNextWinReqInfoWithNoLock();
		int GetNextBaseVarWithNoLock(WinRequestInfo* info, DataRequest& req);
		//void SetWindowVarFlag(WindowVar& wvar, char flag);
	};
}



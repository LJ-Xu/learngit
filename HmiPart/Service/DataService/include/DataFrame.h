#pragma once
#include "HMIProject.h"
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
	struct OneDevReqInfo
	{
		int FSIdx;  //
		int FLen;
		int SSIdx;
		int SLen;
		int Idx;//请求索引
		OneDevReqInfo():FSIdx(0), FLen(0), SSIdx(0), SLen(0), Idx(0)
		{
		}

	};
	struct WinsDevSumReqInfo
	{
		int Count = 0;//该DEVID下请求个数
		int Idx = 0;
	};
	struct WinRequestInfo
	{

		WinRequestInfo(int winno, Project::WindowVar* var);
		~WinRequestInfo();

		void Init(WindowVar* wvar);
		void Reset(int winno, Project::WindowVar* var);

		int WinNo;
		Project::WindowVar* WinVar;
		std::vector<OneDevReqInfo*> DevReqHlp;//设备编号索引
	};
	
	class DataFrame
	{
	public:
		void Init(Project::ProjectPortVarsInfo* vars)
		{
			vars_ = vars;
			DevDIdx_.resize(HMIProject::Ins->Devices.Devs.size(), nullptr);
			for (size_t i = 0; i < DevDIdx_.size(); i++)
			{
				if (!HMIProject::Ins->Devices.Devs[i].IsEmpty())
				{
					DevDIdx_[i] = new WinsDevSumReqInfo();
				}
			}
		}
		void AddWin(int winno);
		void RemoveWin(int winno);
		int MaxDevID();
		//获取当前页面
		int CurrPage();
		//获取下个页面
		int NextPage();
		//获取指定请求
		int GetDR(short devid, DataRequest* dr, int count);

		int NextDR(short devid,int = 1);

		WinsDevSumReqInfo GetSumReqInfo(short devid);
		bool IsCurEnd(short devid);
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
		std::vector<WinsDevSumReqInfo*> DevDIdx_;//设备ID对应编号索引
		int winReqIdx_ = 0;
	private:		
		//WinRequestInfo* GetWinReqInfoByIdx(int idx);
		void IncWinsDevSumReqCount(WinRequestInfo* winreq);
		void DecWinsDevSumReqCount(WinRequestInfo* winreq);
	};
}



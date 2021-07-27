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
				ReqVarList.erase(it++); // ɾ���ڵ㣬������һ���ڵ�
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
		int Idx;//��������
		OneDevReqInfo():FSIdx(0), FLen(0), SSIdx(0), SLen(0), Idx(0)
		{
		}

	};
	struct WinsDevSumReqInfo
	{
		int Count = 0;//��DEVID���������
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
		std::vector<OneDevReqInfo*> DevReqHlp;//�豸�������
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
		//��ȡ��ǰҳ��
		int CurrPage();
		//��ȡ�¸�ҳ��
		int NextPage();
		//��ȡָ������
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
					winReqs_.erase(it++); // ɾ���ڵ㣬������һ���ڵ�
					delete ptr;
				}
				
			}
			 
		}
	private:
		std::mutex mtx_;
		Project::ProjectPortVarsInfo* vars_ = nullptr;
		std::list<WinRequestInfo*> winReqs_;
		std::vector<WinsDevSumReqInfo*> DevDIdx_;//�豸ID��Ӧ�������
		int winReqIdx_ = 0;
	private:		
		//WinRequestInfo* GetWinReqInfoByIdx(int idx);
		void IncWinsDevSumReqCount(WinRequestInfo* winreq);
		void DecWinsDevSumReqCount(WinRequestInfo* winreq);
	};
}



#pragma once
#include "stdafx.h"
#include "ProjectPortVarsInfo.h"
#include "HMIProject.h"
#include "Observer.h"
namespace UI
{
	class LocalSysDataCache;
	struct WinData;
	class DataCache
	{
	public :
 
		void Init(Project::ProjectPortVarsInfo* vars);

		int Cache(Project::DataVarId vid, DWORD val);

		int Cache(Project::DataVarId vid, char* buf,int len);

		int Get(Project::DataVarId vid, DWORD& val);

		int Get(Project::DataVarId vid, char* buf, int len);
		//取可变数据,当count=0时，offset为数据起始获取位,一直取到结束为止
		//int Get(Project::DataVarId vid, int& len, char* buf=nullptr);

		int Get(Project::BaseVar& var, char* buf, int len);

		void AddWin(int winno);

		void RemoveWin(int winno);

		void TD_SetFrameData(int wid, int idx, char* data, int len);

		void TD_SetData(int vid, char* data, int len);
 
		void Attatch(Observer * p);
	private:
		void InitWinData(WinData* wd);
		std::mutex mtx_;
		Observer* obsrv_ = nullptr;
		Project::ProjectPortVarsInfo* vars_ = nullptr;
		WinData* GetWinDataByVid(int vid);
		WinData* GetWinDataByWinNo(int winno);
		vector<unique_ptr<WinData>> winDatas_;//窗体数据缓存

	};
	struct WinData
	{
		int WinNo;
		Project::WindowVar* WinVar;
		//数据写和读保护 读出的数据
		std::unique_ptr<char[]> Buf;// 大于4字节的
		bool IsOffsetOk(unsigned int data)
		{
			return data < WinVar->BufSize;
		}
	};
}